// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: shiftedLVMM.h,v 1.1.2.2 2006/10/17 14:54:57 aleru Exp $ 
//

#ifndef BALL_MOLMEC_MINIMIZATION_SHIFTEDLVMM_H 
#define BALL_MOLMEC_MINIMIZATION_SHIFTEDLVMM_H 

#ifndef	BALL_MOLMEC_MINIMIZATION_ENERGYMINIMIZER_H
#	include <BALL/MOLMEC/MINIMIZATION/energyMinimizer.h>
#endif

namespace BALL 
{ 
	/**	Shifted limited-memory variable metric methods minimizer.
			A minimizer for geometry optimization based upon the shifted
			limited-memory variable metric methods proposed in
			
			[1] Vlcek J., Luksan L.. New variable metric methods for unconstrained
			minimization covering the large-scale case. Report V-876, Prague,
			ICS AS CR, 2002.
	
			[2] Vlcek J., Luksan L.. Additional properties of shifted variable metric
			methods. Report V-899, Prague, ICS AS CR, 2004.
	
			[3] Vlcek J., Luksan L.. Shifted limited-memory variable metric methods
			for large-scale unconstrained optimization. Journal of Computational
			and Applied Mathematics 186 (2), pp. 365-390.
	\ingroup  MolmecEnergyMinimizer
	 */
  class BALL_EXPORT ShiftedLVMMMinimizer 
		: public EnergyMinimizer
	{
    public:
			
		/**	@name	Enums	
		 */
		//@{

		/**	The different variationally derived methods implemented.
		*/
		enum UpdateMethod
		{
			/**	Rank 1 variationally derived method.
			*/
			RANK_1,

			/**	Rank 2 variationally derived method.
			*/
			RANK_2
		};
		
		/**	The different correction parameters implemented.
		*/
		enum CorrectionParameter
		{
			/**	Unit value, i.e. rho = 1.0.
			*/
			UNIT_VALUE,

			/**	Balancing value.
			*/
			BALANCING_VALUE,
			
			/**	Square root.
			*/
			SQUARE_ROOT,
			
			/**	Geometric mean.
			*/
			GEOMETRIC_MEAN,
			
			/**	Ratio of shift parameters.
			*/
			RATIO_OF_SHIFT_PARAMS
		};

    /**	@name	Constructors and Destructors	
    */
    //@{

		BALL_CREATE(ShiftedLVMMMinimizer)

    /**	Default constructor.
    */
		ShiftedLVMMMinimizer();

    /**	Constructor expecting a valid force field 
    */
		ShiftedLVMMMinimizer(ForceField& force_field);

    /**	Constructor expecting a valid force field and a snapshot manager
    */
		ShiftedLVMMMinimizer(ForceField& force_field, SnapShotManager *ssm);

    /**	Constructor expecting a valid force field, a snapshot manager and options 
    */
		ShiftedLVMMMinimizer(ForceField& force_field, SnapShotManager* ssm, const Options& options);

    /**	Constructor expecting a valid force field and options 
    */
		ShiftedLVMMMinimizer(ForceField& force_field, const Options& options);

    /**	Copy constructor
    */
		ShiftedLVMMMinimizer(const ShiftedLVMMMinimizer& rhs);

    /**	Destructor.
    */
		virtual ~ShiftedLVMMMinimizer();

    //@}
    /**	@name	Assignments 
    */
    //@{

    /**	Assignment operator
    */
		const ShiftedLVMMMinimizer& operator = (const ShiftedLVMMMinimizer& rhs);

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
		
		/** Set explicitly the number of columns of the factor of the shifted 
				inverse hessian approximation. Default is 5.
		*/
		void setNumOfColumns(Size num);
							
		/** Returns the maximum number of columns of the factor of the shifted 
				inverse hessian approximation.
		*/
		Size getNumOfColumns() const;
		
		/** Set explicitly the update method for the shifted inverse hessian
				approximation. Default is RANK_2.
		*/
		void setUpdateMethod(UpdateMethod rank);
		
		/** Returns the update method for the shifted inverse hessian approximation.
		*/
		UpdateMethod setUpdateMethod() const;
		
		/** Set explicitly the type of the correction parameter. 
				Default is RATIO_OF_SHIFT_PARAMS.
		*/
		void setCorrectionParameter(CorrectionParameter par);
		
		/** Returns the type of the correction parameter. 
		*/
		CorrectionParameter getCorrectionParameter() const;
		
									
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
		
		/*_ The maximum number of columns of the 
				factor of the shifted inverse hessian approximation.
		*/
		Size max_number_of_cols_;
		
		/*_ The current number of columns of the 
				factor of the shifted inverse hessian approximation.
		*/
		Size curr_number_of_cols_;
		
		/*_ Specifies the update method. There are two update methods.
				A rank 1 correction method and a rank 2 correction method.
		*/
		UpdateMethod updt_method_;
		
		/*_ Specifies the type of the correction parameter.
		*/
		CorrectionParameter corr_par_;
		
		/*_ The shift value of the previous iteration.
		*/
		double prev_shift_val_;
		
		/*_ The shifted previous step.
		*/
		vector<Vector3> shift_s_;
		
		/*_ The current gradient difference.
		*/
		vector<Vector3> grad_diff_;
		
		/*_ The needed vectors for the update.
		*/
		vector<float> updt_u_;
		vector<float> updt_v_;
		
		/*_ The direction of the shifted step, i.e. -U_k*U_k^t*gk.
		*/
		vector<Vector3> shifted_direction_;
		
		/*_ The factor of the shifted inverse hessian approximation
				in column order, U_k in [3].
		*/
		vector<Vector3> hess_factor_;
		
		// AR: TODO: avoid the need of 'initial_atoms_' by some kind of "a direct
		// usage" of an 'AtomVector'. I need a direct access to 'saved_position_'.
		
		// The positions of the movable atoms when we start a step computation.
		vector<Vector3> initial_atoms_;
		
		// The last step size (in respect to the length of the computed direction vector),
		// so the length of the last step was step_*||direction_||.
		double step_;
		
		// Numerical lower bound: we don't want to compute the reciprocal of a number 
		// which is lower than 'cutlo_'.
		float cutlo_;
		
    //@}

	};
} // end of namespace BALL

#endif // BALL_MOLMEC_MINIMIZATION_SHIFTEDLVMM_H 
