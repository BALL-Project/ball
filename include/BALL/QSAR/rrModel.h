// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// 

#ifndef RIDGEMODEL
#define RIDGEMODEL

#ifndef MLRMODEL
#include <BALL/QSAR/mlrModel.h>
#endif

#ifndef QSAR_EXCEPTION
#include <BALL/QSAR/exception.h>
#endif



namespace BALL
{
	namespace QSAR
	{
		class BALL_EXPORT RRModel : public MLRModel
		{
			public:
				/** @name Constructors and Destructors
				 */
				//@{
				/** constructur
				@param q QSARData object, from which the data for this model should be taken */
				RRModel(const QSARData& q, double lambda=0.003);

				~RRModel();
				//@}
				
				/** @name Accessors
				 */
				//@{
				/** Starts multiple linear regression with the current data and saves the resulting linear combination of descriptors to training_result. \n */
				void train();
				
				void setParameters(vector<double>& v);
				
				vector<double> getParameters() const;
				//@}
		
			private:
				/** @name Attributes
				 */
				//@{
				double lambda_;
				
				//@}
		};
	}
}

#endif // RIDGEMODEL

