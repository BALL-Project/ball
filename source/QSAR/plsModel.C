// -*- Mode: weights_++; tab-width: 2; -*-
// vi: set ts=2:
//
//

#include <BALL/QSAR/plsModel.h>

#include <Eigen/Dense>

namespace BALL
{
	namespace QSAR
	{
				
		PLSModel::PLSModel(const QSARData& q) : LinearModel(q) 
		{
			type_="PLS";
			no_components_ = 10;
		}

		PLSModel::~PLSModel()
		{
		}

		int PLSModel::getNoComponents()
		{
			return no_components_;
		}

		void PLSModel::setNoComponents(int no)
		{
			no_components_ = no;
		}
			

		void PLSModel::train()
		{	
			if (descriptor_matrix_.cols() == 0)
			{
				throw Exception::InconsistentUsage(__FILE__, __LINE__, "Data must be read into the model before training!"); 
			}
			int cols = descriptor_matrix_.cols();
			
			Eigen::MatrixXd X = descriptor_matrix_;
			Eigen::MatrixXd P;  // Matrix P saves all vectors p
			
			// determine the number of components that are to be created.
			// no_components_ contains the number of components desired by the user, 
			// but obviously we cannot calculate more PLS components than features
			unsigned int components_to_create = no_components_;
			if (cols < no_components_) components_to_create = cols; 
			
			U_.resize(X.rows(), components_to_create);
			loadings_.resize(cols, components_to_create);
			weights_.resize(Y_.cols(), components_to_create);
			latent_variables_.resize(descriptor_matrix_.rows(), components_to_create);
			P.resize(cols, components_to_create);

			Eigen::VectorXd w;
			Eigen::VectorXd t;
			Eigen::VectorXd c;
			Eigen::VectorXd u = Y_.col(0);
			Eigen::VectorXd u_old;
			
			for (unsigned int j = 0; j < components_to_create; j++)
			{
				for (int i = 0; i < 10000; i++)
				{	
					w = X.transpose()*u / Statistics::scalarProduct(u);
					w = w / Statistics::euclNorm(w);
					t = X*w;
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
				
				X -= t * p.transpose();
			
				U_.col(j) = u;
				loadings_.col(j) = w;
				weights_.col(j) = c;
				P.col(j) = p;
				latent_variables_.col(j) = t;
			}

		// 	try  // p's are not orthogonal to each other, so that in rare cases P.t()*loadings_ is not invertible
		// 	{
		// 		loadings_ = loadings_*(P.t()*loadings_).i();
		// 	}
		// 	catch(BALL::Exception::MatrixIsSingular e)
		// 	{
		// 		Eigen::MatrixXd I; I.setToIdentity(P.cols());
		// 		I *= 0.0001;
		// 		loadings_ = loadings_*(P.t()*loadings_+I).i();
		// 	}

			training_result_ = loadings_*(P.transpose()*loadings_).jacobiSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(weights_.transpose());

			calculateOffsets();
		}


		bool PLSModel::optimizeParameters(int k, int no_steps)
		{
			double best_q2 = 0;
			int best_no = 1;
			for (int i = 1; i <= no_steps && i <= (int)data->getNoDescriptors() && (descriptor_IDs_.empty() || i <= (int)descriptor_IDs_.size()); i++)
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


		const Eigen::MatrixXd* PLSModel::getU()
		{ 
			return &U_;
		}


		void PLSModel::setParameters(vector<double>& v)
		{
			if (v.size() != 1)
			{
				String c = "Wrong number of model parameters! Needed: 1;";
				c = c+" given: "+String(v.size());
				throw Exception::ModelParameterError(__FILE__, __LINE__, c.c_str());
			}
			no_components_ = (int) v[0];
		}

		vector<double> PLSModel::getParameters() const
		{	
			vector<double> v;
			v.push_back(no_components_);
			return v;
		}
	}
}
