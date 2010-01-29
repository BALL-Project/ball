/* knnModel.h
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

#ifndef KNNMODEL
#define KNNMODEL

#ifndef ALLMODEL
#include <BALL/QSAR/allModel.h>
#endif

namespace BALL
{
	namespace QSAR 
	{

		/** class for automated lazy learning (ALL-QSAR) */
		class BALL_EXPORT KNNModel : public ALLModel
		{ 
			public:
				/** @name Constructors and Destructors
				 */
				//@{
				KNNModel(const QSARData& q, int k=3);
				//@}
				
				void calculateWeights(Matrix<double>& dist, Vector<double>& w);
				
				void setParameters(vector<double>& v);
				
				vector<double> getParameters() const;
				
				bool optimizeParameters(int k, int no_steps);
				
			private:
				int k_ ;
		};
	}
}

#endif // KNNMODEL
