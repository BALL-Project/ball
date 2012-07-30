// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: atomContainerIterator.h,v 1.21 2005/10/23 12:02:17 oliver Exp $ 
//

#ifndef BALL_KERNEL_ATOMCONTAINERITERATOR_H
#define BALL_KERNEL_ATOMCONTAINERITERATOR_H

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
	class AtomContainer;

	/** Traits class for the AtomContainerIterators.
			
    	\ingroup  KernelIterators
	*/
	class BALL_EXPORT AtomContainerIteratorTraits
		: public CompositeIteratorTraits
	{
		public:

		inline AtomContainerIteratorTraits();

		AtomContainerIteratorTraits(const Composite& composite);

		inline AtomContainerIteratorTraits(const AtomContainerIteratorTraits& traits);

		AtomContainerIteratorTraits& operator = (const AtomContainerIteratorTraits& traits);

		void resetPredicate();
	};


	inline AtomContainerIteratorTraits::AtomContainerIteratorTraits()
		:	CompositeIteratorTraits()
	{
		predicate_ = &RTTI::getDefault<KernelPredicate<AtomContainer> >();
	}

	inline AtomContainerIteratorTraits::AtomContainerIteratorTraits(const Composite& composite)
		:	CompositeIteratorTraits(composite)
	{
		predicate_ = &RTTI::getDefault<KernelPredicate<AtomContainer> >();
	}

	inline AtomContainerIteratorTraits::AtomContainerIteratorTraits(const AtomContainerIteratorTraits& traits)
		: CompositeIteratorTraits(traits)
	{
	}

	inline
	AtomContainerIteratorTraits& AtomContainerIteratorTraits::operator =
		(const AtomContainerIteratorTraits& traits)
	{
		CompositeIteratorTraits::operator=(traits);
		return *this;
	}

	inline
	void AtomContainerIteratorTraits::resetPredicate()
	{
		predicate_ = &RTTI::getDefault<KernelPredicate<AtomContainer> >();
	}


	/** @name Iterator type definitions for the various AtomContainerIterators.
    	\ingroup  KernelIterators
	*/
	//@{

	/** A mutable bidirectional iterator for the AtomContainer class.
	*/
	typedef BidirectionalIterator
		<Composite, AtomContainer, Composite::CompositeIterator, AtomContainerIteratorTraits>
		AtomContainerIterator;

	/** A constant bidirectional iterator for the AtomContainer class.
	*/
	typedef ConstBidirectionalIterator
		<Composite, AtomContainer, Composite::CompositeIterator, AtomContainerIteratorTraits>
		AtomContainerConstIterator;

	/// A mutable reverse bidirectional iterator for the AtomContainer class.
	typedef std::reverse_iterator<AtomContainerIterator> AtomContainerReverseIterator;

	/// A constant reverse bidirectional iterator for the AtomContainer class.
	typedef std::reverse_iterator<AtomContainerConstIterator> AtomContainerConstReverseIterator;

	//@}

	IteratorWrapper(AtomContainer)
	ConstIteratorWrapper(AtomContainer)

} // namespace BALL

#endif // BALL_KERNEL_ATOMCONTAINERITERATOR_H
