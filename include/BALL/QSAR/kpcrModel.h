// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//

#ifndef BALL_QSAR_KPCRMODEL_H
#define BALL_QSAR_KPCRMODEL_H

#ifndef BALL_QSAR_KERNELMODEL_H
#include <BALL/QSAR/kernelModel.h>
#endif

#ifndef BALL_QSAR_PCRMODEL_H
#include <BALL/QSAR/pcrModel.h>
#endif

#ifndef BALL_QSAR_NONLINEARMODEL_H
#include <BALL/QSAR/nonlinearModel.h>
#endif

namespace BALL
{
	namespace QSAR
	{
		/** class for kernel principal component regression */
		class BALL_EXPORT KPCRModel : public KernelModel, public LatentVariableModel
		{
			public:
				/** @name Constructors and Destructors
				 */
				//@{
				KPCRModel(const QSARData& q, int k_type, double p1, double p2=-1);
				
				KPCRModel(const QSARData& q, Eigen::VectorXd& w);
				
				KPCRModel(const QSARData& q, String s1, String s2);
				
				KPCRModel(const QSARData& q, const LinearModel& lm, int column);

				~KPCRModel();
				//@}
				
				
				/** @name Accessors
				 */
				//@{
				/** set the fraction of the variance that should be explained by the latente variables. */
				void setFracVar(double frac_var);
				
				void train();
				
				void setParameters(vector<double>& v);
				
				vector<double> getParameters() const;
				//@}
				
				
			private:
				
				double frac_var_;
		};
	}
}

#endif //KPCRMODEL
