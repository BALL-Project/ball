// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyBondList.h,v 1.6 2003/02/25 15:53:48 sturm Exp $

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
  /** \ingroup PythonExtensions
   *  @{
   */
	class Bond;
	class Atom;
	class AtomContainer;
	
	/**
	*/
	class PyBondList
		:	public List<Bond*>
	{
		public:

		BALL_CREATE(PyBondList)

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
			
		/**	Construct from a AtomContainer.
				This constructor creates an PyBondList object from
				all bonds of the atoms of a  \link AtomContainer AtomContainer \endlink  object.
				If <tt>selected_only</tt> is set to <b>true</b>, only those
				atoms are inserted into the list where both atoms are selected.
				It is not required that both atoms of a bond are contained
				in <tt>fragment</tt>.
				@param fragment the AtomContainer containing the atoms
				@param selected_only if set to <b>true</b>, only those bonds are inserted
					where at least one atom is selected and contained in <tt>fragment</tt>
		*/
		PyBondList(const AtomContainer& fragment, bool selected_only = false);

		/**	Construct a list of all bonds of an atom.
				This constructor creates a PyBondList object from
				all bonds an atom.
				@param the atom
		*/
		PyBondList(const Atom& atom);

		/**	Destructor
		*/
		virtual ~PyBondList() throw();
		//@}

		/**	@name Assignment
		*/
		//@{
		/**
		*/
		void set(const AtomContainer& fragment, bool selected_only = false);
			
		/**
		*/
		void set(const Atom& atom);
		//@}
	};
   /** @} */
} // namespace BALL

#endif // BALL_PYTHON_PYBONDLIST_H
