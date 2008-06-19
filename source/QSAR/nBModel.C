// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//

#include <BALL/QSAR/nBModel.h>
#include <newmatio.h>

using namespace BALL::QSAR;




NBModel::NBModel(const QSARData& q) : BayesModel(q) 
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


vector<double> NBModel::getParameters() const
{
	vector<double> d;
	d.push_back(discretization_steps_);
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


bool NBModel::isTrained()
{
	uint sel_features=descriptor_IDs_.size();
	if(sel_features==0)
	{
		sel_features = data->getNoDescriptors();
	}
	
	if(probabilities_.size()>0 && (uint)min_max_.Ncols()==sel_features) return true;
	return false;
}


vector<double> NBModel::calculateProbabilities(int activitiy_index, int feature_index, double feature_value)
{
	if(probabilities_.size()==0 || probabilities_[0].size()==0)
	{
		throw Exception::InconsistentUsage(__FILE__,__LINE__,"Model must be trained before a probability for a given feature value can be calculated!");
	}
	int no_features = probabilities_[0][0].Ncols();
	int no_classes = probabilities_[0].size();
	if(activitiy_index>=(int)probabilities_.size() || feature_index>=no_features || activitiy_index<0 || feature_index<0)
	{
		throw Exception::InconsistentUsage(__FILE__,__LINE__,"Index of bound for parameters given to SNBModel::calculateProbability() !");
	}
	
	uint no_discretizations = probabilities_[0][0].Nrows();
	double step = (min_max_(2,feature_index+1)-min_max_(1,feature_index+1))/no_discretizations;
	int disc_index = (int)((feature_value-min_max_(1,feature_index+1))/step)+1;
	
	if(disc_index<1) disc_index=1;
	else if(disc_index>(int)no_discretizations) disc_index=no_discretizations;
	
	vector<double> prob(no_classes);
	for(uint i=0; i<no_classes;i++)
	{
		prob[i] = probabilities_[activitiy_index][i](disc_index,feature_index+1);
	}
	return prob;
}
		

void NBModel::saveToFile(string filename)
{
	bool trained = 1;
	if(probabilities_.size()==0) trained = 0;
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
	
	out<<"# model-type_\tno of featues in input data\tselected featues\tno of response variables\tcentered descriptors?\tno of classes\ttrained?"<<endl;
	out<<type_<<"\t"<<data->getNoDescriptors()<<"\t"<<sel_features<<"\t"<<Y_.Ncols()<<"\t"<<centered_data<<"\t"<<no_substances_.size()<<"\t"<<trained<<"\n\n";

	saveModelParametersToFile(out);
	saveDescriptorInformationToFile(out);
	
	if(!trained) return;
	
	saveClassInformationToFile(out);
	
	out<<min_max_<<endl;
	
	// write probability matrices
	for(uint i=0; i<probabilities_.size();i++)
	{
		for(uint j=0; j<probabilities_[0].size();j++)
		{
			out<<probabilities_[i][j]<<endl;
		}
	}
	
	out.close();
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
	bool trained = line0.getField(6,"\t").toBool();

	substance_names_.clear();
	
	getline(input,line0);  // skip empty line	
	readModelParametersFromFile(input);
	readDescriptorInformationFromFile(input, no_descriptors, centered_data);
	
	if(!trained) return;
	
	readClassInformationFromFile(input, no_classes);
	readMatrix(min_max_,input,2,no_descriptors);
	getline(input,line0);  // skip empty line	
	
	probabilities_.resize(no_y);
	for(int act=0;act<no_y;act++)   // read all probability matrices 
	{
		probabilities_[act].resize(no_classes); // <no_y>*<no_classes> matrices
		for(int i=0; i<no_classes;i++)
		{
			readMatrix(probabilities_[act][i],input,discretization_steps_,no_descriptors);
			getline(input,line0);  // skip empty line
		}
	}
	input.close();
}
