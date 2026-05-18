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

#include <cstring>

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

	// Leaves that K0.5.1 does not yet implement. Treat as "always false"
	// for now so partially-compiled expressions evaluate without crashing;
	// the spec earmarks K0.5.1.b / K0.5.4 for the real implementations.
	void eval_(const SolventLeaf&,    const MoleculeStore& s, std::vector<Byte>& out) { std::memset(out.data(), 0, s.size()); }
	void eval_(const BackboneLeaf&,   const MoleculeStore& s, std::vector<Byte>& out) { std::memset(out.data(), 0, s.size()); }
	void eval_(const ResidueNamePred&,const MoleculeStore& s, std::vector<Byte>& out) { std::memset(out.data(), 0, s.size()); }
	void eval_(const ResidueIDPred&,  const MoleculeStore& s, std::vector<Byte>& out) { std::memset(out.data(), 0, s.size()); }
	void eval_(const ChainPred&,      const MoleculeStore& s, std::vector<Byte>& out) { std::memset(out.data(), 0, s.size()); }
	void eval_(const ProteinPred&,    const MoleculeStore& s, std::vector<Byte>& out) { std::memset(out.data(), 0, s.size()); }
	void eval_(const MoleculePred&,   const MoleculeStore& s, std::vector<Byte>& out) { std::memset(out.data(), 0, s.size()); }
	void eval_(const HybridLeaf&,     const MoleculeStore& s, std::vector<Byte>& out) { std::memset(out.data(), 0, s.size()); }
	void eval_(const OwnedPred&,      const MoleculeStore& s, std::vector<Byte>& out) { std::memset(out.data(), 0, s.size()); }

	// Inner-node evaluators. AND = bitwise AND of child bitmaps;
	// OR  = bitwise OR; NOT = invert (live atoms only).
	void eval_(const std::unique_ptr<AndNode>& node,
	           const MoleculeStore& s, std::vector<Byte>& out)
	{
		const std::size_t n = s.size();
		if (node->children.empty()) { std::memset(out.data(), 1, n); return; }
		eval_node_(node->children[0], s, out);
		std::vector<Byte> tmp(n);
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
		std::vector<Byte> tmp(n);
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
	bool eval1_(const OwnedPred&,       const Atom&) { return false; }

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

} // namespace BALL
