// $Id: RDFParameter.h,v 1.6 2001/02/23 02:40:44 amoll Exp $

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
		{\bf Definition:} \URL{BALL/STRUCTURE/RDFParameter.h}
	 */
	
	class RDFParameter
		:	public ParameterSection
	{
		public:

		BALL_CREATE(RDFParameter)

		/** @name Constructors and destructors. 
		*/
		//@{

		/** Default constructor 
		*/
		RDFParameter() 
			throw();

		/** Copy constructor 
				@param rdf_parameter the RDFParameter instance to copy from
		*/
		RDFParameter(const RDFParameter& rdf_parameter) 
			throw();

		/** Destructor 
		*/
		virtual ~RDFParameter() 
			throw();

		//@}
		/** @name Assignment 
		*/
		//@{

		/** Assignment operator 
				@param rdf_parameter the parameter to assign from
				@return a const reference to {\em this}
		*/
		const RDFParameter& operator = (const RDFParameter& rdf_parameter)
			throw();

		/** Clear method 
		*/
		virtual void clear() 
			throw();

		//@}
		/** @name Accessors 
		*/
		//@{

		/** Get the index in dependance of atom types of solute and solvent
				@param type_i the type of the solvent atom
				@param type_j the type of the solute atom
				@return the index of the respective RDF in the list built from the
								RDFSections.
		*/
		Position getIndex(Atom::Type type_i, Atom::Type type_j) const 
			throw();

		/** Return a radial distribution function determined by type
				@param type_i the type of the solvent atom
				@param type_j the type of the solute atom
				@return the RDF
		*/
		const RadialDistributionFunction& getRDF(Atom::Type type_i,
				Atom::Type type_j) const 
			throw();

		/** Return a radial distribution function determined by index 
				@param index the index of the radial distribution function in the
							 internal list
				@return the specified RDF
		*/
		const RadialDistributionFunction& getRDF(Position index) const 
			throw();

		//@}

		/** Extract the information from the parameter file.
				@param parameters a ForceFieldParameters instance
				@param saection_name the name of the section to be parsed
				@return true if the section could be read, false otherwise
		*/
		virtual bool extractSection(ForceFieldParameters& parameters,
				const String& section_name) 
			throw();

		/** @name Predicates 
		*/
		//@{

		/** Find out, whether the parameter file contained a RDF for a special
				solute/solvent atom combination.
				@param solvent_atom_type the type of the solvent atom
				@param solute_atom_type the type of the solute atom
				@return true, if there was a definition for this combination of
								atom types
		*/
		bool hasRDF(Atom::Type solvent_atom_type,	Atom::Type solute_atom_type) 
			const throw();

		/** hasParameters
				@see hasRDF 
		*/
		bool hasParameters(Atom::Type solvent_atom_type,
				Atom::Type solute_atom_type) const 
			throw();

		//@}

		protected:

		/*_ The vector containing all the read RDF representations */
		std::vector<RadialDistributionFunction> rdf_list_;

		/*_ This maps Atom:Types to Indices of rdf_list_ */
		HashMap< Atom::Type, HashMap<Atom::Type, Position> > rdf_indices_;


	};

} // namespace BALL
#endif // BALL__RDFPARAMETER_H
