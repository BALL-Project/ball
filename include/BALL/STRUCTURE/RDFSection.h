// $Id: RDFSection.h,v 1.2 2000/08/31 21:15:46 oliver Exp $

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

	class RDFSection
		:	public ParameterSection
	{

		public:


		BALL_CREATE(RDFSection)

		/** @name Enums */
		//@{

		enum Type
		{
			UNKNOWN_TYPE = 0,
			PIECEWISE_POLYNOMIAL = 1
		};

		//@}

		/** @name Constructors and destructors */
		//@{

		/** Default constructor */
		RDFSection();

		/** Copy constructor */
		RDFSection(const RDFSection& rdf_section);

		/** Destructor */
		virtual ~RDFSection();

		/** Destroy method */
		virtual void destroy();

		/** Clear method */
		virtual void clear();

		//@}


		/** @name Assignment */
		//@{

		/** */
		void set(const RDFSection& rdf_section);

		/** */
		const RDFSection& operator = (const RDFSection& rdf_section);

		//@}


		/** */
		virtual bool extractSection(Parameters& parameters,
				const String& section_name);
		
		/** */
		RadialDistributionFunction getRDF() const;


		protected:

		RadialDistributionFunction rdf_;

	
	};
} // namespace BALL


#endif //  BALL_STRUCTURE_RDFSECTION_H
