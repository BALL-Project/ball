// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Molecular Mechanics: Amber force field, bond stretch component

#ifndef BALL_MOLMEC_AMBER_AMBERBEND_H
#define BALL_MOLMEC_AMBER_AMBERBEND_H

#ifndef BALL_COMMON_H
# include <BALL/common.h>
#endif

#ifndef BALL_MOLMEC_COMMON_FORCEFIELDCOMPONENT_H
# include <BALL/MOLMEC/COMMON/bendComponent.h>
#endif

namespace BALL 
{
	/**	Amber bond stretch component
			
    	\ingroup  AMBER
	*/
	class BALL_EXPORT AmberBend : public BendComponent
	{
		public:

		BALL_CREATE(AmberBend)

		/// flag to enable ES
		#define AMBER_BEND_ENABLED "enable Bends"

		/** @name	Constructors and Destructors	
		*/
		//@{ 

		/**	Default constructor.
		*/
		AmberBend();

		/**	Constructor.
		*/
		AmberBend(ForceField& force_field);

		/**	Destructor.
		*/
		virtual ~AmberBend();

		//@}

		/**	@name	Setup Methods	
		*/
		//@{

		/**	Setup method.
		*/
		virtual bool setup()
			throw(Exception::TooManyErrors);

		//@}
	};
} // namespace BALL

#endif // BALL_MOLMEC_AMBER_AMBERBEND_H
