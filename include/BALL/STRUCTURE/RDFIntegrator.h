// $Id: RDFIntegrator.h,v 1.6 2000/10/30 00:19:26 amoll Exp $

#ifndef BALL_STRUCTURE_RDFINTEGRATOR_H
#define BALL_STRUCTURE_RDFINTEGRATOR_H

#ifndef BALL_STRUCTURE_RADIALDISTRIBUTIONFUNCTION_H
#include <BALL/STRUCTURE/radialDistributionFunction.h>
#endif

namespace BALL
{
	/** Integrator class for radial distribution functions.
		This class provides the integration functionality needed for the
		computation of interaction energies.
		\\
		{\bf Definition:} \URL{BALL/.../RDFIntegrator.h}
	 */

	class RDFIntegrator
	{
		public:

		BALL_CREATE(RDFIntegrator)

		/** @name Constructors and destructors */
		//@{

		/** Default constructor */
		RDFIntegrator();

		/** */
		RDFIntegrator(const RDFIntegrator& integrator);

		/** */
		RDFIntegrator(const RadialDistributionFunction& rdf);

		/** Destructor */
		virtual ~RDFIntegrator();

		/** */
		virtual void destroy();

		/** */
		virtual void clear();

		//@}


		/** @name Assignment */
		//@{

		/** */
		void set(const RDFIntegrator& integrator);

		/** */
		const RDFIntegrator& operator = (const RDFIntegrator& integrator);

		//@}

		/** */
		void setRDF(const RadialDistributionFunction& rdf);

		/** */
		const RadialDistributionFunction& getRDF() const;

		/** compute the value of the integral at a given x */
		virtual double operator () (double x) const;


		protected:

		RadialDistributionFunction rdf_;

	};
}
#endif // BALL_STRUCTURE_RDFINTEGRATOR_H
