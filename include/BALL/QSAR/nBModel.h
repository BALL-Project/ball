// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//

#ifndef NBMODEL
#define NBMODEL

#ifndef BAYESMODEL
#include <BALL/QSAR/bayesModel.h>
#endif

#ifndef STATISTICS
#include <BALL/QSAR/statistics.h>
#endif

#ifndef SORTEDLIST
#include <BALL/QSAR/sortedList.h>
#endif


namespace BALL
{
	namespace QSAR 
	{

		/** class for Naive Bayes */
		class NBModel : public BayesModel
		{ 
			public:
				/** @name Constructors and Destructors
				 */
				//@{
				NBModel(const QSARData& q);

				~NBModel();
				//@}
				
				
				/** @name Accessors
				 */
				//@{
				void train();
				
				Vector<double> predict(const vector<double>& substance, bool transform=1);
				
				void saveToFile(string filename);
				
				void readFromFile(string filename);
				
				vector<double> getParameters() const;
				
				void setParameters(vector<double>& v);
				
				bool isTrained();
				
				vector<double> calculateProbabilities(int activitiy_index, int feature_index, double feature_value);
				
				//@}
				
				
			private:
				/** @name Attributes
				 */
				//@{
				
				uint discretization_steps_;
				
				
				/** the minmum (row1) and maximum (row2) of each feature. */
				Matrix<double> min_max_;
				
				/** One probability Matrix for each modelled activity and each class. \n
				Each Matrix stores in each cell the probability for a feature lying within a specific range to be in a specific class  */
				vector<vector<Matrix<double> > > probabilities_;
							

				
				//@}
		};
	}
}

#endif //NBMODEL
