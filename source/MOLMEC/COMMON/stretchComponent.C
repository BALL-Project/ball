#include <BALL/MOLMEC/COMMON/stretchComponent.h>

#include <BALL/MOLMEC/COMMON/forceField.h>

namespace BALL
{

	StretchComponent::StretchComponent()
	{
	}

	StretchComponent::StretchComponent(ForceField& ff)
		: ForceFieldComponent(ff)
	{
	}

	StretchComponent::~StretchComponent()
	{
	}

	double StretchComponent::updateEnergy()
	{
		// initial energy is zero
		energy_ = 0;

		bool use_selection = getForceField()->getUseSelection();

		// iterate over all bonds, sum up the energies
		for (Size i = 0; i < stretch_.size(); i++)
		{
			const Atom* atom1 = stretch_[i].atom1;
			const Atom* atom2 = stretch_[i].atom2;
			if (!use_selection || atom1->isSelected() || atom2->isSelected())
			{
				double distance = atom1->getDistance(*atom2);
				energy_ += stretch_[i].values.k * (distance - stretch_[i].values.r0) * (distance - stretch_[i].values.r0);
			}
		}

		return energy_;
	}

	// calculates and adds its forces to the current forces of the force field
	void StretchComponent::updateForces()
	{
		if (getForceField() == 0)
		{
			return;
		}

		bool use_selection = getForceField()->getUseSelection();

		// iterate over all bonds, update the forces
		for (Size i = 0 ; i < stretch_.size(); i++)
		{
			Atom* atom1 = stretch_[i].atom1;
			Atom* atom2 = stretch_[i].atom2;
			Vector3 direction(atom1->getPosition() - atom2->getPosition());
			double distance = direction.getLength();

			if (distance != 0.0)
			{
				// unit conversion: from kJ/(mol A) -> N
				//   kJ -> J: 1e3
				//   A  -> m: 1e10
				//   J/mol -> J: Avogadro
				direction *= 1e13 / Constants::AVOGADRO * 2 * stretch_[i].values.k * (distance - stretch_[i].values.r0) / distance;

				if (!use_selection || atom1->isSelected())
				{
					atom1->getForce() -= direction;
				}
				if (!use_selection || atom2->isSelected())
				{
					atom2->getForce() += direction;
				}
			}
		}
	}
}
