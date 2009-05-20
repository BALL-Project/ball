// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: solventParameter.h,v 1.17 2005/12/23 17:02:00 amoll Exp $
//

#ifndef BALL_SOLVATION_SOLVENTPARAMETER_H
#define BALL_SOLVATION_SOLVENTPARAMETER_H

#ifndef BALL_SOLVATION_SOLVENTDESCRIPTOR_H
#	include <BALL/SOLVATION/solventDescriptor.h>
#endif

#ifndef BALL_FORMAT_PARAMETERS_H
#	include <BALL/FORMAT/parameters.h>
#endif

#ifndef BALL_FORMAT_PARAMETERSECTION_H
#	include <BALL/FORMAT/parameterSection.h>
#endif

#ifndef BALL_MOLMEC_PARAMETER_FORCEFIELDPARAMETERS_H
#	include <BALL/MOLMEC/PARAMETER/forceFieldParameters.h>
#endif

namespace BALL
{
	/** Parameter section for the solvent description.
			This class provides parameter file interface for the SolventDescriptor
			class ( \link SolventDescriptor SolventDescriptor \endlink ).   \par
	\ingroup Solvation
	 */
	class BALL_EXPORT SolventParameter
		:	public ParameterSection
	{

		public:

		BALL_CREATE(SolventParameter)

		/** @name Constructors and destructors. 
		*/
		//@{

		/** Default constructor 
		*/
		SolventParameter();

		//SolventParameter(Parameters& parameters);

		/** Copy constructor 
		*/
		SolventParameter(const SolventParameter& param);

		/** 
		*/
		SolventParameter(const AtomTypes& atom_types);

		/** Destructor */
		virtual ~SolventParameter();

		//@}
		/** @name Assignment 
		*/
		//@{

		/** Assignment operator 
		*/
		const SolventParameter& operator = (const SolventParameter& param);

		/** Clear method 
		*/
		virtual void clear();

		//@}
		/** @name Accessors 
		*/
		//@{

		/** Return a SolventDescriptor with the values from the parameter file 
		*/
		const SolventDescriptor& getSolventDescriptor() const;

		/** Return a SolventDescriptor with the values from the parameter file.
		 * 	Muteable version.
		 */
		SolventDescriptor& getSolventDescriptor();

                /** Return the name of the SolventDescriptor 
		*/
		const String& getSolventName() const;


                /** Return the solvents density 
		*/
		const float& getSolventDensity() const;

                 /** Return the SolventDescription 
		*/
		const std::vector<SolventAtomDescriptor>& getSolventDescription() const;

		//@}
		/** @name Predicates 
		*/
		//@{

		/** Equality operator 
		*/
		bool operator == (const SolventParameter& param) const;

		//@}

		/** 
		*/
		virtual bool extractSection(Parameters& parameters, const String& section_name);


		protected:

		/*_ The name of the solvent description freom the INI file 
		*/
		String name_;

		/*_ The density of the solvent description freom the INI file 
		*/
		float number_density_;

		/*_ The atoms of the solvent description freom the INI file 
		*/
		std::vector<SolventAtomDescriptor> solvent_atoms_;

		/*_ The solvent descriptor to be created 
		*/
		SolventDescriptor solvent_descriptor_;

		/*_
		*/
		AtomTypes atom_types_;

	}; 
} // namespace BALL

#endif // BALL_SOLVATION_SOLVENTPARAMETER_H
