// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Molecular Mechanics: Charmm force field, bond stretch component

#ifndef BALL_MOLMEC_CHARMM_CHARMMBEND_H
#define BALL_MOLMEC_CHARMM_CHARMMBEND_H

#ifndef BALL_COMMON_H
# include <BALL/common.h>
#endif

#ifndef BALL_MOLMEC_COMMON_BENDCOMPONENT_H
# include <BALL/MOLMEC/COMMON/bendComponent.h>
#endif

namespace BALL 
{
	/**	Charmm bond stretch component
			
    	\ingroup  CHARMM
	*/
	class BALL_EXPORT CharmmBend : public BendComponent
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
	};
} // namespace BALL

#endif // BALL_MOLMEC_CHARMM_CHARMMBEND_H
