// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// PRIVATE INTERNAL HEADER for MoleculeStore side-table state.
//
// Per v2.1 D31b (Codex R17 P17-7 DEBT + R17b P17b-N5 closure): this
// header is consumed ONLY by:
//   - source/KERNEL/moleculeStore.C
//   - source/CONCEPT/composite.C       (P1.3 side-table mirror wiring)
//   - source/CONCEPT/property.C        (P1.6 PropertyManager mirror)
//   - source/CONCEPT/selectable.C      (P1.8 Selectable mirror)
//   - test/SideTableParity_test.C      (parity assertions)
//
// Anyone else including this header WILL be caught by the CI grep
// gate in .github/workflows/ci-v2.yml. CompositeNode* MUST NOT leak
// into iterator headers (composite.h, composite.iC, *Iterator*.h).
// The leak boundary is the entire rationale for this file existing.
//
// Why a separate header rather than nested-private-class: PImpl with
// a forward-declared SideTables class requires the full type at
// destructor instantiation; defining it here lets moleculeStore.C
// (and the 3 wiring TUs) materialize the destructor without exposing
// the type via moleculeStore.h.

#ifndef BALL_KERNEL_MOLECULESTORE_INTERNAL_H
#define BALL_KERNEL_MOLECULESTORE_INTERNAL_H

#include <atomic>
#include <cstdint>
#include <memory>
#include <vector>

namespace BALL
{
	// v2.1 D22b: discriminator for the unified Composite node table.
	// Every Composite-derived class in the KERNEL hierarchy registers
	// its tag here. Atom is the dominant case at 100k+ per System;
	// non-Atom kinds are O(thousands) so the same node table amortises
	// across all of them.
	enum class CompositeKind : std::uint8_t
	{
		NONE                = 0,
		ATOM                = 1,
		BOND                = 2,
		FRAGMENT            = 3,
		RESIDUE             = 4,
		NUCLEOTIDE          = 5,
		SECONDARY_STRUCTURE = 6,
		CHAIN               = 7,
		MOLECULE            = 8,
		PROTEIN             = 9,
		NUCLEIC_ACID        = 10,
		SYSTEM              = 11,
	};

	// v2.1 D22b: 8-byte handle pointing into the side-table node array.
	// kind + 32-bit idx + 3 B padding. Null handle = {NONE, 0}.
	struct CompositeHandle
	{
		CompositeKind  kind = CompositeKind::NONE;
		std::uint8_t   _pad0 = 0;
		std::uint16_t  _pad1 = 0;
		std::uint32_t  idx   = 0;

		bool isNull() const { return kind == CompositeKind::NONE; }

		bool operator==(const CompositeHandle& o) const
		{ return kind == o.kind && idx == o.idx; }

		bool operator!=(const CompositeHandle& o) const
		{ return !(*this == o); }
	};
	static_assert(sizeof(CompositeHandle) == 8,
		"CompositeHandle must be 8 B for D22b memory math");

	// v2.1 D22b CompositeNode: 5-link topology (parent + first_child +
	// last_child + next_sibling + prev_sibling) + child_count + kind.
	// Bidirectional sibling links are required by current Composite
	// iterators (ChildCompositeIteratorTraits::backward uses
	// last_child_ + previous_; CompositeIteratorTraits::
	// findPreviousPosition uses both for reverse DFS). The 3-handle
	// design in original D22 was BLOCKER per R17 P17-1.
	//
	// Sizing: 5 × 8 B handle + 4 B child_count + 1 B kind = 45 B,
	// rounded to 48 B by alignment padding. At 100k Atoms,
	// composite_nodes_ takes 4.8 MB — net ~28 MB saved vs the inline
	// Composite topology + counters Atom currently carries.
	struct CompositeNode
	{
		CompositeHandle parent;
		CompositeHandle first_child;
		CompositeHandle last_child;
		CompositeHandle next_sibling;
		CompositeHandle prev_sibling;
		std::uint32_t   child_count = 0;
		CompositeKind   kind        = CompositeKind::NONE;
		std::uint8_t    _pad[3]     = {0, 0, 0};
	};
	static_assert(sizeof(CompositeNode) == 48,
		"CompositeNode must be 48 B for D22b memory math; check padding");

	// v2.1 D22b: a Composite's API-visible topology view, returned
	// by-value from Composite::getNode_() so iterator implementations
	// never need to name CompositeNode directly. Honours D31b
	// encapsulation: this struct CAN appear in iterator translation
	// units because it holds Composite* pointers, not node handles.
	class Composite; // fwd-decl
	struct CompositeTopologyView
	{
		Composite*    parent       = nullptr;
		Composite*    first_child  = nullptr;
		Composite*    last_child   = nullptr;
		Composite*    next_sibling = nullptr;
		Composite*    prev_sibling = nullptr;
		std::uint32_t child_count  = 0;
	};

	// v2.1 P1.2: side-table state owned by every MoleculeStore. PImpl
	// hides it from moleculeStore.h consumers. Only the 4 TUs listed
	// at the top of this header touch it directly.
	//
	// Sub-phases P1.4-P1.8 will extend this with:
	//   - PropertyColumnRegistry property_columns_
	//   - HashMap<atom_idx, PropertyBag> sparse_property_bag_
	//   - std::unique_ptr<std::atomic<uint64_t>[]> selected_bits_
	//   - std::size_t selected_bits_word_capacity_
	//
	// For P1.2, only composite_nodes_ + free list are populated.
	class MoleculeStoreSideTables
	{
		public:
		MoleculeStoreSideTables() = default;
		MoleculeStoreSideTables(const MoleculeStoreSideTables&)            = delete;
		MoleculeStoreSideTables& operator=(const MoleculeStoreSideTables&) = delete;

		// Unified composite-node table. Atoms / Bonds / Molecules /
		// Chains / Residues / Systems all allocate handles here. The
		// idx is per-kind, so handle.kind disambiguates between two
		// allocations both reporting idx=42.
		//
		// In P1.2/P1.3 these are populated by explicit
		// allocate_composite_node_ / release_composite_node_ calls.
		// P2 wires Composite mutations to maintain the table; P1.3
		// ships only the infrastructure, with the SideTableParity_test
		// exercising allocate/release round-trip.
		std::vector<CompositeNode>   composite_nodes_;
		std::vector<std::uint32_t>   composite_free_list_;

		// v2.1 P1.3 allocator: hands out a fresh CompositeHandle of the
		// given kind. The returned handle's idx is either pulled from
		// composite_free_list_ (recycled) or appended to composite_nodes_.
		// The new node's topology fields are all NONE/null.
		//
		// Thread-safety: caller is the owning MoleculeStore; D16
		// (single-thread per-store mutation) applies. No internal lock.
		CompositeHandle allocate_composite_node_(CompositeKind kind);

		// v2.1 P1.3 release: returns the node's slot to the free list
		// after zeroing its topology. Idempotent on NULL handle.
		void            release_composite_node_(CompositeHandle h);

		// v2.1 P1.3 accessor: returns a reference to the node for the
		// given handle. Asserts on NULL handle or out-of-range idx.
		// Reference invalidates on composite_nodes_ vector growth —
		// callers must not hold across allocate_composite_node_ calls.
		CompositeNode&       node_(CompositeHandle h);
		const CompositeNode& node_(CompositeHandle h) const;
	};

} // namespace BALL

#endif // BALL_KERNEL_MOLECULESTORE_INTERNAL_H
