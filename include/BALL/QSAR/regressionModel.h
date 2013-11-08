// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// 

#ifndef BALL_QSAR_REGRESSIONMODEL_H
#define BALL_QSAR_REGRESSIONMODEL_H

#include <BALL/QSAR/Model.h>
#include <BALL/QSAR/regressionValidation.h>

#include <fstream>

namespace BALL
{
	class Rescoring;

	namespace QSAR
	{
		class BALL_EXPORT RegressionModel : public Model
		{
			public:
				/** @name Constructors and Destructors
				 */
				//@{
				/** constructur,
				@param q QSAR-wrapper object, from which the data for this model should be taken */
				RegressionModel(const QSARData& q);
				
				~RegressionModel();

				EIGEN_MAKE_ALIGNED_OPERATOR_NEW

				virtual void operator=(const RegressionModel& m);
				//@}
				
				
				/** @name Accessors
				 */
				//@{
				/** a ModelValidation object, that is used to validate this model and that will contain the results of the validations */
				RegressionValidation* validation;
				
				/** returns a const pointer to the matrix containing the coefficients obtained by Model.train() */
				const Eigen::MatrixXd* getTrainingResult() const; 
				
				virtual void saveToFile(string filename);
				
				virtual void readFromFile(string filename);
				
				void show();
				//@}
				
				
			protected:
				/** @name Attributes
				 */
				//@{
				/** Eigen::MatrixXd containing the coefficients obtained by Model.train().\n
				raining_result will have the following dimensions for the different types of models, with m=no of descriptors and c=no of modelled activities (=no of columns of Model.Y) : \n
				LinearModel : mxc \n
				KernelModel : nxc \n
				ALLModel : mxc \n
				FitModel : mxc \n
				SVMModel : m x (c*no of classes) \n
				SVRModel : m x (c*no of classes)    */
				Eigen::MatrixXd training_result_;
				
				// RowVector holding the regression constants (one value for each feature)
				Eigen::RowVectorXd offsets_;
				//@}
				
				
				/** @name Input and Output. The following methods can be used to implement the functions saveToFile() and readFromFile() in final classes derived from this base-class 
				 */
				//@{
				virtual void calculateOffsets() = 0;
				
				void readDescriptorInformationFromFile(std::ifstream& in, int no_descriptors, bool transformation, int no_coefficients);
				
				void saveDescriptorInformationToFile(std::ofstream& out);
				//@}

				friend class RegressionValidation;
				friend class Rescoring;

		};
	}
}

#endif // BALL_QSAR_REGRESSIONMODEL_H
