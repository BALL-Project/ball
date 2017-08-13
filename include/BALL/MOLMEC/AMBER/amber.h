// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
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

#ifndef BALL_COMMON_EXCEPTION_H
# include <BALL/COMMON/exception.h>
#endif

namespace BALL 
{
	/**	AMBER force field class.
			
      \ingroup  AMBER
	*/
	class BALL_EXPORT AmberFF 
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
			/**	The parameter file name (@see Default::FILENAME)
			*/
			static const char* FILENAME;

			/**	Nonbonded cutoff. This value is used as cutoff radius in
					calculations of nonbonded interactions. The unit of this option
					is \f${\AA}\f$ (@see Default::NONBONDED_CUTOFF)
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

		/** Default values for AMBER options.
		*/
		struct BALL_EXPORT Default
		{
			/**	Default filename for the parameter file (@see Option::FILENAME).
			*/
			static const char* FILENAME;

			/**	Default non bonded cutoff of 20.0 \f${\AA}\f$.
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

			/**	Default electrosstatic scaling factor for 1-4 interaction. The
					default is 2.0.
			*/
			static const float SCALING_ELECTROSTATIC_1_4; 

			/**	Vdw scaling factor for 1-4 interaction
			*/
			static const float SCALING_VDW_1_4; 

			/**	Use of distance dependent dielectric constant.
          default: false 
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
		/** @name	Constructors and Destructor
		*/
		//@{


		BALL_CREATE(AmberFF)

		/**	Default constructor.
		*/
		AmberFF();

    /** Constructor.
    */
    AmberFF(System& system);

    /** Constructor.
    */
    AmberFF(System& system, const Options& options);
 
		/**	Copy constructor
		*/
		AmberFF(const AmberFF& force_field);

		/**	Destructor.
		*/
		virtual ~AmberFF();

		//@}
		/**	@name Assignment
		*/
		//@{

		/**	Assignment operator
		*/
		const AmberFF& operator = (const AmberFF& force_field)
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
		 * @throws BALL::Exception::TooManyErrors
		*/
		virtual bool specificSetup();

		//@}
		/**	@name Accessors specific to the AMBER force field
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

		//@}

		protected:
			
		String	filename_;

		bool		parameters_initialized_;

	};
} // namespace BALL

#endif // BALL_MOLMEC_AMBER_H
