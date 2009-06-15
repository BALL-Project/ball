// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//

#ifndef SNBMODEL
#define SNBMODEL

#ifndef STATISTICS
#include <BALL/QSAR/statistics.h>
#endif

#ifndef SORTEDLIST
#include <BALL/QSAR/sortedList.h>
#endif

#ifndef BAYESMODEL
#include <BALL/QSAR/bayesModel.h>
#endif


namespace BALL{
	namespace QSAR {

		/** class for simple naive Bayes */
		class SNBModel : public BayesModel
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
				
				Vector<double> predict(const vector<double>& substance, bool transform=1);
				
				vector<double> getParameters() const;
				
				void setParameters(vector<double>& v);
				
				void saveToFile(string filename);
				
				bool isTrained();
				
				void readFromFile(string filename);
				
				/** calculate the probability for the specified feature to assume the given value for each class
				@return a probability for each class */
				vector<double> calculateProbabilities(int activitiy_index, int feature_index, double feature_value);
				
				int getNoResponseVariables();
				//@}
				
				
			private:
				/** @name Attributes
				 */
				//@{
				/** vector containing one matrix for each modelled activity which holds the mean of each feature for each class */
				vector<Matrix<double> > mean_;
				
				/** vector containing one matrix for each modelled activity which holds the standard deviation of each feature for each class */
				vector<Matrix<double> > stddev_;
				
				static const double sqrt2Pi_;
							

				
				//@}
		};
	}
}

#endif //SNBMODEL
