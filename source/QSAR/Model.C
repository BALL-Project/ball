/* Model.C
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

#include <BALL/QSAR/Model.h>
#include <BALL/QSAR/kernelModel.h>
#include <BALL/QSAR/registry.h>

namespace BALL
{
	namespace QSAR
	{

		Model::Model(const QSARData& q)
		{
			data=&q;
			default_no_opt_steps_ = 30;
		}


		Model::~Model()
		{
		}


		void Model::copyData(const Model& m)
		{
			descriptor_matrix_=m.descriptor_matrix_;
			substance_names_=m.substance_names_;
			descriptor_names_=m.descriptor_names_;
			descriptor_transformations_ = m.descriptor_transformations_;
			y_transformations_ = m.y_transformations_;
			Y_=m.Y_;
			descriptor_IDs_=m.descriptor_IDs_;
		}


		void Model::operator=(const Model& m)
		{
			if(type_!=m.type_)
			{
				String message = "Wrong usage of '='-operator. A copy of a "+m.type_+" model is no "+type_+" model!";
				throw Exception::InconsistentUsage(__FILE__,__LINE__,message.c_str());
			}
			descriptor_names_=m.descriptor_names_;
			descriptor_transformations_ = m.descriptor_transformations_;
			y_transformations_ = m.y_transformations_;
			descriptor_IDs_=m.descriptor_IDs_;
			vector<double> v=m.getParameters();
			setParameters(v);
			model_val->selectStat(m.model_val->getStat()); // set validation statistic
		}
			

		const string* Model::getType()
		{
			return &type_;
		}


		void Model::copyDescriptorIDs(const Model& m)
		{
			descriptor_IDs_=m.descriptor_IDs_;
		}


		void Model::deleteDescriptorIDs()
		{
			std::multiset<unsigned int> s;
			descriptor_IDs_=s;
		}


		void Model::readTrainingData()
		{
			if(data->descriptor_matrix_.size()==0)
			{
				throw Exception::InconsistentUsage(__FILE__,__LINE__,"Data must be fetched from input-files by QSARData before a model can use this data as training input!");
			}
			
			bool fs=0; // has feature selection being done?
			if(!descriptor_IDs_.empty())
			{
				fs=1;
			}
			std::multiset<unsigned int>::iterator it=descriptor_IDs_.begin();

			int col=data->descriptor_matrix_.size();
			if(fs)
			{
				col=descriptor_IDs_.size();
			}
			int lines=data->descriptor_matrix_[0].size();
			
			descriptor_matrix_.ReSize(lines,col);
			
			substance_names_=data->substance_names_;
			descriptor_names_.resize(col);
			Y_.ReSize(lines, data->Y_.size());
			bool transform=0; bool y_transform = 0;
			
			if(data->descriptor_transformations_.size()!=0)
			{
				transform=1;
				descriptor_transformations_.ReSize(2,col);
				if(data->y_transformations_.size()!=0)
				{
					y_transformations_.ReSize(2,data->Y_.size());
					y_transform=1;
				}
			}
			bool xlabels=0;
			if(data->column_names_.size()!=0)
			{
				xlabels=1;
			}
			
			// copy activity values of all substances and transformation of each activity column
			for(unsigned int i=0; i<data->Y_.size();i++)
			{
				for(unsigned int j=0;j<data->Y_[0].size();j++)
				{
					Y_(j+1,i+1)=data->Y_[i][j];
				}
				if(y_transform)
				{
					y_transformations_(1,i+1)=data->y_transformations_[i][0];
					y_transformations_(2,i+1)=data->y_transformations_[i][1];
				}
			}
			int t=0; // index in line of training data

			it=descriptor_IDs_.begin();
			
			// copy selected descriptors, their names and the information about their transformations (mean and stddev of each descriptor)
			for(unsigned int j=0; j<data->descriptor_matrix_.size() && (!fs || it!=descriptor_IDs_.end()); j++)
			{	
				// set only those columns that belong to selected descriptors
				if ( (fs && *it==j) || !fs )
				{
					if(xlabels)
					{
						descriptor_names_[t]=data->column_names_[j];
					}
					if(transform)
					{
						descriptor_transformations_(1,t+1)=data->descriptor_transformations_[j][0];
						descriptor_transformations_(2,t+1)=data->descriptor_transformations_[j][1];
					}		
					 // set each cell of the current column j
					for(int i=0; i<lines; i++)
					{
						//cout<<"trying to write to cell "<<i+1<<","<<t+1<<" from cell "<<j<<","<<i<<endl;
						descriptor_matrix_(i+1,t+1)=data->descriptor_matrix_[j][i];
					}
					t++;
					if(fs)
					{
						it++;
					}
				}
			}
		}


		void Model::readDescriptorInformation()
		{
			bool fs=0; // has feature selection being done?
			if(!descriptor_IDs_.empty())
			{
				fs=1;
			}
			int col=data->descriptor_matrix_.size();
			if(fs)
			{
				col=descriptor_IDs_.size();
			}
			descriptor_names_.resize(col);
			bool transform=0; bool y_transform = 0;
			if(data->descriptor_transformations_.size()!=0)
			{
				transform=1;
				descriptor_transformations_.ReSize(2,col);
				if(data->y_transformations_.size()!=0)
				{
					y_transformations_.ReSize(2,data->Y_.size());
					y_transform=1;
				}
			}
			bool xlabels=0;
			if(data->column_names_.size()!=0)
			{
				xlabels=1;
			}
				
			int t=0; // index in line of training data
			std::multiset<unsigned int>::iterator it=descriptor_IDs_.begin();
			// copy selected descriptors, their names and the information about their transformations (mean and stddev of each descriptor)
			for(unsigned int j=0; j<data->descriptor_matrix_.size() && (!fs || it!=descriptor_IDs_.end()); j++)
			{	
				// set only those columns that belong to selected descriptors
				if ( (fs && *it==j) || !fs )
				{
					if(xlabels)
					{
						descriptor_names_[t]=data->column_names_[j];
					}
					if(transform)
					{
						descriptor_transformations_(1,t+1)=data->descriptor_transformations_[j][0];
						descriptor_transformations_(2,t+1)=data->descriptor_transformations_[j][1];
					}
					t++;
					if(fs)
					{
						it++;
					}
				}
			}
		}


		BALL::Vector<double> Model::getSubstanceVector(const vector<double>& substance, bool transform)
		{
			if(transform==1 && descriptor_transformations_.Ncols()==0)
			{
				transform=0;
				//throw Exception::InconsistentUsage(__FILE__,__LINE__,"Transformation of test data requested although no scaling of training data was done!!");
			}
			
			if( (data!=NULL && data->getNoDescriptors()!=substance.size()) || (data==NULL&&substance.size()<=*(descriptor_IDs_.end()--)) )	
			{
				String message="For compounds whose activity is to be predicted, the same features must be available as for the training data!\n";
				message+="No of features of given compound: ";
				message+=String(substance.size())+"\n";
				message+="No of required features: ";
				if(data) message+=String(data->getNoDescriptors());
				else message+=String(*(descriptor_IDs_.end()--));
				throw Exception::InconsistentUsage(__FILE__,__LINE__,message.c_str());
			}

			bool fs=0; // has feature selection being done?
			if(!descriptor_IDs_.empty())
			{
				fs=1;
			}
			std::multiset<unsigned int>::iterator it=descriptor_IDs_.begin();

			int t=0; // index in line of test data
			int length=descriptor_matrix_.Ncols();
			if(fs)
			{	
				length=descriptor_IDs_.size();
			}
			
			Vector<double> v(length);
			v.setVectorType(0); // this is a row, not a column-vector
			
			// if no feature selection was done, i.e. if descriptor_IDs_ is empty
			if (!fs)
			{
				if(transform)
				{
					for(int i=0; i<length;i++)
					{
						double stddev=descriptor_transformations_(2,i+1);
						if(stddev==0) {stddev=0.001;}
						v(i+1)=(substance[i]-descriptor_transformations_(1,i+1))/stddev;
					}
				}
				else
				{
					for(int i=0; i<length;i++)
					{
						v(i+1)=substance[i];
					}
				}
			}

			//if feature selection was done, consider only those descriptors whose IDs are in descriptor_IDs_
			else
			{
				if(transform)
				{
					for(int i=0; i<length;i++)
					{
						t=*it;
						double stddev=descriptor_transformations_(2,i+1);
						if(stddev==0) {stddev=0.001;}
						v(i+1)=(substance[t]-descriptor_transformations_(1,i+1))/stddev;
						it++;
					}
				}
				else
				{
					for(int i=0; i<length;i++)
					{
						t=*it;
						v(i+1)=substance[t];
						it++;
					}
				}
			}
			return v;	
		}


		BALL::Vector<double> Model::getSubstanceVector(const Vector<double>& substance, bool transform)
		{
			if(transform==1 && descriptor_transformations_.Ncols()==0)
			{
				throw Exception::InconsistentUsage(__FILE__,__LINE__,"Transformation of test data requested although no scaling of training data was done!!");
			}
			if( (data!=NULL && data->getNoDescriptors()!=substance.getSize()) || (data==NULL&&substance.getSize()<=*(descriptor_IDs_.end()--)) )
			{
				String message="For compounds whose activity is to be predicted, the same number of features must be present than within the training data!\n";
				message+="No of features of given compound: ";
				message+=String(substance.getSize())+"\n";
				message+="No of required features: ";
				if(data) message+=String(data->getNoDescriptors());
				else message+=String(*(descriptor_IDs_.end()--));
				throw Exception::InconsistentUsage(__FILE__,__LINE__,message.c_str());
			}
			
			bool fs=0; // has feature selection being done?
			if(!descriptor_IDs_.empty())
			{
				fs=1;
			}
			std::multiset<unsigned int>::iterator it=descriptor_IDs_.begin();

			int t=0; // index in line of test data
			int length=descriptor_matrix_.Ncols();
			if(fs)
			{	
				length=descriptor_IDs_.size();
			}
			
			Vector<double> v(length);
			v.setVectorType(0); // this is a row, not a column-vector
			
			// if no feature selection was done, i.e. if descriptor_IDs_ is empty
			if (!fs)
			{
				if(transform)
				{
					for(int i=1; i<=length;i++)
					{
						double stddev=descriptor_transformations_(2,i);
						if(stddev==0) {stddev=0.001;}
						v(i)=(substance(i)-descriptor_transformations_(1,i))/stddev;
					}
				}
				else
				{
					v=substance;
				}
			}

			//if feature selection was done, consider only those descriptors whose IDs are in descriptor_IDs_
			else
			{
				if(transform)
				{
					for(int i=1; i<=length;i++)
					{
						t=*it; // descr. IDs start at 0 !
						double stddev=descriptor_transformations_(2,i);
						if(stddev==0) {stddev=0.001;}
						v(i)=(substance(t+1)-descriptor_transformations_(1,i))/stddev;
						it++;
					}
				}
				else
				{
					for(int i=1; i<=length;i++)
					{
						t=*it;
						v(i)=substance(t+1);
						it++;
					}
				}
			}
			return v;	
		}


		void Model::backTransformPrediction(Vector<double>& pred)
		{
			for(int i=1; i<=y_transformations_.Ncols();i++)
			{
				double stddev=y_transformations_(2,i);
				pred(i)=pred(i)*stddev+y_transformations_(1,i);
			}	
		}


		const BALL::Matrix<double>* Model::getDescriptorMatrix()
		{
			return &descriptor_matrix_;
		}
						

		const vector<string>* Model::getSubstanceNames()
		{ 
			return &substance_names_;
		}


		const vector<string>* Model::getDescriptorNames()
		{
			return &descriptor_names_;
		}

			
		const BALL::Matrix<double>* Model::getY()
		{
			return &Y_;
		}


		void Model::setDescriptorIDs(const std::multiset<unsigned int>& sl)
		{
			descriptor_IDs_=sl;
		}


		std::multiset<unsigned int>* Model::getDescriptorIDs()
		{
			return &descriptor_IDs_;
		}

		vector<double> Model::getParameters() const
		{
			vector<double> d(0);
			return d;
		}

		void Model::setDataSource(const QSARData* q)
		{
			data = q;
		}

		void Model::addLambda(Matrix<double>& matrix, double& lambda)
		{
			if(matrix.Nrows()!=matrix.Ncols())
			{
				throw BALL::Exception::GeneralException(__FILE__,__LINE__,"Model::addLambda error: ", "Lambda can only be added to a square matrix!");
			}
						
			for(int i=1;i<matrix.Nrows();i++)
			{
				matrix(i,i)+=lambda;
			}
		}
			
		void Model::readMatrix(Matrix<double>& mat, ifstream& in, uint lines, uint col)
		{
			mat.ReSize(lines,col);
			String line;
			
			for(uint i=1;i<=lines;i++)
			{
				//getline(in,line);
				for(uint j=1; j<=col;j++)
				{
					String s;
					in>>s;
					mat(i,j)=s.toDouble(); // = line.getField(j,"\t").toDouble();
				}
			}
			getline(in,line); // read the rest of the last matrix-line
		}

		void Model::readVector(Vector<double>& vec, ifstream& in, uint no_cells, bool column_vector)
		{
			vec.resize(no_cells);
			vec.setVectorType(column_vector);
			String line;
			
			for(uint i=1;i<=no_cells;i++)
			{
				String s;
				in>>s;
				vec(i)=s.toDouble(); // = line.getField(j,"\t").toDouble();
			}
			getline(in,line); // read the rest of the last matrix-line
		}

		void Model::readModelParametersFromFile(ifstream& input)
		{
			String line;
			getline(input,line);  // skip comment line
			getline(input,line);
			int c = line.countFields("\t");
			vector<double> v;
			for(int i=0; i<c; i++)
			{
				v.push_back(line.getField(i,"\t").toDouble());
			}
			setParameters(v);
			getline(input,line);  // skip empty line
		}

		void Model::saveModelParametersToFile(ofstream& out)
		{
			out<<"# model-parameters"<<endl;
			vector<double> v = getParameters();
			for(unsigned int i=0;i<v.size();i++)
			{
				out<<v[i]<<"\t";
			}
			out<<endl<<endl;
		}


		void Model::readDescriptorInformationFromFile(ifstream& input, int no_descriptors, bool transformation)
		{
			descriptor_names_.clear();
			if(transformation) descriptor_transformations_.resize(2,no_descriptors);
			else descriptor_transformations_.resize(0,0);
			String line;
			getline(input,line);  // skip comment line
			for(int i=1; i<=no_descriptors; i++)
			{
				getline(input,line);
				unsigned int id = (unsigned int) line.getField(0,"\t").toInt();
				descriptor_IDs_.insert(id);
				descriptor_names_.push_back(line.getField(1,"\t"));
				if(transformation)
				{
					descriptor_transformations_(1,i)= line.getField(2,"\t").toDouble();
					descriptor_transformations_(2,i)=line.getField(3,"\t").toDouble();
				}
			}
			getline(input,line);  // skip empty line	
		}


		void Model::saveDescriptorInformationToFile(ofstream& out)
		{
			out<<"# ID\tdescriptor-name\tcoefficient(s)\t";
			
			bool centered_data = (descriptor_transformations_.Ncols()>0);
			
			if(centered_data)
			{
				out<<"mean of desc.\tstddev of desc.\t";
			}
			if(stderr)
			{
				out<<"stderr(s) of coeff.";
			}
			out<<endl;
			
			if(!descriptor_IDs_.empty())  // write information about transformation of descriptors
			{
				std::multiset<unsigned int>::iterator d_it = descriptor_IDs_.begin();
				for(uint i=0; i<descriptor_IDs_.size();i++, ++d_it)
				{
					out<<String(*d_it)<<"\t"<<descriptor_names_[i]<<"\t";
					if(centered_data)
					{
						out<<descriptor_transformations_(1,i+1)<<"\t"<<descriptor_transformations_(2,i+1)<<"\t";
					}
					out <<"\n";
				}
			}
			else
			{
				for(uint i=0; i<descriptor_names_.size();i++)
				{
					out<<String(i)<<"\t"<<descriptor_names_[i]<<"\t";
					if(centered_data)
					{
						out<<descriptor_transformations_(1,i+1)<<"\t"<<descriptor_transformations_(2,i+1)<<"\t";
					}
					out <<"\n";
				}
			}
			out<<endl;
		}


		void Model::readResponseTransformationFromFile(ifstream& input, int no_y)
		{
			y_transformations_.ReSize(2,no_y);
			String line;
			for(int i=1; i<=no_y; i++)
			{
				getline(input,line);
				y_transformations_(1,i)=line.getField(0,"\t").toDouble();	
				y_transformations_(2,i)=line.getField(1,"\t").toDouble();
			}
			getline(input,line);  // skip empty line
		}


		void Model::saveResponseTransformationToFile(ofstream& out)
		{
			if(y_transformations_.Ncols()!=0)
			{
				for(int i=1;i<=y_transformations_.Ncols();i++)
				{
					out<<y_transformations_(1,i)<<"\t"<<y_transformations_(2,i)<<endl;
				}
				out<<endl;
			}	
		}


		bool Model::optimizeParameters(int k)
		{
			return optimizeParameters(k, default_no_opt_steps_);
		}


		void Model::getUnnormalizedFeatureValue(int compound, int feature, double& return_value)
		{
			if(compound<1 || feature<1 || compound>(int)descriptor_matrix_.getRowCount() || feature>(int)descriptor_matrix_.getColumnCount())
			{
				cout<<"Model::getUnnormalizedFeatureValue(): Specified compound or feature ID is out of range!"<<endl;
				BALL::Exception::OutOfRange e(__FILE__,__LINE__);
				e.setMessage("Specified compound or feature ID is out of range!");
				throw e;
			}

			return_value = descriptor_matrix_(compound,feature);
			if(descriptor_transformations_.getColumnCount()>0)
			{
				return_value *= descriptor_transformations_(2,feature); // stddev
				return_value += descriptor_transformations_(1,feature); // mean
			}
		}

		void Model::getUnnormalizedResponseValue(int compound, int response, double& return_value)
		{
			if(compound<1 || response<1 || compound>(int)Y_.getRowCount() || response>(int)Y_.getColumnCount())
			{
				cout<<"Model::getUnnormalizedFeatureValue(): Specified compound or response ID is out of range!"<<endl;
				BALL::Exception::OutOfRange e(__FILE__,__LINE__);
				e.setMessage("Specified compound or response ID is out of range!");
				throw e;
			}

			return_value = Y_(compound,response);
			if(y_transformations_.getColumnCount()>0)
			{
				return_value *= y_transformations_(2,response); // stddev
				return_value += y_transformations_(1,response); // mean
			}
		}


		Model* createNewModelFromFile(String model_file, const QSARData& q)
		{
			Registry reg;
			Model* m;
			String model_type;
			ifstream model_input(model_file.c_str()); // read model-abbreviation
			if(!model_input)
			{
				throw BALL::Exception::FileNotFound(__FILE__,__LINE__,model_file);
				return NULL;
			}
			getline(model_input,model_type);
			getline(model_input,model_type);
			model_type = model_type.getField(0,"\t");
			model_input.close();
			RegistryEntry* entry = reg.getEntry(model_type);	
			if(!entry->kernel)
			{
				m = (*entry->create)(q);
			}
			else
			{	
				// parameters irrelevant; will be overwritten by those read from file
				m = (*entry->createKernel1)(q,1,1, -1);
			}
			
			m->readFromFile(model_file);
			return m;
		}
	}
}
