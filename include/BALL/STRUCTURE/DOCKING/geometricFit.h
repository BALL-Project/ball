#ifndef BALL_STRUCTURE_DOCKING_GEOMETRICFIT_H
#define BALL_STRUCTURE_DOCKING_GEOMETRICFIT_H

#ifndef BALL_DATATYPE_REGULARDATA3D_H
#include <BALL/DATATYPE/regularData3D.h>
#endif

#ifndef BALL_KERNEL_SYSTEM_H
#include <BALL/KERNEL/system.h>
#endif

#ifndef BALL_MATHS_COMPLEX_H
#include <BALL/MATHS/complex.h>
#endif

#ifndef BALL_MATHS_FFT3D_H
#include <BALL/MATHS/FFT3D.h>
#endif

#ifndef BALL_STRUCTURE_DOCKING_DOCKINGALGORITHM_H
#include <BALL/STRUCTURE/DOCKING/dockingAlgorithm.h>
#endif

namespace BALL
{
  /**
     class GeometricFit
     This class is derived from class DockingAlgorithm.
     Protein docking algorithm via geometric fit invented by Katchalski-Katzir,
     et at. (1992) is implemented in this class.
  */
  class GeometricFit : public DockingAlgorithm
  {
    public:
      BALL_CREATE(GeometricFit)
			
			/**	Symbolic names for option keys.
				This struct contains a symbolic name
				for each recognized key in FDPB::options. \par
				For each symbol the required type is given under parameters.
			*/
			typedef struct Option 
			{
				// the parameter r,
				// "Any grid point is considered inside the molecule if there is at 
				// least one atom nucleus within a distance r from it." 
				// Default value is 1.8 Angstrom
				static const String NEAR_RADIUS;

				// Default grid spacing. 
				// Default value is 1.0 Angstrom
	 			static const String GRID_SPACING;
	
				// grid size = (ceil)(molecule size / grid spacing);
			  static const String GRID_SIZE;	

				// The thickness of the surface of the protein
				// Default value is 1.0 Angstrom
				static const String SURFACE_THICKNESS;

				// Degree interval is the delta parameter, 
				// which is the degrees that protein B rotates every time
				// Default value is 20 degrees
				static const String DEGREE_INTERVAL;

				// How many peaks should the program keep for each rotation
				static const String TOP_N;

				/** Surface definition to use for the inside/outside criterion.
				 *  Possible options are:
				 *  SurfaceType::CONNOLLY and SurfaceType::VAN_DER_WAALS
				 *	@see Default::SURFACE_TYPE
				 *	@param surface_type int
				 */
				static const String SURFACE_TYPE;	

				// How many results should be kept in total (also in DockingAlgorithms)
				static const String BEST_NUM;

				// Verbosity of the Algorithm (also in DockingAlgorithm)
				static const String VERBOSITY;
			};
    
      struct Default
			{
				// Default parameter r,
				// Default value is 1.8 Angstrom
				static const float NEAR_RADIUS;

				// Default grid spacing. 
				// Default value is 1.0 Angstrom
				static const float GRID_SPACING;

				// grid size = (ceil)(molecule size / grid spacing);
				static const int GRID_SIZE;	

				// Default thickness of the surface of the protein
				// Default value is 1.0 Angstrom
				static const float SURFACE_THICKNESS;

				// Degree interval is the delta parameter, 
				// which is the degrees that protein B rotates every time
				// Default value is 20 degrees
				static const double DEGREE_INTERVAL;

				/** Default surface type.
				 * 	Default is SurfaceType::CONNOLLY
				 */
				static const int SURFACE_TYPE;

				// How many peaks should the program keep for each rotation
				static const int TOP_N;

				// How many overall top best peaks should the program keep 
				// after doing all rotations.
				static const int BEST_NUM;

				// Verbosity of the algorithm
				// Default value is 0
				// For values > 1, some information is printed on Log
				// For values > 5, timing information is included
				static const int VERBOSITY;
			};

      /**
				nested class Peak_ 
				This class is needed for an efficient handling of all peaks corresponding
				to one orientation in a multiset
      */
      class Peak_
			{
				public:

					// constructor
					Peak_() throw();

					// destructor
					~Peak_() throw();

					// Operator <
					bool operator < (const Peak_& p) const throw();

					double  value;
					Vector3 orientation;
					Vector3 translation;
			};


      /**
				nested class RotationAngles_
      */
      class RotationAngles_
			{
				public: 

					// constructor
					RotationAngles_() throw();
					
					// constructor
					RotationAngles_( int step ) throw();

					// destructor
					~RotationAngles_() throw() {};

					// generate all non-degenerate rotation angles.
					// This algorithm is based on ???
					bool generateAllAngles( const int deg ) throw();

					int getRotationNum()
						throw()
					{
						return ang_num_;
					}

					int getXAng( int n ) // get the euler angle rotate around x axis
						throw()
					{
						return phi_[n];
					}

					int getYAng( int n ) // get the euler angle rotate around y axis
						throw()
					{
						return theta_[n];
					}

					int getZAng( int n ) // get the euler angle rotate around z axis
						throw()
					{
						return psi_[n];
					}

				private:
					int max_rotation_;
					int  ang_num_;
					vector<int> phi_;
					vector<int> theta_;
					vector<int> psi_;
			};

      // PROTEIN_A is the static protein, i.e., the bigger one;
      // PROTEIN_B is the mobile protein, i.e., the smaller one.
      enum ProteinIndex{  PROTEIN_A = 1, PROTEIN_B    = 2 };

			// The surface type to use for construction of the grids.
      enum SurfaceType {  CONNOLLY  = 1, VAN_DER_WAALS = 2 };

      // Default constructor
      // Creates an empty GeometricFit object
      GeometricFit() 
				throw();

			/** Constructor.
					Creates an instance of Geometric Fit and calls
					setup(system1, system2).
					The options used are the default options.
			*/
			GeometricFit(System &system1, System &system2)
				throw();

			/** Constructor.
					Creates an instance of Geometric Fit and assigns
					the given options to the Geometric Fit object's options.
			*/
			GeometricFit(Options& new_options)
				throw();
			
			/** Constructor.
					Creates an instance of FDPB and calls
					setup(system1, system2, new_options)
			*/
			GeometricFit(System &system1,System &system2 ,Options& new_options)
				throw();
				
				
/*       // Copy constructor */
/*       // Copies an existing GeometricFit object */
/*       GeometricFit( const GeometricFit& geo_fit ); */

      // Destructor
      ~GeometricFit() throw();

			/** The setup routines.
			 */
			virtual void setup(System& system1, System& system2, Options& new_options)
				throw();
			
			virtual void setup(System& system1, System& system2)
				throw();		
	
			/** Start the main loop of the algorithm.
			 */
			void start()
				throw();

      // return the overall docking progress as a percentage
      float getProgress() const
				throw();
      
      // 
      bool hasFinished() const
				throw();

			/** Return the ranked conformations.
			 */
			ConformationSet getConformationSet(Index total_number = 0)
				throw();
			
      //////////////////////////////////////////
      // the member variables

			/**	The options for the algorithm.
			*/
			Options	options;
      
		protected:

      // Free all allocated memory and destroys the options and results
      void destroy_()
				throw();

      // find the inside points
      void findInsidePoints_( System& system, ProteinIndex pro_idx )
				throw();

      // find out the surface points according to the Connolly's surface definition.
      void findConnollySurfacePoints_( System& system, ProteinIndex pro_idx )
				throw();

      // find out the surface points according to the van der Waal's surface definition.
      void findVanDerWaalsSurfacePoints_( System& system, ProteinIndex pro_idx )
				throw();

			/** Compute the center of mass of system
			 */
      Vector3 getMassCenter_( System& system ) 
				throw();

			/** Compute the radius of the circumsphere of all atoms in system.
			 */
      float getRadius_( System& system )
				throw();

      void doPreTranslation_( ProteinIndex pro_idx )
				throw();

      void initGridSizes_()
				throw();

      /** Optimize grid size for the FFTW.
			 *	Certain combinations of certain powers are optimal for the fft algorithm, and
			 *	this function tries to determine the smallest optimal combination large enough
			 *	to accomodate the original data.
			 */ 
      int optimizeGridSize_( int raw_size )
				throw();

			/** Initialize the grid.
			 */
      void initFFTGrid_( ProteinIndex pro_idx )
				throw();

      // make grid from System
      void makeFFTGrid_( ProteinIndex pro_idx )
      	throw();

      // get the global peaks and put them into a list
      void getGlobalPeak_(Peak_* peak_list)
				throw();

      // change the orientation of protein around its center according to euler_ang
      void changeProteinOrientation_( System& system, Vector3 euler_ang )
				throw();

      // calculate the conjugate of each point in FFT grid
      void calcConjugate_( ProteinIndex pro_idx )
				throw();

      // calculate the product of the two FFT grids
      void FFTGridMulti_()
				throw();

      // get the transformation of the peak value according to its position in matrix
      Vector3 getTranslation_( const Vector3& mat_pos )
				throw();

      Vector3 getSeparation_( const Vector3& mat_pos )
				throw();


      // here we have two units for the size
      // index : in the unit of grid points
      // coord : in the unit of Angstrom

      // the FFT grid for protein A
      FFT3D*  FFT_grid_a_;
      
      // the FFT grid for protein B
      FFT3D*  FFT_grid_b_;

      float radius_a_;
      float radius_b_;

      Vector3 FFT_grid_lower_index_;
      Vector3 FFT_grid_upper_index_;
      Vector3 FFT_grid_lower_coord_;
      Vector3 FFT_grid_upper_coord_;     
      
      Vector3 FFT_grid_size_index_;

      // the translation we do to the system b before FFT
      // it is to reduse the size of the FFT_grid
      Vector3 pre_translation_a_;
      Vector3 pre_translation_b_;

      Vector3 FFT_grid_origin_;

      // current execution progress
      int current_round_;

      // execution round of whole work
      int total_round_;

			// ordered set of the top_n peaks
			multiset<Peak_> peak_set_;

			// Needed to produce the correct results in getRankedConformations
			System system_backup_a_;
			System system_backup_b_;

    }; // class GeometricFit

} // namespace BALL

#endif
