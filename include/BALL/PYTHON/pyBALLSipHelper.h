// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyBALLSipHelper.h,v 1.5 2003/03/26 13:08:21 sturm Exp $

#ifndef BALL_PYTHON_PYBALLSIPHELPER_H
#define BALL_PYTHON_PYBALLSIPHELPER_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

namespace BALL 
{
  /**  \addtogroup  PythonExtensions
   *  @{
   */
	/**	Virtual mapping of BALL objects to their true class.
	*/
	PyObject* pyMapBALLObjectToSip(Composite& object);
   
} // namespace BALL

#endif // BALL_PYTHON_PYBALLSIPHELPER_H
