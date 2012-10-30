// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//

#ifndef BALL_QSAR_KERNEL_H
#define BALL_QSAR_KERNEL_H

#ifndef BALL_QSAR_LINEARMODEL_H
#include <BALL/QSAR/linearModel.h>
#endif

#ifndef BALL_QSAR_MODEL_H
#include <BALL/QSAR/Model.h>
#endif

#ifndef BALL_QSAR_EXCEPTION_H
#include <BALL/QSAR/exception.h>
#endif

#ifndef BALL_QSAR_REGRESSIONMODEL_H
#include <BALL/QSAR/regressionModel.h>
#endif

#include <cmath>
#include <sstream>

namespace BALL
{
	namespace QSAR
	{
		class BALL_EXPORT Kernel
		{
			public:
				/** @name Constructors and Destructors
				 */
				//@{
				Kernel(Model* m, int k_type, double p1, double p2=-1);

				Kernel(Model* m, String f, String g);

				Kernel(Model* m, Eigen::VectorXd& w);

				/** constructor for weighted distance kernel.
				@param column no of column of LinearModel.training_result that is to be used as weights vector */
				Kernel(Model* m, const LinearModel& lm, int column);

				~Kernel();

				EIGEN_MAKE_ALIGNED_OPERATOR_NEW
				//@}


				/** @name Accessors
				 */
				//@{
				/** calculates pairwise distances between all substances in Eigen::MatrixXd input and saves them to Eigen::MatrixXd output.\n
				If Kernel.weights is not empty, function Kernel.calculateWeightedDistanceMatrix() is used \n 
				Else if: Kernel.f=="" and Kernel.g="", the distance between two substances a and b is calculated as \f$ \sum_{i=1}^m (input_{ai} * input_{bi})^p \f$, with m=\#descriptors  \n
				Else: distance is calculated as \f$ g(\sum_{i=1}^m f(input_{ai}, input_{bi})) \f$*/
				void calculateKernelMatrix(Eigen::MatrixXd& input, Eigen::MatrixXd& output);
				
				/** calculates pairwise distance between all substances of m1 and m2 and saves them to Eigen::MatrixXd output. \n
				If Kernel.weights is not empty, function Kernel.calculateWeightedDistanceMatrix() is used \n 
				Esle if: Kernel.f=="" and Kernel.g="", the distance between two substances a and b is calculated as \f$ \sum_{i=1}^m (m1_{ai} * m2_{bi})^p \f$, with m=\#descriptors \n
				Else: distance is calculated as \f$ g(\sum_{i=1}^m f(m1_{ai}, m2_{bi})) \f$*/
				void calculateKernelMatrix(Eigen::MatrixXd& K, Eigen::MatrixXd& m1, Eigen::MatrixXd& m2, Eigen::MatrixXd& output);
				
				
				/** transforms test data 'input' into the kernel-saves and saves it to matrix 'output' */
				void calculateKernelVector(Eigen::MatrixXd& K,Eigen::VectorXd& m1, Eigen::MatrixXd& m2, Eigen::RowVectorXd& output);
				
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

				/** calculates pairwise distances between all substances in Eigen::MatrixXd input, weighted by the contribution of every descriptor (as encoded in Kernel.weights), and saves them to Eigen::MatrixXd output.\n
				Distance between two substances a and b is calculated as \f$ \sum_{i=1}^m w_i * (input_{ai}- input_{bi})^2 \f$, with m=\#descriptors */
				void calculateWeightedKernelMatrix(Eigen::MatrixXd& input, Eigen::MatrixXd& output);
				
				
				/** calculates pairwise distances between all substances of m1 and m2, weighted by the contribution of every descriptor (as encoded in Kernel.weights), and saves them to Eigen::MatrixXd output.\n
				Distance between two substances a and b is calculated as \f$ \sum_{i=1}^m w_i * (m1_{ai}- m2_{bi})^2 \f$, with m=\#descriptors */
				void calculateWeightedKernelMatrix(Eigen::MatrixXd& m1, Eigen::MatrixXd& m2, Eigen::MatrixXd& output);
				
				
				void calculateKernelMatrix1(Eigen::MatrixXd& input, Eigen::MatrixXd& output);
				
				void calculateKernelMatrix2(Eigen::MatrixXd& input, Eigen::MatrixXd& output);
				
				void calculateKernelMatrix3(Eigen::MatrixXd& input, Eigen::MatrixXd& output);
				
				void calculateKernelMatrix4(Eigen::MatrixXd& input, Eigen::MatrixXd& output);
				
				
				void calculateKernelMatrix1(Eigen::MatrixXd& m1, Eigen::MatrixXd& m2, Eigen::MatrixXd& output);
				
				void calculateKernelMatrix2(Eigen::MatrixXd& m1, Eigen::MatrixXd& m2, Eigen::MatrixXd& output);
				
				void calculateKernelMatrix3(Eigen::MatrixXd& m1, Eigen::MatrixXd& m2, Eigen::MatrixXd& output);
				
				void calculateKernelMatrix4(Eigen::MatrixXd& m1, Eigen::MatrixXd& m2, Eigen::MatrixXd& output);
				//@}
				
				
				/** @name Attributes
				*/
				//@{
				/** pointer to the model which uses this kernel */
				Model* model_;
				
				Eigen::VectorXd weights_;
				//@}
				
				
				friend class FeatureSelection;
			
		};
	}
}

#endif // KERNEL
