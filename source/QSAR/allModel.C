// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//

#include <BALL/QSAR/allModel.h>
#include <newmat/newmatio.h>
using namespace BALL::QSAR;



ALLModel::ALLModel(const QSARData& q, double kw) : NonLinearModel(q) 
{
	kw_ = kw;
	type_ = "ALL";
	lambda_ = 0.005;
}

ALLModel::~ALLModel()
{
}

void ALLModel::setKw(double kw)
{
	kw_= kw;
}


double ALLModel::getKw()
{
	return kw_;
}

void ALLModel::calculateEuclDistanceMatrix(Matrix& m1, Matrix& m2, Matrix& output)
{
	output.ReSize(m1.Nrows(),m2.Nrows());
	output=0;
	Statistics n;
	for(int i=1; i<=m1.Nrows();i++)
	{
		for(int j=1; j<=m2.Nrows(); j++)
		{
			//get euclidean distances of the two current rows
			output(i,j)=n.euclDistance(m1,m2,i,j);
		}	
	}
}


RowVector ALLModel::predict(const vector<double>& substance, bool transform)
{
	if(descriptor_matrix_.Ncols()==0)
	{
		throw Exception::InconsistentUsage(__FILE__,__LINE__,"Training data must be read into the ALL-model before the activity of a substance can be predicted!");
	}
 	RowVector v=getSubstanceVector(substance,transform);
 	Matrix dist;
	
	// calculate distances between the given substance and the substances of X
	// dimension of dist: 1xn 
 	calculateEuclDistanceMatrix(v, descriptor_matrix_, dist);
	RowVector w;
	calculateWeights(dist, w);
	
	Matrix XX;
	
	// calculate X.t()*X as cross-products weighted by the similarity of the given substance to the respective row of X
 	calculateXX(w, XX);
	
 	Matrix XY;
	
	// calculate X.t()*Y_ as cross-products weighted by the similarity of the given substance to the respective row of X
 	calculateXY(w, XY);
	
	// rigde regression in order to be able to cope with linearly dependent columns, i.e. singular matrices
	IdentityMatrix im(XX.Nrows());
	
	im=im*lambda_;
	XX=XX+im;
	Matrix train = XX.i()*XY;
	RowVector res;
	res = v*train;
	
	if(transform && y_transformations_.Ncols()!=0)
	{
		backTransformPrediction(res);
	}
	return res;	
}

void ALLModel::calculateWeights(Matrix& dist, RowVector& w)
{
	w.ReSize(dist.Ncols());
	for(int i=1; i<=dist.Ncols();i++)
	{
		w(i)=exp(-pow(dist(1,i),2)/(2*pow(kw_,2)));
	}
}


void ALLModel::calculateXX(RowVector& w, Matrix& res)
{
	res.ReSize(descriptor_matrix_.Ncols(),descriptor_matrix_.Ncols());
	res=0;
	// for all descriptors, calculate their weighted cross-product
	for(int i=1; i<=descriptor_matrix_.Ncols();i++)
	{
		for(int j=i; j<=descriptor_matrix_.Ncols();j++)
		{
			for(int s=1;s<=descriptor_matrix_.Nrows();s++) 
			{
				res(i,j)+=w(s)*descriptor_matrix_(s,i)*descriptor_matrix_(s,j);
			}
			res(j,i)=res(i,j);
		}
		
	}
}


void ALLModel::calculateXY(RowVector& w, Matrix& res)
{
	res.ReSize(descriptor_matrix_.Ncols(),Y_.Ncols());
	res=0;
	
	for(int i=1; i<=descriptor_matrix_.Ncols();i++)
	{
		for(int j=1; j<=Y_.Ncols();j++)
		{
			for(int s=1; s<=descriptor_matrix_.Nrows();s++)
			{
				res(i,j)+=w(s)*descriptor_matrix_(s,i)*Y_(s,j);
			}
		}
	}		
}


bool ALLModel::optimizeParameters(int k, int /*no_steps*/)
{
	double best_q2=0;
	double best_kw=0;
	kw_ = 0;
	for(int i=1;kw_<15.0;i++)
	{
		kw_+=0.25;
		validation->crossValidation(k);
		if(validation->getQ2()>best_q2)
		{
			best_q2=validation->getQ2();
			best_kw=kw_;
		}
		else if(validation->getQ2()<best_q2*0.5)
		{
			validation->setQ2(best_q2);
			break;
		}
	}
	kw_=best_kw;
	
	return 1;
}


void ALLModel::setParameters(vector<double>& v)
{
	if(v.size()!=2)
	{
		String c = "Wrong number of model parameters! Needed: 2;";
		c = c+" given: "+String(v.size());
		throw Exception::ModelParameterError(__FILE__,__LINE__,c.c_str());
	}
	kw_ = v[0];
	lambda_ = v[1];
}


vector<double> ALLModel::getParameters() const
{
	vector<double> d;
	d.push_back(kw_);
	d.push_back(lambda_);
	return d;
}


void ALLModel::saveToFile(string filename)
{
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
	
	out<<"# model-type_\tno of featues in input data\tselected featues\tno of response variables\tcentered descriptors?\tcentered response?\tno of substances"<<endl;
	out<<type_<<"\t"<<data->getNoDescriptors()<<"\t"<<sel_features<<"\t"<<Y_.Ncols()<<"\t"<<centered_data<<"\t"<<centered_y<<"\t"<<descriptor_matrix_.Nrows()<<"\n\n";
		
	out<<"# model-parameters"<<endl;  /// write model parameters 
	vector<double> v = getParameters();
	for(unsigned int i=0;i<v.size();i++)
	{
		out<<v[i]<<"\t";
	}
	out<<endl;
	
	if(centered_y) /// write information about transformation of result
	{
		out<<endl;
		for(int i=1;i<=y_transformations_.Ncols();i++)
		{
			out<<y_transformations_(1,i)<<"\t"<<y_transformations_(2,i)<<"\n";
		}
	}	
		
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
		
	out<<descriptor_matrix_<<endl; /// write descriptor matrix
	out<<Y_<<endl;  		    /// write response values
	out.close();
}


void ALLModel::readFromFile(string filename)
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
	int no_substances = line0.getField(6,"\t").toInt();
	training_result_.ReSize(no_substances,no_y);
	descriptor_matrix_.ReSize(no_substances,no_descriptors);
	Y_.ReSize(no_substances,no_y);
	descriptor_names_.clear();
	substance_names_.clear();
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
	
	if(centered_y)  /// read information about transformation of response
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
	
	for(int i=1; i<=no_substances;i++) /// read descriptor matrix
	{
		String line;
		getline(input,line);
		for(int j=1; j<=no_descriptors;j++)
		{
			descriptor_matrix_(i,j) = line.getField(j-1," ").toDouble();
		}
	}
	
	for(int i=1; i<=no_substances;i++) /// read response values
	{
		String line;
		getline(input,line);
		for(int j=1; j<=no_y;j++)
		{
			Y_(i,j) = line.getField(j-1," ").toDouble();
		}
	}
}

