// $Id: pair6_12InteractionEnergyProcessor.h,v 1.5 2000/11/06 17:56:55 anker Exp $

// BAUSTELLE: Pfad?
#ifndef BALL_SOLVATION_PAIR6_12INTERACTIONENERGYPROCESSOR_H
#define BALL_SOLVATION_PAIR6_12INTERACTIONENERGYPROCESSOR_H

#ifndef BALL_COMMON_H
#include <BALL/common.h>
#endif

#ifndef BALL_DATATYPE_OPTIONS_H
#include <BALL/DATATYPE/options.h>
#endif

#ifndef BALL_MATHS_SURFACE_H
#include <BALL/MATHS/surface.h>
#endif

// BAUSTELLE: Pfad?
#ifndef BALL_ENERGY_ENERGYPROCESSOR_H
#include <BALL/ENERGY/energyProcessor.h>
#endif

#ifndef BALL_STRUCTURE_RDFPARAMETER_H
#include <BALL/STRUCTURE/RDFParameter.h>
#endif

#ifndef BALL_SOLVATION_PAIR6_12RDFINTEGRATOR_H
#include <BALL/SOLVATION/pair6_12RDFIntegrator.h>
#endif

#ifndef BALL_SOLVATION_SOLVENTDESCRIPTOR_H
#include <BALL/SOLVATION/solventDescriptor.h>
#endif


namespace BALL
{

	/** Processor for the computation of the van-der-Waals interaction energy
	  of a molecule with its surrounding.
		This processor uses a 6_12 pair potential for the calculation of
		dispersion and repulsion energies.
	  \\
		Energies are computed in units of kJ/mol.
		\\
		{\bf Definition:} \URL{BALL/SOLVATION/pair6_12InteractionEnergyProcessor.h}
	 */
	
	class Pair6_12InteractionEnergyProcessor
		:	public EnergyProcessor
	{

		public:

		// BAUSTELLE: Doku.
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
		struct Option
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

			/** The number density of the solvent.
				This option defines the number density of the surrounding solvent. Use
				float values of unit $\A^{-3}$ with this option.
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
		struct Default
		{
			/** Default verbosity level.
				@see Option::VERBOSITY
			 */
			static const int VERBOSITY;

			/** Default RDF setting.
				We use RDF information for the calculation of the interaction
				energy by default.
				@see Option::USE_RDF
			 */
			static const bool USE_RDF;
			static const char* RDF_FILENAME;
			static const char* SOLVENT_FILENAME;

			/** Default number density.
				This default value is the number density of water at 300 K and
				standard pressure (3.33253e-2 $\A^{-3}$).
				@see Option::SOLVENT_NUMBER_DENSITY;
			 */
			static const float SOLVENT_NUMBER_DENSITY;

			/** 
				@see Option::SURFACE_TYPE
			 */
			static const int SURFACE_TYPE;

			/** 
				@see Option::SURFACE_FILENAME
			 */
			static const char* SURFACE_FILENAME;

		};


		/** @name Constructors and destructors */
		//@{

		/** Default constructor */
		Pair6_12InteractionEnergyProcessor() throw();

		/** Copy constructor */
		Pair6_12InteractionEnergyProcessor
			(const Pair6_12InteractionEnergyProcessor& proc) throw();

		/** Destructor */
		virtual ~Pair6_12InteractionEnergyProcessor() throw();

		//@}


		/** @name Assignment */
		//@{

		/** Assignment operator */
		const Pair6_12InteractionEnergyProcessor& operator =
			(const Pair6_12InteractionEnergyProcessor& proc) throw();

		/** Clear function */
		virtual void clear() throw();
		
		//@}


		/** @name Processor functions */
		//@{

		/** */
		virtual bool finish() throw();

		//@}


		/** @name Options */
		//@{

		/** Options for the calculation of the caviation free energy */
		Options options;

		//@}


		/** @name Predicates */
		//@{

		/** Equality operator */
		bool operator == (const Pair6_12InteractionEnergyProcessor& proc) 
			const throw();

		//@}


		protected:

		/*_ solvent description, to be read from an INIFile */
		SolventDescriptor solvent_;

		/*_ the rdf description, also from an INIFile */
		RDFParameter rdf_parameter_;

		/*_ This is the tool to perform integrations with embedded RDf
		 * information */
		Pair6_12RDFIntegrator rdf_integrator_;


		private:

		void getExternalSurface_(vector< pair<Vector3, Surface> >& surface_map, 
				const char* surface_file) throw();
	};

} // namespace BALL

#endif // BALL__PAIR6_12INTERACTIONENERGYPROCESSOR_H
