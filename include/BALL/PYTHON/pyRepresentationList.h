// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyRepresentationList.h,v 1.1 2003/11/15 12:53:56 oliver Exp $
//

#ifndef BALL_PYTHON_PYREPRESENTATIONLIST_H
#define BALL_PYTHON_PYREPRESENTATIONLIST_H

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
		class Representation;
	}

	/** pyRepresentationList
		\ingroup PythonExtensions
	*/
	class PyRepresentationList
		:	public List<VIEW::Representation*>
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
