// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: snapShotManager.C,v 1.15.20.2 2007/06/09 16:08:08 anhi Exp $
//

#include <BALL/KERNEL/PTE.h>
#include <BALL/MOLMEC/COMMON/snapShotManager.h>
#include <BALL/MOLMEC/COMMON/forceField.h>
#include <BALL/FORMAT/trajectoryFile.h>

#include <iostream>

using namespace std;

namespace BALL
{

	// Definition of class-specific options and default values
	const char *SnapShotManager::Option::FLUSH_TO_DISK_FREQUENCY = "flush_to_disk_frequency";
	const Size SnapShotManager::Default::FLUSH_TO_DISK_FREQUENCY = 10;

	SnapShotManager::SnapShotManager()
		: options(),
		  system_ptr_(0),
		  force_field_ptr_(0),
		  snapshot_buffer_(0),
		  trajectory_file_ptr_(0),
		  flush_to_disk_frequency_(0),
		  buffer_counter_(0),
		  current_snapshot_(0)
	{
		options.setDefaultInteger(SnapShotManager::Option::FLUSH_TO_DISK_FREQUENCY,
		                          SnapShotManager::Default::FLUSH_TO_DISK_FREQUENCY);
	}

	// The constructor of the SnapshotManager.  
	// It expects a valid system, a force field, and a filename  of a
	// snapshot file along with a boolean that indicates if an existing file
	// shall be overwritten.  If the file does not exist yet, it will be
	// created and a header consisting of the system and some internal
	// information will be written to it.  If the file is already existent,
	// the header will read in and compared
	// with the given system. They must match.  Subsequent snapshots will
	// then be appended to the file.
	SnapShotManager::SnapShotManager(System* my_system, const ForceField* my_forcefield,
	                                 TrajectoryFile* file)
		: options(),
		  system_ptr_(my_system),
		  force_field_ptr_(my_forcefield),
		  snapshot_buffer_(0),
		  trajectory_file_ptr_(file),
		  flush_to_disk_frequency_(0),
		  buffer_counter_(0),
		  current_snapshot_(0)
	{
		options.setDefaultInteger(SnapShotManager::Option::FLUSH_TO_DISK_FREQUENCY,
		                          SnapShotManager::Default::FLUSH_TO_DISK_FREQUENCY);

		// call the setup method
		setup();
	}

	SnapShotManager::SnapShotManager(System* my_system, TrajectoryFile* file)
		: options(),
		  system_ptr_(my_system),
		  force_field_ptr_(0),
		  snapshot_buffer_(0),
		  trajectory_file_ptr_(file),
		  flush_to_disk_frequency_(0),
		  buffer_counter_(0),
		  current_snapshot_(0)
	{
		options.setDefaultInteger(SnapShotManager::Option::FLUSH_TO_DISK_FREQUENCY,
		                          SnapShotManager::Default::FLUSH_TO_DISK_FREQUENCY);

		// call the setup method
		setup();
	}

	// The constructor of the SnapshotManager.
	// It expects a valid system and force field, a filename  of a snapshot
	// file along with a boolean that indicates if an existing file shall be
	// overwritten, and the options.  If the file does not exist yet, it will
	// be created and a header consisting of the system and some internal
	// information will be written to it.  If the file is already existent
	// the header will read in and compared
	// with the given system. They must match. Subsequent snapshots will be
	// appended to the file. 
	SnapShotManager::SnapShotManager(System* my_system, const ForceField* my_forcefield,
	                                 const Options& my_options, TrajectoryFile* file)
		: options(my_options),
		  system_ptr_(my_system),
		  force_field_ptr_(my_forcefield),
		  snapshot_buffer_(0),
		  trajectory_file_ptr_(file),
		  flush_to_disk_frequency_(0),
		  buffer_counter_(0),
		  current_snapshot_(0)
	{
		// simply call the setup method
		setup();
	}


	// The copy constructor of the SnapShotManager
	// All data will be copied. But the copy will flush snapshots to the same
	// file as the original instance and it will use the same system and
	// force field!
	// NOTE, that this copy constructor DOES NOT copy the system and force
	// field but only the pointers!
	SnapShotManager::SnapShotManager (const SnapShotManager& manager)
		: options(manager.options),
		  system_ptr_(manager.system_ptr_),
		  force_field_ptr_(manager.force_field_ptr_),
		  snapshot_buffer_(manager.snapshot_buffer_),
		  trajectory_file_ptr_(manager.trajectory_file_ptr_),
		  flush_to_disk_frequency_(manager.flush_to_disk_frequency_),
		  buffer_counter_(0),
		  current_snapshot_(0)
	{
	}

	// The destructor of SnapShotManager 
	SnapShotManager::~SnapShotManager()
	{
		// clear() handles open files, so the destructor does not handle them
		clear();
	}

	const SnapShotManager& SnapShotManager::operator = (const SnapShotManager& manager)
	{
		options = manager.options;
		system_ptr_ = manager.system_ptr_;
		force_field_ptr_ = manager.force_field_ptr_;
		snapshot_buffer_ = manager.snapshot_buffer_;
		trajectory_file_ptr_ = manager.trajectory_file_ptr_;
		flush_to_disk_frequency_ = manager.flush_to_disk_frequency_;
		buffer_counter_ = 0;
		current_snapshot_ = 0;

		return *this;
	}

	void SnapShotManager::clear()
	{
		// bring the instance to initial state
		options.clear();
		system_ptr_ = 0;
		force_field_ptr_ = 0;
		snapshot_buffer_.clear();
		// this destructor does not destruct or clear the associated
		// trajectory file
		trajectory_file_ptr_ = 0;
		flush_to_disk_frequency_ = (Size)options.getInteger(SnapShotManager::Option::FLUSH_TO_DISK_FREQUENCY);
		buffer_counter_ = 0;
		current_snapshot_ = 0;
	}

	bool SnapShotManager::isValid() const
	{
		// first test for existence of system and force field
		if (system_ptr_ == 0 || !system_ptr_->isValid())
		{
			Log.error() << "SnapShotManager::setup(): No/invalid system." << endl;
			return false;
		}

		/// we dont really always need a force field, do we ?????
		if (force_field_ptr_ == 0) return true;

		if (!force_field_ptr_->isValid())
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

		return true;
	}


	// The setup method does the actual preparations
	bool SnapShotManager::setup(System* my_system, const ForceField* my_forcefield,
	                            TrajectoryFile* my_snapshot_file)
	{
		setSystem(my_system);
		setForceField(my_forcefield);
		setTrajectoryFile(my_snapshot_file);
		return setup();
	}

	// The setup method does the actual preparations
	bool SnapShotManager::setup(System* my_system, TrajectoryFile* my_snapshot_file)
	{
		setSystem(my_system);
		setForceField(0);
		setTrajectoryFile(my_snapshot_file);
		return setup();
	}

	// The setup method does the actual preparations
	bool SnapShotManager::setup()
	{
		if (!isValid()) return false;

		// first get the options
		flush_to_disk_frequency_ = (Size)options.getInteger(SnapShotManager::Option::FLUSH_TO_DISK_FREQUENCY);

		// if there was already snapshot data, clear it.
		// clear() does too much... Should I rewrite setup()? Or do I believe,
		// that setup is called only(!) in a constructor?
		snapshot_buffer_.clear();
		current_snapshot_ = 0;
		buffer_counter_ = 0;
		return true;
	}

	// This method calculates the current kinetic energy of the system
	// (only selected atoms are considered) 
	double SnapShotManager::calculateKineticEnergy_()
	{
		if (force_field_ptr_ == 0) return 0;

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

	} // end of 'calculateKineticEnergy'


	void SnapShotManager::setSystem(System* my_system)
	{
		system_ptr_ = my_system;
	}


	System* SnapShotManager::getSystem() const
	{
		return system_ptr_;
	}


	void SnapShotManager::setForceField(const ForceField* my_ff)
	{
		force_field_ptr_ = my_ff;
	}


	const ForceField* SnapShotManager::getForceField() const
	{
		return force_field_ptr_;
	}


	void SnapShotManager::setTrajectoryFile(TrajectoryFile* my_file)
	{
		trajectory_file_ptr_ = my_file;
	}


	TrajectoryFile* SnapShotManager::getTrajectoryFile() const
	{
		return trajectory_file_ptr_;
	}


	void SnapShotManager::setFlushToDiskFrequency(Size number)
	{
		flush_to_disk_frequency_ = number;
	}


	Size SnapShotManager::getFlushToDiskFrequency() const
	{
		return flush_to_disk_frequency_;
	}


	// The following is not true at the moment...
	// This method takes a snapshot of the system's current state and stores
	// it in memory. If no memory is available or a maximum number of
	// snapshots has been taken, all snapshots so far are saved to disk The
	// first snapshot taken (and with no other previous snapshots in a file)
	// has index 1. 
	void SnapShotManager::takeSnapShot() throw(File::CannotWrite)
	{
		if (system_ptr_ == 0) return;

		// One more snapshot. Add it to the current list of snapshots
		// The way we do it here should be faster, than filling the 
		// SnapShot with data first and pushing it into the vector afterwards.
		snapshot_buffer_.push_back(SnapShot());
		SnapShot& snapshot = snapshot_buffer_[snapshot_buffer_.size() - 1];

		// store all current positions, forces, velocities in the
		// snapshot object
		snapshot.takeSnapShot(*system_ptr_);

		// store the potential energies      
		if (force_field_ptr_ != 0)
		{
			snapshot.setPotentialEnergy(force_field_ptr_->getEnergy());
		}

		// store the kinetic energy of all selected atoms in the system 
		// the current value must be calculated as it is not provided by
		// the force field 
		snapshot.setKineticEnergy(calculateKineticEnergy_());

		buffer_counter_++;

		// We could! use pushback() and size() to determine when to flush the
		// buffers, but this is faster. 
		if (buffer_counter_ >= flush_to_disk_frequency_)
		{
			// write all snapshots to disk in order to prevent memory overflow
			flushToDisk();
		}
	} // end of SnapShotManager::takeSnapShot()


	bool SnapShotManager::applySnapShot(Size number)
	{
		// we start at SnapShot number 1!
		if (number > 0) number --;

		if (system_ptr_ == 0) return false;

		// if we have read the Snapshots from the trajectory file
		// use these, otherwise "seek" to the correct file position
		if (!snapshot_buffer_.empty())
		{
			if (snapshot_buffer_.size() <= number) return false;

			snapshot_buffer_[number].applySnapShot(*system_ptr_);
			current_snapshot_ = number;
			return true;
		}

		// ok, we are gone read from the file
		if (trajectory_file_ptr_ == 0) return false;

		SnapShot buffer;

		if (number >= trajectory_file_ptr_->getNumberOfSnapShots())
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
		for (Size count = 0; count <= number; ++count)
		{
			if (!trajectory_file_ptr_->read(buffer))
			{
				Log.error() << "SnapShotManager::applySnapShot(): "
				            << "error reading from the TrajectoryFile" << endl;
				return false;
			}
		}
		// now apply the last snapshot we read
		buffer.applySnapShot(*system_ptr_);
		current_snapshot_ = number;
		return true;
	}


	bool SnapShotManager::applyFirstSnapShot()
	{
		if (system_ptr_ == 0) return false;

		// if we have read the Snapshots from the trajectory file
		// use these, otherwise read one Snapshot from the file
		if (!snapshot_buffer_.empty())
		{
			snapshot_buffer_[0].applySnapShot(*system_ptr_);
			current_snapshot_ = 0;
			return true;
		}

		// ok, we are gone read from the file
		if (trajectory_file_ptr_ == 0) return false;

		trajectory_file_ptr_->reopen();
		trajectory_file_ptr_->readHeader();
		current_snapshot_ = 0;

		if (trajectory_file_ptr_->getNumberOfSnapShots() == 0) return false;

		return applyNextSnapShot();
	}


	bool SnapShotManager::applyNextSnapShot()
	{
		if (system_ptr_ == 0) return false;

		// if we have read the Snapshots from the trajectory file
		// use these, otherwise read one Snapshot from the file
		if (!snapshot_buffer_.empty())
		{
			current_snapshot_++;
			if (current_snapshot_ >= snapshot_buffer_.size()) return false;

			snapshot_buffer_[current_snapshot_].applySnapShot(*system_ptr_);
			return true;
		}

		// ok, we are gone read from the file
		if (trajectory_file_ptr_ == 0) return false;

		SnapShot buffer;

		if (trajectory_file_ptr_->read(buffer))
		{
			buffer.applySnapShot(*system_ptr_);
			current_snapshot_++;
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
	{
		if (system_ptr_ == 0) return false;

		// if we have read the Snapshots from the trajectory file
		// use these, otherwise read one Snapshot from the file
		if (!snapshot_buffer_.empty())
		{
			snapshot_buffer_[snapshot_buffer_.size() - 1].applySnapShot(*system_ptr_);
			current_snapshot_ = snapshot_buffer_.size() - 1;
			return true;
		}

		// ok, we are gone read from the file
		if (trajectory_file_ptr_ == 0) return false;

		Size count = 0;
		SnapShot buffer;

		while (trajectory_file_ptr_->read(buffer))
		{
			count++;
		}

		if (count == trajectory_file_ptr_->getNumberOfSnapShots())
		{
			buffer.applySnapShot(*system_ptr_);
			current_snapshot_ = count - 1;
			return true;
		}

		Log.error() << "SnapShotManager::applyLastSnapShot(): "
		            << "read " << count
		            << " SnapShots while TrajectoryFile claims to have "
		            << trajectory_file_ptr_->getNumberOfSnapShots()
		            << " SnapShots" << endl;

		return false;
	}


	// This method writes all snapshots in memory to the snapshot file on disk
	void SnapShotManager::flushToDisk()
		throw(File::CannotWrite)
	{
		// if no snapshots are in main memory, then there is nothing to do
		// also abort, if no trajectory file was set
		if (snapshot_buffer_.empty() || !trajectory_file_ptr_)
		{
			return;
		}

		trajectory_file_ptr_->flushToDisk(snapshot_buffer_);
		snapshot_buffer_.clear();
		buffer_counter_ = 0;
	}

	bool SnapShotManager::readFromFile()
	{
		if (trajectory_file_ptr_ == 0) return false;
		snapshot_buffer_.clear();

		trajectory_file_ptr_->reopen();
		trajectory_file_ptr_->readHeader();
		Size number = trajectory_file_ptr_->getNumberOfSnapShots();
		for (Size count = 0; count < number; ++count)
		{
			SnapShot buffer;
			if (!trajectory_file_ptr_->read(buffer))
			{
				Log.error() << "SnapShotManager::Could not read "
				            << "from the TrajectoryFile (Out of memory?)" << endl;
				return false;
			}
			snapshot_buffer_.push_back(buffer);
		}

		return true;
	}

	void SnapShotManager::clearBuffer()
	{
		snapshot_buffer_.clear();
		current_snapshot_ = 0;
		buffer_counter_ = 0;
	}

}
