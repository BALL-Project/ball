// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: strangLBFGS.h,v 1.1.4.6 2007/08/07 09:12:36 aleru Exp $ 
//

#ifndef BALL_MOLMEC_MINIMIZATION_STRANGLBFGS_H 
#define BALL_MOLMEC_MINIMIZATION_STRANGLBFGS_H 

#ifndef	BALL_MOLMEC_MINIMIZATION_ENERGYMINIMIZER_H
#	include <BALL/MOLMEC/MINIMIZATION/energyMinimizer.h>
#endif

#ifndef BALL_MOLMEC_MINIMIZATION_LINESEARCH_H
# include <BALL/MOLMEC/MINIMIZATION/lineSearch.h>
#endif

namespace BALL 
{
	/** Limited-memory BFGS minimizer based on the Strang recurrence.
	 *  A minimizer for geometry optimization based on the limited-memory
	 *  BFGS method proposed in
	 *
	 *  [1] Nocedal J.: Updating quasi-Newton matrices with limited storage.
	 *  Math. Comp. 35 (1980) 773-782.
	 *
	 *  [2] Liu D.C., Nocedal J.: On the limited memory BFGS method for large
	 *  scale optimization. Math. Programming 45 (1989) 503-528.
	 *
	 *  If the 'improved' option is set, ideas proposed in
	 *
	 *  [3] Mehiddin Al-Baali: Improved Hessian approximations for the limited 
	 *  memory BFGS method. Numerical Algorithms 22 (1999) 99-112
	 *
	 *  are adopted.
	 * \ingroup  MolmecEnergyMinimizer
	 */
	class BALL_EXPORT StrangLBFGSMinimizer 
		: public EnergyMinimizer
	{
		public:
		
		BALL_CREATE(StrangLBFGSMinimizer)
		
			/** @name Options and Defaults
			 */
			//@{
			
			/** Symbolic names for option keys.
			 *  This struct contains a symbolic name
			 *  for each recognized key in StrangLBFGSMinimizer::options. \par
			 *  For each symbol the required type is given under parameters.
			 */
			struct Option 
			{
				/** Should the improved version be used?
				 */
				static const char* IMPROVED;
				
				/** Maximum number of stored vector pairs
				 */
				static const char* NUM_OF_STORED_VECT_PAIRS;
			};
			
			struct BALL_EXPORT Default
			{
				/** Should the improved version be used?
				 *  Default is false.
				 */
				static const bool IMPROVED;
				
				/** Maximum number of stored vector pairs.
				 *  Default is 5.
				 */
				static const Size NUM_OF_STORED_VECT_PAIRS;
			};
			
			//@}
			/**	@name	Constructors and Destructors	
			 */
			//@{
			
			/** Default constructor.
			 */
			StrangLBFGSMinimizer();
			
			/** Constructor expecting a valid force field
			*/
			StrangLBFGSMinimizer(ForceField& force_field);
			
			/** Constructor expecting a valid force field and a snapshot manager
			*/
			StrangLBFGSMinimizer(ForceField& force_field, SnapShotManager *ssm);
			
			/** Constructor expecting a valid force field, a snapshot manager and options
			*/
			StrangLBFGSMinimizer(ForceField& force_field, SnapShotManager* ssm, const Options& options);
			
			/** Constructor expecting a valid force field and options
			*/
			StrangLBFGSMinimizer(ForceField& force_field, const Options& options);
			
			/** Copy constructor
			*/
			StrangLBFGSMinimizer(const StrangLBFGSMinimizer& rhs);
			
			/** Destructor.
			*/
			virtual ~StrangLBFGSMinimizer();
			
			//@}
			/**	@name	Assignments 
			*/
			//@{
			
			/** Assignment operator
			*/
			const StrangLBFGSMinimizer& operator=(const StrangLBFGSMinimizer& rhs);
			
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
			
			/** Set the maximum number of stored vector pairs
			 */
			void setMaxNumOfStoredVectPairs(Size num);
			
			/** Return the maximum number of stored vector pairs
			 */
			Size getMaxNumOfStoredVectPairs() const;
			
			/** Set the improved flag
			 */
			void setImprovedFlag(bool flag);
			
			/** Return the improved flag
			 */
			bool getImprovedFlag() const;
			
			/** Calculate the next step.
			 *  First, this method updates the model. Second, it performs a line search
			 *  along the calculated direction afterwards.
			 *  @return double \f$\geq 0\f$ if the line search found an acceptable solution, otherwise -1.
			 *  @see EnergyMinimizer::findStep
			 */
			virtual double findStep();
			
			/** Update the stored vector pairs, either by adding a new pair or if there
			 *  is no space left by replacing the oldest pair.
			 *  Compute the search direction afterwards by using the Strang recurrence formula
			 *  (optional: with an improvement idea of Al-Baali).
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
			
			/** Is this an initial first iteration?
			*/
			bool first_iter_;
			
			/** Number of movable atoms.
			*/
			Size number_of_atoms_;
			
			/** Maximum number of stored vector pairs.
			*/
			Size max_num_of_vect_pairs_;
			
			/** Current number of stored vector pairs.
			*/
			Size curr_num_of_vect_pairs_;
			
			/** Should the improved version be used?
			*/
			bool improved_;
			
			/** Old and new scaling values. Also used in the Strang recurrence formula.
			*/
			vector<float> rho_;
			
			/** Stored former steps. Vectors stored in column order.
			*/
			vector<Vector3> stored_s_;
			
			/** Stored former changes in gradients.
			 *  Vectors stored in column order.
			 */
			vector<Vector3> stored_y_;
			
			/** Temporarily used memory for saving scalars associated with
			 *  the stored vector pairs.
			 */
			vector<float> work_val_;
			
			/** Index of the vector pair which will be used for saving the data
			 *  of the current step (usually by replacing old data).
			 */
			Size index_of_free_vect_;
			
			/** Positions of movable atoms when starting an iteration.
			 *  This is used to reduce slightly rounding errors
			 */
			std::vector<Vector3> initial_atoms_;
			
			//@}
			
	};
} // end of namespace BALL

#endif // BALL_MOLMEC_MINIMIZATION_STRANGLBFGS_H
