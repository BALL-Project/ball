// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//

#ifndef BALL_QSAR_GPMODEL_H
#define BALL_QSAR_GPMODEL_H

#ifndef BALL_QSAR_KERNELMODEL_H
#include <BALL/QSAR/kernelModel.h>
#endif

#ifndef BALL_QSAR_NONLINEARMODEL_H
#include <BALL/QSAR/nonlinearModel.h>
#endif

namespace BALL
{
	namespace QSAR
	{	/** class for gaussian process regression */
		class BALL_EXPORT GPModel : public KernelModel
		{
			public:
				/** @name Constructors and Destructors
				 */
				//@{
				GPModel(const QSARData& q, int k_type, double p1, double p2=-1);
				
				GPModel(const QSARData& q, Eigen::VectorXd& w);
				
				/** constructor that sets KernelModel.f to s1 and KernelModel.g to s2 */
				GPModel(const QSARData& q, String s1, String s2);
				
				GPModel(const QSARData& q, const LinearModel& lm, int column);

				~GPModel();

				EIGEN_MAKE_ALIGNED_OPERATOR_NEW
				//@}
				
				/** @name Accessors
				 */
				//@{
				void train();
				
				Eigen::VectorXd predict(const vector<double>& substance, bool transform=1);
				
				/** calculates standart error for the last prediction as \f$ \sqrt{k(x_*,x_*)-\sum_{i=1}^n \sum_{j=1}^n k(x_*,x_i)*k(x_*,x_j)-L_{ij} } \f$*/
				double calculateStdErr();
				
				void setParameters(vector<double>& v);
				
				vector<double> getParameters() const;
				//@}
				
			private:
				/** @name Attributes
				 */
				//@{
				Eigen::MatrixXd L_;
				
				/** the last predicted substance */
				Eigen::VectorXd input_;
				
				Eigen::RowVectorXd K_t_;
				
				double lambda_;
				//@}
		};
	}
}

#endif // GPMODEL
