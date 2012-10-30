// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//

#ifndef BALL_QSAR_ALLMODEL_H
#define  BALL_QSAR_ALLMODEL_H

#ifndef BALL_QSAR_NONLINEARMODEL_H
#include <BALL/QSAR/nonlinearModel.h>
#endif


namespace BALL
{
	namespace QSAR
	{

		/** class for automated lazy learning (ALL-QSAR) */
		class BALL_EXPORT ALLModel : public NonLinearModel
		{
			public:
				/** @name Constructors and Destructors
				 */
				//@{
				ALLModel(const QSARData& q, double kw=4);

				~ALLModel();
				//@}
				
				/** @name Accessors
				 */
				//@{
				void setKw(double kw);
				
				virtual Eigen::VectorXd predict(const vector<double>& substance, bool transform=1);
				
				/** automated lazy learning does not have a seperate training step */
				void train(){};
				
				/** Tries to find the optimal kernel width for the current data. Therefore some training data must have been read by the connected QSARData object before running this method. */
				virtual bool optimizeParameters(int d, int no_steps);
				
				/** returns the current kernel width */
				double getKw();
				
				virtual void setParameters(vector<double>& v);
				
				virtual vector<double> getParameters() const;
				
				void saveToFile(string filename);
				
				void readFromFile(string filename);
				//@}
				
				
			protected:
				
				/** @name Accessors
				 */
				//@{
				virtual void calculateWeights(Eigen::MatrixXd& dist, Eigen::VectorXd& w);
				
				/** calculates weighted Eigen::MatrixXd X^T*X  */
				void calculateXX(Eigen::VectorXd& w, Eigen::MatrixXd& res);
			
				/** calculates weighted Eigen::MatrixXd X^T*Y  */
				void calculateXY(Eigen::VectorXd& w, Eigen::MatrixXd& res);
				
				/** calculates pairwise euclidean distance between all substances of m1 and m2 and saves them to Eigen::MatrixXd output */
				void calculateEuclDistanceMatrix(Eigen::MatrixXd& m1, Eigen::MatrixXd& m2, Eigen::MatrixXd& output);
				//@}
				
				
				/** @name Attributes
				 */
				//@{
				/** kernel width */
				double kw_;
				double lambda_;
				//@}
		};
	}
}

#endif // ALLMODEL
