// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MMFF94Stretch.C,v 1.1.2.2 2005/03/21 16:51:23 amoll Exp $
//

#include <BALL/MOLMEC/MMFF94/MMFF94Stretch.h>
#include <BALL/MOLMEC/MMFF94/MMFF94.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/SYSTEM/path.h>

using namespace std;

namespace BALL 
{

	// default constructor
	MMFF94Stretch::MMFF94Stretch()
		:	ForceFieldComponent()
	{	
		// set component name
		setName("MMFF94 Stretch");
	}


	// constructor
	MMFF94Stretch::MMFF94Stretch(ForceField& force_field)
		: ForceFieldComponent(force_field)
	{
		// set component name
		setName("MMFF94 Stretch");
	}


	// copy constructor
	MMFF94Stretch::MMFF94Stretch(const MMFF94Stretch&	component)
		:	ForceFieldComponent(component)
	{
	}

	// destructor
	MMFF94Stretch::~MMFF94Stretch()
	{
	}


	// setup the internal datastructures for the component
	bool MMFF94Stretch::setup()
		throw(Exception::TooManyErrors)
	{
		if (getForceField() == 0) 
		{
			Log.error() << "MMFF94Stretch::setup(): component not bound to force field" << endl;
			return false;
		}

		if (!parameters_.isInitialized())
		{
			Path    path;
			String  filename(path.find("MMFF94/MMFFBOND.PAR"));

			if (filename == "") 
			{
				throw Exception::FileNotFound(__FILE__, __LINE__, filename);
			}

			parameters_.readParameters(filename);
		}

		stretch_.clear();

		bool use_selection = getForceField()->getUseSelection();

		Stretch dummy_stretch;

		AtomVector::ConstIterator atom_it = getForceField()->getAtoms().begin();
//   		Atom::AttributeVector& attributes = Atom::getAttributes();
		for ( ; atom_it != getForceField()->getAtoms().end(); ++atom_it)
		{
			for (Atom::BondIterator it = (*atom_it)->beginBond(); +it ; ++it) 
			{
				if (*atom_it != it->getFirstAtom()) continue;
				
				Bond&	bond = const_cast<Bond&>(*it);
				if (bond.getType() == Bond::TYPE__HYDROGEN)
				{	
					// Ignore hydrogen bonds!
					continue;
				}

				if (!use_selection ||
						(use_selection && bond.getFirstAtom()->isSelected() && 
															bond.getSecondAtom()->isSelected()))
				{
					const Atom::Type atom_type_A = bond.getFirstAtom()->getType();
					const Atom::Type atom_type_B = bond.getSecondAtom()->getType();
					if (!parameters_.getParameters(atom_type_A, atom_type_B, dummy_stretch.kb, dummy_stretch.r0))
					{
						getForceField()->error() << "cannot find stretch parameters for atom types " 
								<< atom_type_A << " " << atom_type_B 
								<< " (atoms are: " 
								<< bond.getFirstAtom()->getFullName(Atom::ADD_VARIANT_EXTENSIONS_AND_ID) << " " 
								<< bond.getSecondAtom()->getFullName(Atom::ADD_VARIANT_EXTENSIONS_AND_ID) << ")" << std::endl;

						getForceField()->getUnassignedAtoms().insert(bond.getFirstAtom());
						getForceField()->getUnassignedAtoms().insert(bond.getSecondAtom());
						continue;
					}

					dummy_stretch.atom1 = (Atom*) bond.getFirstAtom();
					dummy_stretch.atom2 = (Atom*) bond.getSecondAtom();

					stretch_.push_back(dummy_stretch);
				}
			}
		}

		return true;
	}

	// update bond lists if the selection has changed
	void MMFF94Stretch::update()
		throw(Exception::TooManyErrors)
	{
	}
	

	// calculates the current energy of this component
	double MMFF94Stretch::updateEnergy()
	{
		// initial energy is zero
		energy_ = 0;

//   		bool use_selection = getForceField()->getUseSelection();

		for (Size i = 0 ; i < stretch_.size(); i++)
		{
			const Vector3 direction(stretch_[i].atom1->getPosition() - stretch_[i].atom2->getPosition());
			double distance = direction.getLength(); 
			const float delta(::std::fabs(distance - stretch_[i].r0));
			const float delta_2(delta * delta);

			float eb_ij = 143.9325 * stretch_[i].kb / 2.0 * delta_2 *
				            (1.0 + CUBIC_STRENGTH_CONSTANT * delta + 7.0 / 12.0 CUBIC_STRENGTH_CONSTANT * CUBIC_STRENGTH_CONSTANT * delta_2);

			Log.info() << stretch_[i].atom1->getFullName() << " -> " 
								 << stretch_[i].atom2->getFullName() << " : bond stretch energy " << eb_ij << std::endl;

			energy_ += eb_ij;
		}


		return energy_;
	}

	// calculates and adds its forces to the current forces of the force field
	void MMFF94Stretch::updateForces()
	{
		if (getForceField() == 0)
		{
			return;
		}

//   		bool use_selection = getForceField()->getUseSelection();

		/*
		// iterate over all bonds, update the forces
		for (Size i = 0 ; i < stretch_.size(); i++)
		{
			Atom::StaticAtomAttributes& atom1(*stretch_[i].atom1);
			Atom::StaticAtomAttributes& atom2(*stretch_[i].atom2);
			Vector3 direction(atom1.position - atom2.position);
			double distance = direction.getLength(); 

			if (distance != 0.0) 
			{
				// unit conversion: from kJ/(mol A) -> N
				//   kJ -> J: 1e3
				//   A  -> m: 1e10
				//   J/mol -> J: Avogadro
//   				direction *= 1e13 / Constants::AVOGADRO * 2 * stretch_[i].values.k * (distance - stretch_[i].values.r0) / distance;

				if (!use_selection || atom1.ptr->isSelected()) 
				{
					atom1.force -= direction;
				}
				if (!use_selection || atom2.ptr->isSelected()) 
				{
					atom2.force += direction;
				}
			}
		}                                                                                                          
		*/
	}

} // namespace BALL
