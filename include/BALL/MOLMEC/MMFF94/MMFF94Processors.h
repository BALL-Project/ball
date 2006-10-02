// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MMFF94Processors.h,v 1.1.4.8 2006/10/02 17:01:40 amoll Exp $ 
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

#ifndef BALL_KERNEL_BOND_H
# include <BALL/KERNEL/bond.h>
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
		
		///
		void setAromaticRings(const vector<HashSet<Atom*> >& rings) { aromatic_rings_ = rings;}

		///
		void setRings(const vector<HashSet<Atom*> >& rings) { rings_ = rings;}

		// just for debugging: computational times for the individual SMARTS expressions
		static StringHashMap<float> rule_times;

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
		vector<HashSet<Atom*> > 		rings_;
		vector<HashSet<Atom*> > 		aromatic_rings_;
		HashMap<String, vector<Position> > element_to_rules_;
		HashSet<Atom*> atoms_;
	};
	
	///
	class BALL_EXPORT MMFF94AtomTyper
		: public AtomTyper
	{
		public:

		struct AromaticType
		{
			String new_type;
			Position atomic_number;
			bool     cation;
			bool     anion;
		};

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
		
		/// Read the matching from general ID to ID in aromatic rings
		virtual bool setupAromaticTypes(const String& filename);

		///
		void collectHeteroAtomTypes(const MMFF94AtomTypes& atom_types);
		
		protected:

		bool assignAromaticType_5_(Atom& atom, Position L5, bool anion, bool cation);

		StringHashMap<String> 		partner_type_to_htype_;
		StringHashMap<Position> 	id_to_type_;
		HashMap<String, AromaticType> aromatic_types_5_map_;
		HashSet<String> 					cation_atoms_;
		MMFF94AtomTypes* 					atom_types_;
		HashSet<Position> 				hetero_atom_types_;
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

		///
		void setup(const String& filename);

		///
		void setAromaticRings(const vector<HashSet<Atom*> >& rings) { aromatic_rings_ = rings;}

		protected:

		void assignPartialCharges_();

		vector<Atom*> 							atoms_;
		HashSet<Atom*> 							unassigned_atoms_;
		const MMFF94ESParameters* 	es_parameters_;
		HashMap<String, float> 			types_to_charges_;
		HashSet<String> 						rule_types_;
		vector<HashSet<Atom*> > 		aromatic_rings_;
	};
	

	///
	class BALL_EXPORT Kekuliser
	{
		struct AtomInfo
		{
			Atom* atom;

			// needed for sorting:
			bool operator < (const AtomInfo& info) const;

			vector<Bond*> abonds;
			vector<Position> partner_id;

		 	Index current_charge;

			Index curr_double;

			Index min_double;
			Index max_double;

			Index min_double_charged;
			Index max_double_charged;
		};

		public:

		BALL_CREATE(Kekuliser)

		///
		Kekuliser();

		///
		virtual ~Kekuliser() {}

		///
		bool setup(Molecule& mol);

		///
		void setAromaticRings(const vector<HashSet<Atom*> >& rings) { aromatic_rings_ = rings;}
		
		///
		const vector<Bond*>& getUnassignedBonds() const { return unassigned_bonds_; }

		protected:

		bool fixAromaticRings_();
		bool fixAromaticSystem_(Position it);
		bool buildConjugatedSystem_(Position it);
 		bool idealValenceAchieved_();

		void getMaximumValence_();

		// merge aromatic rings:
		void calculateAromaticSystems_();
		void collectSystems_(Atom& atom);

		vector<HashSet<Atom*> > aromatic_systems_;
		vector<HashSet<Atom*> > aromatic_rings_;
		vector<Bond*> 					unassigned_bonds_;

		// atoms that take part in an aromatic bond:
		HashSet<const Atom*> 		aromatic_atoms_;
		HashMap<Atom*, Index> 	max_valence_;

		HashSet<Atom*> 					current_aromatic_system_;

		// current aromatic system:
		vector<AtomInfo> 				atom_infos_;
		bool 										try_charge_;
	};

} // namespace BALL

#endif // BALL_MOLMEC_MMFF94_PROCESSORS_H
