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
		class LogitModel : public ClassificationModel
		{ 
			public:
				/** @name Constructors and Destructors
				 */
				//@{
				LogitModel(const QSARData& q);

				~LogitModel();
				//@}
				
				
				/** @name Accessors
				 */
				//@{
				void train();
				
				Vector<double> predict(const vector<double>& substance, bool transform);
				
				void saveToFile(string filename);
				
				void readFromFile(string filename);
				//@}
				
				
			protected:	
				/** @name Attributes
				 */
				//@{
				Matrix<double> training_result_;
				//@}
				
		};
	}
}

#endif // LOGITMODEL
