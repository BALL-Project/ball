// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: bondIterator.h,v 1.3.2.1 2003/01/07 13:17:51 anker Exp $

#ifndef BALL_KERNEL_BONDITERATOR_H
#define BALL_KERNEL_BONDITERATOR_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
#	include <BALL/KERNEL/atom.h>
#endif

namespace BALL 
{

	/** @name Typedefs for the various BondIterators
	*/
	//@{

	/** A mutable iterator for atom bonds.
	*/
	typedef Atom::BondIterator 							AtomBondIterator;

	/** A constant iterator for atom bonds.
	*/
	typedef Atom::BondConstIterator 				AtomBondConstIterator;

	/** A mutable reverse iterator for atom bonds.
	*/
	typedef Atom::BondReverseIterator 			AtomBondReverseIterator;

	/** A constant reverse iterator for atom bonds.
	*/
	typedef Atom::BondConstReverseIterator 	AtomBondConstReverseIterator;

	//@}

}

#endif // BALL_KERNEL_BONDITERATOR_H
