// $Id: RDFParameter.h,v 1.3 2000/10/17 17:15:13 anker Exp $

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

		BALL_CREATE(RDFParameter)

		/** @name Constructors and destructors. */
		//@{

		/** Default constructor */
		RDFParameter() throw();

		/** Copy constructor */
		RDFParameter(const RDFParameter& rdf_parameter) throw();

		/** Destructor */
		virtual ~RDFParameter() throw();

		//@}


		/** @name Assignment */
		//@{

		/** Assignment operator */
		const RDFParameter& operator = (const RDFParameter& rdf_parameter)
		throw();

		/** Clear method */
		virtual void clear() throw();

		//@}


		/** @name Accessors */
		//@{

		/** */
		Position getIndex(Atom::Type type_i, Atom::Type type_j) const throw();

		/** Return a radial distribution function determined by type */
		const RadialDistributionFunction& getRDF(Atom::Type type_i, 
				Atom::Type type_j) const throw();

		/** Return a radial distribution function determined by index */
		const RadialDistributionFunction& getRDF(Position index) const throw();

		//@}

		/** */
		virtual bool extractSection(ForceFieldParameters& parameters,
				const String& section_name) throw();

		/** @name Predicates */
		//@{

		/** */
		bool hasRDF(Atom::Type solvent_atom_type,
				Atom::Type solute_atom_type) const throw();

		/** @see hasRDF */
		bool hasParameters(Atom::Type solvent_atom_type,
				Atom::Type solute_atom_type) const throw();

		//@}

		protected:

		/*_ The vector containing all the read RDF representations */
		std::vector<RadialDistributionFunction> rdf_list_;

		/*_ This maps Atom:Types to Indices of rdf_list_ */
		HashMap< Atom::Type, HashMap<Atom::Type, Position> > rdf_indices_;


	};

} // namespace BALL
#endif // BALL__RDFPARAMETER_H
