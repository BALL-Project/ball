// $Id: amber.h,v 1.1 1999/08/26 07:53:21 oliver Exp $ 
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
		};

		struct Default
		{
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
	};

} // namespace BALL

#endif // BALL_MOLMEC_AMBER_H
