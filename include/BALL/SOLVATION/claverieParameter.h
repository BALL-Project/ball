// $Id: claverieParameter.h,v 1.2 2000/09/28 13:37:17 anker Exp $

#ifndef BALL_SOLVATION_CLAVERIEPARAMETER_H
#define BALL_SOLVATION_CLAVERIEPARAMETER_H

#ifndef BALL_FORMAT_PARAMETERS_H
#include <BALL/FORMAT/parameters.h>
#endif

#ifndef BALL_FORMAT_PARAMETERSECTION_H
#include <BALL/FORMAT/parameterSection.h>
#endif

#ifndef BALL_MOLMEC_PARAMETER_FORCEFIELDPARAMETERS_H
#include <BALL/MOLMEC/PARAMETER/forceFieldParameters.h>
#endif

namespace BALL
{

	/** 
		{\bf Definition:} \URL{BALL/.../claverieParameter.h}
	 */

	class ClaverieParameter
		:	public ParameterSection
	{

		public:

		BALL_CREATE(ClaverieParameter)

		/** @name Constructors and destructors. */
		//@{

		/** Default constructor */
		ClaverieParameter();

		/** Detailes constructor */
		ClaverieParameter(Parameters& parameters);

		/** */
		ClaverieParameter(const ClaverieParameter& param);

		/** Destructor */
		virtual ~ClaverieParameter();

		/** */
		virtual void destroy();

		/** */
		virtual void clear();

		//@}


		/** @name Assignment */
		//@{

		/** */
		void set(const ClaverieParameter& param);

		/** */
		const ClaverieParameter& operator = (const ClaverieParameter& param);

		//@}


		/** @name Accessors */
		//@{

		/** */
		bool hasParameters(Atom::Type solvent_type, Atom::Type solute_type) const;

		/** */
		std::pair<float, float> getParameters(Atom::Type solvent_type,
				Atom::Type solute_type) const;

		/** */
		std::pair<float, float> getParameters(Atom::Type type) const;

		//@}


		/** */
		virtual bool extractSection(ForceFieldParameters& parameters,
				const String& section_name);


		protected:

		std::vector< std::pair<float, float> > parameters_;
		HashMap<Atom::Type, Index> indices_;


	};
} // namespace BALL

#endif // BALL_SOLVATION_CLAVERIEPARAMETER_H
