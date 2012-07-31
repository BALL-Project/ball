// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: atomIterator.h,v 1.21 2005/10/23 12:02:17 oliver Exp $
//

#ifndef BALL_KERNEL_ATOMITERATOR_H
#define BALL_KERNEL_ATOMITERATOR_H

#ifndef BALL_KERNEL_ATOM_H
#	include <BALL/KERNEL/atom.h>
#endif

#ifndef BALL_KERNEL_ITERATOR_H
#	include <BALL/KERNEL/iterator.h>
#endif

#ifndef BALL_KERNEL_PREDICATE_H
#	include <BALL/KERNEL/predicate.h>
#endif

#ifndef BALL_CONCEPT_STDITERATORWRAPPER_H
#	include <BALL/CONCEPT/stdIteratorWrapper.h>
#endif

namespace BALL
{
	/** Traits class for the AtomIterators
			
    	\ingroup  KernelIterators
	*/
	class BALL_EXPORT AtomIteratorTraits
		: public CompositeIteratorTraits
	{
		public:

		inline AtomIteratorTraits()
			:	CompositeIteratorTraits()
		{
			predicate_ = &RTTI::getDefault<KernelPredicate<Atom> >();
		}

		inline AtomIteratorTraits(const AtomIteratorTraits& traits)

			:	CompositeIteratorTraits(traits)
		{
		}

		AtomIteratorTraits(const Composite& composite);

		AtomIteratorTraits& operator = (const AtomIteratorTraits& traits);

		void resetPredicate();
	};


	inline AtomIteratorTraits::AtomIteratorTraits(const Composite& composite)
		:	CompositeIteratorTraits(composite)
	{
		predicate_ = &RTTI::getDefault<KernelPredicate<Atom> >();
	}

	inline AtomIteratorTraits& AtomIteratorTraits::operator = (const AtomIteratorTraits& traits)
	{
		CompositeIteratorTraits::operator = (traits);
		return *this;
	}

	inline void AtomIteratorTraits::resetPredicate()
	{
		predicate_ = &RTTI::getDefault<KernelPredicate<Atom> >();
	}


	/** @name Iterator type definitions for the various AtomIterators
    	\ingroup  KernelIterators
	*/
	//@{

	/** A mutable bidirectional iterator for the Atom class.
	*/
	typedef BidirectionalIterator
		<Composite, Atom, Composite::CompositeIterator, AtomIteratorTraits>
		AtomIterator;

	/** A constant bidirectional iterator for the Atom class.
	*/
	typedef ConstBidirectionalIterator
		<Composite, Atom, Composite::CompositeIterator, AtomIteratorTraits>
		AtomConstIterator;

	/// A mutable reverse bidirectional iterator for the Atom class.
	typedef std::reverse_iterator<AtomIterator> AtomReverseIterator;

	/// A constant reverse bidirectional iterator for the Atom class.
	typedef std::reverse_iterator<AtomConstIterator> AtomConstReverseIterator;

	//@}

	IteratorWrapper(Atom)
	ConstIteratorWrapper(Atom)
} // namespace BALL

#endif // BALL_KERNEL_ATOMITERATOR_H

