// $Id: atomIterator.h,v 1.8 2001/07/15 20:23:57 oliver Exp $

#ifndef BALL_KERNEL_ATOMITERATOR_H
#define BALL_KERNEL_ATOMITERATOR_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
#	include <BALL/KERNEL/atom.h>
#endif

#ifndef BALL_KERNEL_ITERATOR_H
#	include <BALL/KERNEL/iterator.h>
#endif

#ifndef BALL_KERNEL_PREDICATE_H
#	include <BALL/KERNEL/predicate.h>
#endif

namespace BALL 
{

	/** Traits class for the AtomIterators
			{\bf Definition:} \URL{BALL/KERNEL/atomIterator.h}
	*/
	class AtomIteratorTraits_
		: public CompositeIteratorTraits_ 
	{
		public:

		BALL_CREATE_DEEP(AtomIteratorTraits_)

		AtomIteratorTraits_()
			throw();
			
		AtomIteratorTraits_(const Composite& composite)
			throw();

		AtomIteratorTraits_(const AtomIteratorTraits_& traits, bool /* deep */)
			throw();
			
		AtomIteratorTraits_& operator = (const AtomIteratorTraits_& traits)
			throw();

		void resetPredicate()
			throw();
	};


	inline
	AtomIteratorTraits_::AtomIteratorTraits_()
		throw()
		:	CompositeIteratorTraits_()
	{
		predicate_ = &RTTI::getDefault<KernelPredicate<Atom> >();
	}
		
	inline
	AtomIteratorTraits_::AtomIteratorTraits_(const Composite& composite)
		throw()
		:	CompositeIteratorTraits_(composite)
	{
		predicate_ = &RTTI::getDefault<KernelPredicate<Atom> >();
	}
		
	inline
	AtomIteratorTraits_::AtomIteratorTraits_(const AtomIteratorTraits_& traits, bool /* deep */ = true)
		throw()
		:	CompositeIteratorTraits_(traits)
	{
	}
		
	inline
	AtomIteratorTraits_& AtomIteratorTraits_::operator = (const AtomIteratorTraits_& traits)
		throw()
	{
		CompositeIteratorTraits_::operator=(traits);
		return *this;
	}

	inline
	void AtomIteratorTraits_::resetPredicate()
		throw()
	{
		predicate_ = &RTTI::getDefault<KernelPredicate<Atom> >();
	}


	/** @name Iterator type definitions for the various AtomIterators
	*/
	//@{

	/** A mutable bidirectional iterator for the Atom class.
	*/
	typedef BidirectionalIterator
		<Composite, Atom, Composite::SubcompositeIterator, AtomIteratorTraits_>
		AtomIterator;

	/** A constant bidirectional iterator for the Atom class.
	*/
	typedef ConstBidirectionalIterator
		<Composite, Atom, Composite::SubcompositeIterator, AtomIteratorTraits_>
		AtomConstIterator;

	/** A mutable reverse bidirectional iterator for the Atom class.
	*/
	typedef ReverseBidirectionalIterator
		<Composite, Atom, Composite::SubcompositeIterator, AtomIteratorTraits_>
		AtomReverseIterator;

	/** A constant reverse bidirectional iterator for the Atom class.
	*/
	typedef ConstReverseBidirectionalIterator
		<Composite, Atom, Composite::SubcompositeIterator, AtomIteratorTraits_>
		AtomConstReverseIterator;
	
	//@}

} // namespace BALL

#endif // BALL_KERNEL_ATOMITERATOR_H
