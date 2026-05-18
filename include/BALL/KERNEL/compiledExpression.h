// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// BALL 2.0 KERNEL — K0.5 compiled selection.
// Spec: .planning/v2.x/COMPILED-SELECTION-SPEC.md
// Sub-phase: K0.5.0 (skeleton — node types + CompiledExpression class).
// Evaluation (K0.5.1), parser→AST compile + cache (K0.5.2), Selector
// integration (K0.5.3) land in subsequent sub-phases.
//

#ifndef BALL_KERNEL_COMPILEDEXPRESSION_H
#define BALL_KERNEL_COMPILEDEXPRESSION_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

// std::variant needs ExpressionPredicate to be complete (OwnedPred
// holds a unique_ptr<ExpressionPredicate> alternative; the variant's
// move ctor materialises the destructor of every alternative).
#ifndef BALL_KERNEL_EXPRESSIONPREDICATE_H
# include <BALL/KERNEL/expressionPredicate.h>
#endif

#include <cstdint>
#include <memory>
#include <string>
#include <variant>
#include <vector>

namespace BALL
{
	class Atom;
	class MoleculeStore;

	/** Compiled-selection AST node taxonomy (K0.5).

			The variant collapses 27 polymorphic `ExpressionPredicate`
			subclasses (see `source/KERNEL/expression.C:209-239`) into a
			small closed set of node types. Each leaf carries pre-compiled
			constants (atomic number, interned string offset, etc.) so the
			inner evaluation loop is integer/float compare per atom on the
			MoleculeStore's SoA columns.

			See COMPILED-SELECTION-SPEC.md §2 for the design rationale.

			\ingroup  KernelSelection
	*/

	/// Trivially-true predicate.
	struct TrueLeaf      {};
	/// Trivially-false predicate.
	struct FalseLeaf     {};
	/// Atom-is-selected predicate (reads MoleculeStore::selection_).
	struct SelectedLeaf  {};
	/// Atom-is-solvent (per residue.is_solvent).
	struct SolventLeaf   {};
	/// Atom-is-backbone (per Residue's backbone atom list).
	struct BackboneLeaf  {};

	/// Element-by-atomic-number predicate. `element_idx` indexes PTE.
	struct ElementPred       { std::uint8_t element_idx; };
	/// Atom name = interned string in MoleculeStore string pool.
	struct AtomNamePred      { std::uint32_t name_offset; };
	/// Atom type name = interned string in MoleculeStore string pool.
	struct AtomTypePred      { std::uint32_t type_name_offset; };
	/// Residue name = interned string in store pool (one CSR lookup).
	struct ResidueNamePred   { std::uint32_t name_offset; };
	/// Residue numeric id (or string-match for non-integer ids).
	struct ResidueIDPred     { std::int32_t  id;  bool string_match; std::string raw; };
	/// Chain name match.
	struct ChainPred         { std::uint32_t name_offset; };
	/// Protein (top-level fragment) name match.
	struct ProteinPred       { std::uint32_t name_offset; };
	/// Molecule name match.
	struct MoleculePred      { std::uint32_t name_offset; };

	/// Comparison operators for numeric-range leaves.
	enum class CmpOp : std::uint8_t { LT, LE, EQ, GE, GT };

	/// Charge-range predicate over MoleculeStore::charges_[idx].
	struct ChargeRange       { CmpOp op; float value; };
	/// Hybridisation predicate. 1=sp, 2=sp2, 3=sp3 (matches v1.x enum).
	struct HybridLeaf        { std::uint8_t kind; };
	/// Bond count predicate. `order_filter == 0` = any order; otherwise
	/// only bonds with `order == order_filter` are counted.
	struct NumberOfBondsPred { CmpOp op; std::uint8_t n; std::uint8_t order_filter; };

	/** Slow-path leaf — owns a v1.x ExpressionPredicate instance whose
			evaluation cannot be vectorised over the store (ring detection,
			SMARTS, user-registered custom predicates). Per-atom evaluation
			falls back to virtual dispatch through the Atom back-pointer.
			`name` + `argument` are kept so two OwnedPreds with identical
			source can be hashed/compared for cache de-dup.
	*/
	struct OwnedPred
	{
		std::unique_ptr<ExpressionPredicate> impl;
		std::string                          name;
		std::string                          argument;
	};

	// Forward declarations for boxed recursive node alternatives.
	struct AndNode;
	struct OrNode;
	struct NotNode;

	/** Variant of every legal AST node type. unique_ptr boxing on
			And/Or/Not keeps the variant bounded-size despite the recursive
			definition. Move-only.
	*/
	using PredNode = std::variant<
		TrueLeaf, FalseLeaf, SelectedLeaf, SolventLeaf, BackboneLeaf,
		ElementPred, AtomNamePred, AtomTypePred,
		ResidueNamePred, ResidueIDPred, ChainPred, ProteinPred, MoleculePred,
		ChargeRange, HybridLeaf, NumberOfBondsPred,
		OwnedPred,
		std::unique_ptr<AndNode>,
		std::unique_ptr<OrNode>,
		std::unique_ptr<NotNode>
	>;

	/// Inner AND node — true iff every child evaluates true.
	struct AndNode { std::vector<PredNode> children; };
	/// Inner OR node — true iff any child evaluates true.
	struct OrNode  { std::vector<PredNode> children; };
	/// Inner NOT node — true iff child evaluates false.
	struct NotNode { PredNode child; };

	/** Compiled selection expression.

			Replaces the v1.x `Expression` evaluation pipeline. Holds the
			compiled variant AST root, the original source string (for
			caching + diagnostics), and a hash of the registered-predicate
			name-set at compile time (so cached compilations from a stale
			predicate registry are rejected).

			Construction is via `Expression::compile_()` (K0.5.2); direct
			instantiation is allowed for tests and for callers that build an
			AST programmatically.

			Move-only at the public level; the cache (K0.5.2) wraps
			instances in `shared_ptr<const CompiledExpression>` for shared
			read-only use.

			\ingroup  KernelSelection
	*/
	class BALL_EXPORT CompiledExpression
	{
		public:

		CompiledExpression(PredNode root,
		                   std::string source,
		                   std::size_t pred_set_hash);
		~CompiledExpression();

		CompiledExpression(const CompiledExpression&)            = delete;
		CompiledExpression& operator=(const CompiledExpression&) = delete;
		CompiledExpression(CompiledExpression&&)            noexcept;
		CompiledExpression& operator=(CompiledExpression&&) noexcept;

		/** Bitmap evaluation (K0.5.1 will implement).
				`out_bitmap[i] == 1` iff root evaluates true for store atom i.
				`out_bitmap` is resized to `store.size()` (freed slots get 0).
		*/
		void evaluate(const MoleculeStore& store,
		              std::vector<std::uint8_t>& out_bitmap) const;

		/** Per-atom fallback (used by Expression::operator()(const Atom&)). */
		bool evaluate_one(const Atom& atom) const;

		const std::string& source() const          { return source_; }
		std::size_t        pred_set_hash() const   { return pred_set_hash_; }
		const PredNode&    root() const            { return root_; }

		/** K0.5.2: parse `source` and lower the resulting SyntaxTree into
				a CompiledExpression bound to `store`. String-literal leaves
				(AtomName, AtomType) intern their arguments into the store's
				pool, so the resulting CompiledExpression is store-specific.
				Throws Exception::ParseError on syntax errors or unsupported
				predicate names. K0.5.2 supports the fast-path predicates
				only ({true, false, selected, element, name, type, charge,
				numberOfBonds, AND, OR, NOT}); K0.5.4 will extend to
				OwnedPred for ring/SMARTS/user predicates.
		*/
		static std::shared_ptr<const CompiledExpression>
		    compile(MoleculeStore& store,
		            const std::string& source,
		            std::size_t pred_set_hash = 0);

		private:
		PredNode    root_;
		std::string source_;
		std::size_t pred_set_hash_;
	};

	/** K0.5.2 LRU cache of compiled expressions, keyed on (source string,
			MoleculeStore*). Per-process singleton via `instance()`. Default
			capacity 1024 entries; evicts least-recently-used on insert
			past capacity. Cached entries remain valid until either:
				- evicted by capacity pressure;
				- explicitly removed via invalidate() / clear();
				- the originating store calls compact() (intern table may
					reshuffle offsets — K0.5.6 will wire compact()->clear).

			\ingroup  KernelSelection
	*/
	class BALL_EXPORT CompiledExpressionCache
	{
		public:
		struct Key
		{
			std::string    source;
			MoleculeStore* store;
			bool operator==(const Key& o) const { return store == o.store && source == o.source; }
		};
		struct KeyHash
		{
			std::size_t operator()(const Key& k) const noexcept;
		};

		CompiledExpressionCache();
		~CompiledExpressionCache();
		CompiledExpressionCache(const CompiledExpressionCache&)            = delete;
		CompiledExpressionCache& operator=(const CompiledExpressionCache&) = delete;

		static CompiledExpressionCache& instance();

		/** Look up `source` for `store`. If present, returns the cached
				shared_ptr and marks it MRU. If absent, compiles via
				CompiledExpression::compile, installs, evicts LRU on overflow,
				and returns the new shared_ptr. */
		std::shared_ptr<const CompiledExpression>
		    get_or_compile(MoleculeStore& store, const std::string& source);

		/** Forget any cached entries that name `store`. Call from
				MoleculeStore::compact() (K0.5.6 wiring) and ~MoleculeStore. */
		void invalidate_store(MoleculeStore* store);

		void invalidate(const Key& key);
		void clear();
		std::size_t size() const;
		std::size_t capacity() const;
		void set_capacity(std::size_t n);

		private:
		struct Impl;
		std::unique_ptr<Impl> impl_;
	};

} // namespace BALL

#endif // BALL_KERNEL_COMPILEDEXPRESSION_H
