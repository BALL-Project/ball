// $Id: pyAtomList.h,v 1.1 2000/06/27 07:44:09 oliver Exp $

#ifndef BALL_PYTHON_PYATOMLIST_H
#define BALL_PYTHON_PYATOMLIST_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_DATAYPE_LIST_H
#	include <BALL/DATATYPE/list.h>
#endif

namespace BALL 
{
	class Atom;
	class BaseFragment;
	
	/**
	*/
	class PyAtomList
		:	public List<Atom*>
	{
		public:

		BALL_CREATE_NODEEP(PyAtomList)

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
			
		/**	Construct from a BaseFragment
				This constructor creates an PyAtomList object from
				all atoms of a \Ref{BaseFragment} object.
		*/
		PyAtomList(const BaseFragment& fragment, bool selected_only = false);

		/**	Destructor
		*/
		virtual ~PyAtomList();
		//@}

		/**	@name Assignment
		*/
		//@{
		/**
		*/
		void set(const BaseFragment& fragment, bool selected_only = false);
		//@}
	};

} // namespace BALL

#endif // BALL_PYTHON_PYATOMLIST_H
