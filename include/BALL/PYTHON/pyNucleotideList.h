// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyNucleotideList.h,v 1.11 2004/05/27 19:49:45 oliver Exp $
//

#ifndef BALL_PYTHON_PYNUCLEOTIDELIST_H
#define BALL_PYTHON_PYNUCLEOTIDELIST_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_DATAYPE_LIST_H
#	include <BALL/DATATYPE/list.h>
#endif

namespace BALL 
{

	class Nucleotide;
	class AtomContainer;
	
	/** Equivalent for a STL::List of Nucleotide Pointers in Python
			\ingroup PythonExtensions
	*/
	class PyNucleotideList
		:	public List<Nucleotide*>
	{
		public:

		BALL_CREATE(PyNucleotideList)

		/**	@name	Type Definitions
		*/
		//@{

		/**	Nucleotide* type
		*/
		typedef Nucleotide* ValueType;

		/**	Pointer type
		*/
		typedef Nucleotide** PointerType;

		/**	Iterator type.
		*/
		typedef List<Nucleotide*>::iterator Iterator;

		/**	Constant iterator type.
		*/
		typedef List<Nucleotide*>::const_iterator ConstIterator;

		//@}

		/**	@name	Constructors and Destructors */
		//@{

		/**	Default constructor.
				Create an empty list.
		*/
		PyNucleotideList();

		/** Copy constructor.
				Create a copy of an existing list.
				@param	map the list to be copied
				@param	deep ignored
		*/
		PyNucleotideList(const PyNucleotideList& new_list);
			
		/**	Construct from a AtomContainer
				This constructor creates an PyNucleotideList object from
				all nucleotides of a  \link AtomContainer AtomContainer \endlink  object.
		*/
		PyNucleotideList(const AtomContainer& fragment, bool selected_only = false);

		/**	Destructor
		*/
		virtual ~PyNucleotideList() throw();
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

#endif // BALL_PYTHON_PYNUCLEOTIDELIST_H
