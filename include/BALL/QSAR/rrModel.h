/* rrModel.h
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

#ifndef RIDGEMODEL
#define RIDGEMODEL

#ifndef MLRMODEL
#include <BALL/QSAR/mlrModel.h>
#endif

#ifndef QSAR_EXCEPTION
#include <BALL/QSAR/exception.h>
#endif



namespace BALL
{
	namespace QSAR
	{
		class BALL_EXPORT RRModel : public MLRModel
		{
			public:
				/** @name Constructors and Destructors
				 */
				//@{
				/** constructur
				@param q QSARData object, from which the data for this model should be taken */
				RRModel(const QSARData& q, double lambda=0.003);

				~RRModel();
				//@}
				
				/** @name Accessors
				 */
				//@{
				/** Starts multiple linear regression with the current data and saves the resulting linear combination of descriptors to training_result. \n */
				void train();
				
				void setParameters(vector<double>& v);
				
				vector<double> getParameters() const;
				//@}
		
			private:
				/** @name Attributes
				 */
				//@{
				double lambda_;
				
				//@}
		};
	}
}

#endif // RIDGEMODEL

