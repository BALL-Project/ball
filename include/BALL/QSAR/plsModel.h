// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//

#ifndef PLSMODEL
#define PLSMODEL

#ifndef LMODEL
#include <BALL/QSAR/linearModel.h>
#endif

#ifndef STATISTICS
#include <BALL/QSAR/statistics.h>
#endif

#ifndef QSAR_EXCEPTION
#include <BALL/QSAR/exception.h>
#endif

#include <BALL/QSAR/latentVariableModel.h>


namespace BALL
{
	namespace QSAR
	{	
		class BALL_EXPORT PLSModel : public LinearModel, public LatentVariableModel
		{
			public:
				/** @name Constructors and Destructors
				 */
				//@{
				/** constructur
				@param q QSAR-wrapper object, from which the data for this model should be taken */
				PLSModel(const QSARData& q);

				~PLSModel();

				EIGEN_MAKE_ALIGNED_OPERATOR_NEW
				//@}
				
				
				/** @name Accessors
				 */
				//@{
				/** Starts partial least squares regression. \n
				In order to find the optimal number of latente variables for the current data of this model, run findNoLatenteVariables() first. */
				virtual void train();
				
				/** Tries to find the optimal number of PLS components (latente variables) for the current data of this model */
				virtual bool optimizeParameters(int k, int no_steps);

				/** set the number of PLS components to create */
				void setNoComponents(int no);
				
				/** get the number of PLS components */
				int getNoComponents();
				
				/** returns a pointer to the  PLS Y-scores matrix U */
				const Eigen::MatrixXd* getU();
				
				void setParameters(vector<double>& v);
				
				vector<double> getParameters() const;
				//@}
		
				
			protected:
				/** @name Attributes
				 */
				//@{
				
				Eigen::MatrixXd U_;
	
				int no_components_;
				//@}
		};
	}
}


#endif // PLSMODEL
