// $Id: periodicBoundary.C,v 1.14 2001/01/26 15:00:27 anker Exp $

#include <BALL/MOLMEC/COMMON/periodicBoundary.h>
#include <BALL/MOLMEC/COMMON/forceField.h>
#include <BALL/MOLMEC/COMMON/support.h>
#include <BALL/STRUCTURE/geometricProperties.h>
#include <BALL/STRUCTURE/geometricTransformations.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/CONCEPT/processor.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/MATHS/common.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/PTE.h>

using namespace std;

namespace BALL 
{

	const char* PeriodicBoundary::Option::PERIODIC_BOX_LOWER = "periodic_box_lower";
	const char* PeriodicBoundary::Option::PERIODIC_BOX_UPPER = "periodic_box_upper";
	const char* PeriodicBoundary::Option::PERIODIC_BOX_ENABLED = "periodic_box_enabled";
	const char* PeriodicBoundary::Option::PERIODIC_BOX_DISTANCE = "periodic_box_distance";
	const char* PeriodicBoundary::Option::PERIODIC_BOX_ADD_SOLVENT = "periodic_box_add_solvent";
	const char* PeriodicBoundary::Option::PERIODIC_BOX_SOLVENT_FILE = "periodic_box_solvent_file";
	const char* PeriodicBoundary::Option::PERIODIC_BOX_SOLVENT_SOLUTE_DISTANCE = "periodic_box_solvent_solute_distance";
	const char* PeriodicBoundary::Option::PERIODIC_WATER_FILE = "periodic_water_file";
 
	const bool    PeriodicBoundary::Default::PERIODIC_BOX_ENABLED = false;
	const float   PeriodicBoundary::Default::PERIODIC_BOX_DISTANCE = 5.0;
	const bool    PeriodicBoundary::Default::PERIODIC_BOX_ADD_SOLVENT = true;
	const char*   PeriodicBoundary::Default::PERIODIC_BOX_SOLVENT_FILE = "solvents/water.hin";
	const float   PeriodicBoundary::Default::PERIODIC_BOX_SOLVENT_SOLUTE_DISTANCE = 2.3;
	const char*   PeriodicBoundary::Default::PERIODIC_WATER_FILE = "solvents/water.hin";

	// Default Constructor
	PeriodicBoundary::PeriodicBoundary()
		:	force_field_(0), 
			enabled_(false)
	{
	}

	// Constructor
	PeriodicBoundary::PeriodicBoundary(const ForceField& force_field)
		: options(const_cast<Options*>(&force_field.options)),
			force_field_(const_cast<ForceField*>(&force_field)), 
			enabled_(false)
	{
	}

	// Copy Constructor
	PeriodicBoundary::PeriodicBoundary(const PeriodicBoundary& periodic_boundary)
		: options(periodic_boundary.options), 
			force_field_(periodic_boundary.force_field_), 
			enabled_(periodic_boundary.enabled_),
			box_(periodic_boundary.box_), 
			molecules_(periodic_boundary.molecules_)
	{
	}

	// Clear method
	void PeriodicBoundary::clear()
		throw()
	{
		options = 0;
		force_field_ = 0;
		enabled_ = false;
		box_.clear();
		molecules_.clear();
	}

	// Destructor
	PeriodicBoundary::~PeriodicBoundary()
	{
		clear();
	}

	// assignment operator
	PeriodicBoundary& PeriodicBoundary::operator = (const PeriodicBoundary& periodic_boundary)
	{
		// avoid self assignment
		if (&periodic_boundary != this)
		{
			force_field_	= periodic_boundary.force_field_;
			enabled_	= periodic_boundary.enabled_;
			options		= periodic_boundary.options;
			box_		= periodic_boundary.box_;
			molecules_	= periodic_boundary.molecules_;
		}

		return *this;
	}
 
	// Accessor for enabling periodic boundary
	void PeriodicBoundary::enable()
	{
		enabled_ = true;
	}

	// Accessor for disabling periodic boundary
	void PeriodicBoundary::disable()
	{
		enabled_ = false;
	}

	// Accessor for setting the box of the periodic boundary
	void PeriodicBoundary::setBox(const Box3& box)
	{
		box_.a = box.a;
		box_.b = box.b;
	}

	// Accessor for reading the box of the periodic boundary
	Box3 PeriodicBoundary::getBox() const
	{
		return box_;
	}

	//	Predicate for testing if the periodic boundary is enabled or not
	bool PeriodicBoundary::isEnabled() const
	{
		return enabled_;
	}

	// Accessor for updating the positions of the molecules:
	// If the center of gravity of a molecules lies outside the box of the periodic boundary,
	// the molecule is moved according to the periodic boundary conditions.
	void PeriodicBoundary::updateMolecules()
	{
		float mass;
		float atomic_mass;
		Vector3 center_of_gravity;
		AtomVector& atom = const_cast<AtomVector&>(force_field_->getAtoms());
		float shift_x = box_.b.x - box_.a.x;
		float shift_y = box_.b.y - box_.a.y;
		float shift_z = box_.b.z - box_.a.z;
		bool shift;
		Vector3 translation;

		AtomIndexArray::iterator it = molecules_.begin();

		// Iterate over all molecules stored in molecule_
		for ( ; it != molecules_.end() ; ++it) 
		{
			mass = 0;
			center_of_gravity.x = 0;
			center_of_gravity.y = 0;
			center_of_gravity.z = 0;
			shift = false;
			translation.x = 0;
			translation.y = 0;
			translation.z = 0;

			// Iterate over all atoms of the molecule and calculate the center of gravity 
			for (Size i = (*it).first; i < (*it).second ; i++) 
			{
				atomic_mass = atom[i]->getElement().getAtomicWeight();
				mass += atomic_mass;
				center_of_gravity += (atom[i]->getPosition() * atomic_mass);
			}  

			center_of_gravity /= mass;


			// Test if the center of gravity is outside the box and determine 
			// the translation (according to the periodic boundary definition)
			// that moves the center of gravity back into the box.

			if (center_of_gravity.x < box_.a.x) 
			{
				translation.x += shift_x;
				shift = true;
			} 
			else if (center_of_gravity.x > box_.b.x) 
			{
				translation.x -= shift_x;
				shift = true;
			}

			if (center_of_gravity.y < box_.a.y) 
			{
				translation.y += shift_y;
				shift = true;
			} 
			else if (center_of_gravity.y > box_.b.y) 
			{
				translation.y -= shift_y;
				shift = true;
			}

			if (center_of_gravity.z < box_.a.z) 
			{
				translation.z += shift_z;
				shift = true;
			} 
			else if (center_of_gravity.z > box_.b.z) 
			{
				translation.z -= shift_z;
				shift = true;
			}

			// Translate the atoms of the molecule if it has to be shifted
			if (shift) 
			{
				for (Size i = (*it).first; i < (*it).second ; i++) 
				{
					// BAUSTELLE: debugging code
					Log.info() << "PerBound: translating molecule " 
							<< atom[i]->getFullName() << "/" << i << " by " << translation << endl;
					atom[i]->setPosition(atom[i]->getPosition() + translation);
				}
			}
		}
	}

	Size PeriodicBoundary::generateMoleculesVector()
	{
		// Periodic boundary is enabled:
		// Generate molecules_ with the start and end indices of the atoms of the molecules in atom_. 
		vector<Atom*>::const_iterator it = force_field_->getAtoms().begin();
		Molecule* old_molecule = (*it)->getMolecule();
		Molecule* new_molecule;
		Size start = 0;
		Size end = 0;
		float mass = 0;

		molecules_.clear();

		for ( ; it != force_field_->getAtoms().end() ; ++it, ++end) 
		{
			new_molecule = (*it)->getMolecule();
			if (new_molecule != old_molecule) 
			{
				if (end > start && mass != 0)
				{
					 molecules_.push_back(pair<Size,Size>(start,end));
				}

				start = end;
				old_molecule = new_molecule;
				mass = 0;
			} 
			else 
			{
				mass += (*it)->getElement().getAtomicWeight();
			}
		}

		return(molecules_.size());
	}

	// Setup the periodic boundary	
	bool PeriodicBoundary::setup()
	{
		// we have no options from the force field: give up
		if (options == 0)
		{
			Log.level(LogStream::WARNING) << "PeriodicBoundary not bound to a force field!" << endl;
			return false;
		}

		// check whether the box is enabled 
		if (options->has(Option::PERIODIC_BOX_ENABLED))
		{
			enabled_ = options->getBool(Option::PERIODIC_BOX_ENABLED);
		}

		// Periodic boundary not enabled
		if (!enabled_)
		{
			return true;
		}

		// set default values if necessary
		options->setDefaultBool(Option::PERIODIC_BOX_ENABLED, true);
		options->setDefault(Option::PERIODIC_BOX_SOLVENT_FILE, Default::PERIODIC_BOX_SOLVENT_FILE);
		options->setDefaultBool(Option::PERIODIC_BOX_ADD_SOLVENT,Default::PERIODIC_BOX_ADD_SOLVENT);
		options->setDefaultReal(Option::PERIODIC_BOX_DISTANCE, Default::PERIODIC_BOX_DISTANCE);
		options->setDefaultReal(Option::PERIODIC_BOX_SOLVENT_SOLUTE_DISTANCE, Default::PERIODIC_BOX_SOLVENT_SOLUTE_DISTANCE);
		options->setDefault(Option::PERIODIC_WATER_FILE, Default::PERIODIC_WATER_FILE);

		// first, check whether the box dimensions are already set
		if (options->has(Option::PERIODIC_BOX_LOWER) && options->has(Option::PERIODIC_BOX_UPPER))
		{

			// now check whether the options contain valid coordinates
			Vector3 tmp_lower = options->getVector(Option::PERIODIC_BOX_LOWER);
			Vector3 tmp_upper = options->getVector(Option::PERIODIC_BOX_UPPER);
			
			// store the box
			box_.set(tmp_lower, tmp_upper);
		} 
		else 
		{
			// we didn`t find box dimensions - perhaps we got a distance?

			if (options->has(Option::PERIODIC_BOX_DISTANCE))
			{
				float dist = options->getReal(Option::PERIODIC_BOX_DISTANCE);
				
				// make sure we have a system
				if (force_field_->getSystem() == 0)
				{
					Log.level(LogStream::ERROR) << "Force field has no system!" << endl;
					return false;
				}

				// the minimum distance has to be non-negative
				if (dist < 0)
				{
					Log.level(LogStream::ERROR) << "Minimum distance for periodic boundary is negative: " << dist << endl;
					return false;
				}

				// calculate the system`s bounding box 
				BoundingBoxProcessor p;
				const_cast<System*>(force_field_->getSystem())->apply(p);
				box_.a = p.getLower() - Vector3(dist);
				box_.b = p.getUpper() + Vector3(dist);
			
				Log.level(LogStream::INFORMATION) << "Creating periodic boundary with a minimum distance of " 
					<< dist << " Angstrom" << endl;
			}
		}
		
		
		// ensure that the box is non-degenerate
		if ((box_.a.x >= box_.b.x) || (box_.a.y >= box_.b.y) || (box_.a.z >= box_.b.z))
		{
			Log.level(LogStream::ERROR) << "Illegal coordinates for periodic boundary: " << box_.a << "/" << box_.b << endl;
			return false;
		}

		// check whether we should add water
		if (options->has(Option::PERIODIC_BOX_ADD_SOLVENT) && options->isBool(Option::PERIODIC_BOX_ADD_SOLVENT))
		{
			String filename(Option::PERIODIC_WATER_FILE);
			
			if (options->has(Option::PERIODIC_BOX_SOLVENT_FILE))
			{
				filename = options->get(Option::PERIODIC_BOX_SOLVENT_FILE);
			}
			
			// avoid further addition of water
			options->setBool(Option::PERIODIC_BOX_ADD_SOLVENT, false);


			// add the solvent
			try 
			{
				addSolvent(filename);
			}
			catch (Exception::FileNotFound e)
			{
				Log.error() << "PeriodicBoundary::setup: Solvent file not found: " << e.getName() << endl;
				return false;
			}
		}


		// store the box dimensions in the options
		options->setVector(Option::PERIODIC_BOX_LOWER, box_.a);
		options->setVector(Option::PERIODIC_BOX_UPPER, box_.b);
	
		return true;
	}

	// Fill the part of the box that contains no system atoms with solvent
	// molecules
	Size PeriodicBoundary::addSolvent(const String& filename) const
	{
		// try to find and read the file
		Path p;
		HINFile hin(p.find(filename));
		if (!hin.isValid())
		{
			throw Exception::FileNotFound(__FILE__, __LINE__, filename);
		}

		// remove old solvent molecules
		removeSolvent();

		// read the system
		System solvent;
		hin >> solvent;

		
		if (!hin.hasPeriodicBoundary())
		{
			Log.error() << "Solvent file " << hin.getName() 
									<< " does not contain a periodic boundary!" << endl;
			return 0;
		}
		
		Box3 solvent_box = hin.getPeriodicBoundary();
		Log.info() << "Box from HINFile = " << solvent_box << endl;

		// adapt foreign water boxes to our definition
		MolmecSupport::adaptWaterBox(solvent, solvent_box);

		// copy solvent molecules from the solvent system into 
		// the simulation system
		System* system = force_field_->getSystem();
		if ((force_field_ == 0) || (system == 0))
		{
			Log.error() << "Force field does not contain  a system." << endl;
			return 0;
		}
		
		// get the minimum solvent-solute distance from the options
		float solvent_solute_distance = 0.0;
		if (options->has(Option::PERIODIC_BOX_SOLVENT_SOLUTE_DISTANCE))
		{
			solvent_solute_distance = options->getReal(Option::PERIODIC_BOX_SOLVENT_SOLUTE_DISTANCE);
		}

		// build a hash grid containing _only_ the solute molecules; this is
		// needed for addNonOverlappingMolecules()
		// (we add 102% of the distance to make sure we get no points
		// on the grid boundary)
		Vector3	additional_space(solvent_solute_distance * 1.02);
		HashGrid3<Atom*> solute_grid(box_.a - additional_space,
				box_.b - box_.a + additional_space + additional_space,
				solvent_solute_distance);

		// Insert the atoms of the solute into the hash grid
		AtomIterator atom_it = system->beginAtom();
		for ( ; +atom_it; ++atom_it) 
		{
			solute_grid.insert(atom_it->getPosition(), &*atom_it);
		}

		// set the property IS_SOLVENT for all molecules in the solvent box
		MoleculeIterator mol_it = solvent.beginMolecule();
		for (; +mol_it; ++mol_it)
		{
			mol_it->setProperty(Molecule::IS_SOLVENT);
		}

		// add the solvent molecules
		Size added_molecules = 0;
		TranslationProcessor translation;
		
		// This is the vector that translates the lower vector of the solvent
		// box to the lower vector of the periodic box
		Vector3 basis = box_.a - solvent_box.a;
		
		// calculate the number of solvent boxes needed for each dimension
		float width = solvent_box.getWidth();
		float height = solvent_box.getHeight();
		float depth = solvent_box.getDepth();
		Size N_x = (Size)(box_.getWidth() / width + 1.0);
		Size N_y = (Size)(box_.getHeight() / height + 1.0);
		Size N_z = (Size)(box_.getDepth() / depth + 1.0);
		
		for (Size i = 0; i <= N_x; ++i)
		{
			for (Size j = 0; j <= N_y; ++j)
			{
				for (Size k = 0; k <= N_z; ++k)
				{
					Vector3 tmp;
					tmp.x = basis.x + (float)i * width;
					tmp.y = basis.y + (float)j * height;
					tmp.z = basis.z + (float)k * depth;
					translation.setTranslation(tmp);
					solvent.apply(translation);
					
					// check for overlaps and insert solvent molecules
					added_molecules += MolmecSupport::addNonOverlappingMolecules
															(*system, solute_grid, solvent, box_, 
																solvent_solute_distance);

					tmp.x = -tmp.x;
					tmp.y = -tmp.y;
					tmp.z = -tmp.z;

					translation.setTranslation(tmp);
					solvent.apply(translation);
				}
			}
		}

		// DEBUG
		Log.info() << "added " << added_molecules << " molecules" << endl;
		return added_molecules;
	}


	// Remove the solvent molecules that have been added by periodic boundary
	Size PeriodicBoundary::removeSolvent() const
	{
		// check whether the force field and the system therein
		// are set
		if ((force_field_ == 0) || (force_field_->getSystem() == 0))
		{
			return 0;
		}

		Size counter = 0;
		MoleculeIterator mol_it = force_field_->getSystem()->beginMolecule();
		for (; +mol_it; ++mol_it) 
		{
			if (mol_it->hasProperty(Molecule::IS_SOLVENT)) 
			{
				counter++;
				Molecule* mol = &(*mol_it);
				force_field_->getSystem()->remove((*mol_it));

				// check whether the molecule is static or dynamic
				if (mol->isAutoDeletable()) 
				{
					// it was created dynamically - destruct it!
					delete mol;
				} 
				else 
				{
					// this is a static object - destroy it only!
					mol->destroy();
				}
			}
		}

		return counter;
	}

}
