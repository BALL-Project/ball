// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyBALLSipHelper.h,v 1.1.2.1 2003/01/07 13:18:51 anker Exp $

#ifndef BALL_PYTHON_PYBALLSIPHELPER_H
#define BALL_PYTHON_PYBALLSIPHELPER_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

namespace BALL 
{
	/**	Virtual mapping of BALL objects to their true class.
	*/
	PyObject* pyMapBALLObjectToSip(Composite& object);
} // namespace BALL

#endif // BALL_PYTHON_PYBALLSIPHELPER_H
