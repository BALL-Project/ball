// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: PDBAtomIterator.h,v 1.21 2003/06/11 08:08:53 oliver Exp $ 
//

#ifndef BALL_KERNEL_PDBATOMITERATOR_H
#define BALL_KERNEL_PDBATOMITERATOR_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_PDBATOM_H
#	include <BALL/KERNEL/PDBAtom.h>
#endif

#ifndef BALL_KERNEL_ITERATOR_H
#	include <BALL/KERNEL/iterator.h>
#endif

#ifndef BALL_KERNEL_PREDICATE_H
#	include <BALL/KERNEL/predicate.h>
#endif

namespace BALL 
{
	/** Traits class for the PDBAtomIterator.
			
    	\ingroup  KernelIterators
	*/
	class PDBAtomIteratorTraits_
		:	public CompositeIteratorTraits_
	{
		public:

		inline PDBAtomIteratorTraits_()	throw();
			
		PDBAtomIteratorTraits_(const Composite& composite) throw();
			
		inline PDBAtomIteratorTraits_(const PDBAtomIteratorTraits_& traits)	throw();
			
		PDBAtomIteratorTraits_& operator = (const PDBAtomIteratorTraits_& traits)	throw();

		void resetPredicate()	throw();
	};

	inline PDBAtomIteratorTraits_::PDBAtomIteratorTraits_() throw()
		:	CompositeIteratorTraits_()
	{
		predicate_ = &RTTI::getDefault<KernelPredicate<PDBAtom> >();
	}

	inline PDBAtomIteratorTraits_::PDBAtomIteratorTraits_(const PDBAtomIteratorTraits_& traits)	throw()
		:	CompositeIteratorTraits_(traits)
	{
	}

	inline PDBAtomIteratorTraits_::PDBAtomIteratorTraits_(const Composite& composite)	throw()
		:	CompositeIteratorTraits_(composite)
	{
		predicate_ = &RTTI::getDefault<KernelPredicate<PDBAtom> >();
	}

	inline PDBAtomIteratorTraits_& PDBAtomIteratorTraits_::operator = (const PDBAtomIteratorTraits_& traits) throw()
	{
		CompositeIteratorTraits_::operator=(traits);
		return *this;
	}

	inline void PDBAtomIteratorTraits_::resetPredicate() throw()
	{
		predicate_ = &RTTI::getDefault<KernelPredicate<PDBAtom> >();
	}


	/** @name Iterator type definitions for the various PDBAtomIterators
    	\ingroup  KernelIterators
	*/
	//@{

	/// A mutable bidirectional iterator for the PDBAtom class.
	typedef BidirectionalIterator<Composite, PDBAtom, Composite::SubcompositeIterator, PDBAtomIteratorTraits_>
		PDBAtomIterator;

	/// A constant bidirectional iterator for the PDBAtom class.
	typedef ConstBidirectionalIterator<Composite, PDBAtom, Composite::SubcompositeIterator, PDBAtomIteratorTraits_>
		PDBAtomConstIterator;

	/// A mutable reverse bidirectional iterator for the PDBAtom class.
	typedef std::reverse_iterator<PDBAtomIterator> PDBAtomReverseIterator;

	/// A constant reverse bidirectional iterator for the PDBAtom class.
	typedef std::reverse_iterator<PDBAtomConstIterator> PDBAtomConstReverseIterator;
	//@}

} // namespace BALL

#endif // BALL_KERNEL_PDBATOMITERATOR_H
