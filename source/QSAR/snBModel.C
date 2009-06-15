// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//

#include <BALL/QSAR/snBModel.h>

using namespace BALL::QSAR;


const double SNBModel::sqrt2Pi_ = sqrt(2*BALL::Constants::PI);



SNBModel::SNBModel(const QSARData& q) : BayesModel(q) 
{
	type_="snB";
	mean_.resize(0);
	stddev_.resize(0);
}

SNBModel::~SNBModel()
{
}

void SNBModel::train()
{
	if(descriptor_matrix_.Ncols()==0)
	{
		throw Exception::InconsistentUsage(__FILE__,__LINE__,"Data must be read into the model before training!");
	}
	readLabels();
		
	// map values of Y to their index
	map<int,uint> label_to_pos; 
	for(uint i=0;i<labels_.size();i++)
	{
		label_to_pos.insert(make_pair(labels_[i],i));
	}	

	
	mean_.resize(Y_.Ncols());
	stddev_.resize(Y_.Ncols());
	no_substances_.clear();
	no_substances_.resize(labels_.size(),0);
	for(int act=0; act<Y_.Ncols();act++)
	{
		// calculate mean and stddev of each feature for _each_ class
		mean_[act].ReSize(labels_.size(),descriptor_matrix_.Ncols());
		stddev_[act].ReSize(labels_.size(),descriptor_matrix_.Ncols());
		mean_[act]=0; stddev_[act]=0;
	
		for(int i=1;i<=descriptor_matrix_.Ncols();i++)
		{
			vector<double> v0(0,0);
			v0.reserve(descriptor_matrix_.Nrows());
			vector<vector<double> > class_values(labels_.size(),v0); 
			
			// sort values of current feature into the respective vector (one for each class)
			for(int j=1; j<=descriptor_matrix_.Nrows();j++)
			{
				uint index = label_to_pos.find((int)Y_(j,act+1))->second;
				class_values[index].push_back(descriptor_matrix_(j,i));
				if(act==0 && i==1) no_substances_[index]++;
			}
			
			// calculate mean and stddev for current feature for all classes
			for(uint j=1; j<=labels_.size();j++)
			{
				mean_[act](j,i) = Statistics::getMean(class_values[j-1]);
				stddev_[act](j,i)=Statistics::getStddev(class_values[j-1],mean_[act](j,i));
			}
		}
	}
}


BALL::Vector<double> SNBModel::predict(const vector<double>& substance, bool transform)
{
	if(mean_.size()==0)
	{
		throw Exception::InconsistentUsage(__FILE__,__LINE__,"Model must be trained before it can predict the activitiy of substances!");
	}
	
	Vector<double> s = getSubstanceVector(substance,transform);
	
	uint no_activities = mean_.size();
	uint no_classes = mean_[0].Nrows();
	uint no_features = mean_[0].Ncols();
	
	Vector<double> result(no_activities);
	result=0;
	
	for(uint act=0; act<no_activities;act++)
	{
		vector<double> d(no_classes,0);
		vector<vector<double> > probabilities (no_features,d);
		vector<double> pdf_sums(no_features,0);
		
		// calculate probability-density-function value for each given feature value
		for(uint i=0;i<no_features;i++)
		{
			double x = s(i+1);
			for(uint j=0;j<no_classes;j++)
			{
				double stddev = stddev_[act](j+1,i+1);
				if(stddev==0) stddev = 0.000001; // zero is not allowed by the below equation
				probabilities[i][j] = (1/(stddev*sqrt2Pi_)) * exp(-pow((x-mean_[act](j+1,i+1)),2)/(2*stddev*stddev));
				pdf_sums[i] += probabilities[i][j];
			}
		}
		
		// convert probability-density values to probabilities;
		// then calculate probability for each class by muliplying the probabilities for each feature value;
		// finally find most probable class
		vector<double> substance_prob(no_classes,1); // the prob for the given subst. to be in each of the classes
		double max=0;
		double second_best=0;
		int best_label=labels_[0];
		for(uint i=0; i<no_features;i++)
		{
			for(uint j=0; j<no_classes;j++)
			{
				probabilities[i][j] /= pdf_sums[i];
				substance_prob[j] *= probabilities[i][j];
				if(i==no_features-1 && substance_prob[j]>max)
				{
					second_best = max;
					max = substance_prob[j];
					best_label = labels_[j];
				}				
			}
		}
		
		if(max>=second_best+min_prob_diff_)
		{
			result(act+1) = best_label;
		}
		else
		{
			result(act+1) = undef_act_class_id_;
		}
	}
	
	return result;	
}


vector<double> SNBModel::calculateProbabilities(int activitiy_index, int feature_index, double feature_value)
{
	if(mean_.size()==0)
	{
		throw Exception::InconsistentUsage(__FILE__,__LINE__,"Model must be trained before a probability for a given feature value can be calculated!");
	}
	int no_features = mean_[0].Ncols();
	int no_classes = mean_[0].Nrows();
	if(activitiy_index>=(int)stddev_.size() || feature_index>=no_features || activitiy_index<0 || feature_index<0)
	{
		throw Exception::InconsistentUsage(__FILE__,__LINE__,"Index of bound for parameters given to SNBModel::calculateProbability() !");
	}
	
	double pdf_sum=0; // sum of all pdf-values for the given feature-value
	vector<double> pdf_values(no_classes);
	
	for(int i=0; i<no_classes;i++) // calculate pdf-value for given feature-value to be dervied from each class
	{
		double stddev = stddev_[activitiy_index](i+1,feature_index+1);
		if(stddev==0) stddev = 0.000001; // zero is not allowed by the below equation
		pdf_values[i] = (1/(stddev*sqrt2Pi_)) * exp(-pow((feature_value-mean_[activitiy_index](i+1,feature_index+1)),2)/(2*stddev*stddev));
		pdf_sum += pdf_values[i];
	}
	for(int i=0; i<no_classes;i++) // convert pdf-values to probabilities
	{
		pdf_values[i]/=pdf_sum;
	}
	return pdf_values;	
}


bool SNBModel::isTrained()
{
	uint sel_features=descriptor_IDs_.size();
	if(sel_features==0)
	{
		sel_features = data->getNoDescriptors();
	}
	if(mean_.size()>0 && (uint)mean_[0].Ncols()==sel_features) return true;
	return false;
}


int SNBModel::getNoResponseVariables()
{
	if(!isTrained()) return 0;
	else return mean_.size();	
}


vector<double> SNBModel::getParameters() const
{
	vector<double> d;
	d.push_back(min_prob_diff_);
	d.push_back(undef_act_class_id_);
	return d;
}


void SNBModel::setParameters(vector<double>& v)
{
	if(v.size()!=0 && v.size()!=2)
	{
		String c = "Wrong number of model parameters! Needed: 2;";
		c = c+" given: "+String(v.size());
		throw Exception::ModelParameterError(__FILE__,__LINE__,c.c_str());
	}
	
	if(v.size()==2)
	{
		min_prob_diff_ = v[0];
		undef_act_class_id_ = v[1];
	}
}


void SNBModel::saveToFile(string filename)
{
	bool trained = 1;
	if(mean_.size()==0) trained=0;
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
	
	int no_y = mean_.size();
	if(no_y==0) no_y = y_transformations_.Ncols(); // correct no because transformation information will have to by read anyway when reading this model later ...
	
	out<<"# model-type_\tno of featues in input data\tselected featues\tno of response variables\tcentered descriptors?\tno of classes\ttrained?"<<endl;
	out<<type_<<"\t"<<data->getNoDescriptors()<<"\t"<<sel_features<<"\t"<<no_y<<"\t"<<centered_data<<"\t"<<no_substances_.size()<<"\t"<<trained<<"\n\n";

	saveModelParametersToFile(out);
	saveDescriptorInformationToFile(out);
	
	if(!trained) return;
	
	saveClassInformationToFile(out);
	
	// write mean_ matrices
	for(uint i=0; i<mean_.size();i++)
	{
		out<<mean_[i]<<endl;
	}
	
	// write stddev_ matrices
	for(uint i=0; i<stddev_.size();i++)
	{
		out<<stddev_[i]<<endl;
	}
}



void SNBModel::readFromFile(string filename)
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
	//int no_subst = line0.getField(6,"\t").toInt();

	substance_names_.clear();
	
	getline(input,line0);  // skip empty line	
	readModelParametersFromFile(input);
	readDescriptorInformationFromFile(input, no_descriptors, centered_data);
	
	if(!trained) 
	{
		mean_.resize(0);
		return;
	}
	
	readClassInformationFromFile(input, no_classes);
	
	mean_.resize(no_y);
	for(int c=0; c<no_y;c++) // read all mean-vector matrices
	{
		readMatrix(mean_[c],input,no_classes,no_descriptors);
		getline(input,line0);  // skip empty line
	}
	
	stddev_.resize(no_y);
	for(int act=0;act<no_y;act++)  // read all stddev matrices (each containing a stddev for each feature for each class)
	{
		readMatrix(stddev_[act],input,no_classes,no_descriptors);
		getline(input,line0);  // skip empty line 
	}
}
