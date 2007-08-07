// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: conjugateGradient.h,v 1.29.20.6 2007/08/07 09:12:33 aleru Exp $ 
//

#ifndef BALL_MOLMEC_MINIMIZATION_CONJUGATEGRADIENT_H 
#define BALL_MOLMEC_MINIMIZATION_CONJUGATEGRADIENT_H 

#ifndef	BALL_MOLMEC_MINIMIZATION_ENERGYMINIMIZER_H
#	include <BALL/MOLMEC/MINIMIZATION/energyMinimizer.h>
#endif

#ifndef BALL_MOLMEC_MINIMIZATION_LINESEARCH_H
# include <BALL/MOLMEC/MINIMIZATION/lineSearch.h>
#endif

namespace BALL
{
	/** A minimizer for geometry optimization based on different
	 *  conjugate gradient (CG) algorithms. See
	 *
	 *  [1] Fletcher, R. and Reeves, C.M. "Function Minimization by 
	 *  Conjugate Gradients" Comp. J., 7, 149-154, 1964
	 *
	 *  [2] David Shanno "Conjugate Gradient Methods With Inexact Searches"
	 *  Mathematics of Operations Research, Vol. 3, No. 3, August 1978, pp. 244-256
	 *
	 *  [3] S. Watowich et. al "A Stable Rapidly Converging
	 *  Conjugate Gradient Method for Energy Minimization" Journal of Computational 
	 *  Chemistry, Vol. 9, No. 6, pp. 650-661 (1988)
	 *
	 *  [4] M.J.D. Powell "Convergence properties of algorithms for nonlinear
	 *  optimization" Report No. DAMPT 1985/NA1, University of Cambridge, 1985
	 *
	 *  [5] D.F. Shanno "Globally convergent conjugate gradient algorithms"
	 *  Mathematical Programming 33 (1985), pp. 61-67
	 *  \ingroup  MolmecEnergyMinimizer
	 */
	class BALL_EXPORT ConjugateGradientMinimizer 
		: public EnergyMinimizer
	{
		public:
			
			/** @name Enums
			*/
			//@{
			
			/** The different conjugate gradient methods implemented.
			 *  @see updateDirection for details on the implementation and references
			 */
			enum UpdateMethod
			{
				/** Polak-Ribiere method
				*/
				POLAK_RIBIERE = 1,
				
				/** Fletcher-Reeves method
				*/
				FLETCHER_REEVES = 2,
				
				/** Shanno
				*/
				SHANNO = 3
			};
			
			//@}
			/** @name Options and Defaults
			 */
			//@{
			
			/** Options names
			*/
			struct Option
			{
				/** The initial step length used in the line search 
				*/
				static const char* UPDATE_METHOD; 
			};
			
			/**	Defaults for all options
			*/
			struct BALL_EXPORT Default
			{
				/** The initial step length used in the line search 
				*/
				static const Size UPDATE_METHOD;
			};
			
			//@}
			/** @name Constructors and Destructors	
			*/
			//@{
			
			BALL_CREATE(ConjugateGradientMinimizer)
			
			/** Default constructor.
			*/
			ConjugateGradientMinimizer();
			
			/** Constructor expecting a valid force field
			*/
			ConjugateGradientMinimizer(ForceField& force_field);
			
			/** Constructor expecting a valid force field and a snapshot manager
			*/
			ConjugateGradientMinimizer(ForceField& force_field,SnapShotManager *ssm);
			
			/** Constructor expecting a valid force field, a snapshot manager and options
			*/
			ConjugateGradientMinimizer(ForceField& force_field, 
																 SnapShotManager* ssm, const Options& options);
			
			/** Constructor expecting a valid force field and options 
			*/
			ConjugateGradientMinimizer(ForceField& force_field, const Options& options);
			
			/** Copy constructor
			*/
			ConjugateGradientMinimizer(const ConjugateGradientMinimizer& rhs);
			
			/** Destructor.
			*/
			virtual ~ConjugateGradientMinimizer();
			
			//@}
			/** @name Assignments
			*/
			//@{
			
			/** Assignment operator
			*/
			const ConjugateGradientMinimizer& operator = (const ConjugateGradientMinimizer& rhs);
			
			//@}
			/** @name Setup methods. They do all necessary preparations.
			*/
			//@{
			
			/** Specific setup
			*/
			virtual bool specificSetup();
			
			//@}
			/** @name Accessors
			*/
			//@{
			
			/** Set explicitly the criterion used for updateDirection. If the
			 *  method is not recognized, this function fails.
			 */
			void setUpdateMethod(UpdateMethod method);
			
			/** Returns the current method for updateDirection.
			 */
			UpdateMethod getUpdateMethod() const;
			
			/** Calculate the next step.
			 *  First, this method updates the model. Second, it performs a line search
			 *  along the calculated direction afterwards.
			 *  @return double \f$\geq 0\f$ if the line search found an acceptable solution, otherwise -1.
			 *  @see EnergyMinimizer::findStep
			 */
			virtual double findStep();
			
			/** Update the search direction.
			 *  This method updates the search direction.
			 *  It uses the different conjugate gradient caculations
			 *  dependend on the options.
			 */
			virtual void updateDirection();
			
			/** Minimize the energy of the system.
			 *  This method executes at most <tt>iterations</tt> minimization steps.
			 *  If the number of iterations is not given, the number specified in the
			 *  options is taken.
			 *  @param iterations the maximum number of iterations
			 *  @param resume <b>true</b> to resume a previous run
			 *  @see EnergyMinimizer::minimize
			 */
			virtual bool minimize(Size iterations = 0, bool resume = false); 
			
		protected:
		
			//@}
			/** @name Protected Attributes
			*/
			//@{
			
			/** The line search
			*/
			LineSearch line_search_;
			
			/** Unscaled last search direction
			*/
			Gradient unscaled_direction_;
			
			/** Number of movable atoms.
			*/
			Size number_of_atoms_;
			
			/** Update method used for the CG
			*/
			Size updt_method_;
			
			/** Is this the (initial) first iteration?
			*/
			bool first_iter_;
			
			/** \f$g^T \cdot g\f$ where \f$g\f$ is the gradient of the last iteration
			*/
			double old_gtg_;
			
			/** Variable needed for the Shanno direction calculation
			*/
			vector<Vector3> a_i_;
			
			/** Variable needed for the Shanno direction calculation
			 */
			vector<Vector3> b_i_;
			 
			/** Variable needed for the Shanno direction calculation
				*/
			vector<Vector3> p_t_;
			 
			 /** Variable needed for the Shanno direction calculation
				*/
			vector<Vector3> y_t_;
			 
			/** Variable needed for the Shanno direction calculation
				*/
			vector<Vector3> p_i_;
								 
			/** Variable needed for the Shanno direction calculation
				*/
			vector<Vector3> y_i_;
			
			/** Variable needed for the Shanno direction calculation
			 */
			double D_1_;
			
			/** Variable needed for the Shanno direction calculation
			 */
			double D_4_;
			
			/** Frequency for restarts.
			*/
			Size restart_frequency_;
			
			/** We count the iterations since the last restart.
			 *  There are other necessities for a restart so the
			 *  iteration counter may not coincide (by modulo) with the
			 *  restart frequency.
			 */
			Size last_restart_iter_;
			
			//@}
		
	};
} // end of namespace BALL

#endif // BALL_MOLMEC_MINIMIZATION_CONJUGATEGRADIENT_H 
