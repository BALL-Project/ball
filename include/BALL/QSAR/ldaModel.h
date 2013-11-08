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

namespace BALL{
	namespace QSAR {

		/** class for support vector classification */
		class BALL_EXPORT LDAModel : public ClassificationModel
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
				
				Eigen::VectorXd predict(const vector<double>& substance, bool transform=1);

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
				Eigen::MatrixXd sigma_;
				

				double lambda_;				

				/** vector containing one matrix for each modelled activity. Each matrix contains a mean vector of a class in each line */
				vector<Eigen::MatrixXd > mean_vectors_;
				//@}
		};
	}
}

#endif //LDAMODEL
