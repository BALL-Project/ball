// $Id: assignTypes.h,v 1.2 1999/09/04 09:20:50 oliver Exp $
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

namespace BALL {
	
	class	AssignTypeProcessor
		:	public UnaryProcessor<Atom>
	{
		public:

		AssignTypeProcessor(const FFPSAtomTypes& atom_types);

		virtual Processor::Result operator () (Atom& atom);

		
		protected:
		
		FFPSAtomTypes atom_types_;
	};

	/**	Assign type names to atoms.
	*/
	class	AssignTypeNameProcessor
		:	public UnaryProcessor<Atom>
	{
		public:

		/**
		*/
		AssignTypeNameProcessor(const String& filename, bool overwrite = false);

		/**
		*/
		virtual Processor::Result operator () (Atom& atom);

		
		protected:
		
		StringHashMap<String>	type_map_;
		
		bool overwrite_;
	};

} // namespace BALL


#endif // BALL_MOLMEC_COMMON_ASSIGNTYPES_H
