// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: atomContainerIterator.h,v 1.17 2003/06/19 13:09:54 oliver Exp $ 
//

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
			
    	\ingroup  KernelIterators
	*/
	class AtomContainerIteratorTraits
		: public CompositeIteratorTraits
	{
		public:

		inline AtomContainerIteratorTraits()	throw();
			
		AtomContainerIteratorTraits(const Composite& composite) throw();
			
		inline AtomContainerIteratorTraits(const AtomContainerIteratorTraits& traits)	throw();
			
		AtomContainerIteratorTraits& operator = (const AtomContainerIteratorTraits& traits)	throw();

		void resetPredicate()	throw();
	};


	inline AtomContainerIteratorTraits::AtomContainerIteratorTraits()	throw()
		:	CompositeIteratorTraits()
	{
		predicate_ = &RTTI::getDefault<KernelPredicate<AtomContainer> >();
	}
		
	inline AtomContainerIteratorTraits::AtomContainerIteratorTraits(const Composite& composite) throw()
		:	CompositeIteratorTraits(composite)
	{
		predicate_ = &RTTI::getDefault<KernelPredicate<AtomContainer> >();
	}
		
	inline AtomContainerIteratorTraits::AtomContainerIteratorTraits(const AtomContainerIteratorTraits& traits)	throw()
		: CompositeIteratorTraits(traits)
	{
	}
		
	inline
	AtomContainerIteratorTraits& AtomContainerIteratorTraits::operator =
		(const AtomContainerIteratorTraits& traits)
		throw()
	{
		CompositeIteratorTraits::operator=(traits);
		return *this;
	}

	inline
	void AtomContainerIteratorTraits::resetPredicate()
		throw()
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
} // namespace BALL

#endif // BALL_KERNEL_FRAGMENTITERATOR_H
