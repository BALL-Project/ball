// $Id: solventParameter.h,v 1.2 2000/09/02 14:35:06 oliver Exp $

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
		{\bf Definition:} \URL{BALL/.../SolventParameter.h}
	 */

	class SolventParameter
		:	public ParameterSection
	{

		public:

		// BALL_CREATE(SolventParameter)

		/** @name Constructors and destructors. */
		//@{

		/** Default constructor */
		SolventParameter();

		/** Detailed constructor */
		SolventParameter(Parameters& parameters);

		/** */
		SolventParameter(const SolventParameter& param);

		/** Destructor */
		virtual ~SolventParameter();

		/** */
		virtual void destroy();

		/** */
		virtual void clear();

		//@}


		/** @name Assignment */
		//@{

		/** */
		void set(const SolventParameter& param);

		/** */
		const SolventParameter& operator = (const SolventParameter& param);

		//@}


		/** @name Accessors */
		//@{

		/** Return a SolventDescriptor with the values from the parameter file */
		SolventDescriptor getSolventDescriptor() const;

		//@}

		/** */
		virtual bool extractSection(ForceFieldParameters& parameters, const String&
				section_name);

		protected:

		String name_;
		float number_density_;
		std::vector<SolventAtomDescriptor> solvent_atoms_;
		SolventDescriptor solvent_descriptor_;

	};
} // namespace BALL

#endif // BALL_SOLVATION_SOLVENTPARAMETER_H
