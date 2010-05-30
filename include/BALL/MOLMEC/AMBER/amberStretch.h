// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
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
			
    	\ingroup  AMBER
	*/
	class BALL_EXPORT AmberStretch 
		: public ForceFieldComponent
	{
		public:

		/// flag to enable Stretches
		#define AMBER_STRETCH_ENABLED "enable Stretches"

		/**	@name	Constructors and Destructors	
		*/
		//@{

		BALL_CREATE(AmberStretch)

		/**	Default constructor.
		*/
		AmberStretch();

		/**	Constructor.
		*/
		AmberStretch(ForceField& force_field);

		/**	Copy constructor
		*/
		AmberStretch(const AmberStretch& amber_stretch);

		/**	Destructor.
		*/
		virtual ~AmberStretch();

		//@}
		/**	@name Setup Methods	
		*/
		//@{

		/**	Setup method.
		*/
		virtual bool setup()
			throw(Exception::TooManyErrors);

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

		/**	Update the bond list.
		*/
		virtual void update()
			throw(Exception::TooManyErrors);

		//@} 

		private:

		/*_	@name	Private Attributes	
		*/
		//_@{

		/*_
		*/
		std::vector<QuadraticBondStretch::Data>	stretch_;

		/*_	The stretch parameters section
		*/
		QuadraticBondStretch  stretch_parameters_;

		//_@}
		
	};
} // namespace BALL

#endif // BALL_MOLMEC_AMBER_AMBERSTRETCH_H
