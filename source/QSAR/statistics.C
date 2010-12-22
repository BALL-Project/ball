/* statistics.C
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

#include <BALL/QSAR/statistics.h>
#include <BALL/COMMON/limits.h>
#include <iostream>

namespace BALL
{
	namespace QSAR
	{

		void Statistics::scaling(vector<vector<double> >& m)
		{
			for (unsigned int i = 0; i < m.size(); i++)
			{
				scaling(m[i]);
			}
		}


		void Statistics::scaling(vector<double>& v)
		{
			double std = sqrt(getVariance(v));
	
			// standard deviation = 0, i.e. all values of this vector are identical, so do nothing!
			if (std < 5*std::numeric_limits < double > ::epsilon()) return; 
	
			for (unsigned int i = 0; i < v.size(); i++)
			{
				v[i] /= std;
			}
		}


		void Statistics::centering(vector<vector<double> >& m)
		{
			for (unsigned int i = 0; i < m.size(); i++)
			{
				centering(m[i]);
			}
		}


		void Statistics::centering(vector<double>& v)
		{
			double mean = getMean(v);
			double std = sqrt(getVariance(v, mean));

			// standard deviation = 0, i.e. all values of this vector are identical, so do nothing!
			if (std < 5*std::numeric_limits < double > ::epsilon()) return; 
	
			for (unsigned int i = 0; i < v.size(); i++)
			{
				v[i] = (v[i]-mean)/std;
			}
		}


		void Statistics::centering(vector<double>& v, double& mean, double& std)
		{
			mean = getMean(v);
			std = sqrt(getVariance(v, mean));

			// standard deviation = 0, i.e. all values of this vector are identical, so do nothing!
			if (std < 5*std::numeric_limits < double > ::epsilon()) return; 

			for (unsigned int i = 0; i < v.size(); i++)
			{
				v[i] = (v[i]-mean)/std;
			}
		}


		double Statistics::getVariance(const vector<double>& v, double mean)
		{
			if (mean == -1) {	mean = getMean(v); }
			double sum_of_squares = 0;
			for (uint i = 0; i < v.size(); i++)
			{
				sum_of_squares += (v[i]-mean)*(v[i]-mean);
			}
			return sum_of_squares/(v.size()-1);
		}


		double Statistics::getStddev(const vector<double>& v, double mean)
		{
			double var = getVariance(v, mean);
			return sqrt(var);
		}


		double Statistics::getCovariance(const vector<double>& v1, const vector<double>& v2, double mean1, double mean2)
		{
			if (mean1 == -1) {mean1 = getMean(v1); }
			if (mean2 == -1) {mean2 = getMean(v2); }
			double sum_of_squares = 0;
			for (uint i = 0; i < v1.size() && i < v2.size(); i++)
			{
				sum_of_squares += (v1[i]-mean1)*(v2[i]-mean2);
			}
			return sum_of_squares/(v1.size()-1);
		}



		double Statistics::getMean(const vector<double>& v)
		{
			double sum = 0;
			for (uint i = 0; i < v.size(); i++)
			{
				sum += v[i];
			}
			return sum/v.size();
		}



		//---------------- methods for calculating mean, covar, var of matrix-ROWS  ----------


		double Statistics::getRowCovariance(const vector<vector<double> >& v, int row1, int row2, double mean1, double mean2, std::multiset<int>* features_to_use)
		{
			if (mean1 == -1) {mean1 = getRowMean(v, row1, features_to_use); }
			if (mean2 == -1) {mean2 = getRowMean(v, row2, features_to_use); }
			double sum_of_squares = 0;
			int size = v.size();
			std::multiset<int>::iterator it;
			if (features_to_use != 0) 
			{
				it = features_to_use->begin();
				size = features_to_use->size();
			}	
			
			for (uint i = 0; i < v.size(); i++)
			{
				if (features_to_use != 0 && *it != (int)i) continue; 
				sum_of_squares += (v[i][row1]-mean1)*(v[i][row2]-mean2);
				if (features_to_use != 0) it++; 
			}
			return sum_of_squares/(size-1);
		}

		double Statistics::getRowMean(const vector<vector<double> >& v, int row, std::multiset<int>* features_to_use)
		{
			double sum = 0;
			int size = v.size();
			std::multiset<int>::iterator it;
			if (features_to_use != 0) 
			{
				it = features_to_use->begin();
				size = features_to_use->size();
			}	
			
			for (uint i = 0; i < v.size(); i++)
			{
				if (features_to_use != 0 && *it != (int)i) continue; 
				sum += v[i][row];
				if (features_to_use != 0) it++; 
			}
			return sum/size;
		}

		double Statistics::getRowVariance(const vector<vector<double> >& v, int row, double mean, std::multiset<int>* features_to_use)
		{
			if (mean == -1) {	mean = getRowMean(v, row, features_to_use); }
			double sum_of_squares = 0;
			int size = v.size();
			std::multiset<int>::iterator it;
			if (features_to_use != 0) 
			{
				it = features_to_use->begin();
				size = features_to_use->size();
			}
			
			for (uint i = 0; i < v.size(); i++)
			{
				if (features_to_use != 0 && *it != (int)i) continue; 
				sum_of_squares += (v[i][row]-mean)*(v[i][row]-mean);
				if (features_to_use != 0) it++; 
			}
			return sum_of_squares/(size-1);
		}

		double Statistics::getRowStddev(const vector<vector<double> >& v, int row, double mean, std::multiset<int>* features_to_use)
		{
			double var = getRowVariance(v, row, mean, features_to_use);
			return sqrt(var);
		}


		// -----------------------------------------------------------------


		void Statistics::centering(Matrix<double>& m)
		{
			for (int i = 1; i <= m.Ncols(); i++)
			{
				centering(m, i);
			}
		}


		void Statistics::centering(Matrix<double>& m, int col)
		{
			double mean = getMean(m, col);
			double std = sqrt(getVariance(m, col, mean));
	
			// standard deviation = 0, i.e. all values of this column are identical, so do nothing!
			if (std < 5*std::numeric_limits < double > ::epsilon()) return; 
	
			for (int i = 1; i <= m.Nrows(); i++)
			{
				m(i, col) = (m(i, col)-mean)/std;
			}
		}

		double Statistics::getMean(const Matrix<double>& m, int col)
		{
			double sum = 0;
			for (int i = 1; i <= m.Nrows(); i++)
			{
				sum += m(i, col);
			}
			return sum/m.Nrows();
		}

		double Statistics::getVariance(const Matrix<double>& m, int col, double mean)
		{
			if (mean == -1) {	mean = getMean(m, col); }
			double sum_of_squares = 0;
			for (int i = 1; i <= m.Nrows(); i++)
			{
				sum_of_squares += pow(m(i, col)-mean, 2);
			}
			return sum_of_squares/(m.Nrows()-1);
		}

		double Statistics::getStddev(const Matrix<double>& m, int col, double mean)
		{
			double d = getVariance(m, col, mean);
			return sqrt(d);
		}

		double Statistics::getCovariance(const Matrix<double>& m, int col1, int col2, double mean1, double mean2)
		{
			if (mean1 == -1) {mean1 = getMean(m, col1); }
			if (mean2 == -1) {mean2 = getMean(m, col2); }
			double sum_of_squares = 0;
			for (int i = 1; i <= m.Nrows(); i++)
			{
				sum_of_squares += (m(i, col1)-mean1)*(m(i, col2)-mean2);
			}
			return sum_of_squares/(m.Nrows()-1);
		}


		double Statistics::sq(const Matrix<double>& m, int col, double mean)
		{
			if (mean == -1) {	mean = getMean(m, col); }
			double sum_of_squares = 0;
			for (int i = 1; i <= m.Nrows(); i++)
			{
				sum_of_squares += pow(m(i, col)-mean, 2);
			}
			return sum_of_squares;
		}

		double Statistics::euclNorm(const Vector<double>& cv)
		{
			return sqrt(scalarProduct(cv));
		}


		double Statistics::scalarProduct(const Vector<double>& cv)
		{
			double n = 0;
			for (uint i = 1; i <= cv.getSize(); i++)
			{
				n += cv(i)*cv(i);
			}
			return n;
		}


		double Statistics::euclDistance(const Vector<double>& c1, const Vector<double>& c2)
		{
			if (c1.getSize() != c2.getSize()) 
			{
				BALL::Exception::VectorHasWrongDimension e;
				e.setMessage("Vectors must have the same number of cells in order to be able to calculate the euclidian distance between them!!");
				throw e;
			}
			
			double n = 0;
			for (uint i = 1; i <= c1.getSize(); i++)
			{
				n += pow((c1(i)-c2(i)), 2);
			}
			return sqrt(n);
		}

		//---------------------------


		double Statistics::distance(const Matrix<double>& m, int& row1, int& row2, double& p)
		{
			double dist = 0;
			for (int j = 1; j <= m.Ncols(); j++)
			{
				dist += m(row1, j)*m(row2, j);
				
			}
				
			int i_p = static_cast <int> (p);
			if (i_p != p) // if a root of dist should be taken, then dist may not be negative
			{
				dist = abs(dist);
			}
			return pow(dist, p);
		}


		double Statistics::distance(const Matrix<double>& m1, const Matrix<double>& m2, int& row1, int& row2, double& p)
		{
			if (m1.Ncols() != m2.Ncols()) 
			{
				BALL::Exception::MatrixHasWrongDimension e;
				e.setMessage("Matrices must have the same number of columns in order to be able to calculate a distance between two of their rows!!");
				throw e;
			}
			
			double dist = 0;
			for (int j = 1; j <= m1.Ncols(); j++)
			{
				dist += m1(row1, j)*m2(row2, j);
			}
				
			int i_p = static_cast <int> (p);
			if (i_p != p) // if a root of dist should be taken, then dist may not be negative
			{
				dist = abs(dist);
			}
			return pow(dist, p);
		}


		double Statistics::distance(const Matrix<double>& m1, const Matrix<double>& m2, int& row1, int& row2, String& f, String& g)
		{
			if (m1.Ncols() != m2.Ncols()) 
			{
				BALL::Exception::MatrixHasWrongDimension e;
				e.setMessage("Matrices must have the same number of columns in order to be able to calculate a distance between two of their rows!!");
				throw e;
			}
			
			double dist = 0;
			for (int j = 1; j <= m1.Ncols(); j++)
			{
				String var="";
				var = var+"x1="+String(m1(row1, j))+";x2="+String(m2(row2, j))+";";
			//	cout<<"f = "<<var+f<<endl;
				ParsedFunction<double> pf(var+f);
				dist += pf(0);
			}	
			String var2="";
			var2 = var2+"sum="+String(dist)+";";
			//cout<<"g = "<<var+g<<endl;
			ParsedFunction<double> pf2(var2+g);
			return pf2(0);
		}


		double Statistics::euclDistance(const Matrix<double>& m1, const Matrix<double>& m2, int row1, int row2)
		{
			if (m1.Ncols() != m2.Ncols()) 
			{
				BALL::Exception::MatrixHasWrongDimension e;
				e.setMessage("Matrices must have the same number of columns in order to be able to calculate the euclidian distance between two of their rows!!");
				throw e;
			}
			
			double dist = 0;
			for (int j = 1; j <= m1.Ncols(); j++)
			{
				dist += pow(m1(row1, j)-m2(row2, j), 2);
			}	
			return sqrt(dist);
		}
	}
}
