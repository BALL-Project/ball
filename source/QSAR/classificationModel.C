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
