// $Id: PDBAtomIterator.h,v 1.6 2001/07/03 20:47:27 anker Exp $ 

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
	*/
	class PDBAtomIteratorTraits_
		:	public CompositeIteratorTraits_
	{
		public:

		BALL_CREATE_DEEP(PDBAtomIteratorTraits_)

		/// BAUSTELLE
		PDBAtomIteratorTraits_()
			throw();
			
		/// BAUSTELLE
		PDBAtomIteratorTraits_(const Composite& composite)
			throw();
			
		/// BAUSTELLE
		PDBAtomIteratorTraits_(const PDBAtomIteratorTraits_& traits, 
				bool /* deep */ = true)
			throw();
			
		/// BAUSTELLE
		PDBAtomIteratorTraits_ &operator = (const PDBAtomIteratorTraits_ &traits)
			throw();

		/// BAUSTELLE
		void resetPredicate()
			throw();

	};

	PDBAtomIteratorTraits_::PDBAtomIteratorTraits_()
		throw()
		:	CompositeIteratorTraits_()
	{
		predicate_ = &RTTI::getDefault<KernelPredicate<PDBAtom> >();
	}
			
	PDBAtomIteratorTraits_::PDBAtomIteratorTraits_(const Composite& composite)
		throw()
		:	CompositeIteratorTraits_(composite)
	{
		predicate_ = &RTTI::getDefault<KernelPredicate<PDBAtom> >();
	}
		
	PDBAtomIteratorTraits_::PDBAtomIteratorTraits_(const PDBAtomIteratorTraits_& traits, bool /* deep */)
		throw()
		:	CompositeIteratorTraits_(traits)
	{
	}
		
	PDBAtomIteratorTraits_& PDBAtomIteratorTraits_::operator = (const PDBAtomIteratorTraits_ &traits)
		throw()
	{
		CompositeIteratorTraits_::operator=(traits);
		return *this;
	}

	void PDBAtomIteratorTraits_::resetPredicate()
		throw()
	{
		predicate_ = &RTTI::getDefault<KernelPredicate<PDBAtom> >();
	}


	/** @name Iterator type definitions for the various PDBAtomIterators
	*/
	//@{

	/** A mutable bidirectional iterator for the PDBAtom class.
	*/
	typedef BidirectionalIterator<Composite, PDBAtom, Composite::SubcompositeIterator, PDBAtomIteratorTraits_>
		PDBAtomIterator;

	/** A constant bidirectional iterator for the PDBAtom class.
	*/
	typedef ConstBidirectionalIterator
		<Composite, PDBAtom, Composite::SubcompositeIterator, PDBAtomIteratorTraits_>
		PDBAtomConstIterator;

	/** A mutable reverse bidirectional iterator for the PDBAtom class.
	*/
	typedef ReverseBidirectionalIterator
		<Composite, PDBAtom, Composite::SubcompositeIterator, PDBAtomIteratorTraits_>
		PDBAtomReverseIterator;

	/** A constant reverse bidirectional iterator for the PDBAtom class.
	*/
	typedef ConstReverseBidirectionalIterator
		<Composite, PDBAtom, Composite::SubcompositeIterator, PDBAtomIteratorTraits_>
		PDBAtomConstReverseIterator;

	//@}

} // namespace BALL

#endif // BALL_KERNEL_PDBATOMITERATOR_H
