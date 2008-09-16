  // -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//

#ifndef ALLMODEL
#define ALLMODEL

#ifndef NLMODEL
#include <BALL/QSAR/nonlinearModel.h>
#endif


namespace BALL
{
	namespace QSAR 
	{

		/** class for automated lazy learning (ALL-QSAR) */
		class ALLModel : public NonLinearModel
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
				
				virtual Vector<double> predict(const vector<double>& substance, bool transform=1);
				
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
				virtual void calculateWeights(BALL::Matrix<double>& dist, BALL::Vector<double>& w);
				
				/** calculates weighted BALL::Matrix<double> X^T*X  */
				void calculateXX(BALL::Vector<double>& w, BALL::Matrix<double>& res);
			
				/** calculates weighted BALL::Matrix<double> X^T*Y  */
				void calculateXY(BALL::Vector<double>& w, BALL::Matrix<double>& res);
				
				/** calculates pairwise euclidean distance between all substances of m1 and m2 and saves them to BALL::Matrix<double> output */
				void calculateEuclDistanceMatrix(BALL::Matrix<double>& m1, BALL::Matrix<double>& m2, BALL::Matrix<double>& output);
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
