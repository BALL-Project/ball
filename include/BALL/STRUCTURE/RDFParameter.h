// $Id: RDFParameter.h,v 1.1 2000/08/31 13:43:06 anker Exp $

#ifndef BALL__RDFPARAMETER_H
#define BALL__RDFPARAMETER_H

#ifndef BALL_KERNEL_ATOM_H
#include <BALL/KERNEL/atom.h>
#endif

#ifndef BALL_STRUCTURE_RADIALDISTRIBUTIONFUNCTION_H
#include <BALL/STRUCTURE/radialDistributionFunction.h>
#endif

#ifndef BALL_FORMAT_PARAMETERSECTION_H
#include <BALL/FORMAT/parameterSection.h>
#endif

#ifndef BALL_MOLMEC_PARAMETERS_FORCEFIELDPARAMETERS_H
#include <BALL/MOLMEC/PARAMETER/forceFieldParameters.h>
#endif

namespace BALL 
{

	/** Parametersection for radial distribution function
		BAUSTELLE
		{\bf Definition:} \URL{BALL/.../RDFParameter.h}
	 */
	
	class RDFParameter
		:	public ParameterSection
	{

		public:

		// BALL_CREATE_NODEEP(RDFParameter)

		/** @name Constructors and destructors. */
		//@{

		/** Default constructor */
		RDFParameter();

		/** Copy constructor */
		RDFParameter(const RDFParameter& rdf_parameter);

		/** Destructor */
		virtual ~RDFParameter();

		/** Destroy method */
		virtual void destroy();

		/** Clear method */
		virtual void clear();

		//@}


		/** @name Assignment */
		//@{

		/** */
		void set(const RDFParameter& rdf_parameter);

		/** */
		const RDFParameter& operator = (const RDFParameter& rdf_parameter);

		//@}


		/** @name Accessors */
		//@{

		/** */
		Position getIndex(Atom::Type type_i, Atom::Type type_j) const;

		/** Return a radial distribution function determined by type */
		const RadialDistributionFunction& getRDF(Atom::Type type_i, 
				Atom::Type type_j) const;

		/** Return a radial distribution function determined by index */
		const RadialDistributionFunction& getRDF(Position index) const;

		//@}

		/** */
		virtual bool extractSection(ForceFieldParameters& parameters,
				const String& section_name);

		/** @name Predicates */
		//@{

		/** */
		bool hasRDF(Atom::Type solvent_atom_type,
				Atom::Type solute_atom_type) const;

		/** @see hasRDF */
		bool hasParameters(Atom::Type solvent_atom_type,
				Atom::Type solute_atom_type) const;

		//@}

		protected:

		std::vector<RadialDistributionFunction> rdf_list_;
		HashMap< Atom::Type, HashMap<Atom::Type, Position> > rdf_indices_;


	};

} // namespace BALL
#endif // BALL__RDFPARAMETER_H
