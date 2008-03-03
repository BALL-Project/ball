// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//

#include <BALL/QSAR/ldaModel.h>
#include <newmatio.h>

using namespace BALL::QSAR;




LDAModel::LDAModel(const QSARData& q) : ClassificationModel(q) 
{
	lambda_=0.0001;
	type_="LDA";
}

LDAModel::~LDAModel()
{
}

void LDAModel::train()
{
	if(descriptor_matrix_.Ncols()==0)
	{
		throw Exception::InconsistentUsage(__FILE__,__LINE__,"Data must be read into the model before training!");
	}
	readLabels();

	// calculate sigma_ = covariance matrix of descriptors
	sigma_.ReSize(descriptor_matrix_.Ncols(),descriptor_matrix_.Ncols());
	for(int i=1;i<=descriptor_matrix_.Ncols();i++)
	{
		double mi=Statistics::getMean(descriptor_matrix_,i);
		for(int j=1;j<=descriptor_matrix_.Ncols();j++)
		{
			sigma_(i,j)=Statistics::getCovariance(descriptor_matrix_,i,j,mi);
		}
	}
	IdentityMatrix I(sigma_.Ncols());
	I=I*lambda_;
	sigma_=sigma_+I;
	sigma_=sigma_.i();

	mean_vectors_.resize(Y_.Ncols());
	no_substances_.resize(labels_.size(),0);
	for(int c=0; c<Y_.Ncols();c++)
	{
		vector<int> no_substances_c(labels_.size(),0);  // no of substances in each class for activitiy c
		mean_vectors_[c].ReSize(labels_.size(),descriptor_matrix_.Ncols());
		mean_vectors_[c]=0;
		
		for(int i=1;i<=descriptor_matrix_.Nrows();i++) // calculate sum vector of each class
		{
			int yi = static_cast<int>(Y_(i,1)); // Y_ will contains only ints for classifications
			int pos = yi - labels_[0]; 
			if(labels_[pos]==yi)
			{
				mean_vectors_[c].Row(pos+1) += descriptor_matrix_.Row(i);
				no_substances_c[pos]++;
			}
		}
	
		for(int i=1; i<=mean_vectors_[c].Nrows();i++) // calculate mean vector of each class
		{
			if(no_substances_c[i-1]==0)
			{
				mean_vectors_[c].Row(i)=exp(100);  // set mean of classes NOT occurring for activitiy c to inf
			}
			mean_vectors_[c].Row(i) = mean_vectors_[c].Row(i)/no_substances_c[i-1];
		}
		
		for(unsigned int i=0; i<no_substances_.size();i++) // update overall number of substances per class
		{
			no_substances_[i] += no_substances_c[i];
		}
	}
}


RowVector LDAModel::predict(const vector<double>& substance, bool transform)
{
	if(sigma_.Ncols()==0)
	{
		throw Exception::InconsistentUsage(__FILE__,__LINE__,"Model must be trained before it can predict the activitiy of substances!");
	}
	// search class to which the given substance has the smallest distance
	RowVector s = getSubstanceVector(substance,transform);
	int min_k=0;
	double min_dist = 99999999;
	RowVector result(mean_vectors_.size());
	
	for(unsigned int c=0; c<mean_vectors_.size();c++)
	{
		for (int k=1; k<=mean_vectors_[c].Nrows();k++)
		{
			RowVector diff=s-mean_vectors_[c].Row(k);
			double dist = (diff*sigma_*diff.t()).AsScalar();
			if(dist<min_dist)
			{
				min_dist=dist;
				min_k=k;
			}
		}
		result(c+1)=labels_[min_k-1];
	}
	
	return result;
	
}

void LDAModel::setParameters(vector<double>& v)
{
	if(v.size()!=1)
	{
		String c = "Wrong number of model parameters! Needed: 1;";
		c = c+" given: "+String(v.size());
		throw Exception::ModelParameterError(__FILE__,__LINE__,c.c_str());
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
	if(sigma_.Nrows()==0)
	{
		throw Exception::InconsistentUsage(__FILE__,__LINE__,"Model must have been trained before the results can be saved to a file!");
	}
	ofstream out(filename.c_str());
	
	bool centered_data = 0;
	bool centered_y = 0;
	if(descriptor_transformations_.Ncols()!=0)
	{
		centered_data=1;
		if(y_transformations_.Ncols()!=0)
		{
			centered_y=1;
		}
	}
	
	int sel_features=descriptor_IDs_.size();
	if(sel_features==0)
	{
		sel_features = data->getNoDescriptors();
	}
	
	out<<"# model-type_\tno of featues in input data\tselected featues\tno of response variables\tcentered descriptors?\tno of classes"<<endl;
	out<<type_<<"\t"<<data->getNoDescriptors()<<"\t"<<sel_features<<"\t"<<Y_.Ncols()<<"\t"<<centered_data<<"\t"<<no_substances_.size()<<"\n\n";

	out<<"# model-parameters"<<endl;  /// write model parameters 
	vector<double> v = getParameters();
	for(unsigned int i=0;i<v.size();i++)
	{
		out<<v[i]<<"\t";
	}
	out<<endl;
	
	out<<"\n# ID\tdescriptor-name\t";
	if(centered_data)
	{
		out<<"mean of desc.\tstddev of desc.\t";
	}
	if(stderr)
	{
		out<<"stderr(s) of coeff.";
	}
	out<<endl;
	
	/// write (selected) descriptors and information about their transformation
	if(!descriptor_IDs_.empty())
	{
		descriptor_IDs_.front();
		for(int i=0; i<descriptor_matrix_.Ncols();i++)
		{
			out<<String(descriptor_IDs_.next())<<"\t"<<descriptor_names_[i]<<"\t";
			
			if(centered_data)
			{
				out<<descriptor_transformations_(1,i+1)<<"\t"<<descriptor_transformations_(2,i+1)<<"\t";
			}
			out <<"\n";
		}
	}
	else
	{
		for(int i=0; i<descriptor_matrix_.Ncols();i++)
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
	out<<endl<<endl;
	
	out<<sigma_<<endl;	/// write Matrix sigma_
	
	for(unsigned int i=0; i<mean_vectors_.size();i++)   /// write all mean vectors
	{
		out<<mean_vectors_[i]<<endl;
	}
}



void LDAModel::readFromFile(string filename)
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
	int no_classes = line0.getField(5,"\t").toInt();
	//int no_subst = line0.getField(6,"\t").toInt();

	sigma_.ReSize(no_descriptors,no_descriptors);
	Matrix means(no_classes,no_descriptors);
	mean_vectors_.resize(no_y,means);
	no_substances_.clear();
	descriptor_names_.clear();
	substance_names_.clear();
	labels_.clear();
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
	getline(input,line0);  // skip comment line 
	
	for(int i=1; i<=no_descriptors; i++) /// read descriptors and infor. about their transformation
	{
		String line;
		getline(input,line);
		unsigned int id = (unsigned int) line.getField(0,"\t").toInt();
		descriptor_IDs_.push_back(id);
		descriptor_names_.push_back(line.getField(1,"\t"));
		if(centered_data)
		{
			descriptor_transformations_(1,i)= line.getField(2,"\t").toDouble();
			descriptor_transformations_(2,i)= line.getField(3,"\t").toDouble();
		}
	}	
	getline(input,line0);  // skip empty line 
	getline(input,line0);  // skip comment line 
	
	getline(input,line0);    /// read class-labels_
	for(int i=0;i<no_classes;i++)
	{
		labels_.push_back(line0.getField(i,"\t").toInt());
	}	
	getline(input,line0);  // skip empty line 
	getline(input,line0);  // skip comment line 
	
	getline(input,line0);	/// read numbers of substances of all classes
	for(int i=0; i<no_descriptors; i++)
	{
		int n = line0.getField(i,"\t").toInt();
		no_substances_.push_back(n);
	}
	getline(input,line0);  // skip empty line 
	
	for(int i=1; i<=no_descriptors;i++) /// read matrix sigma_
	{
		String line;
		getline(input,line);
		for(int j=1; j<=no_descriptors;j++)
		{
			sigma_(i,j) = line.getField(j-1," ").toDouble();
		}
	}
	getline(input,line0);  // skip empty line 

	for(int c=0; c<no_y;c++) /// read all mean vectors
	{
		for(int i=1; i<=no_classes;i++) 
		{
			String line;
			getline(input,line);
			for(int j=1; j<=no_descriptors;j++)
			{
				mean_vectors_[c](i,j) = line.getField(j-1," ").toDouble();
			}
		}
	}
}
