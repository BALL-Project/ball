// $Id: fragmentIterator.h,v 1.5 2001/07/15 20:23:57 oliver Exp $

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

	/** Traits class for the FragmentIterator.
			Intended for internal use only!
			\\
			{\bf Definition:} \URL{BALL/KERNEL/PDBAtomIterator.h}
	*/
	class FragmentIteratorTraits_
		: public CompositeIteratorTraits_
	{
		public:

		BALL_CREATE_DEEP(FragmentIteratorTraits_)

		FragmentIteratorTraits_()
			throw();

		FragmentIteratorTraits_(const Composite& composite)
			throw();

		FragmentIteratorTraits_(const FragmentIteratorTraits_& traits, bool /* deep */ = true)
			throw();

		FragmentIteratorTraits_ &operator =(const FragmentIteratorTraits_& traits)
			throw();

		void resetPredicate()
			throw();
	};

	inline
	FragmentIteratorTraits_::FragmentIteratorTraits_()
		throw()
		:	CompositeIteratorTraits_()
	{
		predicate_ = &RTTI::getDefault<KernelPredicate<Fragment> >();
	}

	inline
	FragmentIteratorTraits_::FragmentIteratorTraits_(const Composite& composite)
		throw()
		:	CompositeIteratorTraits_(composite)
	{
		predicate_ = &RTTI::getDefault<KernelPredicate<Fragment> >();
	}

	inline
	FragmentIteratorTraits_::FragmentIteratorTraits_(const FragmentIteratorTraits_& traits, bool /* deep */)
		throw()
		:	CompositeIteratorTraits_(traits)
	{
	}

	inline
	FragmentIteratorTraits_& FragmentIteratorTraits_::operator =(const FragmentIteratorTraits_& traits)
		throw()
	{
		CompositeIteratorTraits_::operator=(traits);
		return *this;
	}

	inline
	void FragmentIteratorTraits_::resetPredicate()
		throw()
	{
		predicate_ = &RTTI::getDefault<KernelPredicate<Fragment> >();
	}

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
