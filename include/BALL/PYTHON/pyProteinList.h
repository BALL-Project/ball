// $Id: pyProteinList.h,v 1.3 2001/07/25 11:30:49 oliver Exp $

#ifndef BALL_PYTHON_PYPROTEINLIST_H
#define BALL_PYTHON_PYPROTEINLIST_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_DATAYPE_LIST_H
#	include <BALL/DATATYPE/list.h>
#endif

namespace BALL 
{
	class Protein;
	class AtomContainer;
	
	/**
	*/
	class PyProteinList
		:	public List<Protein*>
	{
		public:

		BALL_CREATE(PyProteinList)

		/**	@name	Type Definitions
		*/
		//@{

		/**	Protein* type
		*/
		typedef Protein* ValueType;

		/**	Pointer type
		*/
		typedef Protein** PointerType;

		/**	Iterator type.
		*/
		typedef List<Protein*>::iterator Iterator;

		/**	Constant iterator type.
		*/
		typedef List<Protein*>::const_iterator ConstIterator;

		//@}

		/**	@name	Constructors and Destructors */
		//@{

		/**	Default constructor.
				Create an empty list.
		*/
		PyProteinList();

		/** Copy constructor.
				Create a copy of an existing list.
				@param	map the list to be copied
				@param	deep ignored
		*/
		PyProteinList(const PyProteinList& new_list);
			
		/**	Construct from a AtomContainer
				This constructor creates an PyProteinList object from
				all atoms of a \Ref{AtomContainer} object.
		*/
		PyProteinList(const AtomContainer& fragment, bool selected_only = false);

		/**	Destructor
		*/
		virtual ~PyProteinList() throw();
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

#endif // BALL_PYTHON_PYPROTEINLIST_H
