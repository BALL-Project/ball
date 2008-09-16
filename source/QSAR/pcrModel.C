// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// 

#include <BALL/QSAR/pcrModel.h>
using namespace BALL::QSAR;

#ifndef BALL_LINALG_SVDSOLVER_H
#include <BALL/MATHS/LINALG/SVDSolver.h>
#endif


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

void PCRModel::calculateEigenvectors(const Matrix<double>& data, double frac_var, Matrix<double>& output)
{
	SVDSolver<double> solver(data);
	solver.computeSVD();
	Vector<double> singular_values = solver.getSingularValues();
	
	cout<<"SV="<<singular_values<<endl;

	// find the smallest singular vector that should be taken into account
	// complete variance == sum of all eigen-values == sum of squared singular values
	double complete_var=singular_values.sum();
	
	double explained_var=0;
	int cols=0; 
	if(complete_var==0)
	{
		throw Exception::NoPCAVariance(__FILE__,__LINE__,"No variance present to be explained by PCA!");
	}
	
	int last_vector=1;
	for (; last_vector<=singular_values.getSize() && cols<data.Nrows() && explained_var/complete_var<frac_var ; last_vector++) 
	{
		// (singular-value)^2 == eigen-value
		explained_var+=singular_values(last_vector);
		cols++;
	}
	last_vector--;
	
	cout<<"variance="<<complete_var<<endl;
	cout<<"last_vector="<<last_vector<<endl;
	
	output.ReSize(data.Nrows(),cols);
	
	// getRightSingularVectors() returns V.t() NOT V, so we have to transform back to V here !!
	Matrix<double> V = solver.getRightSingularVectors().t();
	
	for (int i=1; i<=data.Nrows(); i++)
	{
		for (int j=1; j<=last_vector ; j++)
		{
			output(i,j) = V(i,j);
		}
	}
	
	cout<<output.Nrows()<<"  "<<output.Ncols()<<endl;
}


void PCRModel::train()
{
	if(descriptor_matrix_.Ncols()==0)
	{
		throw Exception::InconsistentUsage(__FILE__,__LINE__,"Data must be read into the model before training!");
	}
	Matrix<double> XX = descriptor_matrix_.t()*descriptor_matrix_;
	
	calculateEigenvectors(XX,frac_var_,loadings_);

	latent_variables_=descriptor_matrix_*loadings_;
	
	RRModel m(*data);
	m.descriptor_matrix_=latent_variables_;
	m.Y_=Y_;
	m.train();

	//result of RR is a linear combination of latente variables 
	// = column with length=no of latente variables => matrix for more than one modelled activity
	weights_ = *m.getTrainingResult();
	training_result_ = loadings_*weights_;
	
	calculateOffsets();
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
