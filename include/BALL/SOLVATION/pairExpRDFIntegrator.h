// $Id: pairExpRDFIntegrator.h,v 1.2 2000/09/01 10:23:55 anker Exp $

#ifndef BALL_SOLVATION_PAIREXPRDFINTEGRATOR_H
#define BALL_SOLVATION_PAIREXPRDFINTEGRATOR_H

#ifndef BALL_COMMON_H
#include <BALL/common.h>
#endif

#ifndef BALL_STRUCTURE_RDFINTEGRATOR_H
#include <BALL/STRUCTURE/RDFIntegrator.h>
#endif

#ifndef BALL_MATHS_PIECEWISEFUNCTION_H
#include <BALL/STRUCTURE/piecewiseFunction.h>
#endif

namespace BALL
{
	/** */

	class PairExpRDFIntegrator
		:	public RDFIntegrator
	{

		public:

		// BALL_CREATE(PairExpRDFIntegrator)

		/** @name Constructors and destructors */
		//@{

		/** Default constructor */
		PairExpRDFIntegrator();

		/** Copy constructor */
		PairExpRDFIntegrator(const PairExpRDFIntegrator& integrator);

		/** Detailed constructor */
		PairExpRDFIntegrator(double alpha, double C1, double C2, double R_ij_o,
				double k1, double k2, const RadialDistributionFunction& rdf);

		/** Destructor */
		virtual ~PairExpRDFIntegrator();

		/** */
		virtual void destroy();

		/** */
		virtual void clear();

		//@}

		
		/** @name Assignment */
		//@{

		/** */
		void set(const PairExpRDFIntegrator& integrator);

		/** */
		const PairExpRDFIntegrator& operator = 
			(const PairExpRDFIntegrator& integrator);

		//@}


		/** @name Accessors */
		//@{

		/** */
		void setConstants(double alpha, double C1, double C2, double R_ij_o,
				double k1, double k2);

		/** */
		void setRDF(const RadialDistributionFunction& rdf);

		/** */
		double integrateToInf(double from) const;

		/** */
		double integrateToInf(double from, double alpha, double C1, double C2,
				double R_ij_o, double k1, double k2);

		/** */
		double integrate(double from, double to) const;

		/** */
		double integrate(double from, double to, double alpha, double C1, 
				double C2, double R_ij_o, double k1, double k2);
		
		/** */
		virtual double operator () (double x) const;

		//@}


		protected:

		double alpha_;
		double C1_;
		double C2_;
		double R_ij_o_;
		double k1_;
		double k2_;
		RadialDistributionFunction rdf_;

		bool valid_;


		private:

		double numericallyIntegrateInterval(Interval interval,
				Coefficients coeffs, Position index) const;

	};

} // namespace BALL

#endif //  BALL_SOLVATION_PAIREXPRDFINTEGRATOR_H
