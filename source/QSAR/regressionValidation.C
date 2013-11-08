// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// 
#include <BALL/QSAR/regressionValidation.h>
#include <BALL/QSAR/statistics.h>
#include <BALL/QSAR/regressionModel.h>
#include <BALL/QSAR/kernelModel.h>
#include <BALL/QSAR/latentVariableModel.h>
#include <BALL/QSAR/registry.h>

#include <boost/random/mersenne_twister.hpp>

using namespace std;

namespace BALL
{
	namespace QSAR
	{

		RegressionValidation::RegressionValidation(RegressionModel* m) : Validation(m)
		{
			ssR_ = 0; ssE_ = 0; ssY_ = 0; Q2_ = -1; F_cv_ = -1; R2_ = -1; std_err_ = -1; F_regr_ = -1; max_error_ = -1;
			regr_model_ = m;
			predQualFetcher_ = &RegressionValidation::getQ2;
			fitQualFetcher_ = &RegressionValidation::getR2;
			selectStat(0); // choose standard == Q^2
		}

		RegressionValidation::~RegressionValidation()
		{
		}

		void RegressionValidation::setQ2(double d)
		{
			Q2_ = d;
		}

		void RegressionValidation::crossValidation(int k, bool restore)
		{
			crossValidation(k, NULL, restore);
		}


		void RegressionValidation::backupTrainingResults()
		{
			backup_data_.descriptor_matrix = regr_model_->descriptor_matrix_;
			backup_data_.training_result = regr_model_->training_result_;
			backup_data_.Y = regr_model_->Y_;
			
			KernelModel* k_model = dynamic_cast<KernelModel*>(regr_model_);
			LatentVariableModel* lv_model = dynamic_cast<LatentVariableModel*>(regr_model_);
			if (k_model)
			{
				backup_data_.K = k_model->K_;
			}
			if (lv_model)
			{
				backup_data_.latent_variables = *lv_model->getLatentVariables();
				backup_data_.loadings = *lv_model->getLoadings();
				backup_data_.weights = *lv_model->getWeights();
			}	
		}


		void RegressionValidation::restoreTrainingResults()
		{
			regr_model_->descriptor_matrix_ = backup_data_.descriptor_matrix;
			regr_model_->training_result_ = backup_data_.training_result;
			regr_model_->Y_ = backup_data_.Y;
			
			KernelModel* k_model = dynamic_cast<KernelModel*>(regr_model_);
			LatentVariableModel* lv_model = dynamic_cast<LatentVariableModel*>(regr_model_);
			if (k_model)
			{
				k_model->K_ = backup_data_.K;
			}
			if (lv_model)
			{
				lv_model->latent_variables_ = backup_data_.latent_variables;
				lv_model->loadings_ = backup_data_.loadings;
				lv_model->weights_ = backup_data_.weights;
			}
		}


		void RegressionValidation::crossValidation(int k, MatrixVector* results, bool restore)
		{
			if (model_->data->descriptor_matrix_.size() == 0 || model_->data->Y_.size() == 0)
			{
				throw Exception::InconsistentUsage(__FILE__, __LINE__, "Data must be fetched from input-files by QSARData before cross-validation can be done!"); 
			}
			
			if (restore) backupTrainingResults(); 
			
			int lines = model_->data->descriptor_matrix_[0].size();
			int col = model_->data->descriptor_matrix_.size();
			if (!model_->descriptor_IDs_.empty())
			{
				col = model_->descriptor_IDs_.size();
			}
			Q2_ = 0; ssE_ = 0; ssR_ = 0; F_cv_ = 0;

			// test k times
			for (int i = 0; i < k; i++)
			{	
				int test_size = (lines+i)/k;
				int training_size = lines-test_size;
				model_->Y_.resize(training_size, model_->data->Y_.size());
				model_->descriptor_matrix_.resize(training_size, col); 
				test_substances_.resize(test_size);
				test_Y_.resize(test_size, model_->data->Y_.size());
				
				int train_line = 0;  // no of line in descriptor_matrix_ of model_
				int test_line = 0;
				
				//copy data to training and test data set
				for (int line = 0; line < lines; line++)
				{
					if ((line+1+i)%k == 0)
					{
						setTestLine(test_line, line);
						test_line++;
					}
					else
					{
						setTrainingLine(train_line, line);
						train_line++;
					}
					
				}
				// test Model with model_->predict() for each line of test-data
				model_->train();
				if (results != NULL){ results->push_back(*regr_model_->getTrainingResult()); }
				testAllSubstances(0); 	  // do not transform cross-validation test-data again...
				Q2_ += quality_;
			}
			Q2_ = Q2_/k;
		//	F_cv_ = gsl_cdf_fdist_P((Q2_*(lines-col-1))/((1-Q2_)*col), col, lines-col-1);
			
			std_err_ = std_err_ / ((k-1)*lines);
			
			if (restore) restoreTrainingResults(); 
		}


		void RegressionValidation::testAllSubstances(bool transform)
		{	
			quality_ = 0; ssE_ = 0; ssR_ = 0; std_err_ = 0; ssY_ = 0;
			
			Eigen::VectorXd mean_Y(test_Y_.cols()); // mean of each activity
			//RowVector sum_of_squares(test_Y_.cols());
			
			/// In case of external test data (for which 'transform' == 1), data in test_Y_ has been backtransformed to original space and model_->predict(.., 1) will return the activity value in original space.
			/// In case of internal testing ('transform' == 0), data in test_Y_ is in transformed space and model->predict(.., 0) will return the activity value in the same space.
			
			for (int i = 0; i < test_Y_.cols(); i++)
			{
				mean_Y(i) = Statistics::getMean(test_Y_, i);	
			}
			//ssT_ =  sum_of_squares.Sum();
			
			for (unsigned int i = 0; i < test_substances_.size(); i++)
			{       
				Eigen::VectorXd rv = model_->predict(test_substances_[i], transform); 
				double error = 0;
				for (int k = 0; k < test_Y_.cols(); k++)
				{
					error += pow(test_Y_(i, k)-rv(k), 2);
					ssR_ += pow(mean_Y(k)-rv(k), 2);
					ssY_ += pow(mean_Y(k)-test_Y_(i, k), 2);
				}
				if (error > max_error_)
				{
					max_error_ = error;
				}
				ssE_ += error;
				
		// 		if (model_->type_ == "GP")
		// 		{
		// 			std_err_ += model_->calculateStdErr();
		// 		}
			}
			
			(this->*qualCalculation)();
			
		/*	if (model_->type_ == "GP")
			{
				std_err_ = std_err_/test_substances_.size();
			}*/	
		}


		void RegressionValidation::testInputData(bool transform)
		{ 
			if (model_->data->descriptor_matrix_.size() == 0)
			{
				throw Exception::InconsistentUsage(__FILE__, __LINE__, "Data must be fetched from input-files by QSARData before a model_'s fit to it can be evaluated!"); 
			}
			
			if (model_->type_ != "ALL" && model_->type_ != "SVR" && model_->type_ != "KNN") // do not check dimensions of traning results for ALL, since ALL does no training
			{
				//unsigned int des = model_->descriptor_IDs_.size();
				//unsigned int data_cols = model_->data->descriptor_matrix_.size();
				unsigned int res_rows = regr_model_->training_result_.rows();
				//unsigned int desmat_cols = model_->descriptor_matrix_.cols();
				
				if (res_rows == 0)
				{
					throw Exception::InconsistentUsage(__FILE__, __LINE__, "Model must be trained before its fit to the input data can be evaluated!"); 
				}
				unsigned int test_col = model_->descriptor_IDs_.size(); // no of columns of X^T X (linear model)
				unsigned int kernel_test_col = model_->descriptor_matrix_.rows();  // no of columns of X X^T (nonlinear model)
				if (test_col == 0)
				{
					test_col = model_->data->getNoDescriptors();
				}
				
				if (!transform && test_col != res_rows && kernel_test_col != res_rows)
				{
					throw Exception::InconsistentUsage(__FILE__, __LINE__, "Model must be trained before its fit to the input data can be evaluated!"); 
				}
						 		 		 
		// 		else if ( des == 0 && data_cols != res_rows && data_cols != desmat_cols)
		// 		{
		// 			throw Exception::InconsistentUsage(__FILE__, __LINE__, "Model must be trained with data containing the same number of features as the test data set!");
		// 		}
		// 		else if (des != 0 && res_rows != des && data_cols != desmat_cols)
		// 		{cout<<data_cols<<" "<<res_rows<<" "<<desmat_cols<<endl;cout.flush();
		// 			throw Exception::InconsistentUsage(__FILE__, __LINE__, "Model must be trained with data containing the same number of features as the test data set!");
		// 		}
			}
			
			R2_ = 0; ssE_ = 0; ssR_ = 0; std_err_ = 0; F_regr_ = 0;
			int lines = model_->data->descriptor_matrix_[0].size();
			test_substances_.resize(lines);
			test_Y_.resize(lines, model_->data->Y_.size());
			
			bool back_transform = 0; 
			if (transform && model_->data->descriptor_transformations_.size() != 0)
			{
				// if test data is to be transformed according to centering of training data, BUT has already been centered itself
				back_transform = 1; 
			}

			for (int i = 0; i < lines; i++)
			{
				setTestLine(i, i, back_transform); 
			}
			testAllSubstances(transform); 
			R2_ = quality_;
			
			int col = model_->data->descriptor_matrix_.size();
			if (!model_->descriptor_IDs_.empty())
			{
				col = model_->descriptor_IDs_.size();
			}
			
			//F_regr_ = gsl_cdf_fdist_P((R2_*(lines-col-1))/((1-R2_)*col), col, lines-col-1);
		//	F_regr_ = gsl_cdf_fdist_P( (ssR_*(lines-col-1))/(ssE_*col), col, lines-col-1 );
		}


		void RegressionValidation::calculateCoefficientStdErrors(int k, bool b)
		{
			if (model_->data->descriptor_matrix_.size() == 0 || model_->data->Y_.size() == 0)
			{
				throw Exception::InconsistentUsage(__FILE__, __LINE__, "Data must be fetched from input-files by QSARData before standart errors of coefficients can be calculated!"); 
			}
			if (dynamic_cast < KernelModel* > (model_))
			{
				throw Exception::InconsistentUsage(__FILE__, __LINE__, "Calculation of the standard deviation of regression coefficients can only be done for _linear_ regression models in a meaningful way!"); 
			}
			backupTrainingResults();
			
			int no_activities = model_->data->Y_.size();
			MatrixVector* results = new MatrixVector;
			int no_descriptors = model_->data->descriptor_matrix_.size();
			if (!model_->descriptor_IDs_.empty())
			{
				no_descriptors = model_->descriptor_IDs_.size();
			}
			coefficient_stderr_.resize(no_descriptors, no_activities);
			
			if (b == 1)
			{
				bootstrap(k, results, false);
			}
			else
			{
				crossValidation(k, results, false);
			}
			
			for (int c = 0; c < no_activities; c++) // for all modelled activities
			{
				for (int m = 0; m < no_descriptors; m++) // for all descriptors
				{			
					double mean_mc = 0;
					double sumsquares_mc = 0;
					
					for (int i = 0; i < k; i++) // for all training results
					{
						mean_mc += (*results)[i](m, c);
						sumsquares_mc += pow((*results)[i](m, c), 2);
					}
					mean_mc /= k;
					
					// calculate standard deviation of coefficient
					// = sqrt(1/k * \sum_{i = 1}^k (x_i \^bar x)^2)
					// <=> sqrt(1/k (\sum_{i = 1}^k x_i - k*\bar x^2))
					coefficient_stderr_(m, c) = sqrt(abs(sumsquares_mc-k*pow(mean_mc, 2))/(k-1));
					
					// standard-error == standard-deviation/sqrt(k)
					//coefficient_stderr_(m, c) /= sqrt(k);
				}
			}
			
			delete results;
			restoreTrainingResults();
		}


		void RegressionValidation::bootstrap(int k, bool restore)
		{ 
			bootstrap(k, NULL, restore);
		}


		void RegressionValidation::bootstrap(int k, MatrixVector* results, bool restore)
		{
			if (model_->data->descriptor_matrix_.size() == 0 || model_->data->Y_.size() == 0)
			{
				throw Exception::InconsistentUsage(__FILE__, __LINE__, "Data must be fetched from input-files by QSARData before bootstrapping can be done!"); 
			}
			if (restore) backupTrainingResults(); 
			
			
			Q2_ = 0; double r2 = 0; max_error_ = 0;
			int N = model_->data->descriptor_matrix_[0].size();
			int no_descriptors = model_->data->descriptor_matrix_.size();
			if (!model_->descriptor_IDs_.empty())
			{
				no_descriptors = model_->descriptor_IDs_.size();
			}
			
			boost::mt19937 rng(PreciseTime::now().getMicroSeconds());
			
			for (int i = 0; i < k; i++) // create and evaluate k bootstrap samples
			{
				vector<int> sample_substances(N, 0); // numbers of occurences of substances within this sample
				
				/// create training matrix and train the model_
				model_->descriptor_matrix_.resize(N, no_descriptors);
				model_->Y_.resize(N, model_->data->Y_.size());
				for (int j = 0; j < N; j++)
				{
					int pos = rng() % N;
					setTrainingLine(j, pos);
					sample_substances[pos]++;
				}
				model_->train(); // train the model_ on current bootstrap sample
				
				
				/// find size of test data set
				int test_size = 0;
				for (int j = 0; j < N; j++) 
				{
					if (sample_substances[j] > 0) 
					{
						continue;
					}
					test_size++;
				}
				test_substances_.resize(test_size);
				test_Y_.resize(test_size, model_->data->Y_.size());
				
				
				/// create test data set and calculate Q^2
				int test_line = 0;
				for (int j = 0; j < N; j++)
				{
					if (sample_substances[j] == 0) 
					{	
						setTestLine(test_line, j);
						test_line++;
					}
				}
				if (results != NULL){ results->push_back(*regr_model_->getTrainingResult()); }
				testAllSubstances(0);
				Q2_ += quality_;
				
				/// create test data set and calculate R^2
				test_substances_.resize(N);
				test_Y_.resize(N, model_->data->Y_.size());
				test_line = 0;
				for (int j = 0; j < N; j++)  
				{
					while (sample_substances[j] > 0) // insert substance as often as it occurs in the training data set 
					{	
						setTestLine(test_line, j);
						test_line++;
						sample_substances[j]--;
					}
				}
				testAllSubstances(0);
				//r2 += 1-(ssE_/(ssE_+ssR_));
				r2 += quality_;
			}
			
			Q2_ = Q2_/k;
			r2 = r2/k;
			
			Q2_ = 0.632*Q2_ + 0.368*r2;
				
			if (restore) restoreTrainingResults(); 
		}


		const Eigen::MatrixXd& RegressionValidation::yRandomizationTest(int runs, int k)
		{
			if (model_->data->descriptor_matrix_.size() == 0 || model_->data->Y_.size() == 0)
			{
				throw Exception::InconsistentUsage(__FILE__, __LINE__, "Data must be fetched from input-files by QSARData object before response permutation tests can be done!"); 
			}	
			
			backupTrainingResults();
			vector<vector<double> > dataY_backup = model_->data->Y_;
						
			yRand_results_.resize(runs, 2);
			yRand_results_.setConstant(-1);

			for (int i = 0; i < runs; i++)
			{
				yRand(); // randomize all columns of Y_
				crossValidation(k, NULL, 0);
				model_->readTrainingData();
				model_->train();
				testInputData(0);	
				yRand_results_(i, 0) = R2_;
				yRand_results_(i, 1) = Q2_;
			}	
			
			restoreTrainingResults();
			QSARData* data = const_cast <QSARData*> (model_->data);
			data->Y_ = dataY_backup;
			
			return yRand_results_;
		}

		void RegressionValidation::calculateQOF()
		{
			quality_ = (ssY_-ssE_)/ssY_;	
		}


		double RegressionValidation::getQ2()
		{
			return Q2_;
		}


		double RegressionValidation::getFcv()
		{
			return F_cv_;
		}


		double RegressionValidation::getFregr()
		{
			return F_regr_;
		}


		double RegressionValidation::getMaxError()
		{ 
			return max_error_;
		}


		double RegressionValidation::getR2()
		{
			return R2_;
		}


		double RegressionValidation::getCVRes()
		{
			//return getQ2();
			return (this->*predQualFetcher_)();
		}


		double RegressionValidation::getFitRes()
		{
			return (this->*fitQualFetcher_)();
		}


		void RegressionValidation::selectStat(int s)
		{
			predQualFetcher_ = &RegressionValidation::getQ2;
			fitQualFetcher_ = &RegressionValidation::getR2;
			
			if (s == 0)
			{
				validation_statistic_ = 0;
				qualCalculation = &RegressionValidation::calculateQOF;
			}
			else 
			{
				throw BALL::Exception::GeneralException(__FILE__, __LINE__, "RegressionValidation error", "Validation statistic "+String(s)+" is unknown!");
			}
		}
						
						
		void RegressionValidation::setCVRes(double d)
		{
			setQ2(d);
		}


		const Eigen::MatrixXd* RegressionValidation::getCoefficientStdErrors()
		{
			return &coefficient_stderr_;
		}


		void RegressionValidation::setCoefficientStdErrors(const Eigen::MatrixXd* sterr)
		{
			coefficient_stderr_ = *sterr;	
		}


		void RegressionValidation::saveToFile(string filename) const
		{
			saveToFile(filename, R2_, Q2_, coefficient_stderr_, yRand_results_);	
		}

		void RegressionValidation::saveToFile(string filename, const double& r2, const double& q2, const Eigen::MatrixXd& coefficient_stddev, const Eigen::MatrixXd& yRand_results) const
		{
			ofstream out(filename.c_str());
			
			Registry reg;
			out<<"# used quality statistic: "<<reg.getRegressionStatisticName(validation_statistic_)<<endl<<endl;	
			out << "Fit to training data = "<<r2<<endl;
			out << "Predictive quality = "<<q2<<endl;
			
			if (coefficient_stddev.cols() > 0)
			{
				out<<endl;
				out<<"[Coefficient stddev]"<<endl;
				out<<"dimensions = "<<coefficient_stddev.rows()<<" "<<coefficient_stddev.cols()<<endl;
				out<<coefficient_stddev<<endl;
			}
			if (yRand_results.cols() > 0)
			{
				out<<endl;
				out<<"[Response Permutation]"<<endl;
				out<<"dimensions = "<<yRand_results.rows()<<" "<<yRand_results.cols()<<endl;
				out<<yRand_results<<endl;
			}
		}


		void RegressionValidation::readFromFile(string filename)
		{
			ifstream in(filename.c_str()); 
			
			bool stddev_section = 0;
			bool yRand_section = 0;
			
			while (in)
			{
				String line;
				getline(in, line);
				line.trimLeft();
				if(line=="" || line.hasPrefix("#") || line.hasPrefix("//") || line.hasPrefix("%"))
				{
					continue;
				}
				if (stddev_section)
				{
					if (line.hasPrefix("dimensions"))
					{
						line = ((String)line.after("="));
						unsigned int no_rows = line.getField(0).toInt();
						unsigned int no_cols = line.getField(1).toInt();
						model_->readMatrix(coefficient_stderr_, in, no_rows, no_cols);
					}
					stddev_section = 0;
					
				}
				else if (yRand_section)
				{
					if (line.hasPrefix("dimensions"))
					{
						line = ((String)line.after("="));
						unsigned int no_rows = line.getField(0).toInt();
						unsigned int no_cols = line.getField(1).toInt();
						model_->readMatrix(yRand_results_, in, no_rows, no_cols);
					}
					yRand_section = 0;
				}
				if (line.hasPrefix("Fit to training data"))
				{
					R2_ = ((String)line.after("=")).trimLeft().toDouble();
				}
				else if (line.hasPrefix("Predictive quality"))
				{
					Q2_ = ((String)line.after("=")).trimLeft().toDouble();
				}
				else if (line.hasPrefix("[Coefficient stddev]"))
				{
					yRand_section = 0;
					stddev_section = 1;
				}
				else if (line.hasPrefix("[Response Permutation]"))
				{
					yRand_section = 1;
					stddev_section = 0;
				}
			}
		}
	}
}
