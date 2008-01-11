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

namespace BALL
{
	namespace QSAR
	{
		/** kernel partial-least-squares class */
		class KPLSModel : public KernelModel
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
				RowVector predict(const vector<double>& substance, bool transform=1);
				
				void train();
				
				/** tries to find the optimal number of latente variables (=PLS components) */
				void findNoComponents(int k);
				
				bool optimizeParameters(int k);
				
				/** returns the number of latente variables (=PLS components) */
				int getNoComponents();
				
				void setNoComponents(int d);
				
				/** returns a pointer to the  PLS score matrix T */
				const Matrix* getT();

				/** returns a pointer to the  PLS X-weights matrix W */
				const Matrix* getW();
				
				/** returns a pointer to the  PLS components-weights matrix C */
				const Matrix* getC();
				
				/** returns a pointer to the  PLS Y-scores matrix U */
				const Matrix* getU();
				
				void setParameters(vector<double>& v);
				
				vector<double> getParameters() const;
				//@}
				
				
			protected:
				/** @name Attributes
				 */
				//@{
				Matrix T_;

				Matrix W_;
				
				Matrix U_;

				Matrix C_;
				
				int no_components_;
				//@}
				
		};
	}
}

#endif // KPLSMODEL
