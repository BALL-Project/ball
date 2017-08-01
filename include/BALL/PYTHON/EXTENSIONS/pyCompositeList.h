// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyCompositeList.h,v 1.6.20.1 2007/03/25 21:25:20 oliver Exp $
//

#ifndef BALL_PYTHON_PYCOMPOSITELIST_H
#define BALL_PYTHON_PYCOMPOSITELIST_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_DATAYPE_HASHSET_H
#	include <BALL/DATATYPE/hashSet.h>
#endif

#ifndef BALL_VIEW_KERNEL_REPRESENTATION_H
#	include <BALL/VIEW/KERNEL/representation.h>
#endif

namespace BALL 
{
	class Composite;
	
	/** Equivalent for a STL::List of Composite Pointers in Python
			\ingroup PythonExtensions
	*/
	class PyCompositeList
		:	public std::list<Composite*>
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
		typedef std::list<Composite*>::iterator Iterator;

		/**	Constant iterator type.
		*/
		typedef std::list<Composite*>::const_iterator ConstIterator;

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
		PyCompositeList(const std::list<Composite*>& composite_list);
		
		/// Copy constructor.
		PyCompositeList(const std::list<const Composite*>& composite_list);

		/// Constructor from a const Composite pointer list
		PyCompositeList& operator = (const std::list<const Composite*>& composite_list);
				
		/// Construction from a HashSet
		PyCompositeList(const HashSet<const Composite*>& composite_set);
			
		/// Construction from a HashSet
		PyCompositeList(const HashSet<Composite*>& composite_set);
			
		///	Destructor
		virtual ~PyCompositeList() throw();
		//@}
	};
   
} // namespace BALL

#endif // BALL_PYTHON_PYCOMPOSITELIST_H
