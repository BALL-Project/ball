// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyFragmentList.h,v 1.7 2003/03/03 14:18:13 anhi Exp $

#ifndef BALL_PYTHON_PYFRAGMENTLIST_H
#define BALL_PYTHON_PYFRAGMENTLIST_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_DATAYPE_LIST_H
#	include <BALL/DATATYPE/list.h>
#endif

namespace BALL 
{
  /**  \addtogroup  PythonExtensions
   *  @{
   */
	class Fragment;
	class AtomContainer;
	
	/**
	*/
	class PyFragmentList
		:	public List<Fragment*>
	{
		public:

		BALL_CREATE(PyFragmentList)

		/**	@name	Type Definitions
		*/
		//@{

		/**	Fragment* type
		*/
		typedef Fragment* ValueType;

		/**	Pointer type
		*/
		typedef Fragment** PointerType;

		/**	Iterator type.
		*/
		typedef List<Fragment*>::iterator Iterator;

		/**	Constant iterator type.
		*/
		typedef List<Fragment*>::const_iterator ConstIterator;

		//@}

		/**	@name	Constructors and Destructors */
		//@{

		/**	Default constructor.
				Create an empty list.
		*/
		PyFragmentList();

		/** Copy constructor.
				Create a copy of an existing list.
				@param	map the list to be copied
				@param	deep ignored
		*/
		PyFragmentList(const PyFragmentList& new_list);
			
		/**	Construct from a AtomContainer
				This constructor creates an PyFragmentList object from
				all atoms of a  \link AtomContainer AtomContainer \endlink  object.
		*/
		PyFragmentList(const AtomContainer& fragment, bool selected_only = false);

		/**	Destructor
		*/
		virtual ~PyFragmentList() throw();
		//@}

		/**	@name Assignment
		*/
		//@{
		/**
		*/
		void set(const AtomContainer& fragment, bool selected_only = false);
		//@}
	};
   /** @} */
} // namespace BALL

#endif // BALL_PYTHON_PYFRAGMENTLIST_H
