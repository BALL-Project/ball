// $Id: poissonBoltzmann.h,v 1.15 2001/08/01 23:28:00 oliver Exp $ 
// Finite Difference Poisson Boltzmann Solver

#ifndef BALL_SOLVATION_POISSONBOLTZMANN_H
#define BALL_SOLVATION_POISSONBOLTZMANN_H

#ifndef BALL_COMMON_H_
#	include <BALL/common.h>
#endif

#ifndef BALL_DATATYPE_REGULARDATA3D_H
#	include <BALL/DATATYPE/regularData3D.h>
#endif

#ifndef BALL_KERNEL_SYSTEM_H
#	include <BALL/KERNEL/system.h>
#endif

#ifndef BALL_DATATYPE_OPTIONS_H
#	include <BALL/DATATYPE/options.h>
#endif

#include <vector>
using std::vector;

namespace BALL 
{

	/** Finite Difference Poisson Boltzmann Solver.
			{\bf Definition:} \URL{BALL/MOLMEC/COMMON/poissonBoltzmann.h}
	*/			
	class FDPB 
	{

		public:

		/**	Error codes: these are the possible error codes that can be produced by FDPB.
				@see	FDPB::getErrorCode()
				@see	FDPB::getErrorMessage()
		*/
		enum ErrorCode 
		{
			/**	No error.	
			*/
			ERROR__NONE							= -1,

			/**	Unknown error.
			*/
			ERROR__UNKNOWN					=  0,

			/**	Not implemented error. Someone has been too lazy to implement the
					this method. Wait for the next release...
			*/
			ERROR__NOT_IMPLEMENTED	=  1,

			/**	Unable to create the atom array/out of memory.
					FDPB internally creates a dynamic array
					containing the atoms of the system.
					If FDPB::setupAtomArray() cannot create this
					array you normally ran out of virtual memory.
			*/
			ERROR__CANNOT_CREATE_ATOM_ARRAY,

			/**	Unable to create SAS grid/out of memory.
					FDPB uses a TRegularData3D<char> (FDPB::SAS_grid) to describe 
					whether a point in space is inside the ion exclusion layer.
					This grid is created by FDPB::setupSASGrid(). On failure
					this error code is set. It usually indicates a lack of virtual memory.
			*/
			ERROR__CANNOT_CREATE_SAS_GRID,

			/**	Unable to create dielectric grid/out of memory.
					FDPB uses a {\tt TRegularData3D<float>} (\Ref{FDPB::eps_grid}) to describe the
					dielectric constant $\varepsilon$ as a function of space.
					This grid is created by calling FDPB::setupEpsGrid().
					It contains the relative dielectric constant between neighbouring grid points.\\
					If virtual memory is exhausted and the grid could not be created 
					this error is set.
			*/
			ERROR__CANNOT_CREATE_EPSILON_GRID,

			/**	Unable to create grid for the modified Debye Hueckel parameter/out of memory.
					The modified Debye Hueckel parameter $\bar{\kappa}$ is also a function of 
					space and therefore represented by a TRegularData3D<float> (FDPB::kappa_grid).
					The grid is created by FDPB::setupKappaGrid().\\
					If the creation of this grid fails due to a alack of virtual memory
					this error code is set.
			*/
			ERROR__CANNOT_CREATE_KAPPA_GRID,

			/**	Unable to create charge grid/out of memory.
					FDPB::setupQGrid() distributes the charge of the atoms
					in a grid. This grid is named FDPB::q_grid.\\
					If the creation of this grid fails due to a lack of virtual memory,
					this error code is set.
			*/
			ERROR__CANNOT_CREATE_CHARGE_GRID,
		
			/**	Unable to create electrostatic potential grid/out of memory.
					FDPB::setupPhiGrid() creates a TRegularData3D<float> (FDPB::phi_grid)
					containing the electrostatic potential as a function of space.
					If the creation of this grid fails due to a lack of virtual memory,
					this error code is set.
			*/
			ERROR__CANNOT_CREATE_PHI_GRID,

			/**	Create solvent accessible surface grid first.
					This error code is set by FDPB::setupKappGrid() if it is called
					but the ion excluded surface has not been set (usually by calling
					FDPB::setupSASGrid).\\
					Solution: call  FDPB::setupKappaGrid after calling FDPB::setupSASGrid.
			*/
			ERROR__SAS_GRID_REQUIRED,

			/**	Create dielectric constant grid first.
					This error code is set by FDPB::setupQGrid(), FDPB::setupKappaGrid(), 
					or FDPB::setupPhiGrid() if it was called, 
					but \Ref{FDPB::eps_grid} was not defined yet (this is usually done
					by calling \Ref{FDPB::setupEpsGrid}).\\
					Solution:	call FDPB::setupEpsGrid first
			*/
			ERROR__EPSILON_GRID_REQUIRED,

			/**	Create atom array first.
					This error code is set by FDPB::setupQGrid() or
					FDPB::setupBoundary() if it was called but FDPB::atom_array
					was not yet defined (this is usually done by calling FDPB::setupAtomArray()).\\
					Solution: call FDPB::setupAtomArray() first
			*/
			ERROR__ATOM_ARRAY_REQUIRED,

			/**	Create electrostatic potential grid first.
					FDPB::phi_grid contains the electrostatic
					potential at each point in space. FDPB::setupBoundary()
					sets this error code if it is called but FDPB::phi_grid
					has not been set yet.
					Solution: call FDPB::setupPhiGrid() before calling FDPB::setupBoundary()
			*/
			ERROR__PHI_GRID_REQUIRED,

			/**	Not enough virtual memory.
					This error code is set if FDPB::solve() ran out of
					virtual memory while creating some internal datastructures.
					Solution: reduce grid dimensions or increase grid spacing.
			*/
			ERROR__OUT_OF_MEMORY,

			/**	The specified method to smooth the dielectric constant grid 
					is not allowed.
					FDPB::setupEpsGrid sets this error code, if it cannot
					identify the method given in FDPB::Option::dielectric_smoothing.\\
					Solution: specify a valid smoothing method in FDPB::options
					@see	FDPB::Option::dielectric_smoothing
					@see	FDPB::DielectricSmoothing
			*/
			ERROR__UNKNOWN_DIELECTRIC_SMOOTHING_METHOD,
		
			/**	The specified charge distribution is not allowed.
					FDPB::setupQGrid() sets this error code, if it cannot
					identify the method given in FDPB::Option::charge_distribution.\\
					Solution: specify a valid charge distribution method in FDPB::options
					@see	FDPB::Option::charge_distribution
					@see	FDPB::ChargeDistribution
			*/
			ERROR__UNKNOWN_CHARGE_DISTRIBUTION_METHOD,

			/**	The specified boundary condition type is not allowed.
					FDPB::setupBoundary() sets this error code, if it cannot
					identify the boundary condition given in FDPB::Option::boundary.\\
					Solution: specify a valid boundary condition in FDPB::options
					@see	FDPB::Option::boundary
					@see	FDPB::Boundary
			*/
			ERROR__UNKNOWN_BOUNDARY_CONDITION_TYPE,

			/**	Upper or lower grid coordinates were specified in an incorrect format.
					This error code is set by FDPB::setupEpsGrid if the string
					given in FDPB::options (key FDPB::Option::LOWER or FDPB::Option::UPPER)
					were not in vector format.\\
					Solution: specify upper/lower coordinates in the correct format
					@see	Options::isVector
			*/
			ERROR__NOT_A_VECTOR_IN_UPPER_LOWER,		

			/**	Lower and upper corner of the grid were set to wrong values.
					Lower and upper corners of the grid given in FDPB::options
					(key FDPB::Option::LOWER and FDPB::Option::UPPER)
					must fulfill just one condition: every coordinate of lower
					hast to be less (not equal!) to the corresponding coordinate of
					upper.\\
					Solution: specify a reasonable non-degenerate grid
			*/
			ERROR__ILLEGAL_VALUE_FOR_LOWER_UPPER,

			/**	Call setup first.
					This error code is set by FDPB::solve() if FDPB::q_grid
					or FDPB::phi_grid or FDPB::eps_grid are undefined.\\
					Solution: define each of the above mentioned grids or call FDPB::setup()
			*/
			ERROR__SETUP_REQUIRED,

			/**	Total number of errors defined.
			*/
			NUMBER_OF_ERRORS
		};

		/**	Symbolic names for option keys.
				This struct contains a symbolic name
				for each recognized key in FDPB::options.\\
				For each symbol the required type is given under parameters.
		*/
		struct Option 
		{
			/**	The verbosity level.
					Use integer values with this option.
					0 = terse\\
					99 = tell me everything
					@see		Default::VERBOSITY
					@param	verbosity  integer
			*/
			static const char* VERBOSITY;

			/**	Print the timing for each step.
					
					This prints the timing (if Option::verbosity > 1)
					of each setup routine and the time needed to solve the
					equations.\\
					Results are also included in FDPB::results.
					@see		Default::PRINT_TIMING
					@param	print_timing	bool
			*/
			static const char* PRINT_TIMING;
			
			/**	The grid spacing in Angstrom.
					Use this option to define the distance between a grid point
					and its closest neighbour.
					@see	Default::SPACING
					@parameter	spacing float
			*/
			static const char* SPACING;
			static const char* BORDER;
			static const char* IONIC_STRENGTH;
			static const char* SOLUTE_DC;
			static const char* SOLVENT_DC;
			static const char* PROBE_RADIUS;
			static const char* ION_RADIUS;
			static const char* TEMPERATURE;
			static const char* BOUNDARY;
			static const char* CHARGE_DISTRIBUTION;
			static const char* DIELECTRIC_SMOOTHING;
			static const char* OFFSET;
			static const char* RMS_CRITERION;
			static const char* MAX_CRITERION;
			static const char* CHECK_AFTER_ITERATIONS;
			static const char* MAX_ITERATIONS;
			static const char* LOWER;
			static const char* UPPER;
			static const char* BOUNDING_BOX_LOWER;
			static const char* BOUNDING_BOX_UPPER;
		};

		/** This struct contains symbols for the available 
				boundary conditions.
		*/
		struct Boundary 
		{
			/**	Boundary condition zero: boundary points have zero potential.
					A Potential of zero is assigned to all points on the
					grid boundary.
					This is the simplest and least accurate method. It's use is
					not recommended.
			*/
			static const char* ZERO;

			/** Boundary condition Debye: potential at boundary points is
					estimated using Debye Hueckel theory.
					The Potential at each point of the grid boundary is estimated
					as the Debye Hueckel potential according to the following formula:\\
					\[
							\phi_{x,y,z} = \sum_i \frac{1}{4 \pi \varepsilon \varepsilon_0}
													\frac{q_i}{r} e^{-\frac{r}{d}}
					\]
					This options tends to become {\em very} slow for large grids.
			*/
			static const char* DEBYE;

			/**	Boundary condition Coulomb: potential at boundary points is estimated
					using coulomb's law.
			*/
			static const char* COULOMB;

			/**	Boundary condition Dipole: potential is estimated via dipole potentials
			*/
			static const char* DIPOLE;

			/**	Boundary condition Focusing: potential is estimated via a larger but coarser grid.
					Focusing calculates a larger grid (double extension in each direction)
					centered on the final grid with a four times the spacing of the final grid.
					Focusing also assigns an estimate of the electrostatic potential to 
					each grid point in the final grid, thus acceleratingthe convergence.
			*/
			static const char* FOCUSING;
		};
		
		/**	Constants to define  the charge distribution methods.
		*/
		struct ChargeDistribution 
		{
			/**	Charge distribution method Trilinear: charge is distributed 
					over the eight nearest gridpoints.
			*/
			static const char* TRILINEAR;

			/**	Charge distribution method Uniform: charge is distributed 
					over all gridpoints inside the atom radius.
			*/
			static const char* UNIFORM;
		};

		/**	Constants to define  the dielectric smoothing methods.
				To increase the accuracy of a FDPB calculation it prooved
				advantageous to smooth the discrete values for the dielectric 
				constant on the grid. We recommend the use of harmonic smoothing
				(which happens to be the default Default::dielectric_smoothing).
		*/
		struct DielectricSmoothing 
		{
			/**	Do not perform any smoothing
			*/
			static const char* NONE;

			/**	Uniform smoothing.
					This method assigns the arithmetic average of the point's value and
					its 26 next grid neighbours to the grid point:\\
					\[
							\varepsilon_0 = \frac{1}{27} \left( \sum_{i=1}^{26} \varepsilon_i + \varepsilon_0 \right)
					\]\\
			*/
			static const char* UNIFORM;

			/**	Harmonic smoothing.
					This method assigns the harmonic average of the point's value and 
					its 26 next grid neighbours to the grid point:\\
					\[
							\varepsilon_0 = \frac{1}{\sum_{i=1}^{26} \frac{1}{\varepsilon_i} + \frac{1}{\varepsilon_0}}
					\]\\
			*/
			static const char* HARMONIC;
		};

		/** Default values for FDPB options.  
				These values are used by 
				FDPB::setup methods if no options have been set.
		*/
		struct Default 
		{
			/** Default verbosity level.
					0 - shut up!
					@see	Option::VERBOSITY
			*/
			static const int VERBOSITY;

			/**	Default timing verbosity.
					false - don't tell anything
					@see	Option::print_timing
			*/
			static const bool PRINT_TIMING ;

			/** Default grid spacing.
					Default grid spacing is 0.6 Angstrom.
					@see	Option::SPACING
			*/
			static const float SPACING;

			/** Default distance between outmost atom and border.
					Default is 4.0 Angstrom.
					@see	Option::border
			*/
			static const float BORDER;

			/**	Default ionic strength of the solvent.
					Default is 0.0 mol/l 
					@see	Option::IONIC_STRENGTH
			*/
			static const float IONIC_STRENGTH;

			/**	Default temperature.
					Default is 298.15 K
					@see	Option::TEMPERATURE
			*/
			static const float TEMPERATURE;

			/**	Default probe radius.
					The probe radius used for the creation of the 
					solvent excluded surface of the solute. It should 
					be the solvent radius. Default is 1.5 Angstrom for water.
					@see	Option::PROBE_RADIUS
			*/
			static const float PROBE_RADIUS;

			/**	Default ionic exclusion radius.
					Default is 2.0 Angstrom
					@see	Option::ION_RADIUS
			*/
			static const float ION_RADIUS;

			/**	Default boundary condition.
					Default is Boundary::DIPOLE
					@see	Option::BOUNDARY
					@see	Boundary
			*/
			static const char*  BOUNDARY;

			/**	Default charge distribution method.
					Default is ChargeDistribution::uniform
					@see	Option::charge_distribution
					@see	ChargeDistribution
			*/
			static const char* CHARGE_DISTRIBUTION;

			/**	Default method for dielectric smoothing.
					Default is DielectricSmoothing::HARMONIC
					@see	Option::DIELECTRIC_SMOOTHING
					@see	DielectricSmoothing
			*/
			static const char* DIELECTRIC_SMOOTHING;

			/**	Default solvent dielectric constant.
					Default is 78.0 (roughly the dielectric constant of water at 298 K)
					@see	Option::SOLVENT_DC
			*/
			static const float SOLVENT_DC;

			/** Default solute dielectric constant.
					Default is 2.0 (good average derive from organic solvents).
					@see	Option::SOLUTE_DC
			*/
			static const float SOLUTE_DC;

			/**	Default convergence criterion for RMS change.
					Default is 1E-5
					@see	Option::RMS_CRITERION
					@see	Option::MAX_CRITERION
					@see	Default::MAX_CRITERION
			*/
			static const float RMS_CRITERION; 

			/**	Default convergence criterion for maximum change in potential.
					Default is 1E-4
					@see	Option::MAX_CRITERION
					@see	Option::RMS_CRITERION
					@see	Default::RMS_CRITERION
			*/
			static const float MAX_CRITERION;

			/**	Default for maximum number of iterations.
					Default is 500. This value is inappropriate for
					very large grids. 
					@see	Option::MAX_ITERATIONS
			*/
			static const Index MAX_ITERATIONS;

			/**	Default for the number of iterations between two checks for convergence.
					Default is 10
					@see	Option::CHECK_AFTER_ITERATIONS
			*/
			static const Index CHECK_AFTER_ITERATIONS;
		};

		/** 	Compact internal datastructure for the 
					administration of the atoms extracted from
					the system.
					This datastructure only contains position (x, y, z),
					charge (q) and radius (r) of the atoms.
					Index is used to store the index of the
					gridpoint defining the lower left front corner of
					the cube containing the atom. This is needed to
					calculate the potential at the atom's position
					(to calculate the total energy).
		*/
		struct FastAtomStruct 
		{
			float q;
			float r;
			float x, y, z;
			Index index;
		};

		typedef struct FastAtomStruct FastAtom;


		/**	@name	Constructors and Destructors 
		*/
		//@{
		
		/**	Default constructor.
				Creates an empty FDPB object.
		*/
		FDPB();
		
		/** Copy constructor.
				Copies an existing FDPB object.
		*/
		FDPB(FDPB&);

		/** Constructor.
				Creates an instance of FDPB and calls
				setup(system).
				The options used are the default options.
				@see	setup(system)
		*/
		FDPB(System& system);

		/** Constructor.
				Creates an instance of FDPB and assigns
				the given options to the FDPB object's options.
				@see	options
				@see	Options
		*/
		FDPB(Options& new_options);

			/** Constructor.
				Creates an instance of FDPB and calls
				setup(system, options)
				@see	setup(system, options)
				@see	options
				@see	Options
		*/
		FDPB(System& system, Options& new_options);

		/**	Destructor.
		*/
		virtual ~FDPB();

		/**	Frees all allocated memory and destroys the options and results.
		*/
		void destroy();

		/**	Destroys all allocated grids and the atom array.
				This method reverts the FDPB object to the state it had prior to
				a call to setup. Especially it frees all memory intensive datastructures.\\
				{\em destroyGrids} deletes eps_grid, kappa_grid, q_grid, phi_grid, and SAS_grid.
				Contrary to destroy, it doesnt't clear options and results.\\
				@see	destroy
				@see	setup
		*/
		void destroyGrids();

		//@}
		/**	@name Setup methods
				Using these methods, a FDPB object can be prepared
				for a calculation.\\
		*/
		//@{

		/**	General setup method.
				Setup calls (in this order!)
				\begin{itemize}
					\item setupAtomArray
					\item setupEpsGrid
					\item setupSASGrid
					\item setupKappaGrid
					\item setupPhiGrid
					\item setupQGrid
					\item setupBoundary
				\end{itemize}
				If any of theses method invocations fail, it terminates at this point and
				returns false.\\
				On successful execution it returns true and the FDPB object is
				ready to solve the Poisson Boltzmann equation by calling solve().
				@see	setup(System& system, Options& options)
				@param 	System&	system the molecular system to be examined.
				@return	bool true on success, call getErrorCode otherwise
		*/
		bool setup(System& system);

		/**	Setup with assignment of options.
				This method copies the options given by options into the
				options variable of the FDPB object and invokes setup(system) afterwards.
				@see	setup(System& system)
				@param	options the new options
				@param	system 	the molecular system to be evaluated
				@return 	bool true on success, call getErrorCode otherwise
		*/
		bool setup(System& system, Options& options);

		/**	Setup the dielectric grid.
				The Finite Difference Poisson Boltzmann Method is based
				on the assumption that one can determine which points on a 
				given grid are inside a given solute (with low dielectric constant)
				and which points are outside (i.e., they are in the high dielectric
				constant solvent).\\
				\Ref{setupEpsGrid} creates a grid containing the dielectric constant
				between any two neighbouring grid points (i.e., it contains 3 N values).
				Points inside the molecule (i.i, inside the radius of any atom) are set to
				the solute dielectric constant, all other points are set to the solvent
				dielectric constant.\\
				This method also sets the coordinates and dimensions of the grid (extracted
				from either options or system) and the grid spacing.\\
				Normally this method is not called by the user, but automatically
				by setup. If you consider to call it by yourself, be sure to call 
				the single setup methods in the correct order (as described for setup).\\
				This method may set one of the following error codes and return false afterwards:
				\begin{itemize}
					\item ERROR__NOT_A_VECTOR_IN_UPPER_LOWER
					\item ERROR__ILLEGAL_VALUE_FOR_LOWER_UPPER
				\end{itemize}
				@param	system the system to be evaluated
				@return	true on success, call getErrorCode otherwise
		*/
		bool setupEpsGrid(System& system);

					
		// BAUSTELLE
		/**	Setup the ion accessible grid.
				Not yet implemented!
		*/
		bool setupSASGrid(System& system);

		/**	Setup an compact datastructure containing all charged atoms.
				This method creates a dynamic array containing all charged atoms.\\
				The method may set the error code to ERROR__CANNOT_CREATE_ATOM_ARRAY 
				and terminate with false if insufficient virtual memory is available to create
				the array.
				@param	system	the system to be evaluated
				@return	bool	true on success, call getErrorCode otherwise
				@see	atom_array
				@see	FastAtom
		*/
		bool setupAtomArray(System& system);

		/**	Setup the Debye Hueckel parameter grid.
		*/
		bool setupKappaGrid();

		/**	Setup charge grid.
		*/
		bool setupQGrid();

		/**	Setup electrostatic potential grid
		*/
		bool setupPhiGrid();

		/**	Setup boundary conditions for the electrostatic potential.
		*/
		bool setupBoundary();

		//@}
		/**	@name Executing the calculation and retrieving the results 
		*/
		//@{	

		/**	Solves the linearized Poisson-Boltzmann equation.
		*/
		bool 	solve();

		/**	Returns the energy of the last calculation.
				The total electrostatic energy of the FDPB object after
				the last iteration (even if no convergence was reached!)
				is returned in units of kJ/mol.
				@see	getNumberOfIterations
				@return	energy in kJ/mol
		*/
		double getEnergy() const;

		/** Return the reaction field energy.
				@return reaction field energy in kJ/mol
		*/
		double getReactionFieldEnergy() const;

		/**	Calculate the reaction field energy.
				@return reaction field energy in kJ/mol
		*/
		double calculateReactionFieldEnergy() const;

		/**	Returns the number of iterations needed to converge.
				Returns the number of iterations taken in the last call to FDPB::solve().
				If convergence could not be reached (i.e., the convergence criterions defined
				in options could not be met), -1 is returned.
				@return	int number of iterations
				@see	Option::max_iterations
				@see	Default::max_iterations
		*/
		Size	getNumberOfIterations() const;

		//@}

    /** @name Debugging 
    */
    //@{

    /** Return the last error code.
        If a method fails, an internal error code is set in
        FDPB. This error code can be queried by calling this method.
        If no error occured it should return FDPB::ERROR__NONE.
        @see  getErrorMessage
        @see  ErrorCodes
    */
    Index getErrorCode() const;

    /** Return the last error message.
        @see  getErrorCode
        @see  ErrorCodes
    */
    static String getErrorMessage(Index error_code);
    //@}


		/**	@name	Options and results of the calculation 
		*/
		//@{

		/**	The options for the FDPB calculation.
		*/
		Options	options;

		/**	The results of the last calculation.
		*/
		Options	results;

		//@}
		/**	@name	Grids and arrays	
		*/
		//@{

		/**	The grid containing the spatial dependent dielectric constant.
				The (relative) dielectric constant is unitless.
				@see	setupEpsGrid
		*/
		TRegularData3D<Vector3>*	eps_grid;

		/**	The grid containing the modified Debye Hueckel parameter.
				@see	setupKappaGrid
		*/
		TRegularData3D<float>*	kappa_grid;

		/**	The grid containing the atom charges (distributed).
				Each atom's charge is distributed on the grid by setupQGrid, according
				to the charge distribution method specified in options.\\
				q_grid contains these partial charges. Units are elementary charges, if
				the atom charges were given in multiples of the elementary charge (Atom::setCharge).
				@see	BALL_ELEMENTARY_CHARGE
				@see	setupQGrid
		*/
		TRegularData3D<float>*	q_grid;

		/**	The grid containing the electrostatic potential. 
				Before a calculation this is grid is initialized with
				the boundary condition. After the calculation (i.e. after
				a call to {\tt solve()}) it contains the electrostatic potential
				in units of J/C resulting from the Poisson-Boltzmann equation).
				@see		setupPhiGrid()
				@see		setupBoundary()
				@see		solve()
		*/
		TRegularData3D<float>*	phi_grid;

		/**	The grid describing the solvent accessible surface of the system.
		*/
		TRegularData3D<char>*	SAS_grid;

		/**	An array containing a fast representation of all atoms in the system.
				@see		FastAtom
		*/
		vector<FDPB::FastAtom>*	atom_array;
		
		//@}

		protected:

		Vector3	lower_;
		Vector3	upper_;
		Vector3	offset_;

		bool		use_offset_;

		float		spacing_;

		// final energy of the last calculation
		double 	energy_;

		// the reaction field energy of the last calculation
		double 	reaction_field_energy_;

		// the indices of the boundary points,
		// i.e., the points that have at least one neighbouring
		// grid point that is inside the solute
		vector<Position>	boundary_points_;

		// number of iterations of the last calculation
		Size number_of_iterations_;

		// error code. use getErrorMessage to access the corresponding 
		// error message
		int	error_code_;
 
		static const char* error_message_[];
	};

} // namespace BALL 

#endif 
