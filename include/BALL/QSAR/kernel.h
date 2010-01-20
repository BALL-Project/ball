/* kernel.h
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

#ifndef KERNEL
#define KERNEL

#ifndef LMODEL
#include <BALL/QSAR/linearModel.h>
#endif

#ifndef MODEL
#include <BALL/QSAR/Model.h>
#endif

#ifndef QSAR_EXCEPTION
#include <BALL/QSAR/exception.h>
#endif

#ifndef REGRESSION
#include <BALL/QSAR/regressionModel.h>
#endif

#include <cmath>
#include <sstream>


namespace BALL
{
	namespace QSAR
	{
		class Kernel
		{	
			
		
			public:
				/** @name Constructors and Destructors
				 */
				//@{
				Kernel(Model* m, int k_type, double p1, double p2=-1);
				
				Kernel(Model* m, String f, String g);
						
				Kernel(Model* m, Vector<double>& w);
				
				/** constructor for weighted distance kernel.
				@param column no of column of LinearModel.training_result that is to be used as weights vector */
				Kernel(Model* m, const LinearModel& lm, int column);

				~Kernel();
				//@}
				
				
				/** @name Accessors
				 */
				//@{
				/** calculates pairwise distances between all substances in Matrix<double> input and saves them to Matrix<double> output.\n
				If Kernel.weights is not empty, function Kernel.calculateWeightedDistanceMatrix() is used \n 
				Else if: Kernel.f=="" and Kernel.g="", the distance between two substances a and b is calculated as \f$ \sum_{i=1}^m (input_{ai} * input_{bi})^p \f$, with m=\#descriptors  \n
				Else: distance is calculated as \f$ g(\sum_{i=1}^m f(input_{ai}, input_{bi})) \f$*/
				void calculateKernelMatrix(Matrix<double>& input, Matrix<double>& output);
				
				/** calculates pairwise distance between all substances of m1 and m2 and saves them to Matrix<double> output. \n
				If Kernel.weights is not empty, function Kernel.calculateWeightedDistanceMatrix() is used \n 
				Esle if: Kernel.f=="" and Kernel.g="", the distance between two substances a and b is calculated as \f$ \sum_{i=1}^m (m1_{ai} * m2_{bi})^p \f$, with m=\#descriptors \n
				Else: distance is calculated as \f$ g(\sum_{i=1}^m f(m1_{ai}, m2_{bi})) \f$*/
				void calculateKernelMatrix(Matrix<double>& K, Matrix<double>& m1, Matrix<double>& m2, Matrix<double>& output);
				
				
				/** transforms test data 'input' into the kernel-saves and saves it to matrix 'output' */
				void calculateKernelVector(Matrix<double>& K,Vector<double>& m1, Matrix<double>& m2, Vector<double>& output);
				
				/** grid search for the best kernel parameters.\n
				Grid search is done locally around the current kernel parameter value(s).
				@param opt if ==1, Model.optitimizeParameters() is used in each step of grid search, optimizing the parameter of the *Model* in addition to those of the kernel.
				@param step_width the size of each step to be made
				@param steps the number of steps for grid search 
				@param recursions number of recursions of grid search; in each recursion the step width is decreased by factor of 10 and searching is done in 20 steps around the values of the best kernel parameters determined in last recursion */
				void gridSearch(double step_width, int steps, int recursions, int k, bool opt=0);
				//@}
				
				
				/** @name Attributes
				 */
				//@{
				/** specifies which kind of kernel is chosen:\n
				1 = polynomial kernel \n
				2 = radial basis function kernel \n
				3 = sigmoid kernel \n
				4 = individual kernel-function \n
				5 = weighted distance kernel */
				int type;
				
				/** parameters for kernel functions set by the user */
				double par1, par2;
				
				/** Equation for distance function for calculation of kernel matrix.\n
				Distance of two substances a and b is calculated as \f$ dist(a,b)= g(\sum_{i=1}^m f(x1,x2)) \f$, with m=\#descriptors  \n
				Use "x1" and "x2" in the String, e.g. "x1*x2" */
				String equation1;
				
				/** Equation for distance function for calculation of kernel matrix.\n
				Distance of two substances a and b is calculated as \f$ dist(a,b)= g(\sum_{i=1}^m f(x1,x2)) \f$, with m=\#descriptors  \n
				g determines what is to be done with the calculated "sum" over all elements (use "sum" in String); e.g. "sum^0.5" => euclidean distance if f=="x1*x2" */
				String equation2;
				//@}
				
				
			protected:
				/** @name Accessors
				 */
				//@{

				void gridSearch(double step_width, int steps, bool first_rec, int k, double par1_start, double par2_start, bool opt);

				/** calculates pairwise distances between all substances in Matrix<double> input, weighted by the contribution of every descriptor (as encoded in Kernel.weights), and saves them to Matrix<double> output.\n
				Distance between two substances a and b is calculated as \f$ \sum_{i=1}^m w_i * (input_{ai}- input_{bi})^2 \f$, with m=\#descriptors */
				void calculateWeightedKernelMatrix(Matrix<double>& input, Matrix<double>& output);
				
				
				/** calculates pairwise distances between all substances of m1 and m2, weighted by the contribution of every descriptor (as encoded in Kernel.weights), and saves them to Matrix<double> output.\n
				Distance between two substances a and b is calculated as \f$ \sum_{i=1}^m w_i * (m1_{ai}- m2_{bi})^2 \f$, with m=\#descriptors */
				void calculateWeightedKernelMatrix(Matrix<double>& m1, Matrix<double>& m2, Matrix<double>& output);
				
				
				void calculateKernelMatrix1(Matrix<double>& input, Matrix<double>& output);
				
				void calculateKernelMatrix2(Matrix<double>& input, Matrix<double>& output);
				
				void calculateKernelMatrix3(Matrix<double>& input, Matrix<double>& output);
				
				void calculateKernelMatrix4(Matrix<double>& input, Matrix<double>& output);
				
				
				void calculateKernelMatrix1(Matrix<double>& m1, Matrix<double>& m2, Matrix<double>& output);
				
				void calculateKernelMatrix2(Matrix<double>& m1, Matrix<double>& m2, Matrix<double>& output);
				
				void calculateKernelMatrix3(Matrix<double>& m1, Matrix<double>& m2, Matrix<double>& output);
				
				void calculateKernelMatrix4(Matrix<double>& m1, Matrix<double>& m2, Matrix<double>& output);
				//@}
				
				
				/** @name Attributes
				*/
				//@{
				/** pointer to the model which uses this kernel */
				Model* model_;
				
				Vector<double> weights_;
				//@}
				
				
				friend class FeatureSelection;
			
		};
	}
}

#endif // KERNEL
