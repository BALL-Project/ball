/* classificationModel.C
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

#include <BALL/QSAR/classificationModel.h>

namespace BALL
{
	namespace QSAR
	{
				
		ClassificationModel::ClassificationModel(const QSARData& q) : Model(q) 
		{
			validation=new ClassificationValidation(this);
			model_val=validation;
		}


		ClassificationModel::~ClassificationModel()
		{
			delete validation;
		}


		void ClassificationModel::readLabels()
		{
			std::multiset<int> l;
			for(int i=1; i<=Y_.Nrows();i++)
			{
				for(int j=1;j<=Y_.Ncols();j++)
				{
					int label=static_cast<int>(Y_(i,j));
					if(label!=Y_(i,j))
					{
						throw Exception::WrongDataType(__FILE__,__LINE__,"Some class labels are not discrete values!! Creation of a classification model is therefore not possible!");
					}
					if(l.find(label) == l.end())  // for classification experiments, Y will contain only ints
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

		vector<int> ClassificationModel::getClassLabels()
		{
			return labels_;	
		}

		void ClassificationModel::readClassInformationFromFile(ifstream& input, int no_classes)
		{
			labels_.clear();
			no_substances_.clear();
			
			String line;
			getline(input,line);  // skip comment line 
			getline(input,line);    
			for(int i=0;i<no_classes;i++)
			{
				labels_.push_back(line.getField(i,"\t").toInt());
			}	
			getline(input,line);  // skip empty line
			getline(input,line);  // skip comment line 
			getline(input,line);
			for(int i=0; i<no_classes; i++)
			{
				int n = line.getField(i,"\t").toInt();
				no_substances_.push_back(n);
			}
			getline(input,line);  // skip empty line 
		}


		void ClassificationModel::saveClassInformationToFile(ofstream& out)
		{
			out<<"# class-labels_\n";
			for(unsigned int i=0; i<labels_.size();i++) // write class-labels_
			{
				out<<labels_[i]<<"\t";
			}
			out<<endl<<endl;
			
			out<<"# no of substances of each class\n";
			for(unsigned int i=0;i<no_substances_.size();i++)  // write numbers of substances of each class
			{
				out<<no_substances_[i]<<"\t";
			}
			out<<endl<<endl;
		}


		void ClassificationModel::equalSpaceDiscretization(uint bins, Matrix<double>& discretization_information)
		{
			uint no_features = descriptor_matrix_.Ncols();
			uint no_compounds = descriptor_matrix_.Nrows();
			
			discretization_information.ReSize(2,no_features);
			discretization_information.setRow(1,1e10); // minimum of each feature in first row
			discretization_information.setRow(2,-1e10); // maximum of each feature in second row
			
			// find minimum and maximum of each feature
			for(uint i=1;i<=no_features;i++)
			{
				for(uint j=1;j<=no_compounds;j++)
				{
					if(descriptor_matrix_(j,i)<discretization_information(1,i))
					{
						discretization_information(1,i) = descriptor_matrix_(j,i);
					}
					if(descriptor_matrix_(j,i)>discretization_information(2,i))
					{
						discretization_information(2,i) = descriptor_matrix_(j,i);
					}
				}
			}

			// transform each feature value to a discrete value
			for(uint i=1;i<=no_features;i++)
			{
				double step_width = (discretization_information(2,i)-discretization_information(1,i)) / bins;
				for(uint j=1;j<=no_compounds;j++)
				{
					uint feat_bucket = (uint)((descriptor_matrix_(j,i)-discretization_information(1,i)) / step_width);
					if(feat_bucket>=bins) feat_bucket=bins-1; // for max.
					descriptor_matrix_(j,i) = feat_bucket;
				}	
			}
		}


		void ClassificationModel::equalSpaceDiscretizationTestData(Vector<double>& compound, uint bins, const Matrix<double>& discretization_information)
		{
			if(compound.getSize()!=discretization_information.getColumnCount())
			{
				throw BALL::Exception::GeneralException(__FILE__,__LINE__,"Discretization error","no of features of test compound and of discretized training data are different!");
			}
			
			uint no_features = compound.getSize();

			for(uint i=1;i<=no_features;i++)
			{
				double step_width = (discretization_information(2,i)-discretization_information(1,i))/bins;
				int feat_bucket = (int)((compound(i)-discretization_information(1,i))/step_width);
				if(feat_bucket<1) feat_bucket=0;
				else if((uint)feat_bucket>=bins) feat_bucket=bins-1;
				compound(i) = feat_bucket;
			}
		}
	}
}