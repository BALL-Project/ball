// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//

#ifndef KPLSMODEL
#define KPLSMODEL

#ifndef KMODEL
#include <BALL/QSAR/kernelModel.h>
#endif

#ifndef NLMODEL
#include <BALL/QSAR/nonlinearModel.h>
#endif

#include <BALL/QSAR/latentVariableModel.h>

namespace BALL
{
	namespace QSAR
	{
		/** kernel partial-least-squares class */
		class KPLSModel : public KernelModel, public LatentVariableModel
		{
			public:
				/** @name Constructors and Destructors
				 */
				//@{
				KPLSModel(const QSARData& q, int k_type, double p1, double p2=-1);
				
				KPLSModel(const QSARData& q, RowVector& w);
				
				KPLSModel(const QSARData& q, String s1, String s2);
				
				KPLSModel(const QSARData& q, const LinearModel& lm, int column);

				~KPLSModel();
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
				const Matrix* getU();
				
				void setParameters(vector<double>& v);
				
				vector<double> getParameters() const;
				//@}
				
				
			protected:
				/** @name Attributes
				 */
				Matrix U_;
				
				int no_components_;
				//@}
				
		};
	}
}

#endif // KPLSMODEL
