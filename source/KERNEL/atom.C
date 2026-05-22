// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/fragment.h>
#include <BALL/KERNEL/chain.h>
#include <BALL/KERNEL/secondaryStructure.h>
#include <BALL/KERNEL/residue.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/molecularInteractions.h>
#include <BALL/KERNEL/moleculeStore.h>  // K0.3b.1

#include <algorithm>
#include <mutex>

using namespace::std;

namespace BALL
{
	// K0.3b.1 / K0.4.6: process-global orphan store now lives on
	// MoleculeStore::orphanStore() with a serialising mutex on
	// MoleculeStore::orphanMutex(). This accessor remains for the
	// existing in-tree callers; it forwards to the canonical singleton.
	MoleculeStore& Atom::globalOrphanStore_()
	{
		return MoleculeStore::orphanStore();
	}

	void Atom::bindToStore_(MoleculeStore& store)
	{
		// K0.3c.8 / K0.4.6: atomic slot + back-ptr binding. The unified
		// allocate_atom(this) path writes back_ptr_ BEFORE clearing
		// is_freed_, so no concurrent reader can see a live-but-unbound
		// slot. For the orphan store, the orphan mutex additionally
		// serialises the column mutation against other Atom() ctors and
		// ~Atom calls running on other threads.
		const bool is_orphan = (&store == &MoleculeStore::orphanStore());
		if (is_orphan)
		{
			std::lock_guard<std::recursive_mutex> lk(MoleculeStore::orphanMutex());
			store_ = &store;
			store_idx_ = store.allocate_atom(this);
			store_generation_ = store.generation();
		}
		else
		{
			store_ = &store;
			store_idx_ = store.allocate_atom(this);
			store_generation_ = store.generation();
		}
	}

	// K0.4.2: retarget handle to a new store + slot after System::adopt
	// migrates the payload. Called only by System::adopt; the new slot
	// is already allocated (atomically with back_ptr=this) by adopt
	// before this is invoked.
	//
	// Also called from ~System with new_store == nullptr to sever the
	// handle's tie to a store that's about to be destroyed; the Atom
	// destructor then skips release_atom (since store_ is null).
	void Atom::migrateTo_(MoleculeStore* new_store, std::uint32_t new_idx)
	{
		store_     = new_store;
		store_idx_ = new_idx;
		store_generation_ = (new_store != nullptr) ? new_store->generation() : 0;
	}

	// K0.4.8 (Codex Round 4 MEDIUM-9): the old persistentRead / set /
	// operator= / clear_ paths all carried "if (store_) ..." gates that
	// silently dropped payload writes when the handle was detached. The
	// detached state only happens after ~System nulls its atoms'
	// store_ pointers — at which point further mutation either drops
	// data or crashes. K0.4.8 restores correctness by re-binding the
	// detached handle to the orphan store before any mutation, so the
	// data flows somewhere safe instead of being silently lost.
	void Atom::ensureStoreBinding_()
	{
		if (store_ == nullptr)
		{
			bindToStore_(MoleculeStore::orphanStore());
		}
	}

	// K0.3b.2a/3: write-side helpers, called from atom.iC setters
	// (which only forward-declare MoleculeStore via atom.h).
	//
	// 2026-05-18 layered fixes:
	// - R12 K12: every helper calls ensureStoreBinding_() first, so a
	//   detached atom (post-~System) re-binds to orphan before write
	//   instead of dereferencing null store_.
	// - V21-ORPHAN-MUTATOR-LOCK (R11 fix A complement): if bound to the
	//   orphan store, the actual store write happens under the orphan
	//   mutex. Pre-fix, only the Atom() ctor initial-writes block held
	//   the mutex; post-ctor setters (setName/setCharge/setPosition
	//   called from worker code) ran lock-free and could race with
	//   another thread's ctor allocate_atom that triggers a column
	//   reallocation. The R11 64k pre-reserve made this rare; this
	//   change makes it impossible (or at least, defers it to the
	//   already-mutex-protected reallocation path).
	//
	//   ORPHAN_WRITE_LOCK_(stmt): if store_ is the orphan store, run
	//   `stmt` under the orphan mutex; otherwise run it directly. Used
	//   to wrap each helper's actual column write.
	#define BALL_ATOM_ORPHAN_WRITE_LOCK_(stmt) \
		do { \
			if (store_ == &MoleculeStore::orphanStore()) { \
				std::lock_guard<std::recursive_mutex> lk(MoleculeStore::orphanMutex()); \
				stmt; \
			} else { \
				stmt; \
			} \
		} while (0)

	void Atom::writeStorePosition_(const Vector3& p)
	{
		ensureStoreBinding_();
		BALL_ATOM_ORPHAN_WRITE_LOCK_(store_->position(store_idx_) = p);
	}
	void Atom::writeStoreCharge_(float c)
	{
		ensureStoreBinding_();
		BALL_ATOM_ORPHAN_WRITE_LOCK_(store_->charge(store_idx_) = c);
	}
	void Atom::writeStoreVelocity_(const Vector3& v)
	{
		ensureStoreBinding_();
		BALL_ATOM_ORPHAN_WRITE_LOCK_(store_->velocity(store_idx_) = v);
	}
	void Atom::writeStoreForce_(const Vector3& f)
	{
		ensureStoreBinding_();
		BALL_ATOM_ORPHAN_WRITE_LOCK_(store_->force(store_idx_) = f);
	}
	void Atom::writeStoreName_(const String& s)
	{
		ensureStoreBinding_();
		// HCP-1P.B: bind the String straight to set_name(const std::string&) via
		// String::operator const string& -- zero-copy, no c_str()+strlen+alloc.
		BALL_ATOM_ORPHAN_WRITE_LOCK_(
			store_->set_name(store_idx_, s)
		);
	}
	void Atom::writeStoreTypeName_(const String& s)
	{
		ensureStoreBinding_();
		// HCP-1P.B: zero-copy String -> set_type_name(const std::string&).
		BALL_ATOM_ORPHAN_WRITE_LOCK_(
			store_->set_type_name(store_idx_, s)
		);
	}
	void Atom::writeStoreElement_(const Element* e)
	{
		ensureStoreBinding_();
		// Mirror Element* as atomic number into uint8 column. Null Element
		// stores as 0 (unknown).
		const std::uint8_t ei =
			(e == 0) ? 0 : static_cast<std::uint8_t>(e->getAtomicNumber());
		BALL_ATOM_ORPHAN_WRITE_LOCK_(store_->element_index(store_idx_) = ei);
	}
	const Element& Atom::readStoreElement_() const
	{
		// K0.3b.LATER.7: resolve atomic-number column entry to PTE element
		// reference. atomic_number 0 maps to Element::UNKNOWN via PTE[0].
		// Read path: if detached, return Element::UNKNOWN (no rebind on
		// read — read of detached handle returns sensible default).
		// NOTE: read does not take orphan mutex. Reads of column entries
		// race against concurrent reallocate; the 64k pre-reserve makes
		// this safe in practice. V21+ work to add full read-side locking
		// is filed if needed.
		if (store_ == nullptr) return PTE[(Position)0];
		return PTE[(Position)store_->element_index(store_idx_)];
	}
	void Atom::writeStoreRadius_(float r)
	{
		ensureStoreBinding_();
		BALL_ATOM_ORPHAN_WRITE_LOCK_(store_->radius(store_idx_) = r);
	}
	void Atom::writeStoreAtomType_(short t)
	{
		ensureStoreBinding_();
		BALL_ATOM_ORPHAN_WRITE_LOCK_(store_->atom_type(store_idx_) = t);
	}
	void Atom::writeStoreFormalCharge_(short fc)
	{
		ensureStoreBinding_();
		BALL_ATOM_ORPHAN_WRITE_LOCK_(store_->formal_charge(store_idx_) = fc);
	}

	#undef BALL_ATOM_ORPHAN_WRITE_LOCK_

	// 2026-05-18 (Codex R11 fix A): the orphan-store mutex must be held
	// across the initial-writes block following bindToStore_, not just
	// during the allocate_atom call inside bindToStore_. Otherwise, after
	// bindToStore_ releases the mutex, another thread can enter
	// bindToStore_ → allocate_atom → push_back which reallocates the
	// orphan-store columns; the first thread's mid-flight
	// `store_->position(store_idx_) = p` re-evaluates `position(store_idx_)`
	// against a now-stale base pointer and writes to freed memory.
	// MoleculeStore_test K0.4.6 HIGH-4 stress was flaking ~3% on this race.
	// Helper macro keeps the 3 Atom ctors readable.
	#define BALL_ATOM_ORPHAN_INITIAL_WRITES_LOCK_(initial_writes_block) \
		do { \
			if (store_ == &MoleculeStore::orphanStore()) { \
				std::lock_guard<std::recursive_mutex> lk(MoleculeStore::orphanMutex()); \
				initial_writes_block \
			} else { \
				initial_writes_block \
			} \
		} while (0)

	Atom::Atom()
		: Composite(),
		  PropertyManager(),
		  interactions(0),
		  store_interactions_disabled_(0),
		  number_of_bonds_(0)
	{
		// HCP-1P.A: allocate_atom births the slot at the BALL_ATOM_DEFAULT_*
		// values already (position/charge/velocity/force = 0, radius = 0,
		// name = "", element = UNKNOWN -> atomic# 0, formal_charge = 0,
		// atom_type = UNKNOWN_TYPE = -1, type_name = "" = offset 0), so the former
		// per-atom default re-init block is gone. This is also a concurrency
		// WIN: with no post-bind orphan writes, the R11 stale-base-pointer
		// window (a concurrent ctor's allocate_atom reallocating the columns
		// between bindToStore_ releasing the mutex and these writes) cannot
		// occur for default construction -- the born-default writes happen
		// INSIDE allocate_atom under the same orphan mutex bindToStore_ holds.
		bindToStore_(globalOrphanStore_());
	}

	Atom::Atom(const Atom& atom, bool deep)
		: Composite(atom, deep),
		  PropertyManager(atom),
		  interactions(0),
		  store_interactions_disabled_(0),
			number_of_bonds_(0)
	{
		bindToStore_(globalOrphanStore_());
		// K0.3b.LATER.1-10 + R11 fix A: copy under orphan mutex.
		BALL_ATOM_ORPHAN_INITIAL_WRITES_LOCK_({
			store_->position(store_idx_) = atom.getPosition();
			store_->charge(store_idx_) = atom.getCharge();
			store_->velocity(store_idx_) = atom.getVelocity();
			store_->force(store_idx_) = atom.getForce();
			store_->set_name(store_idx_, atom.getName());          // HCP-1P.B zero-copy
			store_->set_type_name(store_idx_, atom.getTypeName()); // HCP-1P.B zero-copy
			writeStoreRadius_(atom.getRadius());
			writeStoreAtomType_(static_cast<short>(atom.getType()));
			writeStoreFormalCharge_(static_cast<short>(atom.getFormalCharge()));
			writeStoreElement_(&atom.getElement());
		});
	}

	Atom::Atom
			(Element& element, const String& name,
			 const String& type_name, Atom::Type type,
			 const Vector3& position, const Vector3& velocity,
			 const Vector3& force, float charge, float radius, Index formal_charge)
		: Composite(),
		  PropertyManager(),
		  interactions(0),
		  store_interactions_disabled_(0),
		  number_of_bonds_(0)
	{
		bindToStore_(globalOrphanStore_());
		// K0.3b.LATER.1-10 + R11 fix A: write ctor args under orphan mutex.
		BALL_ATOM_ORPHAN_INITIAL_WRITES_LOCK_({
			store_->position(store_idx_) = position;
			store_->charge(store_idx_) = charge;
			store_->velocity(store_idx_) = velocity;
			store_->force(store_idx_) = force;
			store_->set_name(store_idx_, name);           // HCP-1P.B zero-copy
			store_->set_type_name(store_idx_, type_name); // HCP-1P.B zero-copy
			writeStoreRadius_(radius);
			writeStoreAtomType_(static_cast<short>(type));
			writeStoreFormalCharge_(static_cast<short>(formal_charge));
			writeStoreElement_(&element);
		});
	}

	#undef BALL_ATOM_ORPHAN_INITIAL_WRITES_LOCK_

	Atom::~Atom()
	{
		setBeingDestroyed_();  // v2.2 H2a (D69): before destroy()'s teardown
		destroy();
		// K0.3c.1: release this atom's store slot. Without this, every
		// Atom() construction leaked a slot for the process lifetime.
		// Note: destroy() above triggers v1.x bond cleanup; K0.3c.2 will
		// also wire store-side bond removal so the freed slot's incident
		// bond records get tombstoned (currently CSR-rebuild filters
		// them via is_freed()).
		if (store_ != nullptr)
		{
			// K0.4.6: serialise orphan-store release against concurrent
			// bindToStore_/release on other threads.
			if (store_ == &MoleculeStore::orphanStore())
			{
				std::lock_guard<std::recursive_mutex> lk(MoleculeStore::orphanMutex());
				store_->release_atom(store_idx_);
			}
			else
			{
				store_->release_atom(store_idx_);
			}
			store_ = nullptr;  // defensive — handle is now invalid
		}
	}

	void Atom::clear()
	{
		Composite::clear();
		PropertyManager::clear();
		delete interactions;
		interactions = 0;

		clear_();
	}

	void Atom::destroy()
	{
		Composite::destroy();
		PropertyManager::destroy();

		clear_();
	}

	void Atom::persistentWrite(PersistenceManager& pm, const char* name) const
	{
		pm.writeObjectHeader(this, name);

			Composite::persistentWrite(pm);

			pm.writeStorableObject(dynamic_cast<const PropertyManager&>(*this),
														 "PropertyManager");

			pm.writePrimitive((String)getElement().getSymbol(), "element_");
			pm.writePrimitive(getFormalCharge(), "formal_charge_");
			pm.writePrimitive(getCharge(), "charge_");
			pm.writePrimitive(getRadius(), "radius_");
			pm.writePrimitive(getName(), "name_");
			pm.writePrimitive(getTypeName(), "type_name_");
			pm.writePrimitive((Index)getType(), "type_");

			pm.writeStorableObject(getPosition(), "position_");
			pm.writeStorableObject(getVelocity(), "velocity_");
			pm.writeStorableObject(getForce(), "force_");

			pm.writePrimitive(number_of_bonds_, "number_of_bonds_");
			pm.writeObjectPointerArray((Bond**)bond_, "bond_", (Size)number_of_bonds_);
		pm.writeObjectTrailer(name);
	}

	void Atom::persistentRead(PersistenceManager& pm)
	{
		// K0.4.8: rebind detached handles to orphan so reads don't drop.
		ensureStoreBinding_();

		pm.checkObjectHeader(RTTI::getStreamName<Composite>());
			Composite::persistentRead(pm);
		pm.checkObjectTrailer(0);

		pm.readStorableObject(dynamic_cast<PropertyManager&>(*this),
													"PropertyManager");

		String s;
		pm.readPrimitive(s, "element_");
		// K0.3b.LATER.7: element_ deleted; resolve via PTE and write to store.
		const Element* tmp_element = &PTE[s];
		Index tmp_formal_charge;
		float tmp_radius;
		pm.readPrimitive(tmp_formal_charge, "formal_charge_");
		{ float tmp_ch; pm.readPrimitive(tmp_ch, "charge_"); writeStoreCharge_(tmp_ch); }
		pm.readPrimitive(tmp_radius, "radius_");
		// K0.3b.LATER.5+6: read into temporaries, then write through to store.
		String tmp_name, tmp_type_name;
		pm.readPrimitive(tmp_name, "name_");
		pm.readPrimitive(tmp_type_name, "type_name_");
		Index tmp_type;
		pm.readPrimitive(tmp_type, "type_");
		// K0.3b.LATER.5-10: mirror persisted scalars + element + names
		// into store columns/pool (charge already written above).
		writeStoreFormalCharge_(static_cast<short>(tmp_formal_charge));
		writeStoreRadius_(tmp_radius);
		writeStoreAtomType_(static_cast<short>(tmp_type));
		writeStoreElement_(tmp_element);
		// R14.2 fix: route through writeStoreXxx_ helpers (orphan-lock + rebind).
		writeStoreName_(tmp_name);
		writeStoreTypeName_(tmp_type_name);

		// K0.3b.LATER.1+3+4: vectors land in store columns via lock-safe helpers.
		{ Vector3 tmp_pos;   pm.readStorableObject(tmp_pos,   "position_"); writeStorePosition_(tmp_pos); }
		{ Vector3 tmp_vel;   pm.readStorableObject(tmp_vel,   "velocity_"); writeStoreVelocity_(tmp_vel); }
		{ Vector3 tmp_force; pm.readStorableObject(tmp_force, "force_");    writeStoreForce_(tmp_force); }

		pm.readPrimitive(number_of_bonds_, "number_of_bonds_");
		Size n;
		pm.readObjectPointerArray(bond_, "bond_", n);
		if (n != (Size)number_of_bonds_)
		{
			Log.error() << "Atom::persistentRead: size of bond array: read " << n
				<< " instead of " << number_of_bonds_ << endl;
		}
	}

  void Atom::set(const Atom& atom, bool deep)
  {
    // K0.4.8: rebind detached handle to orphan so writes don't drop.
    ensureStoreBinding_();

    Composite::set(atom, deep);
    PropertyManager::operator = (atom);

    // K0.3b.LATER.5-10 + R14.2 fix: payload fields land via lock-safe
    // writeStoreXxx_ helpers (orphan-mutex + ensureStoreBinding).
    number_of_bonds_ = 0;
		writeStorePosition_(atom.getPosition());
		writeStoreCharge_(atom.getCharge());
		writeStoreVelocity_(atom.getVelocity());
		writeStoreForce_(atom.getForce());
		writeStoreName_(atom.getName());
		writeStoreTypeName_(atom.getTypeName());
		writeStoreRadius_(atom.getRadius());
		writeStoreAtomType_(static_cast<short>(atom.getType()));
		writeStoreFormalCharge_(static_cast<short>(atom.getFormalCharge()));
		writeStoreElement_(&atom.getElement());
  }

	Atom& Atom::operator = (const Atom& atom)
	{
		// K0.4.8: rebind detached handle to orphan so writes don't drop.
		ensureStoreBinding_();

		Composite::operator =(atom);
		PropertyManager::operator = (atom);

		// K0.3b.LATER.5-10 + R14.2 fix: route through lock-safe helpers.
		number_of_bonds_ = 0;
		writeStorePosition_(atom.getPosition());
		writeStoreCharge_(atom.getCharge());
		writeStoreVelocity_(atom.getVelocity());
		writeStoreForce_(atom.getForce());
		writeStoreName_(atom.getName());
		writeStoreTypeName_(atom.getTypeName());
		writeStoreRadius_(atom.getRadius());
		writeStoreAtomType_(static_cast<short>(atom.getType()));
		writeStoreFormalCharge_(static_cast<short>(atom.getFormalCharge()));
		writeStoreElement_(&atom.getElement());

		return *this;
	}

	void Atom::swap(Atom &atom)
	{
		Composite::swap(atom);
		PropertyManager::swap(atom);

		// Swap the residual handle-local bond array + count first; those
		// fields still live on the Atom handle (D2 thin-stub stance).
		Bond *temp_bond = 0;
		for (int i = 0; i < MAX_NUMBER_OF_BONDS;++i)
		{
			temp_bond = bond_[i];
			bond_[i] = atom.bond_[i];
			atom.bond_[i] = temp_bond;
		}
		std::swap(number_of_bonds_, atom.number_of_bonds_);

		// K0.4.4 (Codex Round 4 HIGH-1 + HIGH-2): unified snapshot+write
		// model for ALL store-backed payload fields. The pre-K0.4.4 code
		// had two independent swap paths (direct std::swap of vector/
		// charge columns gated on same-store, plus a separate mirror_all
		// snapshot-write for names/element/scalars). Cross-store swap
		// dropped the position/charge/velocity/force exchange entirely.
		// The unified path below snapshots both atoms' full payload BEFORE
		// touching either, then writes the exchanged snapshots back. Works
		// the same way for same-store and cross-store atoms.

		struct Snap {
			Vector3 position;
			Vector3 velocity;
			Vector3 force;
			float   charge;
			float   radius;
			short   atom_type;
			short   formal_charge;
			std::uint8_t element_index;
			String  name;
			String  type_name;
		};
		auto capture = [](const Atom& a) -> Snap {
			Snap s{};
			if (a.store_ == nullptr) return s;
			s.position      = a.store_->position(a.store_idx_);
			s.velocity      = a.store_->velocity(a.store_idx_);
			s.force         = a.store_->force(a.store_idx_);
			s.charge        = a.store_->charge(a.store_idx_);
			s.radius        = a.store_->radius(a.store_idx_);
			s.atom_type     = a.store_->atom_type(a.store_idx_);
			s.formal_charge = a.store_->formal_charge(a.store_idx_);
			s.element_index = a.store_->element_index(a.store_idx_);
			s.name          = a.store_->name(a.store_idx_);
			s.type_name     = a.store_->type_name(a.store_idx_);
			return s;
		};
		const Snap self_snap  = capture(*this);
		const Snap other_snap = capture(atom);

		// R14.2 fix: route writes through lock-safe writeStoreXxx_
		// helpers so orphan-bound atoms get the orphan mutex during
		// payload write. Direct st->* pre-fix could race with another
		// thread's orphan ctor allocate_atom.
		auto apply = [](Atom& target, const Snap& s) {
			if (target.store_ == nullptr) return;
			target.writeStorePosition_(s.position);
			target.writeStoreVelocity_(s.velocity);
			target.writeStoreForce_(s.force);
			target.writeStoreCharge_(s.charge);
			target.writeStoreRadius_(s.radius);
			target.writeStoreAtomType_(s.atom_type);
			target.writeStoreFormalCharge_(s.formal_charge);
			// element_index is uint8 atomic-number; reverse-resolve via PTE.
			const Element* el = &PTE[(Position)s.element_index];
			target.writeStoreElement_(el);
			target.writeStoreName_(s.name);
			target.writeStoreTypeName_(s.type_name);
		};
		apply(*this, other_snap);
		apply(atom,  self_snap);

		// K0.3c.4: same-store bond-connectivity swap rewrites BondRecord
		// .a/.b endpoints so the store bond graph follows the handle-side
		// bond_[] swap above. Cross-store swap leaves the bond records
		// alone; partial bond migration across stores isn't defined.
		if (store_ != nullptr && store_ == atom.store_)
		{
			store_->swap_atom_connectivity(store_idx_, atom.store_idx_);
		}
	}

	Molecule* Atom::getMolecule()
	{
		return Composite::getAncestor(RTTI::getDefault<Molecule>());
	}

	Fragment* Atom::getFragment()
	{
		return Composite::getAncestor(RTTI::getDefault<Fragment>());
	}

	Residue* Atom::getResidue()
	{
		return Composite::getAncestor(RTTI::getDefault<Residue>());
	}

	Chain* Atom::getChain()
	{
		return Composite::getAncestor(RTTI::getDefault<Chain>());
	}

	SecondaryStructure* Atom::getSecondaryStructure()
	{
		return Composite::getAncestor(RTTI::getDefault<SecondaryStructure>());
	}

	String Atom::getFullName(Atom::FullNameType type) const
	{
		// determine the parent`s name
		String parent_name;
		const Residue* parent = getAncestor(RTTI::getDefault<Residue>());
		if (parent == 0)
		{
			// we don't have a residue, look for a fragment
			if (getFragment() != 0)
			{
				parent_name = getFragment()->getName();
				parent_name.trim();
				parent_name += ":";
			}
			// we don't have a fragment either, look for molecule
			else if (getMolecule() != 0)
			{
				parent_name = getMolecule()->getName();
				parent_name.trim();
				parent_name += ":";
			}
		}
		else
		{
			// retrieve the fragment name
			parent_name = parent->getFullName((Residue::FullNameType)type) + ":";
		}

		// retrieve the atom name
		String name = getName();
		name.trim();

		// add the parent name only if non-empty
		if (parent_name != ":")
		{
			name = parent_name + name;
		}

		return name;
	}

	Bond* Atom::getBond(Position index)
	{
		if (index >= (Index)MAX_NUMBER_OF_BONDS)
		{
			throw Exception::IndexOverflow(__FILE__, __LINE__, (Index)index, (Size)MAX_NUMBER_OF_BONDS);
		}

		return ((Size)index < number_of_bonds_) ? bond_[index] : 0;
	}

	const Bond* Atom::getBond(Position index) const
	{
		return (const Bond *)((Atom *)this)->getBond(index);
	}

	Bond* Atom::getBond(const Atom &atom)
	{
		if (&atom != this)
		{
			for (int i = 0; i < number_of_bonds_; ++i)
			{
				if (bond_[i]->first_ == &atom || bond_[i]->second_ == &atom)
				{
					return bond_[i];
				}
			}
		}

		return 0;
	}

	const Bond* Atom::getBond(const Atom &atom) const
	{
		return ((Atom*)this)->getBond(atom);
	}

	Bond* Atom::createBond(Atom& atom)
	{
		// Check whether this bond exists already
		Bond* bond = getBond(atom);

		// If the atom does not yet exist and the bond is not
		// to this atom, we have to construct a new one.
		if ((bond == 0) && (&atom != this))
		{
			// No, we have to create a new one.
			bond = Bond::createBond(*new Bond, *this, atom);
		}
		return bond;
	}

	Bond* Atom::createBond(Bond& bond, Atom& atom)
	{
		return Bond::createBond(bond, *this, atom);
	}

	Bond* Atom::cloneBond(Bond& bond, Atom& atom)
	{
		Bond* bond_ptr = getBond(atom);

		if (bond_ptr == 0)
		{
			bond_ptr = (Bond*)bond.create();
			bond_ptr->setFirstAtom(0);
			bond_ptr->setSecondAtom(0);

			try
			{
				bond_ptr = Bond::createBond(*bond_ptr, *this, atom);
			}
			catch (Exception::TooManyBonds&)
			{
				// Clear up the mess we made.
				delete bond_ptr;
				bond_ptr = 0;
			}
		}
		return bond_ptr;
	}

	bool Atom::destroyBond(const Atom& atom)
	{
		Bond* bond = getBond(atom);

		if (bond == 0)
		{
			return false;
		}

		if (bond->isAutoDeletable())
		{
			delete bond;
		}
		else
		{
			bond->destroy();
		}

		return true;
	}

	void Atom::destroyBonds()
	{
		for (int i = char(number_of_bonds_) - 1; i >= 0; --i)
		{
			if (bond_[i]->isAutoDeletable())
			{
				delete bond_[i];
			}
			else
			{
				bond_[i]->destroy();
			}
		}

		number_of_bonds_ = 0;
	}

	Atom* Atom::getPartnerAtom(Position i)
	{
		return getBond(i)->getBoundAtom(*this);
	}

	const Atom* Atom::getPartnerAtom(Position i) const
	{
		return getBond(i)->getBoundAtom(*this);
	}

	bool Atom::hasBond(const Bond& bond) const
	{
		for (int i = 0; i < number_of_bonds_; ++i)
		{
			if (bond_[i] == &bond)
			{
				return true;
			}
		}
		return false;
	}

	bool Atom::isBoundTo(const Atom& atom) const
	{
		const Bond* bond = getBond(atom);
		if (bond != 0)
		{
			// Ignore H-bonds
			return (bond->getType() != Bond::TYPE__HYDROGEN);
		}
		return false;
	}

	bool Atom::isGeminal(const Atom& atom) const
	{
		if (atom == *this) return false;

		// an atom is geminal to another, if it 
		// is not directly bonded to it
		bool is_geminal = !isBoundTo(atom);

		// second, it has to be bonded to an atom
		// that is bonded to *this atom, too
		if (is_geminal)
		{
			is_geminal = false;
			for (Size i = 0; (i < countBonds()) && !is_geminal; i++)
			{
				Atom* partner_a = getBond(i)->getPartner(*this);
				if (partner_a != 0)
				{
					if (partner_a->isBoundTo(atom))
					{
						is_geminal = true;
						break;
					}
				}
			}
		}

		return is_geminal;
	}

	bool Atom::isVicinal(const Atom& atom) const
	{
		if (atom == *this)
		{
			return false;
		}

		// an atom is vicinal to another, if it 
		// is not directly bonded to it
		bool is_vicinal = !isBoundTo(atom);

		// second, it has to be bonded to an atom
		// that is bonded to an atom that is bonded 
		// to *this atom
		if (is_vicinal)
		{
			is_vicinal = false;
			for (Size i = 0; (i < countBonds()) && !is_vicinal; i++)
			{
				Atom* partner_a = getBond(i)->getPartner(*this);
				if (partner_a != 0)
				{
					for (Size j = 0; (j < partner_a->countBonds()) && !is_vicinal; j++)
					{
						Atom* partner_b = partner_a->getBond(j)->getPartner(*partner_a);
						if ((partner_b) != 0 && (partner_b->isBoundTo(atom)))
						{
							is_vicinal = true;
							break;
						}
					}
				}
			}
		}

		return is_vicinal;
	}

	bool Atom::isValid() const
	{
		// K0.3b.LATER.7: element_ deleted; readStoreElement_ always returns a
		// valid PTE reference (index 0 = Element::UNKNOWN). The pre-flip
		// "element_ == 0" check rejected null pointers, which the API never
		// produced anyway; we drop it.
		if (!Composite::isValid()
				|| !PropertyManager::isValid())
		{
			return false;
		}

		for (int i = 0; i < number_of_bonds_; ++i)
		{
			if (!bond_[i]->isValid())
			{
				return false;
			}
			if (bond_[i]->first_ != this && bond_[i]->second_ != this)
			{
				return false;
			}
		}

		return true;
	}

	void Atom::dump(ostream& s, Size depth) const
	{
		BALL_DUMP_STREAM_PREFIX(s);

		Composite::dump(s, depth);

		BALL_DUMP_DEPTH(s, depth);
		s << "  element: " << getElement() << endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "  formal charge: " << getFormalCharge() << endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "  charge: " << getCharge() << endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "  name: " << getName() << endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "  type name: " << getTypeName() << endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "  position: " << getPosition() << endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "  radius: " << getRadius() << endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "  type: " << getType() << endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "  velocity: " << getVelocity() << endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "  force: " << getForce() << endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "  number of bonds: " << (int)number_of_bonds_ << endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "  bonds: " << endl;

		for (int i = 0; i < number_of_bonds_; ++i)
		{
			bond_[i]->dump(s, depth + 1);
		}
		s << endl;

		BALL_DUMP_STREAM_SUFFIX(s);
	}

	bool Atom::applyBonds(UnaryProcessor<Bond>& processor)

	{
		if (!processor.start())
		{
			return false;
		}
		Processor::Result result;

		for (int i = 0; i < number_of_bonds_; ++i)
		{
			result = processor(*bond_[i]);

			if (result <= Processor::BREAK)
			{
				return result == Processor::BREAK;
			}
		}

		return processor.finish();
	}

	void Atom::clear_()
	{
		// K0.4.8 + R12 fix K12: clear_ is called from BOTH destroy()/~Atom
		// AND user-facing clear(). For the destroy path, ~Atom will run
		// release_atom only if store_ is non-null, so skipping payload
		// resets when store_ is null is correct (the slot is being torn
		// down anyway). For the user-facing path, callers expect a clean
		// handle ready for reuse; the writeStoreXxx_ helpers now all call
		// ensureStoreBinding_() (R12 fix K12), so they rebind a detached
		// handle to the orphan store before writing. Route all payload
		// writes through the helpers for uniform detached-handle handling.
		if (store_ == nullptr)
		{
			// destroy()-path. ~Atom will skip release_atom on null store_,
			// so payload reset is a no-op. Just tear down side state.
			delete interactions;
			interactions = 0;
			destroyBonds();
			return;
		}
		// K0.3b.LATER.1-10: payload fields store-backed; routed through
		// writeStoreXxx_ helpers (which call ensureStoreBinding_ for the
		// detached-handle case — though here store_ is already non-null).
		writeStorePosition_(Vector3(BALL_ATOM_DEFAULT_POSITION));
		writeStoreCharge_(BALL_ATOM_DEFAULT_CHARGE);
		writeStoreVelocity_(Vector3(BALL_ATOM_DEFAULT_VELOCITY));
		writeStoreForce_(Vector3(BALL_ATOM_DEFAULT_FORCE));
		writeStoreName_(String(BALL_ATOM_DEFAULT_NAME));
		writeStoreTypeName_(String(BALL_ATOM_DEFAULT_TYPE_NAME));
		writeStoreRadius_(BALL_ATOM_DEFAULT_RADIUS);
		writeStoreAtomType_(static_cast<short>(BALL_ATOM_DEFAULT_TYPE));
		writeStoreFormalCharge_(static_cast<short>(BALL_ATOM_DEFAULT_FORMAL_CHARGE));
		writeStoreElement_(BALL_ATOM_DEFAULT_ELEMENT);

		delete interactions;
		interactions = 0;

		destroyBonds();
	}

	void Atom::swapLastBond_(const Atom *atom)
	{
		for (int i = 0; i < number_of_bonds_; ++i)
		{
			if (bond_[i]->first_ == atom
					|| bond_[i]->second_ == atom)
			{
				bond_[i] = bond_[--number_of_bonds_];
				break;
			}
		}
	}

	void Atom::addInteraction(const Atom* atom, String interaction_type, double energy)
	{
		if(store_interactions_disabled_) return;
		if(interactions==NULL) interactions = new MolecularInteractions;
		interactions->addInteraction(atom, interaction_type, energy);
	}

	void Atom::addInteraction(String interaction_type, double energy)
	{
		if(store_interactions_disabled_) return;
		if(interactions==NULL) interactions = new MolecularInteractions;
		interactions->addInteraction(interaction_type, energy);
	}

	void Atom::disableStoreInteractions()
	{
		store_interactions_disabled_ = 1;
	}

	void Atom::enableStoreInteractions()
	{
		store_interactions_disabled_ = 0;
	}

# ifdef BALL_NO_INLINE_FUNCTIONS
#   include <BALL/KERNEL/atom.iC>
# endif

// Phase 5.1 carry-forward (CI run 25948256780 verification):
// Plan 05.1-07 added `template class BALL_EXPORT std::vector<Atom*>;`
// here to provide a single TU-local definition matching the extern
// declaration in atom.h:1010. That worked for vector3 (value-element)
// but broke atom (pointer-element) — libBALL.dll exported the symbol as
// a strong global; libVIEW.dll's cartoonModel.obj + editMode.obj
// implicit-instantiate the same template AND emit strong symbols
// (MSVC's `extern template class` doesn't reliably suppress pointer-
// element-vector implicit instantiation). Linker sees duplicates →
// LNK2005 in VIEW.dll's link. Revert this addition; atom.h's bare
// `extern template class` declaration stays. libVIEW's TUs implicit-
// instantiate as COMDATs (weak symbols), linker dedupes, no LNK2005.
// vector3 keeps Plan 05.1-07's pattern intact (value-element vectors
// don't have the same MSVC quirk).

} // namespace BALL
