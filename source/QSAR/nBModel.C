// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//

#include <BALL/QSAR/nBModel.h>
#include <newmatio.h>

using namespace BALL::QSAR;




NBModel::NBModel(const QSARData& q) : ClassificationModel(q) 
{
	type_="nB";
	probabilities_.resize(0);
	discretization_steps_ = 5;
}

NBModel::~NBModel()
{
}

void NBModel::train()
{
	if(descriptor_matrix_.Ncols()==0)
	{
		throw Exception::InconsistentUsage(__FILE__,__LINE__,"Data must be read into the model before training!");
	}
	readLabels();
	
	uint no_features = descriptor_matrix_.Ncols();
	uint no_classes = labels_.size();
	uint no_compounds = descriptor_matrix_.Nrows();
	uint no_activities = Y_.Ncols();
		
	// map values of Y to their index
	map<int,uint> label_to_pos; 
	for(uint i=0;i<no_classes;i++)
	{
		label_to_pos.insert(make_pair(labels_[i],i));
	}	

	
	min_max_.ReSize(2,no_features);
	min_max_.Row(1) = 1e10;
	min_max_.Row(2) = -1e10;
	
	// find minimum and maximum of each feature
	for(uint i=1;i<=no_features;i++)
	{
		for(uint j=1;j<=no_compounds;j++)
		{
			if(descriptor_matrix_(j,i)<min_max_(1,i))
			{
				min_max_(1,i) = descriptor_matrix_(j,i);
			}
			if(descriptor_matrix_(j,i)>min_max_(2,i))
			{
				min_max_(2,i) = descriptor_matrix_(j,i);
			}
		}
	}

	
	probabilities_.clear();
	probabilities_.resize(no_activities);
	no_substances_.clear();
	no_substances_.resize(no_classes,0);

	for(uint act=0; act<no_activities;act++)
	{
		Matrix sums(discretization_steps_,no_features); sums = 0;
		probabilities_[act].resize(no_classes,sums);
		
		for(uint i=1;i<=no_features;i++)
		{
			double step = (min_max_(2,i)-min_max_(1,i))/discretization_steps_;
			
			for(uint j=1;j<=no_compounds;j++)
			{
				uint class_id = label_to_pos.find(Y_(j,act+1))->second;
				if(act==0 && i==1) no_substances_[class_id]++;
				
				uint feat_bucket = (uint)((descriptor_matrix_(j,i)-min_max_(1,i))/step)+1;		
				if(feat_bucket>discretization_steps_) feat_bucket=discretization_steps_; // for max.
				
				probabilities_[act][class_id](feat_bucket,i)++;
				sums(feat_bucket,i)++;
			}	
		}
;		
		for(uint i=1;i<=no_features;i++)
		{
			for(uint j=1; j<=discretization_steps_;j++)
			{
				for(uint k=0; k<no_classes;k++)
				{
					// divide number of occurences of feature i within range j and in class k by its number of occurences in all classes
					// ==> probability for i within range j to be in class k
					if(probabilities_[act][k](j,i)!=0)
					{
						probabilities_[act][k](j,i) /= sums(j,i);
					}
				}
			}
		}
			
	}
	
}


RowVector NBModel::predict(const vector<double>& substance, bool transform)
{
	if(probabilities_.size()==0)
	{
		throw Exception::InconsistentUsage(__FILE__,__LINE__,"Model must be trained before it can predict the activitiy of substances!");
	}
	
	RowVector s = getSubstanceVector(substance,transform);
	
	uint no_activities = probabilities_.size();
	uint no_classes = probabilities_[0].size();
	uint no_features = probabilities_[0][0].Ncols();
	uint no_discretizations = probabilities_[0][0].Nrows();
	
	RowVector result(no_activities);
	result=0;
	
	for(uint act=0; act<no_activities;act++)
	{
		vector<double> substance_prob(no_classes,1); // prob. for the entire substance
		double max=0;
		int best_label=0;
		
		for(uint i=1; i<=no_features;i++)
		{
			double step = (min_max_(2,i)-min_max_(1,i))/no_discretizations;
			int disc_index = (int)((s(i)-min_max_(1,i))/step)+1;
			if(disc_index<1) disc_index=1;
			else if(disc_index>(int)no_discretizations) disc_index=no_discretizations;			
			
			for(uint j=0;j<no_classes;j++)
			{
				substance_prob[j] *= probabilities_[act][j](disc_index,i);
				
				if(i==no_features-1 && substance_prob[j]>max)
				{
					max = substance_prob[j];
					best_label = labels_[j];
				}
			}
		}
		result(act+1) = best_label;	
	}	
	
	return result;	
}


vector<double> NBModel::getParameters()
{
	vector<double> d(1,discretization_steps_);
	return d;
}


void NBModel::setParameters(vector<double>& v)
{
	if(v.size()!=1)
	{
		String c = "Wrong number of model parameters! Needed: 1;";
		c = c+" given: "+String(v.size());
		throw Exception::ModelParameterError(__FILE__,__LINE__,c.c_str());
	}
	discretization_steps_ = (int) v[0];	
}
		

void NBModel::saveToFile(string filename)
{
	if(probabilities_.size()==0)
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
	out<<endl<<endl;
	
	out<<"# no of substances of each class\n";
	for(unsigned int i=0;i<no_substances_.size();i++)  /// write numbers of substances of each class
	{
		out<<no_substances_[i]<<"\t";
	}
	out<<endl<<endl;
	
	/// write min_max_ matrix
	out<<min_max_<<endl;
	
	/// write probability matrices
	for(uint i=0; i<probabilities_.size();i++)
	{
		for(uint j=0; j<probabilities_[0].size();j++)
		{
			out<<probabilities_[i][j]<<endl;
		}
	}
}



void NBModel::readFromFile(string filename)
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

	min_max_.ReSize(2,no_descriptors);
	for(int i=1;i<=2;i++)
	{
		String line;
		getline(input,line);
		for(int j=1;j<=no_descriptors;j++)
		{
			min_max_(i,j) = line.getField(j-1," ").toDouble();
		}
	}
	
	getline(input,line0);  // skip empty line 

	Matrix m(discretization_steps_,no_descriptors);
	probabilities_.resize(no_y);
	for(int act=0;act<no_y;act++)   /// read all probability matrices 
	{
		probabilities_[act].resize(no_classes,m);
		for(int i=0; i<no_classes;i++)
		{
			for(uint j=1; j<=discretization_steps_;j++)
			{
				String line;
				getline(input,line);
				for(int k=1; k<=no_descriptors;k++)
				{
					probabilities_[act][i](j,k) = line.getField(k-1," ").toDouble();
				}
			}
			getline(input,line0);  // skip empty line 
		}
		getline(input,line0);  // skip empty line 	
	}
}
