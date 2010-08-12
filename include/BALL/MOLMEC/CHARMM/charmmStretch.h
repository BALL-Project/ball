// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Molecular Mechanics: Charmm force field, bond stretch component

#ifndef BALL_MOLMEC_CHARMM_CHARMMSTRETCH_H
#define BALL_MOLMEC_CHARMM_CHARMMSTRETCH_H

#ifndef BALL_COMMON_H
# include <BALL/common.h>
#endif

#ifndef BALL_MOLMEC_COMMON_STRETCHCOMPONENT_H
# include <BALL/MOLMEC/COMMON/stretchComponent.h>
#endif

namespace BALL
{
	/**	Charmm bond stretch component
			
    	\ingroup  CHARMM
	*/
	class BALL_EXPORT CharmmStretch : public StretchComponent
	{
		public:

		/// flag to enable Stretches
		#define CHARMM_STRETCHES_ENABLED "enable Stretches"

		/**	@name	Constructors and Destructors	
		*/
		//@{

		BALL_CREATE(CharmmStretch)

		/**	Default constructor.
		*/
		CharmmStretch();

		/**	Constructor.
		*/
		CharmmStretch(ForceField& force_field);

		/**	Destructor.
		*/
		virtual ~CharmmStretch();

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

#endif // BALL_MOLMEC_CHARMM_CHARMMSTRETCH_H
