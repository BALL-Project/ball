// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// BALL v2.2 H2c: atom value-handle TYPE (D71/D71a).
//
// The atom analog of ContainerHandleBase: a value handle
// {MoleculeStore*, atom_idx, StableId} reading the MoleculeStore atom SoA
// columns. It is the yield type of the H2c handle-yielding traversal
// (StructureQuery::atoms / apply). Through dual existence (H2c-H3) it
// coexists with the v0 `Atom` C++ object; at the H4 flip `Atom` itself
// becomes the handle and `AtomHandle` reconciles with it (H4 audit, see
// V22-H2-DESIGN.md D71a).
//
// IDENTITY / VALIDITY (D71a): atom slots have only the store-wide
// generation() (D7), and release_atom() does NOT bump it, so a coarse
// generation check would ABA-alias a recycled slot. Each live atom has a
// unique StableId (a reallocated slot draws a fresh one), so AtomHandle
// captures stable_id at bind and compares it on isValid() -- the ABA-safe
// atom identity.
//
// READ-ONLY: H2c exposes identity + validity + the dual-existence
// getAtom() bridge. Mutators / consumer migration are H3.
//

#ifndef BALL_KERNEL_ATOMHANDLE_H
#define BALL_KERNEL_ATOMHANDLE_H

#ifndef BALL_KERNEL_MOLECULESTORE_H
# include <BALL/KERNEL/moleculeStore.h>
#endif

#ifndef BALL_KERNEL_PTE_H
# include <BALL/KERNEL/PTE.h>           // H3a.3b: getElement/setElement use the PTE table
#endif

#ifndef BALL_KERNEL_CONTAINERHANDLE_H
# include <BALL/KERNEL/containerHandle.h>  // H3a.3b.2: nav returns ContainerHandle/*Handle
#endif

#include <cstdint>
#include <vector>
#include <functional>

namespace BALL
{
	class Atom;
	class BondHandle;   // H3a (D-H3.2): getBond()/bonds() return BondHandle;
	                    // defined out-of-line in bondHandle.h to break the cycle.

	/** Atom value handle (D71a) -- the atom analog of ContainerHandleBase.

			Holds `{MoleculeStore*, u32 atom_idx, StableId stable_id}`. A
			default-constructed handle is the NULL handle (`store_ == nullptr`,
			`operator bool` false). Non-polymorphic / no vtable.

			Identity is the captured `StableId`, which is ABA-safe across
			`release_atom()` + slot reuse (a reallocated slot draws a fresh
			stable id). 24 B; the D13 16 B target applies to the H4 flipped
			`Atom`, not this transient traversal handle.

			\ingroup KernelContainers
	*/
	class BALL_EXPORT AtomHandle
	{
		public:

		/// Null handle.
		AtomHandle() = default;

		/** Bind to atom slot `idx` in `store`, capturing the slot's current
				`StableId` (the ABA-safe identity).
		*/
		AtomHandle(MoleculeStore& store, std::uint32_t idx)
			: store_(&store), idx_(idx), stable_id_(store.stable_id(idx))
		{}

		/// True if this is the null handle (store == nullptr).
		bool isNull() const { return store_ == nullptr; }

		/// Non-null test (the D61 null-handle idiom: `if (h) ...`).
		explicit operator bool() const { return store_ != nullptr; }

		/** Full validity (D71a): non-null, in-range, not freed, and the slot's
				current `StableId` still matches the one captured at binding. A
				released-then-reallocated slot draws a fresh stable id, so a stale
				alias returns false here.
		*/
		bool isValid() const
		{
			return store_ != nullptr
				&& idx_ < store_->size()
				&& !store_->is_freed(idx_)
				&& store_->stable_id(idx_) == stable_id_;
		}

		/// Owning store (nullptr for the null handle).
		MoleculeStore* getStore() const { return store_; }

		/// Atom SoA slot index.
		std::uint32_t getStoreIndex() const { return idx_; }

		/// The captured ABA-safe identity.
		MoleculeStore::StableId getStableId() const { return stable_id_; }

		/** Dual-existence bridge: the v0 `Atom` backing this slot, or nullptr
				if the handle is null / invalid. Identity-aware (H2c-CR fix): gated
				on isValid(), so a stale handle into a released-then-reallocated slot
				returns nullptr rather than ABA-aliasing the recycled slot's new
				atom. (At the H4 flip there is no v0 object and this returns nullptr
				/ is removed.)
		*/
		Atom* getAtom() const
		{
			return isValid() ? store_->back_ptr(idx_) : nullptr;
		}

		// --- v2.2 H3a (D-H3.2): migration shim — the Atom consumer surface,
		// forwarding to the same MoleculeStore SoA columns the v0 Atom reads /
		// writes (zero-copy; the column is the single source of truth). Read/
		// write on a VALID handle; UB-on-misuse, the same contract class as the
		// MoleculeStore reference getters and the other v2.2 handles. (element
		// getters, hierarchy nav, and the property-visitor surface land in a
		// follow-on H3a.3b — they need PTE / role-walk / property-column infra.)
		const Vector3& getPosition() const          { return store_->position(idx_); }
		void           setPosition(const Vector3& v) { store_->position(idx_) = v; }
		const Vector3& getForce() const              { return store_->force(idx_); }
		void           setForce(const Vector3& f)    { store_->force(idx_) = f; }
		const Vector3& getVelocity() const           { return store_->velocity(idx_); }
		void           setVelocity(const Vector3& v) { store_->velocity(idx_) = v; }
		float          getCharge() const             { return store_->charge(idx_); }
		void           setCharge(float c)            { store_->charge(idx_) = c; }
		float          getRadius() const             { return store_->radius(idx_); }
		void           setRadius(float r)            { store_->radius(idx_) = r; }
		short          getType() const               { return store_->atom_type(idx_); }
		void           setType(short t)              { store_->atom_type(idx_) = t; }
		short          getFormalCharge() const       { return store_->formal_charge(idx_); }
		void           setFormalCharge(short fc)     { store_->formal_charge(idx_) = fc; }
		String         getName() const               { return store_->name(idx_); }
		void           setName(const String& s)      { store_->set_name(idx_, s); }
		String         getTypeName() const           { return store_->type_name(idx_); }
		void           setTypeName(const String& s)  { store_->set_type_name(idx_, s); }

		// Element via PTE (matches v0 Atom::getElement/setElement: store stores
		// the uint8 atomic number; PTE resolves to the static Element ref).
		const Element& getElement() const            { return PTE[(Position)store_->element_index(idx_)]; }
		void           setElement(const Element& e)  { store_->element_index(idx_) = static_cast<std::uint8_t>(e.getAtomicNumber()); }

		// Selection (per-atom bit; container subtree counters are mirrored by
		// the v0 select/deselect path during dual existence).
		bool isSelected() const { return store_->selected(idx_); }
		void select()           { store_->set_selected(idx_, true); }
		void deselect()         { store_->set_selected(idx_, false); }

		// Bonds (D-H3.3): degree via the store CSR; the BondHandle-yielding
		// getBond(i) / bonds() are defined out-of-line in bondHandle.h (which
		// has both AtomHandle and BondHandle complete).
		Size countBonds() const { return static_cast<Size>(store_->bond_degree(idx_)); }
		BondHandle getBond(Size i) const;
		std::vector<BondHandle> bonds() const;

		// --- v2.2 H3a.3b.2 (D-H3.2): hierarchy nav — walk up the atom's
		// container-row parent chain filtering by role. Mirrors v0
		// Atom::getParent / getResidue / getChain / getMolecule / getFragment /
		// getSecondaryStructure. Each returns a null handle if no ancestor of
		// that role exists. The walk uses ContainerHandleBase's public parent
		// API, so the atom -> container-row entry is the only D66a-clean store
		// accessor needed (atom_parent_container_idx, defined above).
		ContainerHandleBase getParent() const
		{
			if (!isValid()) return ContainerHandleBase();
			std::uint32_t p = store_->atom_parent_container_idx(idx_);
			if (p == MoleculeStore::CONTAINER_NONE) return ContainerHandleBase();
			return ContainerHandleBase(*store_, p);
		}

		/// Immediate fragment-kind ancestor (the v0 Atom::getFragment target).
		FragmentHandle getFragment() const
		{
			ContainerHandleBase p = getParent();
			while (p)
			{
				FragmentHandle f = p.as<FragmentHandle>();
				if (f) return f;
				p = p.getParent();
			}
			return FragmentHandle();
		}

		/// First RESIDUE-role fragment ancestor (v0 Atom::getResidue).
		FragmentHandle getResidue() const
		{
			ContainerHandleBase p = getParent();
			while (p)
			{
				FragmentHandle f = p.as<FragmentHandle>();
				if (f && f.getFragmentRole() == FragmentRole::RESIDUE) return f;
				p = p.getParent();
			}
			return FragmentHandle();
		}

		/// First CHAIN-role fragment ancestor (v0 Atom::getChain).
		FragmentHandle getChain() const
		{
			ContainerHandleBase p = getParent();
			while (p)
			{
				FragmentHandle f = p.as<FragmentHandle>();
				if (f && f.getFragmentRole() == FragmentRole::CHAIN) return f;
				p = p.getParent();
			}
			return FragmentHandle();
		}

		/// First SECONDARY_STRUCTURE-role fragment ancestor (v0 Atom::getSecondaryStructure).
		FragmentHandle getSecondaryStructure() const
		{
			ContainerHandleBase p = getParent();
			while (p)
			{
				FragmentHandle f = p.as<FragmentHandle>();
				if (f && f.getFragmentRole() == FragmentRole::SECONDARY_STRUCTURE) return f;
				p = p.getParent();
			}
			return FragmentHandle();
		}

		/// Top-level molecule ancestor (v0 Atom::getMolecule).
		MoleculeHandle getMolecule() const
		{
			ContainerHandleBase p = getParent();
			while (p)
			{
				MoleculeHandle m = p.as<MoleculeHandle>();
				if (m) return m;
				p = p.getParent();
			}
			return MoleculeHandle();
		}

		/// Identity equality (same store, slot, and captured stable id).
		bool operator == (const AtomHandle& o) const
		{
			return store_ == o.store_ && idx_ == o.idx_ && stable_id_ == o.stable_id_;
		}
		bool operator != (const AtomHandle& o) const { return !(*this == o); }

		/// Stable-id key ordering (D-H3.8): deterministic across runs.
		bool operator < (const AtomHandle& o) const
		{
			if (store_ != o.store_) return store_ < o.store_;
			return stable_id_ < o.stable_id_;
		}

		private:

		MoleculeStore*           store_     = nullptr;
		std::uint32_t            idx_       = 0;
		MoleculeStore::StableId  stable_id_ = 0;
	};

} // namespace BALL

// v2.2 H3a (D-H3.8): AtomHandle is a drop-in stable-id key for the pointer-
// identity containers re-keyed from Atom* (Selector/MolecularGraph/SMARTS/...).
namespace std
{
	template <> struct hash<BALL::AtomHandle>
	{
		std::size_t operator () (const BALL::AtomHandle& h) const noexcept
		{
			return std::hash<BALL::MoleculeStore::StableId>()(h.getStableId())
			     ^ (std::hash<const void*>()(h.getStore()) << 1);
		}
	};
}

#endif // BALL_KERNEL_ATOMHANDLE_H
