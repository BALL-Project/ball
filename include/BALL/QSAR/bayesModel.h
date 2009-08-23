/* bayesModel.h
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


#ifndef BAYESMODEL
#define BAYESMODEL

#include <BALL/QSAR/classificationModel.h>


namespace BALL
{
	namespace QSAR 
	{
		/** interface class for all Bayes classification models */
		class BayesModel : public ClassificationModel
		{ 
			public: 
				
				BayesModel(const QSARData& q);
				
				virtual bool isTrained() = 0;
				
				/** calculate the probability for the specified feature to assume the given value for each class
				@return a probability for each class */
				virtual vector<double> calculateProbabilities(int activitiy_index, int feature_index, double feature_value) = 0;
				
				/** returns the number of response variables for which this model has been trained */
				virtual int getNoResponseVariables() = 0;
			
				
			protected:
				/** the minimal desired difference between the predicted class and the second best class. If the difference is smaller than this value, undef_act_class_id_ should be used instead.*/
				double min_prob_diff_;
				
				/** the class_id that is to be assigned, if no class could be savely predicted, if difference between best and second best potential class is smaller than min_prob_diff_ */
				double undef_act_class_id_;
	
		};
	}
}

#endif //BAYESMODEL
