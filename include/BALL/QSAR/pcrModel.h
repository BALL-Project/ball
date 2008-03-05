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
#include <newmatap.h>
#include <newmatio.h>


namespace BALL 
{
	namespace QSAR
	{
		class PCRModel : public LinearModel
		{
			public:

				/** @name Constructors and Destructors
				 */
				//@{
				/** constructur
				@param q QSAR-wrapper object, from which the data for this model should be taken
				@param frac_var the part of the variance that is to be explained by the latent variables; as many latent variables as necessary to achieve this are created */
				PCRModel(const QSARData& q, double frac_var=0.99, bool k=0);

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
				//@}

				
				
			protected:
				
				/** fraction of the variance that is to be explained */
				double frac_var_; 
				
				/** @name Attributes
				*/
				//@{
				bool kernel_;	
				//@}

		};
	}
}

#endif // PCRMODEL
