// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//

#ifndef KPCRMODEL
#define KPCRMODEL

#ifndef KMODEL
#include <BALL/QSAR/kernelModel.h>
#endif

#ifndef PCRMODEL
#include <BALL/QSAR/pcrModel.h>
#endif

#ifndef NLMODEL
#include <BALL/QSAR/nonlinearModel.h>
#endif

namespace BALL
{
	namespace QSAR
	{
		/** class for kernel principal component regression */
		class KPCRModel : public KernelModel, public LatentVariableModel
		{
			public:
				/** @name Constructors and Destructors
				 */
				//@{
				KPCRModel(const QSARData& q, int k_type, double p1, double p2=-1);
				
				KPCRModel(const QSARData& q, RowVector& w);
				
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
