/* kplsModel.h
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

#ifndef KPLSMODEL
#define KPLSMODEL

#ifndef KMODEL
#include <BALL/QSAR/kernelModel.h>
#endif

#ifndef NLMODEL
#include <BALL/QSAR/nonlinearModel.h>
#endif

#include <BALL/QSAR/latentVariableModel.h>

namespace BALL
{
	namespace QSAR
	{
		/** kernel partial-least-squares class */
		class BALL_EXPORT KPLSModel : public KernelModel, public LatentVariableModel
		{
			public:
				/** @name Constructors and Destructors
				 */
				//@{
				KPLSModel(const QSARData& q, int k_type, double p1, double p2=-1);
				
				KPLSModel(const QSARData& q, Eigen::VectorXd& w);
				
				KPLSModel(const QSARData& q, String s1, String s2);
				
				KPLSModel(const QSARData& q, const LinearModel& lm, int column);

				~KPLSModel();

				EIGEN_MAKE_ALIGNED_OPERATOR_NEW
				//@}
				
				
				/** @name Accessors
				 */
				//@{
				
				void train();
				
				/** tries to find the optimal number of latente variables (=PLS components) */
				bool optimizeParameters(int k, int no_steps);
				
				/** returns the number of latente variables (=PLS components) */
				int getNoComponents();
				
				void setNoComponents(int d);
				
				/** returns a pointer to the  PLS Y-scores matrix U */
				const Eigen::MatrixXd* getU();
				
				void setParameters(vector<double>& v);
				
				vector<double> getParameters() const;
				//@}
				
				
			protected:
				/** @name Attributes
				 */
				Eigen::MatrixXd U_;
				
				int no_components_;
				//@}
				
		};
	}
}

#endif // KPLSMODEL
