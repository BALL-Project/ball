// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyBALLSipHelper.h,v 1.6.2.1 2004/05/19 13:48:09 amoll Exp $
//

#ifndef BALL_PYTHON_PYBALLSIPHELPER_H
#define BALL_PYTHON_PYBALLSIPHELPER_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

namespace BALL 
{
	/**	Virtual mapping of BALL objects to their true class.
    	\ingroup  PythonExtensions
	*/
	PyObject* pyMapBALLObjectToSip(Composite& object);
   
} // namespace BALL

#endif // BALL_PYTHON_PYBALLSIPHELPER_H
