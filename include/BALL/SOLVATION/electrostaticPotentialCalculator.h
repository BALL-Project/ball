// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: electrostaticPotentialCalculator.h,v 1.10 2005/12/23 17:01:59 amoll Exp $
//

#ifndef BALL_SOLVATION_ELECTROSTATICPOTENTIALCALCULATOR_H
#define BALL_SOLVATION_ELECTROSTATICPOTENTIALCALCULATOR_H

#ifndef BALL_KERNEL_SYSTEM_H
 # include <BALL/KERNEL/system.h>
#endif

#ifndef BALL_DATATYPE_OPTIONS_H
 # include <BALL/DATATYPE/options.h>
#endif

#ifndef BALL_STRUCTURE_DEFAULTPROCESSORS_H
 # include <BALL/STRUCTURE/defaultProcessors.h>
#endif

namespace BALL
{
	class FragmentDB;

	/** This class is used to compute the electrostatic potential for
	    of a system. It is intended that several different models will
			be implemented. Right now, the only model is the nonlocal electrostatic
			potential of a sum of Born ions.
			Dont forget to set the FragmentDB before using this class.
      \ingroup Solvation
	 */
	class BALL_EXPORT ElectrostaticPotentialCalculator
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
			;

		/** Copy constructor
		 */
		ElectrostaticPotentialCalculator(const ElectrostaticPotentialCalculator& epc)
			;

		/** Destructor.
		 */
		virtual ~ElectrostaticPotentialCalculator()
			;

		//@}
		/** @name Assignment
		 */
		//@{
		
		/** Apply the current model to the System S and prepare everything for a calculation.
		 		@exception NullPointer if FragmentDB is not set
		 */
		void apply(System &S)
			throw(Exception::NullPointer);

		///
		void setFragmentDB(const FragmentDB* db)
			;

		///
		const FragmentDB* getFragmentDB() const
			;

		//@}
		/** @name Accessors
		 */
		//@{
		
		/** Returns the electrostatic potential at position pos.
		 */
		float operator() (const Vector3& pos)
			;

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
		protected:

		System mySys_;

		FragmentDB *frag_db_;

		AssignRadiusProcessor radii_;
		
		AssignChargeProcessor charges_;
		//_@}
	};
}


#endif // BALL_SOLVATION_ELECTROSTATICPOTENTIALCALCULATOR_H
