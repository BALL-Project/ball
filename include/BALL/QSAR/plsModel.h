/* plsModel.h
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

#ifndef PLSMODEL
#define PLSMODEL

#ifndef LMODEL
#include <BALL/QSAR/linearModel.h>
#endif

#ifndef STATISTICS
#include <BALL/QSAR/statistics.h>
#endif

#ifndef QSAR_EXCEPTION
#include <BALL/QSAR/exception.h>
#endif

#include <BALL/QSAR/latentVariableModel.h>


namespace BALL
{
	namespace QSAR
	{	
		class BALL_EXPORT PLSModel : public LinearModel, public LatentVariableModel
		{
			public:
				/** @name Constructors and Destructors
				 */
				//@{
				/** constructur
				@param q QSAR-wrapper object, from which the data for this model should be taken */
				PLSModel(const QSARData& q);

				~PLSModel();

				EIGEN_MAKE_ALIGNED_OPERATOR_NEW
				//@}
				
				
				/** @name Accessors
				 */
				//@{
				/** Starts partial least squares regression. \n
				In order to find the optimal number of latente variables for the current data of this model, run findNoLatenteVariables() first. */
				virtual void train();
				
				/** Tries to find the optimal number of PLS components (latente variables) for the current data of this model */
				virtual bool optimizeParameters(int k, int no_steps);

				/** set the number of PLS components to create */
				void setNoComponents(int no);
				
				/** get the number of PLS components */
				int getNoComponents();
				
				/** returns a pointer to the  PLS Y-scores matrix U */
				const Eigen::MatrixXd* getU();
				
				void setParameters(vector<double>& v);
				
				vector<double> getParameters() const;
				//@}
		
				
			protected:
				/** @name Attributes
				 */
				//@{
				
				Eigen::MatrixXd U_;
	
				int no_components_;
				//@}
		};
	}
}


#endif // PLSMODEL
