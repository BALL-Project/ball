/* allModel.C
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

#include <BALL/QSAR/allModel.h>
#include <BALL/MATHS/LINALG/matrixInverter.h>

namespace BALL
{
	namespace QSAR
			{

		ALLModel::ALLModel(const QSARData& q, double kw) : NonLinearModel(q) 
		{
			kw_ = kw;
			type_ = "ALL";
			lambda_ = 0.005;
			training_result_.ReSize(0,0);
			default_no_opt_steps_ = 50;
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

		void ALLModel::calculateEuclDistanceMatrix(Matrix<double>& m1, Matrix<double>& m2, Matrix<double>& output)
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


		BALL::Vector<double> ALLModel::predict(const vector<double>& substance, bool transform)
		{
			if(descriptor_matrix_.Ncols()==0)
			{
				throw Exception::InconsistentUsage(__FILE__,__LINE__,"Training data must be read into the ALL-model before the activity of a substance can be predicted!");
			}
 			Vector<double> v0=getSubstanceVector(substance,transform);
		 	
			Matrix<double> v(1,v0.getSize()); v.copyVectorToRow(v0,1);
			Matrix<double> dist;
			
			// calculate distances between the given substance and the substances of X
			// dimension of dist: 1xn 
 			calculateEuclDistanceMatrix(v, descriptor_matrix_, dist);
			Vector<double> w;
			calculateWeights(dist, w);

			Matrix<double> XX;
			
			// calculate X.t()*X as cross-products weighted by the similarity of the given substance to the respective row of X
			calculateXX(w, XX);
			
 			Matrix<double> XY;
			
			// calculate X.t()*Y_ as cross-products weighted by the similarity of the given substance to the respective row of X
 			calculateXY(w, XY);
			
			// rigde regression in order to be able to cope with linearly dependent columns, i.e. singular matrices
			Matrix<double> im; im.setToIdentity(XX.Nrows());
			im*=lambda_;
			XX+=im;

			MatrixInverter<double, StandardTraits> inverter(XX);
			inverter.computeInverse();

			Matrix<double> train = inverter.getInverse();
			
			train*=XY;
			
			Vector<double> res(Y_.getColumnCount());
			res = v0*train;
			
			if(transform && y_transformations_.Ncols()!=0)
			{
				backTransformPrediction(res);
			}
			
			return res;	
		}

		void ALLModel::calculateWeights(Matrix<double>& dist, Vector<double>& w)
		{
			w.resize(dist.Ncols());
			for(int i=1; i<=dist.Ncols();i++)
			{
				w(i)=exp(-pow(dist(1,i),2)/(2*pow(kw_,2)));
			}
		}


		void ALLModel::calculateXX(Vector<double>& w, Matrix<double>& res)
		{
			res.resize(descriptor_matrix_.Ncols(),descriptor_matrix_.Ncols());
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


		void ALLModel::calculateXY(Vector<double>& w, Matrix<double>& res)
		{
			res.resize(descriptor_matrix_.Ncols(),Y_.Ncols());
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


		bool ALLModel::optimizeParameters(int k, int no_steps)
		{
			double best_q2=0;
			double best_kw=0;
			kw_ = 0;
			for(int i=0;i<no_steps;i++)
			{
				kw_+=1;
				validation->crossValidation(k);
				if(validation->getQ2()>best_q2)
				{
					best_q2=validation->getQ2();
					best_kw=kw_;
				}
				else if(validation->getQ2()<best_q2*0.5)
				{
					break;
				}
			}
			kw_=best_kw;
			validation->setQ2(best_q2);
			
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
			
			const Matrix<double>* coeffErrors = validation->getCoefficientStdErrors();
			bool sterr=0;
			if(coeffErrors->Ncols()!=0)
			{
				sterr=1;
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
			
			int no_y = training_result_.Ncols();
			if(no_y==0) no_y = y_transformations_.Ncols(); // correct no because transformation information will have to by read anyway when reading this model later ...
			
			out<<"# model-type_\tno of featues in input data\tselected featues\tno of response variables\tcentered descriptors?\tcentered response?\tno of substances"<<endl;
			out<<type_<<"\t"<<data->getNoDescriptors()<<"\t"<<sel_features<<"\t"<<no_y<<"\t"<<centered_data<<"\t"<<centered_y<<"\t"<<descriptor_matrix_.Nrows()<<"\n\n";
				
			saveModelParametersToFile(out);
			saveResponseTransformationToFile(out);
			saveDescriptorInformationToFile(out);
			out<<descriptor_matrix_<<endl;
			out<<Y_<<endl;
			
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
			
			substance_names_.clear();
			
			getline(input,line0);  // skip empty line	
			readModelParametersFromFile(input);
			if(centered_y)
			{
				readResponseTransformationFromFile(input, no_y);
			}
			Model::readDescriptorInformationFromFile(input, no_descriptors, centered_data);
			readMatrix(descriptor_matrix_,input,no_substances,no_descriptors); 
			getline(input,line0);  // skip empty line
			readMatrix(Y_,input,no_substances,no_y);
		}
	}
}
