/* logitModel.h
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

#ifndef LOGITMODEL
#define LOGITMODEL

#ifndef CLASSIFICATION
#include <BALL/QSAR/classificationModel.h>
#endif



namespace BALL{
	namespace QSAR {

		/** class for logistic regression */
		class BALL_EXPORT LogitModel : public ClassificationModel
		{ 
			public:
				/** @name Constructors and Destructors
				 */
				//@{
				LogitModel(const QSARData& q);

				~LogitModel();
				//@}
				
				
				/** @name Accessors
				 */
				//@{
				void train();
				
				Vector<double> predict(const vector<double>& substance, bool transform);
				
				void saveToFile(string filename);
				
				void readFromFile(string filename);
				//@}
				
				
			protected:	
				/** @name Attributes
				 */
				//@{
				Matrix<double> training_result_;
				//@}
				
		};
	}
}

#endif // LOGITMODEL
