// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyCompositeList.h,v 1.1 2003/11/15 12:53:56 oliver Exp $
//

#ifndef BALL_PYTHON_PYCOMPOSITELIST_H
#define BALL_PYTHON_PYCOMPOSITELIST_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_DATAYPE_LIST_H
#	include <BALL/DATATYPE/list.h>
#endif

#ifndef BALL_DATAYPE_HASHSET_H
#	include <BALL/DATATYPE/hashSet.h>
#endif

namespace BALL 
{
	class Composite;
	
	/** pyCompositeList
		\ingroup PythonExtensions
	*/
	class PyCompositeList
		:	public List<Composite*>
	{
		public:

		BALL_CREATE(PyCompositeList)

		/**	@name	Type Definitions
		*/
		//@{

		/**	Composite* type
		*/
		typedef Composite* ValueType;

		/**	Pointer type
		*/
		typedef Composite** PointerType;

		/**	Iterator type.
		*/
		typedef List<Composite*>::iterator Iterator;

		/**	Constant iterator type.
		*/
		typedef List<Composite*>::const_iterator ConstIterator;

		//@}

		/**	@name	Constructors and Destructors */
		//@{

		/**	Default constructor.
				Create an empty list.
		*/
		PyCompositeList();

		/// Copy constructor.
		PyCompositeList(const PyCompositeList& composite_list);

		/// Copy constructor.
		PyCompositeList(const List<Composite*>& composite_list);
			
		/// Construction from a HashSet
		PyCompositeList(const HashSet<Composite*>& composite_set);
			
		///	Destructor
		virtual ~PyCompositeList() throw();
		//@}

	};
   
} // namespace BALL

#endif // BALL_PYTHON_PYCOMPOSITELIST_H
