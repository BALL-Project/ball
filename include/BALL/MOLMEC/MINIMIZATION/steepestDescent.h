// $Id: steepestDescent.h,v 1.7 2000/08/30 19:58:21 oliver Exp $
// Line Search Minimizer: A special class for the line search minimization algorithm

#ifndef BALL_MOLMEC_MINIMIZATION_STEEPESTDESCENT_H
#define BALL_MOLMEC_MINIMIZATION_STEEPESTDESCENT_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_SYSTEM_H
#	include <BALL/KERNEL/system.h>
#endif

#ifndef BALL_DATATYPE_OPTIONS_H
#	include <BALL/DATATYPE/options.h>
#endif

#ifndef BALL_MOLMEC_PARAMETER_FORCEFIELDPARAMETERS_H
#	include <BALL/MOLMEC/PARAMETER/forceFieldParameters.h>
#endif

#ifndef BALL_MOLMEC_PARAMETER_ATOMTYPES_H
#	include <BALL/MOLMEC/PARAMETER/atomTypes.h>
#endif

#ifndef BALL_MOLMEC_COMMON_PERIODIC_BOUNDARY_H
#	include <BALL/MOLMEC/COMMON/periodicBoundary.h>
#endif

#ifndef BALL_MOLMEC_COMMON_FORCEFIELD_H
#	include <BALL/MOLMEC/COMMON/forceField.h>
#endif

#ifndef BALL_MOLMEC_MINIMIZATION_ENERGYMINIMIZER_H
#       include <BALL/MOLMEC/MINIMIZATION/energyMinimizer.h>
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

		/**     Options names
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
		
		BALL_CREATE_DEEP(SteepestDescentMinimizer)

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
		SteepestDescentMinimizer(ForceField& force_field, SnapShotManager *ssm,
                                                     const Options& options);

		/**	Copy constructor
		*/
		SteepestDescentMinimizer(const SteepestDescentMinimizer& line_search_minimizer, bool deep = true);

		/**	Destructor.
		*/
		virtual ~SteepestDescentMinimizer();

		//@}


		/**	@name	Assignments 
		*/
		//@{

		/**	Assignment operator
		*/
		SteepestDescentMinimizer&	operator = (const SteepestDescentMinimizer& SteepestDescentMinimizer);

		//@}

		/**	@name	Setup methods 
		*/
		//@{

		/**	Specific setup
		*/
		virtual bool specificSetup();

		//@}


		/**	@name	Accessors 
		*/
		//@{

		/**	Set the maximal number of steps in an iteration
		*/
		void	setMaxSteps(Size max_steps);

		/**	Get the maximal number of steps in an iteration
		*/
		Size	getMaxSteps() const;

		/**	Set the maximum RMS gradient (convergence criterion).
				The gradient unit of the gradient is {\bf kJ/(mol \AA)}.
		*/
		void	setMaxGradient(float max_gradient);

		/**	Get the maximum RMS gradient (convergence criterion).
				The gradient unit of the gradient is {\bf kJ/(mol \AA)}.
		*/
		float	getMaxGradient() const;

		/**	Minimize the energy of the system using a greedy steepest descent.
		*/
		virtual bool	minimize(Size steps = 0, bool restart = false);

		//@}


		/**	@name	Public Attributes
		*/
		//@{
		
		/**	Options Force field options
		*/
		Options	options;

		//@}

		protected:

		/*_	@name	Protected Attributes */
		//_@{

		/*_	The maximal number of steps in an iteration step
		*/
		Size	max_steps_; 

		//_@}

	};

} // namespace BALL



#endif // BALL_MOLMEC_MINIMIZATION_STEEPESTDESCENT_H
