// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// 
#include <BALL/QSAR/validation.h>
#include <BALL/QSAR/statistics.h>
#include <BALL/QSAR/Model.h>

#include <boost/random/mersenne_twister.hpp>

namespace BALL
{
	namespace QSAR
	{

		Validation::Validation(Model* m)
		{
			model_ = m;
			validation_statistic_ = 0;
			yRand_results_.resize(0, 0);
		}

		Validation::~Validation()
		{
		}

		int Validation::getStat() const
		{
			return validation_statistic_;
		}

		void Validation::setTrainingLine(int train_line, int current_line)
		{	
			bool fs = 0; // has feature selection being done?
			if (!model_->descriptor_IDs_.empty())
			{
				fs = 1;
			}
			std::multiset<unsigned int>::iterator it = model_->descriptor_IDs_.begin();

			int t = 0; // index in line of training data
			
			 // set each cell of the current line
			for (unsigned int i = 0; i < model_->data->descriptor_matrix_.size() && (!fs || it != model_->descriptor_IDs_.end()); i++)
			{
				// set only those cells that belong to selected descriptors
				if ( (fs && *it == i) || !fs )
				{	
		// 			if (train_line == 0) 
		// 			{	
		// 				model_->transformations(1, t+1) = model_->data->transformations[i][0]; 
		// 				model_->transformations(2, t+1) = model_->data->transformations[i][1]; 
		// 			}
					model_->descriptor_matrix_(train_line, t) = model_->data->descriptor_matrix_[i][current_line];
					t++;
					if (fs)
					{
						it++;
					}
				}
			}
			// set all y-values for current substance
		//	int a = model_->data->transformations.size() - model_->data->Y_.size(); 
			for (unsigned int i = 0; i < model_->data->Y_.size(); i++)
			{
				model_->Y_(train_line, i) = model_->data->Y_[i][current_line];
		// 		if (train_line == 0)
		// 		{
		// 			transformations(1, col+i+1) = model_->data->transformations[a+i][0]; 
		// 			transformations(2, col+i+1) = model_->data->transformations[a+i][1]; 
		// 		}
			}
		}



		void Validation::setTestLine(int test_line, int current_line, bool back_transform)
		{	
			vector<double> v;
			test_substances_[test_line] = v;

			// COPY ENTIRE LINE!!, relevant descriptors will be automatically chosen by Model.getSubstanceVector(...) (called by Model.predict(...))
			for (unsigned int i = 0; i < model_->data->descriptor_matrix_.size(); i++)
			{
				test_substances_[test_line].push_back(model_->data->descriptor_matrix_[i][current_line]);
				if (back_transform)
				{
					double stddev = model_->data->descriptor_transformations_[i][1]; 
					test_substances_[test_line][i] = test_substances_[test_line][i]*stddev+model_->data->descriptor_transformations_[i][0]; 
				}
			}
			
			if (model_->data->y_transformations_.size() == 0)
			{
				back_transform = 0; 
			}
			
			// set all y-values for current substance
			for (unsigned int i = 0; i < model_->data->Y_.size(); i++)
			{	
				test_Y_(test_line, i) = model_->data->Y_[i][current_line];
				if (back_transform)
				{
					double stddev = model_->data->y_transformations_[i][1]; 
					test_Y_(test_line, i) = test_Y_(test_line, i)*stddev+model_->data->y_transformations_[i][0]; 
				}
			}
		}


		void Validation::yRand()
		{
			boost::mt19937 rng(PreciseTime::now().getMicroSeconds());
			
			QSARData* data = const_cast <QSARData*> (model_->data);
			
			for (unsigned int i = 0; i < data->Y_.size(); i++)
			{
				for (unsigned int j = 0; j < data->Y_[0].size(); j++)
				{	
					int pos = rng() % (data->Y_[0].size()-1);  // exchange elements at pos and j
					double tmp = data->Y_[i][pos];
					data->Y_[i][pos] = data->Y_[i][j];
					data->Y_[i][j] = tmp;
				}
			}
		}


		const Eigen::MatrixXd& Validation::getYRandResults() const
		{
			return yRand_results_;
		}
	}
}
