/* linearModel.h
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

#ifndef LMODEL
#define LMODEL

#ifndef REGRESSION
#include <BALL/QSAR/regressionModel.h>
#endif

namespace BALL
{
	namespace QSAR
	{
		class LinearModel : public RegressionModel
		{
		
			public: 
				/** @name Constructors and Destructors
				 */
				//@{
				/** constructur,
				@param q QSAR-wrapper object, from which the data for this model should be taken */
				LinearModel(const QSARData& q);
		
				~LinearModel();
				
				virtual void operator=(const Model& m);
				
				//@}
				
				
				/** @name Accessors
				 */
				//@{
				virtual Vector<double> predict(const vector<double>& substance, bool transform=1);
	
			protected:
				void calculateOffsets();
				//@}
		};
	}
}

#endif // LMODEL
