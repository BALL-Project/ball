// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: assignTypes.h,v 1.16 2005/12/23 17:01:51 amoll Exp $
//

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
	/// Only used for deriving interface
	class	BALL_EXPORT AssignBaseProcessor
		:	public UnaryProcessor<Atom>
	{
		public:

		///
		AssignBaseProcessor();

		/** Set the number of atoms, for which the setup of the forcefield can
		    fail, until the setup() methods aborts and return false.
				By default, there is no limit set.
		*/
		void setMaximumUnassignedAtoms(Size nr);

		/** Get the number of atoms, for which the setup of the forcefield can
		    fail, until the setup() methods aborts and return false.
		*/
		Size getMaximumUnassignedAtoms() const;

		/// Get the number of atoms, for which the force field setup failed.
		Size getNumberOfUnassignedAtoms() const;

		/// Get the atoms, for which the force field setup failed.
		HashSet<const Atom*>& getUnassignedAtoms();

		protected:

		//_ Atoms, for which the setup of the force field fails
		HashSet<const Atom*> unassigned_atoms_;

		//_ max number of unassigned atoms
		Size max_number_unassigned_atoms_;
	};


	/**	Type assignment processor.
    	\ingroup  MolmecCommon
	*/
	class	BALL_EXPORT AssignTypeProcessor
		:	public AssignBaseProcessor
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
    	\ingroup  MolmecCommon
	*/
	class	BALL_EXPORT AssignTypeNameProcessor
		:	public AssignBaseProcessor
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
} // namespace BALL


#endif // BALL_MOLMEC_COMMON_ASSIGNTYPES_H
