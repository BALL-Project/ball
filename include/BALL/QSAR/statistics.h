/* statistics.h
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

#ifndef STATISTICS
#define STATISTICS

#include <vector>
#include <iostream>
#include <BALL/MATHS/LINALG/matrix.h>
#include <cmath>
#include <BALL/MATHS/parsedFunction.h>

#include <set>

namespace BALL 
{
	namespace QSAR
	{
		/** class for statistical correction of input-data */
		class BALL_EXPORT Statistics
		{
			public:
				/** @name Constructors and Destructors
				 */
				//@{
				/** defaut constructor */
				Statistics(){};
				
				/** defaut destructor */
				~Statistics(){};
				//@}
				
				
				
				/** @name static functions
				 */
				//@{
				/** scales the given vector to a variance of 1 */
				static void scaling(std::vector<double>& v);
	
				/** scales each column of the given vector-based matrix to a variance of 1 */
				static void scaling(std::vector<std::vector<double> >& m);
	
				/** centers each colum of the given vector-based matrix to a variance of 1 and mean of 0 */
				static void centering(std::vector<std::vector<double> >& m);
				
				/** centers the given vector to a variance of 1 and mean of 0 */
				static void centering(std::vector<double>& v);
				
				/** centers the given vector to a variance of 1 and mean of 0  
				@param mean the mean of v will be saved here
				@param std the standart deviation of v will be saved here */ 
				static void centering(vector<double>& v, double& mean, double& std);
				
				/** calculate variance of a given vector */
				static double getVariance(const std::vector<double>& v, double mean=-1);
				
				/** calculate standart deviation of a given vector */
				static double getStddev(const vector<double>& v, double mean=-1);
				
				/** calculate covariance between two given vectors */
				static double getCovariance(const vector<double>& v1, const vector<double>& v2, double mean1, double mean2);
	
				/** calculate mean of given vector */
				static double getMean(const std::vector<double>& v);
	
				
				//--- methods for calculating mean, covar, var of matrix-ROWS ---
				
				/** calculates covarianve between two rows of the given matrix 
				@param features_to_use if specified, only the contained features are used for calculation of the covariance */
				static double getRowCovariance(const vector<vector<double> >& v, int row1, int row2, double mean1=-1, double mean2=-1, std::multiset<int>* features_to_use=0);
				
				/** calculates mean of a row of the given matrix 
				@param features_to_use if specified, only the contained features are used for calculation of the mean */
				static double getRowMean(const vector<vector<double> >& v, int row, std::multiset<int>* features_to_use=0);
				
				/** calculates variance of a row of the given matrix 
				@param features_to_use if specified, only the contained features are used for calculation of the variance */
				static double getRowVariance(const vector<vector<double> >& v, int row, double mean=-1, std::multiset<int>* features_to_use=0);
				
				/** calculates standard deviation of a row of the given matrix 
				@param features_to_use if specified, only the contained features are used for calculation of the standard deviation */
				static double getRowStddev(const vector<vector<double> >& v, int row, double mean=-1, std::multiset<int>* features_to_use=0);
								
				//  ------------------------
				
				
				/** scales each column of the given newmat-matrix to a variance of 1 */
				static void centering(BALL::Matrix<double>& m);
	
				/** scales column col of the given newmat-matrix m to a variance of 1 */
				static void centering(BALL::Matrix<double>& m, int col);
	
				/** calculates mean of column col of newmat-matrix m */
				static double getMean(const BALL::Matrix<double>& m, int col);
	
				/** calculates variance of column col of newmat-matrix m */
				static double getVariance(const BALL::Matrix<double>& m, int col, double mean=-1);
				
				/** calculated standard deviation of column col of matrix m */
				static double getStddev(const BALL::Matrix<double>& m, int col, double mean=-1);
				
				/** calculates covariance between columns col1 and col2 of matrix m*/
				static double getCovariance(const BALL::Matrix<double>& m, int col1, int col2, double mean1=-1, double mean2=-1);
				
				/** calculates the sum-of-square of column col of newmat-matrix m */
				static double sq(const BALL::Matrix<double>& m, int col, double mean=-1);
				
				/** calculates polynomial distance between two rows of newmat-matrix m */
				static double distance(const BALL::Matrix<double>& m, int& row1, int& row2, double& p);
				
				/** calculates polynomial distance between row1 of BALL::Matrix<double> m1 and row2 of BALL::Matrix<double> m2.\n
				m1 and m2 must therefore have the same number of columns.*/
				static double distance(const BALL::Matrix<double>& m1, const BALL::Matrix<double>& m2, int& row1, int& row2, double& p);

				
				/** calculates the distance between row1 of BALL::Matrix<double> m1 and row2 of BALL::Matrix<double> m2 according to the given equations 
				@param f equation f, determines how elements "x1" of row1 are charged against elements "x2" of row2. Use "x1" and "x2" in the String, e.g. "x1*x2"
				@param g equation g, determines what is to be done with the calculated "sum" over all elements (use "sum" in String); e.g. "sum^0.5" => euclidean distance if f=="x1*x2" */
				static double distance(const BALL::Matrix<double>& m1, const BALL::Matrix<double>& m2, int& row1, int& row2, String& f, String& g);
				
				
				/** calculates euclidean distance between row1 of BALL::Matrix<double> m1 and row2 of BALL::Matrix<double> m2 as \f$ sqrt(\sum_i (row1_i-row2_i)^2) \f$\n
				m1 and m2 must therefore have the same number of columns.*/
				static double euclDistance(const BALL::Matrix<double>& m1, const BALL::Matrix<double>& m2, int row1, int row2);
				
				/** Returns the euclidic norm of a ColumnVector as \f$ sqrt(\sum_i cv_i^2) \f$*/
				static double euclNorm(const Vector<double>& cv);
		
				/** Returns the euclidician difference between two ColumnVectors as \f$ sqrt(\sum_i (cv1_i-cv2_i)^2) \f$ */
				static double euclDistance(const Vector<double>& c1, const Vector<double>& c2);
				
				/** returns the scalar product \f$ cv^T*cv = \sum_i cv_i^2 \f$ */
				static double scalarProduct(const Vector<double>& cv);
				//@}
		
		};
	}
}


#endif // STATISTICS
