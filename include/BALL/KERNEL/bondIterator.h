// $Id: bondIterator.h,v 1.1 1999/08/26 07:53:19 oliver Exp $

#ifndef BALL_KERNEL_BONDITERATOR_H
#define BALL_KERNEL_BONDITERATOR_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
#	include <BALL/KERNEL/atom.h>
#endif

namespace BALL {

	typedef Atom::BondIterator 							AtomBondIterator;
	typedef Atom::BondConstIterator 				AtomBondConstIterator;
	typedef Atom::BondReverseIterator 			AtomBondReverseIterator;
	typedef Atom::BondConstReverseIterator 	AtomBondConstReverseIterator;

}

#endif // BALL_KERNEL_BONDITERATOR_H
