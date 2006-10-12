// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: strangLBFGS.h,v 1.1.2.3 2006/10/12 10:19:32 aleru Exp $ 
//

#ifndef BALL_MOLMEC_MINIMIZATION_STRANGLBFGS_H 
#define BALL_MOLMEC_MINIMIZATION_STRANGLBFGS_H 

#ifndef	BALL_MOLMEC_MINIMIZATION_ENERGYMINIMIZER_H
#	include <BALL/MOLMEC/MINIMIZATION/energyMinimizer.h>
#endif

namespace BALL 
{ 
	/**	Improved limited-memory BFGS minimizer based on the Strang recurrence.
			A minimizer for geometry optimization based on the limited-memory
			BFGS method proposed in

			[1] Nocedal J.: Updating quasi-Newton matrices with limited storage.
			Math. Comp. 35 (1980) 773-782.

			[2] Liu D.C., Nocedal J.: On the limited memory BFGS method for large
			scale optimization. Math. Programming 45 (1989) 503-528.

			This is an improved version which adopts ideas proposed in
	
			[3] Mehiddin Al-Baali: Improved Hessian approximations for the limited 
			memory BFGS method. Numerical Algorithms 22 (1999) 99-112.
			
			\ingroup  MolmecEnergyMinimizer
	*/
  class BALL_EXPORT StrangLBFGSMinimizer 
		: public EnergyMinimizer
	{
    public:

    /**	@name	Constructors and Destructors	
    */
    //@{

		BALL_CREATE(StrangLBFGSMinimizer)

    /**	Default constructor.
    */
		StrangLBFGSMinimizer();

    /**	Constructor expecting a valid force field 
    */
		StrangLBFGSMinimizer(ForceField& force_field);

    /**	Constructor expecting a valid force field and a snapshot manager
    */
		StrangLBFGSMinimizer(ForceField& force_field, SnapShotManager *ssm);

    /**	Constructor expecting a valid force field, a snapshot manager and options 
    */
		StrangLBFGSMinimizer(ForceField& force_field, SnapShotManager* ssm, const Options& options);

    /**	Constructor expecting a valid force field and options 
    */
		StrangLBFGSMinimizer(ForceField& force_field, const Options& options);

    /**	Copy constructor
    */
		StrangLBFGSMinimizer(const StrangLBFGSMinimizer& rhs);

    /**	Destructor.
    */
		virtual ~StrangLBFGSMinimizer();

    //@}
    /**	@name	Assignments 
    */
    //@{

    /**	Assignment operator
    */
		const StrangLBFGSMinimizer& operator = (const StrangLBFGSMinimizer& rhs);

    //@}
    /**	@name	Setup methods. They do all necessary preparations. 
    */
    //@{

    /**	Specific setup
    */
    virtual bool specificSetup();

    //@}
    /**	@name	Accessors 
    */
    //@{
		
		/** Set explicitly the maximum number of stored vector pairs
				used in the Strang recurrence. Default is 5.
		*/
		void setNumOfStoredVects(Size num);
							
		/** Returns the maximum number of stored pairs of vectors.
		*/
		Size getNumOfStoredVects() const;
									
		/**	Calculate the next step.
				This method calculates direction and step length for the
				next optimization step. The step is not yet performed.
				This method first tries to determine a better solution 
				with a line search along the current search direction.
				If this fails, it resets the search direction to the negative 
				current gradient and performs a new line search.
				The best solution found is returned.
				@return	double <b>\geq 0.0</b> if the line search found an acceptable solution, otherwise -1.0.
				@see	EnergyMinimizer::findStep
		*/
		virtual double findStep();
			
		/**	Update the stored vector pairs, either by adding a new pair or if there 
				is no space left by replacing the oldest pair. 
				Compute the search direction afterwards by using the Strang recurrence formula
				with an improvement idea of Al-Baali.
		*/
		virtual void updateDirection();

    /**	Minimize the energy of the system.
				This method executes at most <tt>iterations</tt> minimization steps.
				If the number of iterations is not given, the number specified in the
				options is taken.
				@param	iterations the maximum number of iterations
				@param	resume <b>true</b> to resume a previous run
				@see		EnergyMinimizer::minimize
    */
    virtual bool minimize(Size iterations = 0, bool resume = false); 

    protected:

    //@}
    /**	@name	Protected Attributes 
		*/
    //@{

    /*_ The number of movable atoms.
    */
		Size number_of_atoms_;
		
		/*_ The maximum number of stored vector pairs.
		*/
		Size max_num_of_vect_pairs_;
		
		/*_ Current number of stored vector pairs.
		*/
		Size curr_num_of_vect_pairs_;
		
		// Old and new scaling values. Also used in the Strang recurrence formula.
		vector<float> rho_;
		
		// The stored former steps. We store the vectors in column order.
		vector<Vector3> stored_s_;
		
		// The stored former changes in gradients.
		// We store the vectors in column order.
		vector<Vector3> stored_y_;
		
		// AR: TODO: avoid the need of 'initial_atoms_' by some kind of "a direct
		// usage" of an 'AtomVector'. I need a direct access to 'saved_position_'.
		
		// The positions of the movable atoms when we start a step computation.
		vector<Vector3> initial_atoms_;
		
		// Temporarily used memory for saving scalars associated with
		// the stored vector pairs, small size i.e. max_num_of_vect_pairs_.
		vector<float> work_val_;

		// Index of the vector pair which will be used for saving the data
		// of the current step (usually by replacing the old data).
		Size index_of_free_vect_;
		
    //@}

	};
} // end of namespace BALL

#endif // BALL_MOLMEC_MINIMIZATION_STRANGLBFGS_H 
