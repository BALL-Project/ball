// $Id: snapShot.C,v 1.16 2000/12/19 14:02:39 anker Exp $

// BALL includes 
#include <BALL/MOLMEC/COMMON/snapShot.h>
#include <BALL/CONCEPT/XDRPersistenceManager.h>

// STL includes 
#include <algorithm>

using namespace std;

namespace BALL
{

	// SnapShot
	// ------------------------------------------------------------


	// The default constructor of the SnapShot class. 
  SnapShot::SnapShot()
		throw()
		:	index_(0),
			number_of_atoms_(0),
			potential_energy_(0.0),
			kinetic_energy_(0.0),
			data_length_(0),
			total_length_(0),
			atom_positions_(0),
			atom_velocities_(0),
			atom_forces_(0),
			valid_(false)
	{
	}


	// The copy constructor of the SnapShot class. 
	SnapShot::SnapShot (const SnapShot& snapshot)
		throw()
		:	index_(snapshot.index_),
			number_of_atoms_(snapshot.number_of_atoms_),
			potential_energy_(snapshot.potential_energy_),
			kinetic_energy_(snapshot.kinetic_energy_),
			data_length_(snapshot.data_length_),
			total_length_(snapshot.total_length_),
			atom_positions_(snapshot.atom_positions_),
			atom_velocities_(snapshot.atom_velocities_),
			atom_forces_(snapshot.atom_forces_),
			valid_(snapshot.valid_)
	{
	}


	// BAUSTELLE
	// The constructor of this class expects the number of atoms in the system
	// and reserves the necessary amount of memory for the data
	SnapShot::SnapShot(const Size number)
		throw()
	{
		Vector3 tmp;

		number_of_atoms_ = number;
		// BAUSTELLE: Wieso true? Wieso ist ein snapshot mit reserviertem
		// Speicher aber ohne Daten valid?
		valid_ = true;
		index_ = 0;

		data_length_ = 9 * number_of_atoms_ * sizeof (tmp.x) 
			+ 2 * sizeof (potential_energy_);
		total_length_ = data_length_ + sizeof (number_of_atoms_) + sizeof (valid_) 
			+ sizeof (index_) + sizeof (data_length_) + sizeof (total_length_);

		// BAUSTELLE: Warum nicht resize? Hier stand vorher reserve.
		atom_positions_.resize(number);
		atom_velocities_.resize(number);
		atom_forces_.resize(number);
		potential_energy_ = 0.0;
		kinetic_energy_ = 0.0;
	}


	// The destructor just deletes all STL components (superfluous and just
	// for clarity) 
	SnapShot::~SnapShot()
		throw()
	{
		clear();

		valid_ = false;
	}


	// The assignment operator 
	const SnapShot& SnapShot::operator = (const SnapShot& snapshot)
		throw()
	{
		valid_ = snapshot.valid_;
		index_ = snapshot.index_;
		number_of_atoms_ = snapshot.number_of_atoms_;
		data_length_ = snapshot.data_length_;
		total_length_ = snapshot.total_length_;
		potential_energy_ = snapshot.potential_energy_;
		kinetic_energy_ = snapshot.kinetic_energy_;
		atom_positions_ = snapshot.atom_positions_;
		atom_velocities_ = snapshot.atom_velocities_;
		atom_forces_ = snapshot.atom_forces_;

		return *this;
	}


	void SnapShot::clear()
		throw()
	{
		valid_ = false;
		index_ = 0;
		number_of_atoms_ = 0;
		data_length_ = 0;
		total_length_ = 0;
		potential_energy_ = 0.0;
		kinetic_energy_ = 0.0;

		atom_positions_.clear();
		atom_velocities_.clear();
		atom_forces_.clear();
	}


	bool SnapShot::operator == (const SnapShot& snapshot) const
		throw()
	{
		return ((valid_ == snapshot.valid_)
			&& (index_ == snapshot.index_)
			&& (number_of_atoms_ == snapshot.number_of_atoms_)
			&& (data_length_ == snapshot.data_length_)
			&& (total_length_ == snapshot.total_length_)
			&& (potential_energy_ == snapshot.potential_energy_)
			&& (kinetic_energy_ == snapshot.kinetic_energy_)
			&& (atom_positions_ == snapshot.atom_positions_)
			&& (atom_velocities_ == snapshot.atom_velocities_)
			&& (atom_forces_ == snapshot.atom_forces_));
	}


	bool SnapShot::isValid() const
		throw()
	{
		return valid_;
	}

	
	void SnapShot::setIndex(Size index)
		throw()
	{
		index_ = index;
	}


	Size SnapShot::getIndex() const
		throw()
	{
		return index_;
	}


	void SnapShot::setNumberOfAtoms(Size number_of_atoms)
		throw()
	{
		number_of_atoms_ = number_of_atoms;
	}


	Size SnapShot::getNumberOfAtoms() const
		throw()
	{
		return number_of_atoms_;
	}


	Size SnapShot::getDataLength() const
		throw()
	{
		return data_length_;
	}


	Size SnapShot::getTotalLength() const
		throw()
	{
		return total_length_;
	}


	double SnapShot::getPotentialEnergy() const
		throw()
	{
		return potential_energy_;
	}


	double SnapShot::getKineticEnergy() const
		throw()
	{
		return kinetic_energy_;
	}


	void SnapShot::setAtomPositions(const std::vector<Vector3>& atom_postions)
		throw()
	{
		atom_positions_ = atom_postions;
	}


	const std::vector<Vector3>& SnapShot::getAtomPositions() const
		throw()
	{
		return atom_positions_;
	}


	void SnapShot::setAtomVelocitites(const std::vector<Vector3>& atom_velocities)
		throw()
	{
		atom_velocities_ = atom_velocities;
	}


	const std::vector<Vector3>& SnapShot::getAtomVelocities() const
		throw()
	{
		return atom_velocities_;
	}


	void SnapShot::setAtomForces(const std::vector<Vector3>& atom_forces)
		throw()
	{
		atom_forces_ = atom_forces;
	}


	const std::vector<Vector3>& SnapShot::getAtomForces() const
		throw()
	{
		return atom_forces_;
	}


	// SnapShotManager
	// ------------------------------------------------------------

	// Definition of class-specific options and default values
	const char *SnapShotManager::Option::FLUSH_TO_DISK_FREQUENCY 
		= "flush_to_disk_frequency";

	const unsigned int SnapShotManager::Default::FLUSH_TO_DISK_FREQUENCY = 10;


	SnapShotManager::SnapShotManager()
		throw()
		: options(),
			system_ptr_(0),
			force_field_ptr_(0),
			snapshot_counter_(0),
			number_of_atoms_(0),
			snapshot_list_(0),
			trajectory_file_(0),
			flush_to_disk_frequency_(0),
			valid_(false)
	{
		options.setDefaultInteger(SnapShotManager::Option::FLUSH_TO_DISK_FREQUENCY,
				SnapShotManager::Default::FLUSH_TO_DISK_FREQUENCY);
	}


	// The constructor of the SnapshotManager.  
	// It expects a valid system, a force field, and a filename  of a
	// snapshot file along with a boolean that indicates if an existing file
	// shall be overwritten.  If the file does not exist yet, it will be
	// created and a header consisting of the system and some internal
	// information will be written to it.  It the file is already existent
	// and overwrite is set to false, the header will read in and compared
	// with the given system. They must match.  Subsequent snapshots will
	// then be appended to the file.
	SnapShotManager::SnapShotManager
		(const System* my_system, const ForceField* my_forcefield, 
		 TrajectoryFile* file, bool /* overwrite */)
		throw()
		: options(),
			system_ptr_(my_system),
			force_field_ptr_(my_forcefield),
			snapshot_counter_(0),
			number_of_atoms_(0),
			snapshot_list_(0),
			trajectory_file_(file),
			flush_to_disk_frequency_(0),
			buffer_counter_(0),
			valid_(false)
	{
		options.setDefaultInteger(SnapShotManager::Option::FLUSH_TO_DISK_FREQUENCY,
				SnapShotManager::Default::FLUSH_TO_DISK_FREQUENCY);

		// call the setup method
		valid_ = setup();
	}


	// The constructor of the SnapshotManager.
	// It expects a valid system and force field, a filename  of a snapshot
	// file along with a boolean that indicates if an existing file shall be
	// overwritten, and the options.  If the file does not exist yet, it will
	// be created and a header consisting of the system and some internal
	// information will be written to it.  It the file is already existent
	// and overwrite is set to false, the header will read in and compared
	// with the given system. They must match. Subsequent snapshots will be
	// appended to the file. 
	SnapShotManager::SnapShotManager 
		(const System* my_system, const ForceField* my_forcefield, 
		 const Options& my_options, TrajectoryFile* file,
		 bool /* overwrite */)
		throw()
		: options(my_options),
			system_ptr_(my_system),
			force_field_ptr_(my_forcefield),
			snapshot_counter_(0),
			number_of_atoms_(0),
			snapshot_list_(0),
			trajectory_file_(file),
			flush_to_disk_frequency_(0),
			buffer_counter_(0),
			valid_(false)
	{
		// simply call the setup method
		valid_ = setup();
	}


	// The copy constructor of the SnapShotManager
	// All data will be copied. But the copy will flush snapshots to the same
	// file as the original instance and it will use the same system and
	// force field!
	// NOTE, that this copy constructor DOES NOT copy the system and force
	// field but only the pointers!
	SnapShotManager::SnapShotManager (const SnapShotManager& manager)
		throw()
		: options(manager.options),
			system_ptr_(manager.system_ptr_),
			force_field_ptr_(manager.force_field_ptr_),
			snapshot_counter_(manager.snapshot_counter_),
			number_of_atoms_(manager.number_of_atoms_),
			snapshot_list_(manager.snapshot_list_),
			trajectory_file_(manager.trajectory_file_),
			flush_to_disk_frequency_(manager.flush_to_disk_frequency_),
			buffer_counter_(0),
			valid_(manager.valid_)
	{
	}


	// The destructor of SnapShotManager 
	SnapShotManager::~SnapShotManager()
		throw()
	{
		// clear() handles open files, so the destructor does not handle them
		clear();

		valid_ = false;
	}


	const SnapShotManager& SnapShotManager::operator = 
		(const SnapShotManager& manager)
		throw()
	{
		options = manager.options;
		system_ptr_ = manager.system_ptr_;
		force_field_ptr_ = manager.force_field_ptr_;
		snapshot_counter_ = manager.snapshot_counter_;
		number_of_atoms_ = manager.number_of_atoms_;
		snapshot_list_ = manager.snapshot_list_;
		trajectory_file_ = manager.trajectory_file_;
		flush_to_disk_frequency_ = manager.flush_to_disk_frequency_;
		buffer_counter_ = manager.buffer_counter_;
		valid_ = manager.valid_;

		return *this;
	}


	void SnapShotManager::clear()
		throw()
	{
		// BAUSTELLE
		flushToDisk();

		// now bring the instance to initial state
		options.clear();
		system_ptr_ = 0;
		force_field_ptr_ = 0;
		snapshot_counter_ = 0;
		number_of_atoms_ = 0;
		snapshot_list_.clear();
		trajectory_file_->clear();
		flush_to_disk_frequency_ = 
			options.getInteger(SnapShotManager::Option::FLUSH_TO_DISK_FREQUENCY);
		buffer_counter_ = 0;
		valid_ = false;
	}


	bool SnapShotManager::isValid() const
		throw()
	{
		return valid_;
	}


	// The setup method does the actual preparations
	bool SnapShotManager::setup()
		throw()
	{

		// first test for existence of system and force field
		if (system_ptr_ == 0 || !system_ptr_->isValid())
		{
			Log.error() << "SnapShotManager::setup(): No/invalid system." << endl;
			return false;
		}
		if (force_field_ptr_ == 0 || !force_field_ptr_->isValid())
		{
			Log.error() << "SnapShotManager::setup(): No/invald force field." << endl;
			return false;
		}

		// now check that the force field is assigned to the system
		if (force_field_ptr_->getSystem() != system_ptr_)
		{
			Log.error() << "SnapShotManager::setup(): Systems do not match." << endl;
			return false;
		}

		// first get the options
		flush_to_disk_frequency_ =
			options.getInteger(SnapShotManager::Option::FLUSH_TO_DISK_FREQUENCY);

		// if there was already snapshot data, clear it.
		// clear() does too much... Should I rewrite setup()? Or do I believe,
		// that setup is called _only_ in a constructor?
		snapshot_list_.clear();
		snapshot_list_.resize(flush_to_disk_frequency_);

		// the number of atoms in the system
		number_of_atoms_ = system_ptr_->countAtoms();

		// we start out with zero snapshots 
		snapshot_counter_ = 0;

		return true;

	}	// end of SnapShotManager::setup


	// This method calculates the current kinetic energy of the system
	// (only selected atoms are considered) 
	double SnapShotManager::calculateKineticEnergy_()
	{
		// First define some local variables 
		double sq_velocity;
		double sum, result;

		AtomVector::Iterator atom_it;
		AtomVector selected_atoms;

		sum = 0.0;
		selected_atoms = force_field_ptr_->getAtoms();

		// The current temperature (calculated as instantaneous kinetic energy)
		// If we use a periodic boundary box, then we use the molecules' centres of
		// gravity, otherwise we iterate directly over the individual atoms of the system 

		atom_it = selected_atoms.begin();

		if (force_field_ptr_->periodic_boundary.isEnabled() == true)
		{
			float molecule_mass = 0;
			float mass;

			int no_of_molecules = 0;

			Molecule *old, *current;
			Vector3 centre_velocity;


			centre_velocity.x = centre_velocity.y = centre_velocity.z = 0;

			// Get the molecule of the first atom in the system 
			old = (*atom_it)->getMolecule();

			// Iterate over all atoms in the system and determine for every
			// molecule the velocity of its centre of mass
			// This velocity contributes to the overall temperature 
			AtomVector::Iterator end_it = selected_atoms.end();

			while (atom_it != end_it)
			{
				// determine the molecule of the current atom
				current = (*atom_it)->getMolecule();

				if (current != old)
				{
					// This atom belongs to an other molecule than the 
					// previous atom -> all atoms of the previous 
					// molecule have been visited
					centre_velocity /= molecule_mass;
					sq_velocity = centre_velocity * centre_velocity;
					sum += molecule_mass * sq_velocity;

					no_of_molecules++;

					// reset some values for the new molecule
					molecule_mass = 0;
					centre_velocity.x = centre_velocity.y = centre_velocity.z = 0;

					// the new molecule is now the old one
					old = current;
				}

				// add the atom's velocity to the total velocity  
				// of the current molecule
				mass = (*atom_it)->getElement().getAtomicWeight();
				molecule_mass += mass;
				centre_velocity += mass * (*atom_it)->getVelocity();


				// go on to the next atom
				++atom_it;
			}

			// The last molecule must be added as this is not done in the while-loop
			if (molecule_mass == 0)
			{
				// A strange molecule. Let the centre_velocity as it is
			}
			else
			{
				centre_velocity /= molecule_mass;
			}

			sq_velocity = centre_velocity * centre_velocity;
			sum += molecule_mass * sq_velocity;
			no_of_molecules++;

			// 0.01 scales Da*A^2/ps^2 to kJ/mol 
			result = 0.5 * sum * 0.01;

		}
		else
		{
			// No periodic boundary conditions, i.e., just a few molecules 
			// are in the system
			int number_of_atoms_ = 0;

			// Iterate over all atoms and calculate \sum m_i \cdot v_i^2  
			for (atom_it = selected_atoms.begin(); atom_it != selected_atoms.end(); ++atom_it)
			{
				number_of_atoms_++;
				sq_velocity = (*atom_it)->getVelocity() * (*atom_it)->getVelocity();
				sum += (*atom_it)->getElement().getAtomicWeight() * sq_velocity;
			}

			// 0.01 scales Da*A^2/ps^2 to kJ/mol 
			result = 0.5 * sum * 0.01;
		}

		return result;

	}	// end of 'calculateKineticEnergy'            


	// This method takes a snapshot of the system's current state and stores it in
	// memory. If no memory is available or a maximum number of snapshots has been
	// taken, all snapshots so far are saved to disk 
	// The first snapshot taken (and with no other previous snapshots in a file)
	// has index 1. 
	void SnapShotManager::takeSnapShot()
		throw(Exception::OutOfMemory)
	{

		// create a new Snapshot-Object
		SnapShot snapshot(number_of_atoms_);
		/*
		SnapShot* snapshot_ptr = new SnapShot(number_of_atoms_);

		if (snapshot_ptr == 0)
		{
			// there was not enough memory available 
			// flush all snapshots taken so far to disk and give up
			flushToDisk();
			throw Exception::OutOfMemory(__FILE__, __LINE__);
			return;
		}
		*/

		// store all current positions, forces, velocities in the
		// snapshot object

		AtomIterator atom_it = system_ptr_->beginAtom();

		// This is the data section of the snapshot object 
		for (Size i = 0; +atom_it; ++atom_it, ++i)
		{
			// snapshot_ptr->atom_positions_[i] = atom_it->getPosition();
			// snapshot_ptr->atom_velocities_[i] = atom_it->getVelocity();
			// snapshot_ptr->atom_forces_[i] = atom_it->getForce();
			snapshot.atom_positions_[i] = atom_it->getPosition();
			snapshot.atom_velocities_[i] = atom_it->getVelocity();
			snapshot.atom_forces_[i] = atom_it->getForce();
		}

		// store the potential energies      
		// snapshot_ptr->potential_energy_ = force_field_ptr_->getEnergy();
		snapshot.potential_energy_ = force_field_ptr_->getEnergy();

		// store the kinetic energy of all selected atoms in the system 
		// the current value must be calculated as it is not provided by
		// the force field 
		// snapshot_ptr->kinetic_energy_ = calculateKineticEnergy_();
		snapshot.kinetic_energy_ = calculateKineticEnergy_();

		// These items are the admininistrative data 
		// First store the index of this snapshot. SnapShots are counted
		// starting with 1. 
		// snapshot_ptr->index_ = snapshot_counter_ + 1;
		snapshot.index_ = snapshot_counter_ + 1;

		// One more snapshot. Add it to the current list of snapshots
		// snapshot_list_.push_back(*snapshot_ptr);
		snapshot_list_[buffer_counter_] = snapshot;
		snapshot_counter_++;
		buffer_counter_++;

		// We _could_ use pushback() and size() to determine when toflush the
		// buffers, but this is faster. 
		if (buffer_counter_ >= flush_to_disk_frequency_)
		{
			// write all snapshots to disk in order to prevent memory overflow
			flushToDisk();
			snapshot_list_.clear();
			// BAUSTELLE is that REALLY necessary?
			snapshot_list_.resize(flush_to_disk_frequency_);
			buffer_counter_ = 0;
		}
	}	// end of SnapShotManager::takeSnapShot() 


	// This method writes all snapshots in memory to the snapshot file on disk
	void SnapShotManager::flushToDisk()
		throw()
	{
		// if no snapshots are in main memory, then there is nothing to do
		// if (snapshot_list_.size() == 0 || valid_ == false)
		if (snapshot_list_.size() == 0)
		{
			Log.info() << "SnapShotManager::flushToDisk(): "
				<< "nothing to do." << endl;
			return;
		}

		trajectory_file_->reopen(File::IN | File::BINARY);
		// BAUSTELLE
		// trajectory_file_->readHeader();

		trajectory_file_->updateHeader(*this);

		trajectory_file_->reopen(File::IN | File::OUT | File::BINARY);
		trajectory_file_->writeHeader();

		trajectory_file_->reopen(File::APP | File::BINARY);
		for (Size i = 0; i < buffer_counter_; ++i)
		{
			trajectory_file_->append(snapshot_list_[i]);
		}
		trajectory_file_->close();

	}	// end of SnapShotManager::flushToDisk()


	Size SnapShotManager::getNumberOfSnapShots() const 
		throw()
	{
		return snapshot_counter_;
	} 

}	// end of namespace BALL
