// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MMFF94Bend.C,v 1.1.2.2 2005/03/24 16:17:33 amoll Exp $
//

#include <BALL/MOLMEC/MMFF94/MMFF94Bend.h>
#include <BALL/MOLMEC/MMFF94/MMFF94.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/SYSTEM/path.h>

#include <math.h>

#define BALL_DEBUG_MMFF

using namespace std;

namespace BALL 
{

	// default constructor
	MMFF94Bend::MMFF94Bend()
		:	ForceFieldComponent()
	{	
		// set component name
		setName("MMFF94 Bend");
	}


	// constructor
	MMFF94Bend::MMFF94Bend(ForceField& force_field)
		: ForceFieldComponent(force_field)
	{
		// set component name
		setName("MMFF94 Bend");
	}


	// copy constructor
	MMFF94Bend::MMFF94Bend(const MMFF94Bend&	component)
		:	ForceFieldComponent(component)
	{
	}

	// destructor
	MMFF94Bend::~MMFF94Bend()
	{
	}


	// setup the internal datastructures for the component
	bool MMFF94Bend::setup()
		throw(Exception::TooManyErrors)
	{
		if (getForceField() == 0) 
		{
			Log.error() << "MMFF94Bend::setup(): component not bound to force field" << endl;
			return false;
		}

		if (!parameters_.isInitialized())
		{
			Path    path;
			String  filename(path.find("MMFF94/MMFFANG.PAR"));

			if (filename == "") 
			{
				throw Exception::FileNotFound(__FILE__, __LINE__, filename);
			}

			parameters_.readParameters(filename);
		}

		bends_.clear();

		bool use_selection = getForceField()->getUseSelection();

		// a working instance to put the current values in and push it back
		Bend this_bend;

		const vector<MMFF94AtomTypeData>& atom_types = ((MMFF94*)getForceField())->getAtomTypes();

		vector<Atom*>::const_iterator	atom_it = getForceField()->getAtoms().begin();
		Atom::BondIterator it1;
		Atom::BondIterator it2;
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

					if (!use_selection ||
					   (use_selection && (this_bend.atom1->ptr->isSelected() &&
																this_bend.atom2->ptr->isSelected() &&
																this_bend.atom3->ptr->isSelected())))
					{ 
						Atom::Type atom_type_a1 = this_bend.atom1->type;
						Atom::Type atom_type_a2 = this_bend.atom2->type;
						Atom::Type atom_type_a3 = this_bend.atom3->type;

						// check for parameters
						if (!parameters_.getParameters(atom_type_a1, atom_type_a2, atom_type_a3, this_bend.ka, this_bend.theta0))
						{
							// handle wildcards: if the atom type is not known, try to match *-A2-* 
//   							if (!bend_parameters.assignParameters(this_bend.values, Atom::ANY_TYPE, atom_type_a2, Atom::ANY_TYPE))
							{
								// complain if nothing was found
								getForceField()->error() << "AmberBend::setup: cannot find bend parameters for atom types:"
									<< atom_type_a1 << "-" << atom_type_a2 << "-" << atom_type_a3 
									<< " (atoms are: " << this_bend.atom1->ptr->getFullName(Atom::ADD_VARIANT_EXTENSIONS_AND_ID) << "/" 
									<< this_bend.atom2->ptr->getFullName(Atom::ADD_VARIANT_EXTENSIONS_AND_ID) << "/" 
									<< this_bend.atom3->ptr->getFullName(Atom::ADD_VARIANT_EXTENSIONS_AND_ID) << ")" << endl;

								getForceField()->getUnassignedAtoms().insert(it2->getPartner(**atom_it));
   							getForceField()->getUnassignedAtoms().insert(*atom_it);
								getForceField()->getUnassignedAtoms().insert(it1->getPartner(**atom_it));
								continue;
							}
						} 
						else 
						{
							this_bend.is_linear = atom_types[atom_type_a2].lin;

							// store the bend parameters otherwise
							bends_.push_back(this_bend);
						}
					}
				}
			}
		}

		// everything went well
		return true;
	}

	// calculates the current energy of this component
	double MMFF94Bend::updateEnergy()
	{
		energy_ = 0;
		
		Vector3 v1, v2;
		vector<Bend>::iterator bend_it = bends_.begin();

		float degree_to_radian = Constants::PI / 180.0;
		float radian_to_degree = 180.0 / Constants::PI;

		// -0.4 rad^-1
		const float k1 = -0.4 * radian_to_degree;

		for (; bend_it != bends_.end(); ++bend_it) 
		{
			const float& ka = bend_it->ka;
			const float& theta0 = bend_it->theta0;

			if (bend_it->is_linear)
			{
				const float energy = 143.9325 * ka * (1.0 + cos(theta0 * degree_to_radian));
				energy_ += energy;
				continue;
			}
			
			v1 = bend_it->atom1->position - bend_it->atom2->position;
			v2 = bend_it->atom3->position - bend_it->atom2->position;
			const double square_length = v1.getSquareLength() * v2.getSquareLength();

			if (square_length == 0.0) 
			{
				continue;
			}

			const double costheta = v1 * v2 / sqrt(square_length);
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

			theta *= radian_to_degree;
			theta -= theta0;

#ifdef BALL_DEBUG_MMFF
Log.info() << "Bend " << bend_it->atom1->ptr->getName() << " " 
											<< bend_it->atom2->ptr->getName() << " " 
											<< bend_it->atom3->ptr->getName() << "  T: " 
											<< theta0 << std::endl;
#endif

			energy_ += K0 * ka * theta * theta * (1.0 + k1 * theta);
		}
		
		return energy_;
	}


	// calculates and adds its forces to the current forces of the force field
	void MMFF94Bend::updateForces()
	{
	}

} // namespace BALL
