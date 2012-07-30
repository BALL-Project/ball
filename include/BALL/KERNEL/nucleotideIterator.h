// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: nucleotideIterator.h,v 1.13 2005/10/23 12:02:19 oliver Exp $
//

#ifndef BALL_KERNEL_NUCLEOTIDEITERATOR_H
#define BALL_KERNEL_NUCLEOTIDEITERATOR_H

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
	class Nucleotide;

	/** NucleotideIteratorTraits
	 	  \ingroup  KernelIterators
	*/
	class BALL_EXPORT NucleotideIteratorTraits
		: public CompositeIteratorTraits
	{
		public:

		NucleotideIteratorTraits()
			:	CompositeIteratorTraits()
		{
			predicate_ = &RTTI::getDefault<KernelPredicate<Nucleotide> >();
		}
			
		NucleotideIteratorTraits(const Composite& composite)
			:	CompositeIteratorTraits(composite)
		{
			predicate_ = &RTTI::getDefault<KernelPredicate<Nucleotide> >();
		}
			
		NucleotideIteratorTraits(const NucleotideIteratorTraits& traits, bool /* deep */ = true)
			:	CompositeIteratorTraits(traits)
		{
		}
			
		NucleotideIteratorTraits& operator =(const NucleotideIteratorTraits& traits)
		{
			CompositeIteratorTraits::operator=(traits);
			return *this;
		}

		void resetPredicate()
		{
			predicate_ = &RTTI::getDefault<KernelPredicate<Nucleotide> >();
		}
	};



	typedef BidirectionalIterator
		<Composite, Nucleotide, Composite::CompositeIterator, NucleotideIteratorTraits>
		NucleotideIterator;

	typedef ConstBidirectionalIterator
		<Composite, Nucleotide, Composite::CompositeIterator, NucleotideIteratorTraits>
		NucleotideConstIterator;

	typedef std::reverse_iterator<NucleotideIterator> NucleotideReverseIterator;

	typedef std::reverse_iterator<NucleotideConstIterator> NucleotideConstReverseIterator;

	IteratorWrapper(Nucleotide)
	ConstIteratorWrapper(Nucleotide)
} // namespace BALL

#endif // BALL_KERNEL_NUCLEOTIDEITERATOR_H
