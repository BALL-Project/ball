/* kpcrModel.h
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

#ifndef KPCRMODEL
#define KPCRMODEL

#ifndef KMODEL
#include <BALL/QSAR/kernelModel.h>
#endif

#ifndef PCRMODEL
#include <BALL/QSAR/pcrModel.h>
#endif

#ifndef NLMODEL
#include <BALL/QSAR/nonlinearModel.h>
#endif

namespace BALL
{
	namespace QSAR
	{
		/** class for kernel principal component regression */
		class BALL_EXPORT KPCRModel : public KernelModel, public LatentVariableModel
		{
			public:
				/** @name Constructors and Destructors
				 */
				//@{
				KPCRModel(const QSARData& q, int k_type, double p1, double p2=-1);
				
				KPCRModel(const QSARData& q, Eigen::VectorXd& w);
				
				KPCRModel(const QSARData& q, String s1, String s2);
				
				KPCRModel(const QSARData& q, const LinearModel& lm, int column);

				~KPCRModel();
				//@}
				
				
				/** @name Accessors
				 */
				//@{
				/** set the fraction of the variance that should be explained by the latente variables. */
				void setFracVar(double frac_var);
				
				void train();
				
				void setParameters(vector<double>& v);
				
				vector<double> getParameters() const;
				//@}
				
				
			private:
				
				double frac_var_;
		};
	}
}

#endif //KPCRMODEL
