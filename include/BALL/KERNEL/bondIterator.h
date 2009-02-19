// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: bondIterator.h,v 1.9 2003/08/26 08:04:17 oliver Exp $
//

#ifndef BALL_KERNEL_BONDITERATOR_H
#define BALL_KERNEL_BONDITERATOR_H

#ifndef BALL_KERNEL_ATOM_H
#	include <BALL/KERNEL/atom.h>
#endif

namespace BALL
{
	/** @name Typedefs for the various BondIterators
    	\ingroup  KernelIterators
	*/
	//@{

	/** A mutable iterator for atom bonds.
	*/
	typedef Atom::BondIterator             AtomBondIterator;

	/** A constant iterator for atom bonds.
	*/
	typedef Atom::BondConstIterator        AtomBondConstIterator;

	/** A mutable reverse iterator for atom bonds.
	*/
	typedef Atom::BondReverseIterator      AtomBondReverseIterator;

	/** A constant reverse iterator for atom bonds.
	*/
	typedef Atom::BondConstReverseIterator AtomBondConstReverseIterator;

	//@}
}

#endif // BALL_KERNEL_BONDITERATOR_H
