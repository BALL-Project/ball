// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// 

#include <BALL/QSAR/mlrModel.h>

#include <Eigen/Dense>

namespace BALL
{
	namespace QSAR
	{

		MLRModel::MLRModel(const QSARData& q) : LinearModel(q) 
		{
			type_="MLR";
		}

		MLRModel::~MLRModel()
		{
		}

		void MLRModel::train()
		{	
			if (descriptor_matrix_.cols() == 0)
			{
				throw Exception::InconsistentUsage(__FILE__, __LINE__, "Data must be read into the model before training!"); 
			}
			if (Y_.cols() == 0)
			{
				throw Exception::InconsistentUsage(__FILE__, __LINE__, "No response values have been read! Model can not be trained!");
			}
			if (descriptor_matrix_.cols() >= descriptor_matrix_.rows())
			{	
				throw Exception::SingularMatrixError(__FILE__, __LINE__, "For MLR model, matrix must have more rows than columns in order to be invertible!!");
				//training_result_.ReSize(0, 0);
				//return;
			}

  			Eigen::MatrixXd m = descriptor_matrix_.transpose()*descriptor_matrix_;

				
			try
			{
				training_result_ = m.colPivHouseholderQr().solve(descriptor_matrix_.transpose()*Y_);
			}
			catch(BALL::Exception::GeneralException e)
			{
				training_result_.resize(0, 0);
				throw Exception::SingularMatrixError(__FILE__, __LINE__, "Matrix for MLR training is singular!! Check that descriptor_matrix_ does not contain empty columns!"); 
				return;
			}

			calculateOffsets();
		}
	}
}
