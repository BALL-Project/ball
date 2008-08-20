// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// 

#include <BALL/QSAR/pcrModel.h>
using namespace BALL::QSAR;



PCRModel::PCRModel(const QSARData& q, double frac_var) : LinearModel(q) 
{
	type_="PCR";
	frac_var_=frac_var;
}

PCRModel::~PCRModel()
{
}

void PCRModel::setFracVar(double frac_var)
{
	frac_var_=frac_var;
}

void PCRModel::calculateEigenvectors(const SymmetricMatrix& data, double frac_var, Matrix& output)
{
	// newmat sorts eigenvalues and -vectors ascendingly !!
	Matrix eigenVectors;
	DiagonalMatrix eigenValues; 
	EigenValues(data,eigenValues,eigenVectors);
	int first_eigenvector=eigenValues.Ncols();
	int last_eigenvector=eigenValues.Ncols();

	// find the smallest eigenvector that should be taken into account
	double complete_var=eigenValues.Sum();
	double explained_var=0;
	int cols=0; 
	if(complete_var==0)
	{
		throw Exception::NoPCAVariance(__FILE__,__LINE__,"No variance present to be explained by PCA!");
	}
	for (; last_eigenvector>=1 && cols<data.Nrows() && explained_var/complete_var<frac_var ; last_eigenvector--) 
	{
		explained_var+=eigenValues(last_eigenvector);
		cols++;
	}
	last_eigenvector++;
	
	output.ReSize(data.Ncols(),cols);
	int c=1;
	for (int i=first_eigenvector; i>=last_eigenvector ; i--)
	{
		output.Column(c)=eigenVectors.Column(i);
		c++;
	}
}


void PCRModel::train()
{
	if(descriptor_matrix_.Ncols()==0)
	{
		throw Exception::InconsistentUsage(__FILE__,__LINE__,"Data must be read into the model before training!");
	}
	SymmetricMatrix X;
	X << descriptor_matrix_.t()*descriptor_matrix_;
	
	calculateEigenvectors(X,frac_var_,loadings_);

	latent_variables_.ReSize(descriptor_matrix_.Nrows(),loadings_.Ncols());
	for(uint i=1;i<=loadings_.Ncols();i++)
	{
		latent_variables_.Column(i)=descriptor_matrix_*loadings_.Column(i);
	}
	
	RRModel m(*data);
	m.descriptor_matrix_=latent_variables_;
	m.Y_=Y_;
	m.train();

	//result of RR is a linear combination of latente variables 
	// = column with length=no of latente variables => matrix for more than one modelled activity
	weights_ = *m.getTrainingResult();
	training_result_ = loadings_*weights_;
}


void PCRModel::setParameters(vector<double>& v)
{
	if(v.size()!=1)
	{
		String c = "Wrong number of model parameters! Needed: 1;";
		c = c+" given: "+String(v.size());
		throw Exception::ModelParameterError(__FILE__,__LINE__,c.c_str());
	}
	frac_var_ = v[0];
}


vector<double> PCRModel::getParameters() const
{
	vector<double> d;
	d.push_back(frac_var_);
	return d;
}
