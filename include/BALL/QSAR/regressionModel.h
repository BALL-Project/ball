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
				const Matrix* getTrainingResult() const; 
				
				virtual void saveToFile(string filename);
				
				virtual void readFromFile(string filename);
				
				void show();
				//@}
				
				
			protected:
				/** @name Attributes
				 */
				//@{
				/** Matrix containing the coefficients obtained by Model.train().\n
				raining_result will have the following dimensions for the different types of models, with m=no of descriptors and c=no of modelled activities (=no of columns of Model.Y) : \n
				LinearModel : mxc \n
				KernelModel : nxc \n
				ALLModel : mxc \n
				FitModel : mxc \n
				SVMModel : m x (c*no of classes) \n
				SVRModel : m x (c*no of classes)    */
				Matrix training_result_;
				//@}
				
				
				friend class RegressionValidation;
				
		};
	}
}

#endif // REGRESSION
