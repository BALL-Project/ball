// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MMFF94Bend.C,v 1.1.2.9 2005/03/28 00:43:55 amoll Exp $
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

	/// 0.043844 / 2
	#define K0 0.021922

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

					if (use_selection && (!this_bend.atom1->ptr->isSelected() ||
																!this_bend.atom2->ptr->isSelected() ||
																!this_bend.atom3->ptr->isSelected()))
					{
						continue;
					}

					Atom::Type atom_type_a1 = this_bend.atom1->type;
					Atom::Type atom_type_a2 = this_bend.atom2->type;
					Atom::Type atom_type_a3 = this_bend.atom3->type;

					this_bend.ATIJK = getBendType(*it1, *it2,
																				*this_bend.atom1->ptr,
																				*this_bend.atom2->ptr,
																				*this_bend.atom3->ptr);

					// check for parameters
					if (!parameters_.getParameters(this_bend.ATIJK, 
																				 atom_type_a1, atom_type_a2, atom_type_a3, 
																				 this_bend.ka, this_bend.theta0))
					{
						// try wildcard matching
						if (!parameters_.getParameters(this_bend.ATIJK, 
																					 0, atom_type_a2, 0, 
																					 this_bend.ka, this_bend.theta0))
						{
							// complain if nothing was found
							getForceField()->error() << "MMFF94Bend::setup: cannot find bend parameters for atom types:"
								<< atom_type_a1 << "-" << atom_type_a2 << "-" << atom_type_a3 << "bend " << this_bend.ATIJK
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

		// everything went well
		return true;
	}

	// calculates the current energy of this component
	double MMFF94Bend::updateEnergy()
	{
		energy_ = 0;
		
		Vector3 v1, v2;
		vector<Bend>::iterator bend_it = bends_.begin();

		float radian_to_degree = 180.0 / Constants::PI;
		float degree_to_radian= Constants::PI / 180;

		// -0.007 degree^-1
		const float k1 = -0.007;

		for (; bend_it != bends_.end(); ++bend_it) 
		{
			const float& ka = bend_it->ka;
			const float& theta0 = bend_it->theta0;

		
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

#ifdef BALL_DEBUG_MMFF
Log.info() << "Bend " << bend_it->atom1->ptr->getName() << " " 
											<< bend_it->atom2->ptr->getName() << " " 
											<< bend_it->atom3->ptr->getName() << " " 
											<< bend_it->atom1->type << " "
											<< bend_it->atom2->type << " "
											<< bend_it->atom3->type << " "
											<< "ATIJK: " << bend_it->ATIJK << "  T: "
											<< theta << "  T0: " << theta0 << " ka " << ka;
#endif

			float energy;
			if (bend_it->is_linear)
			{ 
				energy = 143.9325 * ka * (1.0 + cos(theta * degree_to_radian));

				// we needed the absolute angle, now calculate the delta
				theta -= theta0;
			}
			else
			{
				theta -= theta0;
				energy = K0 * ka * theta * theta * (1.0 + k1 * theta);
			}

			bend_it->delta_theta = theta;

#ifdef BALL_DEBUG_MMFF
	Log.info() << "  E: "<< energy << std::endl;
	bend_it->energy = energy;
#endif

			energy_ += energy;
		}
		
		return energy_;
	}


	// calculates and adds its forces to the current forces of the force field
	void MMFF94Bend::updateForces()
	{
	}

	Position MMFF94Bend::getBendType(const Bond& bond1, const Bond& bond2,
										 					 		 Atom& atom1, Atom& atom2, Atom& atom3) const
	{
		/* 	The angle-bending parameters employ angle-type indices ATIJK ranging 
		  	between 0 and 8.  Their meanings are as defined below:

				ATIJK           Structural significance
				 ---------------------------------------------------------------------------
					 0            The angle i-j-k is a "normal" bond angle
					 1            Either bond i-j or bond j-k has a bond type of 1
					 2            Both i-j and j-k have bond types of 1; the sum is 2.
					 3            The angle occurs in a three-membered ring
					 4            The angle occurs in a four-membered ring
					 5            Is in a three-membered ring and the sum of the bond types is 1
					 6            Is in a three-membered ring and the sum of the bond types is 2
					 7            Is in a four-membered ring and the sum of the bond types is 1
					 8            Is in a four-membered ring and the sum of the bond types is 2 
		*/

		MMFF94& mmff = *(MMFF94*)getForceField();
		const vector<HashSet<Atom*> >& all_rings = mmff.getRings();

		bool in_ring_of_three = false;
		bool in_ring_of_four  = false;

		/// test the rings in which all atoms occur
		for (Position ring_nr = 0; ring_nr < all_rings.size(); ring_nr++)
		{
			Size size = all_rings[ring_nr].size();
			if (size < 3 || size > 4) continue;

			if (!all_rings[ring_nr].has(&atom1) ||
			    !all_rings[ring_nr].has(&atom2) ||
			    !all_rings[ring_nr].has(&atom3))
			{
				continue;
			}

			if (size == 3) in_ring_of_three = true;
			if (size == 4) in_ring_of_four  = true;
		}

		/// calculate sum of bond_types
		Size sum_bond_types = 0;
		if (bond1.hasProperty("MMFF94SBMB")) sum_bond_types ++;
		if (bond2.hasProperty("MMFF94SBMB")) sum_bond_types ++;

		if (in_ring_of_four)
		{
			Position result = 4;
			if (sum_bond_types != 0) result = 6 + sum_bond_types;
			return result;
		}

		if (in_ring_of_three)
		{
			Position result = 3;
			if (sum_bond_types != 0) result = 4 + sum_bond_types;
			return result;
		}

		return sum_bond_types;
	}

} // namespace BALL
