// $Id: atomContainerIterator.h,v 1.6 2002/01/04 03:19:48 oliver Exp $ 

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

	/** Traits class for the AtomContainerIterators.
			{\bf Definition:} \URL{BALL/KERNEL/atomContainerIterator.h}
	*/
	class AtomContainerIteratorTraits_
		: public CompositeIteratorTraits_
	{
		public:

		inline AtomContainerIteratorTraits_()
			throw();
			
		AtomContainerIteratorTraits_(const Composite& composite)
			throw();
			
		inline AtomContainerIteratorTraits_(const AtomContainerIteratorTraits_& traits, bool /* deep */ = true)
			throw();
			
		BALL_CREATE(AtomContainerIteratorTraits_)

		AtomContainerIteratorTraits_& operator =
			(const AtomContainerIteratorTraits_& traits)
			throw();

		void resetPredicate()
			throw();
	};


	inline
	AtomContainerIteratorTraits_::AtomContainerIteratorTraits_()
		throw()
		:	CompositeIteratorTraits_()
	{
		predicate_ = &RTTI::getDefault<KernelPredicate<AtomContainer> >();
	}
		
	inline
	AtomContainerIteratorTraits_::AtomContainerIteratorTraits_(const Composite& composite)
		throw()
		:	CompositeIteratorTraits_(composite)
	{
		predicate_ = &RTTI::getDefault<KernelPredicate<AtomContainer> >();
	}
		
	inline
	AtomContainerIteratorTraits_::AtomContainerIteratorTraits_(const AtomContainerIteratorTraits_& traits, bool /* deep */)
		throw()
		: CompositeIteratorTraits_(traits)
	{
	}
		
	inline
	AtomContainerIteratorTraits_& AtomContainerIteratorTraits_::operator =
		(const AtomContainerIteratorTraits_& traits)
		throw()
	{
		CompositeIteratorTraits_::operator=(traits);
		return *this;
	}

	inline
	void AtomContainerIteratorTraits_::resetPredicate()
		throw()
	{
		predicate_ = &RTTI::getDefault<KernelPredicate<AtomContainer> >();
	}


	/** @name Iterator type definitions for the various AtomContainerIterators.
	*/
	//@{

	/** A mutable bidirectional iterator for the AtomContainer class.
	*/
	typedef BidirectionalIterator
		<Composite, AtomContainer, Composite::SubcompositeIterator, AtomContainerIteratorTraits_>
		AtomContainerIterator;

	/** A constant bidirectional iterator for the AtomContainer class.
	*/
	typedef ConstBidirectionalIterator
		<Composite, AtomContainer, Composite::SubcompositeIterator, AtomContainerIteratorTraits_>
		AtomContainerConstIterator;

	/** A mutable reverse bidirectional iterator for the AtomContainer class.
	*/
	typedef ReverseBidirectionalIterator
		<Composite, AtomContainer, Composite::SubcompositeIterator, AtomContainerIteratorTraits_>
		AtomContainerReverseIterator;

	/** A constant reverse bidirectional iterator for the AtomContainer class.
	*/
	typedef ConstReverseBidirectionalIterator
		<Composite, AtomContainer, Composite::SubcompositeIterator, AtomContainerIteratorTraits_>
	AtomContainerConstReverseIterator;

	//@}

} // namespace BALL

#endif // BALL_KERNEL_FRAGMENTITERATOR_H
