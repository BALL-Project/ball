// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: atomIterator.h,v 1.16 2003/06/11 08:08:54 oliver Exp $
//

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
			
    	\ingroup  KernelIterators
	*/
	class AtomIteratorTraits_
		: public CompositeIteratorTraits_ 
	{
		public:

		inline AtomIteratorTraits_()
			throw()
			:	CompositeIteratorTraits_()
		{
			predicate_ = &RTTI::getDefault<KernelPredicate<Atom> >();
		}
			
		inline AtomIteratorTraits_(const AtomIteratorTraits_& traits)
			throw()
			:	CompositeIteratorTraits_(traits)
		{
		}
			
		AtomIteratorTraits_(const Composite& composite)
			throw();
		
		AtomIteratorTraits_& operator = (const AtomIteratorTraits_& traits)
			throw();

		void resetPredicate()
			throw();
	};


	inline AtomIteratorTraits_::AtomIteratorTraits_(const Composite& composite)
		throw()
		:	CompositeIteratorTraits_(composite)
	{
		predicate_ = &RTTI::getDefault<KernelPredicate<Atom> >();
	}
		
	inline AtomIteratorTraits_& AtomIteratorTraits_::operator = (const AtomIteratorTraits_& traits)
		throw()
	{
		CompositeIteratorTraits_::operator = (traits);
		return *this;
	}

	inline void AtomIteratorTraits_::resetPredicate()
		throw()
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
		<Composite, Atom, Composite::SubcompositeIterator, AtomIteratorTraits_>
		AtomIterator;

	/** A constant bidirectional iterator for the Atom class.
	*/
	typedef ConstBidirectionalIterator
		<Composite, Atom, Composite::SubcompositeIterator, AtomIteratorTraits_>
		AtomConstIterator;

	/// A mutable reverse bidirectional iterator for the Atom class.
	typedef std::reverse_iterator<AtomIterator> AtomReverseIterator;

	/// A constant reverse bidirectional iterator for the Atom class.
	typedef std::reverse_iterator<AtomConstIterator> AtomConstReverseIterator;
	
	//@}
} // namespace BALL

#endif // BALL_KERNEL_ATOMITERATOR_H
