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
		class BALL_EXPORT PCRModel : public LinearModel, public LatentVariableModel
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
				static void calculateEigenvectors(const Eigen::MatrixXd& data, double frac_var, Eigen::MatrixXd& output);
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
