// $Id: electrostaticPotentialCalculator.h,v 1.2 2002/12/12 09:48:54 oliver Exp $

#ifndef BALL_SOLVATION_ELECTROSTATICPOTENTIALCALCULATOR_H
#define BALL_SOLVATION_ELECTROSTATICPOTENTIALCALCULATOR_H

#ifndef BALL_COMMON_H
# include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_SYSTEM_H
# include <BALL/KERNEL/system.h>
#endif

#ifndef BALL_DATATYPE_OPTIONS_H
# include <BALL/DATATYPE/options.h>
#endif

#ifndef BALL_STRUCTURE_FRAGMENTDB_H
# include <BALL/STRUCTURE/fragmentDB.h>
#endif

#ifndef BALL_STRUCTURE_DEFAULTPROCESSORS_H
# include <BALL/STRUCTURE/defaultProcessors.h>
#endif

#ifndef BALL_MOLMEC_COMMON_RADIUSRULEPROCESSOR_H
# include <BALL/MOLMEC/COMMON/radiusRuleProcessor.h>
#endif

namespace BALL
{
	/** This class is used to compute the electrostatic potential for
	    of a system. It is intended that several different models will
			be implemented. Right now, the only model is the nonlocal electrostatic
			potential of a sum of Born ions.
	 */

	class ElectrostaticPotentialCalculator
	{
		BALL_CREATE(ElectrostaticPotentialCalculator)

		public:	

		/** Symbolic names for option keys.
		 		This struct contains a symbolic name for each recognized key
				in ElectrostaticPotentialCalculator::options.
		 */
		struct Option
		{
			/** The method to use for the calculation of the electrostatic potential.
			 */
			static const char* LOCALITY;
		};

		/** Default values for the options in ElectrostaticPotentialCalculator::options
		 */
		struct Default
		{
			/** Default calculation method.
			 		The default is to use a local field.
			 */
			static const bool LOCALITY;
		};
			 		
		/** @name Constructors and Destructors
		 */
		//@{
		
		/** Default constructor
		 */
		ElectrostaticPotentialCalculator()
			throw();

		/** Copy constructor
		 */
		ElectrostaticPotentialCalculator(const ElectrostaticPotentialCalculator& epc)
			throw();

		/** Destructor.
		 */
		~ElectrostaticPotentialCalculator()
			throw();
		//@}
		
		/** @name Assignment
		 */
		//@{
		
		/** Apply the current model to the System S and prepare everything for a calculation.
		 */
		void Apply(System &S)
			throw();

		//@}
		
		/** @name Accessors
		 */
		//@{
		
		/** Returns the electrostatic potential at position pos.
		 */
		float operator() (const Vector3& pos)
			throw();
		//@}

		/** @name Options
		 */
		//@{

		/** Options for the calculation of the electrostatic potential.
		 */
		Options options;

		//@}
		
		/** @name Protected Attributes.
		 */
		//_@{

		System mySys_;

		FragmentDB *frag_db_;

		AssignRadiusProcessor radii_;
		
		AssignChargeProcessor charges_;
		//_@}
	};

}


#endif // BALL_SOLVATION_ELECTROSTATICPOTENTIALCALCULATOR_H
