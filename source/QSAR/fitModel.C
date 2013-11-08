// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//

#include <BALL/QSAR/fitModel.h>
#include <BALL/COMMON/exception.h>

using namespace std;

namespace BALL
{
	namespace QSAR
	{

		FitModel::FitModel(const QSARData& q) : NonLinearModel(q) 
		{
			diffEquations = NULL; 
		}

		FitModel::FitModel(const QSARData& q, vector<String>& eq, vector<double>& guess) : NonLinearModel(q) 
		{
			diffEquations = NULL; 
			initial_guess_ = guess;
			allEquations_ = eq;
		}

		FitModel::FitModel(const QSARData& q, vector<String>& eq, vector<vector<String> >& deq, vector<double>& guess) : NonLinearModel(q) 
		{
			allDiffEquations_ = deq; 
			initial_guess_ = guess;
			allEquations_ = eq;
		}


		Eigen::VectorXd FitModel::predict(const vector<double> & substance, bool transform)
		{
			if (training_result_.cols() == 0)
			{
				throw Exception::InconsistentUsage(__FILE__, __LINE__, "Model must be trained before it can predict the activitiy of substances!"); 
			}
			Eigen::VectorXd v = getSubstanceVector(substance, transform); 
			Eigen::VectorXd res(Y_.cols());
			
			String var="";
			// replace all x-values for the current substance
			for (unsigned int j = 0; j < v.rows(); j++)
			{
				var = var+"x"+String(j)+"="+String(v(j))+";";
			}
			
			//calculated all activities for given substance
			for (int c = 0; c < Y_.cols(); c++)
			{
				String coeff="";
				// get optimized coefficients
				for (int m = 0; m < training_result_.rows(); m++)
				{
					coeff = coeff+"b"+String(m)+"="+String(training_result_(m, c))+";";
				}
				ParsedFunction<float> f = coeff+var+allEquations_[c];
				res(c) = f(0);
			}
			
			if (transform && y_transformations_.cols() != 0)
			{
				backTransformPrediction(res); 
			}
			return res;	
		}


		void FitModel::setEquations(vector<String>& eq, vector<vector<String> >& deq)
		{
		// 	if (deq.size() != eq.size())
		// 	{
		// 		cout << "Derivatives for all equations are needed!"<<endl; return; 
		// 	}
		// 	if (eq.size() != Y_.cols())
		// 	{
		// 		cout << "An equation (and their each derivatives) is needed for each column of Y_. \nPlease either specify all equations or change matrix Y_."<<endl; return; 
		// 	}
		//  	if (deq[0].size() != descriptor_IDs_.size())
		//  	{
		//  		cout << "Equations with all variables ( = descriptors) of this model are needed! Please either specify correct equations or change descriptor_matrix_."<<endl; return; 
		//  	}
			allEquations_ = eq;
			allDiffEquations_ = deq; 
		}


		void FitModel::setEquations(vector<String>& eq)
		{
		// 	if (eq.size() != Y_.cols())
		// 	{
		// 		cout << "An equation is needed for each column of Y_. \nPlease either specify all equations or change matrix Y_."<<endl; return; 
		// 	}
			allEquations_ = eq;
			diffEquations = NULL; 
		}


		void FitModel::setInitialGuess(vector<double>& d)
		{
			initial_guess_ = d;
		}


		void FitModel::train()
		{	
			if (descriptor_matrix_.cols() == 0)
			{
				throw Exception::InconsistentUsage(__FILE__, __LINE__, "Data must be read into the model before training!"); 
			}
			if (allEquations_.empty())
			{
				cout<<"ERROR: No equations specified! Use method setEquations first."<<endl; 
				return;
			}
				
			training_result_.resize(descriptor_matrix_.cols(), Y_.cols());
			
			for (c = 0; c < (unsigned int)Y_.cols(); c++)
			{	
 				fitY = new Eigen::MatrixXd(Y_.rows(), 1);
				for (int n = 0; n < Y_.rows(); n++)
				{
					(*fitY)(n, 0) = Y_(n, c);
				}
				
				fitX = &descriptor_matrix_;
				equation = &allEquations_[c];
				
				if (allDiffEquations_.size() < c)
				{
					diffEquations = &allDiffEquations_[c]; 
				}
				else
				{
					diffEquations = NULL; 
				}
					
				const gsl_multifit_fdfsolver_type* T = gsl_multifit_fdfsolver_lmsder; 
				gsl_multifit_fdfsolver* s = gsl_multifit_fdfsolver_alloc(T, fitX->rows(), fitX->cols()); 
				
				const size_t n = descriptor_matrix_.rows();
				const size_t p = descriptor_matrix_.cols();
				gsl_multifit_function_fdf fdf; 
						
				fdf = make_fdf(&setF, &setDf, &setFdf, n, p, 0);
				
				double* g = new double[initial_guess_.size()];
				for (unsigned int m = 0; m < initial_guess_.size(); m++)
				{
					g[m] = initial_guess_[m];
				}
				
				gsl_vector_view ini = gsl_vector_view_array (g, p);
				
				gsl_multifit_fdfsolver_set(s, &fdf, &ini.vector); 
			
				int status;
				
				for (unsigned int i = 0; i < 50; i++)
				{
					status = gsl_multifit_fdfsolver_iterate(s); 
				}
				
				// save the predicted coefficients
				for (unsigned int m = 0; m < s->x->size; m++)
				{
					training_result_(m, c) = gsl_vector_get(s->x, m);
				}
				
				delete fitY;
				delete [] g;
				gsl_multifit_fdfsolver_free(s); 
			}
			cout <<training_result_<<endl;
		}


		double getFunctionValue(double X, void* /*params*/)
		{
			return (*f)(X);	
		}


		int setF(const gsl_vector* x, void* /*params*/, gsl_vector* f)
		{
			ParsedFunction<float> f0 = (*equation);
			f0(0);
			String constants="";
			
			// replace all constants by their current values
			for (unsigned int m = 0; m < (f0.constants_.size()-1)/2; m++)
			{
				constants = constants+"b"+String(m)+"="+String(gsl_vector_get(x, m))+";";
			}
			
 			for (int n = 0; n < fitX->rows(); n++)
			{
				String var="";
				// replace all x-values for the current substance
				for (int m = 0; m < fitX->cols(); m++)
				{
					var = var+"x"+String(m)+"="+String((*fitX)(n, m))+";";
				}
				
				// evaluate function for current substance
				ParsedFunction<double> f1 = constants+var+(*equation);
				double fn = f1(0);
				gsl_vector_set(f, n, pow((*fitY)(n, 0)-fn, 2));
			}
			
			return GSL_SUCCESS;
		}


		int setDf(const gsl_vector * x, void* /*params*/, gsl_matrix * df)
		{
			ParsedFunction<float> f0 = (*equation);
			f0(0);
			
			
			if (diffEquations != NULL)
			{
				String constants="";
				// replace all constants by their current values
				for (unsigned int i = 0; i < (f0.constants_.size()-1)/2; i++)
				{
					constants = constants+"b"+String(i)+"="+String(gsl_vector_get(x, i))+";";
				}
				
				for (int i = 0; i < fitX->rows(); i++) // for each substance...
				{
					String var="";
					// replace all x-variables for the current substance
					for (int j = 0; j < fitX->cols(); j++)
					{
						var = var+"x"+String(j)+"="+String((*fitX)(i, j))+";";
					}
				
					String y="";
					// replacements for y_i
					for (int j = 0; j < fitY->cols(); j++)
					{
						y = y+"y"+String(j)+"="+String((*fitY)(i, j))+";";
					}
				
					// evaluate all differential derivatives for current substance
					for (int j = 0; j < fitX->cols(); j++)
					{
						ParsedFunction < double > f1 = constants+var+y+(*diffEquations)[j]; 
						double dfi = f1(0);
						gsl_matrix_set (df, i, j, dfi); // set value of part. der. of current substance
					}
				}
					
				return GSL_SUCCESS;
			}
			
			else  /** use numerical derivation if no derivatives are specified: */
			{
				gsl_function F;
				F.function = &getFunctionValue;
				F.params = 0;
				
				for (int i = 0; i < fitX->rows(); i++) // for all substances...
				{
					String y="";
					// replacements for y_i
					for (int j = 0; j < fitY->cols(); j++)
					{
						y = y+"y"+String(j)+"="+String((*fitY)(i, j))+";";
					}
					
					String var="";	
					for (int j = 0; j < fitX->cols(); j++)
					{
						var = var+"x"+String(j)+"="+String((*fitX)(i, j))+";";
					}
				
					// evaluate all differential equations for current substance
					for (int j = 0; j < fitX->cols(); j++)
					{
						String coeff="";
						for (int k = 0; k < fitX->cols(); k++)
						{
							if (k != j)
							{
								coeff = coeff+"b"+String(k)+"="+String(gsl_vector_get(x, k))+";";
							}
							else
							{
								coeff = coeff+"b"+String(k)+"="+"X;";
							}
						}
						
						String es = coeff+var+y+"("+(*equation);
						es = es.getSubstring(0, es.size()-1);
						es = es + ")^2-y"+String(c)+";";
									
						f = new ParsedFunction<double>(es);
						double dfi; double abserr;
						gsl_deriv_central (&F, (*fitX)(i, j), 1e-8, &dfi, &abserr); // finds value of \delta f / \delta x_j

						if (i == 5) {cout<<"f(0) = "<<(*f)(0)<<endl; 
							cout<<es<<"  "<<dfi<<endl;}
						gsl_matrix_set (df, i, j, dfi);
						delete f;
					}
				}
			
				return GSL_SUCCESS;
			}
		}


		gsl_multifit_function_fdf
		make_fdf (int (* f) (const gsl_vector *, void *, gsl_vector *), 
							int (* df) (const gsl_vector *, void *, gsl_matrix *), 
							int (* fdf) (const gsl_vector *, void *, gsl_vector *, gsl_matrix *), 
							size_t n, 
							size_t p, void * params)
		{
			gsl_multifit_function_fdf F_new; 
			F_new.f = f;
			F_new.df = df;
			F_new.fdf = fdf;
			F_new.n = n;
			F_new.p = p;
			F_new.params = params;
			return F_new;
		}



		int setFdf(const gsl_vector * x, void *params, gsl_vector * f, gsl_matrix * df)
		{	
			BALL::QSAR::setF(x, params, f);
			BALL::QSAR::setDf(x, params, df);

			return GSL_SUCCESS;
		}

	}
}
