// $Id: pyAtomContainerList.h,v 1.1 2000/08/30 19:58:22 oliver Exp $

#ifndef BALL_PYTHON_PYATOMCONTAINERLIST_H
#define BALL_PYTHON_PYATOMCONTAINERLIST_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_DATAYPE_LIST_H
#	include <BALL/DATATYPE/list.h>
#endif

namespace BALL 
{
	class AtomContainer;
	
	/**
	*/
	class PyAtomContainerList
		:	public List<AtomContainer*>
	{
		public:

		BALL_CREATE(PyAtomContainerList)

		/**	@name	Type Definitions
		*/
		//@{

		/**	AtomContainer* type
		*/
		typedef AtomContainer* ValueType;

		/**	Pointer type
		*/
		typedef AtomContainer** PointerType;

		/**	Iterator type.
		*/
		typedef List<AtomContainer*>::iterator Iterator;

		/**	Constant iterator type.
		*/
		typedef List<AtomContainer*>::const_iterator ConstIterator;

		//@}

		/**	@name	Constructors and Destructors */
		//@{

		/**	Default constructor.
				Create an empty list.
		*/
		PyAtomContainerList();

		/** Copy constructor.
				Create a copy of an existing list.
				@param	map the list to be copied
				@param	deep ignored
		*/
		PyAtomContainerList(const PyAtomContainerList& new_list);
			
		/**	Construct from a AtomContainer
				This constructor creates an PyAtomContainerList object from
				all base fragments of a \Ref{AtomContainer} object.
		*/
		PyAtomContainerList(const AtomContainer& fragment, bool selected_only = false);

		/**	Destructor
		*/
		virtual ~PyAtomContainerList();
		//@}

		/**	@name Assignment
		*/
		//@{
		/**
		*/
		void set(const AtomContainer& fragment, bool selected_only = false);
		//@}
	};

} // namespace BALL

#endif // BALL_PYTHON_PYATOMCONTAINERLIST_H
