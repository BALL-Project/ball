// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: moleculeIterator.h,v 1.15.4.1 2005/07/28 14:01:56 amoll Exp $
//

#ifndef BALL_KERNEL_MOLECULEITERATOR_H
#define BALL_KERNEL_MOLECULEITERATOR_H

#ifndef BALL_KERNEL_ITERATOR_H
#	include <BALL/KERNEL/iterator.h>
#endif

#ifndef BALL_KERNEL_PREDICATE_H
# include <BALL/KERNEL/predicate.h>
#endif

namespace BALL 
{
	/** MoleculeIteratorTratis_
	 		\ingroup KernelIterators
	*/
	class BALL_EXPORT MoleculeIteratorTraits
		: public CompositeIteratorTraits
	{
		public:
		
		MoleculeIteratorTraits()
			:	CompositeIteratorTraits()
		{
			predicate_ = &RTTI::getDefault<KernelPredicate<Molecule> >();
		}
			
		MoleculeIteratorTraits(const Composite& composite)
			:	CompositeIteratorTraits(composite)
		{
			predicate_ = &RTTI::getDefault<KernelPredicate<Molecule> >();
		}
			
		MoleculeIteratorTraits(const MoleculeIteratorTraits& traits, bool /* deep */ = true)
			:	CompositeIteratorTraits(traits)
		{
		}
			
		MoleculeIteratorTraits &operator = (const MoleculeIteratorTraits& traits)
		{
			CompositeIteratorTraits::operator=(traits);
			return *this;
		}

		void resetPredicate()
		{
			predicate_ = &RTTI::getDefault<KernelPredicate<Molecule> >();
		}
	};


	///
	typedef BidirectionalIterator
		<Composite, Molecule, Composite::CompositeIterator, MoleculeIteratorTraits>
		MoleculeIterator;

	///
	typedef ConstBidirectionalIterator
		<Composite, Molecule, Composite::CompositeIterator, MoleculeIteratorTraits>
		MoleculeConstIterator;

	///
	typedef std::reverse_iterator<MoleculeIterator> MoleculeReverseIterator;
	
	///
	typedef std::reverse_iterator<MoleculeConstIterator> MoleculeConstReverseIterator;

} // namespace BALL

#endif // BALL_KERNEL_MOLECULEITERATOR_H
