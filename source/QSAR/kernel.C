//  -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//

#include <BALL/QSAR/kernel.h>

using namespace std;

namespace BALL
{
	namespace QSAR
	{

		/*Kernel(RegressionModel* m, int k_type, double p1, double p2 = 1)
		{
			if (k_type == 4)
			{
				throw Exception::KernelParameterError(__FILE__, __LINE__, "Equations must be specified in order to create a kernel with an individual kernel function!"); 
			}

			if (k_type == 5)
			{
				throw Exception::KernelParameterError(__FILE__, __LINE__, "A linear model_ or a RowVector must be given in order to create a kernel with a weighted distance function!");
			}
			regr_model = m;
			par1 = p1;
			par2 = p2;
			f="";
			g="";
			type = k_type;
		}*/	
			
			

			
		Kernel::Kernel(Model* m, int k_type, double p1, double p2)
		{
			if (k_type == 4)
			{
				throw Exception::KernelParameterError(__FILE__, __LINE__, "Equations must be specified in order to create a kernel with an individual kernel function!"); 
			}

			if (k_type == 5)
			{
				throw Exception::KernelParameterError(__FILE__, __LINE__, "A linear model_ or a RowVector must be given in order to create a kernel with a weighted distance function!");
			}
			model_ = m;
			par1 = p1;
			par2 = p2;
			equation1="";
			equation2="";
			type = k_type;
		}


		Kernel::Kernel(Model* m, String s1, String s2) 
		{
			type = 4;
			model_ = m;
			par1 = 2;
			equation1 = s1;
			equation2 = s2;
		}


		Kernel::Kernel(Model* m, Eigen::VectorXd& w)
		{
			type = 5;
			model_ = m;
			par1 = 2;
			weights_ = w;
		}


		Kernel::Kernel(Model* m, const LinearModel& lm, int column)
		{
			model_ = m;
			par1 = 2;
			type = 5;
			
			const Eigen::MatrixXd* w = lm.getTrainingResult();
			if (w->rows() == 0)
			{
				throw Exception::KernelParameterError(__FILE__, __LINE__, "linear model_ must be trained before its weights_ can be used for a weighted kernel function!"); 
			}
			Eigen::VectorXd v(w->rows());
			for (int i = 0; i < w->rows(); i++)
			{
				v(i) = (*w)(i, column);
			}
			weights_ = v;

			m->copyDescriptorIDs(lm);	
		}

		Kernel::~Kernel()
		{
		}

		void Kernel::calculateKernelVector(Eigen::MatrixXd& K, Eigen::VectorXd& input, Eigen::MatrixXd& descriptor_matrix, Eigen::RowVectorXd& output)
		{
			Eigen::MatrixXd M1(1, input.rows());
			M1.row(1) = input;
			Eigen::MatrixXd out;
			calculateKernelMatrix(K, M1, descriptor_matrix, out);
			output = out.row(1);
		}

		void Kernel::calculateKernelMatrix(Eigen::MatrixXd& input, Eigen::MatrixXd& output)
		{
			output.resize(input.cols(), input.cols());
			
			if (type == 1)
			{
				calculateKernelMatrix1(input, output);
			}
			else if (type == 2)
			{
				calculateKernelMatrix2(input, output);
			}
			else if (type == 3)
			{
				calculateKernelMatrix3(input, output);
			}
			else if (type == 4)
			{
				calculateKernelMatrix4(input, output);
			}
			else if (type == 5 && weights_.rows() != 0)
			{
				calculateWeightedKernelMatrix(input, output); 
				if (*model_->getType() == "GP"){return; }
			}
			
			// center Eigen::MatrixXd output
		//  	Eigen::MatrixXd I; I.setToIdentity(output.cols());
		//  	Eigen::VectorXd iv(output.cols());
		//  	iv = 1;
		//  	double d = 1./output.cols();
		// 	
		// 	I -= iv*d*iv.t();
		//  	output = I*output*I;
		}


		void Kernel::calculateKernelMatrix(Eigen::MatrixXd& K, Eigen::MatrixXd& m1, Eigen::MatrixXd& m2, Eigen::MatrixXd& output)
		{
			output.resize(m1.cols(), K.cols());
				
			if (type == 1)
			{
				calculateKernelMatrix1(m1, m2, output);
			}
			else if (type == 2)
			{
				calculateKernelMatrix2(m1, m2, output);
			}
			else if (type == 3)
			{
				calculateKernelMatrix3(m1, m2, output);
			}
			else if (type == 4)
			{
				calculateKernelMatrix4(m1, m2, output);
			}
			else if (type == 5 && weights_.rows() != 0)
			{
				calculateWeightedKernelMatrix(m1, m2, output); 
				if (*model_->getType() == "GP"){return; }
			}
			
			// center Eigen::MatrixXd output
		// 	Eigen::MatrixXd I; I.setToIdentity(output.cols());
		//  	Eigen::VectorXd iv(m2.rows()); //dim: nx1 
		// 	iv = 1;
		// 			
		//  	double d = 1./output.cols();
		// 	
		// 	I -= iv*d*iv.t(); 
		// 	output -= iv.t()*d*(iv.t()*K); 
		//  	output *= I;
		}


		void Kernel::gridSearch(double step_width, int steps, int recursions, int k, bool opt)
		{
			bool first_rec = 1;
			
			if (step_width <= 0)
			{
				throw Exception::KernelParameterError(__FILE__, __LINE__, "step-width for kernel parameter grid search must be > 0 !"); 
			}
			
			/// search locally around current kernel parameters, but always start with par1>0 und par2<0
			double par1_start;
			double par2_start;
			if (type == 2) 
			{
				if (par1 < 1) par1_start = par1*pow(2, steps/2.); 
				else par1_start = 10;
			}
			else par1_start = par1-((steps/2.)*step_width);
			if (par1_start <= 0)
			{
				par1_start = step_width;
			}

			par2_start = par2+((steps/2.)*step_width);
			if (par2_start >= 0)
			{
				par2_start = -step_width;
			}
			
		#ifdef BALL_DEBUG
			cout<<"starting kernel parameter grid search:"<<endl;
			cout<<"\tstep-width="<<step_width<<", "<<steps<<" steps, "<<recursions<<" recursions"<<endl;
			cout<<"\tusing "<<k<<"-fold cross-validation"<<endl;
			if (opt) cout<<"\toptimizing model parameters in EACH step of grid search"<<endl; 
			cout<<"\tparmeter1 start-value="<<par1_start<<"  parmeter2 start-value="<<par2_start<<endl<<flush;
		#endif	
			
			/// run grid-search once + one time for each desired recursion
			for (int i = 0; i <= recursions; i++)
			{
				gridSearch(step_width, steps, first_rec, k, par1_start, par2_start, opt);
				step_width = step_width/10;
				steps = 20;
				first_rec = 0;
				par1_start = par1-step_width;
				par2_start = par2+step_width;
			}
			
		#ifdef BALL_DEBUG
			cout<<"grid search done!"<<endl;
			cout<<"\tparameter1="<<par1<<"  parameter2="<<par2<<endl;
		#endif
			
		}



		void Kernel::gridSearch(double step_width, int steps, bool first_rec, int k, double par1_start, double par2_start, bool opt)
		{	
			if (par1_start == 0 || (type == 3 && par2_start == 0)) 
			{
				throw Exception::KernelParameterError(__FILE__, __LINE__, "Kernel Parameter start-values for grid search must be not be zero !!"); 
			}
			
			double best_par1 = par1;
			double best_par2 = par2;
			model_->model_val->crossValidation(4, 0);
			double best_cvres = model_->model_val->getCVRes();

			par1 = par1_start;
			par2 = par2_start;
			
			if (type != 3) // for kernels that use only 1 parameter
			{
				if (type == 2 && first_rec)  // exponential decreasing of gamma for RBF kernel
				{
					for (int i = 1; i <= steps; i++)
					{
					#ifdef BALL_DEBUG
						cout<<"kernel-parameter="<<par1<<endl<<flush;
					#endif
						if (!opt || !model_->optimizeParameters(k))
						{
							model_->model_val->crossValidation(k, 0);
						//cout << par1 <<"  "<< model_->model_val->getCVRes()<<endl;	
						}
						if (model_->model_val->getCVRes() > best_cvres)
						{
							best_cvres = model_->model_val->getCVRes();
							best_par1 = par1;
						}	
						par1 = par1/2;
					
					//cout << "-"<<(((double) i)/steps)*100<<"%";
					//cout.flush();
					}
					step_width = par1*2-par1;
				}
				else  // stepwise increasing
				{
					for (int i = 1; i <= steps; i++)
					{
					#ifdef BALL_DEBUG
						cout<<"kernel-parameter="<<par1<<endl<<flush;
					#endif
						//cout <<par1<<endl;
						if (!opt || !model_->optimizeParameters(k))
						{
							model_->model_val->crossValidation(k, 0);
							//cout << par1 <<"  "<< model_->model_val->getCVRes()<<endl;
						}	
						if (model_->model_val->getCVRes() > best_cvres)
						{
							best_cvres = model_->model_val->getCVRes();
							best_par1 = par1;
						}	
						par1 += step_width;
						
						//cout << "-"<<(((double) i)/steps)*100<<"%";
						//cout.flush();
					}
				}
			}
			else  // for kernels that use 2 parameter (sigmoid kernel)
			{
				double start2 = par2;
				
				for (int i = 1; i <= steps; i++)
				{
				#ifdef BALL_DEBUG
					cout<<"kernel-parameter1="<<par1<<endl<<flush;
				#endif
					par2 = start2;
					
					for (int j = 1; j <= steps; j++)
					{
					#ifdef BALL_DEBUG
						cout<<"kernel-parameter2="<<par2<<endl<<flush;
					#endif
						if (!opt || !model_->optimizeParameters(k))
						{
							model_->model_val->crossValidation(k, 0);
							//cout << par1 <<"  "<<par2 <<"  "<<model_->model_val->getCVRes()<<endl;	
						}
						if (model_->model_val->getCVRes() > best_cvres)
						{
							best_cvres = model_->model_val->getCVRes();
							best_par1 = par1;
							best_par2 = par2;
						}
						par2 -= step_width;
					}	
					par1 += step_width;
					
					//cout << "-"<<(((double) i)/steps)*100<<"%";
					//cout.flush();
				}	
			}
			
			par1 = best_par1;
			par2 = best_par2;
			model_->model_val->setCVRes(best_cvres);

		}



		//----------------------------- private functions ---------------



		void Kernel::calculateKernelMatrix1(Eigen::MatrixXd& input, Eigen::MatrixXd& output) // polynomial kernel
		{
			output.resize(input.rows(), input.rows());
			//output = 0;
			for (int i = 0; i < input.rows(); i++)
			{
				for (int j = i; j < input.rows(); j++)
				{	
					// get distance of the two current rows
					double d = Statistics::distance(input, i, j, par1);
					output(i, j) = d;
					output(j, i) = d;
				}
			}
		}


		void Kernel::calculateKernelMatrix1(Eigen::MatrixXd& m1, Eigen::MatrixXd& m2, Eigen::MatrixXd& output) // polynomial kernel
		{
			output.resize(m1.rows(), m2.rows());
			//output = 0;
			for (int i = 0; i < m1.rows(); i++)
			{
				for (int j = 0; j < m2.rows(); j++)
				{	
					output(i, j) = Statistics::distance(m1, m2, i, j, par1);
				}	
			}
		}


		void Kernel::calculateKernelMatrix2(Eigen::MatrixXd& input, Eigen::MatrixXd& output) // radial basis function kernel
		{
			output.resize(input.rows(), input.rows());
			for (int i = 0; i < input.rows(); i++)
			{
				for (int j = i; j < input.rows(); j++)
				{	
					// get distance of the two current rows
					double d = exp(-par1*Statistics::euclDistance(input, input, i, j));
					output(i, j) = d;
					output(j, i) = d;
				}
			}		
		}		
				
				
		void Kernel::calculateKernelMatrix2(Eigen::MatrixXd& m1, Eigen::MatrixXd& m2, Eigen::MatrixXd& output) // radial basis function kernel
		{
			output.resize(m1.rows(), m2.rows());
			for (int i = 0; i < m1.rows(); i++)
			{
				for (int j = 0; j < m2.rows(); j++)
				{	
					output(i, j) = exp(-par1*Statistics::euclDistance(m1, m2, i, j));
				}	
			}
		}		
				
				
		void Kernel::calculateKernelMatrix3(Eigen::MatrixXd& input, Eigen::MatrixXd& output) // sigmoid kernel
		{
			output.resize(input.rows(), input.rows());
			for (int i = 0; i < input.rows(); i++)
			{
				for (int j = i; j < input.rows(); j++)
				{	
					double p = 1;
					double d = tanh(par1*Statistics::distance(input, input, i, j, p)+par2);
					output(i, j) = d;
					output(j, i) = d;
				}
			}
		}		
				
				
		void Kernel::calculateKernelMatrix3(Eigen::MatrixXd& m1, Eigen::MatrixXd& m2, Eigen::MatrixXd& output) // sigmoid kernel
		{
			output.resize(m1.rows(), m2.rows());
			for (int i = 0; i < m1.rows(); i++)
			{
				for (int j = 0; j < m2.rows(); j++)
				{	
					double p = 1;
					output(i, j) = tanh(par1*Statistics::distance(m1, m2, i, j, p)+par2);
				}	
			}
		}		
				
				
		void Kernel::calculateKernelMatrix4(Eigen::MatrixXd& input, Eigen::MatrixXd& output) // individual kernel
		{
			output.resize(input.rows(), input.rows());
			for (int i = 0; i < input.rows(); i++)
			{
				for (int j = i; j < input.rows(); j++)
				{	
					double d = Statistics::distance(input, input, i, j, equation1, equation2);
					output(i, j) = d;
					output(j, i) = d;
				}
			}
		}		
				
				
		void Kernel::calculateKernelMatrix4(Eigen::MatrixXd& m1, Eigen::MatrixXd& m2, Eigen::MatrixXd& output) // individual kernel
		{
			output.resize(m1.rows(), m2.rows());
			for (int i = 0; i < m1.rows(); i++)
			{
				for (int j = 0; j < m2.rows(); j++)
				{	
					output(i, j) = Statistics::distance(m1, m2, i, j, equation1, equation2);
				}	
			}
		}		


		void Kernel::calculateWeightedKernelMatrix(Eigen::MatrixXd& input, Eigen::MatrixXd& output)
		{
			if (input.cols() != weights_.rows())
			{
				throw Exception::KernelParameterError(__FILE__, __LINE__, "Kernel.weights_ has wrong size! One weight for each column of the given matrix is needed in order to be able to calculate a weighted distance matrix!"); 
			}
			
			output.resize(input.rows(), input.rows());
			output.setZero();
			// for all substances, calculate their weighted cross-product
			for (int i = 0; i < input.rows(); i++)
			{
				for (int j = i; j < input.rows(); j++)
				{
					// get distance of the two current rows
					for (int c = 0; c < input.cols(); c++) 
					{
						output(i, j) += weights_(c)*pow(input(i, c)-input(j, c), par1);
					}
					output(j, i) = output(i, j);
				}
			}
		}


		void Kernel::calculateWeightedKernelMatrix(Eigen::MatrixXd& m1, Eigen::MatrixXd& m2, Eigen::MatrixXd& output)
		{
			output.resize(m1.rows(), m2.rows());
			output.setZero();
			// for all substances, calculate their weighted cross-product
			for (int i = 0; i < m1.rows(); i++)
			{
				for (int j = 0; j < m2.rows(); j++)
				{
					// get distance of the two current rows
					for (int c = 0; c < m1.cols(); c++) 
					{
						output(i, j) += weights_(c)*pow(m1(i, c)-m2(j, c), par1);
					}
				}
			}
		}
	}
}
