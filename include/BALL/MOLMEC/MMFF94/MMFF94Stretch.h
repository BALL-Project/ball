// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MMFF94Stretch.h,v 1.1.2.1 2005/03/17 13:48:49 amoll Exp $ 
//

// Molecular Mechanics: MMFF94 force field, bond stretch component

#ifndef BALL_MOLMEC_MMFF94_MMFF94STRETCH_H
#define BALL_MOLMEC_MMFF94_MMFF94STRETCH_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_MOLMEC_COMMON_FORCEFIELDCOMPONENT_H
#	include <BALL/MOLMEC/COMMON/forceFieldComponent.h>
#endif

#ifndef BALL_MOLMEC_COMMON_FORCEFIELD_H
#	include <BALL/MOLMEC/COMMON/forceField.h>
#endif

namespace BALL 
{
	/**	MMFF94 bond stretch component
			
    	\ingroup  MMFF94
	*/
	class MMFF94Stretch 
		: public ForceFieldComponent
	{
		public:

		/**	@name	Constructors and Destructors	
		*/
		//@{

		BALL_CREATE(MMFF94Stretch)

		/**	Default constructor.
		*/
		MMFF94Stretch();

		/**	Constructor.
		*/
		MMFF94Stretch(ForceField& force_field);

		/**	Copy constructor
		*/
		MMFF94Stretch(const MMFF94Stretch& MMFF94_stretch);

		/**	Destructor.
		*/
		virtual ~MMFF94Stretch();

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

		//_@}
		
	};
} // namespace BALL

#endif // BALL_MOLMEC_MMFF94_MMFF94STRETCH_H
