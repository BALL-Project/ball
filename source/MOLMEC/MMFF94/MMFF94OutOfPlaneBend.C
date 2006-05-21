// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MMFF94OutOfPlaneBend.C,v 1.1.4.1 2006/05/21 22:26:09 amoll Exp $
//

#include <BALL/MOLMEC/MMFF94/MMFF94OutOfPlaneBend.h>
#include <BALL/MOLMEC/MMFF94/MMFF94.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/SYSTEM/path.h>

#include <math.h>

//     #define BALL_DEBUG_MMFF
#define BALL_MMFF94_TEST

using namespace std;

namespace BALL 
{

	/// 0.043844 / 2
	#define K0 0.021922

	// default constructor
	MMFF94OutOfPlaneBend::MMFF94OutOfPlaneBend()
		:	ForceFieldComponent()
	{	
		// set component name
		setName("MMFF94 OutOfPlaneBend");
	}


	// constructor
	MMFF94OutOfPlaneBend::MMFF94OutOfPlaneBend(ForceField& force_field)
		: ForceFieldComponent(force_field)
	{
		// set component name
		setName("MMFF94 OutOfPlaneBend");
	}


	// copy constructor
	MMFF94OutOfPlaneBend::MMFF94OutOfPlaneBend(const MMFF94OutOfPlaneBend&	component)
		:	ForceFieldComponent(component)
	{
	}

	// destructor
	MMFF94OutOfPlaneBend::~MMFF94OutOfPlaneBend()
	{
	}


	// setup the internal datastructures for the component
	bool MMFF94OutOfPlaneBend::setup()
		throw(Exception::TooManyErrors)
	{
		if (getForceField() == 0 ||
				dynamic_cast<MMFF94*>(getForceField()) == 0) 
		{
			Log.error() << "MMFF94OutOfPlaneBend::setup(): component not bound to force field" << endl;
			return false;
		}

		MMFF94* mmff = dynamic_cast<MMFF94*>(getForceField());

		if (!parameters_.isInitialized())
		{
			Path    path;
			String  filename(path.find("MMFF94/MMFFOOP.PAR"));

			if (filename == "") throw Exception::FileNotFound(__FILE__, __LINE__, "[empty]");

			const MMFF94AtomTypeEquivalences& equiv = mmff->getEquivalences();
			parameters_.setEquivalences(equiv);
			parameters_.readParameters(filename);
		}

		bends_.clear();

		bool use_selection = getForceField()->getUseSelection();

		// a working instance to put the current values in and push it back
		OutOfPlaneBend this_bend;

		vector<Position> tp;
		tp.resize(3);

		vector<Atom*>::const_iterator	atom_it = getForceField()->getAtoms().begin();
		Atom::BondIterator bond_it;
		for ( ; atom_it != getForceField()->getAtoms().end(); ++atom_it) 
		{
			// search for tricoordinate centers (atoms with three bonds)
			Size nr_bonds = (*atom_it)->countBonds();
			if (nr_bonds < 3) continue;

			// count hydrogen bonds
			Size nr_hydrogen_bonds = 0;
			for (bond_it = (*atom_it)->beginBond(); +bond_it; ++bond_it)
			{
				if (bond_it->getType() == Bond::TYPE__HYDROGEN) nr_hydrogen_bonds++;
			}

			if (nr_bonds - nr_hydrogen_bonds != 3) continue;

			vector<Atom*> partners;
			Atom& central_atom = **atom_it;

			for (bond_it = central_atom.beginBond(); +bond_it; ++bond_it)
			{
				Bond& bond = *bond_it;

				if (bond.getType() == Bond::TYPE__HYDROGEN) continue; // Skip H-Bonds;

				Atom* partner = bond.getPartner(central_atom);

				partners.push_back(partner);
			}

			if (use_selection && (!central_atom.isSelected() ||
														!partners[1]->isSelected() ||
														!partners[2]->isSelected() ||
														!partners[3]->isSelected()))
			{
				continue;
			}


			this_bend.i= &Atom::getAttributes()[partners[0]->getIndex()];
			this_bend.j= &Atom::getAttributes()[central_atom.getIndex()];
			this_bend.k= &Atom::getAttributes()[partners[1]->getIndex()];
			this_bend.l= &Atom::getAttributes()[partners[2]->getIndex()];

			const Position type_j = central_atom.getType();

			Index tp0 = partners[0]->getType();
			Index tp1 = partners[1]->getType();
			Index tp2 = partners[2]->getType();

			// check for parameters in a step down procedure
			bool found = parameters_.getParameters(tp0, type_j, tp1, tp2, this_bend.k_oop);

			if (!found)
			{
				// complain if nothing was found
				getForceField()->error() << "MMFF94OutOfPlaneBend::setup: cannot find parameters for atom types:"
																 << tp0 << " " << type_j << " " << tp1 << " " << tp2 << " (atoms are: "
																 << central_atom.getFullName() << " " << partners[0]->getFullName() << " " 
																 << partners[1]->getFullName() << " " << partners[2]->getFullName() << ")" << endl;

				getForceField()->getUnassignedAtoms().insert(partners[0]);
				getForceField()->getUnassignedAtoms().insert(partners[1]);
				getForceField()->getUnassignedAtoms().insert(partners[2]);
				getForceField()->getUnassignedAtoms().insert(&central_atom);
				continue;
			}

			// we ignore OOP bends with a zero as force constant
			if (this_bend.k_oop != 0.0) bends_.push_back(this_bend);
		}

		// everything went well
		return true;
	}

	// calculates the current energy of this component
	double MMFF94OutOfPlaneBend::updateEnergy()
	{
		energy_ = 0;
		
		vector<OutOfPlaneBend>::iterator bend_it = bends_.begin();

		double radian_to_degree = 180.0 / Constants::PI;

		// the three vectors from the three partners to the center atom
		TVector3<double> vs[3];
		// the normals of the 3 plane 
		TVector3<double> ns[3];

		for (; bend_it != bends_.end(); ++bend_it) 
		{
			OutOfPlaneBend& b = *bend_it;
			const Vector3& vi = b.i->position;
			const Vector3& vj = b.j->position;
			const Vector3& vk = b.k->position;
			const Vector3& vl = b.l->position;

			vs[0].set(vj.x - vi.x, vj.y - vi.y, vj.z - vi.z);
			vs[1].set(vj.x - vk.x, vj.y - vk.y, vj.z - vk.z);
			vs[2].set(vj.x - vl.x, vj.y - vl.y, vj.z - vl.z);

			ns[0].set(vs[1] % vs[2]);
			ns[1].set(vs[0] % vs[2]);
			ns[2].set(vs[0] % vs[1]);

			double e = 0.0;
			const double k = K0 * b.k_oop;

			for (Position p = 0; p < 3; p++)
			{
				// Maybe a degenerated plane or angle?
				double length_product = vs[p].getSquareLength() * ns[p].getSquareLength();
				if (Maths::isZero(length_product)) continue;
				double intersection_angle = asin(Maths::abs(ns[p] * vs[p]) / sqrt(length_product));
				// we need the angle in degrees!
				intersection_angle *= radian_to_degree;
				e += k * intersection_angle * intersection_angle;
			}

			energy_ += e;
#ifdef BALL_MMFF94_TEST
			b.energy = e;
#endif
		}

		return energy_;
	}


	// calculates and adds its forces to the current forces of the force field
	void MMFF94OutOfPlaneBend::updateForces()
	{
		double cosphi;

		Vector3	ab;	// vector from atom2 to atom1
		Vector3 cb;	// vector from atom2 to atom3
		Vector3 dc;	// vector from atom3 to atom4

		bool use_selection = getForceField()->getUseSelection();

		for (Position t = 0; t < bends_.size(); t++)
		{
			const OutOfPlaneBend& bend = bends_[t];
			Atom& a1 = *bend.i->ptr;
			Atom& a2 = *bend.j->ptr;
			Atom& a3 = *bend.k->ptr;
			Atom& a4 = *bend.l->ptr;

			if (use_selection &&
					!a1.isSelected() &&
					!a2.isSelected() &&
					!a3.isSelected() &&
					!a4.isSelected())
			{
				continue;
			}

			ab = a1.getPosition() - a2.getPosition();
			const double length_ab = ab.getLength();
			Vector3 ba = a2.getPosition() - a1.getPosition();
			cb = a3.getPosition() - a2.getPosition();
			const double length_cb = cb.getLength();
			dc = a4.getPosition() - a3.getPosition();
			const double length_dc = dc.getLength();

			if (length_ab != 0 && length_cb != 0 && length_dc != 0) 
			{
				const Vector3  t = ba % cb;   // cross product of cb and ba
				const Vector3  u = cb % dc;   // cross product of cb and dc

				const double length_t2 = t.getSquareLength();
				const double length_u2 = u.getSquareLength();

				const double length_t = sqrt(length_t2);
				const double length_u = sqrt(length_u2);

				if (length_t != 0 && length_u != 0) 
				{
					cosphi = (t * u) / (length_t * length_u);

					if (cosphi > 1.0)
					{
						cosphi = 1.0;
					}
					if (cosphi < -1.0)
					{
						cosphi = -1.0;
					}

					double direction = (t % u) * cb;
					float factor = K0 * bend.k_oop * acos(cosphi); 
					if (direction > 0.0)
					{
						factor *= -1;
					}

					const Vector3 ca = a3.getPosition() - a1.getPosition();
					const Vector3 db = a4.getPosition() - a2.getPosition();
					const Vector3 dt =   (float)(factor / (length_t2 * cb.getLength())) * (t % cb);
					const Vector3 du = - (float)(factor / (length_u2 * cb.getLength())) * (u % cb);

					if (!use_selection)
					{
						a1.getForce() += dt % cb;
						a2.getForce() += ca % dt + du % dc;
						a3.getForce() += dt % ba + db % du;
						a4.getForce() += du % cb; 
					} 
					else 
					{
						if (a1.isSelected()) a1.getForce() += dt % cb;
						if (a2.isSelected()) a2.getForce() += ca % dt + du % dc;
						if (a3.isSelected()) a3.getForce() += dt % ba + db % du;
						if (a4.isSelected()) a4.getForce() += du % cb; 
					}
				}
			}
		}
	}

} // namespace BALL
