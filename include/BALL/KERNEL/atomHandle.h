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

#include <cstdint>

namespace BALL
{
	class Atom;

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

		/// Identity equality (same store, slot, and captured stable id).
		bool operator == (const AtomHandle& o) const
		{
			return store_ == o.store_ && idx_ == o.idx_ && stable_id_ == o.stable_id_;
		}
		bool operator != (const AtomHandle& o) const { return !(*this == o); }

		private:

		MoleculeStore*           store_     = nullptr;
		std::uint32_t            idx_       = 0;
		MoleculeStore::StableId  stable_id_ = 0;
	};

} // namespace BALL

#endif // BALL_KERNEL_ATOMHANDLE_H
