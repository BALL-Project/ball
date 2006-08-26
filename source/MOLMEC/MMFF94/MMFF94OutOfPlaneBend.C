// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MMFF94OutOfPlaneBend.C,v 1.1.4.8 2006/08/26 21:57:00 amoll Exp $
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
	// Conversion from kJ / (mol A) into Newton
	const double FORCES_FACTOR = 1000 * 1E10 / Constants::AVOGADRO;

	/// 0.043844 / 2
	#define K0 0.021922

	const double RADIAN_TO_DEGREE = 180.0 / Constants::PI;

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

 		Options& options = getForceField()->options;
		if (options.has(MMFF94_OUTOFPLANE_ENABLED) && !options.getBool(MMFF94_OUTOFPLANE_ENABLED))
		{
			setEnabled(false);
			return true;
		}

		setEnabled(true);


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

				partners.push_back(bond.getPartner(central_atom));
			}

			if (use_selection && (!central_atom.isSelected() ||
														!partners[0]->isSelected() ||
														!partners[1]->isSelected() ||
														!partners[2]->isSelected()))
			{
				continue;
			}


			this_bend.i = &Atom::getAttributes()[partners[0]->getIndex()];
			this_bend.j = &Atom::getAttributes()[central_atom.getIndex()];
			this_bend.k = &Atom::getAttributes()[partners[1]->getIndex()];
			this_bend.l = &Atom::getAttributes()[partners[2]->getIndex()];

			const Position type_j = central_atom.getType();
			const Index tp0 = partners[0]->getType();
			const Index tp1 = partners[1]->getType();
			const Index tp2 = partners[2]->getType();

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

		// the three vectors from the three partners to the center atom
		TVector3<double> vs[3];

		// the normals of the 3 planes
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
				intersection_angle *= RADIAN_TO_DEGREE;
				e += k * intersection_angle * intersection_angle;
			}

			e *= Constants::JOULE_PER_CAL;

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
		const double FC = K0 * 2. * RADIAN_TO_DEGREE * RADIAN_TO_DEGREE;
		bool us = getForceField()->getUseSelection();

		//////////////////////////////////////////////////////////////////
		// ids of the non-central atoms for the three runs per out of plane bend:
		vector<vector<Position> > atom_ids;
		// Atom i:
		vector<Position> temp;
		temp.push_back(0);
		temp.push_back(0);
		temp.push_back(1);
		atom_ids.push_back(temp);

		// Atom k:
		temp.clear();
		temp.push_back(1);
		temp.push_back(2);
		temp.push_back(2);
		atom_ids.push_back(temp);

		// Atom l:
		temp.clear();
		temp.push_back(2);
		temp.push_back(1);
		temp.push_back(0);
		atom_ids.push_back(temp);

		// temp variables:
		double length;
		Vector3 delta;

		vector<Atom*> partners(3);
		// normalized vectors from central atom to outer atoms:
		vector<Vector3> nbv(3);
		// lenght of the vectors from central atom to outer atoms:
		vector<double> lengths(3);
		
		// positions of individual atoms in partners and nbv:
		Position pi, pk, pl;

		// normal vectors of the three planes:
		Vector3 an, bn, cn;

		for (Position t = 0; t < bends_.size(); t++)
		{
			const OutOfPlaneBend& bend = bends_[t];
			Atom& ta1 = *bend.i->ptr;
			Atom& ta2 = *bend.j->ptr;
			Atom& ta3 = *bend.k->ptr;
			Atom& ta4 = *bend.l->ptr;

			if (us && !ta1.isSelected() &&
								!ta2.isSelected() &&
								!ta3.isSelected() &&
								!ta4.isSelected())
			{
				continue;
			}

			partners[0] = &ta1;
			partners[1] = &ta3;
			partners[2] = &ta4;
			Atom& center_atom = ta2;

			// abort for this bend if two atoms have the same position:
			bool error = false;

			// calculate normalized vectors from central atom to outer atoms:
			for (Position p = 0; p < 3; p++)
			{
				delta = partners[p]->getPosition() - center_atom.getPosition();
				length = delta.getLength();
				if (Maths::isZero(length))
				{
					error = true;
					break;
				}

				delta /= length;
				nbv[p] = delta;
				lengths[p] = length;
			}

			if (error) continue;

			// three runs per OOP:
			for (Position run = 0; run < 3; run++)
			{
				// position of the individual atoms in partners[] and nbv[]
				pi = atom_ids[0][run];
				pk = atom_ids[1][run];
				pl = atom_ids[2][run];

				Atom& i = *partners[pi];
				Atom& k = *partners[pk];
				Atom& l = *partners[pl];

				// normalized vectors from central atom to outer atoms:
				const Vector3& ji = nbv[pi];
				const Vector3& jk = nbv[pk];
				const Vector3& jl = nbv[pl];

				const double& length_ji = lengths[pi];
				const double& length_jk = lengths[pk];
				const double& length_jl = lengths[pl];

				// the three normal vectors of the planes:
				an = ji % jk;
				bn = jk % jl;
				cn = jl % ji;

				// Angle ji to jk
				const double theta = ji.getAngle(jk);
				const double sin_theta = sin(theta);
				const double cos_theta = cos(theta);
				
				const double sin_dl = an * jl / sin_theta;

				const double dl = asin(sin_dl);
				const double cos_dl = cos(dl);
				const double tan_dl = sin_dl / cos_dl;

				const double cdst = 1. / (cos_dl * sin_theta);

				const double tdst = tan_dl / (sin_theta * sin_theta);

				double c1 = dl * FC * bend.k_oop;
c1 *= FORCES_FACTOR;

				const Vector3 d_l = (an * cdst - jl * tan_dl) / length_jl * c1;
				const Vector3 d_i = (bn * cdst + (-ji + jk * cos_theta) * tdst) / length_ji * c1;
				const Vector3 d_k = (cn * cdst + (-jk + ji * cos_theta) * tdst) / length_jk * c1;

				if (!us || i.isSelected()) i.getForce() += d_i;
				if (!us || k.isSelected()) k.getForce() += d_k;
				if (!us || l.isSelected()) l.getForce() += d_l;
				if (!us || center_atom.isSelected()) center_atom.getForce() -= d_i + d_k + d_l;
			}
		}
	}

} // namespace BALL
