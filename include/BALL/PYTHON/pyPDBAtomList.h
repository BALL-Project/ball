// $Id: pyPDBAtomList.h,v 1.3 2000/07/10 14:09:00 oliver Exp $

#ifndef BALL_PYTHON_PYPDBATOMLIST_H
#define BALL_PYTHON_PYPDBATOMLIST_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_DATAYPE_LIST_H
#	include <BALL/DATATYPE/list.h>
#endif

#ifndef BALL_DATAYPE_STRING_H
# include <BALL/DATATYPE/string.h>
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
			
		/**	Construct from a BaseFragment.
				This constructor creates an PyPDBAtomList object from
				all atoms of a \Ref{BaseFragment} object.
		*/
		PyPDBAtomList(const BaseFragment& fragment);

		/**	Construct from a BaseFragment with expression.
				This constructor creates an PyPDBAtomList object from
				the atoms of a \Ref{BaseFragment} object that match {\tt expression}.
		*/
		PyPDBAtomList(const BaseFragment& fragment, const String& expression);

		/**	Destructor
		*/
		virtual ~PyPDBAtomList();
		//@}

		/**	@name Assignment
		*/
		//@{
		/**
		*/
		void set(const BaseFragment& fragment, const String& expression = "" );
		//@}
	};

} // namespace BALL

#endif // BALL_PYTHON_PYPDBATOMLIST_H
