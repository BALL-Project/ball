// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pair6_12InteractionEnergyProcessor.h,v 1.22 2005/12/23 17:01:59 amoll Exp $
//

#ifndef BALL_SOLVATION_PAIR6_12INTERACTIONENERGYPROCESSOR_H
#define BALL_SOLVATION_PAIR6_12INTERACTIONENERGYPROCESSOR_H

#ifndef BALL_COMMON_H
# include <BALL/common.h>
#endif

#ifndef BALL_DATATYPE_OPTIONS_H
# include <BALL/DATATYPE/options.h>
#endif

#ifndef BALL_MATHS_SURFACE_H
# include <BALL/MATHS/surface.h>
#endif

#ifndef BALL_ENERGY_ENERGYPROCESSOR_H
# include <BALL/ENERGY/energyProcessor.h>
#endif

#ifndef BALL_STRUCTURE_RDFPARAMETER_H
# include <BALL/STRUCTURE/RDFParameter.h>
#endif

#ifndef BALL_SOLVATION_PAIR6_12RDFINTEGRATOR_H
# include <BALL/SOLVATION/pair6_12RDFIntegrator.h>
#endif

#ifndef BALL_SOLVATION_SOLVENTDESCRIPTOR_H
# include <BALL/SOLVATION/solventDescriptor.h>
#endif

namespace BALL
{
	/** Processor for the computation of the van-der-Waals interaction energy
			of a molecule with its surrounding.
			This processor uses a 6_12 pair potential for the calculation of
			dispersion and repulsion energies.  \par
			Energies are computed in units of kJ/mol.  \par
\ingroup Solvation
	 */
	
	class BALL_EXPORT Pair6_12InteractionEnergyProcessor
		:	public EnergyProcessor
	{

		public:

		// ?????: Doku.
		enum SurfaceType
		{
			SURFACE__UNKNOWN = 0,
			SURFACE__SAS = 1,
			SURFACE__SES = 2,
			SURFACE__EXTERNAL = 3
		};

		/** Symbolic names for option keys.
				This struct contains a symbolic name for each recognized key in
				Pair6_12InteractionEnergyProcessor::options.
		*/
		struct BALL_EXPORT Option
		{
			
			/** The verbosity level.
					Use integer values with this option.
					@see Default::VERBOSITY
					@param verbosity integer
			 */
			static const char* VERBOSITY;

			/** RDF option.
					This option states whether the RDF should be considered during the
					integrtion or not. Use bool values with this option.
					@see Default::USE_RDF
					@param verbosity integer
			 */
			static const char* USE_RDF;

			/** RDF file option.
					This options sets the name of the file containing the RDF
					information. Use char* values with this option.
					@see Default::RDF_FILENAME
					@param rdf_file_name char*
			 */
			static const char* RDF_FILENAME;

			/** Solvent description file option.
					This option sets the name of the file containig the solvent
					description. Use char* values with this option.
					@see Default::SOLVENT_FILENAME;
					@param solvent_file_name char*
			 */
			static const char* SOLVENT_FILENAME;

			/** Lennard Jones parameter file option.
					This option sets the name of the file containig the Lennard-Jones
					parameters. Use char* values with this option.
					@see Default::LJ_FILENAME;
					@param lennard_jones_file_name char*
			 */
			static const char* LJ_FILENAME;

			/** The number density of the solvent.
					This option defines the number density of the surrounding solvent. 
					Use	float values of unit $ A^{-3} $ with this option.
					@see Default::SOLVENT_NUMBER_DENSITY
					@param solvent_number_density float
			 */
			static const char* SOLVENT_NUMBER_DENSITY;

			/** The type of surface to be used.
					@see Default::SURFACE_TYPE
					@param surface_type int the type of the surface
			 */
			static const char* SURFACE_TYPE;

			/** The name of the file containing a surface definition.
					@see Default::SURFACE_FILENAME
					@param surface_filename char*
			 */
			static const char* SURFACE_FILENAME;

		};

		/** Default values for interaction energy calculations.
				These values represent the default settings for the calculations of the
				interaction energy.
		 */
		struct BALL_EXPORT Default
		{
			/** Default verbosity level.
					@see Option::VERBOSITY
			 */
			static const Size VERBOSITY;

			/** Default RDF setting.
					We use RDF information for the calculation of the interaction	energy by default.
					@see Option::USE_RDF
			 */
			static const bool USE_RDF;
			static const char* RDF_FILENAME;
			static const char* SOLVENT_FILENAME;
			static const char* LJ_FILENAME;

			/** Default number density.
					This default value is the number density of water at 300 K and
					standard pressure (3.33253e-2 $ A^{-3}$).
					@see Option::SOLVENT_NUMBER_DENSITY;
			 */
			static const float SOLVENT_NUMBER_DENSITY;

			/** 
					@see Option::SURFACE_TYPE
			 */
			static const Size SURFACE_TYPE;

			/** 
					@see Option::SURFACE_FILENAME
			 */
			static const char* SURFACE_FILENAME;

		};

		/** @name Constructors and destructors 
		*/
		//@{

		/** Default constructor 
		*/
		Pair6_12InteractionEnergyProcessor() 
			;

		/** Copy constructor 
				@param proc the processor to copy
		*/
		Pair6_12InteractionEnergyProcessor
			(const Pair6_12InteractionEnergyProcessor& proc) 
			;

		/** Detailed constructor
				@param solvent a description of the surrounding solvent (@see	SolventDescriptor)
				@param rdf_param the parameters for the radial distribution	function (@see RDFParameter)
				@param rdf_integrator the integrator needed for structural 
							 integration (@see Pair6_12RDFIntegrator)
		*/
		Pair6_12InteractionEnergyProcessor(const SolventDescriptor& solvent, 
				const RDFParameter& rdf_param,
				const Pair6_12RDFIntegrator& rdf_integrator) 
			;

		/** Destructor 
		*/
		virtual ~Pair6_12InteractionEnergyProcessor() ;

		//@}
		/** @name Accessors 
		*/
		//@{

		/** Set the solvent descriptor.
				@param solvent the solvent descriptor to be set
		*/
		void setSolventDescriptor(const SolventDescriptor& solvent) ;

		/** Get the solvent descriptor.
				@return the current solvent descriptor of this instance
		*/
		const SolventDescriptor& getSolventDescriptor() const ;

		/** Set the parameters for the radial distribution function 
				@param rdf_parameter the RDF parameter to be set
		*/
		void setRDFParameters(const RDFParameter& rdf_parameter) ;

		/** Get the parameters for the radial distribution function 
				@return a const reference of the current RDF parameter
		*/
		const RDFParameter& getRDFParameter() const ;

		/** Set the integrator of this processor
				@param integrator the integrator to be set
		*/
		void setRDFIntegrator(const Pair6_12RDFIntegrator& integrator) ;

		/** Get the integrator of this processor
				@return the current integrator 
		*/
		const Pair6_12RDFIntegrator& getRDFIntegrator() const ;

		//@}
		/** @name Assignment 
		*/
		//@{

		/** Assignment operator 
		*/
		const Pair6_12InteractionEnergyProcessor& operator =
			(const Pair6_12InteractionEnergyProcessor& proc) ;

		/** Clear function 
		*/
		virtual void clear() ;
		
		//@}
		/** @name Processor functions 
		*/
		//@{

		/// @throws BALL::Exception::DivisionByZero
		virtual bool finish();

		//@}
		/** @name Options 
		*/
		//@{

		/** Options for the calculation of the caviation free energy 
		*/
		Options options;

		//@}
		/** @name Predicates 
		*/
		//@{

		/** Equality operator 
		*/
		bool operator == (const Pair6_12InteractionEnergyProcessor& proc) 
			const ;

		//@}

		protected:

		/*_ solvent description, to be read from an INIFile 
		*/
		SolventDescriptor solvent_;

		/*_ the rdf description, also from an INIFile 
		*/
		RDFParameter rdf_parameter_;

		/*_ This is the tool to perform integrations with embedded RDf information 
		*/
		Pair6_12RDFIntegrator rdf_integrator_;


		private:

		void getExternalSurface_(std::vector<std::pair<Vector3, Surface> >& surface_map, 
				const char* surface_file) ;
	};
   
} // namespace BALL

#endif // BALL__PAIR6_12INTERACTIONENERGYPROCESSOR_H
