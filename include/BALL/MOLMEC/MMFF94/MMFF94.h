// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MMFF94.h,v 1.1.8.3 2007/06/15 09:24:20 amoll Exp $ 
//

// Molecular Mechanics: MMFF94 force field class

#ifndef BALL_MOLMEC_MMFF94_MMFF94_H
#define BALL_MOLMEC_MMFF94_MMFF94_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_MOLMEC_COMMON_FORCEFIELD_H
#	include <BALL/MOLMEC/COMMON/forceField.h>
#endif

#ifndef BALL_KERNEL_SYSTEM_H
#	include <BALL/KERNEL/system.h>
#endif

#ifndef BALL_MOLMEC_MMFF94_MMFF94PARAMETERS_H
# include <BALL/MOLMEC/MMFF94/MMFF94Parameters.h>
#endif

#ifndef BALL_MOLMEC_MMFF94_MMFF94PROCESSORS_H
# include <BALL/MOLMEC/MMFF94/MMFF94Processors.h>
#endif

#ifndef BALL_DATATYPE_HASHSET_H
# include <BALL/DATATYPE/hashSet.h>
#endif

#ifndef BALL_STRUCTURE_KEKULIZER_H
# include <BALL/STRUCTURE/kekulizer.h>
#endif

#include <vector>

namespace BALL 
{
	using std::vector;

	/**	MMFF94 force field class.
      \ingroup  MMFF94
	*/
	class BALL_EXPORT MMFF94 
		: public ForceField
	{
		public:

		/**	@name Constant Definitions
		*/
		//@{

		/**	Option names
		*/
		struct BALL_EXPORT Option
		{
			/**	The parameters folder
			*/
			static const char* FOLDER;

			/**	automatically assign charges to the system (during setup)
			*/
			static const char* ASSIGN_CHARGES;

			/**	automatically assign type names to the system (during setup)
			*/
			static const char* ASSIGN_TYPENAMES;

			/**	automatically assign types to the system's atoms (during setup)
			*/
			static const char* ASSIGN_TYPES;

			/**	during charge assignment, overwrite even non-zero charges
			*/
			static const char* OVERWRITE_CHARGES;

			/**	during charge assignment, overwrite even non-empty type names
			*/
			static const char* OVERWRITE_TYPENAMES;

			/**	Nonbonded cutoff. This value is used as cutoff radius in
					calculations of nonbonded interactions. The unit of this option
					is \f${\AA}\f$ (@see Default::NONBONDED_CUTOFF)
			*/
			static const char* NONBONDED_CUTOFF;

			/**	Van der Waals cutoff
			 		NOT YET IMPLEMENTED!
			*/
			static const char* VDW_CUTOFF;

			/**	Van der Waals cuton
			 		NOT YET IMPLEMENTED!
			*/
			static const char* VDW_CUTON;

			/**	Electrostatic cutoff
			*/
			static const char* ELECTROSTATIC_CUTOFF;

			/**	Electrostatic cuton
			*/
			static const char* ELECTROSTATIC_CUTON;

			///
			static const char* DISTANCE_DEPENDENT_DIELECTRIC;
		};

		/** Default values for MMFF94 options.
		*/
		struct BALL_EXPORT Default
		{
			/**	Default folder name for parameters (MMFF94)
			*/
			static const char* FOLDER;

			/**	automatically assign charges to the system (during setup)
			*/
			static const bool ASSIGN_CHARGES;

			/**	automatically assign type names to the system (during setup)
			*/
			static const bool ASSIGN_TYPENAMES;

			/**	automatically assign types to the system's atoms (during setup)
			*/
			static const bool ASSIGN_TYPES;

			/**	during charge assignment, overwrite even non-zero charges
			*/
			static const bool OVERWRITE_CHARGES;

			/**	during charge assignment, overwrite even non-empty type names
			*/
			static const bool OVERWRITE_TYPENAMES;

			/**	Default non bonded cutoff of 16.0 \f${\AA}\f$.
			*/
			static const float NONBONDED_CUTOFF;

			/**	Default van der Waals cutoff of 15.0 \f${\AA}\f$.
			*/
			static const float VDW_CUTOFF;

			/**	Default van der Waals cuton of 13.0 \f${\AA}\f$.
			*/
			static const float VDW_CUTON;

			/**	Default electrostatic cutoff of 15.0 \f${\AA}\f$.
			*/
			static const float ELECTROSTATIC_CUTOFF;

			/**	Default electrostatic cuton of 13.0 \f${\AA}\f$.
			*/
			static const float ELECTROSTATIC_CUTON;

			/// Default is false
			static const bool DISTANCE_DEPENDENT_DIELECTRIC;
		};

		//@}
		/** @name	Constructors and Destructor
		*/
		//@{

		BALL_CREATE(MMFF94)

		/**	Default constructor.
		*/
		MMFF94();

    /** Constructor.
    */
    MMFF94(System& system);

    /** Constructor.
    */
    MMFF94(System& system, const Options& options);
 
		/**	Copy constructor
		*/
		MMFF94(const MMFF94& force_field);

		/**	Destructor.
		*/
		virtual ~MMFF94();

		//@}
		/**	@name Assignment
		*/
		//@{

		/**	Assignment operator
		*/
		const MMFF94& operator = (const MMFF94& force_field)
			;

		/**	Clear method
		*/
		virtual void clear()
			;
		
		//@}
		/**	@name	Setup Methods
		*/
		//@{

		/**	Force field specific setup
		*/
		virtual bool specificSetup()
			throw(Exception::TooManyErrors);

		//@}
		/**	@name Accessors specific to the MMFF94 force field
		*/
		//@{

		/**	Return the bond stretch contribution to the total energy
		*/
		double getStretchEnergy() const;

		/**	Return the angle bend contribution to the total energy
		*/
		double getBendEnergy() const;

		///
		double getStretchBendEnergy() const;

		/**	Return the torsion contribution to the total energy.
				This energy comprises proper and improper torsions.
		*/
		double getTorsionEnergy() const;

		/**	Return the nonbonded contribution to the total energy.
				This energy comprises Van der Waals energy, hydrogen bond energy, and elesctrostatic energy.
		*/
		double getNonbondedEnergy() const;

		/**	Return the electrostatic contribution to the total energy.
		*/
		double getESEnergy() const;

		/**	Return the Van der Waals and hydrogen bond contribution to the total energy.
		*/
		double getVdWEnergy() const;

		///
		double getPlaneEnergy() const;

		/**	Return true, if the parameters have already been initialized
		*/
		bool hasInitializedParameters() const;

		/**	Return the recommended number of iterations between updates.
				This method return 20 as a default value.
		*/
		Size getUpdateFrequency() const;

		/// Get the current results in String form
		virtual String getResults() const
			;

		///
		const vector<Bond*> getBonds() const { return bonds_;}
		
		///
		const vector<HashSet<Atom*> >& getRings() const { return rings_;}
		
		///
		const vector<HashSet<Atom*> >& getAromaticRings() const { return aromatic_rings_;}
		
		///
		bool isInOneAromaticRing(const Bond& bond) const;
		
		///
		bool assignMMFF94BondType(Bond& bond) const;

		///
		const vector<MMFF94AtomType>& getAtomTypes() const { return atom_types_.getAtomTypes();}

		///
 		const MMFF94StretchParameters& getStretchParameters() const { return bond_parameters_;}

		///
		const MMFF94AtomTypeEquivalences & getEquivalences() const { return equivalences_;}

		///
		bool areInOneRing(vector<Atom*> v, Size ring_size = 0) const;
		
		///
		bool areInOneAromaticRing(const vector<Atom*>& v, Size ring_size = 0) const;

		/** Check wheter the atoms type is valid.
		 		If not, add it to unassigned atoms.
		*/
		bool checkAtomType(Atom& atom);

		//@}

		protected:

		void insertComponents_();

		void transformAromaticBonds_();
		void collectBonds_();
		void assignBondTypes_();
		void collectRings_();
			
		String											folder_;
		MMFF94AtomTypes 						atom_types_;
		MMFF94StretchParameters 		bond_parameters_;
		MMFF94AtomTypeEquivalences  equivalences_;
		MMFF94ESParameters 					es_parameters_;
		vector<HashSet<Atom*> > 		rings_;
		vector<HashSet<Atom*> > 		aromatic_rings_;
		bool												parameters_initialized_;
		vector<Bond*> 							bonds_;
		MMFF94AtomTyper 						atom_typer_;
		MMFF94ChargeProcessor 			charge_processor_;
		Kekuliser 									kekuliser_;
		HashSet<Bond*> 							aromatic_bonds_;
	};

} // namespace BALL

#endif // BALL_MOLMEC_MMFF94_H
