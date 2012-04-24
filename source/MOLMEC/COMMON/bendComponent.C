#include <BALL/MOLMEC/COMMON/bendComponent.h>

#include <BALL/MOLMEC/COMMON/forceField.h>

namespace BALL
{

	BendComponent::BendComponent()
	{
	}

	BendComponent::BendComponent(ForceField& ff)
		: ForceFieldComponent(ff)
	{
	}

	BendComponent::~BendComponent()
	{
	}

	// calculates the current energy of this component
	double BendComponent::updateEnergy()
	{
		energy_ = 0;

		// abort for an empty vector
		if (bend_.empty())
		{
			return 0.0;
		}

		Vector3 v1, v2;
		bool use_selection = getForceField()->getUseSelection();
		QuadraticAngleBend::Data* bend_it = &(bend_[0]);
		QuadraticAngleBend::Data* bend_end = &(bend_[bend_.size() - 1]);
		for (; bend_it <= bend_end ; ++bend_it)
		{
			const Atom* atom1 = bend_it->atom1;
			const Atom* atom2 = bend_it->atom2;
			const Atom* atom3 = bend_it->atom3;
			if (use_selection == false ||
					(   atom1->isSelected()
					 || atom2->isSelected()
					 || atom3->isSelected()))
			{
				v1 = atom1->getPosition() - atom2->getPosition();
				v2 = atom3->getPosition() - atom2->getPosition();
				double square_length = v1.getSquareLength() * v2.getSquareLength();

				if (square_length == 0.0)
				{
					continue;
				}

				double costheta = v1 * v2 / sqrt(square_length);
				double theta;
				if (costheta > 1.0)
				{
					theta = 0.0;
				}
				else if (costheta < -1.0)
				{
					theta = Constants::PI;
				}
				else
				{
					theta = acos(costheta);
				}

				theta -= bend_it->values.theta0;
				energy_ += bend_it->values.k * theta * theta;
			}
		}

		return energy_;
	}

	// calculates and adds its forces to the current forces of the force field
	void BendComponent::updateForces()
	{
		if ((getForceField() == 0) || (getForceField()->getSystem() == 0))
		{
			return;
		}

		bool use_selection = getForceField()->getUseSelection();
		for (Size i = 0; i < bend_.size(); i++)
		{
			Atom* atom1 = bend_[i].atom1;
			Atom* atom2 = bend_[i].atom2;
			Atom* atom3 = bend_[i].atom3;

			if ((use_selection == false)
					|| atom1->isSelected()
					|| atom2->isSelected()
					|| atom3->isSelected())
			{

				// Calculate the vector between atom1 and atom2,
				// test if the vector has length larger than 0 and normalize it

				Vector3 v1 = atom1->getPosition() - atom2->getPosition();
				Vector3 v2 = atom3->getPosition() - atom2->getPosition();
				double length = v1.getLength();

				if (length == 0.0) continue;
				double inverse_length_v1 = 1.0 / length;
				v1 *= inverse_length_v1 ;

				// Calculate the vector between atom3 and atom2,
				// test if the vector has length larger than 0 and normalize it

				length = v2.getLength();
				if (length == 0.0) continue;
				double inverse_length_v2 = 1/length;
				v2 *= inverse_length_v2;

				// Calculate the cos of theta and then theta
				double costheta = v1 * v2;
				double theta;
				if (costheta > 1.0)
				{
					theta = 0.0;
				}
				else if (costheta < -1.0)
				{
					theta = Constants::PI;
				}
				else
				{
					theta = acos(costheta);
				}

				// unit conversion: kJ/(mol A) -> N
				// kJ -> J: 1e3
				// A -> m : 1e10
				// J/mol -> mol: Avogadro
				double factor = 1e13 / Constants::AVOGADRO * 2 * bend_[i].values.k * (theta - bend_[i].values.theta0);

				// Calculate the cross product of v1 and v2, test if it has length unequal 0,
				// and normalize it.

				Vector3 cross = v1 % v2;
				if ((length = cross.getLength()) != 0)
				{
					cross *= (1.0 / length);
				}
				else
				{
					continue;
				}

				Vector3 n1 = v1 % cross;
				Vector3 n2 = v2 % cross;
				n1 *= factor * inverse_length_v1;
				n2 *= factor * inverse_length_v2;

				if (use_selection == false)
				{
					atom1->getForce() -= n1;
					atom2->getForce() += n1;
					atom2->getForce() -= n2;
					atom3->getForce() += n2;
				}
				else
				{
					if (atom1->isSelected())
					{
						atom1->getForce() -= n1;
					}

					if (atom2->isSelected())
					{
						atom2->getForce() += n1;
						atom2->getForce() -= n2;
					}
					if (atom3->isSelected())
					{
						atom3->getForce() += n2;
					}
				}
			}
		}
	}
}
