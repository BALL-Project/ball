// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//

#ifndef LDAMODEL
#define LDAMODEL

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

		/** class for support vector classification */
		class LDAModel : public ClassificationModel
		{ 
			public:
				/** @name Constructors and Destructors
				 */
				//@{
				LDAModel(const QSARData& q);

				~LDAModel();
				//@}
				
				
				/** @name Accessors
				 */
				//@{
				void train();
				
				Vector<double> predict(const vector<double>& substance, bool transform=1);

				void setParameters(vector<double>& v);
				
				vector<double> getParameters() const;
				
				void saveToFile(string filename);
				
				void readFromFile(string filename);
				//@}
				
				
			private:
				/** @name Attributes
				 */
				//@{
				/** covariance matrix of descriptors */
				Matrix<double> sigma_;
				

				double lambda_;				

				/** vector containing one matrix for each modelled activity. Each matrix contains a mean vector of a class in each line */
				vector<Matrix<double> > mean_vectors_;
				//@}
		};
	}
}

#endif //LDAMODEL
