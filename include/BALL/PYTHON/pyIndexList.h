// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyIndexList.h,v 1.1.2.1 2005/11/12 11:17:57 oliver Exp $
//

#ifndef BALL_PYTHON_PYINDEXLIST_H
#define BALL_PYTHON_PYINDEXLIST_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_DATAYPE_LIST_H
#	include <BALL/DATATYPE/list.h>
#endif

#ifndef BALL_DATAYPE_STRING_H
#	include <BALL/DATATYPE/string.h>
#endif

namespace BALL 
{
	/** Equivalent for an STL::List of numbers/indices in Python.
			This is used to express both, lits and arrays of signed/unsigned 
			integers in Python.
			\ingroup PythonExtensions		
	*/
	class PyIndexList
		:	public List<Index>
	{
		public:

		BALL_CREATE(PyIndexList)

		/**	@name	Type Definitions
		*/
		//@{

		/**	Index type
		*/
		typedef Index ValueType;

		/**	Pointer type
		*/
		typedef Index PointerType;

		/**	Iterator type.
		*/
		typedef List<Index>::iterator Iterator;

		/**	Constant iterator type.
		*/
		typedef List<Index>::const_iterator ConstIterator;

		//@}

		/**	@name	Constructors and Destructors */
		//@{

		/**	Default constructor.
				Create an empty list.
		*/
		PyIndexList();

		/// Copy constructor
		PyIndexList(const PyIndexList& new_list);
			
		/**	Construct from a vector of Index.
				This constructor creates an PyIndexList object from
				a vector of Index.
		*/
		PyIndexList(const std::vector<Index>& indices);

		/**	Construct from a list of Index.
				This constructor creates an PyIndexList object from
				a list of Index objects.
		*/
		PyIndexList(const std::list<Index>& fragment);

		/**	Construct from a vector of Index.
				This constructor creates an PyIndexList object from
				a vector of Index.
		*/
		PyIndexList(const std::vector<Position>& indices);

		/**	Construct from a list of Index.
				This constructor creates an PyIndexList object from
				a list of Index objects.
		*/
		PyIndexList(const std::list<Position>& fragment);

		/**	Destructor
		*/
		virtual ~PyIndexList() throw();
		//@}

		/**	@name Assignment
		*/
		//@{

		///
		PyIndexList& operator = (const std::list<Index>& idx_list);
		///
		PyIndexList& operator = (const std::list<Position>& idx_list);
		///
		PyIndexList& operator = (const std::vector<Index>& idx_list);
		///
		PyIndexList& operator = (const std::vector<Position>& idx_list);
		//@}
	};
   
} // namespace BALL

#endif // BALL_PYTHON_PYINDEXLIST_H
