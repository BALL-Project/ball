// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//

#include <BALL/QSAR/Model.h>
using namespace BALL::QSAR;


Model::Model(const QSARData& q)
{
	data=&q;
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
		String message = "Wrong useage of '='-operator. A copy of a "+m.type_+" model is no "+type_+" model!";
		throw Exception::InconsistentUsage(__FILE__,__LINE__,message.c_str());
	}
	descriptor_names_=m.descriptor_names_;
	descriptor_transformations_ = m.descriptor_transformations_;
	y_transformations_ = m.y_transformations_;
	descriptor_IDs_=m.descriptor_IDs_;
	vector<double> v=m.getParameters();
	setParameters(v);
	data = m.data;
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
	SortedList<unsigned int> s;
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
	SortedList<unsigned int>::Iterator it=descriptor_IDs_.begin();

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
	SortedList<unsigned int>::Iterator it=descriptor_IDs_.begin();
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


RowVector Model::getSubstanceVector(const vector<double>& substance, bool transform)
{
	if(transform==1 && descriptor_transformations_.Ncols()==0)
	{
		throw Exception::InconsistentUsage(__FILE__,__LINE__,"Transformation of test data requested although no scaling of training data was done!!");
	}				

	bool fs=0; // has feature selection being done?
	if(!descriptor_IDs_.empty())
	{
		fs=1;
	}
	list<unsigned int>::iterator it=descriptor_IDs_.begin();

	int t=0; // index in line of test data
	int length=descriptor_matrix_.Ncols();
	if(fs)
	{	
		length=descriptor_IDs_.size();
	}
	
	RowVector v(length);
	
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


RowVector Model::getSubstanceVector(const RowVector& substance, bool transform)
{
	bool fs=0; // has feature selection being done?
	if(!descriptor_IDs_.empty())
	{
		fs=1;
	}
	list<unsigned int>::iterator it=descriptor_IDs_.begin();

	int t=0; // index in line of test data
	int length=descriptor_matrix_.Ncols();
	if(fs)
	{	
		length=descriptor_IDs_.size();
	}
	
	RowVector v(length);
	
	// if no feature selection was done, i.e. if descriptor_IDs_ is empty
	if (!fs)
	{
		if(transform)
		{
			for(int i=1; i<length;i++)
			{
				double stddev=descriptor_transformations_(2,i+1);
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
			for(int i=1; i<length;i++)
			{
				t=*it;
				double stddev=descriptor_transformations_(2,i+1);
				if(stddev==0) {stddev=0.001;}
				v(i)=(substance(t+1)-descriptor_transformations_(1,i))/stddev;
				it++;
			}
		}
		else
		{
			for(int i=1; i<length;i++)
			{
				t=*it;
				v(i+1)=substance(t+1);
				it++;
			}
		}
	}
	
	return v;	
}


void Model::backTransformPrediction(RowVector& pred)
{
	for(int i=1; i<=y_transformations_.Ncols();i++)
	{
		double stddev=y_transformations_(2,i);
		pred(i)=pred(i)*stddev+y_transformations_(1,i);
	}	
}


const Matrix* Model::getDescriptorMatrix()
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

	
const Matrix* Model::getY()
{
	return &Y_;
}


void Model::setDescriptorIDs(const SortedList<unsigned int>& sl)
{
	descriptor_IDs_=sl;
}


SortedList<unsigned int>* Model::getDescriptorIDs()
{
	return &descriptor_IDs_;
}

vector<double> Model::getParameters() const
{
	vector<double> d;
	return d;
}

void Model::setDataSource(const QSARData* q)
{
	data = q;
}

void Model::addLambda(Matrix& matrix, double& lambda)
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
