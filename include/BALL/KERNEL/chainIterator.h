// $Id: chainIterator.h,v 1.3 1999/12/30 20:30:40 oliver Exp $

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

	class ChainIteratorTraits_
		: public CompositeIteratorTraits_
	{
		public:

		BALL_CREATE(ChainIteratorTraits_)

		ChainIteratorTraits_()
			:	CompositeIteratorTraits_()
		{
			predicate_ = &RTTI::getDefault<KernelPredicate<Chain> >();
		}
			
		ChainIteratorTraits_(const Composite& composite)
			:	CompositeIteratorTraits_(composite)
		{
			predicate_ = &RTTI::getDefault<KernelPredicate<Chain> >();
		}
			
		ChainIteratorTraits_(const ChainIteratorTraits_& traits, bool /* deep */ = true)
			:	CompositeIteratorTraits_(traits)
		{
		}
			
		ChainIteratorTraits_& operator = (const ChainIteratorTraits_& traits)
		{
			CompositeIteratorTraits_::operator=(traits);
			return *this;
		}

		void resetPredicate()
		{
			predicate_ = &RTTI::getDefault<KernelPredicate<Chain> >();
		}
		
		protected:

		private:
	};



	typedef BidirectionalIterator
		<Composite, Chain, Composite::SubcompositeIterator, ChainIteratorTraits_>
		ChainIterator;

	typedef ConstBidirectionalIterator
		<Composite, Chain, Composite::SubcompositeIterator, ChainIteratorTraits_>
		ChainConstIterator;

	typedef ReverseBidirectionalIterator
		<Composite, Chain, Composite::SubcompositeIterator, ChainIteratorTraits_>
		ChainReverseIterator;

	typedef ConstReverseBidirectionalIterator
		<Composite, Chain, Composite::SubcompositeIterator, ChainIteratorTraits_>
		ChainConstReverseIterator;

} // namespace BALL 

#endif // BALL_KERNEL_CHAINITERATOR_H
