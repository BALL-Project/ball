// $Id: amberBend.h,v 1.2 1999/09/21 11:06:54 oliver Exp $
// Molecular Mechanics: Amber force field, bond stretch component

#ifndef BALL_MOLMEC_AMBER_AMBERBEND_H
#define BALL_MOLMEC_AMBER_AMBERBEND_H

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

	/**	Amber bond stretch component
	*/
	class AmberBend 
		: public ForceFieldComponent
	{
		public:

		/**	@name	Enums
		*/
		//@{

		/**	Error codes of the AmberBend class
		*/
		enum ErrorCode 
		{
			ERROR__UNKNOWN = 0,

			NUMBER_OF_ERRORS
		};
		//@}

		/**@name	Constructors and Destructors	*/
		//@{ 

		/**	Default constructor.
		*/
		AmberBend(void);

		/**	constructor.
		*/
		AmberBend(ForceField* force_field);

		/**	Copy constructor
		*/
		AmberBend(const AmberBend& amber_stretch, bool clone_deep = true);

		/**	Destructor.
		*/
		virtual ~AmberBend(void);

		//@}


		/**	@name	Setup Methods	*/
		//@{

		/**	Setup method.
		*/
		virtual bool setup(void);

		//@}


		/**	@name	Accessors	*/
		//@{

		/**	Calculates and returns the component's energy.
		*/
		virtual float updateEnergy(void);

		/**	Calculates and returns the component's forces.
		*/
		virtual void updateForces(void);

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

#endif // BALL_MOLMEC_AMBER_AMBERBEND_H
