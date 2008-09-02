// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// 

#ifndef CLASSIFICATION
#define CLASSIFICATION

#ifndef MODELH
#include <BALL/QSAR/Model.h>
#endif

#ifndef CLASVALIDATION
#include <BALL/QSAR/classificationValidation.h>
#endif

#ifndef QSAR_EXCEPTION
#include <BALL/QSAR/exception.h>
#endif


namespace BALL
{
	namespace QSAR
	{
		class ClassificationModel : public Model
		{
			public:
				/** @name Constructors and Destructors
				 */
				//@{
				/** constructur,
				@param q QSAR-wrapper object, from which the data for this model should be taken */
				ClassificationModel(const QSARData& q);
				
				~ClassificationModel();
				//@}
				
				
				/** @name Attributes
				 */
				//@{
				/** a ClassificationValidation object, that is used to validate this model and that will contain the results of the validations */
				ClassificationValidation* validation;
				//@}
				
				
			protected:
	
				/** @name Accessors
				 */
				//@{
				/** read all different class labels from matrix Y into ClassificationModel.labels */
				void readLabels();
				
				/** the number of substances in each class of the training data */
				vector<int> no_substances_;
				//@}
				
				/** @name Input and Output. The following methods can be used to implement the functions saveToFile() and readFromFile() in final classes derived from this base-class 
				 */
				//@{
				void readClassInformationFromFile(ifstream& input, int no_classes);
				void saveClassInformationToFile(ofstream& out);
				//@}
				
				
				
				/** @name Feature discretization function
				 */
				//@{
				/** Interface function-definition for discretization of features.\n
				Thus, various functions that do the actual discretization can be created as members of ClassificationModel, allowing switching of the discretization technique by only (re-)setting this function-pointer.
				@param bins the number of discrete values to be created for each features 
				@param discretization_information a Matrix into which the information about the applied discretization should be saved (into one column per feature). \n
				This Matrix can later be used to discretize test data according to the discretization of the training data. */
				void (ClassificationModel::* discretizeFeatures)(int bins, Matrix& discretization_information);
				
				/** Discretizes test data according to the given discretization of the training data.
				@param discretization_information the discretization according to which the current descriptor_matrix should be discretized. */
				void (ClassificationModel::* discretizeTestDataFeatures)(RowVector& compound, int bins, const Matrix& discretization_information);
				
				
				/** discretizes each feature in such a way that each created bin has an equal range of values */
				void equalSpaceDiscretization(int bins, Matrix& discretization_information);
				
				void equalSpaceDiscretizationTestData(RowVector& compound, int bins, const Matrix& discretization_information);
				
				
				
				
				//@}
				
	
				/** @name Attributes
				 */
				//@{
				/** labels for the different classes */
				vector<int> labels_;
				//@}
				
				friend class ClassificationValidation;
				
				
				
		};
	}
}

#endif // CLASSIFICATION
