// $Id: moleculeIterator.h,v 1.2 1999/12/04 18:34:18 oliver Exp $

#ifndef BALL_KERNEL_MOLECULEITERATOR_H
#define BALL_KERNEL_MOLECULEITERATOR_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_MOLECULE_H
#	include <BALL/KERNEL/molecule.h>
#endif

#ifndef BALL_KERNEL_ITERATOR_H
#	include <BALL/KERNEL/iterator.h>
#endif

#ifndef BALL_KERNEL_PREDICATE_H
#	include <BALL/KERNEL/predicate.h>
#endif

namespace BALL {

	class MoleculeIteratorTraits_
		: public CompositeIteratorTraits_
	{
		public:
		
		BALL_CREATE(MoleculeIteratorTraits_)

		MoleculeIteratorTraits_()
			:	CompositeIteratorTraits_()
		{
			predicate_ = &RTTI<KernelPredicate<Molecule> >::getDefault();
		}
			
		MoleculeIteratorTraits_(const Composite& composite)
			:	CompositeIteratorTraits_(composite)
		{
			predicate_ = &RTTI<KernelPredicate<Molecule> >::getDefault();
		}
			
		MoleculeIteratorTraits_(const MoleculeIteratorTraits_& traits, bool /* deep */ = true)
			:	CompositeIteratorTraits_(traits)
		{
		}
			
		MoleculeIteratorTraits_ &operator = (const MoleculeIteratorTraits_& traits)
		{
			CompositeIteratorTraits_::operator=(traits);
			return *this;
		}

		void resetPredicate()
		{
			predicate_ = &RTTI<KernelPredicate<Molecule> >::getDefault();
		}
	};



	typedef BidirectionalIterator
		<Composite, Molecule, Composite::SubcompositeIterator, MoleculeIteratorTraits_>
		MoleculeIterator;

	typedef ConstBidirectionalIterator
		<Composite, Molecule, Composite::SubcompositeIterator, MoleculeIteratorTraits_>
		MoleculeConstIterator;

	typedef ReverseBidirectionalIterator
		<Composite, Molecule, Composite::SubcompositeIterator, MoleculeIteratorTraits_>
		MoleculeReverseIterator;

	typedef ConstReverseBidirectionalIterator
		<Composite, Molecule, Composite::SubcompositeIterator, MoleculeIteratorTraits_>
		MoleculeConstReverseIterator;

} // namespace BALL

#endif // BALL_KERNEL_MOLECULEITERATOR_H
