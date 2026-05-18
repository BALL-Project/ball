// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// BALL 2.0 KERNEL — K0.5 compiled selection (sub-phase K0.5.0).
// Skeleton implementation: ctor/dtor/move plumbing only. evaluate()
// and evaluate_one() are stubbed to false; K0.5.1 will fill them in
// with the bitmap inner loop.
//

#include <BALL/KERNEL/compiledExpression.h>
#include <BALL/KERNEL/moleculeStore.h>

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

// K0.5.1 will implement: walk root_, write bitmap[i] for live atoms.
// For now the skeleton just sizes the output to match the store.
void CompiledExpression::evaluate(const MoleculeStore& store,
                                  std::vector<std::uint8_t>& out_bitmap) const
{
	out_bitmap.assign(store.size(), 0);
}

// K0.5.1 will implement. Skeleton: always false.
bool CompiledExpression::evaluate_one(const Atom& /*atom*/) const
{
	return false;
}

} // namespace BALL
