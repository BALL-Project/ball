// $Id: baseFragmentIterator.h,v 1.1 1999/08/26 07:53:19 oliver Exp $ 

#ifndef BALL_KERNEL_BASEFRAGMENTITERATOR_H
#define BALL_KERNEL_BASEFRAGMENTITERATOR_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_BASEFRAGMENT_H
#	include <BALL/KERNEL/baseFragment.h>
#endif

#ifndef BALL_KERNEL_ITERATOR_H
#	include <BALL/KERNEL/iterator.h>
#endif

#ifndef BALL_KERNEL_PREDICATE_H
#	include <BALL/KERNEL/predicate.h>
#endif

namespace BALL {

	class BaseFragment;
	class BaseFragmentIteratorTraits_
		: public CompositeIteratorTraits_
	{
		public:

		BALL_CREATE(BaseFragmentIteratorTraits_)

		BaseFragmentIteratorTraits_(void)
			:	CompositeIteratorTraits_()
		{
			predicate_ = &RTTI<KernelPredicate<BaseFragment> >::getDefault();
		}
			
		BaseFragmentIteratorTraits_(const Composite& composite)
			:	CompositeIteratorTraits_(composite)
		{
			predicate_ = &RTTI<KernelPredicate<BaseFragment> >::getDefault();
		}
			
		BaseFragmentIteratorTraits_(const BaseFragmentIteratorTraits_& traits, bool /* deep */ = true)
			: CompositeIteratorTraits_(traits)
		{
		}
			
		BaseFragmentIteratorTraits_ &operator =
			(const BaseFragmentIteratorTraits_& traits)
		{
			CompositeIteratorTraits_::operator=(traits);
			return *this;
		}

		void resetPredicate(void)
		{
			predicate_ = &RTTI<KernelPredicate<BaseFragment> >::getDefault();
		}
	
	};



	typedef BidirectionalIterator
		<Composite, BaseFragment, Composite::SubcompositeIterator, BaseFragmentIteratorTraits_>
		BaseFragmentIterator;

	typedef ConstBidirectionalIterator
		<Composite, BaseFragment, Composite::SubcompositeIterator, BaseFragmentIteratorTraits_>
		BaseFragmentConstIterator;

	typedef ReverseBidirectionalIterator
		<Composite, BaseFragment, Composite::SubcompositeIterator, BaseFragmentIteratorTraits_>
		BaseFragmentReverseIterator;

	typedef ConstReverseBidirectionalIterator
		<Composite, BaseFragment, Composite::SubcompositeIterator, BaseFragmentIteratorTraits_>
	BaseFragmentConstReverseIterator;

} // namespace BALL

#endif // BALL_KERNEL_FRAGMENTITERATOR_H
