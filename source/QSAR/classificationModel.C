// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// 

#include <BALL/QSAR/classificationModel.h>
using namespace BALL::QSAR;


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
	SortedList<int> l;
	for(int i=1; i<=Y_.Nrows();i++)
	{
		for(int j=1;j<=Y_.Ncols();j++)
		{
			int label=static_cast<int>(Y_(i,j));
			if(label!=Y_(i,j))
			{
				throw Exception::WrongDataType(__FILE__,__LINE__,"Some class labels are not discrete values!! Creation of a classification model is therefore not possible!");
			}
			if(!l.contains(label))  // for classification experiments, Y will contain only ints
			{
				l.insert(label);
			}
		}
	}
	
	labels_.clear();
	l.front();
	while(l.hasNext())
	{
		labels_.push_back(l.next());
	}
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
