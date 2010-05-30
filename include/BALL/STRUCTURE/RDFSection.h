// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_STRUCTURE_RDFSECTION_H
#define BALL_STRUCTURE_RDFSECTION_H

#ifndef BALL_STRUCTURE_RADIALDISTRIBUTIONFUNCTION_H
# include <BALL/STRUCTURE/radialDistributionFunction.h>
#endif

#ifndef BALL_FORMAT_PARAMETERSECTION_H
# include <BALL/FORMAT/parameterSection.h>
#endif

namespace BALL
{
	/** Helper class for RDFParameter.
	\ingroup RDF
	 */
	class BALL_EXPORT RDFSection
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
		RDFSection() ;

		/** Copy constructor 
		*/
		RDFSection(const RDFSection& rdf_section) ;

		/** Destructor 
		*/
		virtual ~RDFSection() ;

		//@}
		/** @name Assignment 
		*/
		//@{

		/** */
		const RDFSection& operator = (const RDFSection& rdf_section) ;

		/** Clear method 
		*/
		virtual void clear() ;

		//@}
		/** @name Parameter extraction 
		*/
		//@{

		/** */
		virtual bool extractSection(Parameters& parameters,	const String& section_name) 
			;
		
		//@}
		/** @name Accessors 
		*/
		//@{

		/** return the RDF created by extractSection() 
		*/
		const RadialDistributionFunction& getRDF() const ;

		//@}
		/** @name Predicates 
		*/
		//@{

		/** Equality operator 
		*/
		bool operator == (const RDFSection& section) const ;

		//@}

		protected:

		/*_ The RDF that is to be read from a section */
		RadialDistributionFunction rdf_;
	
	};
   
} // namespace BALL


#endif //  BALL_STRUCTURE_RDFSECTION_H
