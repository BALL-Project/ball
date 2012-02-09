// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MMFF94OutOfPlaneBend.C,v 1.1.8.3 2007/05/13 00:06:08 amoll Exp $
//

#include <BALL/MOLMEC/MMFF94/MMFF94OutOfPlaneBend.h>
#include <BALL/MOLMEC/MMFF94/MMFF94.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/SYSTEM/path.h>

#include <cmath>

//     #define BALL_DEBUG_MMFF
//   #define BALL_MMFF94_TEST

using namespace std;

namespace BALL 
{
	// Conversion from kJ / (mol A) into Newton
	const double FORCES_FACTOR = 1000 * 1E10 / Constants::AVOGADRO;

	/// 0.043844 / 2
	#define K0 0.021922

	const double RADIAN_TO_DEGREE = 180.0 / Constants::PI;

	void MMFF94OutOfPlaneBend::AddDV3_(Vector3& f3, const TVector3<double> d3)
	{
		f3.x += d3.x;
		f3.y += d3.y;
		f3.z += d3.z;
	}

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
		if (options.has(MMFF94_OUTOFPLANE_ENABLED))
		{
			if (!options.getBool(MMFF94_OUTOFPLANE_ENABLED))
			{
				setEnabled(false);
				return true;
			}
			else
			{
				setEnabled(true);
			}
		}


		if (!parameters_.isInitialized())
		{
			Path    path;
			String  filename(path.find("MMFF94/mmff94.ini"));

			if (filename == "") throw Exception::FileNotFound(__FILE__, __LINE__, "[empty]");

			const MMFF94AtomTypeEquivalences& equiv = mmff->getEquivalences();
			parameters_.setEquivalences(equiv);
			Parameters p(filename);
			parameters_.readParameters(p, "OutOfPlane");
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


			this_bend.i = partners[0];
			this_bend.j = &central_atom;
			this_bend.k = partners[1];
			this_bend.l = partners[2];

			const Position type_j = central_atom.getType();
			const Index tp0 = partners[0]->getType();
			const Index tp1 = partners[1]->getType();
			const Index tp2 = partners[2]->getType();

			// check for parameters in a step down procedure
			bool found = parameters_.assignParameters(tp0, type_j, tp1, tp2, this_bend.k_oop);

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
		
		bool use_selection = getForceField()->getUseSelection();
		vector<OutOfPlaneBend>::iterator bend_it = bends_.begin();

		// the three vectors from the three partners to the center atom
		TVector3<double> vs[3];

		// the normals of the 3 planes
		TVector3<double> ns[3];

		for (; bend_it != bends_.end(); ++bend_it) 
		{
			OutOfPlaneBend& b = *bend_it;
			if (use_selection && (!b.i->isSelected() &&
			                      !b.j->isSelected() &&
			                      !b.k->isSelected() &&
			                      !b.l->isSelected()))
			{
				continue;
			}

			const Vector3& vi = b.i->getPosition();
			const Vector3& vj = b.j->getPosition();
			const Vector3& vk = b.k->getPosition();
			const Vector3& vl = b.l->getPosition();

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

		////////////////////////////////////////////////////////////////////////
		// all calculations below have to be performed at double precision
		// otherwise the results are far off, especially for small wilson angles
		//
		// temp variables:
		double length;
		TVector3<double> delta;

		// the three atoms bound to the central atom (for the actual plane bend)
		vector<Atom*> partners(3);
		// lenght of the vectors from the central atom to outer atoms:
		vector<double> lengths(3);
		// normalized bond vectors from central atom to outer atoms:
		vector<TVector3<double> > nbv(3);
		
		// index of the individual atoms in partners and nbv:
		Position pi, pk, pl;

		// normal vectors of the three planes:
		TVector3<double> an, bn, cn;

		for (Position t = 0; t < bends_.size(); t++)
		{
			// the current bend
			const OutOfPlaneBend& bend = bends_[t];
			Atom& ta1 = *bend.i;
			Atom& ta2 = *bend.j;
			Atom& ta3 = *bend.k;
			Atom& ta4 = *bend.l;

			// if using selection and no atom is selected: ignore this bend:
			if (us && !ta1.isSelected() &&
								!ta2.isSelected() &&
								!ta3.isSelected() &&
								!ta4.isSelected())
			{
				continue;
			}

			// non central atoms for this bend:
			partners[0] = &ta1;
			partners[1] = &ta3;
			partners[2] = &ta4;
			Atom& center_atom = ta2;

			// abort for this bend if two atoms have the same position:
			bool error = false;

			// calculate normalized bond vectors from central atom to outer atoms:
			for (Position p = 0; p < 3; p++)
			{
				// transformation from single to double precision:
				delta.x = partners[p]->getPosition().x - center_atom.getPosition().x;
				delta.y = partners[p]->getPosition().y - center_atom.getPosition().y;
				delta.z = partners[p]->getPosition().z - center_atom.getPosition().z;
				length = delta.getLength();
				if (Maths::isZero(length))
				{
					error = true;
					break;
				}

				// normalize the bond vector:
				delta /= length;
				// store the normalized bond vector:
				nbv[p] = delta;
				// store length of this bond:
				lengths[p] = length;
			}

			// abort if any bond lenght equals zero
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
				const TVector3<double>& ji = nbv[pi];
				const TVector3<double>& jk = nbv[pk];
				const TVector3<double>& jl = nbv[pl];

				const double& length_ji = lengths[pi];
				const double& length_jk = lengths[pk];
				const double& length_jl = lengths[pl];

				// the normal vectors of the three planes:
				an = ji % jk;
				bn = jk % jl;
				cn = jl % ji;

				// Bond angle ji to jk
				const double cos_theta = ji * jk;
				const double theta = acos(cos_theta);
				// If theta equals 180 degree or 0 degree
				if (Maths::isZero(theta) ||
						Maths::isZero(fabs(theta - Constants::PI)))
				{
					continue;
				}
				
				const double sin_theta = sin(theta);
				const double sin_dl = an * jl / sin_theta;

				// the wilson angle:
				const double dl = asin(sin_dl);

				// In case: wilson angle equals 0 or 180 degree: do nothing
				if (Maths::isZero(dl) ||
						Maths::isZero(fabs(dl - Constants::PI)))
				{
					continue;
				}
				

				const double cos_dl = cos(dl);

				// if wilson angle equal 90 degree: abort
				if (cos_dl < 0.0001)
				{
					continue;
				}

				// scaling factor for all forces:
				// wilson K0 * this_bend_constant * wilson_angle * DEGREE_TO_RADIAN * DEGREE_TO_RADIAN
				double c1 = -dl * FC * bend.k_oop * FORCES_FACTOR * Constants::JOULE_PER_CAL;

				double tmp = cos_dl / c1;

			/*	
			Log.precision(30);
      Log.error() << "bond   " << theta            << std::endl;
      Log.error() << "wilson " << dl            << std::endl;
			Log.error() << "tan_dl " << tan_dl            << std::endl;
      Log.error() << "cdst   " << cdst            << std::endl;
      Log.error() << "tdst   "  << tdst           << std::endl;
      Log.error() << "c1     " << c1            << std::endl;
      Log.error() << "abc "   << an << bn << cn << std::endl << std::endl << std::endl << std::endl;
			*/

				const TVector3<double> d_l = ((an / sin_theta - jl * sin_dl) / length_jl) / tmp;
				const TVector3<double> d_i = (((bn + (((-ji + jk * cos_theta) * sin_dl) / sin_theta)) / length_ji) / tmp) / sin_theta;
				const TVector3<double> d_k = (((cn + (((-jk + ji * cos_theta) * sin_dl) / sin_theta)) / length_jk) / tmp) / sin_theta;

 				if (!us || i.isSelected()) AddDV3_(i.getForce(), d_i);
 				if (!us || k.isSelected()) AddDV3_(k.getForce(), d_k);
 				if (!us || l.isSelected()) AddDV3_(l.getForce(), d_l);
 				if (!us || center_atom.isSelected()) AddDV3_(center_atom.getForce(), -(d_i + d_k + d_l));

  #ifdef BALL_MMFF94_TEST
			 getForceField()->error() << std::endl
																<< i.getName() << " " << d_i << std::endl
																<< center_atom.getName() << " " << -(d_i  +d_k + d_l) << std::endl
																<< k.getName() << " " << d_k << std::endl
																<< l.getName() << " " << d_l << std::endl;
   #endif
			}
		}
	}

} // namespace BALL
