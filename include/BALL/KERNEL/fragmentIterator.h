// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: fragmentIterator.h,v 1.18 2004/05/17 14:02:00 amoll Exp $
//

#ifndef BALL_KERNEL_FRAGMENTITERATOR_H
#define BALL_KERNEL_FRAGMENTITERATOR_H

#ifndef BALL_KERNEL_ITERATOR_H
#	include <BALL/KERNEL/iterator.h>
#endif

#ifndef BALL_KERNEL_PREDICATE_H
# include <BALL/KERNEL/predicate.h>
#endif

namespace BALL 
{
	/** Traits class for the FragmentIterator.
			Intended for internal use only!
			 \par
    	\ingroup  KernelIterators
	*/
	class FragmentIteratorTraits
		: public CompositeIteratorTraits
	{
		public:

		inline FragmentIteratorTraits() throw();

		FragmentIteratorTraits(const Composite& composite)	throw();

		inline FragmentIteratorTraits(const FragmentIteratorTraits& traits)	throw();

		FragmentIteratorTraits& operator = (const FragmentIteratorTraits& traits)	throw();

		void resetPredicate()	throw();
	};

	inline FragmentIteratorTraits::FragmentIteratorTraits() throw()
		:	CompositeIteratorTraits()
	{
		predicate_ = &RTTI::getDefault<KernelPredicate<Fragment> >();
	}

	inline FragmentIteratorTraits::FragmentIteratorTraits(const Composite& composite)	throw()
		:	CompositeIteratorTraits(composite)
	{
		predicate_ = &RTTI::getDefault<KernelPredicate<Fragment> >();
	}

	inline FragmentIteratorTraits::FragmentIteratorTraits(const FragmentIteratorTraits& traits) throw()
		:	CompositeIteratorTraits(traits)
	{
	}

	inline FragmentIteratorTraits& FragmentIteratorTraits::operator = (const FragmentIteratorTraits& traits) throw()
	{
		CompositeIteratorTraits::operator=(traits);
		return *this;
	}

	inline void FragmentIteratorTraits::resetPredicate() throw()
	{
		predicate_ = &RTTI::getDefault<KernelPredicate<Fragment> >();
	}

	///
	typedef BidirectionalIterator
		<Composite, Fragment, Composite::CompositeIterator, FragmentIteratorTraits>
		FragmentIterator;

	///
	typedef ConstBidirectionalIterator
		<Composite, Fragment, Composite::CompositeIterator, FragmentIteratorTraits>
		FragmentConstIterator;

	///
	typedef std::reverse_iterator<FragmentIterator> FragmentReverseIterator;

	///
	typedef std::reverse_iterator<FragmentConstIterator> FragmentConstReverseIterator;

} // namespace BALL 
#endif // BALL_KERNEL_FRAGMENTITERATOR_H
