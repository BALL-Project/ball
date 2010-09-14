/* kernelModel.h
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

#ifndef KMODEL
#define KMODEL

#ifndef NLMODEL
#include <BALL/QSAR/nonlinearModel.h>
#endif

#ifndef LMODEL
#include <BALL/QSAR/linearModel.h>
#endif

#ifndef KERNEL
#include <BALL/QSAR/kernel.h>
#endif



namespace BALL
{
	namespace QSAR
	{
		class BALL_EXPORT KernelModel : public NonLinearModel
		{
			public:
				/** @name Constructors and Destructors
				 */
				//@{
				KernelModel(const QSARData& q, int k_type, double p1, double p2);
				
				KernelModel(const QSARData& q, String f, String g);
						
				KernelModel(const QSARData& q, Vector<double>& w);
				
				KernelModel(const QSARData& q, const LinearModel& lm, int column);
				
				~KernelModel();
				
				virtual void saveToFile(string filename);
				
				virtual void readFromFile(string filename);
				
				virtual Vector<double> predict(const vector<double>& substance, bool transform);
				
				void operator=(const Model& m);
				//@}
				
				/** @name Attributes
				 */
				//@{
				Kernel* kernel;
				//@}

				
			protected:
				
				/** @name Attributes
				 */
				//@{
				/** Matrix containing the pairwise distances between all substances */
				Matrix<double> K_;
				
				/** resulting matrix with one column for each modeled activity and with one coefficient for each substance (i.e. one column for each column for Model.Y) */
				//Matrix B;
				//@}
				
				
				/** @name Input and Output. The following methods can be used to implement the functions saveToFile() and readFromFile() in final classes derived from this base-class 
				 */
				//@{
				void calculateOffsets();
				
				void readKernelParametersFromFile(std::ifstream& in);
				void saveKernelParametersToFile(std::ofstream& out);
				void saveTrainingResult(std::ofstream& out);
				void readTrainingResult(std::ifstream& input, int no_substances, int no_y);
				
				friend class RegressionValidation;
				//}@
				
			
		};
	}
}


#endif // NLMODEL
