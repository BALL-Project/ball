// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: PDBAtomIterator.h,v 1.25 2005/10/23 12:02:17 oliver Exp $ 
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

#ifndef BALL_CONCEPT_STDITERATORWRAPPER_H
# include <BALL/CONCEPT/stdIteratorWrapper.h>
#endif

namespace BALL 
{
	/** Traits class for the PDBAtomIterator.
			
    	\ingroup  KernelIterators
	*/
	class BALL_EXPORT PDBAtomIteratorTraits
		:	public CompositeIteratorTraits
	{
		public:

		inline PDBAtomIteratorTraits();
			
		PDBAtomIteratorTraits(const Composite& composite);
			
		inline PDBAtomIteratorTraits(const PDBAtomIteratorTraits& traits);
			
		PDBAtomIteratorTraits& operator = (const PDBAtomIteratorTraits& traits);

		void resetPredicate();
	};

	inline PDBAtomIteratorTraits::PDBAtomIteratorTraits()
		:	CompositeIteratorTraits()
	{
		predicate_ = &RTTI::getDefault<KernelPredicate<PDBAtom> >();
	}

	inline PDBAtomIteratorTraits::PDBAtomIteratorTraits(const PDBAtomIteratorTraits& traits)
		:	CompositeIteratorTraits(traits)
	{
	}

	inline PDBAtomIteratorTraits::PDBAtomIteratorTraits(const Composite& composite)
		:	CompositeIteratorTraits(composite)
	{
		predicate_ = &RTTI::getDefault<KernelPredicate<PDBAtom> >();
	}

	inline PDBAtomIteratorTraits& PDBAtomIteratorTraits::operator = (const PDBAtomIteratorTraits& traits)
	{
		CompositeIteratorTraits::operator=(traits);
		return *this;
	}

	inline void PDBAtomIteratorTraits::resetPredicate()
	{
		predicate_ = &RTTI::getDefault<KernelPredicate<PDBAtom> >();
	}


	/** @name Iterator type definitions for the various PDBAtomIterators
    	\ingroup  KernelIterators
	*/
	//@{

	/// A mutable bidirectional iterator for the PDBAtom class.
	typedef BidirectionalIterator<Composite, PDBAtom, Composite::CompositeIterator, PDBAtomIteratorTraits>
		PDBAtomIterator;

	/// A constant bidirectional iterator for the PDBAtom class.
	typedef ConstBidirectionalIterator<Composite, PDBAtom, Composite::CompositeIterator, PDBAtomIteratorTraits>
		PDBAtomConstIterator;

	/// A mutable reverse bidirectional iterator for the PDBAtom class.
	typedef std::reverse_iterator<PDBAtomIterator> PDBAtomReverseIterator;

	/// A constant reverse bidirectional iterator for the PDBAtom class.
	typedef std::reverse_iterator<PDBAtomConstIterator> PDBAtomConstReverseIterator;
	//@}

	IteratorWrapper(PDBAtom)
	ConstIteratorWrapper(PDBAtom)
} // namespace BALL

#endif // BALL_KERNEL_PDBATOMITERATOR_H
