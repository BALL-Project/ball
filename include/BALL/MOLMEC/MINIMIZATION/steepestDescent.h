// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: steepestDescent.h,v 1.10 2003/02/03 21:38:18 oliver Exp $
// Line Search Minimizer: A special class for the line search minimization algorithm

#ifndef BALL_MOLMEC_MINIMIZATION_STEEPESTDESCENT_H
#define BALL_MOLMEC_MINIMIZATION_STEEPESTDESCENT_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_DATATYPE_OPTIONS_H
#	include <BALL/DATATYPE/options.h>
#endif

#ifndef BALL_MOLMEC_COMMON_FORCEFIELD_H
#	include <BALL/MOLMEC/COMMON/forceField.h>
#endif

#ifndef BALL_MOLMEC_MINIMIZATION_ENERGYMINIMIZER_H
# include <BALL/MOLMEC/MINIMIZATION/energyMinimizer.h>
#endif

#ifndef BALL_MOLMEC_COMMON_SNAPSHOT_H
#	include <BALL/MOLMEC/COMMON/snapShot.h>
#endif

namespace BALL 
{

	class SteepestDescentMinimizer 
		:	public EnergyMinimizer	
	{

		public:

		/**	@name	Options definitions
		*/
		//@{

		/**  Options names
		*/
		struct Option
		{
			/**	Constant for the maximum of steps in an iteration
			*/
			static const char* MAX_STEPS;

			/**	String constant for the maximum RMS gradient needed for convergence
			*/
			static const char* MAX_GRADIENT;

    };

		struct Default
		{
			/**	Default value for the maximum of steps in an iteration
			*/
			static Size MAX_STEPS;

			/**	Default value for the maximum RMS gradient (termination condition)
			*/
			static float MAX_GRADIENT;
		};

		//@}

		/**	@name	Constructors and Destructors	
		*/
		//@{
		
		BALL_CREATE(SteepestDescentMinimizer)

		/**	Default constructor.
		*/
		SteepestDescentMinimizer();

		/**	Constructor.
		*/
		SteepestDescentMinimizer(ForceField& force_field);

		/**	Constructor.
		*/
		SteepestDescentMinimizer(ForceField& force_field,SnapShotManager *ssm);

		/**	Constructor.
		*/
		SteepestDescentMinimizer(ForceField& force_field, const Options& options);

		/**	Constructor.
		*/
		SteepestDescentMinimizer(ForceField& force_field, SnapShotManager* ssm, const Options& options);

		/**	Copy constructor
		*/
		SteepestDescentMinimizer(const SteepestDescentMinimizer& minimizer);

		/**	Destructor.
		*/
		virtual ~SteepestDescentMinimizer();
		//@}

		/**	@name	Assignments 
		*/
		//@{

		/**	Assignment operator
		*/
		const SteepestDescentMinimizer&	operator = (const SteepestDescentMinimizer& minimizer);
		//@}

		/**	@name	Setup methods 
		*/
		//@{

		/**	Specific setup
		*/
		virtual bool specificSetup();
		//@}

		/**	Minimize the energy of the system using a greedy steepest descent.
		*/
		virtual bool	minimize(Size steps = 0, bool restart = false);

	};

} // namespace BALL

#endif // BALL_MOLMEC_MINIMIZATION_STEEPESTDESCENT_H
