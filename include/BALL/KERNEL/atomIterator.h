// $Id: atomIterator.h,v 1.5 2001/07/03 20:47:28 anker Exp $

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
	*/
	class AtomIteratorTraits_
		: public CompositeIteratorTraits_ 
	{
		public:

		BALL_CREATE_DEEP(AtomIteratorTraits_)

		/// BAUSTELLE
		AtomIteratorTraits_()
			throw();
			
		/// BAUSTELLE
		AtomIteratorTraits_(const Composite& composite)
			throw();

		/// BAUSTELLE
		AtomIteratorTraits_(const AtomIteratorTraits_& traits, bool /* deep */)
			throw();
			
		/// BAUSTELLE
		AtomIteratorTraits_& operator = (const AtomIteratorTraits_& traits)
			throw();

		/// BAUSTELLE
		void resetPredicate()
			throw();

	};


	AtomIteratorTraits_::AtomIteratorTraits_()
		throw()
		:	CompositeIteratorTraits_()
	{
		predicate_ = &RTTI::getDefault<KernelPredicate<Atom> >();
	}
		
	AtomIteratorTraits_::AtomIteratorTraits_(const Composite& composite)
		throw()
		:	CompositeIteratorTraits_(composite)
	{
		predicate_ = &RTTI::getDefault<KernelPredicate<Atom> >();
	}
		
	AtomIteratorTraits_::AtomIteratorTraits_(const AtomIteratorTraits_& traits, bool /* deep */ = true)
		throw()
		:	CompositeIteratorTraits_(traits)
	{
	}
		
	AtomIteratorTraits_& AtomIteratorTraits_::operator = (const AtomIteratorTraits_& traits)
		throw()
	{
		CompositeIteratorTraits_::operator=(traits);
		return *this;
	}

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
