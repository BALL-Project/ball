// $Id: pyPDBAtomList.h,v 1.5 2001/07/25 11:30:49 oliver Exp $

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
	class AtomContainer;
	
	/**
	*/
	class PyPDBAtomList
		:	public List<PDBAtom*>
	{
		public:

		BALL_CREATE(PyPDBAtomList)

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
			
		/**	Construct from a AtomContainer.
				This constructor creates an PyPDBAtomList object from
				all atoms of a \Ref{AtomContainer} object.
		*/
		PyPDBAtomList(const AtomContainer& fragment);

		/**	Construct from a AtomContainer with expression.
				This constructor creates an PyPDBAtomList object from
				the atoms of a \Ref{AtomContainer} object that match {\tt expression}.
		*/
		PyPDBAtomList(const AtomContainer& fragment, const String& expression);

		/**	Destructor
		*/
		virtual ~PyPDBAtomList() throw();
		//@}

		/**	@name Assignment
		*/
		//@{
		/**
		*/
		void set(const AtomContainer& fragment, const String& expression = "" );
		//@}
	};

} // namespace BALL

#endif // BALL_PYTHON_PYPDBATOMLIST_H
