// $Id: residueIterator.h,v 1.1 1999/08/26 07:53:19 oliver Exp $

#ifndef BALL_KERNEL_RESIDUEITERATOR_H
#define BALL_KERNEL_RESIDUEITERATOR_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_RESIDUE_H
#	include <BALL/KERNEL/residue.h>
#endif

#ifndef BALL_KERNEL_ITERATOR_H
#	include <BALL/KERNEL/iterator.h>
#endif

#ifndef BALL_KERNEL_PREDICATE_H
#	include <BALL/KERNEL/predicate.h>
#endif

namespace BALL {

	class ResidueIteratorTraits_
		: public CompositeIteratorTraits_
	{
		public:

		BALL_CREATE(ResidueIteratorTraits_)

		ResidueIteratorTraits_()
			:	CompositeIteratorTraits_()
		{
			predicate_ = &RTTI<KernelPredicate<Residue> >::getDefault();
		}
			
		ResidueIteratorTraits_(const Composite& composite)
			:	CompositeIteratorTraits_(composite)
		{
			predicate_ = &RTTI<KernelPredicate<Residue> >::getDefault();
		}
			
		ResidueIteratorTraits_(const ResidueIteratorTraits_& traits, bool /* deep */ = true)
			:	CompositeIteratorTraits_(traits)
		{
		}
			
		ResidueIteratorTraits_& operator =(const ResidueIteratorTraits_& traits)
		{
			CompositeIteratorTraits_::operator=(traits);
			return *this;
		}

		void resetPredicate()
		{
			predicate_ = &RTTI<KernelPredicate<Residue> >::getDefault();
		}
	};



	typedef BidirectionalIterator
		<Composite, Residue, Composite::SubcompositeIterator, ResidueIteratorTraits_>
		ResidueIterator;

	typedef ConstBidirectionalIterator
		<Composite, Residue, Composite::SubcompositeIterator, ResidueIteratorTraits_>
		ResidueConstIterator;

	typedef ReverseBidirectionalIterator
		<Composite, Residue, Composite::SubcompositeIterator, ResidueIteratorTraits_>
		ResidueReverseIterator;

	typedef ConstReverseBidirectionalIterator
		<Composite, Residue, Composite::SubcompositeIterator, ResidueIteratorTraits_>
		ResidueConstReverseIterator;

} // namespace BALL

#endif // BALL_KERNEL_RESIDUEITERATOR_H
