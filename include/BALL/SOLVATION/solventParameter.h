// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: solventParameter.h,v 1.10 2003/02/21 16:06:25 anhi Exp $

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
			<b>Definition:</b> BALL/SOLVATION/solventParameter.h
	 */
	class SolventParameter
		:	public ParameterSection
	{

		public:

		BALL_CREATE(SolventParameter)

		/** @name Constructors and destructors. 
		*/
		//@{

		/** Default constructor 
		*/
		SolventParameter() throw();

		//SolventParameter(Parameters& parameters) throw();

		/** Copy constructor 
		*/
		SolventParameter(const SolventParameter& param) throw();

		/** Destructor */
		virtual ~SolventParameter() throw();

		//@}
		/** @name Assignment 
		*/
		//@{

		/** Assignment operator 
		*/
		const SolventParameter& operator = (const SolventParameter& param)
			throw();

		/** Clear method 
		*/
		virtual void clear() throw();

		//@}
		/** @name Accessors 
		*/
		//@{

		/** Return a SolventDescriptor with the values from the parameter file 
		*/
		const SolventDescriptor& getSolventDescriptor() const throw();

		/** Return a SolventDescriptor with the values from the parameter file.
		 * 	Muteable version.
		 */
		SolventDescriptor& getSolventDescriptor() throw();

                /** Return the name of the SolventDescriptor 
		*/
		const String& getSolventName() const throw();


                /** Return the solvents density 
		*/
		const float& getSolventDensity() const throw();

                 /** Return the SolventDescription 
		*/
		const std::vector<SolventAtomDescriptor>& getSolventDescription() const throw();

		//@}
		/** @name Predicates 
		*/
		//@{

		/** Equality operator 
		*/
		bool operator == (const SolventParameter& param) const throw();

		//@}

		/** 
		*/
		virtual bool extractSection(ForceFieldParameters& parameters, const String&
				section_name) throw();


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

	};
} // namespace BALL

#endif // BALL_SOLVATION_SOLVENTPARAMETER_H
