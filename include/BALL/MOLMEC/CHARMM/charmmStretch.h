// $Id: charmmStretch.h,v 1.7 2001/02/28 01:17:38 amoll Exp $ 
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
			{\bf Definition:} \URL{BALL/MOLMEC/CHARMM/charmmStretch.h}
	*/
	class CharmmStretch 
		: public ForceFieldComponent
	{
		public:

		/**	@name	Constructors and Destructors	
		*/
		//@{

		BALL_CREATE_DEEP(CharmmStretch)

		/**	Default constructor.
		*/
		CharmmStretch();

		/**	Constructor.
		*/
		CharmmStretch(ForceField& force_field);

		/**	Copy constructor
		*/
		CharmmStretch(const CharmmStretch& charmm_stretch, bool clone_deep = true);

		/**	Destructor.
		*/
		virtual ~CharmmStretch();

		//@}
		/**	@name Setup Methods	
		*/
		//@{

		/**	Setup method.
		*/
		virtual bool setup();

		//@}

		/**	@name Accessors		
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

		/*_
		*/
		QuadraticBondStretch::Data*	stretch_;

		/*_	The number of stretches in the system 
		*/
		Size												number_of_stretches_;

		/*_	The stretch parameters section
		*/
		QuadraticBondStretch				stretch_parameters_;

		//_@}
		
	};

} // namespace BALL

#endif // BALL_MOLMEC_CHARMM_CHARMMSTRETCH_H
