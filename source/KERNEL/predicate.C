// $Id: predicate.C,v 1.2 1999/12/30 18:05:33 oliver Exp $

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
