// $Id: charmm.h,v 1.6 2001/02/28 01:17:38 amoll Exp $
// Molecular Mechanics: CHARMM force field class

#ifndef BALL_MOLMEC_CHARMM_CHARMM_H
#define BALL_MOLMEC_CHARMM_CHARMM_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_MOLMEC_COMMON_FORCEFIELD_H
#	include <BALL/MOLMEC/COMMON/forceField.h>
#endif

#ifndef BALL_KERNEL_SYSTEM_H
#	include <BALL/KERNEL/system.h>
#endif


namespace BALL 
{

	/**	CHARMM force field class.
			{\bf Definition:} \URL{BALL/MOLMEC/CHARMM/charmm.h}
	*/
	class CharmmFF 
		: public ForceField
	{
		public:

		/**	@name Constant Definitions
		*/
		//@{

		/**	Options names
		*/
		struct Option
		{
			/**	The parameter file name
			*/
			static const char* FILENAME;

			/**	Nonbonded cutoff
			*/
			static const char* NONBONDED_CUTOFF;

			/**	Van der Waals cutoff
			*/
			static const char* VDW_CUTOFF;

			/**	Van der Waals cuton
			*/
			static const char* VDW_CUTON;

			/**	Electrostatic cutoff
			*/
			static const char* ELECTROSTATIC_CUTOFF;

			/**	Electrostatic cuton
			*/
			static const char* ELECTROSTATIC_CUTON;

			/**	Solvation cutoff
			*/
			static const char* SOLVATION_CUTOFF;

			/**	Solvation cuton
			*/
			static const char* SOLVATION_CUTON;

			/**	1-4 vdw interaction scaling factor.
			*/
			static const char* SCALING_VDW_1_4;

			/**	Use EEF1 solvation method.
			*/
			static const char* USE_EEF1;

			/**	1-4 electrostatic interaction scaling factor.
			*/
			static const char* SCALING_ELECTROSTATIC_1_4;

			/**	use of distance dependent dielectric constant
			*/
			static const char* DISTANCE_DEPENDENT_DIELECTRIC; 

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
		};

		/** Default values
		*/
		struct Default
		{
			/**	Default filename for the parameter file
			*/
			static const char* FILENAME;

			/**	Non bonded cutoff
			*/
			static const float NONBONDED_CUTOFF;

			/**	Van der Waals cutoff
			*/
			static const float VDW_CUTOFF;

			/**	Van der Waals cuton
			*/
			static const float VDW_CUTON;

			/**	Electrostatic cutoff
			*/
			static const float ELECTROSTATIC_CUTOFF;

			/**	Electrostatic cuton
			*/
			static const float ELECTROSTATIC_CUTON;

			/**	Solvation cutoff
			*/
			static const float SOLVATION_CUTOFF;

			/**	Solvation cuton
			*/
			static const float SOLVATION_CUTON;

			/**	Electrosstatic scaling factor for 1-4 interaction
			*/
			static const float SCALING_ELECTROSTATIC_1_4; 

			/**	Use EEF1 solvation method.
			*/
			static const bool USE_EEF1;

			/**	Vdw scaling factor for 1-4 interaction
			*/
			static const float SCALING_VDW_1_4; 

			/**	use of distance dependent dielectric constant, default = false 
			*/
			static const bool DISTANCE_DEPENDENT_DIELECTRIC; 

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
		};

		//@}
		/** @name	Constructors and Destructors
		*/
		//@{

		BALL_CREATE(CharmmFF)

		/**	Default constructor.
		*/
		CharmmFF();

    /** Constructor.
    */
    CharmmFF(System& system);

    /** Constructor.
    */
    CharmmFF(System& system, const Options& options);
 
		/**	Copy constructor
		*/
		CharmmFF(const CharmmFF& force_field);

		/**	Destructor.
		*/
		virtual ~CharmmFF();

		//@}
		/**	@name Assignment
		*/
		//@{
			
		/**	Assignment operator
		*/
		const CharmmFF& operator = (const CharmmFF& force_field)
			throw();
			
		/**	Clear method	
		*/
		virtual void clear()
			throw();

		//@}
		/**	@name	Setup Methods
		*/
		//@{

		/**	Force field specific setup
		*/
		virtual bool specificSetup();

		//@}
		/**	@name	Accessors specific to the CHARMM force field
		*/
		//@{

		/**	Return the bond stretch contribution to the total energy
		*/
		double getStretchEnergy() const;

		/**	Return the angle bend contribution to the total energy
		*/
		double getBendEnergy() const;

		/**	Return the torsion contribution to the total energy.
				This energy comprises proper and improper torsions.
		*/
		double getTorsionEnergy() const;

		/**	Return the improper torsion contribution to the total energy.
		*/
		double getImproperTorsionEnergy() const;

		/**	Return the proper torsion contribution to the total energy.
		*/
		double getProperTorsionEnergy() const;

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

		/**	Return the solvation contribution to the total energy.
		*/
		double getSolvationEnergy() const;

		/**	Return true, if the parameters have already been initialized
		*/
		bool hasInitializedParameters() const;

		//@}

		protected:
			
		String	filename_;

		bool		parameters_initialized_;

	};

} // namespace BALL

#endif // BALL_MOLMEC_CHARMM_CHARMM_H
