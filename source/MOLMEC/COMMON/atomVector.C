// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: atomVector.C,v 1.13.30.1 2007/05/18 12:05:59 oliver Exp $
//
// Author:
//   Oliver Kohlbacher

#include <BALL/MOLMEC/COMMON/atomVector.h>
#include <BALL/MOLMEC/COMMON/gradient.h>
#include <BALL/MATHS/vector3.h>
#include <BALL/CONCEPT/composite.h>
#include <BALL/KERNEL/atom.h>

#undef BALL_DEBUG

using namespace std;

namespace BALL
{
	AtomVector::AtomVector()
	{
	}

	AtomVector::AtomVector(const AtomVector& atoms, bool /* deep */)
		: vector<Atom*>()
	{
		set(atoms);
	}

	AtomVector::AtomVector(const Composite& composite, bool selected_only)
	{
		set(composite, selected_only);
	}

	AtomVector::~AtomVector()
	{
	}

	const AtomVector& AtomVector::operator = (const AtomVector& rhs)
	{
		set(rhs);
		return *this;
	}

	const AtomVector& AtomVector::operator = (const Composite& rhs)
	{
		set(rhs, rhs.containsSelection());
		return *this;
	}

	void AtomVector::set(const AtomVector& atoms)
	{
		// resize the vector and copy the contents of atoms
		resize((Size)atoms.size());
		copy(atoms.begin(), atoms.end(), begin());

		// copy the saved positions
		saved_position_.resize(atoms.saved_position_.size());
		copy(atoms.saved_position_.begin(), atoms.saved_position_.end(), saved_position_.begin());
	}

	void AtomVector::set(const Composite& composite, bool selected_only)
	{
		// clear all saved positions and atom pointers
		saved_position_.clear();
		clear();

		// iterate over all subcomposites and
		// extract pointers to the atoms
		Composite::CompositeConstIterator it = composite.beginComposite();
		for (; +it; ++it)
		{
			Atom* atom = const_cast<Atom*>(dynamic_cast<const Atom*>(&*it));
			if (atom != 0)
			{
				// store this atom only if it is selected or selected_only == false
				if (!selected_only || atom->isSelected())
				{
					push_back(atom);
				}
			}
		}
	}

	void AtomVector::savePositions()
	{
		// resize the saved positions
		saved_position_.resize(size());

		// copy all positions
		ConstIterator it(begin());
		vector<Vector3>::iterator pos_it(saved_position_.begin());
		for (; it != end(); ++it, ++pos_it)
		{
			*pos_it = (*it)->getPosition();
		}
	}

	void AtomVector::resetPositions()
	{
		// move only if a saved position exists for every atom
		if (saved_position_.size() == size())
		{
			Iterator it(begin());
			vector<Vector3>::const_iterator pos_it(saved_position_.begin());
			for (; it != end(); ++it, ++pos_it)
			{
				(*it)->setPosition(*pos_it);
			}
		}
	}

	void AtomVector::moveTo(const Gradient& gradient, double step)
	{
		// move only if a saved position exists for every atom
		if (gradient.size() == size())
		{
			// use the saves positions
			if (saved_position_.size() == size())
			{
				Iterator it(begin());
				vector<Vector3>::const_iterator pos_it(saved_position_.begin());
				Gradient::ConstIterator grad_it(gradient.begin());
				for (; it != end(); ++it, ++pos_it, ++grad_it)
				{
					(*it)->setPosition(*pos_it + *grad_it * step);
					#ifdef BALL_DEBUG
						Log << "   - atom " << (*it)->getFullName() << " @ " << (*it)->getPosition() << std::endl;
					#endif
				}
			}
			// we don't have saved positions, use the current atom positions
			else 
			{
				Iterator it(begin());
				Gradient::ConstIterator grad_it(gradient.begin());
				for (; it != end(); ++it, ++grad_it)
				{
					(*it)->getPosition() += *grad_it * step;
					#ifdef BALL_DEBUG
						Log << "   - atom " << (*it)->getFullName() << " @ " << (*it)->getPosition() << std::endl;
					#endif
				}				
			}
		}
	}

	void AtomVector::moveTo(const Gradient& gradient, double step, Size number)
	{
		// move only if a saved position exists for every atom
		if (gradient.size() == size())
		{
			// use the saves positions
			if (saved_position_.size() == size())
			{
				Iterator it(begin());
				vector<Vector3>::const_iterator pos_it(saved_position_.begin());
				vector<Vector3>::const_iterator end(saved_position_.begin() + number);
				Gradient::ConstIterator grad_it(gradient.begin());
				for (; pos_it != end; ++it, ++pos_it, ++grad_it)
				{
					(*it)->setPosition(*pos_it + *grad_it * step);
					#ifdef BALL_DEBUG
						Log << "   - atom " << (*it)->getFullName() << " @ " << (*it)->getPosition() << std::endl;
					#endif
				}
			}
			// we don't have saved positions, use the current atom positions
			else 
			{
				Iterator it(begin());
				Gradient::ConstIterator grad_it(gradient.begin());
				Gradient::ConstIterator end(gradient.begin() + number);
				for (; grad_it != end; ++it, ++grad_it)
				{
					(*it)->getPosition() += *grad_it * step;
					#ifdef BALL_DEBUG
						Log << "   - atom " << (*it)->getFullName() << " @ " << (*it)->getPosition() << std::endl;
					#endif
				}				
			}
		}
	}

	void AtomVector::resize(Size new_size)
	{
		Size old_size = (Size)size();
		::std::vector<Atom*>::resize(new_size);
		for (Position p = old_size; p < new_size; p++)
		{
			(*this)[p] = 0;
		}
	}
	
} // namespace BALL
