// $Id: secondaryStructureIterator.h,v 1.3 1999/12/30 20:30:41 oliver Exp $

#ifndef BALL_KERNEL_SECONDARYSTRUCTUREITERATOR_H
#define BALL_KERNEL_SECONDARYSTRUCTUREITERATOR_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_SECONDARY_STRUCTURE_H
#	include <BALL/KERNEL/secondaryStructure.h>
#endif

#ifndef BALL_KERNEL_ITERATOR_H
#	include <BALL/KERNEL/iterator.h>
#endif

#ifndef BALL_KERNEL_PREDICATE_H
#	include <BALL/KERNEL/predicate.h>
#endif

namespace BALL 
{

	class SecondaryStructureIteratorTraits_
		: public CompositeIteratorTraits_
	{
		public:
		
		BALL_CREATE(SecondaryStructureIteratorTraits_)

		SecondaryStructureIteratorTraits_()
			:	CompositeIteratorTraits_()
		{
			predicate_ = &RTTI::getDefault<KernelPredicate<SecondaryStructure> >();
		}
			
		SecondaryStructureIteratorTraits_(const Composite& composite)
			:	CompositeIteratorTraits_(composite)
		{
			predicate_ = &RTTI::getDefault<KernelPredicate<SecondaryStructure> >();
		}
			
		SecondaryStructureIteratorTraits_(const SecondaryStructureIteratorTraits_& traits, bool /* deep */ = true)
			:	CompositeIteratorTraits_(traits)
		{
		}
			
		SecondaryStructureIteratorTraits_ &operator = (const SecondaryStructureIteratorTraits_& traits)
		{
			CompositeIteratorTraits_::operator = (traits);
			return *this;
		}

		void resetPredicate()
		{
			predicate_ = &RTTI::getDefault<KernelPredicate<SecondaryStructure> >();
		}
	};



	typedef BidirectionalIterator
		<Composite, SecondaryStructure, Composite::SubcompositeIterator, SecondaryStructureIteratorTraits_>
		SecondaryStructureIterator;

	typedef ConstBidirectionalIterator
		<Composite, SecondaryStructure, Composite::SubcompositeIterator, SecondaryStructureIteratorTraits_>
		SecondaryStructureConstIterator;

	typedef ReverseBidirectionalIterator
		<Composite, SecondaryStructure, Composite::SubcompositeIterator, SecondaryStructureIteratorTraits_>
		SecondaryStructureReverseIterator;

	typedef ConstReverseBidirectionalIterator
		<Composite, SecondaryStructure, Composite::SubcompositeIterator, SecondaryStructureIteratorTraits_>
		SecondaryStructureConstReverseIterator;

} // namespace BALL 

#endif // BALL_KERNEL_SECONDARYSTRUCTUREITERATOR_H
