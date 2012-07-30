// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: proteinIterator.h,v 1.15 2005/10/23 12:02:20 oliver Exp $
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
	class BALL_EXPORT ProteinIteratorTraits
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


	///
	typedef BidirectionalIterator
		<Composite, Protein, Composite::CompositeIterator,ProteinIteratorTraits>
		ProteinIterator;

	///
	typedef ConstBidirectionalIterator
		<Composite, Protein, Composite::CompositeIterator, ProteinIteratorTraits>
		ProteinConstIterator;

	///
	typedef std::reverse_iterator<ProteinIterator> ProteinReverseIterator;

	///
	typedef std::reverse_iterator<ProteinConstIterator> ProteinConstReverseIterator;

	IteratorWrapper(Protein)
	ConstIteratorWrapper(Protein)
} // namespace BALL 

#endif // BALL_KERNEL_PROTEINITERATOR_H
