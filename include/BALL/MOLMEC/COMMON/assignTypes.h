// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: assignTypes.h,v 1.10 2003/03/03 08:17:20 sturm Exp $
// Molecular Mechanics: atom type assignment

#ifndef BALL_MOLMEC_COMMON_ASSIGNTYPES_H
#define BALL_MOLMEC_COMMON_ASSIGNTYPES_H

#ifndef BALL_CONCEPT_PROCESSOR_H
#	include <BALL/CONCEPT/processor.h>
#endif

#ifndef BALL_MOLMEC_PARAMETER_ATOMTYPES_H
#	include <BALL/MOLMEC/PARAMETER/atomTypes.h>
#endif

#include <vector>

namespace BALL 
{
    /** \ingroup MolmecCommon
     *  @{
     */	
	/**	Type assignment processor.
			<b>Definition:</b> BALL/MOLMEC/COMMON/assignTypes.h
	*/
	class	AssignTypeProcessor
		:	public UnaryProcessor<Atom>
	{
		public:

		/**	@name Constructors and Destructors
		*/
		//@{

		/**
		*/
		AssignTypeProcessor(const AtomTypes& atom_types);

		//@}
		/**	@name Processor related methods
		*/
		//@{

		/**
		*/
		virtual Processor::Result operator () (Atom& atom);

		//@}

		protected:
		
		AtomTypes atom_types_;

	};


	/**	Assign type names to atoms.
	*/
	class	AssignTypeNameProcessor
		:	public UnaryProcessor<Atom>
	{
		public:

		/**	@name Constructors and Destructors
		*/
		//@{

		/**
		*/
		AssignTypeNameProcessor(const String& filename, bool overwrite = false);

		//@}
		/**	@name	Processor related methods
		*/
		//@{

		/**
		*/
		virtual Processor::Result operator () (Atom& atom);

		//@}

		protected:
		
		StringHashMap<String>	type_map_;
		
		bool overwrite_;
	};
     /** @} */
} // namespace BALL


#endif // BALL_MOLMEC_COMMON_ASSIGNTYPES_H
