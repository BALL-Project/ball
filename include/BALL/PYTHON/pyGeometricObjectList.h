// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyGeometricObjectList.h,v 1.1 2003/12/11 02:14:35 amoll Exp $
//

#ifndef BALL_PYTHON_PYGEOMETRICOBJECTLIST_H
#define BALL_PYTHON_PYGEOMETRICOBJECTLIST_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_DATAYPE_LIST_H
#	include <BALL/DATATYPE/list.h>
#endif

namespace BALL 
{
	namespace VIEW
	{
		class GeometricObject;
	}
	
	/** pyGeometricObjectList
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
