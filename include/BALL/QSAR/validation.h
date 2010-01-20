/* validation.h
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

#ifndef VALIDATION
#define VALIDATION

#ifndef QSARDATA
#include <BALL/QSAR/QSARData.h>
#endif

#include <gsl/gsl_randist.h>
#include <gsl/gsl_cdf.h>
#include <iterator>


namespace BALL
{	
	namespace QSAR
	{
		class Model;
		
		/** interface class for validation of QSAR-models */
		class Validation
		{
			public:
				/** @name Constructors and Destructors
				 */
				//@{	
				Validation(Model* m);

				virtual ~Validation();
				//@}
				
				
				/** @name Accessors
				 */
				//@{
				/** Starts cross-validation with k steps. \n
				Data is taken from QSARData.descriptor_matrix and is in each step divided into training- and test-data. \n
				(Data having already been copied into Model.descriptor_matrix will be deleted) */
				virtual void crossValidation(int k, bool restore=1) = 0;
				
				/** fetches the result of cross-validation */
				virtual double getCVRes() = 0;
				
				/** fetches the quality of fit to the input data, as calculated by testInputData() */
				virtual double getFitRes() = 0;
		
				/** set the result of cross-validation to the given value */
				virtual void setCVRes(double d) = 0;
				
				/** return the result of the previous response permutation test .\n
				The return matrix is empty if no such test has been run yet */	
				const Matrix<double>& getYRandResults() const;
				
				/** Fetches input data from QSARData and tests the current (unchanged) model with all these new substances (without cross-validation!). \n
				@param transform if transform==1, the test data is transformed in the same way that the training data was transformed before predicting activities. \n
				If training and test substances are taken from the same input file, set transform to 0 */
				virtual void testInputData(bool transform=0) = 0;
				
				virtual void bootstrap(int k,  bool restore=1) = 0;
				
				virtual const Matrix<double>& yRandomizationTest(int runs, int k) = 0;
				
				/** select the desired statistic to be used for validating the models
				@param s if (s==1) R^2 and Q^2 are used \n
				if(s==2) F_regr and F_cv are used. */
				virtual void selectStat(int s) = 0;
				
				/** return the ID of selected validation statistic */
				int getStat() const;
				
				/** save the result of the applied validation methods to a file */
				virtual void saveToFile(string filename) const = 0;
				
				/** restore validation-results from a file */
				virtual void readFromFile(string filename) = 0;
				//@}
				
				
			protected:
				
				/** @name Accessors
				 */
				//@{
				/** copies one substance (=one line from class QSARData) and its activity into the training data set */
				void setTrainingLine(int train_line, int current_line);
			
				/** copies one substance (=one line from class QSARData) and its activity into the test data set 
				@param back_transform descriptor values are transformed back to in order to un-do centering */
				void setTestLine(int test_line, int current_line, bool back_transform=0);
				
				/** randomizes all columns of Model.Y_ */
				void yRand();
				//@}
				
				
				/** @name Attributes
				 */
				//@{
				/** contains all substances whose activities are to be predicted by the next call of testAllSubstances() */ 
				vector<vector<double> > test_substances_;
				
				/** contains the experimentally determined results (activities) for all current test-substances in one column for each activity */
				Matrix<double> test_Y_;
				
				/** contains the names of all current test-substances */
				vector<string> substances_names_;
				
				
				/** pointer to the model, which the object of this class should test */
				Model* model_;
				
				/** the selected validation statistic */
				int validation_statistic_;
				
				Matrix<double> yRand_results_;
				//@}
			
		};
	}
}



#endif // VALIDATION
