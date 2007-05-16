// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: steepestDescent.h,v 1.24.20.2 2007/05/16 15:56:13 aleru Exp $
// Line Search Minimizer: A special class for the line search minimization algorithm

#ifndef BALL_MOLMEC_MINIMIZATION_STEEPESTDESCENT_H
#define BALL_MOLMEC_MINIMIZATION_STEEPESTDESCENT_H

#ifndef BALL_MOLMEC_MINIMIZATION_ENERGYMINIMIZER_H
# include <BALL/MOLMEC/MINIMIZATION/energyMinimizer.h>
#endif

#ifndef BALL_MOLMEC_MINIMIZATION_CONICLINESEARCH_H
# include <BALL/MOLMEC/MINIMIZATION/conicLineSearch.h>
#endif

//#ifndef BALL_MOLMEC_MINIMIZATION_LINESEARCH_H
//# include <BALL/MOLMEC/MINIMIZATION/lineSearch.h>
//#endif

namespace BALL 
{
	/** A minimizer for geometry optimization based on steepest descent steps.
	 *  \ingroup  MolmecEnergyMinimizer
	 */
	class BALL_EXPORT SteepestDescentMinimizer 
		: public EnergyMinimizer
	{
		
		public:
			
			/** @name Constructors and Destructors
			*/
			//@{
			
			BALL_CREATE(SteepestDescentMinimizer)
			
			/** Default constructor.
			*/
			SteepestDescentMinimizer();
			
			/** Constructor.
			*/
			SteepestDescentMinimizer(ForceField& force_field);
			
			/** Constructor.
			*/
			SteepestDescentMinimizer(ForceField& force_field,SnapShotManager *ssm);
			
			/** Constructor.
			*/
			SteepestDescentMinimizer(ForceField& force_field, const Options& options);
			
			/** Constructor.
			*/
			SteepestDescentMinimizer(ForceField& force_field, SnapShotManager* ssm, const Options& options);
			
			/** Copy constructor
			*/
			SteepestDescentMinimizer(const SteepestDescentMinimizer& minimizer);
			
			/** Destructor.
			*/
			virtual ~SteepestDescentMinimizer();
			
			//@}
			/** @name Assignments
			*/
			//@{
			
			/** Assignment operator
			*/
			const SteepestDescentMinimizer&	operator = (const SteepestDescentMinimizer& minimizer);
			
			//@}
			/** @name Setup methods
			*/
			//@{
			
			/** Specific setup
			*/
			virtual bool specificSetup();
			
			//@}
			/** @name Accessors
			 */
			//@{
			
			/** Minimize the energy of the system using scaled steepest descent steps.
			*/
			virtual bool minimize(Size steps = 0, bool resume = false);
			
			/** Find the next step using a line search.
			*/
			virtual double findStep();
			
			/** Update the search direction.
			 *  (Scaled) steepest descent searches along the current gradient only.
			 *  Therefore, updateDirection only assigns direction to the (scaled) last gradient
			 *  computed (current_gradient_).
			 */
			virtual void updateDirection();
			
		protected:
			
			//@}
			/** @name Protected Attributes
			 */
			//@{
			
			/*_ The line search minimizer.
					This member is used to perform the line search in findStep
			*/
			ConicLineSearch line_search_;
			//LineSearch line_search_;
			//@}
			
	};
} // namespace BALL

#endif // BALL_MOLMEC_MINIMIZATION_STEEPESTDESCENT_H
