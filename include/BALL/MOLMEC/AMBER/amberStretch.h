// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Molecular Mechanics: Amber force field, bond stretch component

#ifndef BALL_MOLMEC_AMBER_AMBERSTRETCH_H
#define BALL_MOLMEC_AMBER_AMBERSTRETCH_H

#ifndef BALL_COMMON_H
# include <BALL/common.h>
#endif

#ifndef BALL_MOLMEC_COMMON_FORCEFIELDCOMPONENT_H
# include <BALL/MOLMEC/COMMON/stretchComponent.h>
#endif

namespace BALL
{
	/**	Amber bond stretch component
			
    	\ingroup  AMBER
	*/
	class BALL_EXPORT AmberStretch : public StretchComponent
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

		/**	Destructor.
		*/
		virtual ~AmberStretch();

		//@}
		/**	@name Setup Methods	
		*/
		//@{

		/**	Setup method.
		*/
		virtual bool setup() throw(Exception::TooManyErrors);

		//@}
	};
} // namespace BALL

#endif // BALL_MOLMEC_AMBER_AMBERSTRETCH_H
