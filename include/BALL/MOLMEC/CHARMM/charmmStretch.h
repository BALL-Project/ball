// $Id: charmmStretch.h,v 1.1 2000/02/10 15:29:59 oliver Exp $ 
// Molecular Mechanics: Charmm force field, bond stretch component

#ifndef BALL_MOLMEC_CHARMM_CHARMMSTRETCH_H
#define BALL_MOLMEC_CHARMM_CHARMMSTRETCH_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_MOLMEC_COMMON_FORCEFIELDCOMPONENT_H
#	include <BALL/MOLMEC/COMMON/forceFieldComponent.h>
#endif

#ifndef BALL_MOLMEC_COMMON_FORCEFIELD_H
#	include <BALL/MOLMEC/COMMON/forceField.h>
#endif

#ifndef BALL_MOLMEC_PARAMETER_QUADRATICBONDSTRETCH_H
#	include <BALL/MOLMEC/PARAMETER/quadraticBondStretch.h>
#endif

namespace BALL 
{

	/**	Charmm bond stretch component
	*/
	class CharmmStretch 
		: public ForceFieldComponent
	{
		public:

		/**	@name	Constructors and Destructors	*/
		//@{

		BALL_CREATE(CharmmStretch)

		/**	Default constructor.
		*/
		CharmmStretch();

		/**	constructor.
		*/
		CharmmStretch(ForceField* force_field);

		/**	Copy constructor
		*/
		CharmmStretch(const CharmmStretch& charmm_stretch, bool clone_deep = true);

		/**	Destructor.
		*/
		virtual ~CharmmStretch();

		//@}


		/**	@name Setup Methods	*/
		//@{

		/**	Setup method.
		*/
		virtual bool setup();

		//@}

		/**	@name Accessors		*/
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

		/*_
		*/
		FFPSQuadraticBondStretch::QuadraticStretch*	stretch_;

		/*_	The number of stretches in the system 
		*/
		Size number_of_stretches_;

		/*_	The stretch parameters section
		*/
		FFPSQuadraticBondStretch  stretch_parameters_;
		//_@}
		
	};

} // namespace BALL

#endif // BALL_MOLMEC_CHARMM_CHARMMSTRETCH_H
