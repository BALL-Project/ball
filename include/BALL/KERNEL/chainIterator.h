// $Id: chainIterator.h,v 1.6 2001/07/03 21:00:32 anker Exp $

#ifndef BALL_KERNEL_CHAINITERATOR_H
#define BALL_KERNEL_CHAINITERATOR_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

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
			{\bf Definition:} \URL{BALL/KERNEL/chainIterator.h}
	*/
	class ChainIteratorTraits_
		: public CompositeIteratorTraits_
	{

		public:

		BALL_CREATE_DEEP(ChainIteratorTraits_)

		/// BAUSTELLE
		ChainIteratorTraits_()
			throw();
			
		/// BAUSTELLE
		ChainIteratorTraits_(const Composite& composite)
			throw();
			
		/// BAUSTELLE
		ChainIteratorTraits_(const ChainIteratorTraits_& traits, bool /* deep */ = true)
			throw();
			
		/// BAUSTELLE
		ChainIteratorTraits_& operator = (const ChainIteratorTraits_& traits)
			throw();

		/// BAUSTELLE
		void resetPredicate()
			throw();
		
	};

	ChainIteratorTraits_::ChainIteratorTraits_()
		throw()
		:	CompositeIteratorTraits_()
	{
		predicate_ = &RTTI::getDefault<KernelPredicate<Chain> >();
	}
		
	ChainIteratorTraits_::ChainIteratorTraits_(const Composite& composite)
		throw()
		:	CompositeIteratorTraits_(composite)
	{
		predicate_ = &RTTI::getDefault<KernelPredicate<Chain> >();
	}
		
	ChainIteratorTraits_::ChainIteratorTraits_(const ChainIteratorTraits_& traits, bool /* deep */)
		throw()
		:	CompositeIteratorTraits_(traits)
	{
	}
		
	ChainIteratorTraits_& ChainIteratorTraits_::operator = (const ChainIteratorTraits_& traits)
		throw()
	{
		CompositeIteratorTraits_::operator=(traits);
		return *this;
	}

	void ChainIteratorTraits_::resetPredicate()
		throw()
	{
		predicate_ = &RTTI::getDefault<KernelPredicate<Chain> >();
	}


	/** @name Iterator type definitions for the various ChainIterators
	*/
	//@{

	/** A mutable bidirectional iterator for the Chain class.
	*/
	typedef BidirectionalIterator
		<Composite, Chain, Composite::SubcompositeIterator, ChainIteratorTraits_>
		ChainIterator;

	/** A constant bidirectional iterator for the Chain class.
	*/
	typedef ConstBidirectionalIterator
		<Composite, Chain, Composite::SubcompositeIterator, ChainIteratorTraits_>
		ChainConstIterator;

	/** A mutable reverse bidirectional iterator for the Chain class.
	*/
	typedef ReverseBidirectionalIterator
		<Composite, Chain, Composite::SubcompositeIterator, ChainIteratorTraits_>
		ChainReverseIterator;

	/** A constant reverse bidirectional iterator for the Chain class.
	*/
	typedef ConstReverseBidirectionalIterator
		<Composite, Chain, Composite::SubcompositeIterator, ChainIteratorTraits_>
		ChainConstReverseIterator;
	
	//@}

} // namespace BALL 

#endif // BALL_KERNEL_CHAINITERATOR_H
