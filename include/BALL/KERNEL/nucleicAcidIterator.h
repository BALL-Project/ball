// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: nucleicAcidIterator.h,v 1.4 2003/02/25 15:53:15 sturm Exp $

#ifndef BALL_KERNEL_NUCLEICACIDITERATOR_H
#define BALL_KERNEL_NUCLEICACIDITERATOR_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_NUCLEOTIDE_H
#	include <BALL/KERNEL/nucleicAcid.h>
#endif

#ifndef BALL_KERNEL_ITERATOR_H
#	include <BALL/KERNEL/iterator.h>
#endif

#ifndef BALL_KERNEL_PREDICATE_H
#	include <BALL/KERNEL/predicate.h>
#endif

namespace BALL 
{
    /** \ingroup KernelIterators
     *  @{
     */
	class NucleicAcid;

	class NucleicAcidIteratorTraits_
		: public CompositeIteratorTraits_
	{
		public:

		BALL_CREATE_DEEP(NucleicAcidIteratorTraits_)

		NucleicAcidIteratorTraits_()
			:	CompositeIteratorTraits_()
		{
			predicate_ = &RTTI::getDefault<KernelPredicate<NucleicAcid> >();
		}
			
		NucleicAcidIteratorTraits_(const Composite& composite)
			:	CompositeIteratorTraits_(composite)
		{
			predicate_ = &RTTI::getDefault<KernelPredicate<NucleicAcid> >();
		}
			
		NucleicAcidIteratorTraits_(const NucleicAcidIteratorTraits_& traits, bool /* deep */ = true)
			:	CompositeIteratorTraits_(traits)
		{
		}
			
		NucleicAcidIteratorTraits_& operator =(const NucleicAcidIteratorTraits_& traits)
		{
			CompositeIteratorTraits_::operator=(traits);
			return *this;
		}

		void resetPredicate()
		{
			predicate_ = &RTTI::getDefault<KernelPredicate<NucleicAcid> >();
		}
	};



	typedef BidirectionalIterator
		<Composite, NucleicAcid, Composite::SubcompositeIterator, NucleicAcidIteratorTraits_>
		NucleicAcidIterator;

	typedef ConstBidirectionalIterator
		<Composite, NucleicAcid, Composite::SubcompositeIterator, NucleicAcidIteratorTraits_>
		NucleicAcidConstIterator;

	typedef ReverseBidirectionalIterator
		<Composite, NucleicAcid, Composite::SubcompositeIterator, NucleicAcidIteratorTraits_>
		NucleicAcidReverseIterator;

	typedef ConstReverseBidirectionalIterator
		<Composite, NucleicAcid, Composite::SubcompositeIterator, NucleicAcidIteratorTraits_>
		NucleicAcidConstReverseIterator;
  /** @} */
} // namespace BALL

#endif // BALL_KERNEL_NUCLEICACIDITERATOR_H
