// $Id: amberStretch.h,v 1.5 2000/02/06 19:45:01 oliver Exp $ 
// Molecular Mechanics: Amber force field, bond stretch component

#ifndef BALL_MOLMEC_AMBER_AMBERSTRETCH_H
#define BALL_MOLMEC_AMBER_AMBERSTRETCH_H

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

	/**	Amber bond stretch component
	*/
	class AmberStretch 
		: public ForceFieldComponent
	{
		public:

		/**	@name	Constructors and Destructors	*/
		//@{

		BALL_CREATE(AmberStretch)

		/**	Default constructor.
		*/
		AmberStretch();

		/**	constructor.
		*/
		AmberStretch(ForceField* force_field);

		/**	Copy constructor
		*/
		AmberStretch(const AmberStretch& amber_stretch, bool clone_deep = true);

		/**	Destructor.
		*/
		virtual ~AmberStretch();

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

#endif // BALL_MOLMEC_AMBER_AMBERSTRETCH_H
