// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pySecondaryStructureList.h,v 1.7 2003/03/03 14:18:15 anhi Exp $

#ifndef BALL_PYTHON_PYSECONDARYSTRUCTURELIST_H
#define BALL_PYTHON_PYSECONDARYSTRUCTURELIST_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_DATAYPE_LIST_H
#	include <BALL/DATATYPE/list.h>
#endif

namespace BALL 
{
  /**  \addtogroup  PythonExtensions
   *  @{
   */
	class SecondaryStructure;
	class AtomContainer;
	
	/**
	*/
	class PySecondaryStructureList
		:	public List<SecondaryStructure*>
	{
		public:

		BALL_CREATE(PySecondaryStructureList)

		/**	@name	Type Definitions
		*/
		//@{

		/**	SecondaryStructure* type
		*/
		typedef SecondaryStructure* ValueType;

		/**	Pointer type
		*/
		typedef SecondaryStructure** PointerType;

		/**	Iterator type.
		*/
		typedef List<SecondaryStructure*>::iterator Iterator;

		/**	Constant iterator type.
		*/
		typedef List<SecondaryStructure*>::const_iterator ConstIterator;

		//@}

		/**	@name	Constructors and Destructors */
		//@{

		/**	Default constructor.
				Create an empty list.
		*/
		PySecondaryStructureList();

		/** Copy constructor.
				Create a copy of an existing list.
				@param	map the list to be copied
				@param	deep ignored
		*/
		PySecondaryStructureList(const PySecondaryStructureList& new_list);
			
		/**	Construct from a AtomContainer
				This constructor creates an PySecondaryStructureList object from
				all atoms of a  \link AtomContainer AtomContainer \endlink  object.
		*/
		PySecondaryStructureList(const AtomContainer& fragment, bool selected_only = false);

		/**	Destructor
		*/
		virtual ~PySecondaryStructureList() throw();
		//@}

		/**	@name Assignment
		*/
		//@{
		/**
		*/
		void set(const AtomContainer& fragment, bool selected_only = false);
		//@}
	};
   /** @} */
} // namespace BALL

#endif // BALL_PYTHON_PYSECONDARYSTRUCTURELIST_H
