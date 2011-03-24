/* oplsModel.h
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

#ifndef OPLSMODEL
#define OPLSMODEL

#ifndef PLSMODEL
#include <BALL/QSAR/plsModel.h>
#endif



namespace BALL
{
	namespace QSAR
	{
		class BALL_EXPORT OPLSModel : public PLSModel
		{
			public:
				/** @name Constructors and Destructors
				 */
				//@{
				OPLSModel(const QSARData& q);

				~OPLSModel();
				EIGEN_MAKE_ALIGNED_OPERATOR_NEW
				//@}
				
				
				/** @name Accessors
				 */
				//@{Constructors and Destructors
				/** Starts orthogonal partial least squares regression. \n
				In order to find the optimal number of latente variables for the current data of this model, run findNoLatenteVariables() first. */
				void train();
				
				const Eigen::MatrixXd* getWOrtho();
				
				const Eigen::MatrixXd* getTOrtho();
				
				int getNoOrthoComponents();
				
				void setNoOrthoComponents(int d);
				
				bool optimizeParameters(int k, int no_steps);
				//RowVector predict(const vector<double>& substance, bool transform);
				
				void setParameters(vector<double>& v);
				
				vector<double> getParameters() const;
				//@}
				
				
			private:
				/** @name Attributes
				 */
				//@{
				Eigen::MatrixXd T_ortho_;
				Eigen::MatrixXd W_ortho_;
				
				int no_ortho_components_;
				//@}
		};
	}
}


#endif // OPLSMODEL
