// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: proteinIterator.h,v 1.12 2003/08/26 08:04:19 oliver Exp $
//

#ifndef BALL_KERNEL_PROTEINITERATOR_H
#define BALL_KERNEL_PROTEINITERATOR_H

#ifndef BALL_KERNEL_ITERATOR_H
#	include <BALL/KERNEL/iterator.h>
#endif

#ifndef BALL_KERNEL_PROTEIN_H
# include <BALL/KERNEL/protein.h>
#endif

#ifndef BALL_KERNEL_PREDICATE_H
# include <BALL/KERNEL/predicate.h>
#endif

namespace BALL 
{
  /** ProteinIteratorTraits 
			
			\ingroup  KernelIterators
	*/
	class ProteinIteratorTraits
		: public CompositeIteratorTraits
	{
		public:

		ProteinIteratorTraits()
			:	CompositeIteratorTraits()
		{
			predicate_ = &RTTI::getDefault<KernelPredicate<Protein> >();
		}
			
		ProteinIteratorTraits(const Composite& composite)
			:	CompositeIteratorTraits(composite)
		{
			predicate_ = &RTTI::getDefault<KernelPredicate<Protein> >();
		}
			
		ProteinIteratorTraits(const ProteinIteratorTraits& traits, bool /* deep */ = true)
			:	CompositeIteratorTraits(traits)
		{
		}
			
		ProteinIteratorTraits& operator = (const ProteinIteratorTraits& traits)
		{
			CompositeIteratorTraits::operator=(traits);
			return *this;
		}

		void resetPredicate()
		{
			predicate_ = &RTTI::getDefault<KernelPredicate<Protein> >();
		}
	};



	typedef BidirectionalIterator
		<Composite, Protein, Composite::CompositeIterator,ProteinIteratorTraits>
		ProteinIterator;

	typedef ConstBidirectionalIterator
		<Composite, Protein, Composite::CompositeIterator, ProteinIteratorTraits>
		ProteinConstIterator;

	typedef std::reverse_iterator<ProteinIterator> ProteinReverseIterator;

	typedef std::reverse_iterator<ProteinConstIterator> ProteinConstReverseIterator;
} // namespace BALL 

#endif // BALL_KERNEL_PROTEINITERATOR_H
