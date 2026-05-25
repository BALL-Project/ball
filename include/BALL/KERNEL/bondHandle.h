// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// BALL v2.2 H3a (D-H3.3/D-H3.8): bond value-handle TYPE.
//
// The bond analog of AtomHandle: a value handle {MoleculeStore*, bond_idx,
// StableId} reading the MoleculeStore bond table (BondRecord + CSR). Through
// dual existence (H3-H3.x) it coexists with the v0 `Bond` object; at the H4
// flip `Bond` becomes the handle and BondHandle reconciles to it (D63 audit).
//
// IDENTITY / VALIDITY (D-H3.8): bond slots are tombstoned + reused via the
// bond free-list, so identity uses the per-bond StableId (a recycled bond slot
// draws a fresh id) — the same ABA-safe pattern as AtomHandle. BondHandle is a
// drop-in stable-id key (std::hash + operator< on {store, stable_id}).
//
// READ-ONLY: identity/validity + endpoint/order/type reads + the
// dual-existence getBond() bridge. Bond-table mutation goes through the store.
//

#ifndef BALL_KERNEL_BONDHANDLE_H
#define BALL_KERNEL_BONDHANDLE_H

#ifndef BALL_KERNEL_MOLECULESTORE_H
# include <BALL/KERNEL/moleculeStore.h>
#endif

#ifndef BALL_KERNEL_ATOMHANDLE_H
# include <BALL/KERNEL/atomHandle.h>
#endif

#include <cstdint>
#include <functional>

namespace BALL
{
	class Bond;

	/** Bond value handle (D-H3.3) — the bond analog of AtomHandle.

			Holds `{MoleculeStore*, u32 bond_idx, StableId stable_id}`. Identity is
			the captured bond `StableId`, ABA-safe across bond free-list reuse.
			Endpoint reads yield `AtomHandle`s. Non-polymorphic / no vtable.

			\ingroup KernelContainers
	*/
	class BALL_EXPORT BondHandle
	{
		public:

		/// Null handle.
		BondHandle() = default;

		/** Bind to bond slot `idx` in `store`, capturing the slot's current
				bond `StableId` (the ABA-safe identity).
		*/
		BondHandle(MoleculeStore& store, std::uint32_t idx)
			: store_(&store), idx_(idx), stable_id_(store.bond_stable_id(idx))
		{}

		/// True if this is the null handle (store == nullptr).
		bool isNull() const { return store_ == nullptr; }

		/// Non-null test (the D61 null-handle idiom).
		explicit operator bool() const { return store_ != nullptr; }

		/** Full validity: non-null, in-range, not tombstoned, and the slot's
				current bond StableId still matches the captured one. A
				released-then-reallocated bond slot draws a fresh id, so a stale
				alias returns false.
		*/
		bool isValid() const
		{
			return store_ != nullptr
				&& idx_ < store_->bond_count()
				&& !store_->is_bond_dead(idx_)
				&& store_->bond_stable_id(idx_) == stable_id_;
		}

		MoleculeStore* getStore() const { return store_; }
		std::uint32_t  getBondIndex() const { return idx_; }
		MoleculeStore::StableId getStableId() const { return stable_id_; }

		/// First endpoint (BondRecord.a) as an AtomHandle.
		AtomHandle getFirstAtom() const
		{ return isValid() ? AtomHandle(*store_, store_->bond(idx_).a) : AtomHandle(); }

		/// Second endpoint (BondRecord.b) as an AtomHandle.
		AtomHandle getSecondAtom() const
		{ return isValid() ? AtomHandle(*store_, store_->bond(idx_).b) : AtomHandle(); }

		/** The endpoint that is NOT `a` (the v0 Bond::getPartner / getBoundAtom).
				Null handle if `a` is not an endpoint of this bond.
		*/
		AtomHandle getPartner(const AtomHandle& a) const
		{
			if (!isValid() || !a) return AtomHandle();
			const BondRecord& r = store_->bond(idx_);
			if (a.getStoreIndex() == r.a) return AtomHandle(*store_, r.b);
			if (a.getStoreIndex() == r.b) return AtomHandle(*store_, r.a);
			return AtomHandle();
		}

		/// Bond order (BondRecord.order; integer value matches Bond::BondOrder).
		std::uint8_t getOrder() const
		{ return isValid() ? store_->bond(idx_).order : 0; }

		/// Bond type (BondRecord.type; integer value matches Bond::BondType).
		std::uint8_t getType() const
		{ return isValid() ? store_->bond(idx_).type : 0; }

		/** Dual-existence bridge: the v0 `Bond` backing this slot, or nullptr if
				the handle is null/invalid (identity-aware, so a stale handle into a
				recycled slot returns nullptr). At H4 there is no v0 object.
		*/
		Bond* getBond() const
		{ return isValid() ? store_->bond_back_ptr(idx_) : nullptr; }

		bool operator == (const BondHandle& o) const
		{ return store_ == o.store_ && idx_ == o.idx_ && stable_id_ == o.stable_id_; }
		bool operator != (const BondHandle& o) const { return !(*this == o); }

		/// Stable-id key ordering (D-H3.8): deterministic across runs.
		bool operator < (const BondHandle& o) const
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

// v2.2 H3a (D-H3.8): BondHandle is a drop-in stable-id key for the pointer-
// identity containers (re-keyed from Bond*).
namespace std
{
	template <> struct hash<BALL::BondHandle>
	{
		std::size_t operator () (const BALL::BondHandle& h) const noexcept
		{
			return std::hash<BALL::MoleculeStore::StableId>()(h.getStableId())
			     ^ (std::hash<const void*>()(h.getStore()) << 1);
		}
	};
}

#endif // BALL_KERNEL_BONDHANDLE_H
