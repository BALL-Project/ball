// $Id: poissonBoltzmann.C,v 1.14 2000/05/04 13:37:06 oliver Exp $ 
// FDPB: Finite Difference Poisson Solver

#include <BALL/SOLVATION/poissonBoltzmann.h>

#include <BALL/SOLVATION/molecularSurfaceGrid.h>
#include <BALL/STRUCTURE/geometricProperties.h>
#include <BALL/DATATYPE/hashGrid.h>
#include <BALL/MATHS/vector4.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/SYSTEM/timer.h>
#include <BALL/COMMON/limits.h>

using namespace std;

namespace BALL 
{

	const char* FDPB::Option::VERBOSITY = "verbosity";
	const char* FDPB::Option::PRINT_TIMING = "print_timing";
	const char* FDPB::Option::SPACING = "spacing";
	const char* FDPB::Option::BORDER = "border";
	const char* FDPB::Option::IONIC_STRENGTH = "ionic_strength";
	const char* FDPB::Option::SOLUTE_DC = "solute_dielectric_constant";
	const char* FDPB::Option::SOLVENT_DC = "solvent_dielectric_constant";
	const char* FDPB::Option::PROBE_RADIUS = "probe_radius";
	const char* FDPB::Option::ION_RADIUS = "ion_radius";
	const char* FDPB::Option::TEMPERATURE = "temperature";
	const char* FDPB::Option::BOUNDARY = "boundary_condition";
	const char* FDPB::Option::CHARGE_DISTRIBUTION = "charge_distribution";
	const char* FDPB::Option::DIELECTRIC_SMOOTHING = "dielectric_smoothing";
	const char* FDPB::Option::OFFSET = "offset";
	const char* FDPB::Option::LOWER = "lower";
	const char* FDPB::Option::UPPER = "upper";
	const char* FDPB::Option::BOUNDING_BOX_LOWER = "bounding_box_lower";
	const char* FDPB::Option::BOUNDING_BOX_UPPER = "bounding_box_upper";
	const char* FDPB::Option::RMS_CRITERION = "rms_criterion";
	const char* FDPB::Option::MAX_CRITERION = "max_criterion";
	const char* FDPB::Option::CHECK_AFTER_ITERATIONS = "check_after_iterations";
	const char* FDPB::Option::MAX_ITERATIONS = "max_iterations";

	const char* FDPB::Boundary::ZERO = "zero";
	const char* FDPB::Boundary::DEBYE = "Debye";
	const char* FDPB::Boundary::COULOMB = "Coulomb";
	const char* FDPB::Boundary::DIPOLE = "dipole";
	const char* FDPB::Boundary::FOCUSING = "focusing";

	const char* FDPB::ChargeDistribution::TRILINEAR = "trilinear";
	const char* FDPB::ChargeDistribution::UNIFORM = "uniform";

	const char* FDPB::DielectricSmoothing::NONE = "none";
	const char* FDPB::DielectricSmoothing::UNIFORM = "uniform";
	const char* FDPB::DielectricSmoothing::HARMONIC = "harmonic";

	const int		FDPB::Default::VERBOSITY  = 0;
	const bool	FDPB::Default::PRINT_TIMING  = false;
	const float FDPB::Default::SPACING =  0.6;
	const float FDPB::Default::BORDER = 4.0;
	const float FDPB::Default::TEMPERATURE = 298.15;
	const float FDPB::Default::PROBE_RADIUS = 1.5;
	const float FDPB::Default::ION_RADIUS = 2.0;
	const float FDPB::Default::IONIC_STRENGTH = 0.0;
	const char* FDPB::Default::BOUNDARY = FDPB::Boundary::DIPOLE;
	const char* FDPB::Default::CHARGE_DISTRIBUTION = FDPB::ChargeDistribution::UNIFORM;
	const char* FDPB::Default::DIELECTRIC_SMOOTHING = FDPB::DielectricSmoothing::HARMONIC;
	const float FDPB::Default::SOLVENT_DC = 78.0;
	const float FDPB::Default::SOLUTE_DC = 2.0;
	const float FDPB::Default::RMS_CRITERION = 1e-5;
	const float FDPB::Default::MAX_CRITERION = 1e-4;
	const long  FDPB::Default::MAX_ITERATIONS = 500;
	const long  FDPB::Default::CHECK_AFTER_ITERATIONS = 10;



	/////////////////////////
	FDPB::FDPB()
		:	eps_grid(0),
			kappa_grid(0),
			q_grid(0),
			phi_grid(0),
			SAS_grid(0),
			atom_array(0),
			lower_(0,0,0),
			upper_(0,0,0),
			use_offset_(false),
			energy_(0),
			reaction_field_energy_(0),
			boundary_points_(),
			number_of_iterations_(0),
			error_code_(0)
	{
	}

	FDPB::FDPB(System& system)
		:	eps_grid(0),
			kappa_grid(0),
			q_grid(0),
			phi_grid(0),
			SAS_grid(0),
			atom_array(0),
			lower_(0,0,0),
			upper_(0,0,0),
			use_offset_(false),
			energy_(0),
			reaction_field_energy_(0),
			boundary_points_(),
			number_of_iterations_(0),
			error_code_(0)
	{
		setup(system);
	}


	FDPB::FDPB(Options& new_options)
		:	eps_grid(0),
			kappa_grid(0),
			q_grid(0),
			phi_grid(0),
			SAS_grid(0),
			atom_array(0),
			lower_(0,0,0),
			upper_(0,0,0),
			use_offset_(false),
			energy_(0),
			reaction_field_energy_(0),
			boundary_points_(),
			number_of_iterations_(0),
			error_code_(0)
	{
		options = new_options;
	}

	FDPB::FDPB(System& system, Options& new_options)
		:	eps_grid(0),
			kappa_grid(0),
			q_grid(0),
			phi_grid(0),
			SAS_grid(0),
			atom_array(0),
			lower_(0,0,0),
			upper_(0,0,0),
			use_offset_(false),
			energy_(0),
			reaction_field_energy_(0),
			boundary_points_(),
			number_of_iterations_(0),
			error_code_(0)
	{
		options = new_options;
		setup(system);
	}


	FDPB::~FDPB()
	{
		destroy();
	}

	double FDPB::getEnergy() const 
	{
		return energy_;
	}

	double FDPB::getReactionFieldEnergy() const 
	{
		return reaction_field_energy_;
	}

	unsigned long FDPB::getNumberOfIterations() const
	{
		return number_of_iterations_;
	}

	int FDPB::getErrorCode() const
	{
		return error_code_;
	}

	const char* FDPB::error_message_[] = 
	{
		"Unknown error.",
		"Not implemented.",

		"The atom array could not be created. You probably ran out of memory. Try a smaller grid size.",
		"The SAS_grid could not be created. You probably ran out of memory. Try a smaller grid size.",
		"The grid for the dielectric constant could not be created. You probably ran out of memory. Try a smaller grid size.",
		"The grid for the Debye parameter could not be created. You probably ran out of memory. Try a smaller grid size.",
		"The charge grid could not be created. You probably ran out of memory. Try a smaller grid size.",
		"The potential grid could not be created. You probably ran out of memory. Try a smaller grid size.",
		"This part of the setup requires a precalculated SAS_grid. Run FDPB::setupSASGrid() first.",
		"This part of the setup requires a precalculated dielectric grid. Run FDPB::setupEpsGrid() first.",
		"This part of the setup requires a precalculated atom_array. Run FDPB::setupAtomArray() first.",
		"This part of the setup requires a precalculated potential grid. Run FDPB::setupPhiGrid() first.",
		"Error while allocating memory: You ran out of virtual memory.",
		"The given dielectric_smoothing_method is invalid.",
		"The given charge_distribution_method is invalid.",
		"The given boundary_condition_type is invalid.",
		"The upper/lower options do not contain valid vectors.",
		"lower should be <= upper.",
		"Please execute setup prior to solve." 
	};


	bool FDPB::setupAtomArray(System& system)
	{
		// create a timer to determine the method's runtime
		Timer	step_timer;
		step_timer.start();

		options.setDefaultInteger(Option::VERBOSITY, Default::VERBOSITY);
		options.setDefaultBool(Option::PRINT_TIMING, Default::PRINT_TIMING);

		// first, check whether we should tell to our user what we`re doing
		int verbosity = (int)options.getInteger(Option::VERBOSITY);

		// ...and whether we should tell how long it took us.
		bool print_timing = options.getBool(Option::PRINT_TIMING);

		if (verbosity > 1)
		{
			Log.info(2) << "creating array of charged atoms..." << endl;
		}
					
		// create a new atom_array
		delete atom_array;
		atom_array = new vector<FDPB::FastAtom>;

		FDPB::FastAtom	fast_atom;											
		AtomIterator		atom_iterator;
		Vector3 position;
		BALL_FOREACH_ATOM(system, atom_iterator)
		{
			fast_atom.q = (*atom_iterator).getCharge();
			position = atom_iterator->getPosition();
			fast_atom.x = position.x;
			fast_atom.y = position.y;
			fast_atom.z = position.z;
			fast_atom.r = atom_iterator->getRadius();
			atom_array->push_back(fast_atom);
		}
		
		step_timer.stop();
		if (print_timing && (verbosity > 1))
		{
			Log.info(2) << "setupAtomArray: " << step_timer.getCPUTime() << endl;
		}
			
		return true;
	}

	bool FDPB::setupEpsGrid(System& system)
	{
		// precondition: setupAtomArray
    if (atom_array == 0)
		{
      error_code_ = FDPB::ERROR__ATOM_ARRAY_REQUIRED;
      return false;
		}                                                                                                                        

		// the verbosity level
		int	verbosity;

		// print_timing decides whether timing information for
		// the different stages of calculation are printed
		bool print_timing;

		// the distance between the box and the closest atoms
		float box_distance;

		// remove an old grid (if it exists)
		delete eps_grid;
		eps_grid = 0;

		options.setDefaultInteger(Option::VERBOSITY, Default::VERBOSITY);
		options.setDefaultBool(Option::PRINT_TIMING, Default::PRINT_TIMING);
		options.setDefaultReal(Option::SPACING, Default::SPACING);
		options.setDefaultReal(Option::BORDER, Default::BORDER);
		options.setDefaultReal(Option::PROBE_RADIUS, Default::PROBE_RADIUS);
		options.setDefaultReal(Option::ION_RADIUS, Default::ION_RADIUS);

		// first, check whether we should tell to our user what we`re doing
		verbosity = (int)options.getInteger(Option::VERBOSITY);

		// ...and whether we should tell him how long it took us...
		print_timing = options.getInteger(Option::PRINT_TIMING);

		Timer	step_timer;
		step_timer.start();

		// second, the grid spacing, option name "spacing"
		spacing_ = options.getReal(Option::SPACING);

		// box_distance is the value between the center of the outmost atom
		// and the box boundary
		box_distance = options.getReal(Option::BORDER);

		// using the keyword "offset" an offset vector may be given	
		// its units are _ grid _ spacings
		// i.e. giving "offset=0.0 1.0 0.0" will cause all atomic
		// centers to be translated by one grid spacing along the y-axis
		// In fact, not the atoms are translated, but the grid is 
		// translated in the opposite direction.
		use_offset_ = false;

		if (options.has(Option::OFFSET))
		{
			offset_ = options.getVector(Option::OFFSET);
			offset_ *= spacing_;
			use_offset_ = true;
		}

		if (options.isSet(Option::LOWER) && options.isSet(Option::UPPER))
		{
			// the grid dimension is given in the options
			// first, check whether the entry contains a valid vector
			if (!options.isVector(Option::LOWER) 
					|| !options.isVector(Option::LOWER))
			{
				error_code_ = FDPB::ERROR__NOT_A_VECTOR_IN_UPPER_LOWER;
				return false;
			} 
			else 
			{
				lower_ = options.getVector(Option::LOWER);
				upper_ = options.getVector(Option::UPPER);

				// check whether lower is really lower than upper
				if ((lower_.x >= upper_.x) || (lower_.y >= upper_.y) || (lower_.z >= upper_.z))
				{
					error_code_ = FDPB::ERROR__ILLEGAL_VALUE_FOR_LOWER_UPPER;
					return false;
				}
			}

		} 
		else 
		{
			// determine the molecule`s extent (bounding box)
				
			if (options.isSet(Option::BOUNDING_BOX_LOWER)
					 && options.isSet(Option::BOUNDING_BOX_UPPER))
			{
				// read the bounding box from the options
				lower_ = options.getVector(Option::BOUNDING_BOX_LOWER);
				upper_ = options.getVector(Option::BOUNDING_BOX_UPPER);
			} 
			else 
			{
				if (verbosity > 5)
				{
					Log.info(6) << "creating bounding box..." << endl;
				}

				BoundingBoxProcessor		box_processor;
				system.apply(box_processor);
					
				lower_ = box_processor.getLower();
				upper_ = box_processor.getUpper();
				options.setVector(Option::BOUNDING_BOX_LOWER, lower_);
				options.setVector(Option::BOUNDING_BOX_UPPER, upper_);
			}

			// closeness: distance between grid and bounding box
			Vector3			closeness;

			if (verbosity > 70)
			{
				Log.info(70) << "grid border: " << box_distance << endl;
			}

			closeness.set(box_distance, box_distance, box_distance);
			upper_ += closeness;
			lower_ -= closeness;


			// we need a cubic grid, so calculate the largest dimension of the grid
			float size = BALL_MAX3(upper_.x - lower_.x, upper_.y - lower_.y, upper_.z - lower_.z);

			// now expand the grid in all three directions. This also changes the origin.
			// The molecule is centered in the resulting box

			lower_.set(lower_.x + (upper_.x - lower_.x - size) / 2,
								 lower_.y + (upper_.y - lower_.y - size) / 2,
								 lower_.z + (upper_.z - lower_.z - size) / 2);
			
			upper_.set(size, size, size);
			upper_ += lower_;

			// store the grid settings in the options
			options.setVector(Option::LOWER, lower_);
			options.setVector(Option::UPPER, upper_);
		}

		if (verbosity > 1)
		{
			Log.info(2) << "grid:" << lower_ << "/" << upper_ << endl;
		}

		if (use_offset_)
		{
			upper_ -= offset_;
			lower_ -= offset_;
		}
		
		// retrieve important settings for the eps_grid from the options
		options.setDefaultReal(Option::SOLVENT_DC, Default::SOLVENT_DC);
		options.setDefaultReal(Option::SOLUTE_DC, Default::SOLUTE_DC);
		options.setDefault(Option::DIELECTRIC_SMOOTHING, Default::DIELECTRIC_SMOOTHING);

		// ...the dielectric smoothing option
		int dielectric_smoothing_method;
		if (options[Option::DIELECTRIC_SMOOTHING] == FDPB::DielectricSmoothing::HARMONIC)
		{
			dielectric_smoothing_method = 1;
		} 
		else if (options[Option::DIELECTRIC_SMOOTHING] == FDPB::DielectricSmoothing::UNIFORM) 
		{
			dielectric_smoothing_method = 2;
		} 
		else if (options[Option::DIELECTRIC_SMOOTHING] == FDPB::DielectricSmoothing::NONE) 
		{
			dielectric_smoothing_method = 0;
		} 
		else 
		{
			error_code_ = FDPB::ERROR__UNKNOWN_DIELECTRIC_SMOOTHING_METHOD;
			return false;
		}

		// the dielectric constants
		float solvent_dielectric_constant = options.getReal(Option::SOLVENT_DC);
		float solute_dielectric_constant = options.getReal(Option::SOLUTE_DC);
		

		// now, create a new grid containing the dielectric constant of each grid point
		delete eps_grid;
		eps_grid = new PointGrid<Vector3>(lower_, upper_, spacing_);

		// check whether the grid is really cubic
		if ((eps_grid->getMaxXIndex() != eps_grid->getMaxYIndex()) 
				|| (eps_grid->getMaxXIndex() != eps_grid->getMaxZIndex()))
		{
			Log.error() << "grid is not cubic - please check dimensions!" << endl;
			return false;
		}		
					
		if (verbosity > 1)
		{
			Log.info(2) << "grid dimensions: " << eps_grid->getMaxXIndex() << "x"
								  << eps_grid->getMaxYIndex() << "x" << eps_grid->getMaxZIndex() << endl;	
		}

		// determine the maximum radius of all atoms
		vector<FDPB::FastAtom>::iterator atom_array_it= atom_array->begin();
		float max_radius = 0.0;
		for (; atom_array_it != atom_array->end(); ++atom_array_it)
		{
			if (atom_array_it->r > max_radius)
			{
				max_radius = atom_array_it->r;
			}
		}
		HashGrid3<Vector4> atom_grid(eps_grid->getOrigin(), eps_grid->getDimension(), max_radius);
		for (atom_array_it= atom_array->begin(); atom_array_it != atom_array->end(); ++atom_array_it)
		{
			Vector4 v(atom_array_it->x, atom_array_it->y, atom_array_it->z, atom_array_it->r * atom_array_it->r);
			Vector3 r(atom_array_it->x, atom_array_it->y, atom_array_it->z);
			atom_grid.insert(r, v);
		}

		// count the points inside and outside (just for curiosity)
		Size inside_points = 0;
		Size outside_points = 0;

		// the offsets of the thre eps points in a grid
		Vector3 offsets[3];
		offsets[0].set(eps_grid->getXSpacing() / 2.0, 0.0, 0.0);
		offsets[1].set(0.0, eps_grid->getYSpacing() / 2.0, 0.0);
		offsets[2].set(0.0, 0.0, eps_grid->getZSpacing() / 2.0);
		
		// iterators needed to walk the grid
		HashGridBox3<Vector4>::BoxIterator box_it;
		HashGridBox3<Vector4>::DataIterator data_it;

		// walk over all grid points
		for (Position i = 0; i < eps_grid->getSize(); ++i)	
		{
			for (Position j = 0; j < 3; ++j)
			{
				// everything is initially outside
				bool outside = true;

				Vector3 position(eps_grid->getGridCoordinates(i) + offsets[j]);
				HashGridBox3<Vector4>* box = atom_grid.getBox(position);
				if (box != 0)
				{
					// iterate over all atoms in this box
					for (data_it = box->beginData(); +data_it; ++data_it)
					{
						// is there something in the box that is closer than its radius?
						if (position.getSquareDistance(Vector3(data_it->x, data_it->y, data_it->z)) <= data_it->h)
						{
							// mark the point as inside
							outside = false; 
							break;
						}
					}
					if (outside)
					{
						// if we didn't find anything, iterate over all 
						// surrounding boxes as well
						for (box_it = box->beginBox(); +box_it; ++box_it)
						{
							// iterate over all items in the box, abort if we found an atom that is close enough
							for (data_it = box_it->beginData(); +data_it && outside; ++data_it)
							{
								// is there something in the box that is closer than its radius?
								if (position.getSquareDistance(Vector3(data_it->x, data_it->y, data_it->z)) <= data_it->h)
								{
									// mark the point as inside and abort the loop	
									// the outer loop is aborted by outside == false
									outside = false; 
									break;
								}
							}
						}
					}
				}

				// mark points inside
				if (outside)
				{
					(*eps_grid)[i][j] = 1.0;
					outside_points++;
				} 
				else
				{
					(*eps_grid)[i][j] = 0.0;
					inside_points++;
				}
			}
		}
		
		// document the number of inside and outside points
		results.setInteger("inside_points", inside_points);
		results.setInteger("outside_points", outside_points);

		// variables for fast index evaluation
		Size Nx = eps_grid->getMaxXIndex() + 1;
		Size Nxy = (eps_grid->getMaxYIndex() + 1) * Nx;
		Position s, t, q;
		unsigned short border;
		boundary_points_.clear();
		for (s = 1; s < eps_grid->getMaxZIndex(); s++)
		{
		  for (t = 1; t < eps_grid->getMaxYIndex(); t++)	
			{
				for (q = 1; q < eps_grid->getMaxXIndex(); q++)
				{
					// calculate the absolute grid index the hard way (faster!)
					Position idx = q + Nx * t + s * Nxy;

					// check for boundary points.
					// We consider the position of the point itself
					// and the six neighbouring points.
					// A point is an the boundary if not all seven
					// points have the same value, i.e., if the
					// sum of the seven values is not zero and not seven
					border = (unsigned short)(((*eps_grid)[idx].x == 0.0)
										+ ((*eps_grid)[idx].y == 0.0)
										+ ((*eps_grid)[idx].z == 0.0)
										+ ((*eps_grid)[idx - 1].x == 0.0)
										+ ((*eps_grid)[idx - Nx].y == 0.0)
										+ ((*eps_grid)[idx - Nxy].z == 0.0));
					if ((border > 0) && (border < 6))
					{
						boundary_points_.push_back(idx);
					}
				}
			}
		}

		if (verbosity > 10)
		{
			Log.info() << "Boundary points: " << boundary_points_.size() << endl;
		}
		results.set("boundary points", boundary_points_.size());

		// assign the dielectric constants
		for (Position i = 0; i < eps_grid->getSize(); i++)
		{
			// we assign the solvent DC to all points that were outside 
			// (marked by 1.0) and the solute DC to al points inside (0.0)
			// We do it in parallel for all three intermediate points...
			(*eps_grid)[i] *= (solvent_dielectric_constant - solute_dielectric_constant);
			(*eps_grid)[i] += solute_dielectric_constant;
		}

		// execute the dielectric smoothing (if any)
		if (dielectric_smoothing_method != 0)
		{
			if (verbosity > 1)
			{
				Log.info(2) << "performing dielectric smoothing..." << endl;
			}

			// harmonic smoothing
			PointGrid<Vector3> tmp_grid(*eps_grid);

			// loop variables;
			Position x, y, z;
			for (z = 1; z < eps_grid->getMaxZIndex(); z++)
			{
				for (y = 1; y < eps_grid->getMaxYIndex(); y++)
				{
					for (x = 1; x < eps_grid->getMaxXIndex(); x++)
					{
						Position idx = x + Nx * y + Nxy * z;
							
						tmp_grid[idx].x =     // the point itself
																	 1 / (*eps_grid)[idx].x
																	// then, a tetragonal prism with distance sqrt(2)/2 * spacing_ 
																	// from the central point
																 + 1 / (*eps_grid)[idx].y
																 + 1 / (*eps_grid)[idx].z
																 + 1 / (*eps_grid)[idx + 1].y
																 + 1 / (*eps_grid)[idx + 1].z
																 + 1 / (*eps_grid)[idx - Nx].y
																 + 1 / (*eps_grid)[idx - Nx + 1].y
																 + 1 / (*eps_grid)[idx - Nxy].z
																 + 1 / (*eps_grid)[idx - Nxy + 1].z;

						tmp_grid[idx].y =   	// the point itself
																	 1 / (*eps_grid)[idx].y
																	// then, a tetragonal prism with distance sqrt(2)/2 * spacing_
																	// from the central point
																 + 1 / (*eps_grid)[idx - 1].x
																 + 1 / (*eps_grid)[idx].x
																 + 1 / (*eps_grid)[idx + Nx - 1].x
																 + 1 / (*eps_grid)[idx + Nx].x
																 + 1 / (*eps_grid)[idx].z
																 + 1 / (*eps_grid)[idx - Nxy].z
																 + 1 / (*eps_grid)[idx + Nx].z
																 + 1 / (*eps_grid)[idx + Nx - Nxy].z;

						tmp_grid[idx].z =   	// the point itself
																	 1 / (*eps_grid)[idx].z
																	// then, a tetragonal prism with distance sqrt(2)/2 * spacing_
																	// from the central point
																 + 1 / (*eps_grid)[idx].x
																 + 1 / (*eps_grid)[idx].y
																 + 1 / (*eps_grid)[idx - 1].x
																 + 1 / (*eps_grid)[idx - Nx].y
																 + 1 / (*eps_grid)[idx + Nxy].y
																 + 1 / (*eps_grid)[idx + Nxy - 1].x
																 + 1 / (*eps_grid)[idx + Nxy - Nx].y
																 + 1 / (*eps_grid)[idx + Nxy].x;
						/* 
						// smooth over futher spheres 

															
														// Y
																	// second, an octaeder with distance spacing_ from the center
																 + 1 / (*eps_grid)[idx -   1].y 
																 + 1 / (*eps_grid)[idx -  Nx].y 
																 + 1 / (*eps_grid)[idx +   1].y
																 + 1 / (*eps_grid)[idx +  Nx].y
																 + 1 / (*eps_grid)[idx - Nxy].y
																 + 1 / (*eps_grid)[idx + Nxy].y
																	
																	// third, a dodecahedron with distance sqrt(2) * spacing_ from the center
																 + 1 / (*eps_grid)[idx -  Nx -   1].y 
																 + 1 / (*eps_grid)[idx +  Nx -   1].y 
																 + 1 / (*eps_grid)[idx +  Nx +   1].y 
																 + 1 / (*eps_grid)[idx -  Nx +   1].y 

																 + 1 / (*eps_grid)[idx -  Nx - Nxy].y 
																 + 1 / (*eps_grid)[idx +  Nx - Nxy].y 
																 + 1 / (*eps_grid)[idx +  Nx + Nxy].y 
																 + 1 / (*eps_grid)[idx -  Nx + Nxy].y 

																 + 1 / (*eps_grid)[idx - Nxy -   1].y 
																 + 1 / (*eps_grid)[idx + Nxy -   1].y 
																 + 1 / (*eps_grid)[idx + Nxy +   1].y 
																 + 1 / (*eps_grid)[idx - Nxy +   1].y 
							*/
						// scale by the number of points used for smoothing
						float points = 9.0;
						tmp_grid[idx] = Vector3(points / tmp_grid[idx].x, 
																		points / tmp_grid[idx].y, 
																		points / tmp_grid[idx].z);
				
					}
				}
			}

			// copy the temporary grid back to the old dielectric grid
			eps_grid->set(tmp_grid);
		}

		step_timer.stop();
		if (print_timing && (verbosity > 1))
		{
			Log.info(2) << "setupEpsGrid: " << step_timer.getCPUTime() << endl;	
		}
				
		return true;
	}

	bool FDPB::setupSASGrid(System& system)
	{

		// timer for run time determination
		Timer	step_timer; 
		step_timer.start();

		options.setDefaultInteger(Option::VERBOSITY, Default::VERBOSITY);
		options.setDefaultBool(Option::PRINT_TIMING, Default::PRINT_TIMING);
		options.setDefaultReal(Option::ION_RADIUS, Default::ION_RADIUS);
		options.setDefaultReal(Option::IONIC_STRENGTH, Default::IONIC_STRENGTH);
		
		// first, check whether we should tell to our user what we`re doing
		int verbosity = (int)options.getInteger(Option::VERBOSITY);

		// ...and whether we should tell how long it took us.
		bool print_timing = options.getBool(Option::PRINT_TIMING);

		// if an old grid exists, remove it
		delete SAS_grid;
		SAS_grid = 0;
			
		float ionic_strength = options.getReal(Option::IONIC_STRENGTH);
		// BAUSTELLE: Ionenstaerke fehlt noch!
		float ion_radius = options.getReal(Option::ION_RADIUS);

		if (ionic_strength == 0.0)
		{
			return true;
		}

		if (verbosity > 1)
		{
			Log.info(1) << "calculating SAS..." << endl;
		}

		SAS_grid = calculateSASGrid(lower_, upper_, spacing_, system, ion_radius);

		step_timer.stop();
		if (print_timing && (verbosity >1))
		{
			Log.info(2) << "setupSASGrid: " << step_timer.getCPUTime() << endl;
		}

		if (SAS_grid != 0)
		{
			return true;
		}
		else 
		{
			error_code_ = FDPB::ERROR__CANNOT_CREATE_SAS_GRID;
			return false;
		}
	}

	bool FDPB::setupQGrid()
	{
		// create a timer to determine the method's runtime
		Timer	step_timer;
		step_timer.start();

		options.setDefaultInteger(Option::VERBOSITY, Default::VERBOSITY);
		options.setDefaultBool(Option::PRINT_TIMING, Default::PRINT_TIMING);

		// first, check whether we should tell to our user what we`re doing
		int verbosity = (int)options.getInteger(Option::VERBOSITY);

		// ...and whether we should tell how long it took us.
		bool print_timing = options.getBool(Option::PRINT_TIMING);

		if (verbosity > 1)
		{
			Log.info(2) << "creating charge grid..." << endl;
		}
					
		// check whether all requirements are met			
		if (eps_grid == 0)
		{
			error_code_ = FDPB::ERROR__EPSILON_GRID_REQUIRED;
			return false;
		}

		if (atom_array == 0)
		{
			error_code_ = FDPB::ERROR__ATOM_ARRAY_REQUIRED;
			return false;
		}

		// now retrieve important settings from the options
		
		// set the neccessary option defaults
		options.setDefault(Option::CHARGE_DISTRIBUTION, Default::CHARGE_DISTRIBUTION);

		int charge_distribution_method;
		// ...the charge distribution method
		if (options[Option::CHARGE_DISTRIBUTION] == FDPB::ChargeDistribution::TRILINEAR)
		{
			charge_distribution_method = 1;
		} 
		else if (options[Option::CHARGE_DISTRIBUTION] == FDPB::ChargeDistribution::UNIFORM) 
		{
			charge_distribution_method = 0;
		} 
		else 
		{
			error_code_ = FDPB::ERROR__UNKNOWN_CHARGE_DISTRIBUTION_METHOD;
			return false;
		}
					
		// create the grid
		delete q_grid;
		q_grid = new PointGrid<float>(lower_, upper_, spacing_);

		// set every grid point to zero
		Index i;
		for (i = 0; i < (Index)q_grid->getSize(); (*q_grid)[i++] = 0.0);
		
		// fraction of charge assigned in x|y|z-direction
		// needed for linear interpolation
		float fraction_x;
		float fraction_y;
		float fraction_z;

		float origin_x = q_grid->getOrigin().x;
		float origin_y = q_grid->getOrigin().y;
		float origin_z = q_grid->getOrigin().z;

		// distribute the charge on the grid
		
		// some commonly used variables
		Position index;
		Size Nx	= q_grid->getMaxXIndex() + 1;
		Size Nxy = (q_grid->getMaxYIndex() + 1) * Nx;
		
		Vector3	position;

		switch (charge_distribution_method) 
		{
			case 1:  
				// TRILINEAR:
				// distribute the charge equally upon the eigth 
				// closest gridpoints
					
				PointGrid<float>::GridIndex	grid_index;
				long i;
				for (i = 0; i < (long)(*atom_array).size(); i++)
				{

					grid_index.x = (int)(((*atom_array)[i].x - origin_x) / spacing_);
					grid_index.y = (int)(((*atom_array)[i].y - origin_y) / spacing_);
					grid_index.z = (int)(((*atom_array)[i].z - origin_z) / spacing_);

					// calculate the absolute grid position
					index = (long)grid_index.x + (long)grid_index.y * Nx + (long)grid_index.z * Nxy;
					
					
					// check whether the charge is outside the grid
					if (index >= ((*q_grid).getSize() - Nxy - Nx - 1))
					{
						Log.warn() << "warning: atom outside grid at (" 
											 << (*atom_array)[i].x << ","
											 << (*atom_array)[i].y << ","
											 << (*atom_array)[i].z << ")" << endl;

						return false;
					} 

					// ...and store it in the atom_array
					(*atom_array)[i].index = index;

					// calculate fractions of grid coordinates for
					// linear interpolation
					position		= phi_grid->getGridCoordinates(index);
					fraction_x	= ((*atom_array)[i].x - position.x) / spacing_;
					fraction_y	= ((*atom_array)[i].y - position.y) / spacing_;
					fraction_z	= ((*atom_array)[i].z - position.z) / spacing_;

					// distribute the charge on the eight grid_points (equally)
					(*q_grid)[index]								+= (*atom_array)[i].q * (1 - fraction_x) * (1 - fraction_y) * (1 - fraction_z);
					(*q_grid)[index + 1]						+= (*atom_array)[i].q * fraction_x *(1 - fraction_y) * (1 - fraction_z);
					(*q_grid)[index + Nx]						+= (*atom_array)[i].q * (1 - fraction_x) * fraction_y * (1 - fraction_z);
					(*q_grid)[index + Nx + 1]				+= (*atom_array)[i].q * fraction_x * fraction_y * (1 - fraction_z);
					(*q_grid)[index + Nxy]					+= (*atom_array)[i].q * (1 - fraction_x) * (1 - fraction_y) * fraction_z;
					(*q_grid)[index + Nxy + 1]			+= (*atom_array)[i].q * fraction_x * (1 - fraction_y) * fraction_z;
					(*q_grid)[index + Nxy + Nx]			+= (*atom_array)[i].q * (1 - fraction_x) * fraction_y * fraction_z;
					(*q_grid)[index + Nxy + Nx + 1]	+= (*atom_array)[i].q * fraction_x * fraction_y * fraction_z;
				}
				break;

			case 0:
				// UNIFORM
				// distribute the charge uniform on each grid point
				// inside the sphere given by an atom`s radius and position

				PointGrid<float>::GridIndex		lower_grid_index, upper_grid_index;

				// the atom radius, and the squared atom radius
				float atom_radius, atom_radius2;

				// counter: counts the numer of grid points inside
				// an atom
				long count;

				// the number of grid points that fully include the atom_radius
				short radius_on_grid;

				// squared diagonal length of a grid box
				float d2 = spacing_ * spacing_ * 3;

				for (i = 0; i < (Index)(*atom_array).size(); i++)
				{
					atom_radius = (*atom_array)[i].r;
					atom_radius2 = atom_radius * atom_radius;

					radius_on_grid = (short)((atom_radius + d2) / spacing_ + 1);
					
					lower_grid_index = phi_grid->getIndex((*atom_array)[i].x, 
																									(*atom_array)[i].y, 
																									(*atom_array)[i].z);
					lower_grid_index.x -= radius_on_grid;
					lower_grid_index.y -= radius_on_grid;
					lower_grid_index.z -= radius_on_grid;

					upper_grid_index = phi_grid->getIndex((*atom_array)[i].x, 
																									(*atom_array)[i].y, 
																									(*atom_array)[i].z);
					upper_grid_index.x += radius_on_grid;
					upper_grid_index.y += radius_on_grid;
					upper_grid_index.z += radius_on_grid;


					/////////
					// first, count the number of grid points inside the atom`s radius
					//

					count = 0;
					// loop variables
					unsigned long q, r, s;

					float x_u, y_u, z_u, squared_distance;

					Vector3		position;

					position = (*q_grid).getOrigin();
					x_u = position.x;
					y_u = position.y;
					z_u = position.z;

					for (s = lower_grid_index.z; s <= upper_grid_index.z; s++)
						for (r = lower_grid_index.y; r <= upper_grid_index.y; r++)
							for (q = lower_grid_index.x; q <= upper_grid_index.x; q++)
							{
								squared_distance = ((x_u + spacing_ * (float)q) - (*atom_array)[i].x)
																 * ((x_u + spacing_ * (float)q) - (*atom_array)[i].x)
																 + ((y_u + spacing_ * (float)r) - (*atom_array)[i].y)
																 * ((y_u + spacing_ * (float)r) - (*atom_array)[i].y)
																 + ((z_u + spacing_ * (float)s) - (*atom_array)[i].z)
																 * ((z_u + spacing_ * (float)s) - (*atom_array)[i].z);

								if (squared_distance <= atom_radius2)	
								{
									count++;
								}
							}

					if (count > 8)
					{  
						// OK, the atom radius is large enough, is uniform charging
						for (s = lower_grid_index.z; s <= upper_grid_index.z; s++)
							for (r = lower_grid_index.y; r <= upper_grid_index.y; r++)
								for (q = lower_grid_index.x; q <= upper_grid_index.x; q++)
								{
									squared_distance = ((x_u + spacing_ * (float)q) - (*atom_array)[i].x)
																	 * ((x_u + spacing_ * (float)q) - (*atom_array)[i].x)
																	 + ((y_u + spacing_ * (float)r) - (*atom_array)[i].y)
																	 * ((y_u + spacing_ * (float)r) - (*atom_array)[i].y)
																	 + ((z_u + spacing_ * (float)s) - (*atom_array)[i].z)
																	 * ((z_u + spacing_ * (float)s) - (*atom_array)[i].z);
		
									if (squared_distance <= atom_radius2)
									{
										// every grid point inside the atom`s radius receives an
										// equal portion of the atom`s total charge
										Index index = (Index)(q + Nx * r + Nxy * s);
										if ((index >= 0) && (index < (Index)q_grid->getSize()))
										{
											(*q_grid)[index] += (*atom_array)[i].q / (float)count;
										}
									}
								}
									
					} 
					else 
					{ 
						// use trilinear charge distribution - radius is too small


						// calculate absolute grid index
						lower_grid_index.x = (int)(((*atom_array)[i].x - origin_x) / spacing_);
						lower_grid_index.y = (int)(((*atom_array)[i].y - origin_y) / spacing_);
						lower_grid_index.z = (int)(((*atom_array)[i].z - origin_z) / spacing_);

						long index = (long)(lower_grid_index.x + Nx * lower_grid_index.y 
													 + Nxy * lower_grid_index.z);
						
						// check whether the point is inside the grid
						if ((index >= 0) && (index < (Index)(phi_grid->getSize() - Nxy - Nx - 1)))
						{
							// calculate fractions if grid coordinates for
							// linear interpolation
							position		= phi_grid->getGridCoordinates(index);
							fraction_x	= ((*atom_array)[i].x - position.x) / spacing_;
							fraction_y	= ((*atom_array)[i].y - position.y) / spacing_;
							fraction_z	= ((*atom_array)[i].z - position.z) / spacing_;

							// distribute the charge on the eight grid_points (equally)
							(*q_grid)[index]									+= (*atom_array)[i].q * (1 - fraction_x) * (1 - fraction_y) * (1 - fraction_z);
							(*q_grid)[index + 1]							+= (*atom_array)[i].q * fraction_x *(1 - fraction_y) * (1 - fraction_z);
							(*q_grid)[index + Nx]							+= (*atom_array)[i].q * (1 - fraction_x) * fraction_y * (1 - fraction_z);
							(*q_grid)[index + Nx + 1]					+= (*atom_array)[i].q * fraction_x * fraction_y * (1 - fraction_z);
							(*q_grid)[index + Nxy]						+= (*atom_array)[i].q * (1 - fraction_x) * (1 - fraction_y) * fraction_z;
							(*q_grid)[index + Nxy + 1]				+= (*atom_array)[i].q * fraction_x * (1 - fraction_y) * fraction_z;
							(*q_grid)[index + Nxy + Nx]				+= (*atom_array)[i].q * (1 - fraction_x) * fraction_y * fraction_z;
							(*q_grid)[index + Nxy + Nx + 1]		+= (*atom_array)[i].q * fraction_x * fraction_y * fraction_z;
						}
					}
				}

		}

		// now calculate the total distributed charge
		// and the number of charged atoms
		float total_charge = 0.0;
		long number_of_charged_atoms = 0;

		for (i = 0; i < (Index)atom_array->size(); i++)
		{
			if((*atom_array)[i].q != 0.0)
			{
				number_of_charged_atoms++;
				total_charge += (*atom_array)[i].q;
			}

		}
					
		// store this information in the results
		results.setReal("total_charge", total_charge);
		results.setInteger("number_of_atoms", (long)atom_array->size() + 1L);
		results.setInteger("number_of_charged_atoms", number_of_charged_atoms);

		step_timer.stop();
		if (print_timing && (verbosity > 1))
		{
			Log.info(2) << "setupEpsGrid: " << step_timer.getCPUTime() << endl;
		}
				
		return true;
	}


	bool FDPB::setupKappaGrid()
	{
		// create a timer to determine the method's runtime
		Timer	step_timer;
		step_timer.start();

		options.setDefaultInteger(Option::VERBOSITY, Default::VERBOSITY);
		options.setDefaultBool(Option::PRINT_TIMING, Default::PRINT_TIMING);
		options.setDefaultReal(Option::IONIC_STRENGTH, Default::IONIC_STRENGTH);

		// first, check whether we should tell to our user what we`re doing
		int verbosity = (int)options.getInteger(Option::VERBOSITY);

		// ...and whether we should tell how long it took us.
		bool print_timing = options.getBool(Option::PRINT_TIMING);

		float ionic_strength = options.getReal(Option::IONIC_STRENGTH);
		// float ion_radius = options.getReal(Option::ION_RADIUS);

		if (ionic_strength == 0.0)
		{
			return true;
		}

		if (verbosity > 1)
		{
			Log.info(2) << "creating kappa grid..." << endl;
		}
					
		// check whether all requirements are met			
		if (eps_grid == 0)
		{
			error_code_ = FDPB::ERROR__EPSILON_GRID_REQUIRED;
			return false;
		}


		// create the grid
		delete kappa_grid;
		kappa_grid = new PointGrid<float>(lower_, upper_, spacing_);

		// we don't need the SAS grid anymore
		delete SAS_grid;
		SAS_grid = 0;

		step_timer.stop();
		if (print_timing && (verbosity > 1))
		{
			Log.info(2) << "setupKappaGrid: " << step_timer.getCPUTime() << endl;
		}

		return true;
	}


	bool FDPB::setupPhiGrid()
	{
		// create a timer to determine the method's runtime
		Timer	step_timer;
		step_timer.start();

		options.setDefaultInteger(Option::VERBOSITY, Default::VERBOSITY);
		options.setDefaultBool(Option::PRINT_TIMING, Default::PRINT_TIMING);

		// first, check whether we should tell to our user what we`re doing
		int verbosity = (int)options.getInteger(Option::VERBOSITY);

		// ...and whether we should tell how long it took us.
		bool print_timing = options.getBool(Option::PRINT_TIMING);

		if (verbosity > 1)
		 	Log.info(2) << "creating phi grid..." << endl;
					
		// check whether all requirements are met			
		if (eps_grid == 0)
		{
			error_code_ = FDPB::ERROR__EPSILON_GRID_REQUIRED;
			return false;
		}


		// create the grid
		delete phi_grid;
		phi_grid = new PointGrid<float>(lower_, upper_, spacing_);

		// setting Phi to zero everywhere
		Index i;
		for (i = 0; i < (Index)phi_grid->getSize(); (*phi_grid)[i++] = 0.0);

		step_timer.stop();
		if (print_timing && (verbosity > 1))
			Log.info(2) << "setupPhiGrid: " << step_timer.getCPUTime() << endl;

		return true;
	}


	bool FDPB::setupBoundary()
	{
		// create a timer to determine the method's runtime
		Timer	step_timer;
		step_timer.start();

		options.setDefaultInteger(Option::VERBOSITY, Default::VERBOSITY);
		options.setDefaultBool(Option::PRINT_TIMING, Default::PRINT_TIMING);

		// first, check whether we should tell to our user what we`re doing
		int verbosity = (int)options.getInteger(Option::VERBOSITY);

		// ...and whether we should tell how long it took us.
		bool print_timing = options.getBool(Option::PRINT_TIMING);

		if (verbosity > 1)
		{
			Log.info(2) << "creating boundary..." << endl;
		}
					
		// check whether all requirements are met			
		if (phi_grid == 0)
		{
			error_code_ = FDPB::ERROR__PHI_GRID_REQUIRED;
			return false;
		}

		if (atom_array == 0)
		{
			error_code_ = FDPB::ERROR__ATOM_ARRAY_REQUIRED;
			return false;
		}

		// extract options 
		options.setDefaultReal(Option::IONIC_STRENGTH, Default::IONIC_STRENGTH);
		float ionic_strength = options.getReal(Option::IONIC_STRENGTH);

		options.setDefaultReal(Option::SOLVENT_DC, Default::SOLVENT_DC);
		float solvent_dielectric_constant = options.getReal(Option::SOLVENT_DC);

		options.setDefaultReal(Option::TEMPERATURE, Default::TEMPERATURE);
		float temperature = options.getReal(Option::TEMPERATURE);


		int boundary_condition;
		options.setDefault(Option::BOUNDARY, Default::BOUNDARY);

		if (options[Option::BOUNDARY] == FDPB::Boundary::DEBYE) 
		{
			boundary_condition = 1;
		} 
		else if (options[Option::BOUNDARY] == FDPB::Boundary::ZERO) 
		{
			boundary_condition = 0;
		} 
		else if (options[Option::BOUNDARY] == FDPB::Boundary::DIPOLE) 
		{
			boundary_condition = 2;
		}	
		else if (options[Option::BOUNDARY] == FDPB::Boundary::COULOMB) 
		{
			boundary_condition = 3;
		}	
		else if (options[Option::BOUNDARY] == FDPB::Boundary::FOCUSING) 
		{
			boundary_condition = 4;
		} 
		else 
		{
			Log.error() << "FDPB::setupBoundary: unknown boundary condition type: " << options[Option::BOUNDARY]
							    << " (possible types: " << FDPB::Boundary::DEBYE << " " 
									<< FDPB::Boundary::ZERO << " "
									<< FDPB::Boundary::DIPOLE << " "
									<< FDPB::Boundary::COULOMB << " "
									<< FDPB::Boundary::FOCUSING << ")" << endl;
			error_code_ = FDPB::ERROR__UNKNOWN_BOUNDARY_CONDITION_TYPE;
			return false;
		}

		///////
		// now setup the boundary conditions 

		// Debye length
		float beta;


		// calculate the Debye length \beta for the given 
		// ionic strength:
		//
		//  \beta = \frac{1}{\sqrt{\frac{2 N_A e_0^2 I}{\varepsilon \varepsilon_0 k T}}}
		//
		//	With:
		//			N_A:							Avogadro Constant
		//      E_0:							positron charge
		//			I:								ionic strength (in units of mol/m^3)
		//			\varepsilon:			solvent dielectric constant
		//			\varepsilon_0:		vacuum dielectric constant
		//			k:								Boltzmann constant
		//			T:								temperature 
		using namespace Constants;
		if (ionic_strength != 0.0)
		{
			beta = 1.0 / sqrt((2.0 * NA * e0 * e0 * ionic_strength)
											 / (VACUUM_PERMITTIVITY * solvent_dielectric_constant * k * temperature));
		}
		else 
		{
			// if the ionic strength equals zero, set beta to MAX_FLOAT
			// since the Debye length becomes infinity
			beta = Limits<float>::max();
		}
													

		// variable to hold the calculated grid index
		long idx;

		// variable to hold the distance between the current grid_point
		// and the current atom
		float distance;

		// needed for dipole method: contains negative and positive charge
		// and its positions in space
		float					positive_x, positive_y, positive_z;
		float					negative_x, negative_y, negative_z;
		float					positive_charge, negative_charge;
		Vector3			positive_vector, negative_vector;

		// loop variables
		long x, y, z, i;
				
		long Nx = (long)phi_grid->getMaxXIndex() + 1L;
		long Ny = (long)phi_grid->getMaxYIndex() + 1L;
		long Nz = (long)phi_grid->getMaxZIndex() + 1L;
		long Nxy = Nx * Ny;

		float charge;
		Vector3	position;
					
		switch (boundary_condition) 
		{	
			case 1:  // Debye approximation for the potential

				if (verbosity > 3)
				{
					Log.info(4) << "setting up xy-planes..." << endl;
				}

				// first, calculate the values for the first and last
				// xy-plane of the grid
		
				for (z = 0; z < Nz; z += Nz - 1)
				{
					for (y = 0; y < Ny; y++)
					{
						for (x = 0; x < Nx; x++)
						{
							// calculate the current grid point`s index
							idx = x + y * Nx + z * Nxy;

							(*phi_grid)[idx] = 0.0;
						
							for (i = 0; i < (Index)atom_array->size(); i++)
							{
							
								// retrieve the atom`s charge and position
								charge = (*atom_array)[i].q;
								position.set((*atom_array)[i].x, 
														 (*atom_array)[i].y, 
														 (*atom_array)[i].z);

								// calculate distance in meters
								distance  = position.getDistance(phi_grid->getGridCoordinates(idx)) * 1e-10;

								// now, calculate the potential caused by this atom at the grid point (x/y/z)
								//		
								//	 phi = \frac{q_i * e_0}{4 \pi \varepsilon \varepsilon_0} * \frac{e^{-r/\beta}}{r}
								//
								//	with	q_i:						the atom's charge
								//				\varepsilon:		the solvent dielectric constant
								//				\varespilon_0:	vacuum dielectric constant
								//				r:							distance grid point - atom
								//				\beta:					Debye length, see above
								//

								(*phi_grid)[idx] += e0 * charge / (4.0  * PI 
																	 * solvent_dielectric_constant * VACUUM_PERMITTIVITY )
																	 * exp(- distance / beta) / distance;
							}
						}
					}
				}
	
				if (verbosity > 3)
				{
				 	Log.info(4) << "setting up xz-planes..." << endl;
				}

				// second, calculate the values for the first and last
				// xz-plane of the grid, 
				for (y = 0; y < Ny; y += (Nx - 1))
				{
					for (z = 0; z < Nz; z++)
					{
						for (x = 0; x < Nx; x++)
						{
							// calculate the current grid point`s index
							idx = x + y * Nx + z * Nxy;

							(*phi_grid)[idx] = 0.0;

							for (i = 0; i < (Index)atom_array->size(); i++)
							{
								charge = (*atom_array)[i].q;
								position.set((*atom_array)[i].x, (*atom_array)[i].y, (*atom_array)[i].z);
								distance  = position.getDistance(phi_grid->getGridCoordinates(idx)) * 1e-10;
								(*phi_grid)[idx] += e0 * charge / (4.0  * PI
																	* solvent_dielectric_constant * VACUUM_PERMITTIVITY )
																	* exp (- distance / beta) / distance;
							}
						}
					}
				}

				if (verbosity > 3)
				{
					Log.info(4) << "setting up yz-planes..." << endl;
				}	

				// last, calculate the values for the first and last
				// yz-plane of the grid, 
				for (x = 0; x < Nx; x += (Nx - 1))
				{
					for (z = 0; z < Nz; z++)
					{
						for (y = 0; y < Ny; y++)
						{
							// calculate the current grid point`s index
							idx = x + y * Nx + z * Nxy;

							(*phi_grid)[idx] = 0.0;

							for (i = 0; i < (long)atom_array->size(); i++)
							{
								charge = (*atom_array)[i].q;
								position.set((*atom_array)[i].x, 
														 (*atom_array)[i].y, 
														 (*atom_array)[i].z);

								distance  = position.getDistance(phi_grid->getGridCoordinates(idx)) * 1e-10;
								(*phi_grid)[idx] += e0 * charge / (4.0  * PI
																	* solvent_dielectric_constant * VACUUM_PERMITTIVITY )
																	* exp (- distance / beta) / distance;
							}
						}
					}
				}

				break;

			case 2:  // Dipole approximation for the potential

				if (verbosity > 3)
				{
					Log.info(4) << "calculating effective dipole..." << endl;
				}

				positive_x = 0.0;
				positive_y = 0.0;
				positive_z = 0.0;
				negative_x = 0.0;
				negative_y = 0.0;
				negative_z = 0.0;

				positive_charge = 0.0;
				negative_charge = 0.0;

				for (i = 0; i < (Index)atom_array->size(); i++)
				{
					charge = (*atom_array)[i].q;
					if (charge > 0.0)
					{
						positive_charge += charge;
						positive_x += (*atom_array)[i].x * charge;
						positive_y += (*atom_array)[i].y * charge;
						positive_z += (*atom_array)[i].z * charge;
					} 
					else 
					{
						negative_charge += charge;
						negative_x += (*atom_array)[i].x * charge;
						negative_y += (*atom_array)[i].y * charge;
						negative_z += (*atom_array)[i].z * charge;
					}
				}
					
				if (positive_charge != 0.0)
				{
					positive_x /= positive_charge;
					positive_y /= positive_charge;
					positive_z /= positive_charge;
				}

				if (negative_charge != 0.0)
				{
					negative_x /= negative_charge;
					negative_y /= negative_charge;
					negative_z /= negative_charge;
				}

				negative_vector.set(negative_x, negative_y, negative_z);
				positive_vector.set(positive_x, positive_y, positive_z);
					
				if (verbosity > 1)
				{
					Log.info(2) << "assigned negative charge: " << negative_charge 
											<< " at (" << negative_x << "/" << negative_y << "/" << negative_z << ")" << endl;
					Log.info(2) << "assigned positive charge: " << positive_charge 
					   					<< " at (" << positive_x << "/" << positive_y << "/" << positive_z << ")" << endl;
				}
		
					
				if (verbosity > 3)
				{
					 Log.info(4) << "setting up xy-planes..." << endl;
				}

				// first, calculate the values for the first and last
				// xy-plane of the grid
				for (z = 0; z < Nz; z += Nz - 1)
				{
					for (y = 0; y < Ny; y++)
					{
						for (x = 0; x < Nx; x++)
						{
							// calculate the current grid point`s index
							idx = x + y * Nx + z * Nxy;

							(*phi_grid)[idx] = 0.0;
						
							// calculate distance in meters
							distance = positive_vector.getDistance(phi_grid->getGridCoordinates(idx)) * 1e-10;

							/* now, calculate the potential caused by this atom at the grid point (x/y/z)
										
									 phi = \frac{q_i * e_0}{4 \pi \varepsilon \varepsilon_0} * \frac{e^{-r/\beta}}{r}
								
									with	q_i:						the atom's charge
												\varepsilon:		the solvent dielectric constant
												\varespilon_0:	vacuum dielectric constant
												r:							distance grid point - atom
												\beta:					Debye length, see above
							*/

							if (beta != 0.0)
							{
								(*phi_grid)[idx] += e0 * positive_charge / (4.0  * PI 
																	 * solvent_dielectric_constant * VACUUM_PERMITTIVITY )
																	 * exp(- distance / beta) / distance;

								// and now for the negative charge
								distance = negative_vector.getDistance(phi_grid->getGridCoordinates(idx)) * 1e-10;

								(*phi_grid)[idx] += e0 * negative_charge / (4.0  * PI 
																	 * solvent_dielectric_constant * VACUUM_PERMITTIVITY )
																	 * exp(- distance / beta) / distance;
							} 
						}
					}
				}
		
				if (verbosity > 3)
				{
					Log.info(4) << "setting up xz-planes..." << endl;
				}

				// second, calculate the values for the first and last
				// xz-plane of the grid, 
				for (y = 0; y < Ny; y += (Nx - 1))
				{
					for (z = 0; z < Nz; z++)
					{
						for (x = 0; x < Nx; x++)
						{
							// calculate the current grid point`s index
							idx = x + y * Nx + z * Nxy;

							(*phi_grid)[idx] = 0.0;

							// calculate distance in meters
							distance = positive_vector.getDistance(phi_grid->getGridCoordinates(idx)) * 1e-10;
							(*phi_grid)[idx] += e0 * positive_charge / (4.0  * PI 
																 * solvent_dielectric_constant * VACUUM_PERMITTIVITY )
																 * exp(- distance / beta) / distance;

							// and now for the negative charge
							distance = negative_vector.getDistance(phi_grid->getGridCoordinates(idx)) * 1e-10;

							(*phi_grid)[idx] += e0 * negative_charge / (4.0  * PI 
																 * solvent_dielectric_constant * VACUUM_PERMITTIVITY )
																 * exp(- distance / beta) / distance;
						}
					}
				}

				if (verbosity > 3)
				{
					Log.info(4) << "setting up yz-planes..." << endl;
				}

				// last, calculate the values for the first and last
				// yz-plane of the grid, 
				for (x = 0; x < Nx; x += (Nx - 1))
				{
					for (z = 0; z < Nz; z++)
					{
						for (y = 0; y < Ny; y++)
						{
							// calculate the current grid point`s index
							idx = x + y * Nx + z * Nxy;

							(*phi_grid)[idx] = 0.0;

							// calculate distance in meters
							distance = positive_vector.getDistance(phi_grid->getGridCoordinates(idx)) * 1e-10;
							(*phi_grid)[idx] += e0 * positive_charge / (4.0  * PI 
																 * solvent_dielectric_constant * VACUUM_PERMITTIVITY )
																 * exp(- distance / beta) / distance;

							// and now for the negative charge
							distance = negative_vector.getDistance(phi_grid->getGridCoordinates(idx)) * 1e-10;

							(*phi_grid)[idx] += e0 * negative_charge / (4.0  * PI 
																 * solvent_dielectric_constant * VACUUM_PERMITTIVITY )
																 * exp(- distance / beta) / distance;
						}
					}
				}
				break;

			case 4:	// use focusing: solve FDPB on a grid with spacing x 4 and dimension x 2
				
				if (boundary_condition == 4)
				{
					// create a FDPB object with double the size (in each direction)
					// and four times the spacing
					FDPB	focusing_grid;
					focusing_grid.options = options;
					focusing_grid.options[Option::BOUNDARY] = Boundary::DIPOLE;
					Vector3 focusing_lower = lower_;
					Vector3 focusing_upper = upper_;
					focusing_lower -= (upper_ - lower_) * 0.5;
					focusing_upper += (upper_ - lower_) * 0.5;
					focusing_grid.options.setVector(Option::LOWER, focusing_lower);
					focusing_grid.options.setVector(Option::UPPER, focusing_upper);
					focusing_grid.options[Option::SPACING] = spacing_ * 2.0;

					System S;
					Protein P;
					Chain C;
					Residue R;
					S.insert(P);
					P.insert(C);
					C.insert(R);
					vector<FDPB::FastAtom>::iterator atom_array_it= atom_array->begin();
					for (; atom_array_it != atom_array->end(); ++atom_array_it)
					{
						PDBAtom* atom = new PDBAtom;
						R.insert(*atom);
						atom->setPosition(Vector3(atom_array_it->x, atom_array_it->y, atom_array_it->z));
						atom->setCharge(atom_array_it->q);
						atom->setRadius(atom_array_it->r);
					}

					if (verbosity > 1)
					{
						Log.info() << "setting up focusing grid." << endl;
					}
					// setup the focusing grid
					focusing_grid.setup(S);

					// solve the FDPB 
					if (verbosity > 1)
					{
						Log.info() << "solving equations for focusing grid." << endl;
					}
					focusing_grid.solve();
					
					// now iterate over all points of the final grid an assign their 
					// potential from the focusing grid
					if (verbosity > 1)
					{
						Log.info() << "copying focusing grid to final grid" << endl;
					}
					
					for (z = 0; z < Nz; z++)
					{
						for (y = 0; y < Ny; y++)
						{
							for (x = 0; x < Nx; x++)
							{
								// calculate the current grid point`s index
								idx = x + y * Nx + z * Nxy;

								// assign the interpolated value to the focusing grid
								(*phi_grid)[idx] = focusing_grid.phi_grid->getInterpolatedValue(phi_grid->getGridCoordinates(idx));
							}
						}
					}

					// remove all unnedded data structure now!
					focusing_grid.destroy();
				}
				break;
				

			case 0: // set boundary to a potential of zero 
							// (phi is already set to zero everywhere - so simply do nothing)
					;
		}// switch

		step_timer.stop();
		if (print_timing && (verbosity > 1))
		{
			Log.info(2) << "setupBoundary: " << step_timer.getCPUTime() << endl;
		}

		return true;
	}

	bool FDPB::setup(System& system)
	{
		// create a timer to determine the method's runtime
		Timer	setup_timer;
		setup_timer.start();

		options.setDefaultInteger(Option::VERBOSITY, Default::VERBOSITY);
		options.setDefaultBool(Option::PRINT_TIMING, Default::PRINT_TIMING);

		// first, check whether we should tell to our user what we`re doing
		int verbosity = (int)options.getInteger(Option::VERBOSITY);

		// ...and whether we should tell how long it took us.
		bool print_timing = options.getBool(Option::PRINT_TIMING);

		if (!setupAtomArray(system))
		{
			return false;
		}

		if (!setupEpsGrid(system))
		{
			return false;
		}

		if (!setupSASGrid(system))
		{
			return false;
		}
		
		if (!setupKappaGrid())
		{
			return false;
		}
				
		if (!setupPhiGrid())
		{
			return false;
		}

		if (!setupQGrid())
		{
			return false;
		}

		if (!setupBoundary())
		{
			return false;
		}

		setup_timer.stop();
		if (print_timing)
		{
			results["setup_CPU_time"] = setup_timer.getCPUTime();
			results["setup_wall_time"] = setup_timer.getClockTime();
			if (verbosity > 0)
			{
				Log.info(1) << "setup time: " << setup_timer.getCPUTime() << endl;
			}
		}

		return true;
	}

	bool FDPB::setup(System& system, Options& new_options)
	{
		options = new_options;
		return setup(system);
	}
						
	bool FDPB::solve()
  {
		// determine the run time
		Timer	solve_timer;
		solve_timer.start();


		if ((phi_grid == 0) || (eps_grid == 0) || (q_grid == 0))
		{
			error_code_ = FDPB::ERROR__SETUP_REQUIRED;
			return false;
		}

		bool print_timing = options.getBool(Option::PRINT_TIMING);
		int verbosity = (int)options.getInteger(Option::VERBOSITY);

		float*	phi;
		float*	T;
		float*	Q;
		float*	tmp_phi;

		// some generally used loop variables
		unsigned long	i;
		unsigned long	j, k, l;

		// retrieve some basic grid properties and set the 
		// corresponding variables

		unsigned long Nx = q_grid->getMaxXIndex() + 1;
		unsigned long Ny = q_grid->getMaxYIndex() + 1;
		unsigned long Nz = q_grid->getMaxZIndex() + 1;

		unsigned long Nxy = Nx * Ny;
		unsigned long N = Nxy * Nz;

		Q	=	new float[N];
		tmp_phi	= new float[N];
		if (tmp_phi == 0)
		{
			throw Exception::OutOfMemory(__FILE__, __LINE__, N * sizeof(float));
		}

		// the potential will remain in its grid,
		// we just take phi for a more convenient
		// access
		phi = phi_grid->data;

		if (verbosity > 0)
		{
		 	Log.info(1) << "setting up some arrays..." << endl;
		}

		// now, setup d and Q    
		// d contains  2 / ( 6 \sum \varepsilon_i ) 
		// Q is set to 4 \pi q_i / ( h * d_i )

		using namespace Constants;
		float d;
		for (i = 1; i < (Nx - 1); i++)
		{
			for (j = 1; j < (Nx - 1); j++)
			{
				for (k = 1; k < (Nx - 1); k++)
				{
          l = i + j * Nx + k * Nxy;
          d = 1 / ((*eps_grid)[(Index)l].x
									 + (*eps_grid)[(Index)l].y
									 + (*eps_grid)[(Index)l].z
									 + (*eps_grid)[(Index)(l - 1)].x
									 + (*eps_grid)[(Index)(l - Nx)].y
									 + (*eps_grid)[(Index)(l - Nxy)].z);
					Q[l] = e0 * *(q_grid->getData((Index)l)) / (1e-10 * VACUUM_PERMITTIVITY * spacing_) * d;
				}
			}
		}

		T	= new float[N * 6];
		if (T == 0)
		{
			throw Exception::OutOfMemory(__FILE__, __LINE__, 6 * N * sizeof(float));
		}

		// T[i] = 0   BAUSTELLE: muss das sein?
		for (i = 0; i < (6 * N); T[i++] = 0.0);

		using namespace Constants;
		for (i = 1; i < (Nx - 1); i++)
		{
			for (j = 1; j < (Nx - 1); j++)
			{
				for (k = 1; k < (Nx - 1); k++)
				{
					l = i + j * Nx + k * Nxy;
					d = 1 / ((*eps_grid)[(Index)l].x
										+ (*eps_grid)[(Index)l].y
										+ (*eps_grid)[(Index)l].z
										+ (*eps_grid)[(Index)(l - 1)].x
										+ (*eps_grid)[(Index)(l - Nx)].y
										+ (*eps_grid)[(Index)(l - Nxy)].z);

					T[(Index)(6 * l)    ]  = (*eps_grid)[(Index)l].x * d;
					T[(Index)(6 * l + 1)]  = (*eps_grid)[(Index)(l - 1)].x * d;
					T[(Index)(6 * l + 2)]  = (*eps_grid)[(Index)l].y * d;
					T[(Index)(6 * l + 3)]  = (*eps_grid)[(Index)(l - Nx)].y * d;
					T[(Index)(6 * l + 4)]  = (*eps_grid)[(Index)l].z * d;
					T[(Index)(6 * l + 5)]  = (*eps_grid)[(Index)(l - Nxy)].z * d;
				}
			}
		}

		if (verbosity > 0)
		{
			Log.info(1) << "calculating charged grid points..." << endl;
		}

		// Now, find out which grid points are charged and store them (or, 
		// more precisely, their indices) into two arrays
		long					number_of_charged_black_points; 
		long					number_of_charged_white_points;

		// pointer to array to hold the indices
		long*				charged_black_points;
		long*				charged_white_points;
		

		// get the number of charged grid_points
		number_of_charged_black_points = 0;
		number_of_charged_white_points = 0;
		
		for (k = 1; k < Nz - 1; k++)
		{
			for (j = 1; j < Ny - 1; j++)
			{
				for (i = 1; i < Nx - 1; i++)
				{
					l = i + j * Nx + k * Nxy;
					if (Q[l] != 0.0)
					{
						if ((i + j + k) % 2 == 1)
						{
							number_of_charged_black_points++;
						}
						else
						{
							number_of_charged_white_points++;
						}
					}
				}
			}
		}
		
		charged_black_points = new long[number_of_charged_black_points];
		if (charged_black_points == 0)
		{
			throw Exception::OutOfMemory(__FILE__, __LINE__, number_of_charged_black_points * sizeof(long));
		}

		charged_white_points = new long[number_of_charged_white_points];
		if (charged_white_points == 0)
		{
			throw Exception::OutOfMemory(__FILE__, __LINE__, number_of_charged_white_points * sizeof(long));
		}


		number_of_charged_black_points = 0;
		number_of_charged_white_points = 0;
		
		for (k = 1; k < Nz - 1; k++)
		{
			for (j = 1; j < Ny - 1; j++)
			{
				for (i = 1; i < Nx - 1; i++)
				{
					l = i + j * Nx + k * Nxy;
					if (Q[l] != 0.0)
					{
						if ((i + j + k) % 2 == 1)
						{
							charged_black_points[number_of_charged_black_points++] = (long)l;
						}
						else
						{
							charged_white_points[number_of_charged_white_points++] = (long)l;
						}
					}
				}
			}
		}
				
		if (verbosity > 70)
		{
			Log.info(71) << "number of charged grid points: "
					 << number_of_charged_black_points + number_of_charged_white_points
					 << " ("
					 << (float)((double)number_of_charged_black_points + (double)number_of_charged_white_points) / (double)N * 100.0
					 << "%)" << endl;
		}

		// setting up atom_array
		// This array should contain all _ CHARGED _ atoms and their
		// position. For each atom, the index of the closest grid
		// point is calculated to speed up the evaluation of the
		// electrostatic energy

		PointGrid<float>::GridIndex		grid_index;
		for ( i = 0; i < atom_array->size(); i++)
		{
			grid_index = phi_grid->getIndex((*atom_array)[i].x, (*atom_array)[i].y, (*atom_array)[i].z);
			(*atom_array)[i].index = grid_index.x + grid_index.y * Nx + grid_index.z * Nxy;
		}
		
		if (verbosity > 0)
		{
			Log.info(1) << "starting iterations." << endl;
		}

		long x, y, z;
					
		long max_iterations;
		if (options.isSet(Option::MAX_ITERATIONS))
		{
			max_iterations = options.getInteger(Option::MAX_ITERATIONS);
		} 
		else 
		{
			max_iterations = Default::MAX_ITERATIONS;
		}

		long check_after_iterations;
		if (options.isSet(Option::CHECK_AFTER_ITERATIONS))
		{
			check_after_iterations = options.getInteger(Option::CHECK_AFTER_ITERATIONS);
		} 
		else 
		{
			check_after_iterations = Default::CHECK_AFTER_ITERATIONS;
		}
		

		// iteration counts the iterations
		long									iteration;
		iteration = 0;

		// needed for determination of convergence
		float residual;
		float residual_norm2;
		float max_residual;
		float rms_change;
		// These two variables contain the thresholds
		// needed to determine convergence.
		// Convergence is reached, if 
		//      rms_change <= rms_criterion
		//  and max_residual max_criterion
		// (see also while loop below)
		options.setDefaultReal(Option::RMS_CRITERION, Default::RMS_CRITERION);
		float rms_criterion = options.getReal(Option::RMS_CRITERION);

		options.setDefaultReal(Option::MAX_CRITERION, Default::MAX_CRITERION);
		float max_criterion = options.getReal(Option::MAX_CRITERION);

		// Now, make sure the loop won't stop before the first
		// evaluation of convergence
		max_residual = max_criterion + 1;
		rms_change = rms_criterion + 1;

		// omega: SOR parameter
		// lambda: 1 - omega
		float omega;
		float lambda;
		long black, white;

		// Gauss-Seidel spectral radius (squared value
		// of the Jacobi spectral radius)
		// needed for Chebyshev acceleration
		float spectral_radius;

		// calculate maximum eigenvalue from grid dimension
		// and use it if no explicit spectral_radius has been defined
		options.setDefaultReal("spectral_radius", cos(Constants::PI / (double)Nx));
		spectral_radius = options.getReal("spectral_radius");

		omega = 1;
		lambda = 1 - omega;

		// iterate, while max. number of iterations hasn't been reached 
		// and convergence criterions aren't met.
		while ((iteration < max_iterations)  && ((max_residual > max_criterion) || (rms_change > rms_criterion)))
		{

			// first half of Gauss-Seidel iteration (black fields only)
			for (z = 1; z < (long)(Nx - 1); z++)
				for (y = 1; y < (long)(Nx - 1); y++)
				{
					black = ((y % 2) + (z % 2)) % 2;
					i = y * Nx + z * Nxy + 1 + black;
					for (x = 1 + black; x < (long)(Nx - 1); x += 2)
					{
						phi[i] = omega * (T[6 * i    ] * phi[i + 1 ]
															+ T[6 * i + 1] * phi[i - 1 ]
															+ T[6 * i + 2] * phi[i + Nx ]
															+ T[6 * i + 3] * phi[i - Nx ]
															+ T[6 * i + 4] * phi[i + Nxy]
															+ T[6 * i + 5] * phi[i - Nxy])
										 + lambda * phi[i];
						i += 2;
					}
				}

			long* charge_pointer;
			charge_pointer = charged_black_points;
			for (charge_pointer = charged_black_points;
					 charge_pointer < &charged_black_points[number_of_charged_black_points]; 
					 charge_pointer++)
			{

				phi[*charge_pointer] += omega * Q[*charge_pointer];
			}
									
			// Chebyshev acceleration: omega approaches its
			// optimal value asymptotically. This usually gives
			// better convergence for the first few iterations
			if (spectral_radius != 0.0)
			{
				if (l == 0)
					omega = 1 / (1 - spectral_radius / 2);
				else
					omega = 1 / (1 - spectral_radius * omega / 4);
				lambda = 1 - omega;
			}

			// second half of Gauss-Seidel iteration (white fields only)
			for (z = 1; z < (long)(Nx - 1); z++)
				for (y = 1; y < (long)(Nx - 1); y++)
				{
					white = 1 - ((y % 2) + (z % 2)) % 2;
					i = y * Nx + z * Nxy + 1 + white;
					for (x = 1 + white; x < (long)(Nx - 1); x += 2)
					{
						phi[i] = omega * (T[6 * i    ] * phi[i + 1 ]
															+ T[6 * i + 1] * phi[i - 1 ]
															+ T[6 * i + 2] * phi[i + Nx ]
															+ T[6 * i + 3] * phi[i - Nx ]
															+ T[6 * i + 4] * phi[i + Nxy]
															+ T[6 * i + 5] * phi[i - Nxy])
										 + lambda * phi[i];
						i += 2;
					}
				}


		
			charge_pointer = charged_white_points;
			for (charge_pointer = charged_white_points;
					 charge_pointer < &charged_white_points[number_of_charged_white_points]; 
					 charge_pointer++)
			{
				phi[*charge_pointer] += omega * Q[*charge_pointer];
			}

			// Chebyshev acceleration for the second Gauss-Seidel step
			if (spectral_radius != 0.0)
			{
				omega = 1 / (1 - spectral_radius * omega / 4);
				lambda = 1 - omega;
			}


			// calculate the gradient every check_after_iterations
			if ((iteration % check_after_iterations) == 0)
			{
					
				max_residual = 0;
				residual_norm2 = 0;

				// sum up all squared changes in the phi array since
				// the last iteration
				for (i = 1; i < N - 1; i++)
				{
					residual = BALL_ABS(tmp_phi[i] - phi[i]);
					max_residual = BALL_MAX(residual, max_residual);
					residual_norm2 += residual * residual;
				}
					
				rms_change = sqrt(residual_norm2 / (float)N);
				
				if (verbosity > 0)
					Log.info(1) << "Iteration " << iteration << " RMS: " << rms_change << "   MAX: " << max_residual << endl;
			}

			if (((iteration + 1) % check_after_iterations) == 0)
			{
				// save the actual settings phi
				memcpy(tmp_phi, phi, N * sizeof(phi[0]));
			}
					
			// increase iteration count
			iteration++;
		}

		if ((rms_change <= rms_criterion) && (max_residual <= max_criterion))
		{
			if (verbosity > 0)
			{
				Log.info(1) << "Converged." << endl;
			}

			results.setBool("converged", true);
			number_of_iterations_ = iteration;
		} 
		else 
		{
			if (verbosity > 0)
			{
				Log.warn(1) << "Not converged." << endl;
			}

			results.setBool("converged", false);
			number_of_iterations_ = 0;
		}



		// free all superfluous arrays
		delete [] Q;
		delete [] T;
		delete [] tmp_phi;

		// calculate the total electrostatic energy (in units of kJ/mol)
		//
		// E =  1/2 * \sum \phi_i q_i
		// 
		energy_ = 0;
		for (i = 0; i < (unsigned long)number_of_charged_black_points; i++)
		{
			l = charged_black_points[i];
			energy_ += phi[l] * (*q_grid)[(Index)l];
		}
		for (i = 0; i < (unsigned long)number_of_charged_white_points; i++)
    {
			l = charged_white_points[i];
			energy_ += phi[l] * (*q_grid)[(Index)l];
		}


		// throw away the arrays containing the charged points
		delete [] charged_white_points;
		delete [] charged_black_points;

		// Save some multiplications till the end...
		// This factor is 0.5 from above formula and 1/1000 from 
		// the conversion from J/mol -> kJ/mol,
		// the electron charge to convert to C
		// and Avogadro`s number to convert to molar
		// energies.
		using namespace Constants;
		energy_ *= e0 * NA * 0.5 * 1E-3;

		results.setInteger("iterations", iteration);
		results.setReal("final energy", energy_);

		if (verbosity > 0)
		{
			Log.info() << "final energy after " << iteration << " iterations: " << energy_ << " kJ/mol" << endl;
		}

		// calculate the reaction field energy
		reaction_field_energy_ = calculateReactionFieldEnergy();
		results.setReal("reaction field energy", reaction_field_energy_);
		if (verbosity > 0)
		{
			Log.info() << "reaction field energy: " << reaction_field_energy_ << " kJ/mol" << endl;
		}

		// throw away the eps grid
		delete eps_grid;
		eps_grid = 0;

		// delete the q_grid
		delete q_grid;
		q_grid = 0;
			
		solve_timer.stop();
		if (print_timing)
		{
			results["CPU time to solve equations"] = solve_timer.getCPUTime();
			results["Wall time to solve equations"] = solve_timer.getClockTime();
			if (verbosity > 0)
			{
				Log.info(1) << "CPU time to solve equations: " << solve_timer.getCPUTime() << endl;
				Log.info(1) << "Wall time to solve equations: " << solve_timer.getClockTime() << endl;
			}
		}


		// return true: everything went well
		return true;
	}


	void FDPB::destroy()
	{
		destroyGrids();
		options.destroy();
		results.destroy();

		if (atom_array != 0)
		{
			delete atom_array;
			atom_array = 0;
		}

		energy_ = 0;
		number_of_iterations_ = 0;
	}

	void FDPB::destroyGrids()
	{
		if (eps_grid != 0)
		{
			delete eps_grid;
			eps_grid = 0;
		}

		if (kappa_grid != 0)
		{
			delete kappa_grid;
			kappa_grid = 0;
		}

		if (q_grid != 0)
		{
			delete q_grid;
			q_grid = 0;
		}

		if (phi_grid != 0)
		{
			delete phi_grid;
			phi_grid = 0;
		}

		if (SAS_grid != 0)
		{
			delete SAS_grid;
			SAS_grid = 0;
		}
	}
	
	double FDPB::calculateReactionFieldEnergy() const
	{
		// create a timer to determine the method's runtime
		Timer	step_timer;
		step_timer.start();

		// If the system does not contain any atoms, there is nothing to calculate
		// 
		if (boundary_points_.size() == 0)
		{
			return 0.0;
		}


		// the distance between the boundary points and the atom surfaces 
		Vector3 boundary_point;
		
		// the vector from the grid point to the atom center
		Vector3 distance_vector;

		Vector3 min_distance_vector;
		float distance;
		float min_distance;

		// clear the reaction_field energy
		double energy = 0.0;

		vector<FastAtom>::const_iterator atom_iterator;
		
		// the position of the image charge (on the molecular surface)
		Vector3 image_position;
		Position i;
		for (i = 0; i < boundary_points_.size(); i++) 
		{
			boundary_point = phi_grid->getGridCoordinates(boundary_points_[i]);

			// 1. Calculate the nearest atom surface and the difference vector
			min_distance = Limits<float>::max();
			min_distance_vector = Vector3(0.0);
			for (atom_iterator = atom_array->begin(); atom_iterator != atom_array->end(); ++atom_iterator)
			{
				distance_vector = boundary_point - Vector3(atom_iterator->x, atom_iterator->y, atom_iterator->z);
				distance = distance_vector.getLength() - atom_iterator->r;
				if (distance < min_distance) 
				{
					min_distance = distance;
					min_distance_vector = distance_vector;
				}
			}
				
			// 2. Beam it.
			if (min_distance < Limits<float>::max())
			{
				image_position = boundary_point - min_distance_vector.normalize() * min_distance;

				// 3. compute the electrostatic field 

				Position llf, rlf, luf, ruf, llb, rlb, lub, rub;

				// if the image charge is not inside the grid, it does not contribute to the 
				// energy of the reaction field

				if (phi_grid->getBoxIndices(image_position, llf, rlf, luf, ruf, llb, rlb, lub, rub)) 
				{
					PointGrid<float>::GridIndex grid_index = phi_grid->getIndex(image_position);
					Size Nx = phi_grid->getMaxXIndex() + 1;
					Size Nxy = (phi_grid->getMaxYIndex() + 1) * Nx;
					Position idx = grid_index.x + grid_index.y * Nx + grid_index.z * Nxy;

					double dPhi = phi_grid->data[idx]
											- (phi_grid->data[idx - 1] + phi_grid->data[idx + 1]
												 + phi_grid->data[idx - Nx] + phi_grid->data[idx + Nx]
												 + phi_grid->data[idx - Nxy] + phi_grid->data[idx + Nxy]) / 6.0;
					double delta_E_dS = dPhi * spacing_ * 1e-10 * 6.0;

					// delta_i is in units of Coulomb (C)
					double delta_i = Constants::VACUUM_PERMITTIVITY * delta_E_dS;
					
					// subtract the grid charge (if any)
					if (q_grid != 0)
					{
						delta_i -= (*q_grid)[boundary_points_[i]] * Constants::e0;
					}
					// compute the charge's energy
					for (atom_iterator = atom_array->begin(); atom_iterator != atom_array->end(); ++atom_iterator)
					{
						// calculate the distance of the charge
						distance_vector.set(atom_iterator->x, atom_iterator->y, atom_iterator->z);
						distance_vector -= image_position;

						// calculate the coulomb energy caused by this image charge
						// with every atom charge						
						double phi_q = delta_i 
												/ (distance_vector.getLength() * 1e-10 * 4.0 
													 * Constants::PI * Constants::VACUUM_PERMITTIVITY);
						energy += atom_iterator->q * Constants::e0 * phi_q;
					}
				}
			}
		}

		// convert the energy to units of kJ/mol
		// and scale it by one half (since E = 1/2 \sum \phi q)
		return 0.5 * energy * 1e-3 * Constants::NA;
	}
			
} // namespace BALL 
