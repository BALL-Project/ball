// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: fragmentIterator.h,v 1.20 2005/10/23 12:02:18 oliver Exp $
//

#ifndef BALL_KERNEL_FRAGMENTITERATOR_H
#define BALL_KERNEL_FRAGMENTITERATOR_H

#ifndef BALL_KERNEL_ITERATOR_H
#	include <BALL/KERNEL/iterator.h>
#endif

#ifndef BALL_KERNEL_PREDICATE_H
# include <BALL/KERNEL/predicate.h>
#endif

#ifndef BALL_CONCEPT_STDITERATORWRAPPER_H
# include <BALL/CONCEPT/stdIteratorWrapper.h>
#endif

namespace BALL 
{
	/** Traits class for the FragmentIterator.
			Intended for internal use only!
			 \par
    	\ingroup  KernelIterators
	*/
	class BALL_EXPORT FragmentIteratorTraits
		: public CompositeIteratorTraits
	{
		public:

		inline FragmentIteratorTraits();

		FragmentIteratorTraits(const Composite& composite);

		inline FragmentIteratorTraits(const FragmentIteratorTraits& traits);

		FragmentIteratorTraits& operator = (const FragmentIteratorTraits& traits);

		void resetPredicate();
	};

	inline FragmentIteratorTraits::FragmentIteratorTraits()
		:	CompositeIteratorTraits()
	{
		predicate_ = &RTTI::getDefault<KernelPredicate<Fragment> >();
	}

	inline FragmentIteratorTraits::FragmentIteratorTraits(const Composite& composite)
		:	CompositeIteratorTraits(composite)
	{
		predicate_ = &RTTI::getDefault<KernelPredicate<Fragment> >();
	}

	inline FragmentIteratorTraits::FragmentIteratorTraits(const FragmentIteratorTraits& traits)
		:	CompositeIteratorTraits(traits)
	{
	}

	inline FragmentIteratorTraits& FragmentIteratorTraits::operator = (const FragmentIteratorTraits& traits)
	{
		CompositeIteratorTraits::operator=(traits);
		return *this;
	}

	inline void FragmentIteratorTraits::resetPredicate()
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

	IteratorWrapper(Fragment)
	ConstIteratorWrapper(Fragment)

} // namespace BALL 
#endif // BALL_KERNEL_FRAGMENTITERATOR_H
