// $Id: atomIterator.h,v 1.3 1999/12/30 20:30:40 oliver Exp $

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

	class AtomIteratorTraits_
		: public CompositeIteratorTraits_ 
	{
		public:

		BALL_CREATE(AtomIteratorTraits_)

		AtomIteratorTraits_()
			:	CompositeIteratorTraits_()
		{
			predicate_ = &RTTI::getDefault<KernelPredicate<Atom> >();
		}
			
		AtomIteratorTraits_(const Composite& composite)
			:	CompositeIteratorTraits_(composite)
		{
			predicate_ = &RTTI::getDefault<KernelPredicate<Atom> >();
		}
			
		AtomIteratorTraits_(const AtomIteratorTraits_& traits, bool /* deep */ = true)
			:	CompositeIteratorTraits_(traits)
		{
		}
			
		AtomIteratorTraits_& operator = (const AtomIteratorTraits_& traits)
		{
			CompositeIteratorTraits_::operator=(traits);
			return *this;
		}

		void resetPredicate()
		{
			predicate_ = &RTTI::getDefault<KernelPredicate<Atom> >();
		}

	};



	typedef BidirectionalIterator
		<Composite, Atom, Composite::SubcompositeIterator, AtomIteratorTraits_>
		AtomIterator;

	typedef ConstBidirectionalIterator
		<Composite, Atom, Composite::SubcompositeIterator, AtomIteratorTraits_>
		AtomConstIterator;

	typedef ReverseBidirectionalIterator
		<Composite, Atom, Composite::SubcompositeIterator, AtomIteratorTraits_>
		AtomReverseIterator;

	typedef ConstReverseBidirectionalIterator
		<Composite, Atom, Composite::SubcompositeIterator, AtomIteratorTraits_>
		AtomConstReverseIterator;

} // namespace BALL

#endif // BALL_KERNEL_ATOMITERATOR_H
