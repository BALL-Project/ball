// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyAtomList.h,v 1.11 2003/08/26 08:04:46 oliver Exp $
//

#ifndef BALL_PYTHON_PYATOMLIST_H
#define BALL_PYTHON_PYATOMLIST_H

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
  /**  \addtogroup  PythonExtensions
   *  @{
   */
	class Atom;
	class AtomContainer;
	
	/**
	*/
	class PyAtomList
		:	public List<Atom*>
	{
		public:

		BALL_CREATE(PyAtomList)

		/**	@name	Type Definitions
		*/
		//@{

		/**	Atom* type
		*/
		typedef Atom* ValueType;

		/**	Pointer type
		*/
		typedef Atom** PointerType;

		/**	Iterator type.
		*/
		typedef List<Atom*>::iterator Iterator;

		/**	Constant iterator type.
		*/
		typedef List<Atom*>::const_iterator ConstIterator;

		//@}

		/**	@name	Constructors and Destructors */
		//@{

		/**	Default constructor.
				Create an empty list.
		*/
		PyAtomList();

		/** Copy constructor.
				Create a copy of an existing list.
				@param	map the list to be copied
				@param	deep ignored
		*/
		PyAtomList(const PyAtomList& new_list);
			
		/**	Construct from a AtomContainer.
				This constructor creates an PyAtomList object from
				all atoms of a  \link AtomContainer AtomContainer \endlink  object.
		*/
		PyAtomList(const AtomContainer& fragment);

		/**	Construct from a AtomContainer with expression.
				This constructor creates an PyAtomList object from
				the atoms of a  \link AtomContainer AtomContainer \endlink  object that match <tt>expression</tt>.
		*/
		PyAtomList(const AtomContainer& fragment, const String& expression);

		/**	Destructor
		*/
		virtual ~PyAtomList() throw();
		//@}

		/**	@name Assignment
		*/
		//@{
		/**
		*/
		void set(const AtomContainer& fragment, const String& expression = "");
		//@}
	};
   
} // namespace BALL

#endif // BALL_PYTHON_PYATOMLIST_H
