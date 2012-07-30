// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: residueIterator.h,v 1.15 2005/10/23 12:02:20 oliver Exp $
//

#ifndef BALL_KERNEL_RESIDUEITERATOR_H
#define BALL_KERNEL_RESIDUEITERATOR_H

#ifndef BALL_KERNEL_ITERATOR_H
#	include <BALL/KERNEL/iterator.h>
#endif

#ifndef BALL_KERNEL_PREDICATE_H
# include <BALL/KERNEL/predicate.h>
#endif

namespace BALL 
{
	/** ResidueIteratorTraits
    
			\ingroup  KernelIterators
	*/
	class BALL_EXPORT ResidueIteratorTraits
		: public CompositeIteratorTraits
	{
		public:

		ResidueIteratorTraits()
			:	CompositeIteratorTraits()
		{
			predicate_ = &RTTI::getDefault<KernelPredicate<Residue> >();
		}
			
		ResidueIteratorTraits(const Composite& composite)
			:	CompositeIteratorTraits(composite)
		{
			predicate_ = &RTTI::getDefault<KernelPredicate<Residue> >();
		}
			
		ResidueIteratorTraits(const ResidueIteratorTraits& traits, bool /* deep */ = true)
			:	CompositeIteratorTraits(traits)
		{
		}
			
		ResidueIteratorTraits& operator =(const ResidueIteratorTraits& traits)
		{
			CompositeIteratorTraits::operator=(traits);
			return *this;
		}

		void resetPredicate()
		{
			predicate_ = &RTTI::getDefault<KernelPredicate<Residue> >();
		}
	};


	///
	typedef BidirectionalIterator
		<Composite, Residue, Composite::CompositeIterator, ResidueIteratorTraits>
		ResidueIterator;

	///
	typedef ConstBidirectionalIterator
		<Composite, Residue, Composite::CompositeIterator, ResidueIteratorTraits>
		ResidueConstIterator;

	///
	typedef std::reverse_iterator<ResidueIterator> ResidueReverseIterator;

	///
	typedef std::reverse_iterator<ResidueConstIterator> ResidueConstReverseIterator;

	IteratorWrapper(Residue)
	ConstIteratorWrapper(Residue)
} // namespace BALL

#endif // BALL_KERNEL_RESIDUEITERATOR_H
