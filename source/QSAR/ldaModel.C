// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//

#include <BALL/QSAR/ldaModel.h>

#include <Eigen/Dense>

#include <limits>

using namespace std;

namespace BALL
{
	namespace QSAR
	{
			
		LDAModel::LDAModel(const QSARData& q) : ClassificationModel(q) 
		{
			lambda_ = 0.0001;
			type_="LDA";
		}

		LDAModel::~LDAModel()
		{
		}

		void LDAModel::train()
		{
			if (descriptor_matrix_.cols() == 0)
			{
				throw Exception::InconsistentUsage(__FILE__, __LINE__, "Data must be read into the model before training!"); 
			}
			readLabels();
			
			// map values of Y to their index
			map<int, unsigned int> label_to_pos; 
			for (unsigned int i = 0; i < labels_.size(); i++)
			{
				label_to_pos.insert(make_pair(labels_[i], i));
			}

			// calculate sigma_ = covariance matrix of descriptors
			sigma_.resize(descriptor_matrix_.cols(), descriptor_matrix_.cols());
			for (int i = 0; i < descriptor_matrix_.cols(); i++)
			{
				double mi = Statistics::getMean(descriptor_matrix_, i);
				for (int j = 0; j < descriptor_matrix_.cols(); j++)
				{
					sigma_(i, j) = Statistics::getCovariance(descriptor_matrix_, i, j, mi);
				}
			}
			Eigen::MatrixXd I(sigma_.cols(), sigma_.cols()); I.setIdentity();
			I *= lambda_;
			sigma_ += I;

			sigma_ = sigma_.inverse();

			mean_vectors_.resize(Y_.cols());
			no_substances_.clear();
			no_substances_.resize(labels_.size(), 0);
			for (int c = 0; c < Y_.cols(); c++)
			{
				vector < int > no_substances_c(labels_.size(), 0);  // no of substances in each class for activitiy c
				mean_vectors_[c].resize(labels_.size(), descriptor_matrix_.cols());
				mean_vectors_[c].setZero();
				
				for (int i = 0; i < descriptor_matrix_.rows(); i++) // calculate sum vector of each class
				{
					int yi = static_cast < int > (Y_(i, c)); // Y_ will contains only ints for classifications
					int pos = label_to_pos.find(yi)->second; 
					
					for (int j = 0; j < descriptor_matrix_.cols(); j++)
					{
						mean_vectors_[c](pos, j) += descriptor_matrix_(i, j);
					}
					
					if (c == 0) no_substances_c[pos]++; 
					
				}
				
				for (int i = 0; i < mean_vectors_[c].rows(); i++) // calculate mean vector of each class
				{
					if (no_substances_c[i] == 0)
					{
						mean_vectors_[c].row(i).setConstant(std::numeric_limits<double>::infinity());  // set mean of classes NOT occurring for activitiy c to inf
					}
					
					for (int j = 0; j < descriptor_matrix_.cols(); j++)
					{
						mean_vectors_[c](i, j) = mean_vectors_[c](i, j)/no_substances_c[i];
					}
				}
				
				for (unsigned int i = 0; i < no_substances_.size(); i++) // update overall number of substances per class
				{
					no_substances_[i] += no_substances_c[i];
				}
			}
			
		}


		Eigen::VectorXd LDAModel::predict(const vector<double> & substance, bool transform)
		{
			if (sigma_.cols() == 0)
			{
				throw Exception::InconsistentUsage(__FILE__, __LINE__, "Model must be trained before it can predict the activitiy of substances!"); 
			}
			// search class to which the given substance has the smallest distance
			Eigen::VectorXd s = getSubstanceVector(substance, transform); // dim: 1xm
			
			int min_k = 0;
			double min_dist = 99999999;
			Eigen::VectorXd result(mean_vectors_.size());
			
			for (unsigned int c = 0; c < mean_vectors_.size(); c++)
			{
				for (int k = 0; k < mean_vectors_[c].rows(); k++)
				{
					Eigen::VectorXd diff(s.rows()); 
					for (unsigned int i = 0; i < s.rows(); i++)
					{
						diff(i) = s(i)-mean_vectors_[c](k, i); 
					}
					double dist = diff.transpose()*sigma_*diff;
					if (dist < min_dist)
					{
						min_dist = dist;
						min_k = k;
					}
				}
				result(c) = labels_[min_k];
			}
			
			return result;
			
		}

		void LDAModel::setParameters(vector<double>& v)
		{
			if (v.size() != 1)
			{
				String c = "Wrong number of model parameters! Needed: 1;";
				c = c+" given: "+String(v.size());
				throw Exception::ModelParameterError(__FILE__, __LINE__, c.c_str());
			}
			lambda_ = v[0];
		}


		vector<double> LDAModel::getParameters() const
		{
			vector<double> d;
			d.push_back(lambda_);
			return d;
		}


		void LDAModel::saveToFile(string filename)
		{
			bool trained = 1;
			if (sigma_.rows() == 0) trained = 0; 
			
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
			
			
			int no_y = sigma_.cols();
			if (no_y == 0) no_y = y_transformations_.cols(); // correct no because transformation information will have to by read anyway when reading this model later ...
			
			out<<"# model-type_\tno of featues in input data\tselected featues\tno of response variables\tcentered descriptors?\tno of classes\ttrained?"<<endl;
			out<<type_<<"\t"<<data->getNoDescriptors()<<"\t"<<sel_features<<"\t"<<no_y<<"\t"<<centered_data<<"\t"<<no_substances_.size()<<"\t"<<trained<<"\n\n";

			saveModelParametersToFile(out);
			saveResponseTransformationToFile(out); 
			saveDescriptorInformationToFile(out); 
			
			if (!trained) return; 
			
			saveClassInformationToFile(out); 
			out<<sigma_<<endl;
			
			for (unsigned int i = 0; i < mean_vectors_.size(); i++)   // write all mean-vector matrices
			{
				out<<mean_vectors_[i]<<endl;
			}

			out.close();
		}



		void LDAModel::readFromFile(string filename)
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
			Model::readDescriptorInformationFromFile(input, no_descriptors, centered_data); 
			
			if (!trained) 
			{
				sigma_.resize(0, 0);
				return;
			}
			
			readClassInformationFromFile(input, no_classes); 
			readMatrix(sigma_, input, no_descriptors, no_descriptors); 
			getline(input, line0);  // skip empty line 
			
			mean_vectors_.resize(no_y);
			for (int c = 0; c < no_y; c++) // read all mean-vector matrices
			{
				readMatrix(mean_vectors_[c], input, no_classes, no_descriptors);
			}
			
			input.close();
		}
	}
}
