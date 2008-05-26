// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// 

#include <BALL/QSAR/regressionModel.h>
using namespace BALL::QSAR;


RegressionModel::RegressionModel(const QSARData& q) : Model(q) 
{
	validation=new RegressionValidation(this);
	model_val=validation;
}

RegressionModel::~RegressionModel() 
{
	delete validation;
}


const Matrix* RegressionModel::getTrainingResult() const
{
	return &training_result_;
}



void RegressionModel::show()
{
// 	if(training_result_.Nrows()==0)
// 	{
// 		throw Exception::InconsistentUsage(__FILE__,__LINE__,"Model must have been trained before the results can be saved displayed!");
// 	}
	
	const Matrix* coeffErrors = validation->getCoefficientErrors();
	bool stderr=0;
	if(coeffErrors->Ncols()!=0)
	{
		stderr=1;
	}
	cout<<type_<<"\t"<<data->getNoDescriptors()<<"\n";
	
	
	if(type_=="ALL")
	{
		descriptor_IDs_.front();
		for(int i=0;descriptor_IDs_.hasNext();i++)
		{
			cout<<descriptor_IDs_.next()<<"  "<<descriptor_names_[i]<<endl;
		}
	}
	else if(!descriptor_IDs_.empty())
	{
		descriptor_IDs_.front();
		for(int i=0; i<training_result_.Nrows();i++)
		{
			if(type_!="KPLS" && type_!="KPCR" && type_!="KPLS" && type_!="GP")
			{
				cout<<String(descriptor_IDs_.next())<<"\t"<<descriptor_names_[i]<<"\t";
			}
			else
			{
				cout<<String(i)<<"    "<<substance_names_[i]<<"    ";
			}
			for(int j=1; j<=training_result_.Ncols();j++) 
			{
				cout<<training_result_(i+1,j)<<"\t";
			}
			for(int j=1; j<=coeffErrors->Ncols();j++)
			{
				cout<<(*coeffErrors)(i+1,j)<<"\t";
			}
			cout <<"\n";
		}
	}
	else
	{
		for(int i=0; i<training_result_.Nrows();i++)
		{
			if(type_!="KPLS" && type_!="KPCR" && type_!="KPLS" && type_!="GP")
			{
				cout<<String(i)<<"\t"<<descriptor_names_[i]<<"\t";
			}
			else
			{
				cout<<String(i)<<"\t"<<substance_names_[i]<<"\t";cout.flush();
			}
			for(int j=1; j<=training_result_.Ncols();j++) 
			{
				cout<<training_result_(i+1,j)<<"\t";
			}
			for(int j=1; j<=coeffErrors->Ncols();j++)
			{
				cout<<(*coeffErrors)(i+1,j)<<"\t";
			}
			cout <<"\n";
		}
	}

}

void RegressionModel::saveToFile(string filename)
{
	if(training_result_.Nrows()==0)
	{
		throw Exception::InconsistentUsage(__FILE__,__LINE__,"Model must have been trained before the results can be saved to a file!");
	}
	ofstream out(filename.c_str());
	
	const Matrix* coeffErrors = validation->getCoefficientErrors();
	bool stderr=0;
	if(coeffErrors->Ncols()!=0)
	{
		stderr=1;
	}
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
	
	out<<"# model-type_\tno of featues in input data\tselected featues\tno of response variables\tcentered descriptors?\tcentered response?"<<endl;
	out<<type_<<"\t"<<data->getNoDescriptors()<<"\t"<<sel_features<<"\t"<<training_result_.Ncols()<<"\t"<<centered_data<<"\t"<<centered_y<<"\n";
	
	out<<endl<<"# model-parameters"<<endl;  /// write model parameters 
	vector<double> v = getParameters();
	for(unsigned int i=0;i<v.size();i++)
	{
		out<<v[i]<<"\t";
	}
	out<<endl;
	
	if(centered_y)
	{
		out<<endl;
		for(int i=1;i<=y_transformations_.Ncols();i++)
		{
			out<<y_transformations_(1,i)<<"\t"<<y_transformations_(2,i)<<"\n";
		}
	}	
	
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
			for(int j=1; j<=coeffErrors->Ncols();j++)
			{
				out<<(*coeffErrors)(i+1,j)<<"\t";
			}
			
			
			out <<"\n";
		}
		out.close();
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
			for(int j=1; j<=coeffErrors->Ncols();j++)
			{
				out<<(*coeffErrors)(i+1,j)<<"\t";
			}
			out <<"\n";
		}
		out.close();
	}	
}


void RegressionModel::readFromFile(string filename)
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
	getline(input,line0);  // skip empty line
	
	readModelParametersFromFile(input);
	if(centered_y)
	{
		readResponseTransformationFromFile(input, no_y);
	}
	readDescriptorInformationFromFile(input, no_descriptors, centered_data, no_y);
}


void RegressionModel::readDescriptorInformationFromFile(ifstream& input, int no_descriptors, bool transformation, int no_coefficients)
{
	descriptor_names_.clear();
	descriptor_transformations_.ReSize(2,no_descriptors);
	training_result_.ReSize(no_descriptors,no_coefficients);
	String line;
	getline(input,line);  // skip comment line
	for(int i=1; i<=no_descriptors; i++)
	{
		getline(input,line);
		unsigned int id = (unsigned int) line.getField(0,"\t").toInt();
		descriptor_IDs_.push_back(id);
		descriptor_names_.push_back(line.getField(1,"\t"));
		int j=2;
		for(; j<2+no_coefficients; j++)
		{	
			training_result_(i,j-1) = line.getField(j,"\t").toDouble();
		}
		if(transformation)
		{
			descriptor_transformations_(1,i)= line.getField(j,"\t").toDouble();
			descriptor_transformations_(2,i)=line.getField(j+1,"\t").toDouble();
		}
	}
	getline(input,line);  // skip empty line	
}
