// $Id: pyMoleculeList.h,v 1.1 2000/06/27 07:44:10 oliver Exp $

#ifndef BALL_PYTHON_PYMOLECULELIST_H
#define BALL_PYTHON_PYMOLECULELIST_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_DATAYPE_LIST_H
#	include <BALL/DATATYPE/list.h>
#endif

namespace BALL 
{
	class Molecule;
	class BaseMolecule;
	
	/**
	*/
	class PyMoleculeList
		:	public List<Molecule*>
	{
		public:

		BALL_CREATE_NODEEP(PyMoleculeList)

		/**	@name	Type Definitions
		*/
		//@{

		/**	Molecule* type
		*/
		typedef Molecule* ValueType;

		/**	Pointer type
		*/
		typedef Molecule** PointerType;

		/**	Iterator type.
		*/
		typedef List<Molecule*>::iterator Iterator;

		/**	Constant iterator type.
		*/
		typedef List<Molecule*>::const_iterator ConstIterator;

		//@}

		/**	@name	Constructors and Destructors */
		//@{

		/**	Default constructor.
				Create an empty list.
		*/
		PyMoleculeList();

		/** Copy constructor.
				Create a copy of an existing list.
				@param	map the list to be copied
				@param	deep ignored
		*/
		PyMoleculeList(const PyMoleculeList& new_list);
			
		/**	Construct from a BaseMolecule
				This constructor creates an PyMoleculeList object from
				all atoms of a \Ref{BaseMolecule} object.
		*/
		PyMoleculeList(const BaseMolecule& fragment, bool selected_only = false);

		/**	Destructor
		*/
		virtual ~PyMoleculeList();
		//@}

		/**	@name Assignment
		*/
		//@{
		/**
		*/
		void set(const BaseMolecule& fragment, bool selected_only = false);
		//@}
	};

} // namespace BALL

#endif // BALL_PYTHON_PYMOLECULELIST_H
