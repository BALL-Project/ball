// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyGeometricObjectList.h,v 1.4 2004/05/27 19:49:45 oliver Exp $
//

#ifndef BALL_PYTHON_PYGEOMETRICOBJECTLIST_H
#define BALL_PYTHON_PYGEOMETRICOBJECTLIST_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_DATAYPE_LIST_H
#	include <BALL/DATATYPE/list.h>
#endif

#ifndef BALL_VIEW_KERNEL_GEOMETRIC_OBJECT
# include <BALL/VIEW/KERNEL/geometricObject.h>
#endif

namespace BALL 
{
	using namespace VIEW;
	
	/** Equivalent for a STL::List of GeometricObject Pointers in Python
			\ingroup PythonExtensions
	*/
	class PyGeometricObjectList
		:	public List<GeometricObject*>
	{
		public:

		BALL_CREATE(PyGeometricObjectList)

		/**	@name	Type Definitions
		*/
		//@{

		/**	GeometricObject* type
		*/
		typedef GeometricObject* ValueType;

		/**	Pointer type
		*/
		typedef GeometricObject** PointerType;

		/**	Iterator type.
		*/
		typedef List<GeometricObject*>::iterator Iterator;

		/**	Constant iterator type.
		*/
		typedef List<GeometricObject*>::const_iterator ConstIterator;

		//@}
		/**	@name	Constructors and Destructors */
		//@{

		/**	Default constructor.
		*/
		PyGeometricObjectList();

		/// Copy constructor.
		PyGeometricObjectList(const PyGeometricObjectList& go_list);

		/// Copy constructor.
		PyGeometricObjectList(const List<GeometricObject*>& go_list);
			
		///	Destructor
		virtual ~PyGeometricObjectList() throw();
		//@}

	};
   
} // namespace BALL

#endif // BALL_PYTHON_PYGEOMETRICOBJECTLIST_H
