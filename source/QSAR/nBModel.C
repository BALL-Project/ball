// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//

#include <BALL/QSAR/nBModel.h>
#include <limits>

using namespace std;

namespace BALL
{
	namespace QSAR
	{

		NBModel::NBModel(const QSARData& q) : BayesModel(q) 
		{
			type_="nB";
			probabilities_.resize(0);
			discretization_steps_ = 5;
			discretizeFeatures = &NBModel::equalSpaceDiscretization;
			discretizeTestDataFeatures = &NBModel::equalSpaceDiscretizationTestData;
		}

		NBModel::~NBModel()
		{
		}

		void NBModel::train()
		{
			if (descriptor_matrix_.cols() == 0)
			{
				throw Exception::InconsistentUsage(__FILE__, __LINE__, "Data must be read into the model before training!"); 
			}
			readLabels();
			
			unsigned int no_features = descriptor_matrix_.cols();
			unsigned int no_classes = labels_.size();
			unsigned int no_compounds = descriptor_matrix_.rows();
			unsigned int no_activities = Y_.cols();
				
			// map values of Y to their index
			map<int, unsigned int> label_to_pos; 
			for (unsigned int i = 0; i < no_classes; i++)
			{
				label_to_pos.insert(make_pair(labels_[i], i));
			}	

			min_max_.resize(2, no_features);
			min_max_.row(0).setConstant( std::numeric_limits<double>::infinity());
			min_max_.row(1).setConstant(-std::numeric_limits<double>::infinity());
			
			probabilities_.clear();
			probabilities_.resize(no_activities);
			no_substances_.clear();
			no_substances_.resize(no_classes, 0);
			
			/// discretize the training data features
			(this->*discretizeFeatures)(discretization_steps_, min_max_);
			
			Eigen::MatrixXd prob_matrix(discretization_steps_, no_features); prob_matrix.setZero();
			probabilities_.resize(no_activities);
			for (unsigned int act = 0; act < no_activities; act++)
			{
				probabilities_[act].resize(no_classes, prob_matrix);
			
				for (unsigned int j = 0; j < no_compounds; j++)
				{
					unsigned int class_id = label_to_pos.find((int)Y_(j, act))->second;
					no_substances_[class_id]++;
					for (unsigned int i = 0; i < no_features; i++)
					{
						// features have been discretized, so that descriptor_matrix_ contains only unsigned int's
						unsigned int feat_bucket = (unsigned int)descriptor_matrix_(j, i);
						probabilities_[act][class_id](feat_bucket, i)++;
					}	
				}
				
				for (unsigned int i = 0; i < no_features; i++)
				{
					for (unsigned int j = 0; j < discretization_steps_; j++)
					{
						for (unsigned int k = 0; k < no_classes; k++)
						{
							// calculate p(x_ij | k)
							probabilities_[act][k](j, i) /= no_substances_[k];
						}
					}
				}
			}
		}


		Eigen::VectorXd NBModel::predict(const vector<double> & substance, bool transform)
		{
			if (probabilities_.size() == 0)
			{
				throw Exception::InconsistentUsage(__FILE__, __LINE__, "Model must be trained before it can predict the activitiy of substances!"); 
			}
			
			Eigen::VectorXd s = getSubstanceVector(substance, transform); 
			
			unsigned int no_activities = probabilities_.size();
			unsigned int no_classes = probabilities_[0].size();
			unsigned int no_features = probabilities_[0][0].cols();
			
			Eigen::RowVectorXd result(no_activities);
			result.setZero();
			
			/// discretize the test data features according to the discretization of training data
			(this->*discretizeTestDataFeatures)(s, discretization_steps_, min_max_);

			for (unsigned int act = 0; act < no_activities; act++)
			{
				vector<double> substance_prob(no_classes, 1); // prob. for the entire substance
				double max = 0;
				int best_label = labels_[0];
				double second_best = 0;
				
				for (unsigned int i = 0; i < no_features; i++)
				{
					// features were discretized, so they contain only unsigned int's
					unsigned int feature_bucket = (unsigned int) s(i);
					
					for (unsigned int j = 0; j < no_classes; j++)
					{
						substance_prob[j] *= probabilities_[act][j](feature_bucket, i);
						
						if (i == no_features-1 && substance_prob[j] > max)
						{
							second_best = max;
							max = substance_prob[j];
							best_label = labels_[j];
						}
					}
				}
				
				if (max >= second_best+min_prob_diff_)
				{
					result(act) = best_label;
				}
				else
				{
					result(act) = undef_act_class_id_;
				}
			}	
			
		// 	cout<<"no features = "<<s.cols()<<endl;
		// 	cout<<"descriptor_IDs_="<<descriptor_IDs_.toStr()<<endl;
		//  	cout<<"discretized s="<<s;
		//  	cout<<"predicted class="<<result;
			
			return result;	
		}


		vector<double> NBModel::getParameters() const
		{
			vector<double> d;
			d.push_back(discretization_steps_);
			d.push_back(min_prob_diff_); 
			d.push_back(undef_act_class_id_);
			return d;
		}


		void NBModel::setParameters(vector<double>& v)
		{
			if (v.size() != 1 && v.size() != 3)
			{
				String c = "Wrong number of model parameters! Needed: 3;";
				c = c+" given: "+String(v.size());
				throw Exception::ModelParameterError(__FILE__, __LINE__, c.c_str());
			}
			discretization_steps_ = (int) v[0];
			
			if (v.size() == 3)
			{
				min_prob_diff_ = v[1]; 
				undef_act_class_id_ = v[2];
			}
		}


		bool NBModel::isTrained()
		{
			unsigned int sel_features = descriptor_IDs_.size();
			if (sel_features == 0)
			{
				sel_features = data->getNoDescriptors();
			}
			
			if (probabilities_.size() > 0 && (unsigned int)min_max_.cols() == sel_features) return true; 
			return false;
		}


		int NBModel::getNoResponseVariables()
		{
			if (!isTrained()) return 0; 
			else return probabilities_.size();	
		}


		vector<double> NBModel::calculateProbabilities(int activitiy_index, int feature_index, double feature_value)
		{
			if (probabilities_.size() == 0 || probabilities_[0].size() == 0)
			{
				throw Exception::InconsistentUsage(__FILE__, __LINE__, "Model must be trained before a probability for a given feature value can be calculated!"); 
			}
			unsigned int no_features = probabilities_[0][0].cols();
			unsigned int no_classes = probabilities_[0].size();
			if (activitiy_index >= (int)probabilities_.size() || feature_index >= (int)no_features || activitiy_index < 0 || feature_index < 0)
			{
				throw Exception::InconsistentUsage(__FILE__, __LINE__, "Index out of bounds for parameters given to SNBModel::calculateProbability() !"); 
			}
			
			unsigned int no_discretizations = probabilities_[0][0].rows();
			double step = (min_max_(1, feature_index)-min_max_(0, feature_index))/no_discretizations;
			int disc_index = (int)((feature_value-min_max_(0, feature_index))/step);
			
			if (disc_index < 0) disc_index = 0; 
			else if (disc_index >= (int)no_discretizations) disc_index = no_discretizations - 1; 
			
			vector<double> prob(no_classes);
			for (unsigned int i = 0; i < no_classes; i++)
			{
				prob[i] = probabilities_[activitiy_index][i](disc_index, feature_index);
			}
			return prob;
		}
				

		void NBModel::saveToFile(string filename)
		{
			bool trained = 1;
			if (probabilities_.size() == 0) trained = 0; 
			ofstream out(filename.c_str());
			
			bool centered_data = 0;
			bool centered_y = 0;
			if (descriptor_transformations_.cols() != 0)
			{
				centered_data = 1;
				if (y_transformations_.cols() != 0)
				{
					centered_y = 1;
				}
			}
			
			int sel_features = descriptor_IDs_.size();
			if (sel_features == 0)
			{
				sel_features = data->getNoDescriptors();
			}
			
				
			int no_y = probabilities_.size();
			if (no_y == 0) no_y = y_transformations_.cols(); // correct no because transformation information will have to by read anyway when reading this model later ...
			
			out<<"# model-type_\tno of featues in input data\tselected featues\tno of response variables\tcentered descriptors?\tno of classes\ttrained?"<<endl;
			out<<type_<<"\t"<<data->getNoDescriptors()<<"\t"<<sel_features<<"\t"<<no_y<<"\t"<<centered_data<<"\t"<<no_substances_.size()<<"\t"<<trained<<"\n\n";

			saveModelParametersToFile(out);
			saveDescriptorInformationToFile(out); 
			
			if (!trained) return; 
			
			saveClassInformationToFile(out); 
			
			out<<min_max_<<endl;
			
			// write probability matrices
			for (unsigned int i = 0; i < probabilities_.size(); i++)
			{
				for (unsigned int j = 0; j < probabilities_[0].size(); j++)
				{
					out<<probabilities_[i][j]<<endl;
				}
			}
			
			out.close();
		}



		void NBModel::readFromFile(string filename)
		{
			ifstream input(filename.c_str()); 
			if (!input)
			{
				throw BALL::Exception::FileNotFound(__FILE__, __LINE__, filename);
			}	
			
			String line0;
			getline(input, line0);  // skip comment line 
			getline(input, line0);  // read read line containing model specification
			
			if (line0.getField(0, "\t") != type_)
			{
				String e = "Wrong input data! Use training data file generated by a ";
				e = e + type_ + " model !";
				throw Exception::WrongDataType(__FILE__, __LINE__, e.c_str());
			}
			
			int no_descriptors = line0.getField(2, "\t").toInt();
			int no_y = line0.getField(3, "\t").toInt();
			bool centered_data = line0.getField(4, "\t").toInt();
			int no_classes = line0.getField(5, "\t").toInt();
			bool trained = line0.getField(6, "\t").toBool();

			substance_names_.clear();
			
			getline(input, line0);  // skip empty line	
			readModelParametersFromFile(input);
			readDescriptorInformationFromFile(input, no_descriptors, centered_data); 
			
			if (!trained) 
			{
				probabilities_.resize(0);
				return;
			}
			
			readClassInformationFromFile(input, no_classes); 
			readMatrix(min_max_, input, 2, no_descriptors);
			getline(input, line0);  // skip empty line	
			
			probabilities_.resize(no_y);
			for (int act = 0; act < no_y; act++)   // read all probability matrices 
			{
				probabilities_[act].resize(no_classes); // <no_y>*<no_classes> matrices
				for (int i = 0; i < no_classes; i++)
				{
					readMatrix(probabilities_[act][i], input, discretization_steps_, no_descriptors);
					getline(input, line0);  // skip empty line
				}
			}
			input.close();
			if (((String)filename).hasSuffix("nB2.mod")) 
			{
				cout<<descriptor_IDs_.size()<<endl<<flush;

				std::multiset<unsigned int>::iterator d_it = descriptor_IDs_.begin();
				for (; d_it != descriptor_IDs_.end(); ++d_it)
				{
					cout << String(*d_it) << " ";
				}
				cout << endl;
				cout<<descriptor_IDs_.size()<<endl<<flush;
			}
		}
	}
}
