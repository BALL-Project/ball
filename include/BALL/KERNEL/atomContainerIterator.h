// $Id: atomContainerIterator.h,v 1.1 2000/08/30 19:58:05 oliver Exp $ 

#ifndef BALL_KERNEL_ATOMCONTAINERITERATOR_H
#define BALL_KERNEL_ATOMCONTAINERITERATOR_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_ATOMCONTAINER_H
#	include <BALL/KERNEL/atomContainer.h>
#endif

#ifndef BALL_KERNEL_ITERATOR_H
#	include <BALL/KERNEL/iterator.h>
#endif

#ifndef BALL_KERNEL_PREDICATE_H
#	include <BALL/KERNEL/predicate.h>
#endif

namespace BALL 
{

	class AtomContainer;
	class AtomContainerIteratorTraits_
		: public CompositeIteratorTraits_
	{
		public:

		BALL_CREATE_DEEP(AtomContainerIteratorTraits_)

		AtomContainerIteratorTraits_()
			:	CompositeIteratorTraits_()
		{
			predicate_ = &RTTI::getDefault<KernelPredicate<AtomContainer> >();
		}
			
		AtomContainerIteratorTraits_(const Composite& composite)
			:	CompositeIteratorTraits_(composite)
		{
			predicate_ = &RTTI::getDefault<KernelPredicate<AtomContainer> >();
		}
			
		AtomContainerIteratorTraits_(const AtomContainerIteratorTraits_& traits, bool /* deep */ = true)
			: CompositeIteratorTraits_(traits)
		{
		}
			
		AtomContainerIteratorTraits_ &operator =
			(const AtomContainerIteratorTraits_& traits)
		{
			CompositeIteratorTraits_::operator=(traits);
			return *this;
		}

		void resetPredicate()
		{
			predicate_ = &RTTI::getDefault<KernelPredicate<AtomContainer> >();
		}
	
	};



	typedef BidirectionalIterator
		<Composite, AtomContainer, Composite::SubcompositeIterator, AtomContainerIteratorTraits_>
		AtomContainerIterator;

	typedef ConstBidirectionalIterator
		<Composite, AtomContainer, Composite::SubcompositeIterator, AtomContainerIteratorTraits_>
		AtomContainerConstIterator;

	typedef ReverseBidirectionalIterator
		<Composite, AtomContainer, Composite::SubcompositeIterator, AtomContainerIteratorTraits_>
		AtomContainerReverseIterator;

	typedef ConstReverseBidirectionalIterator
		<Composite, AtomContainer, Composite::SubcompositeIterator, AtomContainerIteratorTraits_>
	AtomContainerConstReverseIterator;

} // namespace BALL

#endif // BALL_KERNEL_FRAGMENTITERATOR_H
