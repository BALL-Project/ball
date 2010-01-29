/* classificationValidation.h
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

#ifndef CLASVALIDATION
#define CLASVALIDATION

#ifndef QSARDATA
#include <BALL/QSAR/QSARData.h>
#endif

#ifndef VALIDATION
#include <BALL/QSAR/validation.h>
#endif

#include <gsl/gsl_randist.h>
#include <gsl/gsl_cdf.h>
#include <iterator>


namespace BALL
{	
	namespace QSAR
	{
		class ClassificationModel;
		/** class for validation of QSAR regression models */
		class BALL_EXPORT ClassificationValidation : public Validation
		{
			public:
				/** @name Constructors and Destructors
				 */
				//@{
				/** constructor
				@param m pointer to the regression model, which the object of this class should test */
				ClassificationValidation(ClassificationModel* m);
				//@}
				
				
				/** @name Accessors
				 */
				//@{
				void crossValidation(int k, bool restore=1);
				
				double getCVRes();
				
				double getFitRes();
				
				void setCVRes(double d);
				
				void testInputData(bool transform=0);
				
				/** return pointer to the matrix containing the number of TP, FP, TN, FN in one column for each class  */
				const BALL::Matrix<double>* getConfusionMatrix();
				
				/** returns a RowVector holding the one value contituting the validation result for each class if "average accuracy" or "average MCC" is chosen (see selectStat()). */
				const BALL::Vector<double>* getClassResults();
				
				/** starts bootstrapping with k samples \n
				@param k no of bootstrap samples */
				void bootstrap(int k, bool restore=1);
				
				/** Y randomization test \n
				Randomizes all columns of model.Y, trains the model, runs crossValidation and testInputData and saves the resulting accuracy_input_test and accuracy_cv value to a vector, where BALL::Matrix<double>(i,0)=accuracy_input_test, BALL::Matrix<double>(i,1)=accuracy_cv \n
				@param runs this is repeated as often as specified by 'runs' */
				const BALL::Matrix<double>& yRandomizationTest(int runs, int k);
				
				/** get average accuracy value as determined after cross validation */
				double getAccuracyCV();
				
				/** get average accuracy value as determined after testing of input data(); */
				double getAccuracyInputTest();
				
				void selectStat(int s);
				
				void saveToFile(string filename) const;
				
				void saveToFile(string filename, const double& quality_input_test, const double& predictive_quality) const;
				
				void readFromFile(string filename);
				
				//@}
				
				
			private:
				/** @name Accessors
				 */
				//@{
				/** Tests the current model with all substances in the (unchanged) test data set */
				void testAllSubstances(bool transform);
				
				/** calculate average accuracy with the current values of TP, FP, FN, TN in matrix ClassificationValidation.predictions. */
				void calculateAverageSensitivity();
				
				/** calculate weighted average accuracy of all classes. Weighted by the number of training compounds within each class */
				void calculateWeightedSensitivity();
				
				/** calculate accuracy for all classes at once */
				void calculateOverallAccuracy();
				
				/** calculate one MCC for each class and use the average */
				void calculateAverageMCC();
				
				/** calculate MCC for all classes at once */
				void calculateOverallMCC();
				
				/** calculate the True Discovery Rate (only applicable to binary classification validation results). */
				void calculateTDR();
				//@}
				
				
				/** @name Attributes
				 */
				//@{
				/** matrix containing the number of TP, FP, FN, TN in one column for each class  */
				BALL::Matrix<double> confusion_matrix_;
				
				/** RowVector holding the one value contituting the validation result for each class if "average sensitivity" or "average MCC" is chosen (see selectStat()). */
				Vector<double> class_results_;
			
				double quality_;
				
				double quality_input_test_;
				
				double quality_cv_;

				/** pointer to the regression model, which the object of this class should test */
				ClassificationModel* clas_model;
				
				void (ClassificationValidation::* qualCalculation)();
				
				
				//@}
	
		};
	}
}



#endif // REGVALIDATION
