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
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
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
	// kind + 32-bit idx + 3 B padding. Null handle = idx == 0 (idx 0
	// is reserved as a sentinel; the side table's composite_nodes_
	// vector preallocates a dummy entry at slot 0 so allocate_composite_node_
	// returns idx >= 1).
	//
	// Why idx-based null check instead of kind-based: Composite
	// subclasses that don't override compositeKindForSideTable_()
	// (in v2.1, all non-Atom kinds — Molecule, Chain, Residue, ...)
	// get allocated with kind=NONE. A kind-based null check would
	// mark their (valid) handles as null. The idx-based check works
	// regardless of kind. (P2.1.1 fix; D22b sizing math unchanged.)
	struct CompositeHandle
	{
		CompositeKind  kind = CompositeKind::NONE;
		std::uint8_t   _pad0 = 0;
		std::uint16_t  _pad1 = 0;
		std::uint32_t  idx   = 0;

		bool isNull() const { return idx == 0; }

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

	// v2.1 P1.4 (D23b): property column infrastructure. The store-side
	// replacement for the v2.0 per-atom PropertyManager bag. Force-
	// field-typical workloads bulk-assign properties (atom typing,
	// charge assignment), so per-property-name columns are the right
	// shape, not sparse per-atom bags. See D23b for rationale.
	//
	// Type set: BOOL / INT / UNSIGNED_INT / FLOAT / DOUBLE / STRING.
	// OBJECT / SMART_OBJECT live in the sparse fallback (P1.5).

	enum class PropertyColumnType : std::uint8_t
	{
		NONE         = 0,
		BOOL         = 1,
		INT          = 2,
		UNSIGNED_INT = 3,
		FLOAT        = 4,
		DOUBLE       = 5,
		STRING       = 6,
	};

	// Abstract base. Carries the per-row presence bitmap (1 bit per
	// atom) used to disambiguate "not set" from "set to default value".
	// Set/get/clear typed access goes through PropertyColumn<T>.
	class PropertyColumnBase
	{
		public:
		virtual ~PropertyColumnBase() = default;

		PropertyColumnType type() const            { return type_; }
		const std::string& name() const            { return name_; }
		std::size_t        capacity() const        { return size_; }

		// Per-row presence query.
		bool isSet(std::uint32_t i) const
		{
			if (i >= size_) return false;
			return (present_[i >> 3] & (std::uint8_t(1) << (i & 7))) != 0;
		}

		// Reset row presence without touching typed data (caller's
		// PropertyColumn<T> resets the value too via clear()).
		void clearPresence_(std::uint32_t i)
		{
			if (i >= size_) return;
			present_[i >> 3] &= std::uint8_t(~(std::uint8_t(1) << (i & 7)));
		}

		void setPresence_(std::uint32_t i)
		{
			ensureCapacity_(i + 1);
			present_[i >> 3] |= std::uint8_t(1) << (i & 7);
		}

		// Grow the present_ bitmap (and underlying typed storage in
		// PropertyColumn<T>) to hold at least `n` rows. Caller passes
		// a callback that resizes typed storage. The virtual is
		// implemented in PropertyColumn<T>.
		virtual void resize(std::size_t n) = 0;

		// Typed clear — PropertyColumn<T> resets the value AND
		// clears the presence bit.
		virtual void clear(std::uint32_t i) = 0;

		protected:
		PropertyColumnBase(PropertyColumnType t, std::string n)
			: type_(t), name_(std::move(n))
		{}

		void ensureCapacity_(std::size_t n)
		{
			if (n <= size_) return;
			std::size_t bytes = (n + 7) >> 3;
			if (bytes > present_.size()) present_.resize(bytes, 0);
			size_ = n;
		}

		PropertyColumnType        type_;
		std::string               name_;
		std::size_t               size_ = 0;
		std::vector<std::uint8_t> present_;  // packed: bit i in byte (i/8)
	};

	// Typed column for BOOL/INT/UNSIGNED_INT/FLOAT/DOUBLE.
	// String columns get a specialized class below because per-row
	// std::string is too expensive (D23b's R17b P17b-3 fix: intern
	// pool per column instead).
	template <typename T>
	class PropertyColumn : public PropertyColumnBase
	{
		public:
		PropertyColumn(PropertyColumnType t, std::string n)
			: PropertyColumnBase(t, std::move(n))
		{}

		void set(std::uint32_t i, T v)
		{
			ensureCapacity_(i + 1);
			if (i >= data_.size()) data_.resize(i + 1, T{});
			data_[i] = v;
			setPresence_(i);
		}

		T get(std::uint32_t i) const
		{
			if (!isSet(i)) return T{};
			return data_[i];
		}

		void clear(std::uint32_t i) override
		{
			clearPresence_(i);
			if (i < data_.size()) data_[i] = T{};
		}

		void resize(std::size_t n) override
		{
			ensureCapacity_(n);
			if (n > data_.size()) data_.resize(n, T{});
		}

		private:
		std::vector<T> data_;
	};

	// String column with per-column intern pool. Storage per row is
	// std::uint32_t (offset into pool_), not std::string. 8x smaller
	// for repeated short atom-type-name values.
	class StringPropertyColumn : public PropertyColumnBase
	{
		public:
		explicit StringPropertyColumn(std::string n)
			: PropertyColumnBase(PropertyColumnType::STRING, std::move(n))
		{
			// offset 0 is the canonical empty string. Stored as
			// NUL-terminated entries in pool_.
			pool_.push_back('\0');
		}

		void set(std::uint32_t i, const std::string& s)
		{
			ensureCapacity_(i + 1);
			if (i >= offsets_.size()) offsets_.resize(i + 1, 0u);
			offsets_[i] = intern_(s);
			setPresence_(i);
		}

		std::string get(std::uint32_t i) const
		{
			if (!isSet(i)) return {};
			return std::string(pool_.c_str() + offsets_[i]);
		}

		void clear(std::uint32_t i) override
		{
			clearPresence_(i);
			if (i < offsets_.size()) offsets_[i] = 0u;
		}

		void resize(std::size_t n) override
		{
			ensureCapacity_(n);
			if (n > offsets_.size()) offsets_.resize(n, 0u);
		}

		std::size_t poolSize() const { return pool_.size(); }

		private:
		std::uint32_t intern_(const std::string& s)
		{
			if (s.empty()) return 0u;
			auto it = intern_map_.find(s);
			if (it != intern_map_.end()) return it->second;
			std::uint32_t off = static_cast<std::uint32_t>(pool_.size());
			pool_.append(s);
			pool_.push_back('\0');
			intern_map_.emplace(s, off);
			return off;
		}

		std::vector<std::uint32_t>                     offsets_;
		std::string                                    pool_;
		std::unordered_map<std::string, std::uint32_t> intern_map_;
	};

	// Registry: name -> column. Predeclares 10 well-known force-field
	// columns at construction so they bypass the dynamic-cap accounting
	// per D23b.
	class PropertyColumnRegistry
	{
		public:
		PropertyColumnRegistry()
		{
			predeclareWellKnown_();
		}

		PropertyColumnRegistry(const PropertyColumnRegistry&)            = delete;
		PropertyColumnRegistry& operator=(const PropertyColumnRegistry&) = delete;

		// Returns nullptr if the column doesn't exist.
		PropertyColumnBase* findColumn(const std::string& name)
		{
			auto it = columns_.find(name);
			return it == columns_.end() ? nullptr : it->second.get();
		}

		const PropertyColumnBase* findColumn(const std::string& name) const
		{
			auto it = columns_.find(name);
			return it == columns_.end() ? nullptr : it->second.get();
		}

		// Register a new dynamic column of the given type. Returns
		// nullptr and DOES NOT register if the cap is hit. Well-known
		// names use registerWellKnown_ during predeclareWellKnown_ and
		// don't count toward the cap.
		PropertyColumnBase* registerColumn(const std::string& name,
		                                   PropertyColumnType t)
		{
			// Already registered? Return existing (don't double-count).
			if (auto* existing = findColumn(name)) return existing;
			if (dynamic_count_ >= max_dynamic_) return nullptr;
			++dynamic_count_;
			auto* col = makeColumn_(name, t);
			columns_.emplace(name, std::unique_ptr<PropertyColumnBase>(col));
			return col;
		}

		std::size_t dynamicCount() const         { return dynamic_count_; }
		std::size_t maxDynamic() const           { return max_dynamic_; }
		void setMaxDynamic(std::size_t n)        { max_dynamic_ = n; }

		bool isWellKnown(const std::string& name) const
		{
			return well_known_.count(name) > 0;
		}

		std::size_t columnCount() const          { return columns_.size(); }

		private:
		PropertyColumnBase* makeColumn_(const std::string& name,
		                                PropertyColumnType t)
		{
			switch (t)
			{
				case PropertyColumnType::BOOL:
					return new PropertyColumn<bool>(t, name);
				case PropertyColumnType::INT:
					return new PropertyColumn<std::int32_t>(t, name);
				case PropertyColumnType::UNSIGNED_INT:
					return new PropertyColumn<std::uint32_t>(t, name);
				case PropertyColumnType::FLOAT:
					return new PropertyColumn<float>(t, name);
				case PropertyColumnType::DOUBLE:
					return new PropertyColumn<double>(t, name);
				case PropertyColumnType::STRING:
					return new StringPropertyColumn(name);
				default:
					return nullptr;
			}
		}

		void registerWellKnown_(const char* name, PropertyColumnType t)
		{
			auto* col = makeColumn_(name, t);
			columns_.emplace(name, std::unique_ptr<PropertyColumnBase>(col));
			well_known_.insert(name);
		}

		// D23b: 10 force-field-frequent columns predeclared. Anything
		// touched by AmberFF / MMFF94 / charge-assignment passes goes
		// here so the bulk-assign hot path skips registry lookup
		// overhead and the dynamic-cap accounting.
		void predeclareWellKnown_()
		{
			registerWellKnown_("PARTIAL_CHARGE",       PropertyColumnType::FLOAT);
			registerWellKnown_("FORMAL_CHARGE",        PropertyColumnType::INT);
			registerWellKnown_("MMFF94_TYPE",          PropertyColumnType::INT);
			registerWellKnown_("AMBER_TYPE",           PropertyColumnType::STRING);
			registerWellKnown_("RADIUS",               PropertyColumnType::FLOAT);
			registerWellKnown_("EPSILON",              PropertyColumnType::FLOAT);
			registerWellKnown_("HYBRIDIZATION",        PropertyColumnType::INT);
			registerWellKnown_("IS_AROMATIC",          PropertyColumnType::BOOL);
			registerWellKnown_("ATOM_TYPE_NAME",       PropertyColumnType::STRING);
			registerWellKnown_("STEREO_DESCRIPTOR",    PropertyColumnType::INT);
		}

		std::unordered_map<std::string, std::unique_ptr<PropertyColumnBase>> columns_;
		std::unordered_set<std::string> well_known_;
		std::size_t                     dynamic_count_ = 0;
		std::size_t                     max_dynamic_   = 65536;
	};

	// ============================================================
	// v2.2 H1a (A2 flat SoA topology) -- container metadata table.
	//
	// D55/D58: the molecular hierarchy (Molecule/Chain/Residue/...) is
	// store metadata, NOT a C++ Composite object tree. Each container is
	// one ContainerRow; the row's ordered ChildRef vector (D57) is the
	// SOURCE OF TRUTH for child order; the atom->container reverse edge
	// is the atom_parent_ map (a per-atom column in the eventual flip,
	// kept as a map here so H1a does not touch the hot atom-column path).
	//
	// H1a is store-side ONLY: nothing in the v0 consumer path reads or
	// writes these tables yet. They are exercised + parity-checked by
	// HierarchyParity_test. Dual existence (D60): the v0 object tree is
	// the source of truth through H1-H3; this table is a verified mirror,
	// written forward-only (never from a destructor).
	// ============================================================

	// D58: container kind tag. Atoms/Bonds are NOT containers -- they
	// have their own SoA columns + bond table. This tag covers only the
	// molecular-hierarchy container kinds.
	enum class ContainerKind : std::uint8_t
	{
		NONE                = 0,
		MOLECULE            = 1,
		PROTEIN             = 2,
		CHAIN               = 3,
		RESIDUE             = 4,
		SECONDARY_STRUCTURE = 5,
		NUCLEOTIDE          = 6,
		NUCLEIC_ACID        = 7,
		FRAGMENT            = 8,
	};

	// D57: an ordered edge from a container to one child. A child is
	// either a child container row (CONTAINER) or a child atom slot
	// (ATOM); `kind` disambiguates which table `idx` indexes. The
	// per-container vector<ChildRef> reproduces the v0 doubly-linked
	// first_child_/next_ child list as indexed edges.
	struct ChildRef
	{
		enum Kind : std::uint8_t { CONTAINER = 0, ATOM = 1 };
		std::uint8_t  kind = CONTAINER;
		std::uint32_t idx  = 0;

		ChildRef() = default;
		ChildRef(std::uint8_t k, std::uint32_t i) : kind(k), idx(i) {}

		bool operator==(const ChildRef& o) const
		{ return kind == o.kind && idx == o.idx; }
		bool operator!=(const ChildRef& o) const { return !(*this == o); }
	};

	// D58: kind-specific scalar payload. Common state (name, parent,
	// children, selection, generation) lives in ContainerRow; only the
	// per-kind scalars live here.
	//
	// NOTE (D59 + R31 P31-3): Residue/Nucleotide enum-style flags
	// (PROPERTY__AMINO_ACID, __C_TERMINAL, ...) are container PROPERTIES,
	// not payload fields -- confirmed against residue.h `enum Property`.
	// They live in container_properties_ / the sparse bag, not here.
	struct ContainerPayload
	{
		// Protein/NucleicAcid/Residue/Nucleotide id -> offset into the
		// container table string pool (0 = empty). Chain has NO id field
		// (R31 follow-up #2): it uses the common ContainerRow::name_offset.
		std::uint32_t id_offset      = 0;
		// Residue/Nucleotide insertion code (default ' ').
		char          insertion_code = ' ';
		// SecondaryStructure::Type, cast to u8 (HELIX/STRAND/...).
		std::uint8_t  ss_type        = 0;
	};

	// D58: one container metadata row. parent == NONE means a detached
	// container (orphan store) or a direct child of the System root.
	struct ContainerRow
	{
		static constexpr std::uint32_t NONE = 0xFFFFFFFFu;

		ContainerKind         kind                 = ContainerKind::NONE;
		std::uint32_t         parent_container_idx = NONE;
		std::uint32_t         name_offset          = 0;   // common name
		std::vector<ChildRef> children;                   // D57 source of truth
		std::uint32_t         selection_count      = 0;   // D59 / D46
		std::uint32_t         generation           = 0;
		ContainerPayload      payload;
	};

	// v2.2 D55/D56/D57/D58/D59: the container metadata table. Owned by
	// every MoleculeStore's side tables (so the process-global orphan
	// store naturally provides the "container orphan store" of D56). A
	// detached container's row lives in the orphan store's table until
	// migrate_subtree_from() moves it into a System store (D56).
	class ContainerTable
	{
		public:
		static constexpr std::uint32_t NONE = ContainerRow::NONE;

		ContainerTable()
		{
			// Slot 0 reserved as the NULL/sentinel row (mirrors the
			// composite_nodes_ dummy-at-0 convention). allocate() returns
			// idx >= 1; string pool offset 0 is the canonical empty string.
			rows_.emplace_back();
			string_pool_.push_back('\0');
		}

		ContainerTable(const ContainerTable&)            = delete;
		ContainerTable& operator=(const ContainerTable&) = delete;

		// --- lifecycle ---------------------------------------------------

		// Allocate a fresh container row of the given kind. Reuses a freed
		// slot if available, else appends. Returns the row index (>= 1).
		std::uint32_t allocate(ContainerKind kind);

		// Return a row's slot to the free list after clearing it. The row's
		// children vector is cleared; callers must have already detached it
		// from its parent's child list. Idempotent on NONE / slot 0.
		void release(std::uint32_t idx);

		ContainerRow&       row(std::uint32_t idx)       { return rows_[idx]; }
		const ContainerRow& row(std::uint32_t idx) const { return rows_[idx]; }

		// size() includes slot 0 + freed slots; live_count() excludes both.
		std::size_t size() const       { return rows_.size(); }
		std::size_t live_count() const { return rows_.size() - free_list_.size() - 1; }
		std::size_t freed_count() const { return free_list_.size(); }
		bool        is_freed(std::uint32_t idx) const;

		// --- string pool (names + payload ids) ---------------------------

		std::uint32_t intern(const std::string& s);
		std::string   str(std::uint32_t offset) const
		{ return std::string(string_pool_.c_str() + offset); }
		std::size_t   pool_size() const { return string_pool_.size(); }

		// --- D57 ordered child-edge operations ---------------------------
		// Each op maintains BOTH the parent's children vector AND the
		// reverse edge: parent_container_idx for CONTAINER children,
		// atom_parent_ for ATOM children. insert_*_before/after splice at
		// the pivot's ordinal; on a missing pivot they append (defensive
		// -- correct mirroring always supplies a pivot already in the list).

		void append_child (std::uint32_t parent_idx, ChildRef c);
		void prepend_child(std::uint32_t parent_idx, ChildRef c);
		void insert_child_before(std::uint32_t parent_idx, ChildRef c, ChildRef pivot);
		void insert_child_after (std::uint32_t parent_idx, ChildRef c, ChildRef pivot);
		// Remove a child edge; clears the reverse edge. Returns true if found.
		bool remove_child(std::uint32_t parent_idx, ChildRef c);

		// --- reverse edge: atom -> immediate container -------------------
		// (kept as a map in H1a; promoted to a per-atom column at the flip)
		std::uint32_t atom_parent(std::uint32_t atom_idx) const
		{
			auto it = atom_parent_.find(atom_idx);
			return it == atom_parent_.end() ? NONE : it->second;
		}
		const std::unordered_map<std::uint32_t, std::uint32_t>& atom_parents() const
		{ return atom_parent_; }

		// --- D59 / D46 selection counters --------------------------------
		// Walk parent_container_idx up from `start` (inclusive), adding
		// `delta` to each row's selection_count. delta is +1 on select,
		// -1 on deselect. Used by the eventual atom select() mirror; here
		// the parity test drives it directly.
		void bump_selection_up(std::uint32_t start_container_idx, int delta);

		// --- D58/D59 container properties --------------------------------
		// Same typed-column + sparse-bag mechanism as the atom side, keyed
		// by container_idx instead of atom_idx. Residue flag bits + named
		// container properties live here.
		PropertyColumnRegistry container_properties_;

		// --- traversal ---------------------------------------------------
		// Preorder walk of the subtree rooted at `root_idx`. For each
		// CONTAINER edge the visitor is invoked then the child is recursed;
		// for each ATOM edge the atom visitor is invoked. Reproduces the v0
		// Composite preorder over first_child_/next_.
		//   on_container(std::uint32_t container_idx)
		//   on_atom(std::uint32_t atom_idx)
		template <typename CFn, typename AFn>
		void preorder(std::uint32_t root_idx, CFn&& on_container, AFn&& on_atom) const
		{
			on_container(root_idx);
			for (const ChildRef& c : rows_[root_idx].children)
			{
				if (c.kind == ChildRef::CONTAINER)
					preorder(c.idx, on_container, on_atom);
				else
					on_atom(c.idx);
			}
		}

		// --- D56 detached-subtree migration ------------------------------
		// Copy the container subtree rooted at `src_root` out of `src` into
		// *this, remapping container indices (and ChildRef CONTAINER idx +
		// parent_container_idx) through a freshly built old->new map, and
		// remapping ATOM child indices through `atom_remap` (the atom slot
		// map the caller's adoptSubtree already produced; identity is legal
		// for a same-index test). Names/ids are re-interned into this pool;
		// payload, selection_count, and container properties are copied.
		// The returned new-root row has parent_container_idx = NONE; the
		// caller links it under the destination tree with a ChildRef op.
		std::uint32_t migrate_subtree_from(
			const ContainerTable& src,
			std::uint32_t src_root,
			const std::unordered_map<std::uint32_t, std::uint32_t>& atom_remap);

		private:
		std::vector<ContainerRow>  rows_;
		std::vector<std::uint32_t> free_list_;
		std::string                string_pool_;
		std::unordered_map<std::string, std::uint32_t> string_intern_;
		std::unordered_map<std::uint32_t, std::uint32_t> atom_parent_;

		// Helper: position of `c` in parent's children, or npos.
		std::size_t child_ordinal_(std::uint32_t parent_idx, ChildRef c) const;
		// Helper: set the reverse edge for a freshly attached child.
		void set_reverse_edge_(std::uint32_t parent_idx, ChildRef c);
		// Helper: clear the reverse edge for a detached child.
		void clear_reverse_edge_(ChildRef c);
		// Recursive worker for migrate_subtree_from.
		std::uint32_t migrate_one_(
			const ContainerTable& src, std::uint32_t src_idx,
			const std::unordered_map<std::uint32_t, std::uint32_t>& atom_remap);
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

		// v2.2 H1a (D55/D56/D57/D58): the container metadata table -- the
		// store-backed replacement for the molecular Composite object tree
		// (Molecule/Chain/Residue/...). The orphan store's table is the
		// "container orphan store" of D56. Store-side only in H1a; the v0
		// object tree remains the source of truth (D60) and this is a
		// verified mirror exercised by HierarchyParity_test.
		ContainerTable               container_table_;

		// v2.1 P1.4 (D23b): typed property column registry. Well-known
		// force-field columns are predeclared at construction (PARTIAL_
		// CHARGE, FORMAL_CHARGE, MMFF94_TYPE, AMBER_TYPE, RADIUS,
		// EPSILON, HYBRIDIZATION, IS_AROMATIC, ATOM_TYPE_NAME,
		// STEREO_DESCRIPTOR) and don't count against the dynamic-name
		// cap. P1.5 adds the sparse fallback bag for unregistered
		// names. P2 wires PropertyManager mutations to maintain the
		// columns alongside the v0 inline bag.
		PropertyColumnRegistry       property_columns_;

		// v2.1 P1.5 (D23b): sparse fallback for mismatched-type
		// setProperty (per D23b sparse-first lookup precedence) and
		// for dynamic property names that haven't crossed the
		// promote-to-dense threshold. Outer key is atom_idx; inner
		// is property name -> a small variant. P2 wiring populates
		// this from PropertyManager mutations.
		//
		// Storage shape note: we store a parallel pair of (name, type
		// tag, value) instead of a full NamedProperty to avoid pulling
		// the PropertyManager headers into this internal file.
		struct SparseProperty
		{
			std::string         name;
			PropertyColumnType  type = PropertyColumnType::NONE;
			// Inline union of the supported types. STRING uses
			// std::string directly here (sparse entries are O(few per
			// atom), so per-entry std::string cost is fine — the
			// dense-column case is where interning matters).
			bool                v_bool  = false;
			std::int32_t        v_int   = 0;
			std::uint32_t       v_uint  = 0;
			float               v_float = 0.0f;
			double              v_double = 0.0;
			std::string         v_string;
		};
		std::unordered_map<std::uint32_t, std::vector<SparseProperty>> sparse_bag_;

		// v2.1 P1.5: sparse-bag access primitives. P2 wiring builds the
		// public setProperty / getProperty semantics from these.
		SparseProperty*       sparse_find_(std::uint32_t atom_idx, const std::string& name)
		{
			auto it = sparse_bag_.find(atom_idx);
			if (it == sparse_bag_.end()) return nullptr;
			for (auto& p : it->second) if (p.name == name) return &p;
			return nullptr;
		}
		const SparseProperty* sparse_find_(std::uint32_t atom_idx, const std::string& name) const
		{
			auto it = sparse_bag_.find(atom_idx);
			if (it == sparse_bag_.end()) return nullptr;
			for (auto& p : it->second) if (p.name == name) return &p;
			return nullptr;
		}

		// Sparse insert/replace. Returns true if a fresh entry was
		// created; false if an existing entry was overwritten.
		bool sparse_set_(std::uint32_t atom_idx, const SparseProperty& p)
		{
			auto& vec = sparse_bag_[atom_idx];
			for (auto& existing : vec)
			{
				if (existing.name == p.name)
				{
					existing = p;
					return false;
				}
			}
			vec.push_back(p);
			return true;
		}

		// Erase a specific (atom, name). Returns true if found.
		bool sparse_clear_(std::uint32_t atom_idx, const std::string& name)
		{
			auto it = sparse_bag_.find(atom_idx);
			if (it == sparse_bag_.end()) return false;
			auto& vec = it->second;
			for (auto vi = vec.begin(); vi != vec.end(); ++vi)
			{
				if (vi->name == name)
				{
					vec.erase(vi);
					if (vec.empty()) sparse_bag_.erase(it);
					return true;
				}
			}
			return false;
		}

		// v2.1 P1.5: promote-once policy. Called from compact() (P2
		// wiring) for each name in sparse_bag_ whose fill rate has
		// crossed the threshold. Moves the matching-type entries from
		// sparse to a dense column; mismatched-type entries stay
		// sparse. Returns the count of entries promoted.
		std::size_t promote_sparse_(const std::string& name,
		                            PropertyColumnType t);

		// v2.1 P1.7 (D24b): selected-bit storage as an owned atomic
		// word array rebuilt under exclusive store access. NOT a
		// std::vector<std::atomic<u64>> because libc++ rejects that
		// container template (atomic isn't Cpp17MoveInsertable, and
		// MoleculeStore::compact() shrink_to_fit's its side vectors —
		// see R17b P17b-N2). Caller (MoleculeStore::reserve_atoms /
		// compact / dtor) is responsible for holding exclusive access
		// across resize_selected_bits_; steady-state set / is_selected
		// use relaxed atomics and require no lock per D24b + D16.
		std::unique_ptr<std::atomic<std::uint64_t>[]> selected_bits_;
		std::size_t selected_bits_word_capacity_ = 0;

		// Grow the atomic array to cover at least new_atom_capacity
		// atoms. Old contents are copied via relaxed atomic load/store.
		// Caller holds exclusive write access — no concurrent readers
		// during this call.
		void resize_selected_bits_(std::size_t new_atom_capacity);

		// Atomic per-bit set/clear/test. Word index is i/64; bit
		// position is i%64. Caller is responsible for prior
		// resize_selected_bits_(>= i+1) — set_selected does NOT grow
		// the array because growth requires the exclusive contract.
		// is_selected returns false (the C++ default) if i is out of
		// bounds rather than asserting; callers that care about
		// "explicitly cleared" vs "never set" must track separately.
		void set_selected_(std::uint32_t i, bool v);
		bool is_selected_(std::uint32_t i) const;

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
