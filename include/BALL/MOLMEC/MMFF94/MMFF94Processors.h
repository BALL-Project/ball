// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MMFF94Processors.h,v 1.1.2.8 2006/03/01 18:12:41 amoll Exp $ 
//

#ifndef BALL_MOLMEC_MMFF94_PROCESSORS_H
#define BALL_MOLMEC_MMFF94_PROCESSORS_H

#ifndef BALL_MOLMEC_MMFF94_MMFF94PARAMETERS_H
# include <BALL/MOLMEC/MMFF94/MMFF94Parameters.h>
#endif

#ifndef BALL_DATATYPE_HASHSET_H
# include <BALL/DATATYPE/hashSet.h>
#endif

#ifndef BALL_DATATYPE_STRINGHASHMAP_H
# include <BALL/DATATYPE/stringHashMap.h>
#endif

#include <vector>

namespace BALL 
{
	using std::vector;

	class MMFF94ESParameters;
	class Molecule;
	class System;

	///
	class BALL_EXPORT AtomTyper
	{
		public:

		BALL_CREATE(AtomTyper)

		///
		AtomTyper();

		///
		AtomTyper(const AtomTyper& t);

		///
		virtual ~AtomTyper() {};

		///
		bool setup(const String& filename);
		
		///
		virtual void assignTo(System& s);

		protected:
		
		///
		void assignTo(Molecule& mol);

		/** To be overloaded in derived classes 
				to add support for additional properties e.g. charges
		*/
		virtual void assignSpecificValues_(Atom&) {};

		/** To be overloaded in derived classes 
				to add support for additional properties e.g. charges
		*/
		virtual bool specificSetup_() { return true;}

		vector<String> 		names_;
		vector<String> 		rules_;
		vector<Index> 		types_;
		// entries in the file for specificSetup_() :
		vector<vector<String> > fields_;
		// number for fields per line in the config file
		Size 							number_expected_fields_;
	};
	
	///
	class BALL_EXPORT MMFF94AtomTyper
		: public AtomTyper
	{
		public:

		BALL_CREATE(MMFF94AtomTyper)

		///
		MMFF94AtomTyper();

		///
		MMFF94AtomTyper(const MMFF94AtomTyper& t);

		///
		virtual ~MMFF94AtomTyper() {};

		///
		virtual void assignTo(System& s);

		/// Read the hydrogen matching from MMFFHDEF.PAR
		virtual bool setupHydrogenTypes(const String& filename);

		/// Read the matching from ID to numerical type from MFFSYMB.PAR
		virtual bool setupSymbolsToTypes(const String& filename);

		protected:

		StringHashMap<String> type_to_htype_;
		StringHashMap<Position> id_to_type_;
	};
		

	/**	Assign MMFF94 Charges
      \ingroup  MMFF94
	*/
	class BALL_EXPORT MMFF94ChargeProcessor
		:	public UnaryProcessor<Atom>
	{
		public:

		BALL_CREATE(MMFF94ChargeProcessor)

		///
		MMFF94ChargeProcessor();

		///
    MMFF94ChargeProcessor(const MMFF94ChargeProcessor& cp);
 
		///
		virtual ~MMFF94ChargeProcessor() {};

		///
		const MMFF94ChargeProcessor& operator = (const MMFF94ChargeProcessor& cp)
			throw();

		///
		virtual void clear()
			throw();
		
		///
		virtual bool start();

		///
		virtual bool finish();

		///
		virtual Processor::Result operator () (Atom& atom);

		///
		void assignFormalCharge(Atom& atom);
		
		///
		const HashSet<Atom*>& getUnassignedAtoms() { return unassigned_atoms_;}

		///
		void setESParameters(const MMFF94ESParameters& es) { es_parameters_ = &es; }

		protected:

		vector<Atom*> 							atoms_;
		HashSet<Atom*> 							unassigned_atoms_;
		const MMFF94ESParameters* 	es_parameters_;
	};

} // namespace BALL

#endif // BALL_MOLMEC_MMFF94_PROCESSORS_H
