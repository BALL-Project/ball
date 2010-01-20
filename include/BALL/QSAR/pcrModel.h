/* pcrModel.h
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

#ifndef PCRMODEL
#define PCRMODEL

#ifndef LMODEL
#include <BALL/QSAR/linearModel.h>
#endif

#ifndef QSAR_EXCEPTION
#include <BALL/QSAR/exception.h>
#endif

#include <BALL/QSAR/rrModel.h>

#include <BALL/QSAR/latentVariableModel.h>


namespace BALL 
{
	namespace QSAR
	{
		class PCRModel : public LinearModel, public LatentVariableModel
		{
			public:

				/** @name Constructors and Destructors
				 */
				//@{
				/** constructur
				@param q QSAR-wrapper object, from which the data for this model should be taken
				@param frac_var the part of the variance that is to be explained by the latent variables; as many latent variables as necessary to achieve this are created */
				PCRModel(const QSARData& q, double frac_var=0.99);

				~PCRModel();
				//@}
				
				/** @name Accessors
				 */
				//@{
				/** Starts principal component regression with the current data and saves the resulting linear combination of descriptors to training_result.*/
				void train();
				
				
				/** set the fraction of the variance that should be explained by the latente variables. */
				void setFracVar(double frac_var);
				
				void setParameters(vector<double>& v);
				
				vector<double> getParameters() const;
				
				/** calculates the first eigenvectors of the given matrix 'data' and saves them as columns of matrix 'output' 
				@param frac_var the fraction of the variance that is to the covered the selected eigenvectors */
				static void calculateEigenvectors(const Matrix<double>& data, double frac_var, Matrix<double>& output);
				//@}

				
				
			protected:
				
				/** @name Attributes
				*/
				//@{
				/** fraction of the variance that is to be explained */
				double frac_var_; 
				//@}

		};
	}
}

#endif // PCRMODEL
