/* libsvmModel.h
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

#ifndef LIBSVMMODELH
#define LIBSVMMODELH

#ifndef SVRMODEL
#include <BALL/QSAR/svrModel.h>
#endif

#include <fstream>
#include <svm.h>


namespace BALL
{
	namespace QSAR
	{
		class LibsvmModel : public SVRModel
		{
			public:
				/** @name Constructors and Destructors
				 */
				//@{
				LibsvmModel(const QSARData& q, int k_type, double p1, double p2=-1);

				~LibsvmModel();
				//@}
				
				
				/** @name Accessors
				 */
				//@{				
				void train();
				
				//RowVector predict(const vector<double>& substance, bool transform=1);
				
				void setParameters(vector<double>& v);
				
				vector<double> getParameters() const;
				//@}
				
				
			private:
				
				struct svm_problem* createProblem(int response_id);
				
				void createParameters();
				
				struct svm_model* svm_train_result_;
				
				struct svm_parameter parameters_;
				
				struct svm_node* x_space_;
				
				/** determines whether nu-SVR is to be used; else eps-SVR is applied */
				bool use_nu_;
				
				/** determines whether the libsvm shrinking heuristics is to be used */
				bool use_shrinking_;
				
				double nu_;
				double p_;
				double eps_;
				double C_;
				
				
		};
	}
}



#endif // LIBSVMMODELH
