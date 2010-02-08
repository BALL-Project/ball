/* ldaModel.h
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

#ifndef LDAMODEL
#define LDAMODEL

#ifndef CLASSIFICATION
#include <BALL/QSAR/classificationModel.h>
#endif

#ifndef STATISTICS
#include <BALL/QSAR/statistics.h>
#endif

namespace BALL{
	namespace QSAR {

		/** class for support vector classification */
		class BALL_EXPORT LDAModel : public ClassificationModel
		{ 
			public:
				/** @name Constructors and Destructors
				 */
				//@{
				LDAModel(const QSARData& q);

				~LDAModel();
				//@}
				
				
				/** @name Accessors
				 */
				//@{
				void train();
				
				Vector<double> predict(const vector<double>& substance, bool transform=1);

				void setParameters(vector<double>& v);
				
				vector<double> getParameters() const;
				
				void saveToFile(string filename);
				
				void readFromFile(string filename);
				//@}
				
				
			private:
				/** @name Attributes
				 */
				//@{
				/** covariance matrix of descriptors */
				Matrix<double> sigma_;
				

				double lambda_;				

				/** vector containing one matrix for each modelled activity. Each matrix contains a mean vector of a class in each line */
				vector<Matrix<double> > mean_vectors_;
				//@}
		};
	}
}

#endif //LDAMODEL
