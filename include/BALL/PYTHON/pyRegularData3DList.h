// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyRegularData3DList.h,v 1.1.2.1 2006/10/04 11:33:23 amoll Exp $
//

#ifndef BALL_PYTHON_PYREGULARDATA3DLIST_H
#define BALL_PYTHON_PYREGULARDATA3DLIST_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_DATAYPE_LIST_H
#	include <BALL/DATATYPE/list.h>
#endif

#ifndef BALL_DATATYPE_REGULARDATA3D_H
#	include <BALL/DATATYPE/regularData3D.h>
#endif

namespace BALL 
{
	
	/** Equivalent for a STL::List of RegularData3D Pointers in Python
			\ingroup PythonExtensions
	*/
	class PyRegularData3DList
		:	public List<RegularData3D*>
	{
		public:

		BALL_CREATE(PyRegularData3DList)

		/**	RegularData3D* type
		*/
		typedef RegularData3D* ValueType;

		/**	Pointer type
		*/
		typedef RegularData3D** PointerType;

		/**	Iterator type.
		*/
		typedef List<RegularData3D*>::iterator Iterator;

		/**	Constant iterator type.
		*/
		typedef List<RegularData3D*>::const_iterator ConstIterator;

		/**	Default constructor.
				Create an empty list.
		*/
		PyRegularData3DList();

		/// Copy constructor.
		PyRegularData3DList(const PyRegularData3DList& l);

		/// Copy constructor.
		PyRegularData3DList(const List<std::pair<RegularData3D*, String> > l);
		
		///	Destructor
		virtual ~PyRegularData3DList() throw();
	};
   
} // namespace BALL

#endif // BALL_PYTHON_PYREGULARDATA3DLIST_H
