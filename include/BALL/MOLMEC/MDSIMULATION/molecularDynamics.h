// $Id: molecularDynamics.h,v 1.6 2000/03/28 15:30:10 oliver Exp $
// MolecularDynamics: A base class for doing molecular dynamics simulations    
// Useful MD classes must be derived from this class 

#ifndef BALL_MOLMEC_MDSIMULATION_MOLECULARDYNAMICS_H
#define BALL_MOLMEC_MDSIMULATION_MOLECULARDYNAMICS_H


// Include all necessary BALL headers 
#ifndef _BALL_COMMON_H
# include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_PTE_H
# include <BALL/KERNEL/PTE.h>
#endif

#ifndef BALL_KERNEL_SYSTEM_H
# include <BALL/KERNEL/system.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
# include <BALL/KERNEL/atom.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
# include <BALL/MATHS/vector3.h>
#endif


#ifndef BALL_DATATYPE_OPTIONS_H
# include <BALL/DATATYPE/options.h>
#endif

#ifndef BALL_MOLMEC_PARAMETER_FORCEFIELDPARAMETER_H
# include <BALL/MOLMEC/PARAMETER/forceFieldParameters.h>
#endif

#ifndef BALL_MOLMEC_PARAMETER_ATOMTYPES_H
# include <BALL/MOLMEC/PARAMETER/atomTypes.h>
#endif

#ifndef BALL_MOLMEC_COMMON_PERIODIC_BOUNDARY_H
# include <BALL/MOLMEC/COMMON/periodicBoundary.h>
#endif

#ifndef BALL_MOLMEC_COMMON_FORCEFIELD_H
# include <BALL/MOLMEC/COMMON/forceField.h>
#endif

#ifndef BALL_MOLMEC_COMMON_SNAPSHOT_H
# include <BALL/MOLMEC/COMMON/snapShot.h>
#endif


// STL include commands 
#include <vector>

namespace BALL
{
	/**	Molecular Dynamics base class.
			This class is the base class for classical molecular dynamics
			simulations. Actual MD classes must be derived from this class  
	*/
	class MolecularDynamics
	{
		public:
		
		/**     Local classes for option names and default option values 
		 * for all MD(sub)classes 
		 */
		struct Option
		{

			/**     The maximal number of iterations to be carried out 
			 */
			static const char *MAXIMAL_NUMBER_OF_ITERATIONS;

			/**     The current number of iteration 
			 */
			static const char *NUMBER_OF_ITERATION;

			/**     After how many iterations shall energy information be saved 
			 */
			static const char *ENERGY_OUTPUT_FREQUENCY;

			/**    After how many iterations shall a snapshot of the system be taken         
			 */
			static const char *SNAPSHOT_FREQUENCY;

			/**    The time step in picoseconds 
			 */
			static const char *TIME_STEP;

			/**    The reference temperature in Kelvin
			 */
			static const char *REFERENCE_TEMPERATURE;

			/**    The parameter for heat bath coupling in picoseconds. 
			 * It should be greater than 0.1 ps. 
			 */
			static const char *BATH_RELAXATION_TIME;

			/**    The current time of the simulation in picoseconds
			 */
			static const char *CURRENT_TIME;
		};

		struct Default
		{

			/**     The maximal number of iterations to be simulated. 
			 * Default 10,000
			 */
			static const int MAXIMAL_NUMBER_OF_ITERATIONS;

			/**     The current number of iteration 
			 */
			static const int NUMBER_OF_ITERATION;

			/**     After how many iterations shall the current energy/temperature 
			 * be calculated/saved.
			 * Default 50  
			 */
			static const int ENERGY_OUTPUT_FREQUENCY;

			/**    After how many iterations shall the current positions/velocities be saved.
			 * Default 1000
			 */
			static const int SNAPSHOT_FREQUENCY;

			/**    The size of the time step in picoseconds. Default 0.0005 ps
			 */
			static const double TIME_STEP;

			/**     The reference temperature for the simulated system.
			 * Default 300 K.
			 */
			static const double REFERENCE_TEMPERATURE;


			/**    The parameter for heat bath coupling in picoseconds. Default = 0.2 ps
			 */
			static const double BATH_RELAXATION_TIME;

			/**    The current time of the simulation in picoseconds
			 * Useful when doing several successive MD runs on the system 
			 */
			static const double CURRENT_TIME;
		};

		/**  @name Constructors and Destructors  
		 */
		//@{

		BALL_CREATE (MolecularDynamics)
		/**  Default constructor.
		*/
		MolecularDynamics ();

		/**  Constructor. It expects the forcefield
		*/
		MolecularDynamics (ForceField & myforcefield);


		/**  Copy constructor
		*/
		MolecularDynamics (const MolecularDynamics & rhs, bool deep = true);

		/**  Destructor.
		*/
		virtual ~ MolecularDynamics ();

		//@}


		/**  @name Assignments 
		*/
		//@{

		/**  Assignment operator
		*/
		MolecularDynamics& operator = (const MolecularDynamics & rhs);

		//@}

		/**  @name Debugging and Diagnostics 
		*/

		//@{

		/**  Has the molecular dynamics class been successfully set up?
		*/
		bool isValid () const;

		//@}

		/**  @name Setup methods 
		*/
		//@{


		/**  Set up the molecular dynamics 
		*/
		virtual bool setup (ForceField & myforcefield, SnapShotManager * snapshot_man);

		/**  Set up the molecular dynamics    
		*/
		virtual bool setup (ForceField & myforcefield, SnapShotManager * snapshot_man, const Options & myoptions);

		/**  Specific setup; derived class can use this method for
				additional preparations if necessary 
		*/
		virtual bool specificSetup ();

		//@}


		/**  @name Accessors 
		 */
		//@{

		/**  Set the number of the current iteration 
		*/
		void setNumberOfIteration (Size number);

		/**  Set the maximal number of iterations
		*/
		void setMaximalNumberOfIterations (Size number);

		/**  Set the reference temperature for the system 
		*/
		void setReferenceTemperature (double temperature);

		/**        Set the current time of the MD run in picoseconds
		*/
		void setCurrentTime (double time);

		/**  Set the energy output frequency
		*/
		void setEnergyOutputFrequency (Size number);

		/**  Set the snapshot frequency
		*/
		void setSnapShotFrequency (Size number);

		/**  Get the energy output frequency
		*/
		Size getEnergyOutputFrequency () const;

		/**        Get the current iteration of the MD simulation 
		*/
		Size getNumberOfIteration () const;

		/**        Get the maximal number of iterations of the MD simulation 
		*/
		Size getMaximalNumberOfIterations () const;

		/**  Get the snapshot frequency
		*/
		Size getSnapShotFrequency () const;

		/**  Get the current temperature of the system         
		*/
		double getTemperature () const;

		/**        Get the current time of the MD simulation
		*/
		double getTime () const;

		/**        Get the current total energy of the system
		*/
		double getTotalEnergy () const;

		/**        Get the current potential energy of the system
		*/
		double getPotentialEnergy () const;

		/**        Get the current kinetic energy of the system
		*/
		double getKineticEnergy () const;

		/**  Get the force field the MD simulation is bound to 
		*/
		ForceField *getForceField () const;


		/**  Start the molecular dynamics simulation                      
				The base class does not provide any
		  implementation for these methods below. 
		*/
		virtual void simulate (bool restart = false);

		/**  Start the molecular dynamics simulation and carry out
		  the given number of iterations. 
		*/
		virtual void simulateIterations (Size number, bool restart = false);

		/**  Start the molecular dynamics simulation and carry out
		  the given time in picoseconds    
		*/
		virtual void simulateTime (double simulation_time, bool restart = false);

		//@}


		/**  @name Public Attributes
		*/
		//@{

		/*         The options for this class
		*/
		Options options;

		//@}

		protected:

		/**  @name Protected Methods 
		*/
		//@{

		/**        A method for calculating the
		  current temperature in the system
		*/
		void updateInstantaneousTemperature ();
		//@}

		/**  @name Protected Attributes
		*/
		//@{

		/**  The boolean variable indicates if the setup of the 
		  molecular dynamics has been successful 
		*/
		bool valid_;


		/**  The force field the MD class is bound to          
		*/
		ForceField* force_field_ptr_;

		/**        The system the MD class is bound to
		*/
		System* system_ptr_;


		/**        The list of atoms. The simulation 
			will be carried out for these atoms 
		*/
		AtomVector atom_vector_;


		/**  The current iteration number  
		*/
		Size number_of_iteration_;

		/**  The maximal number of iterations to be simulated when
			calling the simulate() method. 
		*/
		Size maximal_number_of_iterations_;

		/**  the time step in picoseconds                  
		*/
		double time_step_;

		/**  The reference temperature  in Kelvin 
		*/
		double reference_temperature_;

		/**  The current(kinetic) temperatue in Kelvin 
		*/
		double current_temperature_;

		/**        The current kinetic energy  in kJ/mol
		*/
		double kinetic_energy_;

		/**        The current total energy in kJ/mol
		*/
		double total_energy_;


		/**        The current time of the MD run                 
		*/
		double current_time_;

		/**  Frequency of energy output (i.e. after how many iterations) 
		*/
		Size energy_output_frequency_;

		/**  Frequency of taking snapshots (i.e. after how many iterations) 
		*/
		Size snapshot_frequency_;

		/**        The Snapshot Manager that is used for taking snapshots
		*/
		SnapShotManager *snapshot_manager_ptr_;
		//@}
	};	// end of class MolecularDynamics 

}	 // end of namespace Ball

#endif // BALL_MOLMEC_MOLECULARDYNAMICS_H
