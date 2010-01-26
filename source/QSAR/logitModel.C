/* logitModel.C
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

#include <BALL/QSAR/logitModel.h>

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
			if(descriptor_matrix_.Ncols()==0)
			{
				throw Exception::InconsistentUsage(__FILE__,__LINE__,"Data must be read into the model before training!");
			}
			readLabels();
			
			int lines=descriptor_matrix_.Nrows();
			int col=descriptor_matrix_.Ncols();
			training_result_.resize(col,Y_.Ncols());
				
			for(int c=1; c<=Y_.Ncols();c++)	
			{
				Vector<double> beta(col);
				beta = 0;
				for( int b=0;b<100;b++)
				{
					// calculate matrix W and vector p
					Vector<double> p(lines);
					Matrix<double> W(lines,lines); W=0; // diagonal matrix
					for(int i=1; i<=lines;i++)
					{
						Vector<double> v; v.setVectorType(0);
						descriptor_matrix_.copyRowToVector(v,i);
						double nom = exp(v*beta);
						p(i)=nom/(1+nom);
						W(i,i)=p(i)*(1/(1+nom));
					}
					Vector<double> beta_old=beta;
					
					Matrix<double> I; I.setToIdentity(col); 
					I*=0.0001;
					Matrix<double> xwx;
					xwx = descriptor_matrix_.t();
					xwx*=W;
					xwx*=descriptor_matrix_;
					xwx+= I;
					
					for(uint i=1; i<Y_.getRowCount(); i++)
					{
						p(i) = Y_(c,i)-p(i);
					}
					beta += xwx.i()*descriptor_matrix_.t()*p;
					//beta = xwx.i()*descriptor_matrix_.t()*W*(descriptor_matrix_*beta+W.i()*(Y_.Column(c)-p));
					
					if (Statistics::euclDistance(beta,beta_old)/Statistics::euclNorm(beta)<0.01) 
					{ 
						break;
					}
				}
				training_result_.copyVectorToColumn(beta,c);
			}
		}


		BALL::Vector<double> LogitModel::predict(const vector<double>& substance, bool transform)
		{
			if(training_result_.Ncols()==0)
			{
				throw Exception::InconsistentUsage(__FILE__,__LINE__,"Model must be trained before it can predict the activitiy of substances!");
			}

			Vector<double> v=getSubstanceVector(substance, transform);
			Vector<double> res=v*training_result_;

			if(transform)
			{
				backTransformPrediction(res);
			}

			for(uint i=1; i<=res.getSize();i++)
			{
				if(res(i)>=0)
				{
					res(i)=1;
				}
				else
				{
					res(i)=0;
				}
			}

			return res;	
		}


		void LogitModel::saveToFile(string filename)
		{
			if(training_result_.Nrows()==0)
			{
				throw Exception::InconsistentUsage(__FILE__,__LINE__,"Model must have been trained before the results can be saved to a file!");
			}
			ofstream out(filename.c_str());
			
			bool centered_data = 0;
			if(descriptor_transformations_.Ncols()!=0)
			{
				centered_data=1;
			}
			int sel_features=descriptor_IDs_.size();
			if(sel_features==0)
			{
				sel_features = data->getNoDescriptors();
			}
			
			int no_y = training_result_.Ncols();
			if(no_y==0) no_y = y_transformations_.Ncols(); // correct no because transformation information will have to by read anyway when reading this model later ...
			
			out<<"# model-type_\tno of featues in input data\tselected featues\tno of response variables\tcentered descriptors?\tno of classes"<<endl;
			out<<type_<<"\t"<<data->getNoDescriptors()<<"\t"<<sel_features<<"\t"<<no_y<<"\t"<<centered_data<<"\t"<<no_substances_.size()<<"\n\n";
			
			out<<"# model-parameters"<<endl;  /// write model parameters 
			vector<double> v = getParameters();
			for(unsigned int i=0;i<v.size();i++)  
			{
				out<<v[i]<<"\t";
			}
			out<<endl;
			
			out<<"\n# ID\tdescriptor-name\tcoefficient(s)\t";
			if(centered_data)
			{
				out<<"mean of desc.\tstddev of desc.\t";
			}
			if(stderr)
			{
				out<<"stderr(s) of coeff.";
			}
			out<<endl;
			
			if(!descriptor_IDs_.empty())  /// write descriptors and information about their transformation
			{
				descriptor_IDs_.front();
				for(int i=0; i<training_result_.Nrows();i++)
				{
					out<<String(descriptor_IDs_.next())<<"\t"<<descriptor_names_[i]<<"\t";
				
					for(int j=1; j<=training_result_.Ncols();j++) 
					{
						out<<training_result_(i+1,j)<<"\t";
					}
					if(centered_data)
					{
						out<<descriptor_transformations_(1,i+1)<<"\t"<<descriptor_transformations_(2,i+1)<<"\t";
					}
					out <<"\n";
				}
			}
			else
			{
				for(int i=0; i<training_result_.Nrows();i++)
				{
					out<<String(i)<<"\t"<<descriptor_names_[i]<<"\t";

					for(int j=1; j<=training_result_.Ncols();j++) 
					{
						out<<training_result_(i+1,j)<<"\t";
					}
					if(centered_data)
					{
						out<<descriptor_transformations_(1,i+1)<<"\t"<<descriptor_transformations_(2,i+1)<<"\t";
					}
					out <<"\n";
				}
			}	
			out<<endl;
			
			out<<"# class-labels_\n";
			for(unsigned int i=0; i<labels_.size();i++) /// write class-labels_
			{
				out<<labels_[i]<<"\t";
			}
			out<<endl;
			
			out<<"\n# no of substances of each class\n";
			for(unsigned int i=0;i<no_substances_.size();i++)  /// write numbers of substances of each class
			{
				out<<no_substances_[i]<<"\t";
			}
			out<<endl;
			
			out.close();
		}


		void LogitModel::readFromFile(string filename)
		{
			ifstream input(filename.c_str());
			if(!input)
			{
				throw BALL::Exception::FileNotFound(__FILE__,__LINE__,filename);
			}	
			
			String line0;
			getline(input,line0);  // skip comment line 
			getline(input,line0);  // read read line containing model specification
			
			
			if(line0.getField(0,"\t")!=type_)
			{
				String e = "Wrong input data! Use training data file generated by a ";
				e = e + type_ + " model !";
				throw Exception::WrongDataType(__FILE__,__LINE__,e.c_str());
			}
			
			int no_descriptors = line0.getField(2,"\t").toInt();
			int no_y = line0.getField(3,"\t").toInt();
			bool centered_data = line0.getField(4,"\t").toInt();
			bool centered_y = line0.getField(5,"\t").toInt();
			training_result_.ReSize(no_descriptors,no_y);
			descriptor_names_.clear();
			if(centered_data)
			{
				descriptor_transformations_.ReSize(2,no_descriptors);
			}
			getline(input,line0);  // skip empty line
			getline(input,line0);  // skip comment line
			
			getline(input,line0);	   /// read model parameters
			int c = line0.countFields("\t");
			vector<double> v;
			for(int i=0; i<c; i++)
			{
				v.push_back(line0.getField(i,"\t").toDouble());
			}
			setParameters(v);
			getline(input,line0);  // skip empty line
			
			if(centered_y)
			{
				y_transformations_.ReSize(2,no_y);
				for(int i=1; i<=no_y; i++)
				{
					getline(input,line0);
					y_transformations_(1,i)=line0.getField(0,"\t").toDouble();	
					y_transformations_(2,i)=line0.getField(1,"\t").toDouble();
				}
				getline(input,line0);  // skip empty line 
			}
			getline(input,line0);  // skip comment line 
			
			for(int i=1; !input.eof(); i++)
			{
				String line;
				getline(input,line);
				if(line==""){break;}
				unsigned int id = line.getField(0,"\t").toInt();
				descriptor_IDs_.push_back(id);
				descriptor_names_.push_back(line.getField(1,"\t"));
				int j=2;
				for(; j<2+no_y; j++)
				{	
					training_result_(i,j-1) = line.getField(j,"\t").toDouble();
				}
				if(centered_data)
				{
					descriptor_transformations_(1,i)= line.getField(j,"\t").toDouble();
					descriptor_transformations_(2,i)=line.getField(j+1,"\t").toDouble();
				}
					
			}
		}
	}
}