// $Id: fragmentIterator.h,v 1.3 2000/08/30 19:58:08 oliver Exp $

#ifndef BALL_KERNEL_FRAGMENTITERATOR_H
#define BALL_KERNEL_FRAGMENTITERATOR_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_FRAGMENT_H
#	include <BALL/KERNEL/fragment.h>
#endif

#ifndef BALL_KERNEL_ITERATOR_H
#	include <BALL/KERNEL/iterator.h>
#endif

#ifndef BALL_KERNEL_PREDICATE_H
#	include <BALL/KERNEL/predicate.h>
#endif

namespace BALL 
{

	class FragmentIteratorTraits_
		: public CompositeIteratorTraits_
	{
		public:

		BALL_CREATE_DEEP(FragmentIteratorTraits_)

		FragmentIteratorTraits_()
			:	CompositeIteratorTraits_()
		{
			predicate_ = &RTTI::getDefault<KernelPredicate<Fragment> >();
		}
			
		FragmentIteratorTraits_(const Composite& composite)
			:	CompositeIteratorTraits_(composite)
		{
			predicate_ = &RTTI::getDefault<KernelPredicate<Fragment> >();
		}
			
		FragmentIteratorTraits_(const FragmentIteratorTraits_& traits, bool /* deep */ = true)
			:	CompositeIteratorTraits_(traits)
		{
		}
			
		FragmentIteratorTraits_ &operator =(const FragmentIteratorTraits_& traits)
		{
			CompositeIteratorTraits_::operator=(traits);
			return *this;
		}
	
		void resetPredicate()
		{
			predicate_ = &RTTI::getDefault<KernelPredicate<Fragment> >();
		}
	};



	typedef BidirectionalIterator
		<Composite, Fragment, Composite::SubcompositeIterator, FragmentIteratorTraits_>
		FragmentIterator;

	typedef ConstBidirectionalIterator
		<Composite, Fragment, Composite::SubcompositeIterator, FragmentIteratorTraits_>
		FragmentConstIterator;

	typedef ReverseBidirectionalIterator
		<Composite, Fragment, Composite::SubcompositeIterator, FragmentIteratorTraits_>
		FragmentReverseIterator;

	typedef ConstReverseBidirectionalIterator
		<Composite, Fragment, Composite::SubcompositeIterator, FragmentIteratorTraits_>
		FragmentConstReverseIterator;

} // namespace BALL 

#endif // BALL_KERNEL_FRAGMENTITERATOR_H
