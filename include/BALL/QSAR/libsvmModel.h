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
		class BALL_EXPORT LibsvmModel : public SVRModel
		{
			public:
				/** @name Constructors and Destructors
				 */
				//@{
				LibsvmModel(const QSARData& q, int k_type, double p1, double p2=-1);

				virtual ~LibsvmModel();
				//@}
				
				
				/** @name Accessors
				 */
				//@{				
				void train();
				
				//RowVector predict(const vector<double>& substance, bool transform=1);
				
				void setParameters(vector<double>& v);
				
				virtual vector<double> getParameters() const;
				//@}
				
				
			private:

				// part of libsvm; unfortunately defined in svm.C instead of svm.h, so that we need this hack ...
				struct svm_model
				{
					svm_parameter param;	// parameter
					int nr_class;		// number of classes, = 2 in regression/one class svm
					int l;			// total #SV
					svm_node **SV;		// SVs (SV[l])
					double **sv_coef;	// coefficients for SVs in decision functions (sv_coef[k-1][l])
					double *rho;		// constants in decision functions (rho[k*(k-1)/2])
					double *probA;          // pariwise probability information
					double *probB;

					// for classification only
					int *label;		// label of each class (label[k])
					int *nSV;		// number of SVs for each class (nSV[k])
					// nSV[0] + nSV[1] + ... + nSV[k-1] = l
					// XXX
					int free_sv;		// 1 if svm_model is created by svm_load_model
									// 0 if svm_model is created by svm_train
				};
				
				struct svm_problem* createProblem(int response_id);
				
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
