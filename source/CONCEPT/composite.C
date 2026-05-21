// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: composite.C,v 1.43 2005/12/23 17:02:33 amoll Exp $
//

#include <BALL/CONCEPT/composite.h>
#include <BALL/CONCEPT/persistenceManager.h>
#include <BALL/DATATYPE/listSort.mac>
#include <BALL/KERNEL/predicate.h>

#include <BALL/KERNEL/atom.h>

// v2.1 P1.3 (D31b): private internal header for CompositeHandle +
// CompositeTopologyView types. composite.C is one of the 4 named
// wiring TUs allowed to include this. Public headers (composite.h,
// composite.iC, anything #include-able by user code) MUST NOT
// include this — the CI grep gate enforces.
#include <BALL/KERNEL/_moleculeStoreInternal.h>
#include <BALL/KERNEL/moleculeStore.h>   // v2.2 H2a: container write accessors

#include <cstring>       // std::memcpy for handle (un)packing
#include <type_traits>   // R19 P19-1: is_trivially_copyable static_assert

using namespace std;

namespace BALL
{

	UnaryPredicate<Composite> Composite::DEFAULT_UNARY_PREDICATE;
	UnaryProcessor<Composite> Composite::DEFAULT_PROCESSOR;

	// v2.1 P1.3: opaque handle (un)packing. composite.h stores the
	// handle as std::uint64_t composite_handle_packed_ to keep the
	// public header free of the internal types. composite.C is allowed
	// to memcpy between the opaque slot and the typed struct because
	// CompositeHandle is POD and the static_assert in
	// _moleculeStoreInternal.h pins sizeof == 8.
	static_assert(sizeof(CompositeHandle) == sizeof(std::uint64_t),
		"CompositeHandle layout must match opaque uint64_t slot");
	// R19 P19-1 hardening: memcpy between CompositeHandle and the
	// opaque uint64_t slot is only safe if CompositeHandle is
	// trivially-copyable AND has at-most-uint64_t alignment. Both
	// conditions hold today (POD aggregate; static_assert below
	// catches a future regression that adds a non-trivial member
	// or stronger alignment requirement).
	static_assert(std::is_trivially_copyable<CompositeHandle>::value,
		"CompositeHandle must remain trivially-copyable for memcpy round-trip");
	static_assert(alignof(CompositeHandle) <= alignof(std::uint64_t),
		"CompositeHandle alignment must not exceed uint64_t alignment");

	CompositeHandle Composite::getCompositeHandle_() const
	{
		CompositeHandle h;
		std::memcpy(&h, &composite_handle_packed_, sizeof h);
		return h;
	}

	void Composite::setCompositeHandle_(CompositeHandle h)
	{
		std::memcpy(&composite_handle_packed_, &h, sizeof h);
	}

	// v2.1 P1.3 + D31b: return Composite topology by-value as
	// Composite*. Iterator code can consume this without ever naming
	// CompositeNode — the side-table type stays encapsulated.
	//
	// Note: P1.3 reads the v2.0 inline pointers directly. Reading
	// from composite_nodes_ instead is a P2 concern (the inheritance-
	// flip phase) — until then the side table is not yet populated
	// because P1.3 ships the *infrastructure* only, not the mutation
	// mirror. See the P1 plan's P1.3 row and the maintainer decision
	// dated 2026-05-19 in V21-CODEX-REVIEW-ROUND17C.md.
	CompositeTopologyView Composite::getNode_() const
	{
		CompositeTopologyView v;
		v.parent       = parent_;
		v.first_child  = first_child_;
		v.last_child   = last_child_;
		v.next_sibling = next_;
		v.prev_sibling = previous_;
		v.child_count  = static_cast<std::uint32_t>(number_of_children_);
		return v;
	}

	// v2.1 P3.2 (D41.1 + R22): centralised dynamic_cast<Atom*>
	// helper bodies. Declarations in include/BALL/CONCEPT/composite.h
	// in namespace BALL::detail. CORE_ONLY-module callers route their
	// Atom-RTTI through these (CI grep gate enforces at P3 close).
	namespace detail
	{
		Atom* compositeAsAtom_(Composite* c) noexcept
		{
			return c ? dynamic_cast<Atom*>(c) : nullptr;
		}
		const Atom* compositeAsAtom_(const Composite* c) noexcept
		{
			return c ? dynamic_cast<const Atom*>(c) : nullptr;
		}
	}

	// v2.1 P2.1.0 + P2.1.1: default store-reach hook walks the parent
	// chain to find a Composite that overrides this method (System or
	// Atom). Returns the first non-null store; nullptr if no ancestor
	// provides one (free-standing tree).
	//
	// Atom overrides with a constant-time return of `store_` — saves
	// the walk for Atom-rooted mutations. System overrides with
	// `store_.get()` so any Composite inside the System reaches it
	// via at most one walk. Molecule / Chain / Residue inherit this
	// default; they'll walk up to System.
	//
	// Per D36 / R20b-2: no dynamic_cast / RTTI — purely virtual
	// dispatch + parent-chain walk.
	MoleculeStore* Composite::getCompositeStore_()
	{
		Composite* p = parent_;
		while (p)
		{
			// Ask the parent — its override may return non-null even
			// if its own parent_ is nullptr (e.g. root System).
			MoleculeStore* s = p->getCompositeStore_();
			if (s) return s;
			p = p->parent_;
		}
		return nullptr;
	}

	// v2.1 P2.1.1: default kind tag is NONE. Atom overrides to ATOM
	// (atom.C). Other subclasses may override later; v2.1 has no
	// read paths through the side-table topology so this stays
	// informational.
	std::uint8_t Composite::compositeKindForSideTable_() const
	{
		return static_cast<std::uint8_t>(CompositeKind::NONE);
	}

	// v2.1 P2.1.1 (D36): mirror this Composite's v0 inline state into
	// its side-table CompositeNode. Lazy-allocates the handle on
	// first touch. No-op when no store is reachable.
	//
	// Parent / first_child / last_child / next / prev links are
	// mirrored from v0 pointers — if a neighbor doesn't yet have a
	// handle, a fresh one is allocated for it. This walks at most one
	// level outward; deep recursion is avoided.
	//
	// Reads in v2.1 still come from v0 inline (D36). The side table
	// is a parallel mirror, verified by parity tests; nothing in
	// production v2.1 code reads from it. v2.2 flips reads to side
	// tables and then this mirror is the source of truth.
	void Composite::mirrorToSideTable_()
	{
		MoleculeStore* store = getCompositeStore_();
		if (!store) return;  // free-standing; P2.4 backfill at adoption.

		MoleculeStoreSideTables& s = store->sideTables_();

		// Allocate own handle if missing.
		CompositeHandle h = getCompositeHandle_();
		if (h.isNull())
		{
			CompositeKind kind = static_cast<CompositeKind>(compositeKindForSideTable_());
			h = s.allocate_composite_node_(kind);
			setCompositeHandle_(h);
		}

		// Resolve a neighbor's handle, but ONLY return what the
		// neighbor already has — do NOT allocate. Reasons:
		//   1. Allocating for a neighbor in this store assumes the
		//      neighbor "belongs to" this store. That breaks the
		//      free-standing-mol case (where mol has no store but
		//      the atom child has an orphan store): if we allocated
		//      mol's handle in the orphan store from the atom's
		//      mirror call, mol's later sys.adopt would have to
		//      migrate that orphan-store handle. Better: leave the
		//      neighbor's handle null; the neighbor's own mirror
		//      call (when it has a reachable store) will allocate it.
		//   2. Allocating across stores (atom-store vs parent-system-
		//      store mismatch in pre-adopt state) would put the
		//      neighbor's handle in the wrong store entirely.
		//
		// Consequence: side-table topology links lag by one mirror
		// call. Caller (Composite::appendChild etc.) MUST mirror
		// both parties involved in the mutation so each picks up
		// the other's handle. Mirror order matters: child first
		// (allocates its handle), then parent (links to child).
		auto handle_of = [](Composite* c) -> CompositeHandle {
			if (!c) return CompositeHandle{};
			return c->getCompositeHandle_();
		};

		// Resolve all 5 neighbor handles BEFORE taking a reference to
		// our own node. Each handle_of() may allocate (push_back to
		// composite_nodes_), which can reallocate the vector and
		// invalidate references — including the one to `n` we're about
		// to write into. Collect first, write last.
		CompositeHandle p_h    = handle_of(parent_);
		CompositeHandle fc_h   = handle_of(first_child_);
		CompositeHandle lc_h   = handle_of(last_child_);
		CompositeHandle next_h = handle_of(next_);
		CompositeHandle prev_h = handle_of(previous_);

		// All allocations complete; vector is stable for the
		// remainder of this call. Now safe to grab a reference and
		// write fields.
		CompositeNode& n = s.node_(h);
		n.parent       = p_h;
		n.first_child  = fc_h;
		n.last_child   = lc_h;
		n.next_sibling = next_h;
		n.prev_sibling = prev_h;
		n.child_count  = static_cast<std::uint32_t>(number_of_children_);
	}

	// v2.2 H2a (D69/D70): mirror a child detach into the container table.
	void Composite::mirrorRemoveChild_(Composite& child)
	{
		// FORWARD-ONLY: never mirror while THIS (the parent) is being
		// destroyed (the P2.1.1 cascade trap). The dying parent's rows are
		// freed wholesale at ~System; v0 remains the source of truth.
		if (being_destroyed_) return;

		// Resolve this parent's container row in its bound store. No row =>
		// free-standing / not yet materialised => nothing to mirror (the
		// table is built at adoption per D73).
		MoleculeStore* store = getContainerRowStore_();
		std::uint32_t  parent_row = getContainerRow_();
		if (store == 0 || parent_row == 0) return;

		// Child is an atom (leaf, addressed by store index) or a container
		// (addressed by its own row, only if it lives in the SAME store).
		if (Atom* a = detail::compositeAsAtom_(&child))
		{
			store->container_remove_atom_(parent_row, a->getStoreIndex());
		}
		else if (child.getContainerRowStore_() == store)
		{
			std::uint32_t child_row = child.getContainerRow_();
			if (child_row != 0)
			{
				store->container_remove_container_(parent_row, child_row);
			}
		}
	}

	// v2.2 H2b: append `child`'s edge to THIS container's row (O(1); the
	// append case). FORWARD-ONLY; no-op when being destroyed or unrooted.
	// append_child is itself idempotent against re-appending the last child.
	void Composite::mirrorAppendChild_(Composite& child)
	{
		if (being_destroyed_) return;
		MoleculeStore* store = getContainerRowStore_();
		std::uint32_t  row   = getContainerRow_();
		if (store == 0 || row == 0) return;
		if (Atom* a = detail::compositeAsAtom_(&child))
		{
			store->container_append_atom_(row, a->getStoreIndex());
		}
		else if (child.getContainerRowStore_() == store
		         && child.getContainerRow_() != 0)
		{
			store->container_append_container_(row, child.getContainerRow_());
		}
	}

	// v2.2 H2b: re-derive THIS container's row child list from the CURRENT
	// v0 child order (O(degree)). The positional / splice / swap / replace /
	// insertParent / live-clear path. FORWARD-ONLY; no-op when being
	// destroyed or unrooted. Children that are not (yet) in this store's
	// table are skipped (materialised later at adoption).
	void Composite::mirrorRederiveOwnRow_()
	{
		if (being_destroyed_) return;
		MoleculeStore* store = getContainerRowStore_();
		std::uint32_t  row   = getContainerRow_();
		if (store == 0 || row == 0) return;
		store->container_clear_children_(row);
		for (Size i = 0; i < getDegree(); ++i)
		{
			Composite* child = getChild(static_cast<Index>(i));
			if (child == 0) continue;
			if (Atom* a = detail::compositeAsAtom_(child))
			{
				store->container_append_atom_(row, a->getStoreIndex());
			}
			else if (child->getContainerRowStore_() == store
			         && child->getContainerRow_() != 0)
			{
				store->container_append_container_(row, child->getContainerRow_());
			}
		}
	}

	// default ctor
	Composite::Composite()
		:	PersistentObject(),
			Selectable(),
			number_of_children_(0),
			parent_(0),
			previous_(0),
			next_(0),
			first_child_(0),
			last_child_(0),
			properties_(0),
			contains_selection_(false),
			number_of_selected_children_(0),
			number_of_children_containing_selection_(0),
			selection_stamp_(),
			modification_stamp_()
	{ }

	// copy ctor
	Composite::Composite(const Composite& composite, bool deep)
		:	PersistentObject(composite),
			Selectable(composite),
			number_of_children_(0),
			parent_(0),
			previous_(0),
			next_(0),
			first_child_(0),
			last_child_(0),
			properties_(0),
			contains_selection_(false),
			number_of_selected_children_(0),
			number_of_children_containing_selection_(0),
			selection_stamp_(),
			modification_stamp_()
	{
		if (deep)
		{
			composite.clone(*this);
		}
		properties_ = composite.properties_;
	}

	// destructor
	Composite::~Composite()
	{
		// v2.2 H2a (D69) backstop: derived molecular destructors set this
		// earlier (before their destroy()); set it here too for any direct
		// Composite. The detach below (and the cascade in destroyChildren_)
		// then skips the container-table mirror for this dying parent.
		setBeingDestroyed_();
		if (parent_ != 0)
		{
			parent_->removeChild(*this);
		}

		destroyChildren_();
	}

	void Composite::set(const Composite& composite, bool deep)
	{
		if (deep)
		{
			// predicative cloning
			composite.clone(*this);
		}
		else
		{
			// clear old contents
			destroy();

			// copy all remaining attributes
			properties_ = composite.properties_;
			selected_ = composite.selected_;
			contains_selection_ = selected_;

			// update modification and selection time stamp
			stamp(BOTH);
		}
	}

  void Composite::persistentWrite(PersistenceManager& pm, const char* name) const
  {
		pm.writeObjectHeader(this, name);
			pm.writeStorableObject(dynamic_cast<const Selectable&>(*this), "Selectable");

			pm.writePrimitive(number_of_children_, "number_of_children_");
			pm.writeObjectPointer(parent_, "parent_");
			pm.writeObjectPointer(previous_, "previous_");
			pm.writeObjectPointer(next_, "next_");
			pm.writeObjectPointer(first_child_, "first_child_");
			pm.writeObjectPointer(last_child_, "last_child_");
			pm.writePrimitive(properties_, "properties_");
			pm.writePrimitive(contains_selection_, "contains_selection_");
			pm.writePrimitive(number_of_selected_children_, "number_of_selected_children_");
			pm.writePrimitive(number_of_children_containing_selection_, "number_of_children_containing_selection_");
			pm.writeStorableObject(selection_stamp_, "selection_stamp_");
			pm.writeStorableObject(modification_stamp_, "modification_stamp_");
		pm.writeObjectTrailer(name);
	}

  void Composite::persistentRead(PersistenceManager& pm)
  {
		pm.readStorableObject(dynamic_cast<Selectable&>(*this), "Selectable");

		pm.readPrimitive(number_of_children_, "number_of_children_");
		pm.readObjectPointer(parent_, "parent_");
		pm.readObjectPointer(previous_, "previous_");
		pm.readObjectPointer(next_, "next_");
		pm.readObjectPointer(first_child_, "first_child_");
		pm.readObjectPointer(last_child_, "last_child_");
		pm.readPrimitive(properties_, "properties_");
		pm.readPrimitive(contains_selection_, "contains_selection_");
		pm.readPrimitive(number_of_selected_children_, "number_of_selected_children_");
		pm.readPrimitive(number_of_children_containing_selection_, "number_of_children_containing_selection_");
		pm.readStorableObject(selection_stamp_, "selection_stamp_");
		pm.readStorableObject(modification_stamp_, "modification_stamp_");
	}
 
	Size Composite::getPathLength(const Composite& composite) const
	{
		// if composite equals *this - return 0
		if (&composite == this)
		{
			return 0;
		}

		// first, try to walk upwards from composite
		Size path_size = 1;
		Composite* composite_ptr = composite.parent_;
		for (; composite_ptr != 0; composite_ptr = composite_ptr->parent_, ++path_size)
		{
			if (composite_ptr == this)
			{	
				// if we found a path from composite to *this - return its length
				return path_size;
			}
		}
		
		// if this didn't help, try to walk upwards from *this
		for (composite_ptr = parent_, path_size = 1; composite_ptr != 0;
				 composite_ptr = composite_ptr->parent_,  ++path_size)
		{
			if (composite_ptr == &composite)
			{
				// if we found a path to composite - return
				return path_size;
			}
		}
		
		// no path from *this to composite exists
		return INVALID_Size;
	}

	Size Composite::getDepth() const
	{
		// walk up to the root (if possible)
		Size size = 0;
		for (Composite* composite_ptr = parent_;
				 composite_ptr != 0; composite_ptr = composite_ptr->parent_)
		{
			++size;
		}

		return size;
	}

	Composite& Composite::getRoot()
	{
		// walk up to the root
		Composite* composite_ptr = this;
		for (; composite_ptr->parent_ != 0; composite_ptr = composite_ptr->parent_) {};

		return *composite_ptr;
	}

	Composite* Composite::getChild(Index index)
	{
		if (first_child_ != 0)
		{
			if (index == 0)
			{
				return first_child_;
			}
			else
			{
				return first_child_->getSibling(index);
			}
		}
		else
		{
			return 0;
		}
	} 

	Composite* Composite::getLowestCommonAncestor(const Composite& composite)
	{
		// determine depth of node A
		const Size size_a = getDepth() + 1;
		Index index_a = 0;
		
		Composite* composite_ptr = 0;

		// create an array and store all nodes on the path to the root	
		Composite** composites_a = new Composite* [size_a];
		for (composite_ptr = this; composite_ptr != 0; composite_ptr = composite_ptr->parent_)
		{
			composites_a[index_a++] = composite_ptr;
		}

		// determine the depth of B
		const Size size_b = composite.getDepth() + 1;
		Index index_b = 0;

		// and store all ancestor nodes of B in an array
		Composite** composites_b = new Composite *[size_b];
		for (composite_ptr = const_cast<Composite*>(&composite); composite_ptr != 0;composite_ptr = composite_ptr->parent_)
		{
			composites_b[index_b++] = composite_ptr;
		}

		// compare all nodes from A and B 
		// if A and B have any common ancestor, the last entry in each array is the root
		// from there we walk backwards in both arrays
		for (composite_ptr = 0, index_a = (Index)(size_a - 1), index_b = (Index)(size_b - 1);
				 index_a >= 0 && index_b >= 0; --index_a, --index_b)
		{
			// as long as both array entries are equal, decrase the indices
			if (composites_a[index_a] == composites_b[index_b])
			{
				composite_ptr = composites_a[index_a];
			} 
			else 
			{
				// if the entries differ, the last equal entry was the lowest
				// common ancestor - exit the loop
				break;
			}
		}

		delete [] composites_a;
		delete [] composites_b;

		return composite_ptr;
	} 

	Composite* Composite::getSibling(Index index)
	{
		Composite* composite_ptr = this;

		if (index < 0)
		{
			// walk "index" steps along the previous_ pointers
			composite_ptr = composite_ptr->previous_;

			for (; ++index < 0 && composite_ptr != 0; 
					 composite_ptr = composite_ptr->previous_) {};
		} 
		else 
		{
			if (index > 0) 
			{
				// walk "index" steps along the next_ pointers
				composite_ptr = composite_ptr->next_;

				for (; --index > 0 && composite_ptr != 0; 
						 composite_ptr = composite_ptr->next_) {};
			}
		}
		return composite_ptr;
	}

	void* Composite::clone(Composite& root) const
	{
		// avoid self-cloning
		if (&root == this)	
		{
			return 0;
		}

		// remove old contents 
		// (update the modification and selection stamp as well)
		root.destroy();
		
		// copy the properties
		root.properties_ = properties_;

		// clone everything else
		clone_(*const_cast<Composite*>(this), root);

		// update the selection of the parent (if it exists)
		if (root.parent_ != 0)
		{
			if (root.containsSelection())
			{
				root.parent_->number_of_children_containing_selection_++;

				if (root.selected_)
				{
					root.parent_->number_of_selected_children_++;
				}

				root.updateSelection_();
			}
		}

		return &root;
	}

	void Composite::stamp(Composite::StampType stamp_type)
	{
		if ((stamp_type & MODIFICATION) != 0)
		{
			modification_stamp_.stamp();
		}
		if ((stamp_type & SELECTION) != 0)
		{
			selection_stamp_.stamp();
		}

		// propagate the time stamp upwards to the root
		if (parent_ != 0)
		{
			parent_->stamp(stamp_type);
		}
	}

	void Composite::select()
	{
		select_(true);
	}

	void Composite::select_(bool update_parent)
	{
		if (!selected_)
		{
			// select all children
			Composite* child = first_child_;

			for (; child != 0; child = child->next_)
			{
				if (!child->selected_)
				{
					// select the child only if it is not already selected
					child->select_(false);
				}
			}
			
			// all children are selected, so this node is selected, too
			number_of_selected_children_ = number_of_children_;
			number_of_children_containing_selection_ = number_of_children_;
			selected_ = true;

			// update the time stamp
			selection_stamp_.stamp();

			// check whether we have to inform the parent
			if (update_parent && (parent_ != 0))
			{
				// we were not selected beforehand, so increase
				// the corresponding counters
				parent_->number_of_selected_children_++;
				if (!contains_selection_)
				{
					// increase the corresponding counter in the parent
					parent_->number_of_children_containing_selection_++;
				}
				contains_selection_ = true;

				// updateSelection updates the time stamp and propagates it upwards...
				parent_->updateSelection_();
			} 
			else 
			{
				contains_selection_ = true;
			}
		}
	}
	
	void Composite::deselect()
	{
		deselect_(true);
	}

	void Composite::deselect_(bool update_parent)
	{
		// if anything is selected, deselect everything below
		if (selected_ || (number_of_selected_children_ > 0)
				|| (number_of_children_containing_selection_ > 0))
		{
			Composite* child = first_child_;
			for (; child != 0; child = child->next_)
			{
				if (child->containsSelection())
				{
					child->deselect_(false);
				}
			}
			
			selected_ = false;
			number_of_selected_children_ = 0;
			number_of_children_containing_selection_ = 0;
			
			// update the time stamp
			selection_stamp_.stamp();

			// check whether we have to inform the parent
			if (update_parent && (parent_ != 0))
			{
				// we were selected beforehand, so decrease
				// the corresponding counters
				parent_->number_of_selected_children_--;
				if (contains_selection_)
				{
					// increase the corresponding counter in the parent
					parent_->number_of_children_containing_selection_--;
				}
				contains_selection_ = false;

				// updateSelection_ updates the time stamp as well
				// and propagates it upwards to the root
				parent_->updateSelection_();
			} 
			else 
			{
				contains_selection_ = false;
			}
		}
	}

	void Composite::destroyChildren_()
	{
		for (Composite* composite_ptr = first_child_; composite_ptr != 0; )
		{
			Composite* next_ptr = composite_ptr->next_;

			if (composite_ptr->isAutoDeletable())
			{
				delete composite_ptr;
			}
			else
			{
				composite_ptr->previous_ = composite_ptr->next_ = composite_ptr->parent_ = 0;
				composite_ptr->clear();
			}

			composite_ptr = next_ptr;
		}

	}

	void Composite::updateSelection_()
	{
		// update the time stamp
		selection_stamp_.stamp();

		// and propagate the time stamp upwards to the root
		Composite* parent(parent_);
		while (parent != 0)
		{
			parent->selection_stamp_.stamp();
			parent = parent->parent_;
		}

		// calculate the new selection flags according 
		// to the current contents of the counters
		bool new_selected = (((number_of_selected_children_ == number_of_children_) && (number_of_children_ > 0))
												 || ((number_of_children_ == 0) && selected_));
		bool new_contains_selection = ((number_of_children_containing_selection_ > 0) || new_selected);
		
		// now check for transitions in the node's state
		if (((selected_ != new_selected) || (new_contains_selection != contains_selection_)) && (parent_ != 0))
		{
			// we have a transition in the selection state
			if (selected_ && !new_selected)
			{
				// this node got deselected
				if (contains_selection_ && !new_contains_selection)
				{
					// it does not even contain a selection any more
					parent_->number_of_selected_children_--;
					parent_->number_of_children_containing_selection_--;
				} 
				else 
				{
					// it still contains selected nodes
					parent_->number_of_selected_children_--;
				}
			} 
			else 
			{
				if (!selected_ && new_selected) 
				{
					// the node was deselected before and now is selected
					if (!contains_selection_)
					{
						// the node didn`t contain selected nodes before
						parent_->number_of_selected_children_++;
						parent_->number_of_children_containing_selection_++;
					} 
					else 
					{
						// the node did contain selected nodes before
						parent_->number_of_selected_children_++;
					}
				} 
				else 
				{
					// selected didn`t change at all
					if (contains_selection_ && !new_contains_selection)
					{
						// there are no more selections left
						parent_->number_of_children_containing_selection_--;
					} 
					else 
					{
						// a child now contains a selection
						parent_->number_of_children_containing_selection_++;
					}
				}
			}
			// store the new flags (have to be set prior to the recursive call 
			// to updateSelection())
			selected_ = new_selected;
			contains_selection_ = new_contains_selection;

			// something changed - 
			// walk up the tree as far as necessary
			parent_->updateSelection_();
		}

		// store the new flags (else case of outer if loop)
		selected_ = new_selected;
		contains_selection_ = new_contains_selection;		
	}

	void Composite::determineSelection_()
	{
		number_of_selected_children_ = 0;
		number_of_children_containing_selection_ = 0;
		
		Composite* composite_ptr = first_child_;
		for (; composite_ptr != 0; composite_ptr = composite_ptr->next_)
		{
			if (composite_ptr->contains_selection_)
			{
				++number_of_children_containing_selection_;
				if (composite_ptr->selected_)
				{
					++number_of_selected_children_;
				}
			}
		}
		
		// set the selction flags and propagate them
		// upwards (if necessary)
		updateSelection_();
	}

	void Composite::prependChild(Composite& composite)
	{
		// avoid self-prepend and prepend of children
		if (&composite == this || isDescendantOf(composite))
		{
			return;
		}
		
		// if it is already the first child, abort
		if (&composite == first_child_)
		{
			return;
		}

		// remove it first, if it has a parent
		if (composite.parent_ != 0)
		{
			composite.parent_->removeChild(composite);
		}

		// insert the new child
		if (first_child_ == 0)
		{ 
			// if there is no child, it's easy
			first_child_ = last_child_ = &composite;
		} 
		else 
		{
			// otherwise insert it into the list
			first_child_->previous_ = &composite;
			composite.next_ = first_child_;
			first_child_ = &composite;
		}

		composite.parent_ = this;
		++number_of_children_;

		// update modification time stamp
		first_child_->stamp(MODIFICATION);
	
		// update selection counters
		if (composite.containsSelection())
		{
			number_of_children_containing_selection_++;
			
			if (composite.selected_)
			{
				number_of_selected_children_++;
			}
			
			// recursively update the nodes` states
			updateSelection_();
		}
	}

	void Composite::appendChild(Composite& composite)
	{
		// avoid self-appending and appending of parent nodes
		if (&composite == this || isDescendantOf(composite))
		{
			return;
		}
		
		// if it is already the last child, everything is done
		if (&composite == last_child_)
		{
			return;
		}
		
		// if composite has a parent, remove it from there
		if (composite.parent_ != 0)
		{
			composite.parent_->removeChild(composite);
		}

		// insert it
		if (last_child_ == 0)
		{
			// its the only child - easy!
			first_child_ = last_child_ = &composite;
		} 
		else 
		{
			// append it to the list of children
			last_child_->next_ = &composite;
			composite.previous_ = last_child_;
			last_child_ = &composite;
		}

		composite.parent_ = this;
		++number_of_children_;

		// update modification time stamp
		last_child_->stamp(MODIFICATION);

		// update selection counters
		if (composite.containsSelection())
		{
			number_of_children_containing_selection_++;

			if (composite.selected_)
			{
				number_of_selected_children_++;
			}

			// recursively update the nodes` states
			updateSelection_();
		}

		// v2.1 P2.1.1 deferred to P2.1.2: side-table mutation mirror
		// here required careful destruction-order handling to avoid
		// heap corruption when ~System or ~Molecule cascades destroy
		// children mid-teardown. P2.1.1 ships only the scaffolding
		// (mirror helper + virtual hooks + parity test via direct
		// allocator API). The wiring of mutation paths lives in
		// P2.1.2 with proper "destruction-in-progress" detection.
	}

	bool Composite::insertParent(Composite& parent, Composite& first, Composite& last, bool destroy_parent)
	{
		// return if first and last have different parents, 
		// if either doesn't possess a parent, or if parent equals first or last
		if (first.parent_ != last.parent_ || first.parent_ == 0 || &first == &parent || &last == &parent)
		{
			return false;
		}

		// if first is already a child of parent, return immediately
		if (first.isDescendantOf(parent))
		{
			return true;
		}

		// destroy the parent's contents
		parent.destroy(destroy_parent);

		// insert the new parent as a child into the
		// "first" node's parent
		Composite* parent_ptr = first.parent_;
		parent.parent_ = parent_ptr;
		parent.first_child_ = &first;
		parent.last_child_ = &last;
		
		if (parent_ptr->first_child_ == &first)
		{
			if (parent_ptr->last_child_ == &last)
			{
				parent_ptr->last_child_ = &parent;
			} 
			else 
			{
				last.next_->previous_ = &parent;
				parent.next_  = last.next_;
				last.next_ = 0;
			}

			parent_ptr->first_child_ = &parent;
		}
		else 
		{
			if (parent_ptr->last_child_ == &last)
			{
				first.previous_->next_ = &parent;
				parent.previous_  = first.previous_;
				first.previous_ = 0;
				
				parent_ptr->last_child_ = &parent;
			} 
			else 
			{
				first.previous_->next_ = &parent;
				parent.previous_ = first.previous_;
				last.next_->previous_ = &parent;
				parent.next_ = last.next_;
				first.previous_ = last.next_ = 0;
			}
		}
		
		for (Composite* composite_ptr = &first; composite_ptr != &last; composite_ptr = composite_ptr->next_)
		{
			++(parent.number_of_children_);
			--(parent_ptr->number_of_children_);
			composite_ptr->parent_ = &parent;
		}

		last.parent_ = &parent;
		++(parent.number_of_children_);

		// update all selection fields recursively
		parent.determineSelection_();

		// update the modification time stamps
		first.stamp(MODIFICATION);
		last.stamp(MODIFICATION);

		return true;
	}

	void Composite::insertBefore(Composite& composite)
	{
		// abort if a self-insertion is requested, there`s no parent at all,
		// or this node is a descendant of composite
		if (parent_ == 0 || &composite == this
				|| isDescendantOf(composite))
		{
			return;
		}

		// if this node is the first child of its parent, this task 
		// reduces to a prependChild
		if (parent_->first_child_ == this)
		{
			parent_->prependChild(composite);

			return;
		}

		// if composite has a parent, remove it from there
		if (composite.parent_ != 0)
		{
			composite.parent_->removeChild(composite);
		}

		// open the linked list and insert composite
		// between this node and its left (previous) neighbour
		previous_->next_ = &composite;
		composite.previous_ = previous_;
		previous_ = &composite;
		composite.next_ = this;
		
		composite.parent_ = parent_;
		++(parent_->number_of_children_);
		
		// update selection counters
		if (composite.containsSelection())
		{
			++(parent_->number_of_children_containing_selection_);
			
			if (composite.selected_)
			{
				++(parent_->number_of_selected_children_);
			}
			
			parent_->updateSelection_();
		}

		// update modification time stamps
		if (previous_ != 0)
		{
			previous_->stamp(MODIFICATION);
		}
		if (next_ != 0)
		{
			next_->stamp(MODIFICATION);
		}
		stamp(MODIFICATION);
	}

	void Composite::insertAfter(Composite& composite)
	{
		// abort if there`s no parent, on self-insertion, or
		// if this node is a descendant of composite
		if (parent_ == 0 || &composite == this
				|| isDescendantOf(composite))
		{
			return;
		}

		// if this node is the last node in its parent`s list
		// the problem reduces to appendChild
		if (parent_->last_child_ == this)
		{
			parent_->appendChild(composite);

			return;
		}

		// if composite has a parent, remove it from there
		if (composite.parent_ != 0)
		{
			composite.parent_->removeChild(composite);
		}
		
		// insert composite in the children`s linked
		// list between this node and its successor (next_)
		next_->previous_ = &composite;
		composite.next_ = next_;
		next_ =& composite;
		composite.previous_ = this;
		
		composite.parent_ = parent_;
		++(parent_->number_of_children_);

		// update selection counters
		if (composite.containsSelection())
		{
			++(parent_->number_of_children_containing_selection_);
			
			if (composite.selected_)
			{
				++(parent_->number_of_selected_children_);
			}
			
			parent_->updateSelection_();
		}

		// update modification time stamps
		if (previous_ != 0)
		{
			previous_->stamp(MODIFICATION);
		}
		if (next_ != 0)
		{
			next_->stamp(MODIFICATION);
		}
		stamp(MODIFICATION);
	}

	void Composite::replace(Composite& composite)
	{
		if (parent_ != 0 && &composite != this)
		{
			insertBefore(composite);
			parent_->removeChild(*this);
			// v2.2 H2b: the parent now has `composite` in this's old slot.
			// Composite::insertBefore above is NOT mirrored (only the
			// AtomContainer wrapper is) and removeChild only removed `this`,
			// so re-derive the affected parent from v0.
			if (Composite* p = composite.getParent()) p->mirrorRederiveOwnRow_();
		}
	}

	void Composite::spliceBefore(Composite& composite)
	{
		// avoid self-splicing and the splicing of nodes that are
		// descendants of composite
		if (&composite == this || isDescendantOf(composite))
		{
			return;
		}
		
		// if composite has a parent, remove it from there
		if (composite.parent_ != 0)
		{
			composite.parent_->removeChild(composite);
		}
		
		// assign the new parent to all children of composite
		for (Composite* composite_ptr =  composite.first_child_;
				 composite_ptr != 0; composite_ptr = composite_ptr->next_)
		{
			composite_ptr->parent_ = this;
		}

		// insert all children of composite into the 
		// double linked list of this composite
		if (composite.first_child_ != 0)
		{
			if (first_child_ != 0)
			{
				composite.last_child_->next_ = first_child_;
				first_child_->previous_  = composite.last_child_;
			} 
			else 
			{
				last_child_ = composite.last_child_;
			}
			
			first_child_ = composite.first_child_;
		}

		// update this composite
		number_of_children_ += composite.number_of_children_;
		number_of_selected_children_ += composite.number_of_selected_children_;
		number_of_children_containing_selection_ += composite.number_of_children_containing_selection_;
		updateSelection_();

		// update the (now empty) composite
		composite.number_of_children_ = 0;
		composite.first_child_  = composite.last_child_ = 0;
		composite.number_of_selected_children_ = 0;
		composite.number_of_children_containing_selection_ = 0;
		composite.contains_selection_ = composite.selected_;

		// update the modification time stamp
		composite.stamp(MODIFICATION);
		stamp(MODIFICATION);

		// v2.2 H2b: composite's children were relocated under this (composite
		// is now empty). Re-derive both rows from v0 (order-independent --
		// clear_children is vector-only, so reverse edges are re-established
		// to the correct final parent by each re-append).
		mirrorRederiveOwnRow_();
		composite.mirrorRederiveOwnRow_();
	}

	void  Composite::spliceAfter(Composite& composite)
	{
		// avoid self-splicing and the splicing of nodes that are
		// descendants of composite
		if (&composite == this || isDescendantOf(composite))
		{
			return;
		}
		
		// if composite has a parent, remove it from there
		if (composite.parent_ != 0)
		{
			composite.parent_->removeChild(composite);
		}

		// assign the new parent to all children of composite
		for (Composite* composite_ptr = composite.first_child_;
				 composite_ptr != 0; composite_ptr = composite_ptr->next_)
		{
			composite_ptr->parent_ = this;
		}
		
		// insert all children of composite into the 
		// double linked list of this composite
		if (composite.first_child_ != 0)
		{
			if (first_child_ != 0)
			{
				last_child_->next_ = composite.first_child_;
				composite.first_child_->previous_  = last_child_;
			} 
			else 
			{
				first_child_ = composite.first_child_;
			}

			last_child_ = composite.last_child_;
		}

		// update this composite
		number_of_children_ += composite.number_of_children_;
		number_of_selected_children_ += composite.number_of_selected_children_;
		number_of_children_containing_selection_ += composite.number_of_children_containing_selection_;
		updateSelection_();

		// update the (now empty) composite
		composite.number_of_children_ = 0;
		composite.first_child_  = composite.last_child_ = 0;
		composite.number_of_selected_children_ = 0;
		composite.number_of_children_containing_selection_ = 0;
		composite.contains_selection_ = composite.selected_;

		// update the modification time stamp
		composite.stamp(MODIFICATION);
		stamp(MODIFICATION);

		// v2.2 H2b: composite's children relocated under this.
		mirrorRederiveOwnRow_();
		composite.mirrorRederiveOwnRow_();
	}

	void Composite::splice(Composite& composite)
	{
		// avoid self-splicing and splicing of descendants of composite
		if (&composite == this|| isDescendantOf(composite))
		{
			return;
		}
		
		// if composite is the first child of this composite or
		// composite is a child of this composite,
		// the problem is a special case of spliceBefore()...
		if (&composite == first_child_ || composite.parent_ != this)
		{
			spliceBefore(composite);
			return;
		} 
		else 
		{
			if (&composite == last_child_)
			{
				// ... or spliceAfter()
				spliceAfter(composite);
				return;
			}
		}

		// set the parent of each of composite`s children
		for (Composite* composite_ptr  = composite.first_child_;
				 composite_ptr != 0; composite_ptr = composite_ptr->next_)
		{
			composite_ptr->parent_ = this;
		}
		
		// now insert all children of composite
		if (composite.first_child_ != 0)
		{
			if (first_child_ != 0)
			{
				composite.first_child_->previous_  = composite.previous_;

				if (composite.previous_ != 0)
				{
					composite.previous_->next_ = composite.first_child_;
				}

				composite.last_child_->next_ = composite.next_;

				if (composite.next_ != 0)
				{
					composite.next_->previous_ = composite.last_child_;
				}
			} 
			else 
			{
				first_child_ = composite.first_child_;
				last_child_ = composite.last_child_;
			}
		}

		// remove composite from this
		composite.parent_ = composite.previous_ = composite.next_ 
			= composite.last_child_ = composite.first_child_  = 0;

		// update counters		
		// - 1 because composite was a child of this, too
		number_of_children_ += composite.number_of_children_ - 1;
		number_of_selected_children_ += composite.number_of_selected_children_;
		number_of_children_containing_selection_ += composite.number_of_children_containing_selection_;
		if (composite.selected_)
		{
			--number_of_selected_children_;
		} 
		if (composite.contains_selection_)
		{
			-- number_of_children_containing_selection_;
		}

		composite.number_of_children_ = 0;
		composite.number_of_selected_children_ = 0;
		composite.number_of_children_containing_selection_ = 0;
		composite.contains_selection_ = composite.selected_;

		// clean up everything
		updateSelection_();

		// update the modification time stamp
		composite.stamp(MODIFICATION);
		stamp(MODIFICATION);

		// v2.2 H2b: composite was a child of this; its children replaced it
		// in this's child list (composite is now detached + empty).
		mirrorRederiveOwnRow_();
		composite.mirrorRederiveOwnRow_();
	}

	bool Composite::removeChild(Composite& child)
	{
		// avoid self-removal and removal of ancestors
		if (&child == this || isDescendantOf(child))
		{
			return false;
		}

		Composite* parent_ptr = child.parent_;

		// if child has no parent, we cannot remove it
		if (parent_ptr != this)
		{
			return false;
		}

		// v2.1 P2.1.1: capture child's siblings BEFORE the v0 detach
		// logic zeros them out. After the detach, child.next_ /
		// child.previous_ are 0, so we can't recover which siblings
		// had their links updated. Mirror those siblings at the end.
		Composite* prev_sibling_to_mirror = child.previous_;
		Composite* next_sibling_to_mirror = child.next_;
			
		// remove child from the list of children
		if (first_child_ == &child)
		{
			first_child_ = first_child_->next_;

			if (first_child_ != 0)
			{
				first_child_->previous_ = 0;
			} 
			else 
			{
				last_child_ = 0;
			}
			
			child.next_ = 0;
		} 
		else 
		{
			if (last_child_ == &child)
			{
				last_child_ = child.previous_;
				last_child_->next_ = child.previous_ = 0;
			} 
			else
			{
				child.previous_->next_ = child.next_;
				
				child.next_->previous_ = child.previous_;
				
				child.previous_ = child.next_ = 0;
			}
		}
		// delete the child`s parent pointer
		child.parent_ = 0;
		
		// adjust some counters
		--number_of_children_;
		if (child.contains_selection_)
		{
			--number_of_children_containing_selection_;
			if (child.selected_)
			{
				--number_of_selected_children_;
			}
		}

		// update the selection
		updateSelection_();

		// update modification time stamp
		stamp(MODIFICATION);

		(void) prev_sibling_to_mirror;
		(void) next_sibling_to_mirror;

		// v2.2 H2a (D69/D70): mirror the detach into the container table.
		// FORWARD-ONLY (D45/D60): skip when THIS (the parent) is being
		// destroyed — during a ~Composite cascade, destroyChildren_ deletes
		// each child whose ~Composite calls parent_->removeChild(*this) on
		// this mid-destruction parent; mirroring then would mutate a
		// half-destroyed parent's row (the P2.1.1 trap). The dying parent's
		// rows leak (freed wholesale at ~System); v0 stays source of truth.
		// Explicit removeChild / clear() / destroy() on a LIVE parent
		// (being_destroyed_ == false) DOES mirror.
		mirrorRemoveChild_(child);

		return true;
	}

	Size Composite::removeSelected()
	{
		// Collect all selected composites in a list.
		std::list<Composite*> composites;
		for (CompositeIterator ci = beginComposite(); +ci; ++ci)
		{
			if (ci->isSelected())
			{
				composites.push_back(&*ci);
			}
		}

		deleteChildrenList_(composites);

		// Return the number of composites deleted.
		return composites.size();
	}

	Size Composite::removeUnselected()
	{
		// Collect all unselected composites in a list.
		std::list<Composite*> composites;
		for (CompositeIterator ci = beginComposite(); +ci; ++ci)
		{
			/*
			 * Each composite has a list of child composites that containes
			 * itself. Thus: do not delete yourself
			 */
			if (!ci->isSelected() && (this != &*ci ))
			{
				composites.push_back(&*ci);
			}
		}

		deleteChildrenList_(composites);

		// Return the number of composites deleted.
		return composites.size();
	}

	void Composite::deleteChildrenList_(std::list<Composite*>& composites)
	{
		// Remove all composites from their hierarchy (to avoid recursive
		// deletion).
		std::list<Composite*>::iterator li;
		for (li = composites.begin(); li != composites.end(); ++li)
		{
			if ((*li)->getParent() != 0)
			{
				(*li)->getParent()->removeChild(**li);
			}
		}

		// Delete the individual composites if they are marked
		// as "autodeletable".
		for (li = composites.begin(); li != composites.end(); ++li)
		{
			if ((*li)->isAutoDeletable())
			{
				delete *li;
			}
		}
	}

	void Composite::clear()
	{
		for (Composite* composite_ptr = first_child_; composite_ptr != 0; )
		{
			Composite* next_ptr = composite_ptr->next_;
		
			if (composite_ptr->isAutoDeletable())
			{
				delete composite_ptr;
			} 
			else
			{
				composite_ptr->previous_ = composite_ptr->next_ = composite_ptr->parent_ = 0;
				composite_ptr->clear();
			}
			
			composite_ptr = next_ptr;
		}

		// clear pointers
		first_child_ = last_child_ = 0;

		// clear properties
		properties_ = 0;

		// update counters and selection
		number_of_children_ = 0;
		number_of_selected_children_ = 0;
		number_of_children_containing_selection_ = 0;
		contains_selection_ = selected_;
		updateSelection_();

		// update modification time stamp
		stamp(MODIFICATION);

		// v2.2 H2b: this container is now empty. Re-derive its row -> empty
		// (self-guards on being_destroyed_, so a destructor-cascade clear()
		// does NOT mirror -- the row is freed wholesale). This also covers
		// the NON-auto-deletable branch above, which detaches children by
		// direct pointer nulling (bypassing removeChild's mirror) -- R35
		// MEDIUM, deferred from H2a.
		mirrorRederiveOwnRow_();
		// NOTE (H2b scope boundary): AtomContainer::clear()/Residue::clear()
		// etc. ALSO reset scalar identity fields (name_/id_) to defaults AFTER
		// this base call returns, so the row's stored scalars desync here.
		// Mirroring SCALAR-field mutation (name/id/insertion-code/SS-type) is a
		// distinct carry-over from the topology mirror (see V2X-ROADMAP H2
		// carry-overs) -- deferred because the collapse (H1b') replaces typed
		// name/id with role+payload, so the scalar mirror is (re)built once
		// against the collapsed model rather than twice. The parity test
		// asserts TOPOLOGY parity for clear()/swap(); scalar parity is covered
		// when the scalar mirror lands.
	}

	void Composite::destroy()
	{
		if (parent_ != 0)
		{
			parent_->removeChild(*this);
		}

		clear();
	}

	void Composite::destroy(bool virtual_flag)
	{
		if (parent_ != 0)
		{
			parent_->removeChild(*this);
		}

		if (virtual_flag)
		{
			clear();
			stamp(MODIFICATION);
		} 
		else 
		{
			Composite::clear();
		}
	}

	void Composite::swap(Composite& composite)
	{
		if (&composite == this || isAncestorOf(composite)
				|| isDescendantOf(composite))
		{
			return;
		}

		// --- v2.2 H2b: snapshot-then-apply position relink ---
		// The legacy in-place pointer surgery here read sibling links it had
		// already mutated, producing self-referential next_/previous_ chains
		// (and corrupted parent first/last pointers) whenever the two nodes
		// shared a parent or were adjacent siblings -- an infinite loop on the
		// next sibling walk (e.g. determineSelection_). Production never swaps
		// siblings (only cross-parent / root swaps, e.g. rigid_docking), so the
		// bug was latent until the H2b sibling-swap parity test. Snapshot every
		// link first, then apply: correct for all cases (siblings, adjacent,
		// cross-parent) and identical to the old behaviour for the cross-parent
		// path. (Moot at H4 -- the Composite tree is deleted -- but a live
		// infinite loop must not ship.)
		Composite* node_a = this;
		Composite* node_b = &composite;
		Composite* a_prev = node_a->previous_; Composite* a_next = node_a->next_;
		Composite* b_prev = node_b->previous_; Composite* b_next = node_b->next_;

		// parents' first/last child pointers (decided from the snapshot)
		const bool ap_first_a = (node_a->parent_ != 0 && node_a->parent_->first_child_ == node_a);
		const bool ap_last_a  = (node_a->parent_ != 0 && node_a->parent_->last_child_  == node_a);
		const bool bp_first_b = (node_b->parent_ != 0 && node_b->parent_->first_child_ == node_b);
		const bool bp_last_b  = (node_b->parent_ != 0 && node_b->parent_->last_child_  == node_b);
		if (ap_first_a) node_a->parent_->first_child_ = node_b;
		if (ap_last_a)  node_a->parent_->last_child_  = node_b;
		if (bp_first_b) node_b->parent_->first_child_ = node_a;
		if (bp_last_b)  node_b->parent_->last_child_  = node_a;

		// external sibling neighbours (skip when the neighbour IS the other
		// swap node -- that link is repaired by the adjacency fix-up below)
		if (a_prev != 0 && a_prev != node_b) a_prev->next_     = node_b;
		if (a_next != 0 && a_next != node_b) a_next->previous_ = node_b;
		if (b_prev != 0 && b_prev != node_a) b_prev->next_     = node_a;
		if (b_next != 0 && b_next != node_a) b_next->previous_ = node_a;

		// swap the internal position links
		std::swap(node_a->parent_,   node_b->parent_);
		std::swap(node_a->previous_, node_b->previous_);
		std::swap(node_a->next_,     node_b->next_);

		// repair direct adjacency (the only links the swap above leaves
		// self-referential when a and b were neighbours)
		if (a_next == node_b)        // a was immediately before b -> ... b, a ...
		{
			node_a->previous_ = node_b;
			node_b->next_     = node_a;
		}
		else if (b_next == node_a)   // b was immediately before a -> ... a, b ...
		{
			node_b->previous_ = node_a;
			node_a->next_     = node_b;
		}

		// adjust pointers to and from the composite`s children
		Composite* composite_ptr = composite.first_child_;
		for (; composite_ptr != 0; composite_ptr = composite_ptr->next_)
		{
			composite_ptr->parent_ = this;
		}
		composite_ptr = first_child_;
		for (; composite_ptr != 0; composite_ptr = composite_ptr->next_)
		{
			composite_ptr->parent_ = &composite;
		}
		std::swap(number_of_children_, composite.number_of_children_);
		std::swap(first_child_, composite.first_child_);
		std::swap(last_child_, composite.last_child_);

		// swap all other attributes
		std::swap(number_of_selected_children_, composite.number_of_selected_children_);
		std::swap(number_of_children_containing_selection_, composite.number_of_children_containing_selection_);
		std::swap(contains_selection_, composite.contains_selection_);
		std::swap(properties_, composite.properties_);
		Selectable::swap(composite);

		// update the modification time stamps
		stamp(MODIFICATION);
		composite.stamp(MODIFICATION);

		// if the two parents are different, we have to update
		// the selection information
		if (parent_ != 0)
		{
			parent_->determineSelection_();
		}
		if (composite.parent_ != 0)
		{
			composite.parent_->determineSelection_();
		}

		// v2.2 H2b: swap exchanges BOTH tree position AND child lists, so up
		// to four rows changed: this + composite (children swapped) and their
		// (now swapped) parents (one child each swapped). Re-derive all four
		// from v0 (order-independent: clear_children is vector-only, so each
		// re-append re-establishes reverse edges to the correct final
		// parent). atom<->container swap is excluded by D70 (an Atom is never
		// a swap target in the molecular hierarchy); not guarded here beyond
		// the row-kind discrimination in re-derive.
		mirrorRederiveOwnRow_();
		composite.mirrorRederiveOwnRow_();
		if (Composite* p = getParent())           p->mirrorRederiveOwnRow_();
		if (Composite* p = composite.getParent())  p->mirrorRederiveOwnRow_();
	}

	bool Composite::isDescendantOf(const Composite& composite) const
	{
		for (Composite* composite_ptr = parent_;
				 composite_ptr != 0; composite_ptr = composite_ptr->parent_)
		{
			if (composite_ptr == &composite)
			{
				return true;
			}
		}
		
		return false;
	}

	bool Composite::isHomomorph(const Composite& composite) const
	{
		if (this == &composite)
		{
			return true;
		}

		if (number_of_children_ != composite.number_of_children_)
		{
			return false;
		}

		Composite* ptr_a = first_child_;
		Composite* ptr_b = composite.first_child_;

		for (; ptr_a != 0 && ptr_b != 0;
				 ptr_a = ptr_a->next_, ptr_b = ptr_b->next_)
		{
			if (!ptr_a->isHomomorph(*ptr_b))
			{
				return false;
			}
		}

		return true;
	}

	bool Composite::isValid() const
	{
		Composite* composite_ptr = first_child_;
		Size number_of_children = 0;
		
		for (; composite_ptr != 0; composite_ptr = composite_ptr->next_)
		{
			if (!composite_ptr->isValid())
			{
#				ifdef BALL_DEBUG      
					Log.error() << "Composite::isValid: INVALID: substructure is invalid" << endl;
#				endif      
				
				return false;
			}
			
			if (composite_ptr->parent_ != this)
			{
#				ifdef BALL_DEBUG      
					Log.error() << "Composite::isValid: INVALID: parent of " << composite_ptr->getHandle()
											<< " should be " << getHandle()
											<< " but is " << composite_ptr->parent_->getHandle() << endl;
#				endif      
				
				return false;
			}
			
			++number_of_children;
			
			if (composite_ptr->next_ == 0)
			{
				if (composite_ptr != last_child_)
				{
#					ifdef BALL_DEBUG      

						Log.error() << "INVALID: last child of " << getHandle()
												<< " should be " << last_child_->getHandle()
												<< " but is " << composite_ptr->getHandle() << endl;
#					endif      
		
					return false;
				}
				
				if (number_of_children != number_of_children_)
				{
#					ifdef BALL_DEBUG      
						Log.error() << "INVALID: number of children " << getHandle()
												<< " should be " << number_of_children_
												<< " but is " << number_of_children << endl;
#					endif      
		
					return false;
				}

				break;
			}
		}
		
		number_of_children = 0;
		
		for (composite_ptr = last_child_; composite_ptr != 0;
				 composite_ptr = composite_ptr->previous_)
		{
			if (!composite_ptr->isValid())
			{
#				ifdef BALL_DEBUG      
					Log.error() << "INVALID: substructure is invalid" << endl;
#				endif      
				
				return false;
			}
			
			++number_of_children;
			
			if (composite_ptr->previous_ == 0)
			{
				if (composite_ptr != first_child_)
				{
#					ifdef BALL_DEBUG      
						Log.error() << "INVALID: first child of " << getHandle()
												<< " should be " << first_child_->getHandle()
												<< " but is " << composite_ptr->getHandle() << endl;
#					endif      
		
					return false;
				}
				
				if (number_of_children != number_of_children_)
				{
#					ifdef BALL_DEBUG      
						Log.error() << "INVALID: number of children of " << getHandle()
												<< " should be " << number_of_children_
												<< " but is " << number_of_children << endl;
#					endif      
		
					return false;
				}

				break;
			}
		}
		
		return true;
	}

	void Composite::dump(std::ostream& s, Size depth) const
	{
		BALL_DUMP_STREAM_PREFIX(s);

		BALL_DUMP_DEPTH(s, depth);
    BALL_DUMP_HEADER(s, this, this)

		Object::dump(s, depth);
		Selectable::dump(s, depth);
		
		BALL_DUMP_DEPTH(s, depth);
		s << "  parent: "
			<< ((parent_) 
					? parent_->getHandle() 
					: (INVALID_Handle)) << endl;
		
		BALL_DUMP_DEPTH(s, depth);
		s << "  first: "
				 << ((first_child_)
			 ? first_child_->getHandle()
			 : INVALID_Handle) << endl;
		
		BALL_DUMP_DEPTH(s, depth);
		s << "  last: "
				 << ((last_child_)
			 ? last_child_->getHandle()
			 : INVALID_Handle) << endl;
		
		BALL_DUMP_DEPTH(s, depth);
		s << "  previous: "
				 << ((previous_)
			 ? previous_->getHandle()
			 : INVALID_Handle) << endl;
		
		BALL_DUMP_DEPTH(s, depth);
		s << "  next: "
				 << ((next_)
			 ? next_->getHandle()
			 : INVALID_Handle) << endl;
		
		BALL_DUMP_DEPTH(s, depth);
		s << "  number of children:" << number_of_children_ << endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "  contains_selection_:" << contains_selection_ << endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "  number of selected children:" << number_of_selected_children_ << endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "  number of children containing selection:" 
			<< number_of_children_containing_selection_ << endl;
		
		BALL_DUMP_DEPTH(s, depth);
		s << "  children:" << endl;
		
		for (Composite* composite_ptr = first_child_;
				 composite_ptr != 0; composite_ptr = composite_ptr->next_)
		{
			BALL_DUMP_DEPTH(s, depth);
			s << "    [" << ((composite_ptr->previous_) 
											 ? composite_ptr->previous_->getHandle() 
											 : INVALID_Handle) << "] <- "
				<< "[" << composite_ptr->getHandle() << "] -> "
				<< "[" << ((composite_ptr->next_) 
									 ? composite_ptr->next_->getHandle() 
									 : INVALID_Handle) << "]" << endl;
			
			composite_ptr->dump(s, depth + 1);
		}

		BALL_DUMP_STREAM_SUFFIX(s);
	}

	Size Composite::getHeight() const
	{
		Size size = 0;

		// if there are children, determine the maximum height
		if (first_child_ != 0)
		{
			return getHeight_(size, size);
		}
		else
		{
			// otherwise we are at height 0
			return 0;
		}
	}

	Size Composite::getHeight_(Size size, Size& max_height) const
	{
		if (++size > max_height)
		{
			max_height = size;
		}
		
		for (Composite* composite_ptr = first_child_;
				 composite_ptr != 0; composite_ptr = composite_ptr->next_)
		{
			if (composite_ptr->first_child_ != 0)
			{
				composite_ptr->getHeight_(size, max_height);
			}
		}
		
		return max_height;
	}
		
	Size Composite::countDescendants_() const
	{
			// walk over all children and recursively call countDescendants
		Size number_of_descendants = 1;
		for (Composite* composite_ptr = first_child_;
				 composite_ptr != 0; 
				 composite_ptr = composite_ptr->next_)
		{
			number_of_descendants += composite_ptr->countDescendants_();
		}
		
		return number_of_descendants;
	}

	Size Composite::count(const KernelPredicateType& predicate) const
	{
		// iterate over the node itself and all its descendants
		// and count hits of the predicate
		Size hits = 0;
		CompositeConstIterator sub_it = beginComposite();
		for (; !sub_it.isEnd(); ++sub_it)
		{
			if (predicate(*sub_it))
			{
				hits++;
			}
		}
		
		return hits;
	}

	void Composite::clone_(Composite& parent, Composite& stack) const
	{
		Composite* cloned_ptr = 0;

		for (Composite* composite_ptr = parent.first_child_;
				 composite_ptr != 0; composite_ptr = composite_ptr->next_)
		{
			cloned_ptr = (Composite*)composite_ptr->create(false);
			stack.appendChild(*cloned_ptr);			
			cloned_ptr->properties_ = composite_ptr->properties_;

			if (composite_ptr->first_child_ != 0)
			{
				clone_(*composite_ptr, *cloned_ptr);
			}		
		}

		// create selection information
		stack.determineSelection_();		
	}
				
	bool Composite::operator == (const Composite& composite) const
	{
		return(Object::operator == (composite));
	}

	bool Composite::operator != (const Composite& composite) const
	{
		return !(*this == composite);
	}

	bool Composite::applyDescendantPreorderNostart_(UnaryProcessor<Atom>& processor)
	{
		Processor::Result result;

		for (Composite* composite = first_child_;
				 composite != 0; composite = composite->next_)
		{
			if (composite->isAtom())
			{
				result = processor(*static_cast<Atom*>(composite));

				if (result <= Processor::BREAK)
				{
					return (result == Processor::BREAK);
				}
			}

			if (composite->first_child_ != 0  && !composite->applyDescendantPreorderNostart_(processor))
			{
				return false;
			}
		}

		return true;
	}

	bool Composite::applyPreorderNostart_(UnaryProcessor<Atom>& processor)
	{
		Processor::Result result;
		bool return_value;
		if (isAtom())
		{
			result = processor(*static_cast<Atom*>(this));

			if (result <= Processor::BREAK)
			{
				return_value = (result == Processor::BREAK);
			}
			else
			{
				return_value =  applyDescendantPreorderNostart_(processor);
			}
		}
		else
		{
			return_value =  applyDescendantPreorderNostart_(processor);
		}

		return return_value;
	}


#	ifdef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/CONCEPT/composite.iC>
#	endif

} // namespace BALL
