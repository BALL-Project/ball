// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/KERNEL/atomContainer.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/global.h>
#include <BALL/KERNEL/system.h>          // K0.4.3: auto-adopt into root System
#include <BALL/KERNEL/atom.h>            // v2.2 H2a: getStoreIndex for the mirror
#include <BALL/KERNEL/moleculeStore.h>   // v2.2 H2a: container write accessors
#include <BALL/CONCEPT/composite.h>      // ancestor lookup
#include <BALL/COMMON/rtti.h>            // RTTI::getDefault<System>
// v2.2 HCP-1b: subtype headers for the scalar-resync RTTI dispatch
// (detail::writeContainerScalars_). These are KERNEL siblings (no cycle at
// the .C level); the gate lifts nothing -- always-on in any build.
#include <BALL/KERNEL/protein.h>
#include <BALL/KERNEL/nucleicAcid.h>
#include <BALL/KERNEL/residue.h>
#include <BALL/KERNEL/nucleotide.h>
#include <BALL/KERNEL/secondaryStructure.h>

using namespace::std;
namespace BALL
{
	// v2.2 HCP-1b: the single source of the container scalar-write dispatch.
	// Shared by System::materialiseContainer_ (initial population) and
	// AtomContainer::mirrorResyncScalars_ (re-sync after clear/swap/setters),
	// so the two can never drift. Mirrors the v0 typed object's scalar
	// identity into its ContainerTable row: common name + per-subtype
	// id / insertion-code / SS-type / ResidueKind (the latter absorbs the
	// Residue::Property identity bits, D-HC1).
	namespace detail
	{
		void writeContainerScalars_(const AtomContainer& c, MoleculeStore& store,
		                            std::uint32_t row)
		{
			if (row == 0) return;
			store.container_set_name_(row, c.getName());
			if (const Protein* p = dynamic_cast<const Protein*>(&c))
			{
				store.container_set_id_(row, p->getID());
			}
			else if (const NucleicAcid* na = dynamic_cast<const NucleicAcid*>(&c))
			{
				store.container_set_id_(row, na->getID());
			}
			else if (const Residue* r = dynamic_cast<const Residue*>(&c))
			{
				store.container_set_id_(row, r->getID());
				store.container_set_insertion_code_(row, r->getInsertionCode());
				ResidueKind rk = ResidueKind::UNKNOWN;
				if (r->hasProperty(Residue::PROPERTY__WATER))             rk = ResidueKind::WATER;
				else if (r->hasProperty(Residue::PROPERTY__AMINO_ACID))   rk = ResidueKind::AMINO_ACID;
				else if (r->hasProperty(Residue::PROPERTY__NON_STANDARD)) rk = ResidueKind::NONSTANDARD;
				store.container_set_residue_kind_(row, rk);
			}
			else if (const Nucleotide* nt = dynamic_cast<const Nucleotide*>(&c))
			{
				store.container_set_id_(row, nt->getID());
				store.container_set_insertion_code_(row, nt->getInsertionCode());
				store.container_set_residue_kind_(row, ResidueKind::NUCLEOTIDE);
			}
			else if (const SecondaryStructure* ss = dynamic_cast<const SecondaryStructure*>(&c))
			{
				store.container_set_ss_type_(row, static_cast<std::uint8_t>(ss->getType()));
			}
		}
	} // namespace detail

	// v2.2 HCP-1b: re-sync THIS container's scalar identity to its row.
	// Self-guards: no-op when being destroyed or unbound.
	void AtomContainer::mirrorResyncScalars_()
	{
		if (isBeingDestroyed_()) return;
		if (container_row_store_ == 0 || container_row_idx_ == 0) return;
		detail::writeContainerScalars_(*this, *container_row_store_, container_row_idx_);
	}
	// v2.2 H2a (R36b fix): the container-table mutation mirror for INSERTS
	// lives here in the AtomContainer insert methods (NOT in adopt()/
	// adoptSubtree()), so it fires regardless of whether adopt early-returns
	// on a same-store move (R36b BLOCKER) and so the insert method's known
	// position is honoured. v0 is the source of truth (D60); mirror is
	// forward-only. `parent` is taken from child.getParent() (robust across
	// receiver semantics). No-op if the parent has no container row
	// (free-standing / System root -- materialised later at adoption).
	namespace
	{
		// v2.2 H2b: these now delegate to the unified Composite mirror
		// helpers (defined in composite.C), which both the AtomContainer
		// insert methods AND the Composite-level topology ops (swap/clear/
		// replace/insertParent/splice) share. `parent` is child.getParent()
		// (robust). No-op when the parent has no container row.
		void mirrorAppendEdge_(Composite* parent, Composite* child)
		{
			if (parent != 0 && child != 0) parent->mirrorAppendChild_(*child);
		}
		void mirrorRederiveParent_(Composite* parent)
		{
			if (parent != 0) parent->mirrorRederiveOwnRow_();
		}
	} // anonymous namespace


	// K0.4.3 helper: walk up the Composite tree to find the root System.
	// Returns nullptr if this AtomContainer isn't rooted under a System.
	// File-local helper used by all the auto-adopting insert/prepend/append
	// overloads below.
	static System* findRootSystem_(Composite* node)
	{
		// Self-or-ancestor lookup. getAncestor with the System type
		// returns the nearest System enclosing this node, or null if none.
		if (node == nullptr) return nullptr;
		// If this node itself is a System, that's the root.
		if (System* self = dynamic_cast<System*>(node)) return self;
		return node->getAncestor(RTTI::getDefault<System>());
	}

	AtomContainer::AtomContainer()
		:	Composite(),
			PropertyManager(),
			name_(BALL_ATOMCONTAINER_DEFAULT_NAME)
	{
	}

	AtomContainer::AtomContainer(const AtomContainer& atom_container, bool deep)
		:	Composite(),
			PropertyManager(),
			name_()
	{
		set(atom_container, deep);
	}

	AtomContainer::AtomContainer(const String& name)
		:	Composite(),
			PropertyManager(),
			name_(name)
	{
	}

	AtomContainer::~AtomContainer()
	{
		setBeingDestroyed_();  // v2.2 H2a (D69): before destroy()'s teardown
		destroy();
	}

	void AtomContainer::clear()
	{
		Composite::clear();
		PropertyManager::clear();

		name_ = BALL_ATOMCONTAINER_DEFAULT_NAME;
		// v2.2 HCP-1b: name (+ subtype id/etc, reset by the most-derived
		// clear() which calls this base first) changed -> re-sync the row.
		mirrorResyncScalars_();
	}

	void AtomContainer::destroy()
	{
		Composite::destroy();
		PropertyManager::destroy();

		name_ = BALL_ATOMCONTAINER_DEFAULT_NAME;
	}

	void AtomContainer::persistentWrite(PersistenceManager& pm, const char* name) const
	{
		pm.writeObjectHeader(this, name);
			Composite::persistentWrite(pm);

			pm.writeStorableObject(dynamic_cast<const PropertyManager&>(*this), "PropertyManager");

			pm.writePrimitive(name_, "name_");
		pm.writeObjectTrailer(name);
	}

	void AtomContainer::persistentRead(PersistenceManager& pm)
	{
		pm.checkObjectHeader(RTTI::getStreamName<Composite>());
			Composite::persistentRead(pm);
		pm.checkObjectTrailer(0);

		pm.readStorableObject(dynamic_cast<PropertyManager&>(*this), "PropertyManager");

		pm.readPrimitive(name_, "name_");
	}

	void AtomContainer::set(const AtomContainer& atom_container, bool deep)
	{
    bool clone_them = clone_bonds;
    clone_bonds = false;

		Composite::set(atom_container, deep);
		PropertyManager::set(atom_container);
		name_ = atom_container.name_;

		// clone the bonds only of we are the outmost set method
		// involved!
    if (clone_them && deep)
    {
			BALL::cloneBonds(atom_container, *this);
		}

    clone_bonds = clone_them;
	}

	AtomContainer& AtomContainer::operator = (const AtomContainer& atom_container)
	{
		set(atom_container);
		return *this;
	}

	void AtomContainer::get(AtomContainer& atom_container, bool deep) const
	{
		atom_container.set(*this, deep);
	}

	void AtomContainer::swap(AtomContainer& atom_container)
	{
		Composite::swap(atom_container);
		PropertyManager::swap(atom_container);
		name_.swap(atom_container.name_);
		// v2.2 HCP-1b: scalars were exchanged between two row-bound nodes ->
		// re-sync both rows (the most-derived swap() runs this last, after its
		// own id/etc swap).
		mirrorResyncScalars_();
		atom_container.mirrorResyncScalars_();
	}

	void AtomContainer::setName(const String& name)
	{
		name_ = name;
		mirrorResyncScalars_();   // v2.2 HCP-1b.2: post-root scalar setter
	}

	const String& AtomContainer::getName() const
	{
		return name_;
	}

	AtomContainer* AtomContainer::getSuperAtomContainer()
	{
		for (Composite::AncestorIterator ancestor_it = beginAncestor();
				 !ancestor_it.isEnd(); ++ancestor_it)
		{
            if (RTTI::isKindOf<AtomContainer>(&*ancestor_it))
			{
				return (AtomContainer *)&*ancestor_it;
			}
		}

		return 0;
	}

	const AtomContainer* AtomContainer::getSuperAtomContainer() const
	{
		return ((AtomContainer *)this)->getSuperAtomContainer();
	}

	AtomContainer* AtomContainer::getAtomContainer(Position position)
	{
		for (AtomContainerIterator atom_container_it = ++beginAtomContainer();
				 !atom_container_it.isEnd(); ++atom_container_it)
		{
			if (position-- == 0)
			{
				return &(*atom_container_it);
			}
		}

		return 0;
	}

	const AtomContainer*  AtomContainer::getAtomContainer(Position position) const
	{
		return ((AtomContainer *)this)->getAtomContainer(position);
	}

	Atom* AtomContainer::getAtom(Position position)
	{
		for (AtomIterator atom_it = beginAtom(); !atom_it.isEnd(); ++atom_it)
		{
			if (position-- == 0)
			{
				return &(*atom_it);
			}
		}

		return 0;
	}

	const Atom* AtomContainer::getAtom(Position position) const
	{
		return ((AtomContainer *)this)->getAtom(position);
	}

	Atom* AtomContainer::getAtom(const String& name)
	{
		for (AtomIterator atom_it = beginAtom(); !atom_it.isEnd(); ++atom_it)
		{
			if ((*atom_it).getName() == name)
			{
				return &(*atom_it);
			}
		}

		return 0;
	}

	const Atom* AtomContainer::getAtom(const String& name) const
	{
		return ((AtomContainer *)this)->getAtom(name);
	}

	Size AtomContainer::countAtomContainers() const
	{
		Size size = 0;

		for (AtomContainerConstIterator atom_container_it = ++beginAtomContainer();
				 !atom_container_it.isEnd(); ++atom_container_it)
		{
			++size;
		}

		return size;
	}

	Size AtomContainer::countAtoms() const
	{
		Size size = 0;

		for (AtomConstIterator atom_it(beginAtom()); !atom_it.isEnd(); ++atom_it)
		{
			++size;
		}

		return size;
	}

	Size  AtomContainer::countBonds() const
	{
		Size								size = 0;
		AtomConstIterator				atom_it;
		Atom::BondConstIterator	bond_it;

		BALL_FOREACH_BOND(*this, atom_it, bond_it)
		{
			++size;
		}

		return size;
	}

	Size AtomContainer::countInterBonds() const
	{
		Size size = 0;
		AtomConstIterator atom_it;
		Atom::BondConstIterator bond_it;

		BALL_FOREACH_INTERBOND(*this, atom_it, bond_it)
		{
			++size;
		}

		return size;
	}

	Size AtomContainer::countIntraBonds() const
	{
		Size size = 0;
		AtomConstIterator atom_it;
		Atom::BondConstIterator bond_it;

		BALL_FOREACH_INTRABOND(*this, atom_it, bond_it)
		{
			++size;
		}

		return size;
	}

	// 2026-05-18 (Codex R12 fix K8): pre-check canAdopt() BEFORE
	// Composite-tree insertion. Pre-fix, `Composite::prependChild` ran
	// first, then `sys->adopt(atom)` could soft-reject (bonded atom
	// with partner in another store), leaving the atom in this
	// container's tree but still bound to its old store. Now if adopt
	// would fail, the whole prepend+adopt sequence is skipped — caller
	// gets the same Log.warn but tree state stays consistent.
	void AtomContainer::prepend(Atom& atom)
	{
		System* sys = findRootSystem_(this);
		if (sys != nullptr && !sys->canAdopt(atom))
		{
			(void) sys->adopt(atom);   // emits the Log.warn; no-op otherwise
			return;
		}
		Composite::prependChild(atom);
		// K0.4.3 auto-adopt: if rooted under a System, migrate the atom
		// from its current store (typically orphan) into the System's
		// store. Single-atom insert path; bond migration is trivial
		// (atom has no bonds to other atoms in src yet, or partner is
		// already in dst).
		if (sys != nullptr) sys->adopt(atom);
		mirrorRederiveParent_(atom.getParent());   // v2.2 H2a (positional)
	}

	void AtomContainer::append(Atom &atom)
	{
		System* sys = findRootSystem_(this);
		if (sys != nullptr && !sys->canAdopt(atom))
		{
			(void) sys->adopt(atom);
			return;
		}
		Composite::appendChild(atom);
		if (sys != nullptr) sys->adopt(atom);
		mirrorAppendEdge_(atom.getParent(), &atom);   // v2.2 H2a
	}

	void AtomContainer::insert(Atom &atom)
	{
		append(atom);
	}

	void AtomContainer::insertBefore(Atom &atom, Composite& before)
	{
		System* sys = findRootSystem_(this);
		if (sys != nullptr && !sys->canAdopt(atom))
		{
			(void) sys->adopt(atom);
			return;
		}
		before.Composite::insertBefore(atom);
		if (sys != nullptr) sys->adopt(atom);
		mirrorRederiveParent_(atom.getParent());   // v2.2 H2a (positional)
	}

	void AtomContainer::insertAfter(Atom& atom, Composite &after)
	{
		// R12 fix K8: pre-check canAdopt before tree insertion.
		System* sys = findRootSystem_(this);
		if (sys != nullptr && !sys->canAdopt(atom))
		{
			(void) sys->adopt(atom);
			return;
		}
		after.Composite::insertAfter(atom);
		if (sys != nullptr) sys->adopt(atom);
		mirrorRederiveParent_(atom.getParent());   // v2.2 H2a (positional)
	}

	bool AtomContainer::remove(Atom& atom)
	{
		return Composite::removeChild(atom);
	}

	Size AtomContainer::removeHavingProperty(BALL::Property p)
	{
		// Collect all unselected composites in a list.
		std::list<Composite*> atoms;
		for (AtomIterator ai = beginAtom(); +ai; ++ai)
		{
			if (ai->hasProperty(p))
			{
				atoms.push_back(&*ai);
			}
		}

		deleteChildrenList_(atoms);

		// Return the number of composites deleted.
		return atoms.size();
	}

	Size AtomContainer::removeNotHavingProperty(BALL::Property p)
	{
		// Collect all unselected composites in a list.
		std::list<Composite*> atoms;
		for (AtomIterator ai = beginAtom(); +ai; ++ai)
		{
			if (!ai->hasProperty(p))
			{
				atoms.push_back(&*ai);
			}
		}

		deleteChildrenList_(atoms);

		// Return the number of composites deleted.
		return atoms.size();
	}

	Size AtomContainer::removeHavingProperty(const string& name)
	{
		// Collect all unselected composites in a list.
		std::list<Composite*> atoms;
		for (AtomIterator ai = beginAtom(); +ai; ++ai)
		{
			if (ai->hasProperty(name))
			{
				atoms.push_back(&*ai);
			}
		}

		deleteChildrenList_(atoms);

		// Return the number of composites deleted.
		return atoms.size();
	}

	Size AtomContainer::removeNotHavingProperty(const string& name)
	{
		// Collect all unselected composites in a list.
		std::list<Composite*> atoms;
		for (AtomIterator ai = beginAtom(); +ai; ++ai)
		{
			if (!ai->hasProperty(name))
			{
				atoms.push_back(&*ai);
			}
		}

		deleteChildrenList_(atoms);

		// Return the number of composites deleted.
		return atoms.size();
	}

	void AtomContainer::prepend(AtomContainer& atom_container)
	{
		Composite::prependChild(atom_container);
		// K0.4.3 batch auto-adopt: subtree's atoms migrate together,
		// preserving intra-subtree bonds (closes the K0.4.2 sequential-
		// adopt orphan-bond limitation).
		if (System* sys = findRootSystem_(this)) sys->adoptSubtree(atom_container);
		mirrorRederiveParent_(atom_container.getParent());   // v2.2 H2a (positional)
	}

	void AtomContainer::append(AtomContainer& atom_container)
	{
		Composite::appendChild(atom_container);
		if (System* sys = findRootSystem_(this)) sys->adoptSubtree(atom_container);
		mirrorAppendEdge_(atom_container.getParent(), &atom_container);   // v2.2 H2a
	}

	void AtomContainer::insert(AtomContainer& atom_container)
	{
		append(atom_container);
	}

	void AtomContainer::insertBefore(AtomContainer& atom_container, Composite& before)
	{
		before.Composite::insertBefore(atom_container);
		if (System* sys = findRootSystem_(this)) sys->adoptSubtree(atom_container);
		mirrorRederiveParent_(atom_container.getParent());   // v2.2 H2a (positional)
	}

	void AtomContainer::insertAfter(AtomContainer& atom_container, Composite& after)
	{
		after.Composite::insertAfter(atom_container);
		if (System* sys = findRootSystem_(this)) sys->adoptSubtree(atom_container);
		mirrorRederiveParent_(atom_container.getParent());   // v2.2 H2a (positional)
	}

	void AtomContainer::spliceBefore(AtomContainer& atom_container)
	{
		Composite::spliceBefore(atom_container);
	}

	void AtomContainer::spliceAfter(AtomContainer& atom_container)
	{
		Composite::spliceAfter(atom_container);
	}

	void AtomContainer::splice(AtomContainer& atom_container)
	{
		Composite::splice(atom_container);
	}

	bool AtomContainer::remove(AtomContainer& atom_container)
	{
		return Composite::removeChild(atom_container);
	}

	void AtomContainer::destroyBonds()
	{
		for (AtomIterator atom_it = beginAtom(); !atom_it.isEnd(); ++atom_it)
		{
			(*atom_it).destroyBonds();
		}
	}

	bool AtomContainer::isSubAtomContainerOf(const AtomContainer& atom_container) const
	{
		return atom_container.isAncestorOf(*this);
	}

	bool AtomContainer::isSuperAtomContainerOf(const AtomContainer& atom_container) const
	{
		return isAncestorOf(atom_container);
	}

	bool AtomContainer::isValid() const
	{
		return (Composite::isValid() && PropertyManager::isValid() && name_.isValid());
	}

	void AtomContainer::dump(ostream& s, Size depth) const
	{
		BALL_DUMP_STREAM_PREFIX(s);

		Composite::dump(s, depth);
		PropertyManager::dump(s, depth);

		BALL_DUMP_DEPTH(s, depth);
		s << "  name: " << name_ << endl;

		BALL_DUMP_STREAM_SUFFIX(s);
	}

	bool AtomContainer::applyInterBond(UnaryProcessor<Bond>& processor)
	{
		if (!processor.start())
		{
			return false;
		}

		Processor::Result		result;
		AtomIterator				atom_it;
		Atom::BondIterator	bond_it;

		BALL_FOREACH_INTERBOND(*this, atom_it, bond_it)
		{
			result = processor(*bond_it);

			if (result <= Processor::BREAK)
			{
				return result == Processor::BREAK;
			}
		}

		return processor.finish();
	}

	bool AtomContainer::applyIntraBond(UnaryProcessor<Bond> &processor)
	{
    if (!processor.start())
		{
      return false;
		}

		Processor::Result  result;
		AtomIterator			 atom_it;
		Atom::BondIterator bond_it;

		BALL_FOREACH_INTRABOND(*this, atom_it, bond_it)
		{
			result = processor(*bond_it);

			if (result <= Processor::BREAK)
			{
				return result == Processor::BREAK;
			}
		}

		return processor.finish();
	}

	bool AtomContainer::operator == (const AtomContainer& atom_container) const
	{
		return(Object::operator == (atom_container));
	}

	bool AtomContainer::operator != (const AtomContainer& atom_container) const
	{
		return ! (*this == atom_container);
	}


} // namespace BALL 
