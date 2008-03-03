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


namespace BALL
{
	namespace QSAR
	{	
		class PLSModel : public LinearModel
		{
			public:
				/** @name Constructors and Destructors
				 */
				//@{
				/** constructur
				@param q QSAR-wrapper object, from which the data for this model should be taken */
				PLSModel(const QSARData& q);

				~PLSModel();
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

				/** returns a pointer to the  PLS score matrix T */
				const Matrix* getT();

				/** returns a pointer to the  PLS X-weights matrix W */
				const Matrix* getW();
				
				/** returns a pointer to the  PLS components-weights matrix C */
				const Matrix* getC();
				
				/** returns a pointer to the  PLS Y-scores matrix U */
				const Matrix* getU();
				
				void setParameters(vector<double>& v);
				
				vector<double> getParameters() const;
				//@}
		
				
			protected:
				/** @name Attributes
				 */
				//@{
				Matrix T_;

				Matrix W_;
				
				Matrix U_;

				Matrix C_;
	
				int no_components_;
				//@}
		};
	}
}


#endif // PLSMODEL
