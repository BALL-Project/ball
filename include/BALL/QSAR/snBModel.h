// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//

#ifndef SNBMODEL
#define SNBMODEL

#ifndef CLASSIFICATION
#include <BALL/QSAR/classificationModel.h>
#endif

#ifndef STATISTICS
#include <BALL/QSAR/statistics.h>
#endif

#ifndef SORTEDLIST
#include <BALL/QSAR/sortedList.h>
#endif


namespace BALL{
	namespace QSAR {

		/** class for simple naive Bayes */
		class SNBModel : public ClassificationModel
		{ 
			public:
				/** @name Constructors and Destructors
				 */
				//@{
				SNBModel(const QSARData& q);

				~SNBModel();
				//@}
				
				
				/** @name Accessors
				 */
				//@{
				void train();
				
				RowVector predict(const vector<double>& substance, bool transform=1);
				
				void saveToFile(string filename);
				
				void readFromFile(string filename);
				//@}
				
				
			private:
				/** @name Attributes
				 */
				//@{
				/** vector containing one matrix for each modelled activity which holds the mean of each feature for each class */
				vector<Matrix> mean_;
				
				/** vector containing one matrix for each modelled activity which holds the standard deviation of each feature for each class */
				vector<Matrix> stddev_;
							

				
				//@}
		};
	}
}

#endif //SNBMODEL
