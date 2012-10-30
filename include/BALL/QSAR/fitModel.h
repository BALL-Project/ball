// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//

#ifndef BALL_QSAR_FITMODEL
#define BALL_QSAR_FITMODEL

#ifndef BALL_CONFIG_CONFIG_H
# include <BALL/CONFIG/config.h>
#endif

#ifdef BALL_HAS_GSL

#ifndef BALL_QSAR_NONLINEARMODEL_H
# include <BALL/QSAR/nonlinearModel.h>
#endif

#include <gsl/gsl_multifit_nlin.h>
#include <gsl/gsl_deriv.h>
#include <gsl/gsl_math.h>

#ifndef BALL_MATHS_PARSEDFUNCTION_H
#  include <BALL/MATHS/parsedFunction.h>
#endif

//#include <list>

namespace BALL
{
	namespace QSAR
	{
		class BALL_EXPORT FitModel : public NonLinearModel
		{
			public:
				/** @name Constructors and Destructors
				 */
				//@{
				FitModel(const QSARData& q);
				
				FitModel(const QSARData& q, vector<String>& eq, vector<double>& guess);
				
				FitModel(const QSARData& q, vector<String>& eq, vector<vector<String> >& deq, vector<double>& guess);

				//~FitModel();
				//@}
				
				
				/** @name Accessors
				 */
				//@{
				/** sets all equations and their derivatives 
				@param eq vector containing one equation for each column of Y
				@param deq derivatives of each equation in one vector */
				void setEquations(vector<String>& eq, vector<vector<String> >& deq);
				
				/** sets all equations but no derivatives. Since no derivatives are given, numerical derivation will be used during training. */
				void setEquations(vector<String>& eq);
			
				/** starts training the FitModel using Levenberg-Marquardt */		
				void train();
				
				Eigen::VectorXd predict(const vector<double>& substance, bool transform=1);
				
				void setInitialGuess(vector<double>& d);
				//@}
				
				
			private:
				/** @name Attributes
				 */
				//@{
				/** saves one equations for the prediction of each activitiy, i.e. one String for each column of Y */
				vector<String> allEquations_;
				
				/** saves all derivatives of one equation in one vector */
				vector<vector<String> > allDiffEquations_;
				
				/** saves the approximated coefficients for each equation, i.e. one String for each column of Y */
				//vector<String> allCoefficients;
				
				vector<double> initial_guess_;
				//@}

		};
	

				
		const Eigen::MatrixXd* fitX;
		Eigen::MatrixXd* fitY;
		String* equation;
		vector<String>* diffEquations;
		ParsedFunction<double>* f;
		unsigned int c; // current column
		
		double getFunctionValue(double X, void* params);
				
		int setF(const gsl_vector* x, void* params, gsl_vector* f);
			
		int setDf(const gsl_vector * x, void* params, gsl_matrix * df);
		
		int setFdf (const gsl_vector * x, void *params, gsl_vector * f, gsl_matrix * df);
		
		gsl_multifit_function_fdf make_fdf (int (* f) (const gsl_vector *, void *, gsl_vector *),
				int (* df) (const gsl_vector *, void *, gsl_matrix *),
				int (* fdf) (const gsl_vector *, void *, gsl_vector *, gsl_matrix *),
				size_t n,
				size_t p, void * params );
		
	}
}

#endif //BALL_HAS_GSL

#endif // BALL_QSAR_FITMODEL
