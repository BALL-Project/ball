// $Id: snapShotManager.C,v 1.6 2001/05/17 01:30:53 oliver Exp $

#include <BALL/KERNEL/PTE.h>
#include <BALL/MOLMEC/COMMON/snapShotManager.h>

#include <iostream>

using namespace std;

namespace BALL
{

	// Definition of class-specific options and default values
	const char *SnapShotManager::Option::FLUSH_TO_DISK_FREQUENCY 
		= "flush_to_disk_frequency";

	const unsigned int SnapShotManager::Default::FLUSH_TO_DISK_FREQUENCY = 10;

	SnapShotManager::SnapShotManager()
		throw()
		: options(),
			system_ptr_(0),
			force_field_ptr_(0),
			snapshot_buffer_(0),
			trajectory_file_ptr_(0),
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
		(System* my_system, const ForceField* my_forcefield, 
		 TrajectoryFile* file, bool /* overwrite */)
		throw()
		: options(),
			system_ptr_(my_system),
			force_field_ptr_(my_forcefield),
			snapshot_buffer_(0),
			trajectory_file_ptr_(file),
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
		(System* my_system, const ForceField* my_forcefield, 
		 const Options& my_options, TrajectoryFile* file,
		 bool /* overwrite */) 
		throw()
		: options(my_options),
			system_ptr_(my_system),
			force_field_ptr_(my_forcefield),
			snapshot_buffer_(0),
			trajectory_file_ptr_(file),
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
			snapshot_buffer_(manager.snapshot_buffer_),
			trajectory_file_ptr_(manager.trajectory_file_ptr_),
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
		snapshot_buffer_ = manager.snapshot_buffer_;
		trajectory_file_ptr_ = manager.trajectory_file_ptr_;
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
		snapshot_buffer_.clear();
		trajectory_file_ptr_ = 0;
		flush_to_disk_frequency_ = (Size)
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
	bool SnapShotManager::setup(System* my_system, 
			const ForceField* my_forcefield, TrajectoryFile* my_snapshot_file)
		throw()
	{
		setSystem(my_system);
		setForceField(my_forcefield);
		setTrajectoryFile(my_snapshot_file);
		return setup();
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
			(Size)options.getInteger(SnapShotManager::Option::FLUSH_TO_DISK_FREQUENCY);

		// if there was already snapshot data, clear it.
		// clear() does too much... Should I rewrite setup()? Or do I believe,
		// that setup is called only(!) in a constructor?
		snapshot_buffer_.clear();
		// snapshot_buffer_.resize(flush_to_disk_frequency_);

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
			// Iterate over all atoms and calculate \sum m_i \cdot v_i^2  
			for (atom_it = selected_atoms.begin(); atom_it != selected_atoms.end(); ++atom_it)
			{
				sq_velocity = (*atom_it)->getVelocity() * (*atom_it)->getVelocity();
				sum += (*atom_it)->getElement().getAtomicWeight() * sq_velocity;
			}

			// 0.01 scales Da*A^2/ps^2 to kJ/mol 
			result = 0.5 * sum * 0.01;
		}

		return result;

	}	// end of 'calculateKineticEnergy'            


	void SnapShotManager::setSystem(System* my_system)
		throw()
	{
		system_ptr_ = my_system;
	}


	System* SnapShotManager::getSystem() const
		throw()
	{
		return system_ptr_;
	}


	void SnapShotManager::setForceField(const ForceField* my_ff)
		throw()
	{
		force_field_ptr_ = my_ff;
	}


	const ForceField* SnapShotManager::getForceField() const
		throw()
	{
		return force_field_ptr_;
	}


	void SnapShotManager::setTrajectoryFile(TrajectoryFile* my_file)
		throw()
	{
		trajectory_file_ptr_ = my_file;
	}


	TrajectoryFile* SnapShotManager::getTrajectoryFile() const
		throw()
	{
		return trajectory_file_ptr_;
	}


	void SnapShotManager::setFlushToDiskFrequency(Size number)
		throw()
	{
		flush_to_disk_frequency_ = number;
	}


	Size SnapShotManager::getFlushToDiskFrequency() const
		throw()
	{
		return flush_to_disk_frequency_;
	}


	// The following is not true at the moment...
	// This method takes a snapshot of the system's current state and stores
	// it in memory. If no memory is available or a maximum number of
	// snapshots has been taken, all snapshots so far are saved to disk The
	// first snapshot taken (and with no other previous snapshots in a file)
	// has index 1. 
	void SnapShotManager::takeSnapShot()
		throw()
	{
		// create a new Snapshot-Object
		SnapShot snapshot;

		// store all current positions, forces, velocities in the
		// snapshot object
		snapshot.takeSnapShot(*system_ptr_);

		// store the potential energies      
		// snapshot_ptr->potential_energy_ = force_field_ptr_->getEnergy();
		snapshot.setPotentialEnergy(force_field_ptr_->getEnergy());

		// store the kinetic energy of all selected atoms in the system 
		// the current value must be calculated as it is not provided by
		// the force field 
		snapshot.setKineticEnergy(calculateKineticEnergy_());

		// One more snapshot. Add it to the current list of snapshots
		snapshot_buffer_.push_back(snapshot);
		// snapshot_buffer_[buffer_counter_] = snapshot;
		buffer_counter_++;

		// We could! use pushback() and size() to determine when to flush the
		// buffers, but this is faster. 
		if (buffer_counter_ >= flush_to_disk_frequency_)
		{
			// write all snapshots to disk in order to prevent memory overflow
			flushToDisk();
			snapshot_buffer_.clear();
			// BAUSTELLE is that REALLY necessary?
			// snapshot_buffer_.resize(flush_to_disk_frequency_);
			buffer_counter_ = 0;
		}
	}	// end of SnapShotManager::takeSnapShot() 


	bool SnapShotManager::applySnapShot(Size number)
		throw()
	{
		SnapShot buffer;

		if (number > trajectory_file_ptr_->getNumberOfSnapShots())
		{
			Log.error() << "SnapShotManager::applySnapShot(): "
				<< "requested SnapShot number " << number 
				<< " while the file only contains " 
				<< trajectory_file_ptr_->getNumberOfSnapShots() 
				<< " SnapShots" << endl;
			return false;
		}

		// We have to avoid seek() so we need to reopen and read from the
		// beginning
		trajectory_file_ptr_->reopen();
		trajectory_file_ptr_->readHeader();
		for (Size count = 0; count < number; ++count)
		{
			if (!trajectory_file_ptr_->read(buffer))
			{
				Log.error() << "SnapShotManager::applySnapShot(): "
					<< "error reading from the TrajectoryFile" << endl;
				return false;
			}
			// DEBUG 
			Log.info() << "read SnapShot number " << count << endl;
		}
		// now apply the last snapshot we read
		buffer.applySnapShot(*system_ptr_);
		return true;
	}


	bool SnapShotManager::applyFirstSnapShot()
		throw()
	{
		SnapShot buffer;

		// BAUSTELLE:
		// We should check for file validity here

		trajectory_file_ptr_->reopen();
		trajectory_file_ptr_->readHeader();
		return applyNextSnapShot();
	}


	bool SnapShotManager::applyNextSnapShot()
		throw()
	{
		SnapShot buffer;

		if (trajectory_file_ptr_->read(buffer))
		{
			buffer.applySnapShot(*system_ptr_);
			return true;
		}
		else
		{
			Log.error() << "SnapShotManager::applyNextSnapShot() "
				<< "error reading from the TrajectoryFile" << endl;
			return false;
		}
	}


	bool SnapShotManager::applyLastSnapShot()
		throw()
	{
		Size count = 0;
		SnapShot buffer;

		while (trajectory_file_ptr_->read(buffer))
		{
			count++;
		}

		if (count == trajectory_file_ptr_->getNumberOfSnapShots())
		{
			buffer.applySnapShot(*system_ptr_);
		}
		else
		{
			Log.error() << "SnapShotManager::applyLastSnapShot(): "
				<< "read " << count 
				<< " SnapShots while TrajectoryFile claims to have "
				<< trajectory_file_ptr_->getNumberOfSnapShots()
				<< " SnapShots" << endl;
			return false;
		}
		return true;
	}


	// This method writes all snapshots in memory to the snapshot file on disk
	void SnapShotManager::flushToDisk()
		throw()
	{
		// if no snapshots are in main memory, then there is nothing to do
		if (snapshot_buffer_.size() == 0)
		{
			Log.info() << "SnapShotManager::flushToDisk(): "
				<< "nothing to do." << endl;
			return;
		}

		trajectory_file_ptr_->flushToDisk(snapshot_buffer_);

	}	// end of SnapShotManager::flushToDisk()

} 
