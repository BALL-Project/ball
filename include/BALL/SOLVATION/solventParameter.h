// $Id: solventParameter.h,v 1.3 2000/10/17 17:14:35 anker Exp $

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
		class (@see SolventDescriptor). 
		\\
		{\bf Definition:} \URL{BALL/.../SolventParameter.h}
	 */

	class SolventParameter
		:	public ParameterSection
	{

		public:

		BALL_CREATE(SolventParameter)

		/** @name Constructors and destructors. */
		//@{

		/** Default constructor */
		SolventParameter() throw();

		/** Detailed constructor */
		SolventParameter(Parameters& parameters) throw();

		/** Copy constructor */
		SolventParameter(const SolventParameter& param) throw();

		/** Destructor */
		virtual ~SolventParameter() throw();

		//@}


		/** @name Assignment */
		//@{

		/** Assignment operator */
		const SolventParameter& operator = (const SolventParameter& param)
		throw();

		/** Clear method */
		virtual void clear() throw();

		//@}


		/** @name Accessors */
		//@{

		/** Return a SolventDescriptor with the values from the parameter file */
		SolventDescriptor getSolventDescriptor() const throw();

		//@}


		/** @name Predicates */
		//@{

		/** Equality operator */
		bool operator == (const SolventParameter& param) const throw();
		//@}


		/** */
		virtual bool extractSection(ForceFieldParameters& parameters, const String&
				section_name) throw();


		protected:

		/*_ The name of the solvent description freom the INI file */
		String name_;
		/*_ The density of the solvent description freom the INI file */
		float number_density_;
		/*_ The atoms of the solvent description freom the INI file */
		std::vector<SolventAtomDescriptor> solvent_atoms_;
		/*_ The solvent descriptor to be created */
		SolventDescriptor solvent_descriptor_;

	};
} // namespace BALL

#endif // BALL_SOLVATION_SOLVENTPARAMETER_H
