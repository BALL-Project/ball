// $Id: predicate.C,v 1.1 1999/08/26 08:02:34 oliver Exp $

#include <BALL/KERNEL/predicate.h>

#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/chain.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/protein.h>

namespace BALL {

	template <class T>
	bool KernelPredicate<T>::operator ()(const Composite& composite)
	{
		return RTTI<T>::isKindOf(composite);
	}

	template <class T>
	bool KernelPredicate::operator ()(const Composite& composite) const
	{
		return RTTI<T>::isKindOf(composite);
	}

} // namespace BALL
