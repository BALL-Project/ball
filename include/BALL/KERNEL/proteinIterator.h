// $Id: proteinIterator.h,v 1.2 1999/12/30 20:30:41 oliver Exp $

#ifndef BALL_KERNEL_PROTEINITERATOR_H
#define BALL_KERNEL_PROTEINITERATOR_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_PROTEIN_H
#	include <BALL/KERNEL/protein.h>
#endif

#ifndef BALL_KERNEL_ITERATOR_H
#	include <BALL/KERNEL/iterator.h>
#endif

#ifndef BALL_KERNEL_PREDICATE_H
#	include <BALL/KERNEL/predicate.h>
#endif

namespace BALL 
{

	class ProteinIteratorTraits_
		: public CompositeIteratorTraits_
	{
		public:

		BALL_CREATE(ProteinIteratorTraits_)

		ProteinIteratorTraits_()
			:	CompositeIteratorTraits_()
		{
			predicate_ = &RTTI::getDefault<KernelPredicate<Protein> >();
		}
			
		ProteinIteratorTraits_(const Composite& composite)
			:	CompositeIteratorTraits_(composite)
		{
			predicate_ = &RTTI::getDefault<KernelPredicate<Protein> >();
		}
			
		ProteinIteratorTraits_(const ProteinIteratorTraits_& traits, bool /* deep */ = true)
			:	CompositeIteratorTraits_(traits)
		{
		}
			
		ProteinIteratorTraits_& operator = (const ProteinIteratorTraits_& traits)
		{
			CompositeIteratorTraits_::operator=(traits);
			return *this;
		}

		void resetPredicate()
		{
			predicate_ = &RTTI::getDefault<KernelPredicate<Protein> >();
		}
	};



	typedef BidirectionalIterator
		<Composite, Protein, Composite::SubcompositeIterator,ProteinIteratorTraits_>
		ProteinIterator;

	typedef ConstBidirectionalIterator
		<Composite, Protein, Composite::SubcompositeIterator, ProteinIteratorTraits_>
		ProteinConstIterator;

	typedef ReverseBidirectionalIterator
		<Composite, Protein, Composite::SubcompositeIterator, ProteinIteratorTraits_>
		ProteinReverseIterator;

	typedef ConstReverseBidirectionalIterator
		<Composite, Protein, Composite::SubcompositeIterator, ProteinIteratorTraits_>
		ProteinConstReverseIterator;
	
} // namespace BALL 

#endif // BALL_KERNEL_PROTEINITERATOR_H
