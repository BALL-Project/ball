// $Id: pyBondList.h,v 1.1 2000/07/04 08:05:34 oliver Exp $

#ifndef BALL_PYTHON_PYBONDLIST_H
#define BALL_PYTHON_PYBONDLIST_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_DATAYPE_LIST_H
#	include <BALL/DATATYPE/list.h>
#endif

namespace BALL 
{
	class Bond;
	class Atom;
	class BaseFragment;
	
	/**
	*/
	class PyBondList
		:	public List<Bond*>
	{
		public:

		BALL_CREATE_NODEEP(PyBondList)

		/**	@name	Type Definitions
		*/
		//@{

		/**	Bond* type
		*/
		typedef Bond* ValueType;

		/**	Pointer type
		*/
		typedef Bond** PointerType;

		/**	Iterator type.
		*/
		typedef List<Bond*>::iterator Iterator;

		/**	Constant iterator type.
		*/
		typedef List<Bond*>::const_iterator ConstIterator;

		//@}

		/**	@name	Constructors and Destructors */
		//@{

		/**	Default constructor.
				Create an empty list.
		*/
		PyBondList();

		/** Copy constructor.
				Create a copy of an existing list.
				@param	map the list to be copied
				@param	deep ignored
		*/
		PyBondList(const PyBondList& new_list);
			
		/**	Construct from a BaseFragment.
				This constructor creates an PyBondList object from
				all bonds of the atoms of a \Ref{BaseFragment} object.
				If {\tt selected_only} is set to {\bf true}, only those
				atoms are inserted into the list where both atoms are selected.
				It is not required that both atoms of a bond are contained
				in {\tt fragment}.
				@param fragment the BaseFragment containing the atoms
				@param selected_only if set to {\bf true}, only those bonds are inserted
					where at least one atom is selected and contained in {\tt fragment}
		*/
		PyBondList(const BaseFragment& fragment, bool selected_only = false);

		/**	Construct a list of all bonds of an atom.
				This constructor creates a PyBondList object from
				all bonds an atom.
				@param the atom
		*/
		PyBondList(const Atom& atom);

		/**	Destructor
		*/
		virtual ~PyBondList();
		//@}

		/**	@name Assignment
		*/
		//@{
		/**
		*/
		void set(const BaseFragment& fragment, bool selected_only = false);
			
		/**
		*/
		void set(const Atom& atom);
		//@}
	};

} // namespace BALL

#endif // BALL_PYTHON_PYBONDLIST_H
