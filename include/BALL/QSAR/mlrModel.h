 // -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// 

#ifndef MLRMODEL
#define MLRMODEL

#ifndef LMODEL
#include <BALL/QSAR/linearModel.h>
#endif

#ifndef QSAR_EXCEPTION
#include <BALL/QSAR/exception.h>
#endif


#include <newmat/newmatio.h>

namespace BALL
{
	namespace QSAR
	{
		class MLRModel : public LinearModel
		{
			public:
				/** @name Constructors and Destructors
				 */
				//@{
				/** constructur
				@param q QSAR-wrapper object, from which the data for this model should be taken */
				MLRModel(const QSARData& q, double ridge_regression=0);

				~MLRModel();
				//@}
				
				/** @name Accessors
				 */
				//@{
				/** Starts multiple linear regression with the current data and saves the resulting linear combination of descriptors to training_result. \n 
				In order for this to work, descriptor_matrix MUST have more rows than columns, so that the matrix is invertible !! \n
				If this is not the case, start a feature selection method before running train() !  */
				void train();
				
				void setParameters(vector<double>& v);
				
				vector<double> getParameters() const;
				//@}
		
			private:
				/** @name Attributes
				 */
				//@{
				/** ridge regression value. If rr!=0, ridge regression is done */
				double rr_;
				//@}
		};
	}
}

#endif // MLRMODEL
