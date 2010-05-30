// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_STRUCTURE_RDFINTEGRATOR_H
#define BALL_STRUCTURE_RDFINTEGRATOR_H

#ifndef BALL_STRUCTURE_RADIALDISTRIBUTIONFUNCTION_H
# include <BALL/STRUCTURE/radialDistributionFunction.h>
#endif

namespace BALL
{
	/** Integrator class for radial distribution functions.
			This class provides the integration functionality needed for the
			computation of interaction energies.	 \par
	\ingroup RDF
  */
	class BALL_EXPORT RDFIntegrator
	{
		public:

		BALL_CREATE(RDFIntegrator)

		/** @name Constructors and destructors 
		*/
		//@{

		/** Default constructor 
		*/
		RDFIntegrator()
			;

		/** Copy constructor.
				@param integrator the integrator to copy from
		*/
		RDFIntegrator(const RDFIntegrator& integrator)
			;

		/** Detailed constructor.
				@param rdf the radial distribution function to initialize <b>  this </b> with
		*/
		RDFIntegrator(const RadialDistributionFunction& rdf)
			;

		/** Destructor 
		*/
		virtual ~RDFIntegrator()
			;

		//@}
		/** @name Assignment 
		*/
		//@{

		/** Assignment operator.
				@param integrator the integrator to assign from
				@return a constant reference to <b>  this </b>
		*/
		const RDFIntegrator& operator = (const RDFIntegrator& integrator)
			;

		/** Clear function.
		*/
		virtual void clear()
			;

		//@}
		/// Predicates
		//@{

		/** Equality operator
				@param integrator the integrator which we want to compare to {\em	this}
				@return true, if the instances are equal, false ow.
		*/
		bool operator == (const RDFIntegrator& integrator) const
			;

		/** Validity test. Return the valid flag.
				@return true, if this instance is valid.
		*/
		virtual bool isValid() const 
			;

		//@}
		/// Accessors
		//@{

		/** Set the radial distribution function of this instance.
				@param rdf a radial distribution function
				@see RadialDistributionFunction
		*/
		void setRDF(const RadialDistributionFunction& rdf)
			;

		/** Get the radial distribution function of this instance.
				@return a const reference to the radial distribution function
		*/
		const RadialDistributionFunction& getRDF() const
			;

		/** Compute the value of the integral at a given x. This function
				should not be called, because it is merely a sort of prototype
				defining interface for derived classes. It always returns 0.0.
				@param x the lower limit of the integration
				@return always 0.0
		*/
		virtual double operator () (double x) const
			;

		//@}

		protected:

		//_ The radial distribution function which is to be integrated.
		RadialDistributionFunction rdf_;

		/*_ A flag indicating validity of this instance. A sole RDFIntegrator
				instance cannot be valid, because it does not have any information
				about the potential for which it should be the integrating means.
				This flag is provided for derived classes.
		*/
		bool valid_;

	};
   
}
#endif // BALL_STRUCTURE_RDFINTEGRATOR_H
