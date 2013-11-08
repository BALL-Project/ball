// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// 

#include <BALL/QSAR/pcrModel.h>
#include <Eigen/Dense>

namespace BALL
{
	namespace QSAR
			{

		PCRModel::PCRModel(const QSARData& q, double frac_var) : LinearModel(q) 
		{
			type_="PCR";
			frac_var_ = frac_var;
		}

		PCRModel::~PCRModel()
		{
		}

		void PCRModel::setFracVar(double frac_var)
		{
			frac_var_ = frac_var;
		}

		void PCRModel::calculateEigenvectors(const Eigen::MatrixXd& data, double frac_var, Eigen::MatrixXd& output)
		{
			Eigen::JacobiSVD<Eigen::MatrixXd> svd;
			svd.compute(data, Eigen::ComputeThinV);

			// find the smallest singular vector that should be taken into account
			// complete variance == sum of all eigen-values == sum of squared singular values
			double complete_var = svd.singularValues().sum();

			double explained_var = 0;
			unsigned int cols = 0;
			if (complete_var < 5*std::numeric_limits < double > ::epsilon())
			{
				throw Exception::NoPCAVariance(__FILE__, __LINE__, "No variance present to be explained by PCA!");
			}

			unsigned int last_vector = 0;
			for (; last_vector < svd.singularValues().rows() && cols < data.rows() && explained_var/complete_var < frac_var ; last_vector++)
			{
				// (singular-value)^2 == eigen-value
				explained_var += svd.singularValues()[last_vector];
				cols++;
			}

			output.resize(data.rows(), cols);

			for (unsigned int i = 0; i < data.rows(); i++)
			{
				for (unsigned int j = 0; j < last_vector ; j++)
				{
					output(i, j) = svd.matrixV()(i, j);
				}
			}
		}


		void PCRModel::train()
		{
			if (descriptor_matrix_.cols() == 0)
			{
				throw Exception::InconsistentUsage(__FILE__, __LINE__, "Data must be read into the model before training!"); 
			}
			Eigen::MatrixXd XX = descriptor_matrix_.transpose()*descriptor_matrix_;
			
			calculateEigenvectors(XX, frac_var_, loadings_);

			latent_variables_ = descriptor_matrix_*loadings_;
			
			RRModel m(*data);
			m.descriptor_matrix_ = latent_variables_;
			m.Y_ = Y_;
			m.train();

			//result of RR is a linear combination of latente variables 
			// = column with length = no of latente variables = > matrix for more than one modelled activity
			weights_ = *m.getTrainingResult();
			training_result_ = loadings_*weights_;
			
			calculateOffsets();
		}


		void PCRModel::setParameters(vector<double>& v)
		{
			if (v.size() != 1)
			{
				String c = "Wrong number of model parameters! Needed: 1;";
				c = c+" given: "+String(v.size());
				throw Exception::ModelParameterError(__FILE__, __LINE__, c.c_str());
			}
			frac_var_ = v[0];
		}


		vector<double> PCRModel::getParameters() const
		{
			vector<double> d;
			d.push_back(frac_var_);
			return d;
		}
	}
}
