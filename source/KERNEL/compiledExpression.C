// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// BALL 2.0 KERNEL — K0.5 compiled selection (sub-phases K0.5.0 + .1).
// K0.5.0 added the variant AST + class skeleton.
// K0.5.1 implements the bitmap evaluation inner loop for the column-
// vectorisable leaves: Element / AtomName / AtomType / Charge /
// NumberOfBonds + And / Or / Not composition, plus the trivial
// True / False / Selected leaves.
//
// Ancestor-walk leaves (ResidueName / ResidueID / Chain / Protein /
// Molecule / Backbone / Solvent), HybridLeaf, and OwnedPred remain
// stubbed; they need either back_ptr walks (K0.5.4 OwnedPred path) or
// new MoleculeStore columns (K0.5.1.b residue/chain back-index).
//

#include <BALL/KERNEL/compiledExpression.h>
#include <BALL/KERNEL/moleculeStore.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/expression.h>
#include <BALL/KERNEL/expressionParser.h>
#include <BALL/KERNEL/expressionPredicate.h>
#include <BALL/KERNEL/expressionTree.h>
#include <BALL/COMMON/exception.h>

#include <algorithm>
#include <cctype>
#include <cstring>
#include <list>
#include <unordered_map>

namespace BALL
{

CompiledExpression::CompiledExpression(PredNode root,
                                       std::string source,
                                       std::size_t pred_set_hash)
	: root_(std::move(root)),
	  source_(std::move(source)),
	  pred_set_hash_(pred_set_hash)
{
}

CompiledExpression::~CompiledExpression() = default;
CompiledExpression::CompiledExpression(CompiledExpression&&) noexcept            = default;
CompiledExpression& CompiledExpression::operator=(CompiledExpression&&) noexcept = default;

// --- K0.5.1 bitmap evaluation engine -------------------------------------
namespace
{
	using Byte = std::uint8_t;

	inline bool cmp_(CmpOp op, float a, float b) {
		switch (op) {
			case CmpOp::LT: return a <  b;
			case CmpOp::LE: return a <= b;
			case CmpOp::EQ: return a == b;
			case CmpOp::GE: return a >= b;
			case CmpOp::GT: return a >  b;
		}
		return false;
	}
	inline bool cmp_(CmpOp op, int a, int b) {
		switch (op) {
			case CmpOp::LT: return a <  b;
			case CmpOp::LE: return a <= b;
			case CmpOp::EQ: return a == b;
			case CmpOp::GE: return a >= b;
			case CmpOp::GT: return a >  b;
		}
		return false;
	}

	void eval_node_(const PredNode& node,
	                const MoleculeStore& store,
	                std::vector<Byte>& out);

	// 2026-05-18 (Codex R6 FYI-6 — AndNode tmp-bitmap reuse): each AND
	// and OR inner-node evaluation used to allocate a fresh
	// `std::vector<Byte> tmp(n)` per call. For a tree of depth D evaluated
	// E times that's O(D*E) heap allocations per evaluate() pass. Switch
	// to a thread-local depth-indexed scratch pool: vector slots keep
	// their capacity across evaluations so the allocation amortises to
	// O(D) once. Each AND/OR scope acquires pool[scratch_depth++], hands
	// it to children, releases on scope exit.
	struct ScratchPool
	{
		std::vector<std::vector<Byte>> bufs;
		std::size_t depth = 0;
	};
	inline ScratchPool& scratch_pool_()
	{
		static thread_local ScratchPool pool;
		return pool;
	}
	struct ScratchScope
	{
		ScratchPool& pool;
		std::vector<Byte>& buf;
		explicit ScratchScope(std::size_t n) : pool(scratch_pool_()),
			buf((pool.depth < pool.bufs.size()
			       ? pool.bufs[pool.depth]
			       : (pool.bufs.emplace_back(), pool.bufs.back())))
		{
			++pool.depth;
			if (buf.size() < n) buf.resize(n);
		}
		~ScratchScope() { --pool.depth; }
		ScratchScope(const ScratchScope&) = delete;
		ScratchScope& operator=(const ScratchScope&) = delete;
	};

	// Fast-path leaf evaluators. Each writes  out[i] = 0 | 1  for every
	// store slot i (freed slots already pre-zeroed by the dispatcher).
	void eval_(const TrueLeaf&, const MoleculeStore& s,
	           std::vector<Byte>& out)
	{
		const Byte* freed = s.is_freed_data();
		const std::size_t n = s.size();
		for (std::size_t i = 0; i < n; ++i) out[i] = freed[i] ? 0 : 1;
	}
	void eval_(const FalseLeaf&, const MoleculeStore& s,
	           std::vector<Byte>& out)
	{
		std::memset(out.data(), 0, s.size());
	}
	void eval_(const SelectedLeaf&, const MoleculeStore& s,
	           std::vector<Byte>& out)
	{
		const Byte* sel   = s.selection_data();
		const Byte* freed = s.is_freed_data();
		const std::size_t n = s.size();
		for (std::size_t i = 0; i < n; ++i)
			out[i] = (!freed[i] && sel[i]) ? 1 : 0;
	}
	void eval_(const ElementPred& p, const MoleculeStore& s,
	           std::vector<Byte>& out)
	{
		const Byte* elem  = s.element_indices_data();
		const Byte* freed = s.is_freed_data();
		const std::size_t n = s.size();
		for (std::size_t i = 0; i < n; ++i)
			out[i] = (!freed[i] && elem[i] == p.element_idx) ? 1 : 0;
	}
	void eval_(const AtomNamePred& p, const MoleculeStore& s,
	           std::vector<Byte>& out)
	{
		const std::uint32_t* off = s.name_offsets_data();
		const Byte*          freed = s.is_freed_data();
		const std::size_t n = s.size();
		for (std::size_t i = 0; i < n; ++i)
			out[i] = (!freed[i] && off[i] == p.name_offset) ? 1 : 0;
	}
	void eval_(const AtomTypePred& p, const MoleculeStore& s,
	           std::vector<Byte>& out)
	{
		const std::uint32_t* off = s.type_name_offsets_data();
		const Byte*          freed = s.is_freed_data();
		const std::size_t n = s.size();
		for (std::size_t i = 0; i < n; ++i)
			out[i] = (!freed[i] && off[i] == p.type_name_offset) ? 1 : 0;
	}
	void eval_(const ChargeRange& p, const MoleculeStore& s,
	           std::vector<Byte>& out)
	{
		const float* ch    = s.charges_data();
		const Byte*  freed = s.is_freed_data();
		const std::size_t n = s.size();
		for (std::size_t i = 0; i < n; ++i)
			out[i] = (!freed[i] && cmp_(p.op, ch[i], p.value)) ? 1 : 0;
	}
	void eval_(const NumberOfBondsPred& p, const MoleculeStore& s,
	           std::vector<Byte>& out)
	{
		const std::size_t n = s.size();
		if (p.order_filter == 0)
		{
			// Any-order: O(1) per atom via the CSR degree.
			for (std::size_t i = 0; i < n; ++i)
			{
				if (s.is_freed(i)) { out[i] = 0; continue; }
				const int deg = static_cast<int>(s.bond_degree(i));
				out[i] = cmp_(p.op, deg, static_cast<int>(p.n)) ? 1 : 0;
			}
		}
		else
		{
			// Filter by bond order via for_each_bond_of snapshot.
			for (std::size_t i = 0; i < n; ++i)
			{
				if (s.is_freed(i)) { out[i] = 0; continue; }
				int count = 0;
				s.for_each_bond_of(static_cast<MoleculeStore::Index>(i),
					[&](std::uint32_t bond_idx) {
						const BondRecord& br = s.bond(bond_idx);
						if (!s.is_bond_dead(bond_idx) && br.order == p.order_filter)
							++count;
					});
				out[i] = cmp_(p.op, count, static_cast<int>(p.n)) ? 1 : 0;
			}
		}
	}

	// Ancestor-walk leaves (ResidueNamePred / ChainPred / etc.) currently
	// stay always-false. K0.5.4 reaches the same behavioural coverage by
	// LOWERING those source-level predicates into OwnedPred at parse time
	// (using the v1.x ResiduePredicate / ChainPredicate / etc. instances),
	// rather than adding a separate per-leaf back-pointer walk for each
	// kind. The K0.5.1 leaf evaluators below remain reachable only if a
	// caller hand-builds an AST that uses these node kinds directly.
	void eval_(const SolventLeaf&,    const MoleculeStore& s, std::vector<Byte>& out) { std::memset(out.data(), 0, s.size()); }
	void eval_(const BackboneLeaf&,   const MoleculeStore& s, std::vector<Byte>& out) { std::memset(out.data(), 0, s.size()); }
	void eval_(const ResidueNamePred&,const MoleculeStore& s, std::vector<Byte>& out) { std::memset(out.data(), 0, s.size()); }
	void eval_(const ResidueIDPred&,  const MoleculeStore& s, std::vector<Byte>& out) { std::memset(out.data(), 0, s.size()); }
	void eval_(const ChainPred&,      const MoleculeStore& s, std::vector<Byte>& out) { std::memset(out.data(), 0, s.size()); }
	void eval_(const ProteinPred&,    const MoleculeStore& s, std::vector<Byte>& out) { std::memset(out.data(), 0, s.size()); }
	void eval_(const MoleculePred&,   const MoleculeStore& s, std::vector<Byte>& out) { std::memset(out.data(), 0, s.size()); }
	void eval_(const HybridLeaf&,     const MoleculeStore& s, std::vector<Byte>& out) { std::memset(out.data(), 0, s.size()); }

	// K0.5.4+K0.5.8: OwnedPred slow path. Build a FRESH v1.x predicate
	// instance per evaluate() (Codex Round 6 OPEN-2), so stateful
	// predicates (ring caches, SMARTS state) can't leak across calls
	// or across cached re-uses. The factory is the v1.x CreationMethod
	// (`void* (*)()`); we wrap the result in unique_ptr for cleanup.
	// One virtual call per atom — strictly slower than column scan,
	// but the only way to evaluate ring/SMARTS/user-registered preds.
	void eval_(const OwnedPred& p, const MoleculeStore& s, std::vector<Byte>& out)
	{
		if (p.factory == nullptr) { std::memset(out.data(), 0, s.size()); return; }
		std::unique_ptr<ExpressionPredicate> pred(
			reinterpret_cast<ExpressionPredicate*>((*p.factory)()));
		if (pred == nullptr) { std::memset(out.data(), 0, s.size()); return; }
		pred->setArgument(String(p.argument.c_str()));
		const Byte* freed = s.is_freed_data();
		const std::size_t n = s.size();
		for (std::size_t i = 0; i < n; ++i)
		{
			if (freed[i]) { out[i] = 0; continue; }
			Atom* a = s.back_ptr(static_cast<MoleculeStore::Index>(i));
			out[i] = (a != nullptr && (*pred)(*a)) ? 1 : 0;
		}
	}

	// Inner-node evaluators. AND = bitwise AND of child bitmaps;
	// OR  = bitwise OR; NOT = invert (live atoms only).
	// 2026-05-18 (R6 FYI-6 fix): the per-call `std::vector<Byte> tmp(n)`
	// was replaced with ScratchScope, which borrows a depth-indexed
	// thread-local buffer that retains its capacity across evaluations.
	void eval_(const std::unique_ptr<AndNode>& node,
	           const MoleculeStore& s, std::vector<Byte>& out)
	{
		const std::size_t n = s.size();
		if (node->children.empty()) { std::memset(out.data(), 1, n); return; }
		eval_node_(node->children[0], s, out);
		if (node->children.size() == 1) return;
		ScratchScope scratch(n);
		std::vector<Byte>& tmp = scratch.buf;
		for (std::size_t c = 1; c < node->children.size(); ++c)
		{
			eval_node_(node->children[c], s, tmp);
			for (std::size_t i = 0; i < n; ++i) out[i] &= tmp[i];
		}
	}
	void eval_(const std::unique_ptr<OrNode>& node,
	           const MoleculeStore& s, std::vector<Byte>& out)
	{
		const std::size_t n = s.size();
		if (node->children.empty()) { std::memset(out.data(), 0, n); return; }
		eval_node_(node->children[0], s, out);
		if (node->children.size() == 1) return;
		ScratchScope scratch(n);
		std::vector<Byte>& tmp = scratch.buf;
		for (std::size_t c = 1; c < node->children.size(); ++c)
		{
			eval_node_(node->children[c], s, tmp);
			for (std::size_t i = 0; i < n; ++i) out[i] |= tmp[i];
		}
	}
	void eval_(const std::unique_ptr<NotNode>& node,
	           const MoleculeStore& s, std::vector<Byte>& out)
	{
		const std::size_t n = s.size();
		eval_node_(node->child, s, out);
		const Byte* freed = s.is_freed_data();
		for (std::size_t i = 0; i < n; ++i)
			out[i] = (!freed[i] && !out[i]) ? 1 : 0;
	}

	void eval_node_(const PredNode& node,
	                const MoleculeStore& store,
	                std::vector<Byte>& out)
	{
		std::visit([&](const auto& v) { eval_(v, store, out); }, node);
	}

	// Per-atom evaluator (K0.5.1 evaluate_one path). Mirrors the bitmap
	// leaves but operates on a single live atom; used by the legacy
	// Expression::operator()(const Atom&) API surface.
	bool eval_one_(const PredNode& node, const Atom& atom);

	bool eval1_(const TrueLeaf&,     const Atom&) { return true; }
	bool eval1_(const FalseLeaf&,    const Atom&) { return false; }
	bool eval1_(const SelectedLeaf&, const Atom& a) {
		const MoleculeStore* s = a.getStore();
		return s != nullptr && s->selected(a.getStoreIndex());
	}
	bool eval1_(const ElementPred& p, const Atom& a) {
		const MoleculeStore* s = a.getStore();
		if (s == nullptr) return false;
		return s->element_index(a.getStoreIndex()) == p.element_idx;
	}
	bool eval1_(const AtomNamePred& p, const Atom& a) {
		const MoleculeStore* s = a.getStore();
		if (s == nullptr) return false;
		return s->name_offset(a.getStoreIndex()) == p.name_offset;
	}
	bool eval1_(const AtomTypePred& p, const Atom& a) {
		const MoleculeStore* s = a.getStore();
		if (s == nullptr) return false;
		return s->type_name_offset(a.getStoreIndex()) == p.type_name_offset;
	}
	bool eval1_(const ChargeRange& p, const Atom& a) {
		return cmp_(p.op, a.getCharge(), p.value);
	}
	bool eval1_(const NumberOfBondsPred& p, const Atom& a) {
		if (p.order_filter == 0)
			return cmp_(p.op, static_cast<int>(a.countBonds()), static_cast<int>(p.n));
		// Filter-by-order on the handle's bond_[] array.
		int count = 0;
		for (std::size_t i = 0; i < a.countBonds(); ++i)
		{
			const Bond* b = a.getBond(static_cast<Position>(i));
			if (b != nullptr && b->getOrder() == p.order_filter) ++count;
		}
		return cmp_(p.op, count, static_cast<int>(p.n));
	}

	bool eval1_(const SolventLeaf&,     const Atom&) { return false; }
	bool eval1_(const BackboneLeaf&,    const Atom&) { return false; }
	bool eval1_(const ResidueNamePred&, const Atom&) { return false; }
	bool eval1_(const ResidueIDPred&,   const Atom&) { return false; }
	bool eval1_(const ChainPred&,       const Atom&) { return false; }
	bool eval1_(const ProteinPred&,     const Atom&) { return false; }
	bool eval1_(const MoleculePred&,    const Atom&) { return false; }
	bool eval1_(const HybridLeaf&,      const Atom&) { return false; }
	bool eval1_(const OwnedPred& p, const Atom& a) {
		// K0.5.4+K0.5.8: build a fresh predicate per call (avoids
		// stateful-predicate cross-call corruption — Codex R6 OPEN-2).
		if (p.factory == nullptr) return false;
		std::unique_ptr<ExpressionPredicate> pred(
			reinterpret_cast<ExpressionPredicate*>((*p.factory)()));
		if (pred == nullptr) return false;
		pred->setArgument(String(p.argument.c_str()));
		return (*pred)(a);
	}

	bool eval1_(const std::unique_ptr<AndNode>& node, const Atom& a) {
		for (const PredNode& c : node->children)
			if (!eval_one_(c, a)) return false;
		return true;
	}
	bool eval1_(const std::unique_ptr<OrNode>& node, const Atom& a) {
		for (const PredNode& c : node->children)
			if (eval_one_(c, a)) return true;
		return false;
	}
	bool eval1_(const std::unique_ptr<NotNode>& node, const Atom& a) {
		return !eval_one_(node->child, a);
	}

	bool eval_one_(const PredNode& node, const Atom& atom)
	{
		return std::visit([&](const auto& v) { return eval1_(v, atom); }, node);
	}
} // namespace

void CompiledExpression::evaluate(const MoleculeStore& store,
                                  std::vector<std::uint8_t>& out_bitmap) const
{
	out_bitmap.assign(store.size(), 0);
	eval_node_(root_, store, out_bitmap);
}

bool CompiledExpression::evaluate_one(const Atom& atom) const
{
	return eval_one_(root_, atom);
}

// ============================================================
// K0.5.2 — parser -> AST lowerer
// ============================================================
namespace
{
	// Trim leading/trailing ASCII whitespace.
	std::string trim_(const std::string& s)
	{
		std::size_t b = 0, e = s.size();
		while (b < e && std::isspace(static_cast<unsigned char>(s[b]))) ++b;
		while (e > b && std::isspace(static_cast<unsigned char>(s[e - 1]))) --e;
		return s.substr(b, e - b);
	}

	// Parse a comparison-prefixed numeric argument:
	//   "<0.5", "<=0.5", "=0.5", "==0.5", ">=0.5", ">0.5", or bare "0.5" -> EQ.
	// Returns true on success and writes op + value. Throws on malformed.
	bool parse_cmp_(const std::string& raw, CmpOp& op, std::string& tail)
	{
		const std::string s = trim_(raw);
		if (s.empty()) return false;
		std::size_t i = 0;
		if (s[0] == '<')
		{
			if (s.size() > 1 && s[1] == '=') { op = CmpOp::LE; i = 2; }
			else                              { op = CmpOp::LT; i = 1; }
		}
		else if (s[0] == '>')
		{
			if (s.size() > 1 && s[1] == '=') { op = CmpOp::GE; i = 2; }
			else                              { op = CmpOp::GT; i = 1; }
		}
		else if (s[0] == '=')
		{
			op = CmpOp::EQ;
			i = (s.size() > 1 && s[1] == '=') ? 2 : 1;
		}
		else
		{
			op = CmpOp::EQ;
		}
		tail = trim_(s.substr(i));
		return !tail.empty();
	}

	float parse_float_(const std::string& s)
	{
		try { return std::stof(s); }
		catch (...)
		{
			throw Exception::ParseError(__FILE__, __LINE__, s, "expected numeric argument");
		}
	}
	int parse_int_(const std::string& s)
	{
		try { return std::stoi(s); }
		catch (...)
		{
			throw Exception::ParseError(__FILE__, __LINE__, s, "expected integer argument");
		}
	}

	PredNode lower_leaf_(const ExpressionParser::SyntaxTree& node,
	                     MoleculeStore& store,
	                     const Expression* expr)
	{
		const std::string name = node.predicate.c_str();
		const std::string arg  = node.argument.c_str();

		if (name == "true")     return TrueLeaf{};
		if (name == "false")    return FalseLeaf{};
		if (name == "selected") return SelectedLeaf{};

		if (name == "element")
		{
			const Element& el = PTE[String(arg.c_str())];
			return ElementPred{static_cast<std::uint8_t>(el.getAtomicNumber())};
		}
		if (name == "name")
		{
			return AtomNamePred{store.intern_name(arg)};
		}
		if (name == "type")
		{
			return AtomTypePred{store.intern_type_name(arg)};
		}
		if (name == "charge")
		{
			CmpOp op = CmpOp::EQ;
			std::string tail;
			if (!parse_cmp_(arg, op, tail))
				throw Exception::ParseError(__FILE__, __LINE__, arg, "charge() needs an argument");
			return ChargeRange{op, parse_float_(tail)};
		}
		if (name == "numberOfBonds")
		{
			CmpOp op = CmpOp::EQ;
			std::string tail;
			if (!parse_cmp_(arg, op, tail))
				throw Exception::ParseError(__FILE__, __LINE__, arg, "numberOfBonds() needs an argument");
			return NumberOfBondsPred{op, static_cast<std::uint8_t>(parse_int_(tail)), 0};
		}

		// K0.5.4: predicate isn't one of the fast-path leaves. If an
		// Expression registry was provided AND the name is registered
		// there, lower into an OwnedPred holding (factory, name, arg).
		// K0.5.8: stores the factory pointer (not an instance) so each
		// evaluate() builds a fresh predicate — stateful v1.x predicates
		// (ring caches, SMARTS state) can't leak across evals.
		if (expr != nullptr && expr->hasPredicate(String(name.c_str())))
		{
			auto& methods = expr->getCreationMethods();
			Expression::CreationMethod factory = nullptr;
			if (methods.has(String(name.c_str())))
				factory = methods[String(name.c_str())];
			if (factory == nullptr)
			{
				throw Exception::ParseError(__FILE__, __LINE__, name,
					"K0.5.8: registered factory missing");
			}
			OwnedPred owned;
			owned.factory  = reinterpret_cast<OwnedPred::Factory>(factory);
			owned.name     = name;
			owned.argument = arg;
			return owned;
		}

		throw Exception::ParseError(__FILE__, __LINE__, name,
			"unknown predicate (not in K0.5 fast path, not registered with Expression)");
	}

	PredNode lower_node_(const ExpressionParser::SyntaxTree& node,
	                     MoleculeStore& store,
	                     const Expression* expr)
	{
		// A SyntaxTree node can be a leaf (LEAF type or no children) or an
		// AND/OR with children. `negate` wraps any node in a NotNode.
		PredNode lowered;
		if (node.type == ExpressionTree::LEAF || node.children.empty())
		{
			lowered = lower_leaf_(node, store, expr);
		}
		else if (node.type == ExpressionTree::AND)
		{
			auto a = std::make_unique<AndNode>();
			for (auto* child : node.children)
				a->children.emplace_back(lower_node_(*child, store, expr));
			lowered = std::move(a);
		}
		else if (node.type == ExpressionTree::OR)
		{
			auto o = std::make_unique<OrNode>();
			for (auto* child : node.children)
				o->children.emplace_back(lower_node_(*child, store, expr));
			lowered = std::move(o);
		}
		else
		{
			throw Exception::ParseError(__FILE__, __LINE__, node.predicate,
				"unknown SyntaxTree node type");
		}
		if (node.negate)
		{
			auto n = std::make_unique<NotNode>();
			n->child = std::move(lowered);
			lowered = std::move(n);
		}
		return lowered;
	}
} // namespace

std::shared_ptr<const CompiledExpression>
CompiledExpression::compile(MoleculeStore& store,
                            const std::string& source,
                            std::size_t pred_set_hash)
{
	ExpressionParser parser;
	parser.parse(String(source.c_str()));
	PredNode root = lower_node_(parser.getSyntaxTree(), store, /*expr=*/nullptr);
	return std::make_shared<const CompiledExpression>(
	    std::move(root), source, pred_set_hash);
}

std::shared_ptr<const CompiledExpression>
CompiledExpression::compile(MoleculeStore& store,
                            const Expression& expr,
                            std::size_t pred_set_hash)
{
	const std::string source = std::string(expr.getExpressionString().c_str());
	ExpressionParser parser;
	parser.parse(String(source.c_str()));
	PredNode root = lower_node_(parser.getSyntaxTree(), store, &expr);
	return std::make_shared<const CompiledExpression>(
	    std::move(root), source, pred_set_hash);
}

// ============================================================
// K0.5.2 — process-global LRU cache
// ============================================================
std::size_t CompiledExpressionCache::KeyHash::operator()(const Key& k) const noexcept
{
	std::size_t h = std::hash<std::string>{}(k.source);
	std::size_t p = std::hash<const void*>{}(static_cast<const void*>(k.store));
	// Mix store ptr into the hash.
	h ^= p + 0x9e3779b97f4a7c15ULL + (h << 6) + (h >> 2);
	return h;
}

struct CompiledExpressionCache::Impl
{
	using Entry  = std::pair<Key, std::shared_ptr<const CompiledExpression>>;
	using LruIt  = std::list<Entry>::iterator;

	std::list<Entry>                         lru;        // front = MRU, back = LRU
	std::unordered_map<Key, LruIt, KeyHash>  index;
	std::size_t                              capacity_ = 1024;
	// K0.5.8 (Codex Round 6 OPEN-1): mutator mutex. Pre-K0.5.8 the
	// cache had no synchronisation; concurrent get_or_compile from
	// different threads (or get_or_compile racing ~MoleculeStore's
	// invalidate_store) corrupted the LRU list + unordered_map.
	mutable std::mutex                       mtx;
};

CompiledExpressionCache::CompiledExpressionCache()
	: impl_(std::make_unique<Impl>())
{}
CompiledExpressionCache::~CompiledExpressionCache() = default;

CompiledExpressionCache& CompiledExpressionCache::instance()
{
	// 2026-05-18 (Codex R10 DTOR-HARDEN): leaky-immortal singleton.
	// Previously this was a Meyers singleton (`static CompiledExpressionCache
	// cache; return cache;`). B1.2 fixed the destruction-order crash by
	// touching this from the MoleculeStore constructor, pinning the cache
	// to construct before any store and therefore destruct after them. That
	// works but is implicit and depends on every store-creation path going
	// through that constructor — Codex R10 flagged this as fragile.
	//
	// Switching to a heap-allocated singleton that's never destroyed (the
	// classic "immortal singleton" pattern) makes the destruction-order
	// concern structurally impossible: ~MoleculeStore can call
	// instance().invalidate_store() at any point in process teardown and
	// always get a valid cache. Cost: ~few KB leaked at process exit (the
	// cache state + LRU list + index map). Acceptable for a process-global
	// singleton — exit is exit.
	//
	// This also lets the MoleculeStore() constructor pin be removed in a
	// future commit (kept for now as belt-and-suspenders).
	static CompiledExpressionCache* cache = new CompiledExpressionCache();
	return *cache;
}

// K0.5.8: cache mutators take impl_->mtx. We DROP the cache lock
// before calling CompiledExpression::compile() because compile itself
// takes the (separate) parser mutex inside parse(); holding both
// risks deadlock if parse later grows to call back into the cache.
// On the slow path we then re-acquire the cache lock and double-check
// — if another thread compiled the same entry concurrently, we use
// theirs and drop our copy.

std::shared_ptr<const CompiledExpression>
CompiledExpressionCache::get_or_compile(MoleculeStore& store, const std::string& source)
{
	Key k{source, &store};
	{
		std::lock_guard<std::mutex> lk(impl_->mtx);
		auto it = impl_->index.find(k);
		if (it != impl_->index.end())
		{
			impl_->lru.splice(impl_->lru.begin(), impl_->lru, it->second);
			return it->second->second;
		}
	}
	auto compiled = CompiledExpression::compile(store, source);
	{
		std::lock_guard<std::mutex> lk(impl_->mtx);
		auto it = impl_->index.find(k);
		if (it != impl_->index.end())
		{
			impl_->lru.splice(impl_->lru.begin(), impl_->lru, it->second);
			return it->second->second;
		}
		impl_->lru.emplace_front(k, compiled);
		impl_->index.emplace(k, impl_->lru.begin());
		while (impl_->lru.size() > impl_->capacity_)
		{
			auto& victim = impl_->lru.back();
			impl_->index.erase(victim.first);
			impl_->lru.pop_back();
		}
	}
	return compiled;
}

std::shared_ptr<const CompiledExpression>
CompiledExpressionCache::get_or_compile(MoleculeStore& store, const Expression& expr)
{
	const std::string source = std::string(expr.getExpressionString().c_str());
	Key k{source, &store};
	{
		std::lock_guard<std::mutex> lk(impl_->mtx);
		auto it = impl_->index.find(k);
		if (it != impl_->index.end())
		{
			impl_->lru.splice(impl_->lru.begin(), impl_->lru, it->second);
			return it->second->second;
		}
	}
	auto compiled = CompiledExpression::compile(store, expr);
	{
		std::lock_guard<std::mutex> lk(impl_->mtx);
		auto it = impl_->index.find(k);
		if (it != impl_->index.end())
		{
			impl_->lru.splice(impl_->lru.begin(), impl_->lru, it->second);
			return it->second->second;
		}
		impl_->lru.emplace_front(k, compiled);
		impl_->index.emplace(k, impl_->lru.begin());
		while (impl_->lru.size() > impl_->capacity_)
		{
			auto& victim = impl_->lru.back();
			impl_->index.erase(victim.first);
			impl_->lru.pop_back();
		}
	}
	return compiled;
}

void CompiledExpressionCache::invalidate(const Key& key)
{
	std::lock_guard<std::mutex> lk(impl_->mtx);
	auto it = impl_->index.find(key);
	if (it == impl_->index.end()) return;
	impl_->lru.erase(it->second);
	impl_->index.erase(it);
}

void CompiledExpressionCache::invalidate_store(MoleculeStore* store)
{
	std::lock_guard<std::mutex> lk(impl_->mtx);
	for (auto it = impl_->lru.begin(); it != impl_->lru.end(); )
	{
		if (it->first.store == store)
		{
			impl_->index.erase(it->first);
			it = impl_->lru.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void CompiledExpressionCache::clear()
{
	std::lock_guard<std::mutex> lk(impl_->mtx);
	impl_->index.clear();
	impl_->lru.clear();
}

std::size_t CompiledExpressionCache::size() const
{
	std::lock_guard<std::mutex> lk(impl_->mtx);
	return impl_->lru.size();
}
std::size_t CompiledExpressionCache::capacity() const
{
	std::lock_guard<std::mutex> lk(impl_->mtx);
	return impl_->capacity_;
}
void CompiledExpressionCache::set_capacity(std::size_t n)
{
	std::lock_guard<std::mutex> lk(impl_->mtx);
	impl_->capacity_ = n;
	while (impl_->lru.size() > impl_->capacity_)
	{
		auto& victim = impl_->lru.back();
		impl_->index.erase(victim.first);
		impl_->lru.pop_back();
	}
}

} // namespace BALL
