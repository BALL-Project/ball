// $Id: lineSearch.h,v 1.3 2000/01/10 15:51:04 oliver Exp $
// Line Search Minimizer: A special class for the line search minimization algorithm

#ifndef BALL_MOLMEC_MINIMIZATION_LINESEARCH_H
#define BALL_MOLMEC_MINIMIZATION_LINESEARCH_H

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

#ifndef	BALL_MOLMEC_MINIMIZATION_ENERGYMINIMIZER_H
#	include <BALL/MOLMEC/MINIMIZATION/energyMinimizer.h>
#endif

/*
#ifndef BALL_MOLMEC_COMMON_SNAPSHOT_H
#	include <BALL/MOLMEC/COMMON/snapshot.h>
#endif
*/

namespace BALL 
{

	class LineSearchMinimizer 
		:	public EnergyMinimizer	
	{


		public:

		/**	@name	Enums
		*/
		//@{

		/**	Error codes of the ForceField class
		*/
		enum	ErrorCode 
		{
			ERROR__NONE	= -1,
			ERROR__UNKNOWN,
			ERROR__NOT_IMPLEMENTED,

			NUMBER_OF_ERRORS
		};

		//@}

		/**	@name	Options definitions
		*/
		//@{

		/**     Options names
		*/
		struct Option
		{
			/**    Constant alpha for the stop criterion 
			*/
			static const char* ALPHA;

			/**    Constant beta for the stop criterion 
			*/
			static const char* BETA;

			/**	Constant for the maximum of steps in an iteration
			*/
			static const char* MAX_STEPS;

			/**	String constant for the maximum RMS gradient needed for convergence
			*/
			static const char* MAX_GRADIENT;

    };

		struct Default
		{
			/**    Default value of the constant alpha for the stop criterion 
			*/
			static float ALPHA;

			/**    Default value of the constant beta for the stop criterion 
			*/
			static float BETA;

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
		
		BALL_CREATE(LineSearchMinimizer)

		/**	Default constructor.
		*/
		LineSearchMinimizer();

		/**	Constructor.
		*/
		LineSearchMinimizer(ForceField& force_field);

		/**	Constructor.
		*/
		LineSearchMinimizer(ForceField& force_field, const Options& options);

		/**	Copy constructor
		*/
		LineSearchMinimizer(const LineSearchMinimizer& line_search_minimizer, bool deep = true);

		/**	Destructor.
		*/
		virtual ~LineSearchMinimizer();

		//@}


		/**	@name	Assignments 
		*/
		//@{

		/**	Assignment operator
		*/
		LineSearchMinimizer&	operator = (const LineSearchMinimizer& LineSearchMinimizer);

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

		/**	Set the parameter alpha
		*/
		void	setAlpha(float alpha);

		/**	Get the parameter alpha
		*/
		float	getAlpha() const;

		/**	Set the parameter beta
		*/
		void	setBeta(float beta);

		/**	Get the parameter beta
		*/
		float	getBeta() const;

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

		/**	Set the direction vector for the line search.
		*/
		void setDirection(const vector<Vector3>& direction);

		/**	Minimize the energy of the system using the line search approach.
		*/
		virtual bool	minimize(Size steps, bool restart);

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

		/*_	The factor alpha in the sufficient decrease of the line search algorithm 
			f(x*) <= f(x_k) + alpha * g(x_k) * p_k
		*/
		float	alpha_;

		/*_	The factor alpha in the sufficient decrease of the line search algorithm 
			|<g(x_{k+1}), p_k>| <= beta |<g(x_k), p_k>|
		*/
		float	beta_;

		/*_	The maximal number of steps in an iteration step
		*/
		Size	max_steps_; 

		/*_	The maximum RMS gradient (covnergence criterion)
		*/
		float	max_gradient_;

		/*_	Direction of the line search.
		*/
		vector<Vector3>*	direction_;

		//_@}

	};

} // namespace BALL



#endif // BALL_MOLMEC_MINIMIZATION/LINESEARCH_H
