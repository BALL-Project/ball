// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/moleculeStore.h>   // K0.4.1: full type for unique_ptr<MoleculeStore>
#include <BALL/KERNEL/atom.h>            // K0.4.2: adopt(Atom&)
#include <BALL/KERNEL/bond.h>            // K0.4.2: bond migration in adopt
#include <string>                        // K0.4.2: std::string for name copy

namespace BALL
{

	System::System()
		:	AtomContainer(),
			store_(std::make_unique<MoleculeStore>())   // K0.4.1
	{
	}

	System::System(const System& system, bool deep)
		: AtomContainer(),
			store_(std::make_unique<MoleculeStore>())   // K0.4.1
	{
		set(system, deep);
	}

	System::System(const String& name)
		:	AtomContainer(name),
			store_(std::make_unique<MoleculeStore>())   // K0.4.1
	{
	}

	MoleculeStore& System::getStore()             { return *store_; }
	const MoleculeStore& System::getStore() const { return *store_; }

	// K0.4.2: System.adopt(Atom&) — migrates atom from current store
	// (typically orphan) into this System's per-instance store.
	//
	// K0.4.5 (Codex Round 4 HIGH-3): single-atom adopt is ONLY safe for
	// atoms with zero bonds, or whose bonds' partners are already in the
	// target store. Adopting an atom whose bond partners are in a third
	// store (typically the orphan store) would orphan those bonds: the
	// source slot is released, its bond records have a freed endpoint,
	// and CSR filters them out — the bond becomes invisible to the
	// partner's future adopt(). Callers with mixed-store bonded atoms
	// must use adoptSubtree(AtomContainer&), which migrates atoms in
	// three batched passes that keep bond endpoints reachable.
	//
	// Sequence:
	//   1. Reject if atom has bonds whose partners aren't in dst (logged
	//      warning + early return; no state change)
	//   2. Snapshot all atom payload columns from the source store
	//   3. allocate_atom(&atom) in target store (atomic bind, K0.3c.8)
	//   4. Copy payload to new slot
	//   5. Migrate the (now-safe) bonds whose partners ARE in dst
	//   6. release_atom(source slot)
	//   7. Update atom.store_/store_idx_/store_generation_
	void System::adopt(Atom& atom)
	{
		MoleculeStore* src = atom.getStore();
		MoleculeStore* dst = store_.get();
		if (src == dst) return;          // already in this System's store
		if (src == nullptr) return;      // unbound atom (shouldn't happen post-K0.3b.1)

		// K0.4.5: bonded-atom safety gate. If any bond's partner lives in
		// a store other than dst, the bond would be orphaned when src
		// released the source slot. Refuse and tell the caller to use
		// adoptSubtree instead.
		for (Size bi = 0; bi < atom.countBonds(); ++bi)
		{
			const Atom* partner = atom.getPartnerAtom(bi);
			if (partner == nullptr) continue;
			MoleculeStore* psrc = partner->getStore();
			if (psrc == dst) continue;   // safe: partner already in dst
			Log.warn() << "System::adopt(Atom&): refusing to adopt bonded "
				"atom whose partner lives in another store; use "
				"adoptSubtree(AtomContainer&) instead." << std::endl;
			return;
		}

		const std::uint32_t src_idx = atom.getStoreIndex();

		// 1+2+3: snapshot + atomic alloc + copy
		Vector3 pos     = src->position(src_idx);
		Vector3 vel     = src->velocity(src_idx);
		Vector3 force   = src->force(src_idx);
		float   charge  = src->charge(src_idx);
		float   radius  = src->radius(src_idx);
		short   atype   = src->atom_type(src_idx);
		short   fcharge = src->formal_charge(src_idx);
		std::uint8_t elem = src->element_index(src_idx);
		bool    sel     = src->selected(src_idx);
		std::string name = src->get_name(src_idx);
		std::string tname = src->get_type_name(src_idx);

		const std::uint32_t dst_idx = dst->allocate_atom(&atom);   // atomic bind

		dst->position(dst_idx)       = pos;
		dst->velocity(dst_idx)       = vel;
		dst->force(dst_idx)          = force;
		dst->charge(dst_idx)         = charge;
		dst->radius(dst_idx)         = radius;
		dst->atom_type(dst_idx)      = atype;
		dst->formal_charge(dst_idx)  = fcharge;
		dst->element_index(dst_idx)  = elem;
		dst->set_selected(dst_idx, sel);
		dst->set_name(dst_idx, name);
		dst->set_type_name(dst_idx, tname);

		// 4: bond migration. Iterate bonds touching src atom (snapshot-
		// safe via K0.3c.10). For each bond, look at its endpoints:
		// if the OTHER endpoint is already in `dst` (because dst is this
		// System's store and the partner was adopted earlier), migrate
		// the bond to dst. Otherwise leave it in src.
		src->for_each_bond_of(src_idx, [&](std::uint32_t bond_idx) {
			const BondRecord& br = src->bond(bond_idx);
			std::uint32_t partner_src_idx =
				(br.a == src_idx) ? br.b : br.a;
			Atom* partner = src->back_ptr(partner_src_idx);
			if (partner == nullptr) return;   // partner already freed; skip
			if (partner->getStore() != dst) {
				// Partner still in src store; bond stays where it is.
				// The Bond object's bond_store_ continues to point at src.
				// When partner adopts, that call will re-find this bond
				// (still live in src) and migrate it then.
				return;
			}
			// Partner already in dst. Migrate the bond.
			std::uint8_t order = br.order;
			std::uint8_t type  = br.type;
			Bond* bond_obj = src->bond_back_ptr(bond_idx);
			std::uint32_t new_bond_idx = dst->add_bond(
				dst_idx, partner->getStoreIndex(), order, type);
			if (bond_obj != nullptr) {
				dst->set_bond_back_ptr(new_bond_idx, bond_obj);
				// Update Bond's store pointer + record index so future
				// setOrder/setType target the new mirror.
				bond_obj->bond_store_ = dst;
				bond_obj->bond_record_idx_ = new_bond_idx;
			}
			src->remove_bond(bond_idx);
		});

		// 5: release source slot
		src->release_atom(src_idx);

		// 6: update Atom handle
		// Reach in via friend or by exposed mutator. For simplicity
		// expose bindToStore_ semantics; we already wrote to back_ptr
		// in allocate_atom(&atom), so just rewrite the public state.
		// Atom has no public setStore — use the friend pattern: System
		// is friend of Atom (need to add). For K0.4.2 minimal, we
		// expose a non-public migrateTo_ on Atom and friend System.
		atom.migrateTo_(dst, dst_idx);
	}

	// K0.4.3: System.adoptSubtree(AtomContainer&) — three-pass batch
	// adoption that preserves bonds. Closes the K0.4.2 limitation where
	// sequential single-atom adopt() orphaned bonds when both endpoints
	// were inserted separately.
	//
	// Pass 1 (snapshot + allocate dst slots): for each Atom in the
	//   subtree, snapshot its payload from src, allocate a dst slot
	//   atomically with back_ptr=&atom, copy payload columns. Record
	//   each atom's old (src, src_idx) tuple in a local table.
	//
	// Pass 2 (bond migration): for each (atom, src, src_idx), iterate
	//   the bonds incident in src (mutation-safe via K0.3c.10). For
	//   each bond:
	//     - If the partner atom is also in dst (either because it was
	//       adopted in pass 1 or because it was already in dst from a
	//       prior insert): re-add the bond in dst, update bond_back_ptr
	//       and Bond's bond_store_/bond_record_idx_, then tombstone the
	//       original in src.
	//     - Else (partner in another store): leave the bond in src.
	//
	// Pass 3 (release src slots): for each (src, src_idx), release the
	//   slot. By this point, all bonds touching it have either migrated
	//   to dst (and been removed from src) or stayed in src with the
	//   slot's partner still live.
	void System::adoptSubtree(AtomContainer& container)
	{
		MoleculeStore* dst = store_.get();

		// Collect atoms + their old (src, src_idx) before we mutate.
		struct AtomEntry { Atom* atom; MoleculeStore* src; std::uint32_t src_idx; };
		std::vector<AtomEntry> entries;

		AtomIterator it = container.beginAtom();
		for (; +it; ++it)
		{
			Atom& a = *it;
			MoleculeStore* src = a.getStore();
			if (src == dst) continue;          // already in our store
			if (src == nullptr) continue;      // unbound
			entries.push_back({&a, src, a.getStoreIndex()});
		}

		// Pass 1: snapshot + allocate dst slots + copy payloads.
		std::vector<std::uint32_t> dst_indices;
		dst_indices.reserve(entries.size());
		for (const auto& e : entries)
		{
			Vector3 pos = e.src->position(e.src_idx);
			Vector3 vel = e.src->velocity(e.src_idx);
			Vector3 force = e.src->force(e.src_idx);
			float ch = e.src->charge(e.src_idx);
			float r = e.src->radius(e.src_idx);
			short at = e.src->atom_type(e.src_idx);
			short fc = e.src->formal_charge(e.src_idx);
			std::uint8_t el = e.src->element_index(e.src_idx);
			bool sel = e.src->selected(e.src_idx);
			std::string nm = e.src->get_name(e.src_idx);
			std::string tn = e.src->get_type_name(e.src_idx);

			std::uint32_t di = dst->allocate_atom(e.atom);
			dst->position(di) = pos;
			dst->velocity(di) = vel;
			dst->force(di) = force;
			dst->charge(di) = ch;
			dst->radius(di) = r;
			dst->atom_type(di) = at;
			dst->formal_charge(di) = fc;
			dst->element_index(di) = el;
			dst->set_selected(di, sel);
			dst->set_name(di, nm);
			dst->set_type_name(di, tn);

			e.atom->migrateTo_(dst, di);
			dst_indices.push_back(di);
		}

		// Pass 2: bond migration. By now, every atom in entries has
		// atom->getStore() == dst. So partner.getStore() == dst means
		// the partner is migratable.
		for (std::size_t k = 0; k < entries.size(); ++k)
		{
			const auto& e = entries[k];
			// Iterate bonds incident to the OLD src slot.
			e.src->for_each_bond_of(e.src_idx, [&](std::uint32_t bond_idx) {
				const BondRecord& br = e.src->bond(bond_idx);
				std::uint32_t partner_src_idx = (br.a == e.src_idx) ? br.b : br.a;
				Atom* partner = e.src->back_ptr(partner_src_idx);
				if (partner == nullptr) return;     // partner already freed; skip
				if (partner->getStore() != dst) return;  // partner not in dst yet

				std::uint8_t order = br.order;
				std::uint8_t type  = br.type;
				Bond* bond_obj = e.src->bond_back_ptr(bond_idx);

				std::uint32_t new_bond_idx = dst->add_bond(
					dst_indices[k], partner->getStoreIndex(), order, type);
				if (bond_obj != nullptr)
				{
					dst->set_bond_back_ptr(new_bond_idx, bond_obj);
					bond_obj->bond_store_ = dst;
					bond_obj->bond_record_idx_ = new_bond_idx;
				}
				e.src->remove_bond(bond_idx);
			});
		}

		// Pass 3: release src slots.
		for (const auto& e : entries)
		{
			e.src->release_atom(e.src_idx);
		}
	}

  void System::persistentWrite(PersistenceManager& pm, const char* name) const
  {
    pm.writeObjectHeader(this, name);
		AtomContainer::persistentWrite(pm);
    pm.writeObjectTrailer(name);
	}

  void System::persistentRead(PersistenceManager& pm)
  {
    pm.checkObjectHeader(RTTI::getStreamName<AtomContainer>());
		AtomContainer::persistentRead(pm);
    pm.checkObjectTrailer(0);
	}

	System::~System()
	{
		destroy();
		// K0.4.2: invalidate atom handles bound to our store before the
		// unique_ptr<MoleculeStore> deletes the store. Otherwise atoms
		// whose lifetime outlasts the System (e.g., test fixtures with
		// `Atom a; System sys; sys.adopt(a);`) would dereference a
		// freed store at ~Atom -> release_atom.
		if (store_ != nullptr)
		{
			for (std::size_t i = 0; i < store_->size(); ++i)
			{
				if (!store_->is_freed(i))
				{
					Atom* handle = store_->back_ptr(i);
					if (handle != nullptr)
					{
						// Severs the handle's tie to this store. ~Atom
						// will see nullptr and skip release_atom.
						handle->migrateTo_(nullptr, 0);
					}
				}
			}
		}
	}
		
	void System::set(const System& system, bool deep)
	{
		AtomContainer::set(system, deep);
	}
			
	System& System::operator = (const System& system)
	{
		AtomContainer::set(system);
		return *this;
	}

	void System::get(System& system, bool deep) const
	{
		system.set(*this, deep);
	}

	Molecule* System::getMolecule(Position position)
	{
		for (MoleculeIterator res_it = beginMolecule(); !res_it.isEnd(); ++res_it)
		{
			if (position-- == 0)
			{
				return &(*res_it);
			}
		}

		return 0;
	}

	const Molecule* System::getMolecule(Position position) const
	{
		return ((System *)this)->getMolecule(position);
	}

	Protein* System::getProtein(Position position)
	{
		for (ProteinIterator res_it = beginProtein(); !res_it.isEnd(); ++res_it)
		{
			if (position-- == 0)
			{
				return &(*res_it);
			}
		}

		return 0;
	}

	const Protein* System::getProtein(Position position) const
	{
		return ((System *)this)->getProtein(position);
	}

	Size System::countMolecules() const
	{
		Size size = 0;

		for (MoleculeConstIterator mol_it = beginMolecule(); !mol_it.isEnd(); ++mol_it)
		{
			++size;
		}

		return size;
	}

	Size  System::countFragments() const
	{
		Size size = 0;

		for (FragmentConstIterator frag_it = beginFragment(); !frag_it.isEnd(); ++frag_it)
		{
			++size;
		}

		return size;
	}

	Size  System::countResidues() const
	{
		Size size = 0;

		for (ResidueConstIterator frag_it = beginResidue(); !frag_it.isEnd(); ++frag_it)
		{
			++size;
		}

		return size;
	}

	Size  System::countAtoms() const
	{
		Size size = 0;

		for (AtomConstIterator frag_it = beginAtom(); !frag_it.isEnd(); ++frag_it)
		{
			++size;
		}

		return size;
	}

	Size  System::countProteins() const
	{
		Size size = 0;

		for (ProteinConstIterator frag_it = beginProtein(); !frag_it.isEnd(); ++frag_it)
		{
			++size;
		}

		return size;
	}

	Size  System::countChains() const
	{
		Size size = 0;

		for (ChainConstIterator frag_it = beginChain(); !frag_it.isEnd(); ++frag_it)
		{
			++size;
		}

		return size;
	}

	Size  System::countSecondaryStructures() const
	{
		Size size = 0;

		for (SecondaryStructureConstIterator frag_it = beginSecondaryStructure(); !frag_it.isEnd(); ++frag_it)
		{
			++size;
		}

		return size;
	}

	Size  System::countNucleotides() const
	{
		Size size = 0;

		for (NucleotideConstIterator frag_it = beginNucleotide(); !frag_it.isEnd(); ++frag_it)
		{
			++size;
		}

		return size;
	}

	Size  System::countNucleicAcids() const
	{
		Size size = 0;

		for (NucleicAcidConstIterator frag_it = beginNucleicAcid(); !frag_it.isEnd(); ++frag_it)
		{
			++size;
		}

		return size;
	}

	// K0.4.3: delegate to AtomContainer overloads so the auto-adopt
	// hooks fire. Molecule is-a AtomContainer; the AtomContainer base
	// methods handle the Composite::*Child wiring + adoptSubtree call.
	void System::prepend(Molecule& molecule)
	{
		AtomContainer::prepend(static_cast<AtomContainer&>(molecule));
	}

	void System::append(Molecule& molecule)
	{
		AtomContainer::append(static_cast<AtomContainer&>(molecule));
	}

	void System::insert(Molecule& molecule)
	{
		append(molecule);
	}

	void System::insertBefore(Molecule& molecule, Composite& before)
	{
		AtomContainer::insertBefore(static_cast<AtomContainer&>(molecule), before);
	}

	void System::insertAfter(Molecule& molecule, Composite& after)
	{
		AtomContainer::insertAfter(static_cast<AtomContainer&>(molecule), after);
	}

	bool System::remove(Molecule& molecule)
	{
		return Composite::removeChild(molecule);
	}

	void System::spliceBefore(System& system)
	{
		Composite::spliceBefore(system);
	}

	void System::spliceAfter(System& system)
	{
		Composite::spliceAfter(system);
	}

	void System::splice(System& system)
	{
		Composite::splice(system);
	}

	bool System::operator == (const System& system) const
	{
		return(Object::operator == (system));
	}

	bool System::operator != (const System& system) const
	{
		return !(*this == system);
	}

} // namespace BALL
