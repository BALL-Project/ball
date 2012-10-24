// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//

#include <BALL/MOLMEC/COMMON/snapShot.h>
#include <BALL/KERNEL/system.h>

using namespace std;

namespace BALL
{

	// The default constructor of the SnapShot class. 
  SnapShot::SnapShot()
		:	index_(0),
			number_of_atoms_(0),
			potential_energy_(0.0),
			kinetic_energy_(0.0),
			atom_positions_(0),
			atom_velocities_(0),
			atom_forces_(0)
	{
	}


	// The copy constructor of the SnapShot class. 
	SnapShot::SnapShot (const SnapShot& snapshot)
		:	index_(snapshot.index_),
			number_of_atoms_(snapshot.number_of_atoms_),
			potential_energy_(snapshot.potential_energy_),
			kinetic_energy_(snapshot.kinetic_energy_),
			atom_positions_(snapshot.atom_positions_),
			atom_velocities_(snapshot.atom_velocities_),
			atom_forces_(snapshot.atom_forces_)
	{
	}


	// The destructor just deletes all STL components (superfluous and just
	// for clarity) 
	SnapShot::~SnapShot()
	{
		clear();
	}


	// The assignment operator 
	const SnapShot& SnapShot::operator = (const SnapShot& snapshot)
	{
		index_ = snapshot.index_;
		number_of_atoms_ = snapshot.number_of_atoms_;
		potential_energy_ = snapshot.potential_energy_;
		kinetic_energy_ = snapshot.kinetic_energy_;
		atom_positions_ = snapshot.atom_positions_;
		atom_velocities_ = snapshot.atom_velocities_;
		atom_forces_ = snapshot.atom_forces_;

		return *this;
	}


	void SnapShot::clear()
	{
		index_ = 0;
		number_of_atoms_ = 0;
		potential_energy_ = 0.0;
		kinetic_energy_ = 0.0;

		atom_positions_.clear();
		atom_velocities_.clear();
		atom_forces_.clear();
	}


	bool SnapShot::operator == (const SnapShot& snapshot) const
	{
		return
			   (index_ == snapshot.index_)
			&& (number_of_atoms_ == snapshot.number_of_atoms_)
			&& (potential_energy_ == snapshot.potential_energy_)
			&& (kinetic_energy_ == snapshot.kinetic_energy_)
			&& (atom_positions_ == snapshot.atom_positions_)
			&& (atom_velocities_ == snapshot.atom_velocities_)
			&& (atom_forces_ == snapshot.atom_forces_);
	}


	bool SnapShot::isValid() const
	{
		return 	number_of_atoms_ > 0 &&
					 	number_of_atoms_ == atom_forces_.size() &&
						number_of_atoms_ == atom_velocities_.size() &&
						number_of_atoms_ == atom_positions_.size();
	}


	void SnapShot::setIndex(Size index)
	{
		index_ = index;
	}


	Size SnapShot::getIndex() const
	{
		return index_;
	}


	void SnapShot::setNumberOfAtoms(Size number_of_atoms)
	{
		number_of_atoms_ = number_of_atoms;
	}


	Size SnapShot::getNumberOfAtoms() const
	{
		return number_of_atoms_;
	}


	DoubleReal SnapShot::getPotentialEnergy() const
	{
		return potential_energy_;
	}


	void SnapShot::setPotentialEnergy(DoubleReal potential_energy)
	{
		potential_energy_ = potential_energy;
	}


	DoubleReal SnapShot::getKineticEnergy() const
	{
		return kinetic_energy_;
	}


	void SnapShot::setKineticEnergy(DoubleReal kinetic_energy)
	{
		kinetic_energy_ = kinetic_energy;
	}


	void SnapShot::setAtomPositions(const ::std::vector<Vector3>& atom_postions)
	{
		atom_positions_ = atom_postions;
	}


	const ::std::vector<Vector3>& SnapShot::getAtomPositions() const
	{
		return atom_positions_;
	}


	void SnapShot::setAtomVelocities(const ::std::vector<Vector3>& atom_velocities)
	{
		atom_velocities_ = atom_velocities;
	}


	const ::std::vector<Vector3>& SnapShot::getAtomVelocities() const
	{
		return atom_velocities_;
	}


	void SnapShot::setAtomForces(const ::std::vector<Vector3>& atom_forces)
	{
		atom_forces_ = atom_forces;
	}


	const ::std::vector<Vector3>& SnapShot::getAtomForces() const
	{
		return atom_forces_;
	}


	void SnapShot::takeSnapShot(const System& system)
		throw(Exception::OutOfMemory)
	{
		number_of_atoms_ = system.countAtoms();

		AtomConstIterator atom_it = system.beginAtom();

		// reserve memory
		atom_positions_.resize(number_of_atoms_);
		atom_velocities_.resize(number_of_atoms_);
		atom_forces_.resize(number_of_atoms_);

		// This is the data section of the snapshot object 
		for (Size i = 0; +atom_it; ++atom_it, ++i)
		{
			atom_positions_[i] = atom_it->getPosition();
			atom_velocities_[i] = atom_it->getVelocity();
			atom_forces_[i] = atom_it->getForce();
		}
	}


	void SnapShot::applySnapShot(System& system) const
	{
		if (system.countAtoms() != number_of_atoms_)
		{
			Log.error () << "SnapShot::applySnapShot(): "
				<< "Atom counts do not match: System: " << system.countAtoms()
				<< " SnapShot: " << number_of_atoms_ << endl;
			return;
		}

		AtomIterator atom_it = system.beginAtom();

		for (Size i = 0; +atom_it; ++atom_it, ++i)
		{
			if (!atom_positions_.empty())
			{
				atom_it->setPosition(atom_positions_[i]);
			}
			if (!atom_velocities_.empty())
			{
				atom_it->setVelocity(atom_velocities_[i]);
			}
			if (!atom_forces_.empty())
			{
				atom_it->setForce(atom_forces_[i]);
			}
		}
	}


	void SnapShot::getAtomPositions(const System& system)
		throw(Exception::OutOfMemory)
	{
		// obtain the number of atoms
		number_of_atoms_ = system.countAtoms();

		// reserve memory 
		atom_positions_.resize(number_of_atoms_);

		// copy data
		AtomConstIterator atom_it = system.beginAtom();
		for (Size i = 0; +atom_it; ++atom_it, ++i)
		{
			atom_positions_[i] = atom_it->getPosition();
		}
	}


	void SnapShot::setAtomPositions(System& system) const
	{
		if (system.countAtoms() != number_of_atoms_)
		{
			Log.error () << "SnapShot::setAtomPositions(): "
				<< "Atom counts do not match: System: " << system.countAtoms()
				<< " SnapShot: " << number_of_atoms_ << endl;
			return;
		}

		AtomIterator atom_it = system.beginAtom();

		for (Size i = 0; +atom_it; ++atom_it, ++i)
		{
			atom_it->setPosition(atom_positions_[i]);
		}
	}


	void SnapShot::getAtomVelocities(const System& system)
		throw(Exception::OutOfMemory)
	{
		number_of_atoms_ = system.countAtoms();
		atom_velocities_.resize(number_of_atoms_);
		AtomConstIterator atom_it = system.beginAtom();
		for (Size i = 0; +atom_it; ++atom_it, ++i)
		{
			atom_velocities_[i] = atom_it->getVelocity();
		}
	}


	void SnapShot::setAtomVelocitites(System& system) const
	{
		if (system.countAtoms() != number_of_atoms_)
		{
			Log.error () << "SnapShot::setAtomVelocitites(): "
				<< "Atom counts do not match: System: " << system.countAtoms()
				<< " SnapShot: " << number_of_atoms_ << endl;
			return;
		}

		AtomIterator atom_it = system.beginAtom();

		// This is the data section of the snapshot object 
		for (Size i = 0; +atom_it; ++atom_it, ++i)
		{
			atom_it->setVelocity(atom_velocities_[i]);
		}
	}


	void SnapShot::getAtomForces(const System& system)
		throw(Exception::OutOfMemory)
	{
		number_of_atoms_ = system.countAtoms();
		atom_forces_.resize(number_of_atoms_);
		AtomConstIterator atom_it = system.beginAtom();
		for (Size i = 0; +atom_it; ++atom_it, ++i)
		{
			atom_forces_[i] = atom_it->getForce();
		}
	}


	void SnapShot::setAtomForces(System& system) const
	{
		if (system.countAtoms() != number_of_atoms_)
		{
			Log.error () << "SnapShot::setAtomForces(): "
				<< "Atom counts do not match: System: " << system.countAtoms()
				<< " SnapShot: " << number_of_atoms_ << endl;
			return;
		}

		AtomIterator atom_it = system.beginAtom();

		// This is the data section of the snapshot object 
		for (Size i = 0; +atom_it; ++atom_it, ++i)
		{
			atom_it->setForce(atom_forces_[i]);
		}
	}


	::std::ostream& operator << (::std::ostream& os, const SnapShot& ss)
	{
		os << ss.getNumberOfAtoms() << endl << endl;

		vector<Vector3> data = ss.getAtomPositions();

		os << "Atom positions:" << endl;
		vector<Vector3>::const_iterator it = data.begin();
		for (; it != data.end(); ++it)
		{
			os << *it << endl;
		}

		os << endl << "Atom velocities:" << endl;

		data = ss.getAtomVelocities();
		for (it = data.begin(); it != data.end(); ++it)
		{
			os << *it << endl;
		}

		os << endl << "Atom forces:" << endl;

		data = ss.getAtomForces();
		for (it = data.begin(); it != data.end(); ++it)
		{
			os << *it << endl;
		}
		return os;
	}


	::std::istream& operator >> (::std::istream& is, SnapShot& ss)
	{
		// ?????:
		// this istream does not check anything!

		Size number;
		is >> number;

		vector<Vector3> data;
		data.resize(number);

		for (Size i = 0; i < number; ++i)
		{
			is >> data[i];
		}
		ss.setAtomPositions(data);

		for (Size i = 0; i < number; ++i)
		{
			is >> data[i];
		}
		ss.setAtomVelocities(data);

		for (Size i = 0; i < number; ++i)
		{
			is >> data[i];
		}
		ss.setAtomForces(data);
		return is;
	}

}	// end of namespace BALL
