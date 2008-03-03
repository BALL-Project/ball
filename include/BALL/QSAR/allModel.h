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
				
				virtual RowVector predict(const vector<double>& substance, bool transform=1);
				
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
				void calculateWeights(Matrix& dist, RowVector& w);
				
				/** calculates weighted Matrix X^T*X  */
				void calculateXX(RowVector& w, Matrix& res);
			
				/** calculates weighted Matrix X^T*Y  */
				void calculateXY(RowVector& w, Matrix& res);
				
				/** calculates pairwise euclidean distance between all substances of m1 and m2 and saves them to Matrix output */
				void calculateEuclDistanceMatrix(Matrix& m1, Matrix& m2, Matrix& output);
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
