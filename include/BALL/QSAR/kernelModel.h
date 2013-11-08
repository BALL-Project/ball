// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//

#ifndef KMODEL
#define KMODEL

#ifndef NLMODEL
#include <BALL/QSAR/nonlinearModel.h>
#endif

#ifndef LMODEL
#include <BALL/QSAR/linearModel.h>
#endif

#ifndef KERNEL
#include <BALL/QSAR/kernel.h>
#endif



namespace BALL
{
	namespace QSAR
	{
		class BALL_EXPORT KernelModel : public NonLinearModel
		{
			public:
				/** @name Constructors and Destructors
				 */
				//@{
				KernelModel(const QSARData& q, int k_type, double p1, double p2);
				
				KernelModel(const QSARData& q, String f, String g);
						
				KernelModel(const QSARData& q, Eigen::VectorXd& w);
				
				KernelModel(const QSARData& q, const LinearModel& lm, int column);
				
				~KernelModel();
				
				virtual void saveToFile(string filename);
				
				virtual void readFromFile(string filename);
				
				virtual Eigen::VectorXd predict(const vector<double>& substance, bool transform);
				
				void operator=(const Model& m);

				EIGEN_MAKE_ALIGNED_OPERATOR_NEW
				//@}
				
				/** @name Attributes
				 */
				//@{
				Kernel* kernel;
				//@}

				
			protected:
				
				/** @name Attributes
				 */
				//@{
				/** Matrix containing the pairwise distances between all substances */
				Eigen::MatrixXd K_;
				
				/** resulting matrix with one column for each modeled activity and with one coefficient for each substance (i.e. one column for each column for Model.Y) */
				//Matrix B;
				//@}
				
				
				/** @name Input and Output. The following methods can be used to implement the functions saveToFile() and readFromFile() in final classes derived from this base-class 
				 */
				//@{
				void calculateOffsets();
				
				void readKernelParametersFromFile(std::ifstream& in);
				void saveKernelParametersToFile(std::ofstream& out);
				void saveTrainingResult(std::ofstream& out);
				void readTrainingResult(std::ifstream& input, int no_substances, int no_y);
				
				friend class RegressionValidation;
				//}@
				
			
		};
	}
}


#endif // NLMODEL
