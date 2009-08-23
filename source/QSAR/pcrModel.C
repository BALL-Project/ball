/* pcrModel.C
 * 
 * Copyright (C) 2009 Marcel Schumann
 * 
 * This file is part of QuEasy -- A Toolbox for Automated QSAR Model
 * Construction and Validation.
 * QuEasy is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 * 
 * QuEasy is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

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
	
	// find the smallest singular vector that should be taken into account
	// complete variance == sum of all eigen-values == sum of squared singular values
	double complete_var=singular_values.sum();
	
	double explained_var=0;
	uint cols=0; 
	if(complete_var < 5*Matrix<double>::MACHINE_EPSILON)
	{
		throw Exception::NoPCAVariance(__FILE__,__LINE__,"No variance present to be explained by PCA!");
	}
	
	uint last_vector=1;
	for (; last_vector<=singular_values.getSize() && cols<data.getRowCount() && explained_var/complete_var<frac_var ; last_vector++) 
	{
		// (singular-value)^2 == eigen-value
		explained_var+=singular_values(last_vector);
		cols++;
	}
	last_vector--;

	output.ReSize(data.Nrows(),cols);
	
	// getRightSingularVectors() returns V.t() NOT V, so we have to transform back to V here !!
	Matrix<double> V = solver.getRightSingularVectors().t();
	
	for (uint i=1; i<=data.getRowCount(); i++)
	{
		for (uint j=1; j<=last_vector ; j++)
		{
			output(i,j) = V(i,j);
		}
	}
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
