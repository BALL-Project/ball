// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: secondaryStructureIterator.h,v 1.10 2003/06/11 16:09:24 oliver Exp $
//

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
	/** SecondaryStructureIteratorTraits
	 		
			\ingroup KernelIterators
	*/
	class SecondaryStructureIteratorTraits
		: public CompositeIteratorTraits
	{
		public:
		
		SecondaryStructureIteratorTraits()
			:	CompositeIteratorTraits()
		{
			predicate_ = &RTTI::getDefault<KernelPredicate<SecondaryStructure> >();
		}
			
		SecondaryStructureIteratorTraits(const Composite& composite)
			:	CompositeIteratorTraits(composite)
		{
			predicate_ = &RTTI::getDefault<KernelPredicate<SecondaryStructure> >();
		}
			
		SecondaryStructureIteratorTraits(const SecondaryStructureIteratorTraits& traits, bool /* deep */ = true)
			:	CompositeIteratorTraits(traits)
		{
		}
			
		SecondaryStructureIteratorTraits &operator = (const SecondaryStructureIteratorTraits& traits)
		{
			CompositeIteratorTraits::operator = (traits);
			return *this;
		}

		void resetPredicate()
		{
			predicate_ = &RTTI::getDefault<KernelPredicate<SecondaryStructure> >();
		}
	};



	typedef BidirectionalIterator
		<Composite, SecondaryStructure, Composite::SubcompositeIterator, SecondaryStructureIteratorTraits>
		SecondaryStructureIterator;

	typedef ConstBidirectionalIterator
		<Composite, SecondaryStructure, Composite::SubcompositeIterator, SecondaryStructureIteratorTraits>
		SecondaryStructureConstIterator;

	typedef std::reverse_iterator<SecondaryStructureIterator> SecondaryStructureReverseIterator;

	typedef std::reverse_iterator<SecondaryStructureConstIterator> SecondaryStructureConstReverseIterator;

} // namespace BALL 

#endif // BALL_KERNEL_SECONDARYSTRUCTUREITERATOR_H
