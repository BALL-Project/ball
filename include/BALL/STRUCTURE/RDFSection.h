// $Id: RDFSection.h,v 1.7 2001/06/21 08:58:41 amoll Exp $

#ifndef BALL_STRUCTURE_RDFSECTION_H
#define BALL_STRUCTURE_RDFSECTION_H

#ifndef BALL_STRUCTURE_RADIALDISTRIBUTIONFUNCTION_H
#include <BALL/STRUCTURE/radialDistributionFunction.h>
#endif

#ifndef BALL_FORMAT_PARAMETERSECTION_H
#include <BALL/FORMAT/parameterSection.h>
#endif

namespace BALL
{

	/** Helper class for RDFParameter.
			{\bf Definition:} \URL{BALL/STRUCTURE/RDFSection.h}
	 */
	class RDFSection
		:	public ParameterSection
	{
		public:

		BALL_CREATE(RDFSection)

		/** @name Enums 
		*/
		//@{

		enum Type
		{
			UNKNOWN_TYPE = 0,
			PIECEWISE_POLYNOMIAL = 1
		};

		//@}
		/** @name Constructors and destructors 
		*/
		//@{

		/** Default constructor 
		*/
		RDFSection() throw();

		/** Copy constructor 
		*/
		RDFSection(const RDFSection& rdf_section) throw();

		/** Destructor 
		*/
		virtual ~RDFSection() throw();

		//@}
		/** @name Assignment 
		*/
		//@{

		/** */
		const RDFSection& operator = (const RDFSection& rdf_section) throw();

		/** Clear method 
		*/
		virtual void clear() throw();

		//@}
		/** @name Parameter extraction 
		*/
		//@{

		/** */
		virtual bool extractSection(Parameters& parameters,	const String& section_name) 
			throw();
		
		//@}
		/** @name Accessors 
		*/
		//@{

		/** return the RDF created by extractSection() 
		*/
		const RadialDistributionFunction& getRDF() const throw();

		//@}
		/** @name Predicates 
		*/
		//@{

		/** Equality operator 
		*/
		bool operator == (const RDFSection& section) const throw();

		//@}

		protected:

		/*_ The RDF that is to be read from a section */
		RadialDistributionFunction rdf_;
	
	};
} // namespace BALL


#endif //  BALL_STRUCTURE_RDFSECTION_H
