// $Id: pyBALLSipHelper.h,v 1.1 2000/07/04 08:05:33 oliver Exp $

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
