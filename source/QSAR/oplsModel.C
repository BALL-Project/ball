// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//

#include <BALL/QSAR/oplsModel.h>
#include <BALL/QSAR/statistics.h>
 
namespace BALL
{
	namespace QSAR
			{

		OPLSModel::OPLSModel(const QSARData& q) : PLSModel(q) 
		{ 
			type_="OPLS";
			no_ortho_components_ = 5;
		}

		OPLSModel::~OPLSModel()
		{
		}

		void OPLSModel::train()
		{
			Eigen::MatrixXd X = descriptor_matrix_;
			
			
		// 	double d[] = {-1, -1, 1, -1, -1, 1, 1, 1};
		// 	//double d[] = {-2.18, -2.18, 1.84, -0.16, -0.48, 1.52, 0.83, 0.83};
		// 	Matrix X(4, 2);
		// 	X << d;
		// 	Y_.ReSize(4, 1);
		// 	double e[] = {2, 2, 0, -4};
		// 	Y_ << e;
			int cols = descriptor_matrix_.cols();
			
			Eigen::VectorXd w;
			Eigen::VectorXd t;
			Eigen::VectorXd c;
			Eigen::VectorXd u(X.rows());
			
			for (unsigned int i = 1; i <= u.rows(); i++)
			{
				u(i) = Y_(i, 1);
			}
			Eigen::VectorXd u_old;
			//w = X.t()*u / Statistics::scalarProduct(u);
			//w = w / Statistics::euclNorm(w);	
			
			// determine the number of orthogonal components that are to be created.
			// no_ortho_components_ contains the number of components desired by the user, 
			// but obviously the number of orthogonal components must be <= #features-1
			// since we will need at least one non-orthogonal component as well
			unsigned int orthogonal_components_to_create = no_ortho_components_;
			if (cols-1 < no_ortho_components_) orthogonal_components_to_create = cols-1; 
			
			W_ortho_.resize(cols, orthogonal_components_to_create);
			T_ortho_.resize(descriptor_matrix_.rows(), orthogonal_components_to_create);

			for (unsigned int j = 0; j < orthogonal_components_to_create; j++)
			{	
				for (int i = 0; ; i++)
				{
					w = X.transpose()*u / Statistics::scalarProduct(u);
					w = w / Statistics::euclNorm(w);	
					
					t = X*w ;
					c = Y_.transpose()*t / Statistics::scalarProduct(t);
					u_old = u;
					u = Y_*c / Statistics::scalarProduct(c);
					
					if (Statistics::euclDistance(u, u_old)/Statistics::euclNorm(u) > 0.0000001) 
					{ 
						continue;				
					}
					else  // if u has converged
					{
						break;
					}
				}
			
				Eigen::VectorXd p = X.transpose()*t / Statistics::scalarProduct(t);
				Eigen::VectorXd w_ortho = w*((w.transpose()*p) / (w.transpose()*w));
				w_ortho = p-w_ortho;
				w_ortho = w_ortho / Statistics::euclNorm(w_ortho);
				Eigen::VectorXd t_ortho = X*w_ortho / Statistics::scalarProduct(w_ortho); 
				Eigen::VectorXd p_ortho = X.transpose()*t_ortho / Statistics::scalarProduct(t_ortho);

				X -= t_ortho * p_ortho.transpose();
	
				W_ortho_.col(j+1) = w_ortho;
				T_ortho_.col(j+1) = t_ortho;
			}
				
			descriptor_matrix_ = X;
			PLSModel::train();
		}


		const Eigen::MatrixXd* OPLSModel::getTOrtho()
		{ 
			return &T_ortho_;
		}


		const Eigen::MatrixXd* OPLSModel::getWOrtho()
		{
			return &W_ortho_;
		}


		bool OPLSModel::optimizeParameters(int k, int no_steps)
		{
			double best_q2 = 0;
			int best_no = 1;
			int best_o_no = 1;
			int cols = data->getNoDescriptors();
			if (!descriptor_IDs_.empty())
			{
				cols = descriptor_IDs_.size();
			}
			no_ortho_components_ = 0;
			
			// first find best number of components, for PLS analysis 
			for (int i = 1; i <= no_steps && i <= cols; i++)
			{	
				no_components_ = i;
					
				validation->crossValidation(k);
			
				if (validation->getQ2() > best_q2)
				{
					best_q2 = validation->getQ2();
					best_no = i;
				}
				else if (validation->getQ2() < 0.75*best_q2)
				{
					break; // for speed-up
				}
			}
			no_components_ = best_no;
			
			//then try to remove orthogonal variance by use of OPLS components
			for (int i = 0; i <= no_steps && i <= cols && i < no_components_; i++)
			{	
				no_ortho_components_ = i;
					
				validation->crossValidation(k);
			
				if (validation->getQ2() > best_q2)
				{
					best_q2 = validation->getQ2();
					best_o_no = i;
				}
				else if (validation->getQ2() < 0.75*best_q2)
				{
					break; // for speed-up
				}
			}
			no_ortho_components_ = best_o_no;

			//finally, try to reduce the number of PLS components (since orthogonal variance has been removed, less PLS components should be neccessary)
			for (int i = no_components_; i >= 1 ; i--)
			{	
				no_components_ = i;
					
				validation->crossValidation(k);
			
				if (validation->getQ2() > best_q2)
				{
					best_q2 = validation->getQ2();
					best_no = i;
				}
				else if (validation->getQ2() < 0.75*best_q2)
				{
					break; // for speed-up
				}
			}
			no_components_ = best_no;
			
			validation->setQ2(best_q2);
			
			return 1;
		}


		int OPLSModel::getNoOrthoComponents()
		{
			return no_ortho_components_;
		}
						
						
		void OPLSModel::setNoOrthoComponents(int d)
		{
			no_ortho_components_ = d;
		}


		void OPLSModel::setParameters(vector<double>& v)
		{
			if (v.size() != 2)
			{
				String c = "Wrong number of model parameters! Needed: 2;";
				c = c+" given: "+String(v.size());
				throw Exception::ModelParameterError(__FILE__, __LINE__, c.c_str());
			}
			no_ortho_components_ = (int)v[0];
			no_components_ = (int)v[1];
		}


		vector<double> OPLSModel::getParameters() const
		{
			vector<double> d;
			d.push_back(no_ortho_components_);
			d.push_back(no_components_);
			return d;
		}

		// RowVector OPLSModel::predict(const vector<double> & substance, bool transform)
		// {
		// 	if (training_result_.cols() == 0)
		// 	{
		// 		throw Exception::InconsistentUsage(__FILE__, __LINE__, "Model must be trained before it can predict the activitiy of substances!"); 
		// 	}
		// 
		// 	RowVector v = getSubstanceVector(substance, transform); 
		// 	
		// // 	ColumnVector t_new_ortho = v*W_ortho_.Column(1) / (W_ortho_.Column(1).t()*W_ortho_.Column(1)).AsScalar();
		// // 	v = v - t_new_ortho*P_ortho.Column(1).t();
		// 	
		// 	RowVector res = v*training_result_;
		// 
		// 	if (transform && y_transformations_.cols() != 0)
		// 	{
		// 		backTransformPrediction(res); 
		// 	}
		// 
		// 	return res;
		// }
	}
}
