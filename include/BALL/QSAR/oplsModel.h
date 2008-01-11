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
		class OPLSModel : public PLSModel
		{
			public:
				/** @name Constructors and Destructors
				 */
				//@{
				OPLSModel(const QSARData& q);

				~OPLSModel();
				//@}
				
				
				/** @name Accessors
				 */
				//@{Constructors and Destructors
				/** Starts orthogonal partial least squares regression. \n
				In order to find the optimal number of latente variables for the current data of this model, run findNoLatenteVariables() first. */
				void train();
				
				const Matrix* getWOrtho();
				
				const Matrix* getTOrtho();
				
				int getNoOrthoComponents();
				
				void setNoOrthoComponents(int d);
				
				void optimizeParameters(int k, unsigned int max=20);
				//RowVector predict(const vector<double>& substance, bool transform);
				
				void setParameters(vector<double>& v);
				
				vector<double> getParameters() const;
				//@}
				
				
			private:
				/** @name Attributes
				 */
				//@{
				Matrix T_ortho_;
				Matrix W_ortho_;
				
				int no_ortho_components_;
				//@}
		};
	}
}


#endif // OPLSMODEL
