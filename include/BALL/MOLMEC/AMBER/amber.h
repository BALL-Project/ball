// $Id: amber.h,v 1.4 1999/09/21 11:06:53 oliver Exp $ 
// Molecular Mechanics: Amber force field class

#ifndef BALL_MOLMEC_AMBER_AMBER_H
#define BALL_MOLMEC_AMBER_AMBER_H

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

	/**	Force field class.
	*/
	class AmberFF 
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

			/**	Electrostatic cutoff
			*/
			static const char* ELECTROSTATIC_CUTOFF;

			/**	1-4 vdw interaction scaling factor.
			*/
			static const char* SCALING_VDW_1_4;

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
			/**	Electrostatic cutoff
			*/
			static const float ELECTROSTATIC_CUTOFF;
			/**	Electrosstatic scaling factor for 1-4 interaction
			*/
			static const float SCALING_ELECTROSTATIC_1_4; 
			/**	Vdw scaling factor for 1-4 interaction
			*/
			static const float SCALING_VDW_1_4; 

			/**	use of distance dependent dielectric constant
                                default = false 
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


		/**	@name Enums
		*/
		//@{

		/**	Error codes of the AmberFF class
		*/
		enum ErrorCode 
		{
			ERROR__UNKNOWN = 0,

			NUMBER_OF_ERRORS
		};

		//@}

		/**@name	Constructors and Destructors
		*/
		//@{


		BALL_CREATE(AmberFF)


		/**	Default constructor.
		*/
		AmberFF(void);

    /** Constructor.
    */
    AmberFF(System& system);

    /** Constructor.
    */
    AmberFF(System& system, const Options& options);
 
		/**	Copy constructor
		*/
		AmberFF(const AmberFF& force_field, bool clone_deep = true);

		/**	Destructor.
		*/
		virtual ~AmberFF(void);

		//@}

		/**	@name	Setup Methods
		*/
		//@{

		/**	Force field specific setup
		*/
		virtual bool specificSetup(void);

		//@}

		/**	Accessors specific to the AMBER force field
		*/
		//@{
		/**	Return the bond stretch contribution to the total energy
		*/
		float getStretchEnergy() const;

		/**	Return the angle bend contribution to the total energy
		*/
		float getBendEnergy() const;

		/**	Return the torsion contribution to the total energy.
				This energy comprises proper and improper torsions.
		*/
		float getTorsionEnergy() const;

		/**	Return the nonbonded contribution to the total energy.
				This energy comprises Van der Waals energy, hydrogen bond energy, and elesctrostatic energy.
		*/
		float getNonbondedEnergy() const;

		/**	Return the electrostatic contribution to the total energy.
		*/
		float getESEnergy() const;

		/**	Return the Van der Waals and hydrogen bond contribution to the total energy.
		*/
		float getVdWEnergy() const;

		/**	Return true, if the parameters have already been initialized
		*/
		bool hasInitializedParameters() const;
		//@}

		protected:
			
		String	filename_;

		bool		parameters_initialized_;

	};

} // namespace BALL

#endif // BALL_MOLMEC_AMBER_H
