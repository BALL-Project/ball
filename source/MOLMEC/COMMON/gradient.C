// $Id: gradient.C,v 1.5 2001/07/05 17:52:34 oliver Exp $
// Atom gradient class: this class represents the gradient (i.e. the negative forces)
// for a given system as a vector<Vector3>. THe gradient is stored in units of kJ/(mol A)

#include <BALL/MOLMEC/COMMON/gradient.h>
#include <BALL/MOLMEC/COMMON/atomVector.h>
#include <BALL/KERNEL/atom.h>

using namespace std;

namespace BALL
{
	Gradient::Gradient()
		: norm(0.0),
			inv_norm(0.0),
			rms(0.0),
			valid_(false)
	{
	}

	Gradient::Gradient(const AtomVector& atoms)
	{
		set(atoms);
	}

	Gradient::Gradient(const Gradient& gradient, bool /* deep */)
		: vector<Vector3>()
	{
		set(gradient);
	}

	Gradient::~Gradient()
	{
		valid_ = false;
	}

	void Gradient::set(const AtomVector& atoms)
	{
		// change the size to hold all vectors
		Size max_index = (Size)atoms.size();
		resize(max_index);

		// copy all forces.
		// the gradient is the negative force and is
		// stored in units of kJ/(mol A). The forces
		// are in units of Newton, so we have to use
		// a conversion factor of -1.0 / 1e3 (J->kJ) / 1e10 (m->A) * NA (1->mol)
		norm = 0.0;
		Iterator it(begin());
		for (Size i = 0; i < max_index; ++i, ++it)
		{
			*it = atoms[i]->getForce() * Constants::NA / -1.0e13;
			norm += (*it) * (*it);
		}

		// calculate the norm and its inverse
		norm = sqrt(norm);
		inv_norm = 1.0 / norm;
		if (max_index > 0)
		{
			rms = norm / sqrt(3.0 * (double)max_index);
		}
		else 
		{ 
			rms = 0.0;
		}

		// the gradient is now valid
		valid_ = true;
	}

	void Gradient::set(const Gradient& gradient)
	{
		// copy the gradient
		resize(gradient.size());
		copy(gradient.begin(), gradient.end(), begin());
		
		//copy the norm and the valid_ flag	
		norm = gradient.norm;
		inv_norm = gradient.inv_norm;
		rms = gradient.rms;
		valid_ = gradient.valid_;
	}

	Gradient& Gradient::operator = (const Gradient& rhs)
	{
		set(rhs);
		return *this;
	}

	Gradient& Gradient::operator = (const AtomVector& rhs)
	{
		set(rhs);
		return *this;
	}

	// dot product of two gradients
	double Gradient::operator * (const Gradient& gradient) const
	{
		Size max_index = (Size)size();
		if (gradient.size() != max_index)
		{
			throw Exception::InvalidRange(__FILE__, __LINE__);
		}

		double result = 0.0;
		for (Size i = 0; i < max_index; i++)
		{
			result += operator[](i) * gradient[i];
		}

		return result;
	}

	void Gradient::negate()
	{
		// iterate over all vectors and flip the sign
		for (Iterator it = begin(); it != end(); ++it)
		{
			*it *= -1.0;
		}
	}

	void Gradient::normalize()
	{
		// iterate over all vectors and flip the sign
		// (TVector3::negate)
		for (Iterator it = begin(); it != end(); ++it)
		{
			*it *= inv_norm;
		}		

		// reset the norm and its inverse
		// and calculate hte root mean square
		norm = 1.0;
		inv_norm = 1.0;
		if (size() > 0)
		{
			rms = 1.0 / sqrt(3.0 * (double)size());
		}
		else 
		{
			rms = 0.0;
		}
	}

	bool Gradient::isValid() const
	{
		return valid_;
	}

	void Gradient::invalidate()
	{
		valid_ = false;
	}

} // namespace BALL
