// $Id: nucleotideIterator.h,v 1.3 2000/08/30 19:58:10 oliver Exp $

#ifndef BALL_KERNEL_NUCLEOTIDEITERATOR_H
#define BALL_KERNEL_NUCLEOTIDEITERATOR_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_NUCLEOTIDE_H
#	include <BALL/KERNEL/nucleotide.h>
#endif

#ifndef BALL_KERNEL_ITERATOR_H
#	include <BALL/KERNEL/iterator.h>
#endif

#ifndef BALL_KERNEL_PREDICATE_H
#	include <BALL/KERNEL/predicate.h>
#endif

namespace BALL 
{

	class Nucleotide;

	class NucleotideIteratorTraits_
		: public CompositeIteratorTraits_
	{
		public:

		BALL_CREATE_DEEP(NucleotideIteratorTraits_)

		NucleotideIteratorTraits_()
			:	CompositeIteratorTraits_()
		{
			predicate_ = &RTTI::getDefault<KernelPredicate<Nucleotide> >();
		}
			
		NucleotideIteratorTraits_(const Composite& composite)
			:	CompositeIteratorTraits_(composite)
		{
			predicate_ = &RTTI::getDefault<KernelPredicate<Nucleotide> >();
		}
			
		NucleotideIteratorTraits_(const NucleotideIteratorTraits_& traits, bool /* deep */ = true)
			:	CompositeIteratorTraits_(traits)
		{
		}
			
		NucleotideIteratorTraits_& operator =(const NucleotideIteratorTraits_& traits)
		{
			CompositeIteratorTraits_::operator=(traits);
			return *this;
		}

		void resetPredicate()
		{
			predicate_ = &RTTI::getDefault<KernelPredicate<Nucleotide> >();
		}
	};



	typedef BidirectionalIterator
		<Composite, Nucleotide, Composite::SubcompositeIterator, NucleotideIteratorTraits_>
		NucleotideIterator;

	typedef ConstBidirectionalIterator
		<Composite, Nucleotide, Composite::SubcompositeIterator, NucleotideIteratorTraits_>
		NucleotideConstIterator;

	typedef ReverseBidirectionalIterator
		<Composite, Nucleotide, Composite::SubcompositeIterator, NucleotideIteratorTraits_>
		NucleotideReverseIterator;

	typedef ConstReverseBidirectionalIterator
		<Composite, Nucleotide, Composite::SubcompositeIterator, NucleotideIteratorTraits_>
		NucleotideConstReverseIterator;

} // namespace BALL

#endif // BALL_KERNEL_NUCLEOTIDEITERATOR_H
