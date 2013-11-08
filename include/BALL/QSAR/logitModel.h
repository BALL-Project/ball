// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//

#ifndef LOGITMODEL
#define LOGITMODEL

#ifndef CLASSIFICATION
#include <BALL/QSAR/classificationModel.h>
#endif



namespace BALL{
	namespace QSAR {

		/** class for logistic regression */
		class BALL_EXPORT LogitModel : public ClassificationModel
		{ 
			public:
				/** @name Constructors and Destructors
				 */
				//@{
				LogitModel(const QSARData& q);

				~LogitModel();

				EIGEN_MAKE_ALIGNED_OPERATOR_NEW
				//@}
				
				
				/** @name Accessors
				 */
				//@{
				void train();
				
				Eigen::VectorXd predict(const vector<double>& substance, bool transform);
				
				void saveToFile(string filename);
				
				void readFromFile(string filename);
				//@}
				
				
			protected:	
				/** @name Attributes
				 */
				//@{
				Eigen::MatrixXd training_result_;
				//@}
				
		};
	}
}

#endif // LOGITMODEL
