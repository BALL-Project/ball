// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: predicate.C,v 1.3 2002/02/27 12:21:24 sturm Exp $

#include <BALL/KERNEL/predicate.h>

#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/chain.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/protein.h>

namespace BALL 
{

	template <typename T>
	bool KernelPredicate<T>::operator () (const Composite& composite)
	{
		return RTTI::isKindOf<T>(composite);
	}

	template <class T>
	bool KernelPredicate::operator () (const Composite& composite) const
	{
		return RTTI::isKindOf<T>(composite);
	}

} // namespace BALL
