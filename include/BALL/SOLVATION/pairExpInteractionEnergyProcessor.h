// $Id: pairExpInteractionEnergyProcessor.h,v 1.4 2000/10/06 11:51:49 anker Exp $

// BAUSTELLE: Pfad?
#ifndef BALL_SOLVATION_PAIREXPINTERACTIONENERGYPROCESSOR_H
#define BALL_SOLVATION_PAIREXPINTERACTIONENERGYPROCESSOR_H

#ifndef BALL_COMMON_H
#include <BALL/common.h>
#endif

#ifndef BALL_DATATYPE_OPTIONS_H
#include <BALL/DATATYPE/options.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
#include <BALL/KERNEL/atom.h>
#endif

#ifndef BALL_MATHS_SURFACE_H
#include <BALL/MATHS/surface.h>
#endif

#ifndef BALL_ENERGY_ENERGYPROCESSOR_H
#include <BALL/ENERGY/energyProcessor.h>
#endif

#ifndef BALL_STRUCTURE_RDFPARAMETER_H
#include <BALL/STRUCTURE/RDFParameter.h>
#endif

#ifndef BALL_SOLVATION_PAIREXPRDFINTEGRATOR_H
#include <BALL/SOLVATION/pairExpRDFIntegrator.h>
#endif

#ifndef BALL_SOLVATION_SOLVENTDESCRIPTOR_H
#include <BALL/SOLVATION/solventDescriptor.h>
#endif

// BAUSTELLE: Die Konstanten alpha, C1 und C2 bzw die K_ij müssen noch in
// sinnvoller Form eingebettet werden.

namespace BALL
{

	/** Processor for the computation of the interaction energy of a molecule.
		This processor uses a Exp pair potential for the calculation of
		dispersion and repulsion energies.
		{\bf Definition:} \URL{BALL/.../pairExpInteractionEnergyProcessor.h}
	 */
	
	class PairExpInteractionEnergyProcessor
		:	public EnergyProcessor
	{

		public:

		// BALL_CREATE(PairExpInteractionEnergyProcessor)
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
			PairExpInteractionEnergyProcessor::options.
		 */
		struct Option
		{
			
			/** The verbosity level.
				Use integer values with this option.
				@see Default::VERBOSITY
				@param verbosity integer
			 */
			static const char* VERBOSITY;

			/** Constants for the pair potential.
				@see Default::ALPHA
				@see Default::C1
				@see Default::C2
				@param alpha float
				@param c1 float
				@param c2 float
			 */
			static const char* ALPHA;
			static const char* C1;
			static const char* C2;

			static const char* CLAVERIE_FILENAME;

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

			/** The rule file containing the atomic radii.
				This option determines the file containing the rules defining atomic
				radii.
				@see Default::RADIUS_RULE_FILE
				@param radius_rule_file shar*
			 */
			static const char* RADIUS_RULE_FILE;

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

			/** Default pair potential constants.
			@see: Option::ALPHA
			@see: Option::C1
			@see: Option::C2
			*/
			static const double ALPHA;
			static const double C1;
			static const double C2;

			static const char* CLAVERIE_FILENAME;

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

			/** The standard radius rule file.
				This is the name of the standard rule file for interaction energy
				calculation ("radius.rul")
				@see Option::RADIUS_RULE_FILE;
			 */
			static const char* RADIUS_RULE_FILE;

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
		PairExpInteractionEnergyProcessor() throw();

		/** Copy constructor */
		PairExpInteractionEnergyProcessor(const
				PairExpInteractionEnergyProcessor& proc) throw();

		/** Destructor */
		virtual ~PairExpInteractionEnergyProcessor() throw();

		//@}


		/** @name Assignment */
		//@{

		/** Assignment operator */
		const PairExpInteractionEnergyProcessor& operator = 
			(const PairExpInteractionEnergyProcessor& proc) throw();

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

		/** Options for the calculation of the free energy */
		Options options;

		//@}


		protected:

		SolventDescriptor solvent_;
		double alpha_;
		double C1_;
		double C2_;
		// RDFParameter rdf_parameter_;
		// PairExpRDFIntegrator rdf_integrator_;
	

		private:

		void computeClaverieParameters(Atom::Type solvent_type,
				Atom::Type solute_type, std::pair<float, float>& parameters)
			throw();
		void getExternalSurface_(
				std::vector< std::pair<Vector3, Surface> >& surface_map, 
				const char* surface_file) throw();

	};

} // namespace BALL

#endif // BALL_SOLVATION_PAIREXPINTERACTIONENERGYPROCESSOR_H
