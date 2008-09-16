// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// 

#ifndef REGRESSION
#define REGRESSION

#ifndef MODELH
#include <BALL/QSAR/Model.h>
#endif

#ifndef REGVALIDATION
#include <BALL/QSAR/regressionValidation.h>
#endif

#include <fstream>

namespace BALL
{
	namespace QSAR
	{
		class RegressionModel : public Model
		{
			public:
				/** @name Constructors and Destructors
				 */
				//@{
				/** constructur,
				@param q QSAR-wrapper object, from which the data for this model should be taken */
				RegressionModel(const QSARData& q);
				
				~RegressionModel();
				//@}
				
				
				/** @name Accessors
				 */
				//@{
				/** a ModelValidation object, that is used to validate this model and that will contain the results of the validations */
				RegressionValidation* validation;
				
				/** returns a const pointer to the matrix containing the coefficients obtained by Model.train() */
				const BALL::Matrix<double>* getTrainingResult() const; 
				
				virtual void saveToFile(string filename);
				
				virtual void readFromFile(string filename);
				
				void show();
				//@}
				
				
			protected:
				/** @name Attributes
				 */
				//@{
				/** BALL::Matrix<double> containing the coefficients obtained by Model.train().\n
				raining_result will have the following dimensions for the different types of models, with m=no of descriptors and c=no of modelled activities (=no of columns of Model.Y) : \n
				LinearModel : mxc \n
				KernelModel : nxc \n
				ALLModel : mxc \n
				FitModel : mxc \n
				SVMModel : m x (c*no of classes) \n
				SVRModel : m x (c*no of classes)    */
				BALL::Matrix<double> training_result_;
				
				// RowVector holding the regression constants (one value for each feature)
				Vector<double> offsets_;
				//@}
				
				
				/** @name Input and Output. The following methods can be used to implement the functions saveToFile() and readFromFile() in final classes derived from this base-class 
				 */
				//@{
				virtual void calculateOffsets() = 0;
				
				void readDescriptorInformationFromFile(ifstream& in, int no_descriptors, bool transformation, int no_coefficients);
				
				void saveDescriptorInformationToFile(ofstream& out);
				//@}
				
				
				friend class RegressionValidation;
				
		};
	}
}

#endif // REGRESSION
