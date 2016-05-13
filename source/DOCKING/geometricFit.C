//TODO: - optimize list of include files
//			- find out if all functions are really ever called in the algorithm
//			- make use of real-to-complex property

#include <BALL/DOCKING/geometricFit.h>

#include <cmath>
#include <list>
#include <set>

////////////////
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/atomIterator.h>
#include <BALL/KERNEL/molecule.h>

#include <BALL/DATATYPE/hashGrid.h>

#include <BALL/STRUCTURE/geometricProperties.h>
#include <BALL/STRUCTURE/geometricTransformations.h>
#include <BALL/STRUCTURE/surfaceProcessor.h>

#include <BALL/SYSTEM/timer.h>

#include <BALL/COMMON/exception.h>
#include <BALL/DATATYPE/string.h>

#ifdef BALL_HAS_MPI
# include <BALL/SYSTEM/MPISupport.h>
#endif


using namespace std;
using namespace BALL;


const String GeometricFit::Option::NEAR_RADIUS = "near_radius";
const String GeometricFit::Option::GRID_SPACING = "grid_spacing";
const String GeometricFit::Option::GRID_SIZE = "grid_size";
const String GeometricFit::Option::SURFACE_THICKNESS = "surface_thickness";
const String GeometricFit::Option::SURFACE_TYPE = "surface_type";
const String GeometricFit::Option::DEGREE_INTERVAL = "degree_interval";
const String GeometricFit::Option::TOP_N = "top_n";
const String GeometricFit::Option::BEST_NUM = "best_num";
const String GeometricFit::Option::VERBOSITY = "verbosity";
const String GeometricFit::Option::PHI_MIN = "phi_min";
const String GeometricFit::Option::PHI_MAX = "phi_max";
const String GeometricFit::Option::DEG_PHI = "deg_phi";
const String GeometricFit::Option::THETA_MIN = "theta_min";
const String GeometricFit::Option::THETA_MAX = "theta_max";
const String GeometricFit::Option::DEG_THETA = "deg_theta";
const String GeometricFit::Option::PSI_MIN = "psi_min";
const String GeometricFit::Option::PSI_MAX = "psi_max";
const String GeometricFit::Option::DEG_PSI = "deg_psi";
const String GeometricFit::Option::PENALTY_STATIC = "penalty_static";
const String GeometricFit::Option::PENALTY_MOBILE = "penalty_mobile";
const String GeometricFit::Option::NUMBER_OF_PROCESSES = "number_of_processes";
const String GeometricFit::Option::SLAVE_PATH = "slave_path";

const float		GeometricFit::Default::NEAR_RADIUS  = 1.8;
const float		GeometricFit::Default::GRID_SPACING  = 1.0;
const int 		GeometricFit::Default::GRID_SIZE  = 128;
const float		GeometricFit::Default::SURFACE_THICKNESS  = 1.0;
const int		  GeometricFit::Default::SURFACE_TYPE  = GeometricFit::CONNOLLY;
const double	GeometricFit::Default::DEGREE_INTERVAL  = 20.0;
const int 		GeometricFit::Default::TOP_N  = 3;
const int 		GeometricFit::Default::BEST_NUM  = 2000;
const int     GeometricFit::Default::VERBOSITY = 0;
const float 	GeometricFit::Default::PHI_MIN = 0.0;
const float 	GeometricFit::Default::PHI_MAX = 360.0;
const float 	GeometricFit::Default::DEG_PHI = 20.0;
const float 	GeometricFit::Default::THETA_MIN = 0.0;
const float 	GeometricFit::Default::THETA_MAX = 360.0;
const float 	GeometricFit::Default::DEG_THETA = 20.0;
const float 	GeometricFit::Default::PSI_MIN = 0.0;
const float 	GeometricFit::Default::PSI_MAX = 180.0;
const float 	GeometricFit::Default::DEG_PSI = 20.0;
const int			GeometricFit::Default::PENALTY_STATIC = -15;
const int			GeometricFit::Default::PENALTY_MOBILE = 1;
const int			GeometricFit::Default::NUMBER_OF_PROCESSES = 1;
const String	GeometricFit::Default::SLAVE_PATH = "";


GeometricFit::GeometricFit()
	: FFT_grid_a_(0),
	  FFT_grid_b_(0)
{
	options.setDefaultReal(Option::NEAR_RADIUS, Default::NEAR_RADIUS);
	options.setDefaultReal(Option::GRID_SPACING, Default::GRID_SPACING);
	options.setDefaultInteger(Option::GRID_SIZE, Default::GRID_SIZE);
	options.setDefaultReal(Option::SURFACE_THICKNESS, Default::SURFACE_THICKNESS);
	options.setDefaultInteger(Option::SURFACE_TYPE, Default::SURFACE_TYPE);
	options.setDefaultReal(Option::DEGREE_INTERVAL, Default::DEGREE_INTERVAL);
	options.setDefaultInteger(Option::TOP_N, Default::TOP_N);
	options.setDefaultInteger(Option::BEST_NUM, Default::BEST_NUM);
	options.setDefaultInteger(Option::VERBOSITY, Default::VERBOSITY);
	options.setDefaultReal(Option::PHI_MIN, Default::PHI_MIN);
	options.setDefaultReal(Option::PHI_MAX, Default::PHI_MAX);
	options.setDefaultReal(Option::DEG_PHI, Default::DEG_PHI);
	options.setDefaultReal(Option::THETA_MIN, Default::THETA_MIN);
	options.setDefaultReal(Option::THETA_MAX, Default::THETA_MAX);
	options.setDefaultReal(Option::DEG_THETA, Default::DEG_THETA);
	options.setDefaultReal(Option::PSI_MIN, Default::PSI_MIN);
	options.setDefaultReal(Option::PSI_MAX, Default::PSI_MAX);
	options.setDefaultReal(Option::DEG_PSI, Default::DEG_PSI);
	options.setDefaultInteger(Option::PENALTY_STATIC, Default::PENALTY_STATIC);
	options.setDefaultInteger(Option::PENALTY_MOBILE, Default::PENALTY_MOBILE);
	options.setDefaultInteger(Option::NUMBER_OF_PROCESSES, Default::NUMBER_OF_PROCESSES);
	options.setDefault(Option::SLAVE_PATH, Default::SLAVE_PATH);

	radius_a_ = 0.0;
	radius_b_ = 0.0;
	current_round_ = 0;
	total_round_   = 0;
}

/** Constructor.
	Creates an instance of Geometric Fit and calls
	setup(system1, system2).
	The options used are the default options.
*/
GeometricFit::GeometricFit(System &system1,System &system2)
	: FFT_grid_a_(0),
	  FFT_grid_b_(0)
{
	options.setDefaultReal(Option::NEAR_RADIUS, Default::NEAR_RADIUS);
	options.setDefaultReal(Option::GRID_SPACING, Default::GRID_SPACING);
	options.setDefaultInteger(Option::GRID_SIZE, Default::GRID_SIZE);
	options.setDefaultReal(Option::SURFACE_THICKNESS, Default::SURFACE_THICKNESS);
	options.setDefaultInteger(Option::SURFACE_TYPE, Default::SURFACE_TYPE);
	options.setDefaultReal(Option::DEGREE_INTERVAL, Default::DEGREE_INTERVAL);
	options.setDefaultInteger(Option::TOP_N, Default::TOP_N);
	options.setDefaultInteger(Option::BEST_NUM, Default::BEST_NUM);
	options.setDefaultInteger(Option::VERBOSITY, Default::VERBOSITY);
	options.setDefaultReal(Option::PHI_MIN, Default::PHI_MIN);
	options.setDefaultReal(Option::PHI_MAX, Default::PHI_MAX);
	options.setDefaultReal(Option::DEG_PHI, Default::DEG_PHI);
	options.setDefaultReal(Option::THETA_MIN, Default::THETA_MIN);
	options.setDefaultReal(Option::THETA_MAX, Default::THETA_MAX);
	options.setDefaultReal(Option::DEG_THETA, Default::DEG_THETA);
	options.setDefaultReal(Option::PSI_MIN, Default::PSI_MIN);
	options.setDefaultReal(Option::PSI_MAX, Default::PSI_MAX);
	options.setDefaultReal(Option::DEG_PSI, Default::DEG_PSI);
	options.setDefaultInteger(Option::PENALTY_STATIC, Default::PENALTY_STATIC);
	options.setDefaultInteger(Option::PENALTY_MOBILE, Default::PENALTY_MOBILE);
	options.setDefaultInteger(Option::NUMBER_OF_PROCESSES, Default::NUMBER_OF_PROCESSES);
	options.setDefault(Option::SLAVE_PATH, Default::SLAVE_PATH);

	setup(system1, system2);
}

/** Constructor.
	Creates an instance of Geometric Fit and assigns
	the given options to the Geometric Fit object's options.
*/
GeometricFit::GeometricFit(Options& new_options)
	: FFT_grid_a_(0),
	  FFT_grid_b_(0)
{
	options = new_options;
}


/** Constructor.
	Creates an instance of FDPB and calls
	setup(system1, system2, options)
*/
GeometricFit::GeometricFit(System &system1, System &system2, Options& new_options)
{
	setup(system1, system2, new_options);
}


GeometricFit::GeometricFit( const GeometricFit& geo_fit )
	: DockingAlgorithm(*geo_fit.system1_, *geo_fit.system2_)
{
	FFT_grid_a_ = geo_fit.FFT_grid_a_;
	FFT_grid_b_ = geo_fit.FFT_grid_b_;

	options = geo_fit.options;
	setup(*geo_fit.system1_, *geo_fit.system2_);
}


GeometricFit::~GeometricFit()
{
	destroy_();
}


void GeometricFit::setup(System& system1, System& system2, Options& new_options)
{
	radius_a_ = 0.0;
	radius_b_ = 0.0;
	current_round_ = 0;
	total_round_   = 0;
	system1_ = &system1;
	system2_ = &system2;
	options  = new_options;
}

void GeometricFit::setup(System& system1, System& system2)
{
	radius_a_ = 0.0;
	radius_b_ = 0.0;
	current_round_ = 0;
	total_round_   = 0;
	system1_ = &system1;
	system2_ = &system2;
}

/** Compute the center of mass of system
 */
Vector3 GeometricFit::getMassCenter_( System& system )
{
	int atom_num = 0;
	Vector3 mass_center = Vector3(0., 0., 0.);

	for( AtomIterator atom_it = system.beginAtom(); +atom_it; ++atom_it )
	{
		atom_num ++;
		mass_center += atom_it->getPosition();
	}

	mass_center.x = (float)mass_center.x / atom_num;
	mass_center.y = (float)mass_center.y / atom_num;
	mass_center.z = (float)mass_center.z / atom_num;

	return mass_center;
}

/** Compute the radius of the circumsphere of all atoms in system.
 */
float GeometricFit::getRadius_( System& system )
{
	Vector3 mc = getMassCenter_(system);

	float radius = 0.0;

	for( AtomIterator atom_it = system.beginAtom(); +atom_it; ++atom_it )
	{
		float dist = sqrt( mc.getSquareDistance( atom_it->getPosition() ) );
		float r    = atom_it->getRadius();  // radius of this atom

		if( (dist + r) > radius )
		{
			radius = dist + r;
		}
	}

	return radius;
}

void GeometricFit::doPreTranslation_( ProteinIndex pro_idx )
{
	System* system = ( pro_idx == PROTEIN_A ) ? system1_ : system2_;
	Vector3 mc = getMassCenter_( *system );

	// pre-translation: to move the protein mass center to origin point.
	TranslationProcessor tp( -mc );
	system->apply( tp );

	if( pro_idx == PROTEIN_A )
	{
		pre_translation_a_ = -mc;
	}
	else
	{
		pre_translation_b_ = -mc;
	}

	return;
}

	/** Optimize grid size for the FFTW.
	 *	 Certain combinations of certain powers are optimal for the fft algorithm, and
	 *	 this function tries to determine the smallest optimal combination large enough
	 *	 to accomodate the original data.
	 */
  int GeometricFit::optimizeGridSize_( int raw_size )
  {
    list<int> size_list;

    // grid size = 1500 is big enough
    for(int d = 0; d < 4; d++)     // 7^3 = 343
      for(int c = 0; c < 5; c++)   // 5^4 = 625
      {
				int tmp1 = (int)pow((float)5.0, (float)c) * (int)pow((float)7.0, (float)d);

				if(tmp1 > 1500)
					continue;

				for(int b = 0; b < 6; b++)     // 3^6 = 729
					for(int a = 0; a < 11; a++)  // 2^10 = 1024
					{
						int tmp2 = (int)pow((float)2, (float)a) * (int)pow((float)3, (float)b);

						if( tmp2 > 1500 )
							continue;

						int size = tmp1 * tmp2;

						if( (size >= 10) && (size <= 1500) )
						{
							size_list.push_back(size);
						}
					}  // for (b ...) for (a ...)
			}  // for (d ...) for (c ...)

		size_list.sort();

		int opt_size=0;

		// just iterate all elements to find out optimal size
    int ss = (int)*(size_list.begin());
    int bs = (int)*(--size_list.end());

    if( raw_size <= ss )
    {
      opt_size = ss;
    }
    else if( raw_size > bs )
    {
      opt_size = bs;
    }
    else
    {
      for(list<int>::iterator it = size_list.begin(); it!=size_list.end(); it++)
      {
				// todo: is this really necessary: ?
				list<int>::iterator temp_it = it;
				temp_it++;

				if( (raw_size <= (int)*(temp_it)) && (raw_size > (int)*(it)) )
				{
					opt_size = (int)*(temp_it);
					break;
				}
			} // for
    } // else

    return opt_size;
  }

  void GeometricFit::initGridSizes_()
  {
		float grid_spacing = options.getReal(Option::GRID_SPACING);

    float r1 = getRadius_( *system1_ );
    float r2 = getRadius_( *system2_ );

    radius_a_ = r1;
    radius_b_ = r2;

    // enlarge the grid a little (2 spacing larger)
		//float d = (r1 + r2) * 2 + 2 * grid_spacing;
		float d = (r1 + r2) * 2 + 1;

    // number of grid point = number of units + 1
		int grid_size = (int)ceil( d / grid_spacing ) + 1;

    // get the optimal number for grid size,
    // which can be handled by FFTW effciently
		// TODO: this should really _not_ be an option of the class, but rather
		// 			 a protected member
    options.setInteger(Option::GRID_SIZE, optimizeGridSize_( grid_size ));

		// TODO: configurable verbosity

		if (options.getInteger(Option::VERBOSITY) > 1)
			Log << "GRID_SIZE = " << options.getInteger(Option::GRID_SIZE) << endl;

    return;
  }

  /** Initialize the grid.
	 */
  void GeometricFit::initFFTGrid_( ProteinIndex pro_idx )
  {
    float r;
    int   r_idx;

		int grid_size = options.getInteger(Option::GRID_SIZE);
		float grid_spacing = options.getReal(Option::GRID_SPACING);

    if(grid_size % 2 == 0)
    {
      r     = grid_size * grid_spacing / 2.0; // Angstrom
      r_idx = grid_size / 2; // index
    }
    else
    {
      r     = (grid_size - 1.0) * grid_spacing / 2.0; // Angstrom
      r_idx = (grid_size - 1) / 2; // index
    }

    FFT_grid_origin_ = Vector3(r, r, r); // in unit of Angstroms

    // index
    FFT_grid_lower_index_ = -r_idx;
    FFT_grid_upper_index_ = grid_size - 1 - r_idx;

    // Angstrom
		FFT_grid_lower_coord_ = FFT_grid_lower_index_ * grid_spacing;
    FFT_grid_upper_coord_ = FFT_grid_upper_index_ * grid_spacing;

    // index
    FFT_grid_size_index_  = Vector3( grid_size, grid_size, grid_size );

    if( pro_idx == PROTEIN_A )  // for the static protein
    {
      FFT_grid_a_ =  new FFT3D( grid_size, grid_size, grid_size,
																grid_spacing, grid_spacing, grid_spacing,
																FFT_grid_origin_,          // grid origin
																false                      // in physical space
															);
    }
    else // ( pro_idx == PROTEIN_B )  // for the mobile protein
    {
      FFT_grid_b_ =  new FFT3D( grid_size, grid_size, grid_size,
																grid_spacing, grid_spacing, grid_spacing,
																FFT_grid_origin_,          // grid origin
																false                      // in physical space
												);
    }

    return;
  }

  // find the inside points
  void GeometricFit::findInsidePoints_( System& system, ProteinIndex pro_idx )
  {
		int number_of_points = 0;
		int verbosity = options.getInteger(Option::VERBOSITY);

		if (verbosity > 15)
			Log << "looking for inside points now ..." << endl;

    FFT3D*  FFT_grid = NULL;

		// penalty value of the inside points
    int PENALTY;

    if ( pro_idx == PROTEIN_A )
    {
      FFT_grid = FFT_grid_a_;
      PENALTY = options.getInteger(Option::PENALTY_STATIC);   // penalty for inside points of protein A (static)
    }
    else // ( pro_idx == PROTEIN_B )
    {
      FFT_grid = FFT_grid_b_;
      PENALTY = options.getInteger(Option::PENALTY_MOBILE);     // penalty for inside points of protein B (mobile)
    }

		double near_radius  = options.getReal(Option::NEAR_RADIUS);
		double grid_spacing = options.getReal(Option::GRID_SPACING);

    const double RADIUS_SQR = near_radius * near_radius; // Angstrom
		Vector3 atom_position;  // Angstrom

    // find all the inside point in grid
    for ( AtomIterator atom_it = system.beginAtom(); +atom_it; ++atom_it )
    {
      atom_position = atom_it->getPosition();  // Angstrom

      // what is the dimension we have to check for each atom
      // in the units of GRID_SPACING (index)
      int i, j, k;

	int lower_loop_bound_x = (int) ceil((atom_position.x - near_radius) / grid_spacing);
      int upper_loop_bound_x = (int)floor((atom_position.x + near_radius) / grid_spacing);
      int lower_loop_bound_y = (int) ceil((atom_position.y - near_radius) / grid_spacing);
      int upper_loop_bound_y = (int)floor((atom_position.y + near_radius) / grid_spacing);
      int lower_loop_bound_z = (int) ceil((atom_position.z - near_radius) / grid_spacing);
      int upper_loop_bound_z = (int)floor((atom_position.z + near_radius) / grid_spacing);

  // check whether the loop points are out of the global grid bound
      if ( lower_loop_bound_x < FFT_grid_lower_index_.x ) { lower_loop_bound_x = (int)FFT_grid_lower_index_.x; }
      if ( upper_loop_bound_x > FFT_grid_upper_index_.x ) { upper_loop_bound_x = (int)FFT_grid_upper_index_.x; }
      if ( lower_loop_bound_y < FFT_grid_lower_index_.y ) { lower_loop_bound_y = (int)FFT_grid_lower_index_.y; }
      if ( upper_loop_bound_y > FFT_grid_upper_index_.y ) { upper_loop_bound_y = (int)FFT_grid_upper_index_.y; }
      if ( lower_loop_bound_z < FFT_grid_lower_index_.z ) { lower_loop_bound_z = (int)FFT_grid_lower_index_.z; }
      if ( upper_loop_bound_z > FFT_grid_upper_index_.z ) { upper_loop_bound_z = (int)FFT_grid_upper_index_.z; }

      // iterate all the grids around the atom
      // in the units of grid points (index)
      Vector3 pos;

      for ( i = lower_loop_bound_x; i <= upper_loop_bound_x; i++ )
				for ( j = lower_loop_bound_y; j <= upper_loop_bound_y; j++ )
					for ( k = lower_loop_bound_z; k <= upper_loop_bound_z; k++ )
					{
						// set the position we want to check, in the units of Angstrom
						pos.set( i * grid_spacing, j * grid_spacing, k * grid_spacing );

						if( ( (*FFT_grid)[pos].real() == 0.) &&
								( pos.getSquareDistance(atom_position) <= RADIUS_SQR ) )
						{
							FFT_grid->setData(pos, Complex(PENALTY, 0.));

							number_of_points++;
						}
					}

		}   // for( atom_it = ...

		if (verbosity > 15)
		{
			Log << "Number of inside points: " << number_of_points << std::endl;
		}

    return;
  }

	// find out the surface points according to the Connolly's surface definition.
  void GeometricFit::findConnollySurfacePoints_( System& system, ProteinIndex pro_idx )
  {
		int verbosity = options.getInteger(Option::VERBOSITY);

		int number_of_points = 0;
    FFT3D* FFT_grid = NULL;

    if ( pro_idx == PROTEIN_A )
    {
      FFT_grid = FFT_grid_a_;
    }
    else // ( pro_idx == PROTEIN_B )
    {
      FFT_grid = FFT_grid_b_;
    }

    // find out Connolly surface
    // this will cost about 30 seconds
    SurfaceProcessor sp;

		// TODO: make this configurable as an Option of the class
		sp.setProbeRadius(1.4);

    system.apply( sp );
    Surface surface = sp.getSurface();

    // find out grid points belong to Connolly surface
		float half_thickness     = options.getReal(Option::SURFACE_THICKNESS) / 2.;
    float half_thickness_sqr = half_thickness * half_thickness;

    Vector3 v;       // vertices on the found Connolly surface

    int vtx_num = surface.getNumberOfVertices();

		if (verbosity > 1)
			Log.info() << "vtx_num = " << vtx_num << std::endl;

		float grid_spacing = options.getReal(Option::GRID_SPACING);

    for(int n = 0; n < vtx_num; n++)
    {
      v = surface.getVertex(n);

      // what is the dimension we have to check for each atom
      // in the unit of grid points (index)
	int lower_loop_bound_x = (int)floor((v.x - half_thickness) / grid_spacing);
      int upper_loop_bound_x = (int) ceil((v.x + half_thickness) / grid_spacing);
      int lower_loop_bound_y = (int)floor((v.y - half_thickness) / grid_spacing);
      int upper_loop_bound_y = (int) ceil((v.y + half_thickness) / grid_spacing);
      int lower_loop_bound_z = (int)floor((v.z - half_thickness) / grid_spacing);
      int upper_loop_bound_z = (int) ceil((v.z + half_thickness) / grid_spacing);

      // check whether the loop points are out of the global grid bound
      if ( lower_loop_bound_x < FFT_grid_lower_index_.x ) { lower_loop_bound_x = (int)FFT_grid_lower_index_.x; }
      if ( upper_loop_bound_x > FFT_grid_upper_index_.x ) { upper_loop_bound_x = (int)FFT_grid_upper_index_.x; }
      if ( lower_loop_bound_y < FFT_grid_lower_index_.y ) { lower_loop_bound_y = (int)FFT_grid_lower_index_.y; }
      if ( upper_loop_bound_y > FFT_grid_upper_index_.y ) { upper_loop_bound_y = (int)FFT_grid_upper_index_.y; }
      if ( lower_loop_bound_z < FFT_grid_lower_index_.z ) { lower_loop_bound_z = (int)FFT_grid_lower_index_.z; }
      if ( upper_loop_bound_z > FFT_grid_upper_index_.z ) { upper_loop_bound_z = (int)FFT_grid_upper_index_.z; }

      Vector3 pos;

      for (int i = lower_loop_bound_x; i <= upper_loop_bound_x; i++ )
				for (int j = lower_loop_bound_y; j <= upper_loop_bound_y; j++ )
					for (int k = lower_loop_bound_z; k <= upper_loop_bound_z; k++ )
					{
						// Angstrom
						pos.set(i * grid_spacing, j * grid_spacing, k * grid_spacing);

						if( ( (*FFT_grid)[pos].real() != 1 ) &&
								( pos.getSquareDistance(v) <= half_thickness_sqr ) )
						{
							FFT_grid->setData(pos, Complex(1.0, 0.0));

							number_of_points++;
						}
					}
		} // for ( all vertices )

		if (verbosity > 1)
			Log.info() << "findConnollySurfacePoints_ : " << number_of_points << std::endl;

    return;
  }

	// find out the surface points according to the van der Waal's surface definition.
  void GeometricFit::findVanDerWaalsSurfacePoints_( System& system, ProteinIndex pro_idx )
  {
		int number_of_points = 0;
    FFT3D* FFT_grid;

    if ( pro_idx == PROTEIN_A )
    {
      FFT_grid = FFT_grid_a_;
    }
    else // ( pro_idx == PROTEIN_B )
    {
      FFT_grid = FFT_grid_b_;
    }

		float grid_spacing 				 = options.getReal(Option::GRID_SPACING);

		float half_thickness       = options.getReal(Option::SURFACE_THICKNESS) / 2.0;

    Vector3 atom_position;
    float   atom_radius;
    float   max_radius, min_radius;

    // find all the inside point in grid
    for ( AtomIterator atom_it = system.beginAtom(); +atom_it; ++atom_it )
    {
      atom_position = atom_it->getPosition();
      atom_radius   = atom_it->getRadius();
      max_radius    = atom_radius + half_thickness;
      min_radius    = atom_radius - half_thickness;
      if(min_radius < 0)
      {
				min_radius = 0;
      }

      // what is the dimension we have to check for each atom
      // in the units of GRID_SPACING
      int i, j, k;

			int lower_loop_bound_x = (int) ceil((atom_position.x - max_radius) / grid_spacing);
      int upper_loop_bound_x = (int)floor((atom_position.x + max_radius) / grid_spacing);
      int lower_loop_bound_y = (int) ceil((atom_position.y - max_radius) / grid_spacing);
      int upper_loop_bound_y = (int)floor((atom_position.y + max_radius) / grid_spacing);
      int lower_loop_bound_z = (int) ceil((atom_position.z - max_radius) / grid_spacing);
      int upper_loop_bound_z = (int)floor((atom_position.z + max_radius) / grid_spacing);

      // check whether the loop points are out of the global grid bound
      if ( lower_loop_bound_x < FFT_grid_lower_index_.x ) { lower_loop_bound_x = (int)FFT_grid_lower_index_.x; }
      if ( upper_loop_bound_x > FFT_grid_upper_index_.x ) { upper_loop_bound_x = (int)FFT_grid_upper_index_.x; }
      if ( lower_loop_bound_y < FFT_grid_lower_index_.y ) { lower_loop_bound_y = (int)FFT_grid_lower_index_.y; }
      if ( upper_loop_bound_y > FFT_grid_upper_index_.y ) { upper_loop_bound_y = (int)FFT_grid_upper_index_.y; }
      if ( lower_loop_bound_z < FFT_grid_lower_index_.z ) { lower_loop_bound_z = (int)FFT_grid_lower_index_.z; }
      if ( upper_loop_bound_z > FFT_grid_upper_index_.z ) { upper_loop_bound_z = (int)FFT_grid_upper_index_.z; }

      // iterate all the grids around the atom
      // in the units of grid points
      Vector3 pos;

      for ( i = lower_loop_bound_x; i <= upper_loop_bound_x; i++ )
				for ( j = lower_loop_bound_y; j <= upper_loop_bound_y; j++ )
					for ( k = lower_loop_bound_z; k <= upper_loop_bound_z; k++ )
					{
						pos.set( i * grid_spacing, j * grid_spacing, k * grid_spacing );


						if( ( (*FFT_grid)[pos].real() == 0 ) &&          // if this grid is still outside grid
								( pos.getSquareDistance(atom_position) <= max_radius * max_radius ) &&
								( pos.getSquareDistance(atom_position) >= min_radius * min_radius ) )
						{
							FFT_grid->setData(pos, Complex(1.0, 0.0));

							number_of_points++;
						}
					}

		}   // for( atom_it = ...

		if (options.getInteger(Option::VERBOSITY) > 1)
			Log.info() << "findVanDerWaalSurfacePoints_ : " << number_of_points << std::endl;

    return;
  }

	// use the same algorithm FTDock uses to find the inside points
	void GeometricFit::findFTDockInsidePoints_(System& system, ProteinIndex pro_idx)
	{
		int number_of_points = 0;
		int verbosity = options.getInteger(Option::VERBOSITY);

		if (verbosity > 15)
			Log << "looking for inside points now (FTDock-algorithm)..." << endl;

		FFT3D* FFT_grid = NULL;

    if ( pro_idx == PROTEIN_A )
    {
      FFT_grid = FFT_grid_a_;
    }
    else // ( pro_idx == PROTEIN_B )
    {
      FFT_grid = FFT_grid_b_;
    }

		Complex penalty(1., 0.);

		double near_radius  = options.getReal(Option::NEAR_RADIUS);
		double grid_spacing = options.getReal(Option::GRID_SPACING);

		int    grid_size    = options.getInteger(Option::GRID_SIZE);

    const double RADIUS_SQR = near_radius * near_radius; // Angstrom

		int points_to_check	= (int)((near_radius/grid_spacing)+1.5);

		FFT3D::IndexType grid_index;
		Vector3 atom_position;
		Vector3 atom_index;
		Vector3 grid_position;

    // find all the inside point in grid
    for ( AtomIterator atom_it = system.beginAtom(); +atom_it; ++atom_it )
    {
      atom_position = atom_it->getPosition();
			atom_index    = (atom_position + FFT_grid_origin_)/grid_spacing;
//			std::cout << atom_index << std::endl;

			for (grid_index.x  = (Position)BALL_MAX((int)Maths::rint(atom_index.x) - points_to_check, 0);
					 grid_index.x <= (Position)BALL_MIN((int)Maths::rint(atom_index.x) + points_to_check, grid_size - 1);
					 grid_index.x++)
			{
				for (grid_index.y  = (Position)BALL_MAX((int)Maths::rint(atom_index.y) - points_to_check, 0);
						 grid_index.y <= (Position)BALL_MIN((int)Maths::rint(atom_index.y) + points_to_check, grid_size - 1);
						 grid_index.y++)
				{
					for (grid_index.z  = (Position)BALL_MAX((int)Maths::rint(atom_index.z) - points_to_check, 0);
							 grid_index.z <= (Position)BALL_MIN((int)Maths::rint(atom_index.z) + points_to_check, grid_size - 1);
							 grid_index.z++)
					{
						// That can be done faster...
						int index = grid_index.z + (grid_index.y + grid_index.x*grid_size)*grid_size;
//						std::cout << "Index: " << index << " ptc " << points_to_check << std::endl;
						Complex& data = (*FFT_grid)[index];
						if ((data.real() == 0.) &&
								(atom_position.getSquareDistance(FFT_grid->getGridCoordinates(index)) <= RADIUS_SQR))
						{
							data = penalty;
							number_of_points++;
						}
					}
				}
			}
		}

		if (verbosity > 15)
		{
			Log << "Number of inside points: " << number_of_points << std::endl;
		}

    return;
	}

	// compute surface points according to FTDock's definition
	void GeometricFit::findFTDockSurfacePoints_(System& /*system*/, ProteinIndex pro_idx)
	{
		int verbosity = options.getInteger(Option::VERBOSITY);

		int number_of_points = 0;
    FFT3D* FFT_grid = NULL;
		int PENALTY;

    if ( pro_idx == PROTEIN_A )
    {
      FFT_grid = FFT_grid_a_;
      PENALTY = options.getInteger(Option::PENALTY_STATIC);   // penalty for inside points of protein A (static)
    }
    else // ( pro_idx == PROTEIN_B )
    {
      FFT_grid = FFT_grid_b_;
      PENALTY = options.getInteger(Option::PENALTY_MOBILE);   // penalty for inside points of protein B (mobile)
    }

		Complex penalty(PENALTY, 0.);
		float  surface_thickness = options.getReal(Option::SURFACE_THICKNESS);
		double grid_spacing      = options.getReal(Option::GRID_SPACING);
		int    grid_size         = options.getInteger(Option::GRID_SIZE);

		int points_to_check = (int) ((surface_thickness/grid_spacing) + 1.5);

		// iterate over the complete grid
		int x, y, z, index;
		int x2, y2, z2, index2;
		float SQR_SPACING   = grid_spacing*grid_spacing;
		float SQR_THICKNESS = surface_thickness*surface_thickness;

		for (x=0; x<grid_size; x++)
		{
			for (y=0; y<grid_size; y++)
			{
				for (z=0; z<grid_size; z++)
				{
					index = z + (y + x*grid_size)*grid_size;
					Complex& data = (*FFT_grid)[index];

					if (data.real() == 1) // this point was classified as inside
					{
						bool is_surface_point = false;

						// iterate over the surrounding points
						for (x2=max(x-points_to_check, 0); !is_surface_point&&(x2<=min(x+points_to_check, grid_size-1)); x2++)
						{
							for (y2=max(y-points_to_check, 0); !is_surface_point&&(y2<=min(y+points_to_check, grid_size-1)); y2++)
							{
								for (z2=max(z-points_to_check, 0); !is_surface_point&&(z2<=min(z+points_to_check, grid_size-1)); z2++)
								{
									index2 = z2+(y2+x2*grid_size)*grid_size;

									// extend the surface to the outside only
									if ((*FFT_grid)[index2].real() == 0)
									{
										// is the point inside the surface thickness?
										if ( ((x-x2)*(x-x2) + (y-y2)*(y-y2) + (z-z2)*(z-z2)) * SQR_SPACING < SQR_THICKNESS )
											is_surface_point = true;
									}
								}
							}
						}

						if (!is_surface_point)
							data = penalty;
						else
							number_of_points++;
					}
				}
			}
		}

		if (verbosity > 1)
			Log.info() << "findFTDockSurfacePoints_ : " << number_of_points << std::endl;

		return;
 }

  // make grid from System
  void GeometricFit::makeFFTGrid_( ProteinIndex pro_idx )
  {
		int surface_type = options.getInteger(Option::SURFACE_TYPE);
		System* system = ( pro_idx == PROTEIN_A ) ? system1_ : system2_;

    // init grid value
    if( pro_idx == PROTEIN_A )
    {
      if( FFT_grid_a_ == NULL )
      {
			throw Exception::NullPointer(__FILE__, __LINE__);
      }

      Complex* grid = &( (*FFT_grid_a_)[0] );
      Complex* end  = &( (*FFT_grid_a_)[FFT_grid_a_->size() - 1] );
      end++; // it's alread out of grid, we just use it as end-mark
      for(; grid != end; grid++)
      {
				*grid = Complex(0.0,0.0);
      }
    }
    else // if ( pro_idx == PROTEIN_B )
    {
      if ( FFT_grid_b_ == NULL )
      {
			throw Exception::NullPointer(__FILE__, __LINE__);
      }

      // since we are re-using FFT_grid_b_,
      // so we have to set number of FFT & iFFT transformations
      FFT_grid_b_->setNumberOfFFTTransforms(0);
      FFT_grid_b_->setNumberOfiFFTTransforms(0);

      Complex* grid = &( (*FFT_grid_b_)[0] );
      Complex* end  = &( (*FFT_grid_b_)[FFT_grid_b_->size() - 1] );
      end++; // it's alread out of grid, we just use it as end-mark
      for(; grid != end; grid++)
      {
				*grid = Complex(0.0,0.0);
      }
    }

		if (surface_type == FTDOCK)
			findFTDockInsidePoints_( *system, pro_idx );
		else
			findInsidePoints_( *system, pro_idx );

    // Since the surface points have the same value with inside points in protein b,
    // so we only search for the surface points for protein a
    if(pro_idx == PROTEIN_A)
    {
      if( surface_type == CONNOLLY )
      {
				findConnollySurfacePoints_( *system, pro_idx );
      }
      else if( surface_type == VAN_DER_WAALS )
      {
				findVanDerWaalsSurfacePoints_( *system, pro_idx );
      }
      else if( surface_type == FTDOCK )
      {
				findFTDockSurfacePoints_( *system, pro_idx );
      }
      else
      {}
    }

		return;
  }

  // Free all allocated memory and destroys the options and results
  void GeometricFit::destroy_()
  {
    // free grid memory
    // we have only two grids to free
    if(FFT_grid_a_ != NULL)
    {
      delete FFT_grid_a_;
      FFT_grid_a_ = NULL;
    }

    if(FFT_grid_b_ != NULL)
    {
      delete FFT_grid_b_;
      FFT_grid_b_ = NULL;
    }

    return;
  }

  // change the orientation of protein around its center according to euler_ang
  void GeometricFit::changeProteinOrientation_( System& mobile_system, Vector3 euler_ang )
  {
    // three euler angles, around axis x,y,z separately
    double phi   = euler_ang.x * Constants::PI / 180.0;
    double theta = euler_ang.y * Constants::PI / 180.0;
    double psi   = euler_ang.z * Constants::PI / 180.0;
    double sphi, stheta, spsi, cphi, ctheta, cpsi;
    sphi   = sin( phi   );
    cphi   = cos( phi   );
    stheta = sin( theta );
    ctheta = cos( theta );
    spsi   = sin( psi   );
    cpsi   = cos( psi   );

    // in the matrix, mat = Rz * Ry * Rx
    Matrix4x4 mat;
    mat.m11 =         ctheta * cpsi;
    mat.m12 =  sphi * stheta * cpsi - cphi * spsi;
    mat.m13 =  cphi * stheta * cpsi + sphi * spsi;
    mat.m14 =  0.0;
    mat.m21 =         ctheta * spsi;
    mat.m22 =  sphi * stheta * spsi + cphi * cpsi;
    mat.m23 =  cphi * stheta * spsi - sphi * cpsi;
    mat.m24 =  0.0;
    mat.m31 =       - stheta;
    mat.m32 =  sphi * ctheta;
    mat.m33 =  cphi * ctheta;
    mat.m34 =  0.0;
    mat.m41 =  0.0;
    mat.m42 =  0.0;
    mat.m43 =  0.0;
    mat.m44 =  1.0;

    TransformationProcessor transformation( mat );
    mobile_system.apply( transformation );

    return;
  }

  // calculate the conjugate of each point in FFT grid
	// TODO: maybe a rewrite would make that readable
  void GeometricFit::calcConjugate_( ProteinIndex pro_idx )
  {
    FFT3D* FFT_grid = NULL;

    if( pro_idx == PROTEIN_A )
    {
      FFT_grid = (FFT3D*)FFT_grid_a_;
    }
    else
    {
      FFT_grid = (FFT3D*)FFT_grid_b_;
    }

    Complex* grid = &( (*FFT_grid)[0] );
    Complex* end  = &( (*FFT_grid)[FFT_grid->size() - 1] );
    end++; // it's alread out of grid, we just use it as end-mark
    for(; grid != end; grid++)
    {
      *grid = Complex((*grid).real(),(*grid).imag()*-1.);
    }

    return;
  }

  // calculate the product of the two FFT grids
  void GeometricFit::FFTGridMulti_()
  {
    // use pointers to do iterations
    Complex* grid_a = &( (*FFT_grid_a_)[0] );
    Complex* grid_b = &( (*FFT_grid_b_)[0] );
    Complex* end_a  = &( (*FFT_grid_a_)[FFT_grid_a_->size() - 1] );
    end_a++;  // it's alread out of grid, we just use it as end-mark

    for(; grid_a != end_a; grid_a++, grid_b++)
    {
			*grid_b = (*grid_a)*(*grid_b);
    }

    return;
  }

  void GeometricFit::getGlobalPeak_(Peak_ *peak_list)
  {
    if( FFT_grid_b_ == 0 )
    {
      Log.error() << "FFT_grid_b_ does not exist!" << endl;
    }

		int top_n = options.getInteger(Option::TOP_N);

		vector<float> peaks(top_n);  // peak values
    vector<int>   index(top_n);  // peak indices

    double re;

    Complex* grid = &( (*FFT_grid_b_)[0] );
    Complex* end  = &( (*FFT_grid_b_)[FFT_grid_b_->size() - 1] );
    end++; // it's alread out of grid, we just use it as end-mark
    int index_counter = 0;
    for(; grid != end; grid++, index_counter++)
    {
      if((*grid).real() < 0.0)
				continue;

      re = (*grid).real();

      if(re > peaks[0])
      {
				int array_pos = 1;

				for(; (array_pos < top_n) && (re > peaks[array_pos]); array_pos ++)
				{
					peaks[array_pos-1] = peaks[array_pos];
					index[array_pos-1] = index[array_pos];
				}

				peaks[array_pos-1] = re;
				index[array_pos-1] = index_counter;
			} // if new peak is larger than the smallest element in list

		} // for all grid values

		int grid_size = options.getInteger(Option::GRID_SIZE);

		for(int i = 0; i < top_n; i++)
    {
      peak_list[i].value = peaks[i] / (double) (grid_size*grid_size*grid_size);

      // get peak position from index (Angstrom)
      peak_list[i].translation = FFT_grid_b_->getGridCoordinates( index[i] );
    }

    return;
  }

  // get the translation between the two proteins after doing
  // pre-translation of the two proteins.
  Vector3 GeometricFit::getSeparation_( const Vector3& mat_pos )
  {
    Vector3 trans = mat_pos + FFT_grid_origin_; // all in angstroms

		int   grid_size    = options.getInteger(Option::GRID_SIZE);
		float grid_spacing = options.getReal(Option::GRID_SPACING);

		float FFT_grid_size = (grid_size - 1) * grid_spacing;

		if(trans.x > radius_a_ + radius_b_ + grid_spacing )
      trans.x -= FFT_grid_size;
    if(trans.y > radius_a_ + radius_b_ + grid_spacing )
      trans.y -= FFT_grid_size;
    if(trans.z > radius_a_ + radius_b_ + grid_spacing )
      trans.z -= FFT_grid_size;

    return -trans;
  }

  // get the translaion between the two >>ORIGINAL<< proteins
  // "original" means the "input" proteins
  Vector3 GeometricFit::getTranslation_( const Vector3& mat_pos )
  {
    Vector3 trans = mat_pos + FFT_grid_origin_; // all in angstroms

		int   grid_size    = options.getInteger(Option::GRID_SIZE);
		float grid_spacing = options.getReal(Option::GRID_SPACING);

		float FFT_grid_size = (grid_size - 1) * grid_spacing;

		if(trans.x > radius_a_ + radius_b_ + grid_spacing )
      trans.x -= FFT_grid_size;
    if(trans.y > radius_a_ + radius_b_ + grid_spacing )
      trans.y -= FFT_grid_size;
    if(trans.z > radius_a_ + radius_b_ + grid_spacing )
      trans.z -= FFT_grid_size;

    return -trans - ( pre_translation_a_ - pre_translation_b_ );
  }


	/** Currently the main loop of the algorithm.
	 */
  void GeometricFit::start()
  {
		DockingAlgorithm::start();

		int verbosity = options.getInteger(Option::VERBOSITY);

	Timer overall_timer;
		Timer detailed_timer;

	overall_timer.start();
		detailed_timer.start();

	initGridSizes_();

		if (verbosity > 5)
		{
			Log << "Time used to init grid sizes for A: " << detailed_timer.getCPUTime() << endl;
		}
		detailed_timer.reset();

		initFFTGrid_( GeometricFit::PROTEIN_A );
		initFFTGrid_( GeometricFit::PROTEIN_B );

		if (verbosity > 5)
		{
			Log << "Time used to init FFT Grid A & B: " << detailed_timer.getCPUTime() << endl;
		}
		detailed_timer.reset();

		doPreTranslation_( GeometricFit::PROTEIN_A );

		detailed_timer.reset();
		makeFFTGrid_( GeometricFit::PROTEIN_A );

		if (verbosity > 5)
		{
	Log << "Time used to make FFT Grid A: " << detailed_timer.getCPUTime() << endl;
		}

		detailed_timer.reset();
		FFT_grid_a_->doFFT();

		if (verbosity > 5)
		{
	    Log << "Time used to do FFT on FFT Grid A: " << detailed_timer.getCPUTime() << endl;
		}

		detailed_timer.reset();
		calcConjugate_( GeometricFit::PROTEIN_A );

		if (verbosity > 5)
		{
	    Log << "Time used to calc conjugate on FFT Grid A: " << detailed_timer.getCPUTime() << endl;
		}

    // since we put the mass center at origin
    // we can do pre-translation of b before all loops.
    doPreTranslation_( GeometricFit::PROTEIN_B );

		// remove old rubbish in peak_set_
		peak_set_.clear();

		// note: the backuped systems are systems after pre-translation.
		system_backup_a_ = *system1_;  // after pre-translation
		system_backup_b_ = *system2_;  // after pre-translation

		// calculate all the rotation angles
		RotationAngles_ rotAng;

		if(     !rotAng.generateSomeAngles( (float)options.getReal(Option::DEG_PHI),
																			 (float)options.getReal(Option::DEG_PSI),
																			 (float)options.getReal(Option::DEG_THETA),
																			 (float)options.getReal(Option::PHI_MIN),
																			 (float)options.getReal(Option::PHI_MAX),
																			 (float)options.getReal(Option::PSI_MIN),
																			 (float)options.getReal(Option::PSI_MAX),
																			 (float)options.getReal(Option::THETA_MIN),
																			 (float)options.getReal(Option::THETA_MAX) )
				||  rotAng.getRotationNum() == 0 )
		{
			Log.error() << "Bad degree interval!" << endl;
			throw Exception::OutOfRange(__FILE__, __LINE__);
		}

		int rotation_num = rotAng.getRotationNum();
		total_round_ = rotAng.getRotationNum();

		if (verbosity > 1)
		{
		Log << "Number of rotations to compute: " << rotation_num << endl;
		}
    Timer loop_timer;
		loop_timer.start();

		std::vector<int> our_phi   = rotAng.phi_;
		std::vector<int> our_theta = rotAng.theta_;
		std::vector<int> our_psi   = rotAng.psi_;

/** This code distributes the data for the slaves in the case of a parallel computation **/
#ifdef BALL_HAS_MPI
		int numprocs = options.getInteger(Option::NUMBER_OF_PROCESSES);
    String slave_path = options[Option::SLAVE_PATH];

		MPISupport mpi;
		if (numprocs > 1 && slave_path != "")
		{
			int argc = 0;
			char **argv = 0;
			mpi.init(argc, argv);

			// spawn the desired number of processes
			numprocs = mpi.spawn(slave_path.c_str(), argv, numprocs) + 1;
			if (verbosity > 1)
				Log.info() << "Master process has spawned " << numprocs -1 << " slaves" << std::endl;

			// Distribute the options
			mpi.sendOptions(options);

			// and the systems
			mpi.sendSystem(*system1_);
			mpi.sendSystem(*system2_);

			// now scatter the angles.
			mpi.distributeDatapoints(rotAng.phi_,   our_phi);
			mpi.distributeDatapoints(rotAng.theta_, our_theta);
			mpi.distributeDatapoints(rotAng.psi_,   our_psi);

			rotation_num = our_phi.size();
		}
#endif

		// TODO: Use a vector
		Peak_* top_n_peaks = new Peak_[options.getInteger(Option::TOP_N)];
		double phi, theta, psi; // phi: around x axis,
														// theta: around y axis,
                            // psi: around z axis;
std::cout << "I have " << rotation_num << " rotations" << std::endl;
		for (current_round_ = 0; (current_round_ < rotation_num) && !abort_; current_round_++)
		{
			// TODO: we should check if pause_ is true and sleep than for a given time

			/**while (pause_)
			{ pause(10); };
			   **/
			phi   =   our_phi[current_round_];
			theta = our_theta[current_round_];
			psi   =   our_psi[current_round_];

      loop_timer.reset();
			if (verbosity > 10)
			{
			Log << "rotation = " << phi << ";" << theta << ";" << psi << endl;
			}

			system2_ = &system_backup_b_;

      detailed_timer.reset();
			changeProteinOrientation_( *system2_, Vector3( phi, theta, psi ) );

			if (verbosity > 10)
			{
	Log << "Time used to rotate protein: " << detailed_timer.getCPUTime() << endl;
			}

      detailed_timer.reset();
			makeFFTGrid_( GeometricFit::PROTEIN_B );

			if (verbosity > 10)
			{
        Log << "Time used to make FFT Grid B: " << detailed_timer.getCPUTime() << endl;
			}

      detailed_timer.reset();
      FFT_grid_b_->doFFT();

			if (verbosity > 10)
			{
        Log << "Time used to do FFT on FFT Grid B: " << detailed_timer.getCPUTime() << endl;
			}

      detailed_timer.reset();
			FFTGridMulti_();

			if (verbosity > 10)
			{
        Log << "Time used to multiply FFT grid A and B: " << detailed_timer.getCPUTime() << endl;
			}

      detailed_timer.reset();

			// we have put the product of the two FFT_grid into FFT_grid_b_
			FFT_grid_b_->doiFFT();

			if (verbosity > 10)
			{
        Log << "Time used to do iFFT on product grid: " << detailed_timer.getCPUTime() << endl;
			}

			detailed_timer.reset();

	    // find out the peak value
	    getGlobalPeak_(top_n_peaks);

			if (verbosity > 10)
			{
		Log << "Time used to find out peaks: " << detailed_timer.getCPUTime() << endl;
			}

			detailed_timer.reset();

			int top_n = options.getInteger(Option::TOP_N);

			for(int j = 0; j < top_n; j++)
			{
				Peak_ temp_p = top_n_peaks[j];
				Vector3 pp = temp_p.translation;
				temp_p.translation = getSeparation_(pp);
				temp_p.orientation = Vector3(phi, theta, psi);

				peak_set_.insert(temp_p);
			}

		} // loop for all conformations

		if( top_n_peaks != NULL )
		{
			delete [] top_n_peaks;
			top_n_peaks = NULL;
		}

		if (verbosity > 5)
		{
			cout << "CPU time needed for docking: " << overall_timer.getCPUTime() << endl;
		}

		// At this point we'll need to gather the data from the slaves in the case of a
		// parallel computation
#ifdef BALL_HAS_MPI
		if (numprocs > 1)
		{
			// this is _not_ particularly clever, but well... it's a start... ;-)
			// TODO: find a nicer way for this stuff
			std::vector<double>  values;
			std::vector<Vector3> translations;
			std::vector<Vector3> rotations;

			std::multiset<Peak_>::iterator it = peak_set_.begin();
			for (; it!=peak_set_.end(); it++)
			{
				values.push_back(it->value);
				translations.push_back(it->translation);
				rotations.push_back(it->orientation);
			}

			//TEST!
			// gather the stuff
			std::vector<double>  all_values;
			std::vector<Vector3> all_translations;
			std::vector<Vector3> all_rotations;

			mpi.acceptCombinedDatapoints(all_values, values);
			mpi.acceptCombinedDatapoints(all_translations, translations);
			mpi.acceptCombinedDatapoints(all_rotations, rotations);

			// and put it in the peak set
			peak_set_.clear();
			for (Size i=0; i<all_values.size(); i++)
			{
				Peak_ p;
				p.value 			= all_values[i];
				p.translation = all_translations[i];
				p.orientation = all_rotations[i];

				peak_set_.insert(p);
			}
		}
#endif // BALL_HAS_MPI
	}

#ifdef BALL_HAS_MPI
	/** This is the main loop of the geometric fit algorithm for the slaves of a parallel run **/
	void GeometricFit::MPI_Slave_start(int argc, char**argv)
	{
		MPISupport mpi(argc, argv);
		Log.info() << "Process " << mpi.getRank() << "just woke up... " << std::endl;

		/** To perform some useful work, we need from our parent:
		 * 		- The options
		 * 		- The Systems
		 * 		- Our part of the angle vector
		 */
		Options *O = mpi.receiveOptions();
		options = *O;
		delete(O);

		System *S = mpi.receiveSystem();
		system1_->set(*S, true);
		delete(S);

		S = mpi.receiveSystem();
		system2_->set(*S, true);
		delete(S);

		/** Now receive the angles **/
		std::vector<int> our_phi, our_theta, our_psi;
		mpi.acceptDatapoints(our_phi);
		mpi.acceptDatapoints(our_theta);
		mpi.acceptDatapoints(our_psi);

		int rotation_num = our_phi.size();

		Timer loop_timer;
		loop_timer.start();

		// Now we can start our part of the loop...
		DockingAlgorithm::start();

		int verbosity = options.getInteger(Option::VERBOSITY);

		Timer overall_timer;
		Timer detailed_timer;

		overall_timer.start();
		detailed_timer.start();

		initGridSizes_();

		if (verbosity > 5)
		{
			Log << "Time used to init grid sizes for A: " << detailed_timer.getCPUTime() << endl;
		}
		detailed_timer.reset();

		initFFTGrid_( GeometricFit::PROTEIN_A );
		initFFTGrid_( GeometricFit::PROTEIN_B );

		if (verbosity > 5)
		{
			Log << "Time used to init FFT Grid A & B: " << detailed_timer.getCPUTime() << endl;
		}
		detailed_timer.reset();

		doPreTranslation_( GeometricFit::PROTEIN_A );

		detailed_timer.reset();
		makeFFTGrid_( GeometricFit::PROTEIN_A );

		if (verbosity > 5)
		{
			Log << "Time used to make FFT Grid A: " << detailed_timer.getCPUTime() << endl;
		}

		detailed_timer.reset();
		FFT_grid_a_->doFFT();

		if (verbosity > 5)
		{
			Log << "Time used to do FFT on FFT Grid A: " << detailed_timer.getCPUTime() << endl;
		}

		detailed_timer.reset();
		calcConjugate_( GeometricFit::PROTEIN_A );

		if (verbosity > 5)
		{
			Log << "Time used to calc conjugate on FFT Grid A: " << detailed_timer.getCPUTime() << endl;
		}

		// since we put the mass center at origin
		// we can do pre-translation of b before all loops.
		doPreTranslation_( GeometricFit::PROTEIN_B );

		// remove old rubbish in peak_set_
		peak_set_.clear();

		// note: the backuped systems are systems after pre-translation.
		system_backup_a_ = *system1_;  // after pre-translation
		system_backup_b_ = *system2_;  // after pre-translation

		// TODO: Use a vector
		Peak_* top_n_peaks = new Peak_[options.getInteger(Option::TOP_N)];
		double phi, theta, psi; // phi: around x axis,
		// theta: around y axis,
		// psi: around z axis;
		for (current_round_ = 0; (current_round_ < rotation_num) && !abort_; current_round_++)
		{
			// TODO: we should check if pause_ is true and sleep than for a given time

			//while (pause_) { pause(10); };

			phi   =   our_phi[current_round_];
			theta = our_theta[current_round_];
			psi   =   our_psi[current_round_];

			loop_timer.reset();
			if (verbosity > 10)
			{
				Log << "rotation = " << phi << ";" << theta << ";" << psi << endl;
			}

			system2_ = &system_backup_b_;

			detailed_timer.reset();
			changeProteinOrientation_( *system2_, Vector3( phi, theta, psi ) );

			if (verbosity > 10)
			{
				Log << "Time used to rotate protein: " << detailed_timer.getCPUTime() << endl;
			}

			detailed_timer.reset();
			makeFFTGrid_( GeometricFit::PROTEIN_B );

			if (verbosity > 10)
			{
				Log << "Time used to make FFT Grid B: " << detailed_timer.getCPUTime() << endl;
			}

			detailed_timer.reset();
			FFT_grid_b_->doFFT();

			if (verbosity > 10)
			{
				Log << "Time used to do FFT on FFT Grid B: " << detailed_timer.getCPUTime() << endl;
			}

			detailed_timer.reset();
			FFTGridMulti_();

			if (verbosity > 10)
			{
				Log << "Time used to multiply FFT grid A and B: " << detailed_timer.getCPUTime() << endl;
			}

			detailed_timer.reset();

			// we have put the product of the two FFT_grid into FFT_grid_b_
			FFT_grid_b_->doiFFT();

			if (verbosity > 10)
			{
				Log << "Time used to do iFFT on product grid: " << detailed_timer.getCPUTime() << endl;
			}

			detailed_timer.reset();

			// find out the peak value
			getGlobalPeak_(top_n_peaks);

			if (verbosity > 10)
			{
				Log << "Time used to find out peaks: " << detailed_timer.getCPUTime() << endl;
			}

			detailed_timer.reset();

			int top_n = options.getInteger(Option::TOP_N);

			for(int j = 0; j < top_n; j++)
			{
				Peak_ temp_p = top_n_peaks[j];
				Vector3 pp = temp_p.translation;
				temp_p.translation = getSeparation_(pp);
				temp_p.orientation = Vector3(phi, theta, psi);

				peak_set_.insert(temp_p);
			}
		}

		if( top_n_peaks != NULL )
		{
			delete [] top_n_peaks;
			top_n_peaks = NULL;
		}

		if (verbosity > 5)
		{
			cout << "CPU time needed for docking: " << overall_timer.getCPUTime() << endl;
		}

		/** Now we'll have to gather the stuff together again... **/

		// this is _not_ particularly clever, but well... it's a start... ;-)
		// TODO: find a nicer way for this stuff
		std::vector<double>  values;
		std::vector<Vector3> translations;
		std::vector<Vector3> rotations;

		// gather the stuff
		std::multiset<Peak_>::iterator it = peak_set_.begin();
		for (; it!=peak_set_.end(); it++)
		{
			values.push_back(it->value);
			translations.push_back(it->translation);
			rotations.push_back(it->orientation);
		}

		// gather the stuff
		mpi.combineDatapoints(values);
		mpi.combineDatapoints(translations);
		mpi.combineDatapoints(rotations);
		// that's it... we're done
	}
#endif

  /** Return the overall docking progress as a percentage
	 */
  float GeometricFit::getProgress() const
  {
		if (total_round_ == 0) return 0.0;

		return (float)current_round_ / (float)total_round_;
  }

	/** Returns true if the docking is already done.
	 */
  bool GeometricFit::hasFinished() const
  {
    return ( (total_round_ != 0) &&  ( current_round_ == total_round_ ) );
  }

	/** Return the translation corresponding to conformation con_num.
	 */
	Vector3 GeometricFit::getTranslation(Index con_num) const
	{
		Vector3 result = (con_num < (Index)translations_.size()) ? translations_[con_num] : Vector3(0.);

		return result;
	}

	/** Return the orientation corresponding to conformation con_num.
	 */
	Vector3 GeometricFit::getOrientation(Index con_num) const
	{
		Vector3 result = (con_num < (Index)orientations_.size()) ? orientations_[con_num] : Vector3(0.);

		return result;
	}
	/** Return the ranked conformations.
	 */
	ConformationSet GeometricFit::getConformationSet(Index total_number)
	{
		// first see how many conformations we should generate
		if ( (total_number == 0) || (total_number > options.getInteger(Option::BEST_NUM)) )
			total_number = options.getInteger(Option::BEST_NUM);

		// clear the translation and orientation vector
		translations_.clear();
		orientations_.clear();
		translations_.resize(total_number);
		orientations_.resize(total_number);

		// 			 this can probably be done smarter
		System S = system_backup_a_;
		System S2 = system_backup_b_;

		S.splice(S2);
		String docking_name = S.getName() + "_" + S2.getName();

		S.setName(docking_name);
		ConformationSet rc(S);

		// iterate over all peaks
		int count = 0;
		multiset<class Peak_>::iterator it = peak_set_.begin();
std::cout << 	"I have " << peak_set_.size() << " peaks...\n";
		int verbosity = options.getInteger(Option::VERBOSITY);
		for (; (it != peak_set_.end()) && (count < total_number); it++)
		{
			Peak_ p = *it;

			if (verbosity > 1)
			{
				Log << "***** result " << count << " *****" << endl;
				Log << endl <<  "peak value =" << p.value << endl;

				Log << "translation = " << p.translation << endl;
				Log << "orientation = " << p.orientation << endl << endl;
			}

			translations_[count] = p.translation;
			orientations_[count] = p.orientation;
			System sys_a = system_backup_a_;
			System sys_b = system_backup_b_;

			changeProteinOrientation_(sys_b, p.orientation);
			TranslationProcessor tp(p.translation);
			sys_b.apply(tp);

			sys_a.splice(sys_b);

			rc.add((-1)*p.value, sys_a);

			count++;
		}

		return rc;
	}

	/** Implementation of the nested helper classes for GeometricFit **/

	/** Peak_ class.
	 *  Constructor.
	 */
	GeometricFit::Peak_::Peak_()
		: value(0.)
	{
	}

	/** Peak_ class
	 * 	Destructor.
	 */
	GeometricFit::Peak_::~Peak_()
	{
	}

	/** Peak_ class
	 *  operator <
	 *  Note: "<" is implemented as ">" on purpose!
	 *  			In this way, the largest peak value is on top of the multiset used in the algorithm.
	 */
	bool GeometricFit::Peak_::operator < (const Peak_& p) const
	{
		return value > p.value;  // based on peak value only
	}

	/** RotationAngles_ class
	 *  Default constructor.
	 */
	GeometricFit::RotationAngles_::RotationAngles_()
		:	phi_(),
			theta_(),
			psi_(),
			ang_num_(0),
			max_rotation_(50000)
	{
		phi_.reserve(max_rotation_);
		theta_.reserve(max_rotation_);
		psi_.reserve(max_rotation_);
	}

	/** RotationAngles_ class
	 * 	Detailed constructor.
	 */
	GeometricFit::RotationAngles_::RotationAngles_( int step )
		:	phi_(),
			theta_(),
			psi_(),
			ang_num_(0),
			max_rotation_(50000)
	{
		phi_.reserve(max_rotation_);
		theta_.reserve(max_rotation_);
		psi_.reserve(max_rotation_);

		generateAllAngles( step );
	}

	/** RotationAngles_ class
	 *  Generate all non-degenerate rotation angles for the given degree interval.
	 * 	This algorithm is based on ???
	 */
	bool GeometricFit::RotationAngles_::generateSomeAngles( const float deg_phi, const float deg_theta, const float deg_psi,
																													const float phi_min, const float phi_max,
																													const float psi_min, const float psi_max,
																													const float theta_min, const float theta_max	)
	{
		ang_num_ = 0;

		int phi_num = (int) ceil((phi_max - phi_min) / deg_phi);
		int psi_num = (int) ceil((psi_max - psi_min) / deg_psi);
		int theta_num = (int) ceil((theta_max - theta_min) / deg_theta);

		for(int psipsi = 0; psipsi < psi_num; psipsi++)
			for(int thetatheta = 0; thetatheta < theta_num; thetatheta++)
				for(int phiphi = 0; phiphi < phi_num; phiphi++)
				{


					double x1 = (phi_min + phiphi * deg_phi) * Constants::PI / 180.0;
					double y1 = (theta_min + thetatheta * deg_theta) * Constants::PI / 180.0;
					double z1 = (psi_min + psipsi * deg_psi) * Constants::PI / 180.0;

					double sx1 = sin(x1);
					double sy1 = sin(y1);
					double sz1 = sin(z1);
					double cx1 = cos(x1);
					double cy1 = cos(y1);
					double cz1 = cos(z1);

					bool degenerate = false;

					for(int k = 0; (k <= psipsi) && (!degenerate); k++)
						for(int j = 0; (j <= thetatheta) && (!degenerate); j++)
							for(int i = 0; (i <= phiphi) && (!degenerate); i++)
							{
								if( (i == phiphi) && (j == thetatheta) && (k == psipsi) )
									continue;

								double x2 = (phi_min + i * deg_phi) * Constants::PI / 180.0;
								double y2 = (theta_min + j * deg_theta) * Constants::PI / 180.0;
								double z2 = (psi_min + k * deg_psi) * Constants::PI / 180.0;

								double sx2 = sin(x2);
								double sy2 = sin(y2);
								double sz2 = sin(z2);
								double cx2 = cos(x2);
								double cy2 = cos(y2);
								double cz2 = cos(z2);


								// Rz * Ry * Rx
								double trace =
									( (     cy1*cz1          ) * (     cy2*cz2          ) +
										( sx1*sy1*cz1 - cx1*sz1) * ( sx2*sy2*cz2 - cx2*sz2) +
										( cx1*sy1*cz1 + sx1*sz1) * ( cx2*sy2*cz2 + sx2*sz2) ) +
									( (     cy1*sz1          ) * (     cy2*sz2          ) +
										( sx1*sy1*sz1 + cx1*cz1) * ( sx2*sy2*sz2 + cx2*cz2) +
										( cx1*sy1*sz1 - sx1*cz1) * ( cx2*sy2*sz2 - sx2*cz2) ) +
									( (    -sy1              ) * (    -sy2              ) +
										( sx1*cy1              ) * ( sx2*cy2              ) +
										( cx1*cy1              ) * ( cx2*cy2              ) );

								double v = (trace - 1.0) / 2.0;

								// v should be in the area [-1.0, 1.0]
								// sometime because of the numerical error,
								// v gets out of this area. so we set it like this:
								if(v < -1.0)
									v = -1.0;
								else if(v > 1.0)
									v = 1.0;

								double alpha = acos( v );

								if(alpha * 180.0 <= 1.0  * Constants::PI)
								{
									degenerate = true;
									break;
								}
							}

					if( !degenerate )
					{
						phi_.push_back((Index)(phi_min + phiphi * deg_phi));
						theta_.push_back((Index)(theta_min + thetatheta * deg_theta));
						psi_.push_back((Index)(psi_min + psipsi * deg_psi));
						ang_num_ ++;

						/*
						if (ang_num_ % 1000 == 0)
						{
							Log.info() << "Calculated angle step: " << ang_num_ << std::endl;
						}
						*/
					}

				}

		// TODO: Why should we decrement ang_num_ again???
		//ang_num_ --;

		return true;
	}

	/** RotationAngles_ class
	 *  Generate all non-degenerate rotation angles for the given degree interval.
	 * 	This algorithm is based on ???
	 */
	bool GeometricFit::RotationAngles_::generateAllAngles( const int deg )
	{
		ang_num_ = 0;

		for(int psipsi = 0; psipsi < 180; psipsi += deg)
			for(int thetatheta = 0; thetatheta < 360; thetatheta += deg)
				for(int phiphi = 0; phiphi < 360; phiphi += deg)
				{

					double x1 = phiphi     * Constants::PI / 180.0;
					double y1 = thetatheta * Constants::PI / 180.0;
					double z1 = psipsi     * Constants::PI / 180.0;

					double sx1 = sin(x1);
					double sy1 = sin(y1);
					double sz1 = sin(z1);
					double cx1 = cos(x1);
					double cy1 = cos(y1);
					double cz1 = cos(z1);

					bool degenerate = false;

					for(int k = 0; (k <= psipsi) && (!degenerate); k += deg)
						for(int j = 0; (j <= thetatheta) && (!degenerate); j += deg)
							for(int i = 0; (i <= phiphi) && (!degenerate); i += deg)
							{
								if( (i == phiphi) && (j == thetatheta) && (k == psipsi) )
									continue;

								double x2 = i * Constants::PI / 180.0;
								double y2 = j * Constants::PI / 180.0;
								double z2 = k * Constants::PI / 180.0;

								double sx2 = sin(x2);
								double sy2 = sin(y2);
								double sz2 = sin(z2);
								double cx2 = cos(x2);
								double cy2 = cos(y2);
								double cz2 = cos(z2);


								// Rz * Ry * Rx
								double trace =
									( (     cy1*cz1          ) * (     cy2*cz2          ) +
										( sx1*sy1*cz1 - cx1*sz1) * ( sx2*sy2*cz2 - cx2*sz2) +
										( cx1*sy1*cz1 + sx1*sz1) * ( cx2*sy2*cz2 + sx2*sz2) ) +
									( (     cy1*sz1          ) * (     cy2*sz2          ) +
										( sx1*sy1*sz1 + cx1*cz1) * ( sx2*sy2*sz2 + cx2*cz2) +
										( cx1*sy1*sz1 - sx1*cz1) * ( cx2*sy2*sz2 - sx2*cz2) ) +
									( (    -sy1              ) * (    -sy2              ) +
										( sx1*cy1              ) * ( sx2*cy2              ) +
										( cx1*cy1              ) * ( cx2*cy2              ) );

								double v = (trace - 1.0) / 2.0;

								// v should be in the area [-1.0, 1.0]
								// sometime because of the numerical error,
								// v gets out of this area. so we set it like this:
								if(v < -1.0)
									v = -1.0;
								else if(v > 1.0)
									v = 1.0;

								double alpha = acos( v );

								if(alpha * 180.0 <= 1.0  * Constants::PI)
								{
									degenerate = true;
									break;
								}
							}

					if( !degenerate )
					{
						phi_.push_back(phiphi);
						theta_.push_back(thetatheta);
						psi_.push_back(psipsi);
						ang_num_ ++;
					}

				}

		ang_num_ --;

		return true;
	}
