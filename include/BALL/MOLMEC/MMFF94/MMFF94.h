// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MMFF94.h,v 1.1.2.2 2005/03/24 16:17:38 amoll Exp $ 
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

#ifndef BALL_COMMON_EXCEPTION_H
# include <BALL/COMMON/exception.h>
#endif

#ifndef BALL_MOLMEC_MMFF94_MMFF94PARAMETERS_H
# include <BALL/MOLMEC/MMFF94/MMFF94Parameters.h>
#endif

namespace BALL 
{
	/**	MMFF94 force field class.
      \ingroup  MMFF94
	*/
	class MMFF94 
		: public ForceField
	{
		public:

		/**	@name Constant Definitions
		*/
		//@{

		/**	Option names
		*/
		struct Option
		{
			/**	The parameter file name (@see Default::FILENAME)
			*/
			static const char* FOLDER;
		};

		/** Default values for MMFF94 options.
		*/
		struct Default
		{
			/**	Default filename for the parameter file (@see Option::FILENAME).
			*/
			static const char* FOLDER;
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
			throw();

		///
		const vector<MMFF94AtomTypeData>& getAtomTypes() const { return atom_types_.getAtomTypes();}

		//@}

		protected:
			
		String										folder_;
		MMFF94AtomTypesContainer 	atom_types_;
		bool											parameters_initialized_;

	};
} // namespace BALL

#endif // BALL_MOLMEC_MMFF94_H
