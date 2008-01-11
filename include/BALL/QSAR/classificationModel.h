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
