// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//

#include <BALL/QSAR/logitModel.h>
#include <Eigen/Dense>

using namespace std;

namespace BALL
{
	namespace QSAR
	{

		LogitModel::LogitModel(const QSARData& q) : ClassificationModel(q)
		{
			type_="logit";
		}

		LogitModel::~LogitModel()
		{
		}

		void LogitModel::train()
		{
			if (descriptor_matrix_.cols() == 0)
			{
				throw Exception::InconsistentUsage(__FILE__, __LINE__, "Data must be read into the model before training!"); 
			}
			readLabels();
			
			int lines = descriptor_matrix_.rows();
			int col = descriptor_matrix_.cols();
			training_result_.resize(col, Y_.cols());

			Eigen::MatrixXd I(col, col);
			I.setIdentity();
			I *= 0.0001;

			for (int c = 0; c < Y_.cols(); c++)	
			{
				Eigen::VectorXd beta(col);
				beta.setZero();
				for ( int b = 0; b < 100; b++)
				{
					// calculate matrix W and vector p
					Eigen::VectorXd p(lines);
					Eigen::MatrixXd W(lines, lines); W.setZero(); // diagonal matrix
					for (int i = 0; i < lines; i++)
					{
						double nom = exp(descriptor_matrix_.row(i).dot(beta));
						p(i) = nom/(1+nom);
						W(i, i) = p(i)*(1/(1+nom));
					}
					Eigen::VectorXd beta_old = beta;
					
					Eigen::MatrixXd xwx = descriptor_matrix_.transpose() * W * descriptor_matrix_ + I;

					p = Y_.row(c) - p;

					beta += xwx.colPivHouseholderQr().solve(descriptor_matrix_.transpose()*p);
					//beta = xwx.i()*descriptor_matrix_.t()*W*(descriptor_matrix_*beta+W.i()*(Y_.Column(c)-p));
					
					if (Statistics::euclDistance(beta, beta_old)/Statistics::euclNorm(beta) < 0.01) 
					{ 
						break;
					}
				}
				training_result_.col(c) = beta;
			}
		}


		Eigen::VectorXd LogitModel::predict(const vector<double> & substance, bool transform)
		{
			if (training_result_.cols() == 0)
			{
				throw Exception::InconsistentUsage(__FILE__, __LINE__, "Model must be trained before it can predict the activitiy of substances!"); 
			}

			Eigen::VectorXd v = getSubstanceVector(substance, transform); 
			Eigen::VectorXd res = v*training_result_;

			if (transform)
			{
				backTransformPrediction(res); 
			}

			for (unsigned int i = 0; i < res.rows(); i++)
			{
				if (res(i) >= 0)
				{
					res(i) = 1;
				}
				else
				{
					res(i) = 0;
				}
			}

			return res;	
		}


		void LogitModel::saveToFile(string filename)
		{
			if (training_result_.rows() == 0)
			{
				throw Exception::InconsistentUsage(__FILE__, __LINE__, "Model must have been trained before the results can be saved to a file!"); 
			}
			ofstream out(filename.c_str());
			
			bool centered_data = 0;
			if (descriptor_transformations_.cols() != 0)
			{
				centered_data = 1;
			}
			int sel_features = descriptor_IDs_.size();
			if (sel_features == 0)
			{
				sel_features = data->getNoDescriptors();
			}
			
			int no_y = training_result_.cols();
			if (no_y == 0) no_y = y_transformations_.cols(); // correct no because transformation information will have to by read anyway when reading this model later ...
			
			out<<"# model-type_\tno of featues in input data\tselected featues\tno of response variables\tcentered descriptors?\tno of classes"<<endl;
			out<<type_<<"\t"<<data->getNoDescriptors()<<"\t"<<sel_features<<"\t"<<no_y<<"\t"<<centered_data<<"\t"<<no_substances_.size()<<"\n\n";
			
			out<<"# model-parameters"<<endl;  /// write model parameters 
			vector<double> v = getParameters();
			for (unsigned int i = 0; i < v.size(); i++)  
			{
				out<<v[i]<<"\t";
			}
			out<<endl;
			
			out<<"\n# ID\tdescriptor-name\tcoefficient(s)\t";
			if (centered_data)
			{
				out<<"mean of desc.\tstddev of desc.\t";
			}
			if (stderr)
			{
				out<<"stderr(s) of coeff.";
			}
			out<<endl;
			
			if (!descriptor_IDs_.empty())  /// write descriptors and information about their transformation
			{
				std::multiset<unsigned int>::iterator d_it = descriptor_IDs_.begin();
				for (int i = 0; i < training_result_.rows(); i++, ++d_it)
				{
					out<<String(*d_it)<<"\t"<<descriptor_names_[i]<<"\t";
				
					for (int j = 0; j < training_result_.cols(); j++) 
					{
						out<<training_result_(i, j)<<"\t";
					}
					if (centered_data)
					{
						out<<descriptor_transformations_(0, i)<<"\t"<<descriptor_transformations_(1, i)<<"\t"; 
					}
					out <<"\n";
				}
			}
			else
			{
				for (int i = 0; i < training_result_.rows(); i++)
				{
					out<<String(i)<<"\t"<<descriptor_names_[i]<<"\t";

					for (int j = 0; j < training_result_.cols(); j++) 
					{
						out<<training_result_(i, j)<<"\t";
					}
					if (centered_data)
					{
						out<<descriptor_transformations_(0, i)<<"\t"<<descriptor_transformations_(1, i)<<"\t"; 
					}
					out <<"\n";
				}
			}	
			out<<endl;
			
			out<<"# class-labels_\n";
			for (unsigned int i = 0; i < labels_.size(); i++) /// write class-labels_
			{
				out<<labels_[i]<<"\t";
			}
			out<<endl;
			
			out<<"\n# no of substances of each class\n";
			for (unsigned int i = 0; i < no_substances_.size(); i++)  /// write numbers of substances of each class
			{
				out<<no_substances_[i]<<"\t";
			}
			out<<endl;
			
			out.close();
		}


		void LogitModel::readFromFile(string filename)
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
			bool centered_y = line0.getField(5, "\t").toInt();
			training_result_.resize(no_descriptors, no_y);
			descriptor_names_.clear();
			if (centered_data)
			{
				descriptor_transformations_.resize(2, no_descriptors); 
			}
			getline(input, line0);  // skip empty line
			getline(input, line0);  // skip comment line
			
			getline(input, line0);	   /// read model parameters
			int c = line0.countFields("\t");
			vector<double> v;
			for (int i = 0; i < c; i++)
			{
				v.push_back(line0.getField(i, "\t").toDouble());
			}
			setParameters(v);
			getline(input, line0);  // skip empty line
			
			if (centered_y)
			{
				y_transformations_.resize(2, no_y); 
				for (int i = 0; i < no_y; i++)
				{
					getline(input, line0);
					y_transformations_(0, i) = line0.getField(0, "\t").toDouble(); 	
					y_transformations_(1, i) = line0.getField(1, "\t").toDouble(); 
				}
				getline(input, line0);  // skip empty line 
			}
			getline(input, line0);  // skip comment line 
			
			for (int i = 0; !input.eof(); i++)
			{
				String line;
				getline(input, line);
				if (line == ""){break; }
				unsigned int id = line.getField(0, "\t").toInt();
				descriptor_IDs_.insert(id);
				descriptor_names_.push_back(line.getField(1, "\t"));
				int j = 2;
				for (; j < 2+no_y; j++)
				{	
					training_result_(i, j-1) = line.getField(j, "\t").toDouble();
				}
				if (centered_data)
				{
					descriptor_transformations_(0, i) = line.getField(j, "\t").toDouble(); 
					descriptor_transformations_(1, i) = line.getField(j+1, "\t").toDouble(); 
				}
					
			}
		}
	}
}
