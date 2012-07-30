// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: chainIterator.h,v 1.21 2005/10/23 12:02:18 oliver Exp $
//

#ifndef BALL_KERNEL_CHAINITERATOR_H
#define BALL_KERNEL_CHAINITERATOR_H

#ifndef BALL_KERNEL_CHAIN_H
#	include <BALL/KERNEL/chain.h>
#endif

#ifndef BALL_KERNEL_ITERATOR_H
#	include <BALL/KERNEL/iterator.h>
#endif

#ifndef BALL_KERNEL_PREDICATE_H
#	include <BALL/KERNEL/predicate.h>
#endif

#ifndef BALL_CONCEPT_STDITERATORWRAPPER_H
# include <BALL/CONCEPT/stdIteratorWrapper.h>
#endif

namespace BALL
{
	/** Traits class for the various ChainIterators.
			
	    \ingroup  KernelIterators
	    @todo Complete Documentation
	*/
	class BALL_EXPORT ChainIteratorTraits
		: public CompositeIteratorTraits
	{

		public:

		inline ChainIteratorTraits();

		ChainIteratorTraits(const Composite& composite);

		inline ChainIteratorTraits(const ChainIteratorTraits& traits);

		ChainIteratorTraits& operator = (const ChainIteratorTraits& traits);

		void resetPredicate();
	};

	inline ChainIteratorTraits::ChainIteratorTraits()
		:	CompositeIteratorTraits()
	{
		predicate_ = &RTTI::getDefault<KernelPredicate<Chain> >();
	}

	inline ChainIteratorTraits::ChainIteratorTraits(const Composite& composite)
		:	CompositeIteratorTraits(composite)
	{
		predicate_ = &RTTI::getDefault<KernelPredicate<Chain> >();
	}

	inline ChainIteratorTraits::ChainIteratorTraits(const ChainIteratorTraits& traits)
		:	CompositeIteratorTraits(traits)
	{
	}

	inline ChainIteratorTraits& ChainIteratorTraits::operator = (const ChainIteratorTraits& traits)
	{
		CompositeIteratorTraits::operator=(traits);
		return *this;
	}

	inline void ChainIteratorTraits::resetPredicate()
	{
		predicate_ = &RTTI::getDefault<KernelPredicate<Chain> >();
	}

	/** @name Iterator type definitions for the various ChainIterators
    	\ingroup  KernelIterators
	*/
	//@{

	/// A mutable bidirectional iterator for the Chain class.
	typedef BidirectionalIterator<Composite, Chain, Composite::CompositeIterator, ChainIteratorTraits>
		ChainIterator;

	/// A constant bidirectional iterator for the Chain class.
	typedef ConstBidirectionalIterator<Composite, Chain, Composite::CompositeIterator, ChainIteratorTraits>
		ChainConstIterator;

	/// A mutable reverse bidirectional iterator for the Chain class.
	typedef std::reverse_iterator<ChainIterator> ChainReverseIterator;

	/// A constant reverse bidirectional iterator for the Chain class.
	typedef std::reverse_iterator<ChainConstIterator> ChainConstReverseIterator;
	//@}

	IteratorWrapper(Chain)
	ConstIteratorWrapper(Chain)
} // namespace BALL 

#endif // BALL_KERNEL_CHAINITERATOR_H

