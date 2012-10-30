// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//

#ifndef BALL_QSAR_KPLSMODEL_H
#define BALL_QSAR_KPLSMODEL_H

#ifndef BALL_QSAR_KERNELMODEL_H
#include <BALL/QSAR/kernelModel.h>
#endif

#ifndef BALL_QSAR_NONLINEARMODEL_H
#include <BALL/QSAR/nonlinearModel.h>
#endif

#ifndef BALL_QSAR_LATENTVARIABLEMODEL_H
#include <BALL/QSAR/latentVariableModel.h>
#endif

namespace BALL
{
	namespace QSAR
	{
		/** kernel partial-least-squares class */
		class BALL_EXPORT KPLSModel : public KernelModel, public LatentVariableModel
		{
			public:
				/** @name Constructors and Destructors
				 */
				//@{
				KPLSModel(const QSARData& q, int k_type, double p1, double p2=-1);
				
				KPLSModel(const QSARData& q, Eigen::VectorXd& w);
				
				KPLSModel(const QSARData& q, String s1, String s2);
				
				KPLSModel(const QSARData& q, const LinearModel& lm, int column);

				~KPLSModel();

				EIGEN_MAKE_ALIGNED_OPERATOR_NEW
				//@}
				
				
				/** @name Accessors
				 */
				//@{
				
				void train();
				
				/** tries to find the optimal number of latente variables (=PLS components) */
				bool optimizeParameters(int k, int no_steps);
				
				/** returns the number of latente variables (=PLS components) */
				int getNoComponents();
				
				void setNoComponents(int d);
				
				/** returns a pointer to the  PLS Y-scores matrix U */
				const Eigen::MatrixXd* getU();
				
				void setParameters(vector<double>& v);
				
				vector<double> getParameters() const;
				//@}
				
				
			protected:
				/** @name Attributes
				 */
				Eigen::MatrixXd U_;
				
				int no_components_;
				//@}
				
		};
	}
}

#endif // KPLSMODEL
