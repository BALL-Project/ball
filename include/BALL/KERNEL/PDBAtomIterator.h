// $Id: PDBAtomIterator.h,v 1.2 1999/12/04 18:34:16 oliver Exp $ 

#ifndef BALL_KERNEL_PDBATOMITERATOR_H
#define BALL_KERNEL_PDBATOMITERATOR_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_PDBATOM_H
#	include <BALL/KERNEL/PDBAtom.h>
#endif

#ifndef BALL_KERNEL_ITERATOR_H
#	include <BALL/KERNEL/iterator.h>
#endif

#ifndef BALL_KERNEL_PREDICATE_H
#	include <BALL/KERNEL/predicate.h>
#endif

namespace BALL {

	class PDBAtomIteratorTraits_
		:	public CompositeIteratorTraits_
	{
		public:

		BALL_CREATE(PDBAtomIteratorTraits_)

		PDBAtomIteratorTraits_()
			:	CompositeIteratorTraits_()
		{
			predicate_ = &RTTI<KernelPredicate<PDBAtom> >::getDefault();
		}
			
		PDBAtomIteratorTraits_(const Composite& composite)
			:	CompositeIteratorTraits_(composite)
		{
			predicate_ = &RTTI<KernelPredicate<PDBAtom> >::getDefault();
		}
			
		PDBAtomIteratorTraits_(const PDBAtomIteratorTraits_& traits, bool /* deep */ = true)
			:	CompositeIteratorTraits_(traits)
		{
		}
			
		PDBAtomIteratorTraits_ &operator = (const PDBAtomIteratorTraits_ &traits)
		{
			CompositeIteratorTraits_::operator=(traits);
			return *this;
		}

		void resetPredicate()
		{
			predicate_ = &RTTI<KernelPredicate<PDBAtom> >::getDefault();
		}

	};



	typedef BidirectionalIterator
		<Composite, PDBAtom, Composite::SubcompositeIterator, PDBAtomIteratorTraits_>
		PDBAtomIterator;

	typedef ConstBidirectionalIterator
		<Composite, PDBAtom, Composite::SubcompositeIterator, PDBAtomIteratorTraits_>
		PDBAtomConstIterator;

	typedef ReverseBidirectionalIterator
		<Composite, PDBAtom, Composite::SubcompositeIterator, PDBAtomIteratorTraits_>
		PDBAtomReverseIterator;

	typedef ConstReverseBidirectionalIterator
		<Composite, PDBAtom, Composite::SubcompositeIterator, PDBAtomIteratorTraits_>
		PDBAtomConstReverseIterator;

} // namespace BALL

#endif // BALL_KERNEL_PDBATOMITERATOR_H
