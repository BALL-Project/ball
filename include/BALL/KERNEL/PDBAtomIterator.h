// $Id: PDBAtomIterator.h,v 1.4 2000/01/10 15:51:00 oliver Exp $ 

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

namespace BALL 
{

	class PDBAtomIteratorTraits_
		:	public CompositeIteratorTraits_
	{
		public:

		BALL_CREATE(PDBAtomIteratorTraits_)

		PDBAtomIteratorTraits_()
			:	CompositeIteratorTraits_()
		{
			predicate_ = &RTTI::getDefault<KernelPredicate<PDBAtom> >();
		}
			
		PDBAtomIteratorTraits_(const Composite& composite)
			:	CompositeIteratorTraits_(composite)
		{
			predicate_ = &RTTI::getDefault<KernelPredicate<PDBAtom> >();
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
			predicate_ = &RTTI::getDefault<KernelPredicate<PDBAtom> >();
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
