// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyBALLSipHelper.h,v 1.3 2003/02/25 15:53:47 sturm Exp $

#ifndef BALL_PYTHON_PYBALLSIPHELPER_H
#define BALL_PYTHON_PYBALLSIPHELPER_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

namespace BALL 
{
  /** \ingroup PythonExtensions
   *  @{
   */
	/**	Virtual mapping of BALL objects to their true class.
	*/
	PyObject* pyMapBALLObjectToSip(Composite& object);
   /** @} */
} // namespace BALL

#endif // BALL_PYTHON_PYBALLSIPHELPER_H
