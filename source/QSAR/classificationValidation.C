// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// 
#include <BALL/QSAR/classificationValidation.h>
#include <BALL/QSAR/statistics.h>
#include <BALL/QSAR/classificationModel.h>
#include <BALL/QSAR/registry.h>

#include <boost/random/mersenne_twister.hpp>

using namespace std;

namespace BALL
{
	namespace QSAR
			{

		ClassificationValidation::ClassificationValidation(ClassificationModel* m) : Validation(m)
		{
			clas_model = m; quality_ = -1; quality_cv_ = -1; quality_input_test_ = -1;
			qualCalculation = &ClassificationValidation::calculateAverageSensitivity; 
		}

		void ClassificationValidation::selectStat(int s)
		{
			if (s >= 0 && s <= 4)
			{
				validation_statistic_ = s;
			}
			if (s == 0)
			{
				qualCalculation = &ClassificationValidation::calculateAverageSensitivity; 
			}
			else if (s == 1)
			{
				qualCalculation = &ClassificationValidation::calculateWeightedSensitivity; 
			}
			else if (s == 2)
			{
				qualCalculation = &ClassificationValidation::calculateOverallAccuracy; 
			}
			else if (s == 3)
			{
				qualCalculation = &ClassificationValidation::calculateAverageMCC; 
			}
			else if (s == 4)
			{
				qualCalculation = &ClassificationValidation::calculateOverallMCC; 
			}
			else if (s == 5)
			{
				qualCalculation = &ClassificationValidation::calculateTDR; 
			}
		}

		void ClassificationValidation::crossValidation(int k, bool restore)
		{
			if (model_->data->descriptor_matrix_.size() == 0 || model_->data->Y_.size() == 0)
			{
				throw Exception::InconsistentUsage(__FILE__, __LINE__, "Data must be fetched from input-files by QSARData before cross-validation can be done!"); 
			}
			
			Eigen::MatrixXd desc_backup;
			//Eigen::MatrixXd res_backup;
			Eigen::MatrixXd y_backup;
			if (restore)
			{
				desc_backup = model_->descriptor_matrix_; // save matrices in order in restore them after cross-validation
				//res_backup = clas_model->training_result_;
				y_backup = model_->Y_;
			}
			
			int lines = model_->data->descriptor_matrix_[0].size();
			int col = model_->data->descriptor_matrix_.size();
			if (!model_->descriptor_IDs_.empty())
			{
				col = model_->descriptor_IDs_.size();
			}
			double average_accuracy = 0;
			class_results_.resize(clas_model->labels_.size());
			class_results_.setZero();
			
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
				testAllSubstances(0);  // do not transform cross-validation test-data again...
				average_accuracy += quality_;
			}
			quality_cv_ = average_accuracy/k;
			class_results_ = class_results_/k;
			
			if (restore)
			{
				model_->descriptor_matrix_ = desc_backup;   // prevent confusion of cross-validation coefficients with coefficients
				model_->Y_ = y_backup;
				model_->readTrainingData();
				model_->train();
			}
		}


		void ClassificationValidation::testAllSubstances(bool transform)
		{	
			confusion_matrix_.resize(4, clas_model->labels_.size());
			confusion_matrix_.setZero();
			class_results_.resize(clas_model->labels_.size());
			class_results_.setZero();
			
			for (int i = 0; i < (int)test_substances_.size(); i++) // for all substances in test-data
			{
				Eigen::VectorXd rv = model_->predict(test_substances_[i], transform); 

				for (int c = 0; c < test_Y_.cols(); c++) // for all modelled activities
				{			
					int y_ic = static_cast<int>(test_Y_(i, c)); 
					int rv_ic = static_cast<int>(rv(c));
					
					for (int k = 0; k < confusion_matrix_.cols(); k++)   // set TP, FP, TN, FN for all classes
					{				
						if ((clas_model->labels_)[k] == y_ic)
						{
							if (y_ic == rv_ic)
							{
								confusion_matrix_(0, k)++;  // TP for class k
							}
							else
							{	
								confusion_matrix_(3, k)++; // FN for class k
							}
						}
						else
						{
							if (clas_model->labels_[k] != rv_ic)
							{
								confusion_matrix_(2, k)++;  // TN for class k
							}
							else
							{
								confusion_matrix_(1, k)++; // FP for class k
							}
						}
					}

				}
				
				
			}

			(this->*qualCalculation)();
		}


		void ClassificationValidation::testInputData(bool transform)
		{	
			int lines = model_->data->descriptor_matrix_[0].size();
			test_substances_.resize(lines);
			test_Y_.resize(lines, model_->data->Y_.size());
			
			class_results_.resize(clas_model->labels_.size());
			class_results_.setZero();
			
			bool back_transform = 0; 
			if (transform && model_->data->descriptor_transformations_.size() > 0)
			{
				// if test data is to be transformed according to centering of training data, BUT has already been centered itself
				back_transform = 1; 
			}
			
			for (int i = 0; i < lines; i++)
			{
				setTestLine(i, i, back_transform); 
			}
			
			testAllSubstances(transform); 
			quality_input_test_ = quality_;
		}


		void ClassificationValidation::bootstrap(int k, bool restore)
		{
			if (model_->data->descriptor_matrix_.size() == 0 || model_->data->Y_.size() == 0)
			{
				throw Exception::InconsistentUsage(__FILE__, __LINE__, "Data must be fetched from input-files by QSARData before bootstrapping can be done!"); 
			}
			Eigen::MatrixXd desc_backup;
			Eigen::MatrixXd res_backup;
			Eigen::MatrixXd y_backup;
			if (restore)
			{
				desc_backup = model_->descriptor_matrix_; // save matrices in order in restore them after cross-validation
				//res_backup = clas_model->training_result_;
				y_backup = model_->Y_;
			}

			class_results_.resize(clas_model->labels_.size());
			class_results_.setZero();
			quality_cv_ = 0;
			int N = model_->data->descriptor_matrix_[0].size();
			int no_descriptors = model_->data->descriptor_matrix_.size();
			if (!model_->descriptor_IDs_.empty())
			{
				no_descriptors = model_->descriptor_IDs_.size();
			}

			boost::mt19937 rng(PreciseTime::now().getMicroSeconds());
			
			double overall_fit = 0;
			double overall_pred = 0;
			Eigen::VectorXd class_results_pred; 
			class_results_pred.resize(clas_model->labels_.size()); class_results_pred.setZero();
			Eigen::VectorXd class_results_fit; 
			class_results_fit.resize(clas_model->labels_.size()); class_results_fit.setZero();

			for (int i = 0; i < k; i++) // create and evaluate k bootstrap samples
			{
				vector<int> sample_substances(N, 0); // numbers of occurences of substances within this sample
				
				class_results_.setZero();
			
				/// create training matrix and train the model_
				model_->descriptor_matrix_.resize(N, no_descriptors);
				model_->Y_.resize(N, model_->data->Y_.size());
				for (int j = 0; j < N; j++)
				{
					//int pos = rand()%N;
					int pos = rng() % N;
					setTrainingLine(j, pos);
					sample_substances[pos]++;
				}
				model_->train();
			
				
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
				
			
				/// create test data set and calculate quality_ of prediction
				int test_line = 0;
				for (int j = 0; j < N; j++) 
				{
					if (sample_substances[j] == 0) 
					{	
						setTestLine(test_line, j);
						test_line++;
					}
				}
				testAllSubstances(0);
				overall_pred += quality_;
				class_results_pred += class_results_;		
			
				class_results_.setZero(); // clear pred. result before adding training fit result!!
				
				/// create test data set and calculate quality_ of fit to training data	
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
				overall_fit += quality_;
				class_results_fit += class_results_;
			}
			
			overall_pred = overall_pred/k;
			overall_fit = overall_fit/k;
			class_results_pred = class_results_pred/k;
			class_results_fit = class_results_fit/k;
			
			quality_cv_ = 0.632*overall_pred + 0.368*overall_fit;
			class_results_ = class_results_pred*0.632 + class_results_fit*0.368;
			
			if (restore)
			{
				model_->descriptor_matrix_ = desc_backup;   // prevent confusion of cross-validation coefficients with coefficients
				model_->Y_ = y_backup;
				model_->readTrainingData();
				model_->train();
			}
		}


		const Eigen::MatrixXd & ClassificationValidation::yRandomizationTest(int runs, int k)
		{
			Eigen::MatrixXd y_backup = model_->Y_;
			Eigen::MatrixXd desc_backup = model_->descriptor_matrix_;
			//Eigen::MatrixXd res_backup = clas_model->training_result_;
			VMatrix dataY_backup = model_->data->Y_;
						
			//Eigen::VectorXd c(2, -1);
			//vector<Eigen::VectorXd > results(runs, 2);
			yRand_results_.resize(runs, 2);
			yRand_results_.fill(-1);
			class_results_.resize(clas_model->labels_.size());
			class_results_.setZero();

			for (int i = 0; i < runs; i++)
			{
				yRand(); // randomize all columns of Y_
				crossValidation(k, 0);
				testInputData(0);
				yRand_results_(i, 0) = quality_input_test_;
				yRand_results_(i, 1) = quality_cv_;
			}
			
			class_results_ = class_results_/runs;
			
			model_->Y_ = y_backup;
			model_->descriptor_matrix_ = desc_backup;
			//clas_model->training_result_ = res_backup;
			QSARData* data = const_cast <QSARData*> (model_->data);
			data->Y_ = dataY_backup;
			model_->train();
			
			return yRand_results_;
		}



		void ClassificationValidation::calculateOverallAccuracy()
		{		
			// do NOT calculate accuracy seperately for each class!
			int TP = 0;
			for (int j = 0; j < confusion_matrix_.cols(); j++)
			{
				TP += (int)confusion_matrix_(0, j);
			}
			int N = 0; // number of predictions
			for (int j = 0; j < confusion_matrix_.rows(); j++)
			{
				N += (int)confusion_matrix_(j, 0);
			}
			quality_ = ((double)TP) / N;
		}
				


		void ClassificationValidation::calculateAverageSensitivity()
		{
			quality_ = 0;
			
			for (int j = 0; j < confusion_matrix_.cols(); j++) // calculate quality_ of all classes
			{	
				int TP = (int)confusion_matrix_(0, j);
				int FN = (int)confusion_matrix_(3, j);
				double sens = 1;
				if (TP != 0 || FN != 0)
				{
					sens = ((double)TP) / (TP+FN);
				}
				class_results_(j) += sens;
				quality_ += sens;
			}
			
			quality_ /= confusion_matrix_.cols(); // mean quality_ of all classes
		}


		void ClassificationValidation::calculateWeightedSensitivity()
		{
			quality_ = 0;
			int no_all = 0;
			
			// get number of substances that were used for training the model_
			for (int i = 0; i < (int)clas_model->no_substances_.size(); i++)
			{
				no_all += clas_model->no_substances_[i];
			}

			for (int j = 0; j < confusion_matrix_.cols(); j++) 
			{	
				int TP = (int)confusion_matrix_(0, j);
				int FN = (int)confusion_matrix_(3, j);
				double sens = 1;
				if (TP != 0 || FN != 0)
				{
					sens = ((double)TP) / (TP+FN);
				}		
				double sens_weighted = sens*(((double)clas_model->no_substances_[j])/no_all);
				class_results_(j) += sens_weighted;
				quality_ += sens_weighted;
			}
		}



		void ClassificationValidation::calculateAverageMCC()
		{
			quality_ = 0;
			double MCC = 0;
			
			for (int j = 0; j < confusion_matrix_.cols(); j++)
			{
				int TP = (int)confusion_matrix_(0, j);
				int FP = (int)confusion_matrix_(1, j);
				int TN = (int)confusion_matrix_(2, j);
				int FN = (int)confusion_matrix_(3, j);
				
				double nom = ((double)TP)*TN-FP*FN; // (often) too big for int...
				double denom = ((double)(TP+FP))*(TP+FN)*(TN+FP)*(TN+FN);
				if (denom != 0) denom = sqrt(denom); 
				else denom = 1; 
				
				double d = nom/denom;
				class_results_(j) += d;
				MCC += d;
			}
			quality_ = MCC/confusion_matrix_.cols();
		}
			
			
		void ClassificationValidation::calculateOverallMCC()
		{
			quality_ = 0;
			int TP = 0; int FP = 0; int TN = 0; int FN = 0;
			for (int j = 0; j < confusion_matrix_.cols(); j++)
			{
				TP += (int)confusion_matrix_(0, j);
				FP += (int)confusion_matrix_(1, j);
				TN += (int)confusion_matrix_(2, j);
				FN += (int)confusion_matrix_(3, j);
			}
			double nom = ((double)TP)*TN-FP*FN; // (often) too big for int...
			double denom = ((double)(TP+FP))*(TP+FN)*(TN+FP)*(TN+FN);
			if (denom != 0) denom = sqrt(denom); 
			else denom = 1; 
			quality_ = nom/denom;
		}


		// calculation of True Discovery Rate
		void ClassificationValidation::calculateTDR()
		{
			quality_ = 0;
			int TP = 0; int FP = 0;
			
			if (confusion_matrix_.cols() > 2)
			{
				throw BALL::Exception::GeneralException(__FILE__, __LINE__, "Classification validation error", "True Discovery Rate can only be calculated for binary classification data sets!"); 
			}
			
			TP = (int)confusion_matrix_(0, 1);
			FP = (int)confusion_matrix_(1, 1);
			
			if (TP == 0) 
			{
				quality_ = 0;
				return;
			}
				
			quality_ = ((double)TP)/(TP+FP);	
		}


		const Eigen::MatrixXd* ClassificationValidation::getConfusionMatrix()
		{
			return &confusion_matrix_;
		}


		const Eigen::VectorXd* ClassificationValidation::getClassResults()
		{
			return &class_results_;
		}


		double ClassificationValidation::getAccuracyInputTest()
		{
			return quality_input_test_;
		}


		double ClassificationValidation::getAccuracyCV()
		{
			return quality_cv_;
		}


		double ClassificationValidation::getCVRes()
		{
			return quality_cv_;
		}
						

						
		void ClassificationValidation::setCVRes(double d)
		{
			quality_cv_ = d;
		}


		double ClassificationValidation::getFitRes()
		{
			return quality_input_test_;
		}

		void ClassificationValidation::saveToFile(string filename) const
		{
			saveToFile(filename, quality_input_test_, quality_cv_);	
		}

		void ClassificationValidation::saveToFile(string filename, const double& quality_input_test, const double& predictive_quality) const
		{
			ofstream out(filename.c_str());
			
			Registry reg;
			out<<"# used quality statistic: "<<reg.getClassificationStatisticName(validation_statistic_)<<endl<<endl;
			out << "Fit to training data = "<<quality_input_test<<endl;
			out << "Predictive quality = "<<predictive_quality<<endl;	
		}


		void ClassificationValidation::readFromFile(string filename)
		{
			ifstream in(filename.c_str()); 
			
			while (in)
			{
				String line;
				getline(in, line);
				line.trimLeft();
				if(line=="" || line.hasPrefix("#") || line.hasPrefix("//") || line.hasPrefix("%"))
				{
					continue;
				}
				if (line.hasPrefix("Fit to training data"))
				{
					quality_input_test_ = ((String)line.after("=")).trimLeft().toDouble();
				}
				else if (line.hasPrefix("Predictive quality"))
				{
					quality_cv_ = ((String)line.after("=")).trimLeft().toDouble();
				}
			}
		}
	}
}
