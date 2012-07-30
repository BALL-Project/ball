// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: nucleicAcidIterator.h,v 1.11 2005/10/23 12:02:19 oliver Exp $
//

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
	class NucleicAcid;

	/** NucleicAcidIteratorTraits
	
			\ingroup  KernelIterators
	*/
	class BALL_EXPORT NucleicAcidIteratorTraits
		: public CompositeIteratorTraits
	{
		public:

		NucleicAcidIteratorTraits()
			:	CompositeIteratorTraits()
		{
			predicate_ = &RTTI::getDefault<KernelPredicate<NucleicAcid> >();
		}
			
		NucleicAcidIteratorTraits(const Composite& composite)
			:	CompositeIteratorTraits(composite)
		{
			predicate_ = &RTTI::getDefault<KernelPredicate<NucleicAcid> >();
		}
			
		NucleicAcidIteratorTraits(const NucleicAcidIteratorTraits& traits, bool /* deep */ = true)
			:	CompositeIteratorTraits(traits)
		{
		}
			
		NucleicAcidIteratorTraits& operator =(const NucleicAcidIteratorTraits& traits)
		{
			CompositeIteratorTraits::operator=(traits);
			return *this;
		}

		void resetPredicate()
		{
			predicate_ = &RTTI::getDefault<KernelPredicate<NucleicAcid> >();
		}
	};



	typedef BidirectionalIterator
		<Composite, NucleicAcid, Composite::CompositeIterator, NucleicAcidIteratorTraits>
		NucleicAcidIterator;

	typedef ConstBidirectionalIterator
		<Composite, NucleicAcid, Composite::CompositeIterator, NucleicAcidIteratorTraits>
		NucleicAcidConstIterator;

	typedef std::reverse_iterator<NucleicAcidIterator> NucleicAcidReverseIterator;

	typedef std::reverse_iterator<NucleicAcidConstIterator> NucleicAcidConstReverseIterator;

	IteratorWrapper(NucleicAcid)
	ConstIteratorWrapper(NucleicAcid)
} // namespace BALL

#endif // BALL_KERNEL_NUCLEICACIDITERATOR_H
