// $Id: charmmBend.h,v 1.1 2000/02/10 15:29:58 oliver Exp $
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
	*/
	class CharmmBend 
		: public ForceFieldComponent
	{
		public:

		/**@name	Constructors and Destructors	*/
		//@{ 

		/**	Default constructor.
		*/
		CharmmBend();

		/**	constructor.
		*/
		CharmmBend(ForceField* force_field);

		/**	Copy constructor
		*/
		CharmmBend(const CharmmBend& charmm_stretch, bool clone_deep = true);

		/**	Destructor.
		*/
		virtual ~CharmmBend();

		//@}


		/**	@name	Setup Methods	*/
		//@{

		/**	Setup method.
		*/
		virtual bool setup();

		//@}


		/**	@name	Accessors	*/
		//@{

		/**	Calculates and returns the component's energy.
		*/
		virtual float updateEnergy();

		/**	Calculates and returns the component's forces.
		*/
		virtual void updateForces();

		//@}


		private:

		/*_	@name	Private Attributes	*/
		//_@{

		/*_	pointer to the array of bends
		*/	
		vector<FFPSQuadraticAngleBend::QuadraticAngleBend> bend_;

		FFPSQuadraticAngleBend	bend_parameters_;
		//_@}
	 
	};

} // namespace BALL

#endif // BALL_MOLMEC_CHARMM_CHARMMBEND_H
