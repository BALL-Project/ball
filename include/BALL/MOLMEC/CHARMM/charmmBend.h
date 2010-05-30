// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

// Molecular Mechanics: Charmm force field, bond stretch component

#ifndef BALL_MOLMEC_CHARMM_CHARMMBEND_H
#define BALL_MOLMEC_CHARMM_CHARMMBEND_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_MOLMEC_COMMON_FORCEFIELDCOMPONENT_H
#	include <BALL/MOLMEC/COMMON/forceFieldComponent.h>
#endif

#ifndef BALL_MOLMEC_PARAMETER_QUADRATICANGLEBEND_H
#	include <BALL/MOLMEC/PARAMETER/quadraticAngleBend.h>
#endif

#ifndef BALL_MOLMEC_COMMON_FORCEFIELD_H
#	include <BALL/MOLMEC/COMMON/forceField.h>
#endif

namespace BALL 
{
	/**	Charmm bond stretch component
			
    	\ingroup  CHARMM
	*/
	class BALL_EXPORT CharmmBend 
		: public ForceFieldComponent
	{
		public:

		BALL_CREATE(CharmmBend)

		/// flag to enable BENDS 
		#define CHARMM_BENDS_ENABLED "enable Bends"

		/**@name	Constructors and Destructors	
		*/
		//@{ 

		/**	Default constructor.
		*/
		CharmmBend();

		/**	Constructor.
		*/
		CharmmBend(ForceField& force_field);

		/**	Copy constructor
		*/
		CharmmBend(const CharmmBend& charmm_stretch);

		/**	Destructor.
		*/
		virtual ~CharmmBend();

		//@}
		/**	@name	Setup Methods	
		*/
		//@{

		/**	Setup method.
		*/
		virtual bool setup()
			throw(Exception::TooManyErrors);

		//@}
		/**	@name	Accessors	
		*/
		//@{

		/**	Calculates and returns the component's energy.
		*/
		virtual double updateEnergy();

		/**	Calculates and returns the component's forces.
		*/
		virtual void updateForces();

		//@}

		private:

		/*_	@name	Private Attributes	
		*/
		//_@{

		/*_	pointer to the array of bends
		*/	
		vector<QuadraticAngleBend::Data>	bend_;

		QuadraticAngleBend								bend_parameters_;

		//_@}
	 
	};
} // namespace BALL

#endif // BALL_MOLMEC_CHARMM_CHARMMBEND_H
