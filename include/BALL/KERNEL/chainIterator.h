// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: chainIterator.h,v 1.20 2003/08/26 08:04:17 oliver Exp $
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

namespace BALL 
{
	/** Traits class for the various ChainIterators.
			
    	\ingroup  KernelIterators
	*/
	class ChainIteratorTraits
		: public CompositeIteratorTraits
	{

		public:

		inline ChainIteratorTraits()	throw();
			
		ChainIteratorTraits(const Composite& composite) throw();
			
		inline ChainIteratorTraits(const ChainIteratorTraits& traits)	throw();
			
		ChainIteratorTraits& operator = (const ChainIteratorTraits& traits)	throw();

		void resetPredicate() throw();
	};

	inline ChainIteratorTraits::ChainIteratorTraits() throw()
		:	CompositeIteratorTraits()
	{
		predicate_ = &RTTI::getDefault<KernelPredicate<Chain> >();
	}
		
	inline ChainIteratorTraits::ChainIteratorTraits(const Composite& composite)	throw()
		:	CompositeIteratorTraits(composite)
	{
		predicate_ = &RTTI::getDefault<KernelPredicate<Chain> >();
	}
		
	inline ChainIteratorTraits::ChainIteratorTraits(const ChainIteratorTraits& traits)	throw()
		:	CompositeIteratorTraits(traits)
	{
	}
		
	inline ChainIteratorTraits& ChainIteratorTraits::operator = (const ChainIteratorTraits& traits) throw()
	{
		CompositeIteratorTraits::operator=(traits);
		return *this;
	}

	inline void ChainIteratorTraits::resetPredicate() throw()
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
} // namespace BALL 

#endif // BALL_KERNEL_CHAINITERATOR_H
