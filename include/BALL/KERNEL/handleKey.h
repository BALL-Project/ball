// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// v2.2 H4 7b.9-7b.16 (D-H3.8): handle-as-container-key support.
//
// The gate-surfaced STRUCTURE consumers store raw `Atom*` / `Bond*` as
// keys/elements of hash/tree/sequence containers -- pointer identity,
// which the v2.2 stable-id policy forbids (a released-then-reallocated
// slot re-churns the pointer; handles re-validate via stable id). This
// header is the single opt-in include those consumers use to re-key on
// `AtomHandle` / `BondHandle` instead:
//
//   * BALL::hash_value(const AtomHandle&) / hash_value(const BondHandle&)
//     -- BALL's HashMap/HashSet are boost::unordered_map/set and hash via
//     boost::hash, whose ADL customization point is a `hash_value(key)`
//     free function. A 24 B handle has no built-in boost hash, so these
//     overloads are required for `HashMap<AtomHandle, V>` /
//     `HashSet<BondHandle>` to compile. They hash the ABA-safe stable id
//     (xor the store ptr), the same basis as the std::hash specializations
//     in atomHandle.h / bondHandle.h (which cover std::unordered_*;
//     operator< covers std::set/std::map).
//
//   * BALL::makeHandle(const Atom&) -> AtomHandle, makeHandle(const Bond&)
//     -> BondHandle -- null-safe Atom*/Bond* -> handle conversion at the
//     container insert boundary. The reverse (handle -> Atom*/Bond*) is
//     AtomHandle::getAtom() / BondHandle::getBond(), already on the
//     handles. An unbound atom/bond (no store) yields a null handle.
//
// Post-H4 (facade deletion) the makeHandle(Atom&)/(Bond&) factories go
// away with the v0 objects; consumers already hold handles by then.

#ifndef BALL_KERNEL_HANDLEKEY_H
#define BALL_KERNEL_HANDLEKEY_H

#ifndef BALL_KERNEL_ATOMHANDLE_H
#	include <BALL/KERNEL/atomHandle.h>
#endif

#ifndef BALL_KERNEL_BONDHANDLE_H
#	include <BALL/KERNEL/bondHandle.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
#	include <BALL/KERNEL/atom.h>
#endif

#ifndef BALL_KERNEL_BOND_H
#	include <BALL/KERNEL/bond.h>
#endif

#ifndef BALL_COMMON_HASH_H
#	include <BALL/COMMON/hash.h>     // BALL::Hash + HashIndex for HashSet
#endif

#include <cstddef>

namespace BALL
{
	// BALL has TWO distinct hash-customization points:
	//   * HashMap = boost::unordered_map -> boost::hash -> hash_value(key)
	//   * HashSet = BALL's own table     -> (HashIndex)Hash(key)
	// Both must be provided so a handle can key either container. Both
	// hash the ABA-safe stable id mixed with the store pointer (the same
	// basis as the std::hash<*Handle> specializations).

	/** boost::hash ADL point for AtomHandle (BALL HashMap keys). */
	inline std::size_t hash_value(const AtomHandle& h)
	{
		return std::hash<BALL::MoleculeStore::StableId>()(h.getStableId())
		     ^ (std::hash<const void*>()(h.getStore()) << 1);
	}
	/** boost::hash ADL point for BondHandle (BALL HashMap keys). */
	inline std::size_t hash_value(const BondHandle& h)
	{
		return std::hash<BALL::MoleculeStore::StableId>()(h.getStableId())
		     ^ (std::hash<const void*>()(h.getStore()) << 1);
	}

	/** BALL::Hash overload for AtomHandle (BALL HashSet keys). */
	inline HashIndex Hash(const AtomHandle& h)
	{
		return static_cast<HashIndex>(hash_value(h));
	}
	/** BALL::Hash overload for BondHandle (BALL HashSet keys). */
	inline HashIndex Hash(const BondHandle& h)
	{
		return static_cast<HashIndex>(hash_value(h));
	}

	/** Null-safe Atom* -> AtomHandle at a container-insert boundary.
			Returns a null handle for an unbound atom (no store). */
	inline AtomHandle makeHandle(const Atom& a)
	{
		MoleculeStore* s = a.getStore();
		return s ? AtomHandle(*s, a.getStoreIndex()) : AtomHandle();
	}

	/** Null-safe Bond* -> BondHandle. The bond's store/record index are
			transitional public members during dual existence (bond.h); an
			unmirrored bond (bond_store_ == null) yields a null handle. */
	inline BondHandle makeHandle(const Bond& b)
	{
		MoleculeStore* s = b.bond_store_;
		return s ? BondHandle(*s, b.bond_record_idx_) : BondHandle();
	}
}

#endif // BALL_KERNEL_HANDLEKEY_H
