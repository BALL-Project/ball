// $Id: pyNucleicAcidList.h,v 1.2 2000/08/30 19:58:24 oliver Exp $

#ifndef BALL_PYTHON_PYNUCLEICACIDLIST_H
#define BALL_PYTHON_PYNUCLEICACIDLIST_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_DATAYPE_LIST_H
#	include <BALL/DATATYPE/list.h>
#endif

namespace BALL 
{
	class NucleicAcid;
	class AtomContainer;
	
	/**
	*/
	class PyNucleicAcidList
		:	public List<NucleicAcid*>
	{
		public:

		BALL_CREATE(PyNucleicAcidList)

		/**	@name	Type Definitions
		*/
		//@{

		/**	NucleicAcid* type
		*/
		typedef NucleicAcid* ValueType;

		/**	Pointer type
		*/
		typedef NucleicAcid** PointerType;

		/**	Iterator type.
		*/
		typedef List<NucleicAcid*>::iterator Iterator;

		/**	Constant iterator type.
		*/
		typedef List<NucleicAcid*>::const_iterator ConstIterator;

		//@}

		/**	@name	Constructors and Destructors */
		//@{

		/**	Default constructor.
				Create an empty list.
		*/
		PyNucleicAcidList();

		/** Copy constructor.
				Create a copy of an existing list.
				@param	map the list to be copied
				@param	deep ignored
		*/
		PyNucleicAcidList(const PyNucleicAcidList& new_list);
			
		/**	Construct from a AtomContainer
				This constructor creates an PyNucleicAcidList object from
				all nucleic acids of a \Ref{AtomContainer} object.
		*/
		PyNucleicAcidList(const AtomContainer& fragment, bool selected_only = false);

		/**	Destructor
		*/
		virtual ~PyNucleicAcidList();
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

#endif // BALL_PYTHON_PYNUCLEICACIDLIST_H
