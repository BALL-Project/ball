// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: amberBend.C,v 1.27.26.1 2007/03/25 22:00:25 oliver Exp $
//

#include <BALL/MOLMEC/AMBER/amberBend.h>
#include <BALL/MOLMEC/AMBER/amber.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>

using namespace std;

namespace BALL 
{

	// default constructor
	AmberBend::AmberBend()
		:	ForceFieldComponent()
	{	
		// set component name
		setName("Amber Bend");
	}


	// constructor
	AmberBend::AmberBend(ForceField& force_field)
		:	ForceFieldComponent(force_field)
	{
		// set component name
		setName("Amber Bend");
	}


	// copy constructor
	AmberBend::AmberBend(const AmberBend&	component)
		:	ForceFieldComponent(component)
	{
		bend_ = component.bend_;
	}

	// destructor
	AmberBend::~AmberBend()
	{
	}

	// setup the internal datastructures for the component
	bool AmberBend::setup()
		throw(Exception::TooManyErrors)
	{
		// clear old bends:
		bend_.clear();

		if (getForceField() == 0) 
		{
			Log.error() << "AmberBend::setup: component not bound to force field" << endl;
			return false;
		}

 		Options& options = getForceField()->options;
		if (options.has(AMBER_BEND_ENABLED))
		{
			if (!options.getBool(AMBER_BEND_ENABLED))
			{
				setEnabled(false);
				return true;
			}
			else
			{
				setEnabled(true);
			}
		}

		static bool result = false;
		AmberFF* amber_force_field = dynamic_cast<AmberFF*>(force_field_);
		if ((amber_force_field == 0) || !amber_force_field->hasInitializedParameters())
		{
			result = bend_parameters_.extractSection(getForceField()->getParameters(), "QuadraticAngleBend");

			if (result == false) 
			{
				Log.error() << "AmberBend::setup: cannot find section QuadraticAngleBend" << endl;
				return false;
			}
		}

		// Assign all bend parameters.
		bool use_selection = getForceField()->getUseSelection();
		vector<Atom*>::const_iterator	atom_it = getForceField()->getAtoms().begin();
		Atom::BondIterator it1;
		Atom::BondIterator it2;
		QuadraticAngleBend::Data this_bend;
		for ( ; atom_it != getForceField()->getAtoms().end(); ++atom_it) 
		{
			for (it2 = (*atom_it)->beginBond(); +it2 ; ++it2) 
			{
				if (it2->getType() == Bond::TYPE__HYDROGEN) continue; // Skip H-bonds
				for (it1 = it2, ++it1; +it1 ; ++it1) 
				{
					if (it1->getType() == Bond::TYPE__HYDROGEN) continue; // Skip H-Bonds;
					this_bend.atom1 = &Atom::getAttributes()[it2->getPartner(**atom_it)->getIndex()];
					this_bend.atom2 = &Atom::getAttributes()[(*atom_it)->getIndex()];
					this_bend.atom3 = &Atom::getAttributes()[it1->getPartner(**atom_it)->getIndex()];

					if ((use_selection == false) ||
					   (use_selection == true && 
					   (this_bend.atom1->ptr->isSelected() 
							&& this_bend.atom2->ptr->isSelected() 
							&& this_bend.atom3->ptr->isSelected())))
					{ 

						Atom::Type atom_type_a1 = this_bend.atom1->type;
						Atom::Type atom_type_a2 = this_bend.atom2->type;
						Atom::Type atom_type_a3 = this_bend.atom3->type;

						// check for parameters
						if (!bend_parameters_.assignParameters(this_bend.values, atom_type_a1, atom_type_a2, atom_type_a3))
						{
							// handle wildcards: if the atom type is not known, try to match *-A2-* 
							if (!bend_parameters_.assignParameters(this_bend.values, Atom::ANY_TYPE, atom_type_a2, Atom::ANY_TYPE))
							{
								// complain if nothing was found
								getForceField()->error() << "AmberBend::setup: cannot find bend parameters for atom types:"
									<< force_field_->getParameters().getAtomTypes().getTypeName(atom_type_a1) << "-"
									<< force_field_->getParameters().getAtomTypes().getTypeName(atom_type_a2) << "-"
									<< force_field_->getParameters().getAtomTypes().getTypeName(atom_type_a3) 
									<< " (atoms are: " << this_bend.atom1->ptr->getFullName(Atom::ADD_VARIANT_EXTENSIONS_AND_ID) << "/" 
									<< this_bend.atom2->ptr->getFullName(Atom::ADD_VARIANT_EXTENSIONS_AND_ID) << "/" 
									<< this_bend.atom3->ptr->getFullName(Atom::ADD_VARIANT_EXTENSIONS_AND_ID) << ")" << endl;

								getForceField()->getUnassignedAtoms().insert(it2->getPartner(**atom_it));
   							getForceField()->getUnassignedAtoms().insert(*atom_it);
								getForceField()->getUnassignedAtoms().insert(it1->getPartner(**atom_it));
							}
							else
							{
								// store the bend parameters otherwise
								bend_.push_back(this_bend);
							}
						} 
						else 
						{
							// store the bend parameters otherwise
							bend_.push_back(this_bend);
						}
					}
				}
			}
		}

		// everything went well
		return true;
	}

	// calculates the current energy of this component
	double AmberBend::updateEnergy()
	{
		energy_ = 0;
		
		// abort for an empty vector
		if (bend_.size() == 0)
		{
			return 0.0;
		}

		Vector3 v1, v2;
		QuadraticAngleBend::Data* bend_it = &(bend_[0]);
		QuadraticAngleBend::Data* bend_end = &(bend_[bend_.size() - 1]);
		for (; bend_it <= bend_end ; ++bend_it) 
		{
			if (getForceField()->considerAtomTriple(bend_it->atom1->ptr, bend_it->atom2->ptr, bend_it->atom3->ptr))
			{
				v1 = bend_it->atom1->position - bend_it->atom2->position;
				v2 = bend_it->atom3->position - bend_it->atom2->position;
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
	void AmberBend::updateForces()
	{
		if ((getForceField() == 0) || (getForceField()->getSystem() == 0))
		{
			return;
		}

		for (Size i = 0; i < bend_.size(); i++) 
		{
			if (getForceField()->considerAtomTriple(bend_[i].atom1->ptr, bend_[i].atom2->ptr, bend_[i].atom3->ptr))
			{

				// Calculate the vector between atom1 and atom2,
				// test if the vector has length larger than 0 and normalize it

				Vector3 v1 = bend_[i].atom1->position - bend_[i].atom2->position;
				Vector3 v2 = bend_[i].atom3->position - bend_[i].atom2->position;
				double length = v1.getLength();

				if (length == 0) continue;
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

				if (getForceField()->considerAtom(bend_[i].atom1->ptr))
				{
					bend_[i].atom1->force -= n1;
				}
				if (getForceField()->considerAtom(bend_[i].atom2->ptr))
				{
					bend_[i].atom2->force += n1;
					bend_[i].atom2->force -= n2;
				}	
				if (getForceField()->considerAtom(bend_[i].atom3->ptr))
				{
					bend_[i].atom3->force += n2;
				}
			}
		}
	}

	bool AmberBend::exportParmFile(File& outfile) const
	{
		return bend_parameters_.exportParmFile(outfile);
	}


} // namespace BALL 
