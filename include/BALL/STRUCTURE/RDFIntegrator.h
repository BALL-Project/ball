// $Id: RDFIntegrator.h,v 1.3 2000/09/01 05:56:28 oliver Exp $

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
		:	public RadialDistributionFunction
	{

		public:


		BALL_CREATE(RDFIntegrator)

		/** @name Constructors and destructors */
		//@{

		/** Default constructor */
		RDFIntegrator();

		/** */
		RDFIntegrator(const RDFIntegrator& integrator);

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


		/** compute the value of the integral at a given x */
		virtual double operator () (double x) const;

	};
}
#endif // BALL_STRUCTURE_RDFINTEGRATOR_H
