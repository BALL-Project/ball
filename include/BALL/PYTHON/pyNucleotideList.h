// $Id: pyNucleotideList.h,v 1.2 2000/08/30 19:58:24 oliver Exp $

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
	
	/**
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
				all nucleotides of a \Ref{AtomContainer} object.
		*/
		PyNucleotideList(const AtomContainer& fragment, bool selected_only = false);

		/**	Destructor
		*/
		virtual ~PyNucleotideList();
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
