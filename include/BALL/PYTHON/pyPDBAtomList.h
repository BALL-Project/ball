// $Id: pyPDBAtomList.h,v 1.1 2000/06/27 07:44:11 oliver Exp $

#ifndef BALL_PYTHON_PYPDBATOMLIST_H
#define BALL_PYTHON_PYPDBATOMLIST_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_DATAYPE_LIST_H
#	include <BALL/DATATYPE/list.h>
#endif

namespace BALL 
{
	class PDBAtom;
	class BaseFragment;
	
	/**
	*/
	class PyPDBAtomList
		:	public List<PDBAtom*>
	{
		public:

		BALL_CREATE_NODEEP(PyPDBAtomList)

		/**	@name	Type Definitions
		*/
		//@{

		/**	PDBAtom* type
		*/
		typedef PDBAtom* ValueType;

		/**	Pointer type
		*/
		typedef PDBAtom** PointerType;

		/**	Iterator type.
		*/
		typedef List<PDBAtom*>::iterator Iterator;

		/**	Constant iterator type.
		*/
		typedef List<PDBAtom*>::const_iterator ConstIterator;

		//@}

		/**	@name	Constructors and Destructors */
		//@{

		/**	Default constructor.
				Create an empty list.
		*/
		PyPDBAtomList();

		/** Copy constructor.
				Create a copy of an existing list.
				@param	map the list to be copied
				@param	deep ignored
		*/
		PyPDBAtomList(const PyPDBAtomList& new_list);
			
		/**	Construct from a BaseFragment
				This constructor creates an PyPDBAtomList object from
				all atoms of a \Ref{BaseFragment} object.
		*/
		PyPDBAtomList(const BaseFragment& fragment, bool selected_only = false);

		/**	Destructor
		*/
		virtual ~PyPDBAtomList();
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

#endif // BALL_PYTHON_PYPDBATOMLIST_H
