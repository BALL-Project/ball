// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// 

#include <BALL/QSAR/gpModel.h>

#include <Eigen/Dense>

namespace BALL
{
	namespace QSAR
			{

		GPModel::GPModel(const QSARData& q, int k_type, double p1, double p2) : KernelModel(q, k_type, p1, p2) 
		{
			type_="GP";
			lambda_ = 0.001;
		}

		GPModel::GPModel(const QSARData& q, Eigen::VectorXd& w) : KernelModel(q, w) 
		{
			type_="GP";
			lambda_ = 0.001;
		}


		GPModel::GPModel(const QSARData& q, String s1, String s2) : KernelModel(q, s1, s2) 
		{
			type_="GP";
			lambda_ = 0.001;
		}

		GPModel::GPModel(const QSARData& q, const LinearModel& lm, int column) : KernelModel(q, lm, column) 
		{
			type_="GP";
			lambda_ = 0.001;
		}

		GPModel::~GPModel()
		{
		}


		void GPModel::train()
		{
			if (descriptor_matrix_.cols() == 0)
			{
				throw Exception::InconsistentUsage(__FILE__, __LINE__, "Data must be read into the model before training!"); 
			}
			kernel->calculateKernelMatrix(descriptor_matrix_, K_);
			
			training_result_ = K_.colPivHouseholderQr().solve(Y_); // B: one coefficient for each substance; dim: nxc
			
			calculateOffsets();
		}


		Eigen::VectorXd GPModel::predict(const vector<double> & substance, bool transform)
		{
			if (training_result_.cols() == 0)
			{
				throw Exception::InconsistentUsage(__FILE__, __LINE__, "Model must be trained before it can predict the activitiy of substances!"); 
			}
			input_ = getSubstanceVector(substance, transform); 
			
			kernel->calculateKernelVector(K_, input_, descriptor_matrix_, K_t_); // dim: 1xn
			
			Eigen::VectorXd res = K_t_*training_result_;
			//if (offsets_.getSize() == res.getSize()) res -= offsets_; 
			
			if (transform && y_transformations_.cols() != 0)
			{
				backTransformPrediction(res); 
			}
			return res;	
		}


		double GPModel::calculateStdErr()
		{
			Eigen::MatrixXd mx;
			Eigen::MatrixXd m1(1, input_.rows());
			m1.row(1) = input_;
			kernel->calculateKernelMatrix(m1, mx); // k(x*, x*), dim: 1x1
			
			double sum = 0;
			for (unsigned int i = 1; i <= K_t_.cols(); i++)
			{
				for (unsigned int j = 1; j <= K_t_.cols(); j++)
				{
					sum += K_t_(i)*K_t_(j)*L_(i, j); // k(x*, _i)*k(x*, x_j)*L_ij
				}
			}
			double res = sqrt(abs(mx(1, 1)-sum));
			return res;	
		}


		void GPModel::setParameters(vector<double>& v)
		{
			if (v.size() != 1)
			{
				String c = "Wrong number of model parameters! Needed: 1;";
				c = c+" given: "+String(v.size());
				throw Exception::ModelParameterError(__FILE__, __LINE__, c.c_str());
			}
			lambda_ = v[0];
		}

		vector<double> GPModel::getParameters() const
		{
			vector<double> d;
			d.push_back(lambda_);
			return d;
		}
	}
}
