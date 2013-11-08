// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// 

#include <BALL/QSAR/classificationModel.h>

namespace BALL
{
	namespace QSAR
	{
				
		ClassificationModel::ClassificationModel(const QSARData& q) : Model(q) 
		{
			validation = new ClassificationValidation(this); 
			model_val = validation;
		}


		ClassificationModel::~ClassificationModel()
		{
			delete validation;
		}

		void ClassificationModel::operator = (ClassificationModel& m)
		{
			Model::operator = (m);
		}

		void ClassificationModel::readLabels()
		{
			std::multiset<int> l;
			for (int i = 0; i < Y_.rows(); i++)
			{
				for (int j = 0; j < Y_.cols(); j++)
				{
					int label = static_cast<int>(Y_(i, j));
					if (label != Y_(i, j))
					{
						throw Exception::WrongDataType(__FILE__, __LINE__, "Some class labels are not discrete values!! Creation of a classification model is therefore not possible!"); 
					}
					if (l.find(label) == l.end())  // for classification experiments, Y will contain only ints
					{
						l.insert(label);
					}
				}
			}
			
			labels_.clear();
			std::multiset<int>::iterator l_it = l.begin();
			for (; l_it != l.end(); ++l_it)
			{
				labels_.push_back(*l_it);
			}
		}

		vector < int > ClassificationModel::getClassLabels()
		{
			return labels_;	
		}

		void ClassificationModel::readClassInformationFromFile(std::ifstream& input, int no_classes)
		{
			labels_.clear();
			no_substances_.clear();
			
			String line;
			getline(input, line);  // skip comment line 
			getline(input, line);    
			for (int i = 0; i < no_classes; i++)
			{
				labels_.push_back(line.getField(i, "\t").toInt());
			}	
			getline(input, line);  // skip empty line
			getline(input, line);  // skip comment line 
			getline(input, line);
			for (int i = 0; i < no_classes; i++)
			{
				int n = line.getField(i, "\t").toInt();
				no_substances_.push_back(n);
			}
			getline(input, line);  // skip empty line 
		}


		void ClassificationModel::saveClassInformationToFile(std::ofstream& out)
		{
			out<<"# class-labels_\n";
			for (unsigned int i = 0; i < labels_.size(); i++) // write class-labels_
			{
				out<<labels_[i]<<"\t";
			}
			out<<std::endl<<std::endl;
			
			out<<"# no of substances of each class\n";
			for (unsigned int i = 0; i < no_substances_.size(); i++)  // write numbers of substances of each class
			{
				out<<no_substances_[i]<<"\t";
			}
			out<<std::endl<<std::endl;
		}


		void ClassificationModel::equalSpaceDiscretization(unsigned int bins, Eigen::MatrixXd & discretization_information)
		{
			unsigned int no_features = descriptor_matrix_.cols();
			unsigned int no_compounds = descriptor_matrix_.rows();
			
			discretization_information.resize(2, no_features); 
			discretization_information.row(0).fill( 1e10); // minimum of each feature in first row
			discretization_information.row(1).fill(-1e10); // maximum of each feature in second row
			
			// find minimum and maximum of each feature
			for (unsigned int i = 0; i < no_features; i++)
			{
				for (unsigned int j = 0; j < no_compounds; j++)
				{
					if (descriptor_matrix_(j, i) < discretization_information(0, i))
					{
						discretization_information(0, i) = descriptor_matrix_(j, i); 
					}
					if (descriptor_matrix_(j, i) > discretization_information(1, i))
					{
						discretization_information(1, i) = descriptor_matrix_(j, i); 
					}
				}
			}

			// transform each feature value to a discrete value
			for (unsigned int i = 0; i < no_features; i++)
			{
				double step_width = (discretization_information(1, i)-discretization_information(0, i)) / bins; 
				for (unsigned int j = 0; j < no_compounds; j++)
				{
					unsigned int feat_bucket = (unsigned int)((descriptor_matrix_(j, i)-discretization_information(0, i)) / step_width); 
					if (feat_bucket >= bins) feat_bucket = bins-1; // for max.
					descriptor_matrix_(j, i) = feat_bucket;
				}	
			}
		}


		void ClassificationModel::equalSpaceDiscretizationTestData(Eigen::VectorXd & compound, unsigned int bins, const Eigen::MatrixXd & discretization_information)
		{
			if (compound.rows() != discretization_information.cols())
			{
				throw BALL::Exception::GeneralException(__FILE__, __LINE__, "Discretization error", "no of features of test compound and of discretized training data are different!"); 
			}
			
			unsigned int no_features = compound.rows();

			for (unsigned int i = 0; i < no_features; i++)
			{
				double step_width = (discretization_information(1, i)-discretization_information(0, i))/bins; 
				int feat_bucket = (int)((compound(i)-discretization_information(0, i))/step_width); 
				if (feat_bucket < 1) feat_bucket = 0; 
				else if ((unsigned int)feat_bucket >= bins) feat_bucket = bins-1; 
				compound(i) = feat_bucket;
			}
		}
	}
}
