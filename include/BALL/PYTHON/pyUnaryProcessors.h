#ifndef BALL_PYTHON_PYUNARYPROCESSORS_H
#define BALL_PYTHON_PYUNARYPROCESSORS_H

#include <BALL/KERNEL/atomContainer.h>

namespace BALL
{
	using AtomContainerProcessor = UnaryProcessor<AtomContainer>;
	using AtomProcessor = UnaryProcessor<Atom>;
	using BondProcessor = UnaryProcessor<Bond>;
	using ChainProcessor = UnaryProcessor<Chain>;
	using CompositeProcessor = UnaryProcessor<Composite>;
	using FragmentProcessor = UnaryProcessor<Fragment>;
	using ResidueProcessor = UnaryProcessor<Residue>;

	using ConstAtomProcessor = ConstUnaryProcessor<Atom>;
	using ConstCompositeProcessor = ConstUnaryProcessor<Composite>;
}

#endif //BALL_PYTHON_PYUNARYPROCESSORS_H
