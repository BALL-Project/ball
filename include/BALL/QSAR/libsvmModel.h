// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//

#ifndef LIBSVMMODELH
#define LIBSVMMODELH

#ifndef SVRMODEL
#include <BALL/QSAR/svrModel.h>
#endif

#include <fstream>
#include <svm.h>


namespace BALL
{
	namespace QSAR
	{
		class LibsvmModel : public SVRModel
		{
			public:
				/** @name Constructors and Destructors
				 */
				//@{
				LibsvmModel(const QSARData& q, int k_type, double p1, double p2=-1);

				~LibsvmModel();
				//@}
				
				
				/** @name Accessors
				 */
				//@{				
				void train();
				
				//RowVector predict(const vector<double>& substance, bool transform=1);
				
				void setParameters(vector<double>& v);
				
				vector<double> getParameters() const;
				//@}
				
				
			private:
				
				struct svm_problem* createProblem();
				
				void createParameters();
				
				struct svm_model* svm_train_result_;
				
				struct svm_parameter parameters_;
				
				struct svm_node* x_space_;
				
				/** determines whether nu-SVR is to be used; else eps-SVR is applied */
				bool use_nu_;
				
				/** determines whether the libsvm shrinking heuristics is to be used */
				bool use_shrinking_;
				
				double nu_;
				double p_;
				double eps_;
				double C_;
				
				
		};
	}
}



#endif // LIBSVMMODELH
