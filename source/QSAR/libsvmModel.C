// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//

#include <BALL/QSAR/libsvmModel.h>
#define Malloc(type,n) (type *)malloc((n)*sizeof(type))

using namespace BALL::QSAR;


LibsvmModel::LibsvmModel(const QSARData& q, int k_type, double p1, double p2) : SVRModel(q, k_type, p1, p2) 
{ 
	type_="SVR";
	svm_train_result_ = NULL;
	use_nu_ = 1;
	nu_ = 0.05;
	p_ = 0.1;
	eps_ = 1e-3;
	C_ = 1; 
	createParameters();
	x_space_ = NULL;
}

LibsvmModel::~LibsvmModel()
{
	delete x_space_;
}


void LibsvmModel::train()
{
	if (descriptor_matrix_.cols() == 0)
	{
		throw Exception::InconsistentUsage(__FILE__, __LINE__, "Data must be read into the model before training!"); 
	}

	kernel->calculateKernelMatrix(descriptor_matrix_, K_);
	struct svm_problem* prob = NULL;
	training_result_.resize(K_.rows(), Y_.cols());
	offsets_.resize(Y_.cols());
	
	for (int act = 0; act <= Y_.cols(); act++)
	{
		prob = createProblem(act);
		svm_train_result_ = (LibsvmModel::svm_model*)svm_train(prob, &parameters_);
		free(prob->y);free(prob->x);free(prob);
		
		const double* const *sv_coef = svm_train_result_->sv_coef;
		//const svm_node* const *SV = svm_train_result_->SV;

		for(int i=0;i<svm_train_result_->l;i++) // l=#support vectors == #compounds
		{
			for (int j = 0; j < svm_train_result_->nr_class-1; j++)
			{
				training_result_(i, j) = sv_coef[j][i];
			}
		}
		
		offsets_(act) = svm_train_result_->rho[0];
		
		//free(prob);
		//free(prob->y); free(prob->x);
		#ifdef LIBSVM_VERSION
			svm_free_and_destroy_model((::svm_model**)&svm_train_result_);			
		#else
			svm_destroy_model((::svm_model*)svm_train_result_);
		#endif
	}
}


struct svm_problem* LibsvmModel::createProblem(int response_id)
{
	struct svm_problem* prob = Malloc(svm_problem, 1);
	prob->l = K_.rows();
	prob->y = Malloc(double, prob->l);
	prob->x = Malloc(struct svm_node*, prob->l);
	int elements = (K_.cols()+2)*K_.rows();
	free(x_space_);
	x_space_ = Malloc(struct svm_node, elements);
	
	int cols = K_.cols();
	int index = 0;
	
	for (int i = 0; i < K_.rows(); i++)
	{
		//prob->x[i-1] = Malloc(struct svm_node, cols+1);	
		prob->x[i] = &x_space_[index];
		prob->y[i] = Y_(i, response_id);
		x_space_[index].index = 0;
		x_space_[index].value = i; // numer of current row
		index++;
		for (int j = 0; j < cols; j++)
		{ 
			//prob->x[i-1][j-1].index = j;
			//prob->x[i-1][j-1].value = K_(i, j);
			x_space_[index].index = j;
			x_space_[index].value = K_(i, j);
			index++;
		}
		//prob->x[i-1][cols].index = -1;
		//prob->x[i-1][cols].value = '?';
		x_space_[index].index = -1;
		x_space_[index].value = '?';
		index++;
	}
	return prob;	
}


void LibsvmModel::createParameters()
{
	parameters_.kernel_type = 4; // use precomputed kernel !
	parameters_.cache_size = 100;
	if (use_nu_)
	{
		parameters_.svm_type = 4;
	}
	else
	{
		parameters_.svm_type = 3;
	}
	parameters_.nu = nu_;
	parameters_.C = C_;
	parameters_.eps = eps_;
	parameters_.p = p_;
	parameters_.shrinking = use_shrinking_;
	parameters_.probability = 0;	
}


// RowVector LibsvmModel::predict(const Eigen::VectorXd& substance, bool transform)
// {	
// 	if(svm_train_result_==NULL)
// 	{
// 		throw Exception::InconsistentUsage(__FILE__,__LINE__,"Model must be trained before it can predict the activitiy of substances!");
// 	}
// 	RowVector input=getSubstanceVector(substance, transform);
// 	Matrix K_t(input.rows(), descriptor_matrix_.rows());
// 	kernel->calculateKernelMatrix(K_,input, descriptor_matrix_, K_t);
// 		
// 	svm_node* node = Malloc(struct svm_node, K_t.cols()+1);
// 	
// 	for(int i=1; i<=K_t.cols(); i++)
// 	{
// 		node[i-1].index = i-1;
// 		node[i-1].value = K_t(1,i);
// 	}
// 	node[K_t.cols()].index = -1;
// 	node[K_t.cols()].value = '?';
// 		
// 	double res = svm_predict(svm_train_result_, node);
// 	
// 	RowVector rv(1);
// 	rv << res;
// 	
// 	if(transform && y_transformations_.cols()!=0)
// 	{
// 		backTransformPrediction(rv);
// 	}
// 
// 	free(node);
// 	return rv;
// }


void LibsvmModel::setParameters(vector<double>& v)
{
	if (v.size() != 6)
	{
		String c = "Wrong number of model parameters! Needed: 6;";
		c = c+" given: "+String(v.size());
		throw Exception::ModelParameterError(__FILE__, __LINE__, c.c_str());
	}
	String d = v[0];
	use_nu_ = d.toBool();
	d = v[1];
	use_shrinking_ = d.toBool();
	nu_ = v[2];
	p_ = v[3];
	eps_ = v[4];
	C_ = v[5];
	createParameters();
}


vector<double> LibsvmModel::getParameters() const
{
	vector<double> d;
	d.push_back(use_nu_);
	d.push_back(use_shrinking_);
	d.push_back(nu_);
	d.push_back(p_);
	d.push_back(eps_);
	d.push_back(C_);
	return d;
}
