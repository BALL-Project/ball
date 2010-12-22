/* ldaModel.C
 * 
 * Copyright (C) 2009 Marcel Schumann
 * 
 * This file is part of QuEasy -- A Toolbox for Automated QSAR Model
 * Construction and Validation.
 * QuEasy is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 * 
 * QuEasy is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//

#include <BALL/QSAR/ldaModel.h>
#include <BALL/MATHS/LINALG/matrixInverter.h>

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
			if (descriptor_matrix_.Ncols() == 0)
			{
				throw Exception::InconsistentUsage(__FILE__, __LINE__, "Data must be read into the model before training!"); 
			}
			readLabels();
			
			// map values of Y to their index
			map<int, uint> label_to_pos; 
			for (uint i = 0; i < labels_.size(); i++)
			{
				label_to_pos.insert(make_pair(labels_[i], i));
			}

			// calculate sigma_ = covariance matrix of descriptors
			sigma_.resize(descriptor_matrix_.Ncols(), descriptor_matrix_.Ncols());
			for (int i = 1; i <= descriptor_matrix_.Ncols(); i++)
			{
				double mi = Statistics::getMean(descriptor_matrix_, i);
				for (int j = 1; j <= descriptor_matrix_.Ncols(); j++)
				{
					sigma_(i, j) = Statistics::getCovariance(descriptor_matrix_, i, j, mi);
				}
			}
			Matrix<double> I; I.setToIdentity(sigma_.Ncols());
			I *= lambda_;
			sigma_ += I;

			MatrixInverter<double, StandardTraits> inverter(sigma_);
			inverter.computeInverse();

			sigma_ = inverter.getInverse();

			mean_vectors_.resize(Y_.Ncols());
			no_substances_.clear();
			no_substances_.resize(labels_.size(), 0);
			for (int c = 0; c < Y_.Ncols(); c++)
			{
				vector < int > no_substances_c(labels_.size(), 0);  // no of substances in each class for activitiy c
				mean_vectors_[c].resize(labels_.size(), descriptor_matrix_.Ncols());
				mean_vectors_[c] = 0;
				
				for (int i = 1; i <= descriptor_matrix_.Nrows(); i++) // calculate sum vector of each class
				{
					int yi = static_cast < int > (Y_(i, c+1)); // Y_ will contains only ints for classifications
					int pos = label_to_pos.find(yi)->second; 
					
					for (int j = 1; j < descriptor_matrix_.Ncols(); j++)
					{
						mean_vectors_[c](pos+1, j) += descriptor_matrix_(i, j);
					}
					
					if (c == 0) no_substances_c[pos]++; 
					
				}
				
				for (int i = 1; i <= mean_vectors_[c].Nrows(); i++) // calculate mean vector of each class
				{
					if (no_substances_c[i-1] == 0)
					{
						mean_vectors_[c].setRow(i, exp(100.));  // set mean of classes NOT occurring for activitiy c to inf
					}
					
					for (int j = 1; j < descriptor_matrix_.Ncols(); j++)
					{
						mean_vectors_[c](i, j) = mean_vectors_[c](i, j)/no_substances_c[i-1];
					}
				}
				
				for (unsigned int i = 0; i < no_substances_.size(); i++) // update overall number of substances per class
				{
					no_substances_[i] += no_substances_c[i];
				}
			}
			
		}


		BALL::Vector<double> LDAModel::predict(const vector<double> & substance, bool transform)
		{
			if (sigma_.Ncols() == 0)
			{
				throw Exception::InconsistentUsage(__FILE__, __LINE__, "Model must be trained before it can predict the activitiy of substances!"); 
			}
			// search class to which the given substance has the smallest distance
			Vector<double> s = getSubstanceVector(substance, transform); // dim: 1xm
			
			int min_k = 0;
			double min_dist = 99999999;
			Vector<double> result(mean_vectors_.size());
			
			for (unsigned int c = 0; c < mean_vectors_.size(); c++)
			{
				for (int k = 1; k <= mean_vectors_[c].Nrows(); k++)
				{
					Vector<double> diff(s.getSize()); 
					for (uint i = 1; i <= s.getSize(); i++)
					{
						diff(i) = s(i)-mean_vectors_[c](k, i); 
					}
					double dist = diff*sigma_*diff.t(); 
					if (dist < min_dist)
					{
						min_dist = dist;
						min_k = k;
					}
				}
				result(c+1) = labels_[min_k-1];
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
			if (sigma_.Nrows() == 0) trained = 0; 
			
			ofstream out(filename.c_str());
			
			bool centered_data = 0;
			bool centered_y = 0;
			if (descriptor_transformations_.Ncols() != 0)
			{
				centered_data = 1;
				if (y_transformations_.Ncols() != 0)
				{
					centered_y = 1;
				}
			}
			
			int sel_features = descriptor_IDs_.size();
			if (sel_features == 0)
			{
				sel_features = data->getNoDescriptors();
			}
			
			
			int no_y = sigma_.Ncols();
			if (no_y == 0) no_y = y_transformations_.Ncols(); // correct no because transformation information will have to by read anyway when reading this model later ...
			
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
