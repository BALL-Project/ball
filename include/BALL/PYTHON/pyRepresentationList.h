// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyRepresentationList.h,v 1.3 2004/05/19 13:48:22 amoll Exp $
//

#ifndef BALL_PYTHON_PYREPRESENTATIONLIST_H
#define BALL_PYTHON_PYREPRESENTATIONLIST_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_DATAYPE_LIST_H
#	include <BALL/DATATYPE/list.h>
#endif

#ifndef BALL_VIEW_KERNEL_PRIMITIVEMANAGER_H
#	include <BALL/VIEW/KERNEL/primitiveManager.h>
#endif

#ifndef BALL_VIEW_KERNEL_REPRESENTATION_H
#	include <BALL/VIEW/KERNEL/representation.h>
#endif

namespace BALL 
{
	/** Equivalent for a STL::List of Representation Pointers in Python
			\ingroup PythonExtensions
	*/
	class PyRepresentationList
			:	public VIEW::PrimitiveManager::RepresentationList
	{
		public:

		BALL_CREATE(PyRepresentationList)

		/**	@name	Type Definitions
		*/
		//@{

		/**	Representation* type
		*/
		typedef VIEW::Representation* ValueType;

		/**	Pointer type
		*/
		typedef VIEW::Representation** PointerType;

		/**	Iterator type.
		*/
		typedef List<VIEW::Representation*>::iterator Iterator;

		/**	Constant iterator type.
		*/
		typedef List<VIEW::Representation*>::const_iterator ConstIterator;

		//@}

		/**	@name	Constructors and Destructors */
		//@{

		///	Default constructor.
		PyRepresentationList();

		/// Copy constructor
		PyRepresentationList(const PyRepresentationList& py_repr_list);
			
		/** Copy constructor.
				Create a copy of an existing list.
				@param	map the list to be copied
				@param	deep ignored
		*/
		PyRepresentationList(const List<VIEW::Representation*>& repr_list);
			
		/**	Destructor
		*/
		virtual ~PyRepresentationList() throw();
		//@}

	};
   
} // namespace BALL

#endif // BALL_PYTHON_PYREPRESENTATIONLIST_H
