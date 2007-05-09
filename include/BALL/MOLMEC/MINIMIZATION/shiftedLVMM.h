// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: shiftedLVMM.h,v 1.1.4.3 2007/05/09 16:43:26 aleru Exp $ 
//

#ifndef BALL_MOLMEC_MINIMIZATION_SHIFTEDLVMM_H 
#define BALL_MOLMEC_MINIMIZATION_SHIFTEDLVMM_H 

#ifndef	BALL_MOLMEC_MINIMIZATION_ENERGYMINIMIZER_H
#	include <BALL/MOLMEC/MINIMIZATION/energyMinimizer.h>
#endif

#ifndef BALL_MOLMEC_MINIMIZATION_LINESEARCH_H
# include <BALL/MOLMEC/MINIMIZATION/lineSearch.h>
#endif

namespace BALL 
{
	/** Shifted limited-memory variable metric methods minimizer.
	 *  A minimizer for geometry optimization based upon the shifted
	 *  limited-memory variable metric methods proposed in
	 *
	 *  [1] Vlcek J., Luksan L.. New variable metric methods for unconstrained
	 *  minimization covering the large-scale case. Report V-876, Prague,
	 *  ICS AS CR, 2002.
	 *
	 *  [2] Vlcek J., Luksan L.. Additional properties of shifted variable metric
	 *  methods. Report V-899, Prague, ICS AS CR, 2004.
	 *
	 *  [3] Vlcek J., Luksan L.. Shifted limited-memory variable metric methods
	 *  for large-scale unconstrained optimization. Journal of Computational
	 *  and Applied Mathematics 186 (2), pp. 365-390.
	 *  \ingroup  MolmecEnergyMinimizer
	 */
	class BALL_EXPORT ShiftedLVMMMinimizer 
		: public EnergyMinimizer
	{
		public:
			
			BALL_CREATE(ShiftedLVMMMinimizer)
			
			/** @name Options and Defaults
			 */
			//@{
			
			/** Symbolic names for option keys.
			 *  This struct contains a symbolic name
			 *  for each recognized key in ShiftedLVMMMinimizer::options. \par
			 *  For each symbol the required type is given under parameters.
			 */
			typedef struct Option
			{
				/** Which update method should be used?
				 */
				static const char* UPDATE_METHOD;
			
				/** Which correction parameter should be used?
				 */
				static const char* CORRECTION_PARAMETER;
				
				/** The number of columns of the factor of the shifted
				 *  inverse hessian approximation.
				 */
				static const char* NUM_OF_COLUMNS;
			};
			
			struct BALL_EXPORT Default
			{
				/** Which update method should be used?
				 *  Default is RANK_2.
				 */
				static const Size UPDATE_METHOD;
			
				/** Which correction parameter should be used?
				 *  Default is RATIO_OF_SHIFT_PARAMS.
				 */
				static const Size CORRECTION_PARAMETER;
				
				/** The number of columns of the factor of the shifted
				 *  inverse hessian approximation. Default is 5.
				 */
				static const Size NUM_OF_COLUMNS;
			};
		
			//@}
			/** @name Enums
			 */
			//@{
			
			/** The different variationally derived methods implemented.
			*/
			enum UpdateMethod
			{
				/** Rank 1 variationally derived method.
				*/
				RANK_1 = 1,
				
				/** Rank 2 variationally derived method.
				*/
				RANK_2 = 2
			};
		
			/** The different correction parameters implemented.
			*/
			enum CorrectionParameter
			{
				/** Unit value, i.e. rho = 1.0.
				*/
				UNIT_VALUE = 1,
			
				/** Balancing value.
				*/
				BALANCING_VALUE = 2,
				
				/**	Square root.
				*/
				SQUARE_ROOT = 3,
				
				/**	Geometric mean.
				*/
				GEOMETRIC_MEAN = 4,
				
				/**	Ratio of shift parameters.
				*/
				RATIO_OF_SHIFT_PARAMS = 5
			};
			
			//@}
			/**	@name	Constructors and Destructors	
			*/
			//@{
		
			/** Default constructor.
			*/
			ShiftedLVMMMinimizer();
			
			/** Constructor expecting a valid force field 
			*/
			ShiftedLVMMMinimizer(ForceField& force_field);
			
			/** Constructor expecting a valid force field and a snapshot manager
			*/
			ShiftedLVMMMinimizer(ForceField& force_field, SnapShotManager *ssm);
			
			/** Constructor expecting a valid force field, a snapshot manager and options 
			*/
			ShiftedLVMMMinimizer(ForceField& force_field, SnapShotManager* ssm, const Options& options);
			
			/** Constructor expecting a valid force field and options 
			*/
			ShiftedLVMMMinimizer(ForceField& force_field, const Options& options);
			
			/** Copy constructor
			*/
			ShiftedLVMMMinimizer(const ShiftedLVMMMinimizer& rhs);
			
			/** Destructor.
			*/
			virtual ~ShiftedLVMMMinimizer();
			
			//@}
			/** @name Assignments
			*/
			//@{
			
			/** Assignment operator
			*/
			const ShiftedLVMMMinimizer& operator = (const ShiftedLVMMMinimizer& rhs);
			
			//@}
			/** @name	Setup methods. They do all necessary preparations. 
			*/
			//@{
			
			/** Specific setup
			*/
			virtual bool specificSetup();
			
			//@}
			/** @name	Accessors 
			*/
			//@{
			
			/** Set explicitly the update method
			 */
			void setUpdateMethod(UpdateMethod updt);
			
			/** Return the update method
			 */
			UpdateMethod getUpdateMethod() const;
			
			/** Set explicitly the correction parameter
			 */
			void setCorrectionParameter(CorrectionParameter corr);
			
			/** Return the correction parameter
			 */
			CorrectionParameter getCorrectionParameter() const;
			
			/** Set explicitly the maximum number of columns of the
			 *  factor of the shifted inverse hessian approximation.
			 */
			void setMaxNumOfColumns(Size num);
			
			/** Return the maximum number of columns of the
			 *  factor of the shifted inverse hessian approximation.
			 */
			Size getMaxNumOfColumns() const;
			
			/** Calculate the next step.
			 *  First this method updates the model and performs a line search
			 *  into the calculated direction afterwards.
			 *  @return double <b>\geq 0.0</b> if the line search found an acceptable solution, otherwise -1.0.
			 *  @see EnergyMinimizer::findStep
			 */
			virtual double findStep();
			
			/** Update the stored vector pairs, either by adding a new pair or if there
			 *  is no space left by replacing the oldest pair.
			 *  Compute the search direction afterwards by using the Strang recurrence formula
			 *  with an improvement idea of Al-Baali.
			 */
			virtual void updateDirection();
			
			/** Minimize the energy of the system.
			 *  This method executes at most <tt>iterations</tt> minimization steps.
			 *  If the number of iterations is not given, the number specified in the
			 *  options is taken.
			 *  @param	iterations the maximum number of iterations
			 *  @param	resume <b>true</b> to resume a previous run
			 *  @see		EnergyMinimizer::minimize
			 */
			virtual bool minimize(Size iterations = 0, bool resume = false); 
			
		protected:
			
			//@}
			/**	@name	Protected Attributes 
			*/
			//@{
			
			/*_ The line search
			*/
			LineSearch line_search_;
			
			/*_ Is this an initial first iteration?
			*/
			bool first_iter_;
		
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
			
			/*_ The update method
			*/
			Size updt_method_;
			
			/*_ The correction parameter
			*/
			Size corr_par_;
			
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
			
			/*_ The positions of the movable atoms when we start an iteration.
					This is used to reduce slightly rounding errors
			*/
			vector<Vector3> initial_atoms_;
			
			//@}
			
	};
} // end of namespace BALL

#endif // BALL_MOLMEC_MINIMIZATION_SHIFTEDLVMM_H 
