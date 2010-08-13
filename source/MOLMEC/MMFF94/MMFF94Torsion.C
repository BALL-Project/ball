// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MMFF94Torsion.C,v 1.1.8.2 2007/05/13 00:06:10 amoll Exp $
//

#include <BALL/MOLMEC/MMFF94/MMFF94Torsion.h>
#include <BALL/MOLMEC/MMFF94/MMFF94.h>
#include <BALL/MOLMEC/COMMON/forceFieldComponent.h>
#include <BALL/MOLMEC/COMMON/forceField.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/FORMAT/parameters.h>

#include <algorithm>

#define BALL_DEBUG_TEST

using namespace std;

namespace BALL 
{

	// Conversion from kJ / (mol A) into Newton
	const double FORCES_FACTOR = 1000 * 1E10 / Constants::AVOGADRO;

	typedef TVector3<double> DVector3;

	#define K_TORSION 0.5 * Constants::JOULE_PER_CAL

	void MMFF94Torsion::AddDV3_(Vector3& f3, const TVector3<double> d3)
	{
		f3.x += d3.x;
		f3.y += d3.y;
		f3.z += d3.z;
	}

	MMFF94Torsion::Torsion::Torsion()
		: type(-1),
			atom1(0),
			atom2(0),
			atom3(0),
			atom4(0),
			v1(-1),
			v2(-1),
			v3(-1),
			energy(0),
			angle(0),
			heuristic(0)
	{
	}

	// default constructor
	MMFF94Torsion::MMFF94Torsion()
		:	ForceFieldComponent()
	{	
		// set component name
		setName("MMFF94 Torsion");
	}


	// constructor
	MMFF94Torsion::MMFF94Torsion(ForceField& force_field)
		:	ForceFieldComponent(force_field)
	{
		// set component name
		setName( "MMFF94 Torsion" );
	}


	// copy constructor
	MMFF94Torsion::MMFF94Torsion(const MMFF94Torsion& component)
		:	ForceFieldComponent(component)
	{
	}

	// destructor
	MMFF94Torsion::~MMFF94Torsion()
	{
	}


	// setup the internal datastructures for the component
	bool MMFF94Torsion::setup()
		throw(Exception::TooManyErrors)
	{
		if (getForceField() == 0 ||
				dynamic_cast<MMFF94*>(getForceField()) == 0) 
		{
			Log.error() << "MMFF94Torsion::setup(): component not bound to force field" << endl;
			return false;
		}

		MMFF94* mmff = dynamic_cast<MMFF94*>(getForceField());

 		Options& options = getForceField()->options;
		if (options.has(MMFF94_TORSIONS_ENABLED))
		{
			if (!options.getBool(MMFF94_TORSIONS_ENABLED))
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

			if (filename == "")
			{
				throw Exception::FileNotFound(__FILE__, __LINE__, filename);
			}

			Parameters p(filename);

			const MMFF94AtomTypeEquivalences& equivalences = mmff->getEquivalences();
			parameters_.setEquivalences(equivalences);
			parameters_.readParameters(p, "Torsions");
		}

		torsions_.clear();

		const vector<MMFF94AtomType>& atom_types = mmff->getAtomTypes();

		// calculate the torsions
		vector<Atom*>::const_iterator atom_it = getForceField()->getAtoms().begin();
		Atom::BondIterator it1, it2, it3;
		Atom *atom1, *atom2, *atom3, *atom4;
		// a working instance to put the current values in and push it back
		Torsion this_torsion;

		bool use_selection = getForceField()->getUseSelection();

		// proper torsion will be added to the torsion vector
		for (; atom_it != getForceField()->getAtoms().end(); ++atom_it) 
		{
			for (it1 = (*atom_it)->beginBond(); +it1 ; ++ it1) 
			{
				if (it1->getType() == Bond::TYPE__HYDROGEN) continue; // ignore H -bonds
				if (*atom_it != it1->getFirstAtom()) continue;
			
				// central atoms
				atom2 = *atom_it;
				atom3 = const_cast<Atom*>(it1->getSecondAtom());

				for (it2 = (*atom_it)->beginBond(); +it2 ; ++it2) 
				{
					if (it2->getType() == Bond::TYPE__HYDROGEN) continue; // ignore H -bonds
					
					// identical to bond 1?
					if (&*it2 == &*it1) continue;
				
					// determine the outer atom a1
					atom1 = (*it2).getPartner(**atom_it);

					for (it3 = atom3->beginBond(); +it3 ; ++it3) 
					{
						if (it3->getType() == Bond::TYPE__HYDROGEN) continue; // ignore H -bonds

						atom4 = (*it3).getPartner(*atom3);

						if (atom4 == atom2 || atom4 == atom1) continue;

						if (use_selection && (!atom1->isSelected() ||
																	!atom2->isSelected() ||
																	!atom3->isSelected() ||
																	!atom4->isSelected()))
						{
							continue;
						}

						////////////////////////////////////////
						// ok, we have the torsion
						////////////////////////////////////////
						
						// if one of the both middle atoms is of linear type, do nothing!
						if (atom_types[atom2->getType()].lin ||
								atom_types[atom3->getType()].lin)
						{
							continue;
						}

						vector<Atom*> atoms;
						atoms.push_back(atom1);
						atoms.push_back(atom2);
						atoms.push_back(atom3);
						atoms.push_back(atom4);
						
						// do nothing for ring systems with 3 atoms
						if (mmff->areInOneRing(atoms, 3)) continue;

						// sort according to atom type:
						// a2 type must always be smaller or equal than a3 type
						// if a2 type == a3 type: a1 type must always be smaller or equal a4 type
						// if not: swap direction
						if ((atoms[1]->getType() >  atoms[2]->getType()) ||
							  (atoms[1]->getType() == atoms[2]->getType() && atoms[0]->getType() > atoms[3]->getType()))
						{
							Atom* temp;
							temp = atoms[0]; atoms[0] = atoms[3]; atoms[3] = temp; 
							temp = atoms[1]; atoms[1] = atoms[2]; atoms[2] = temp; 
						}

						this_torsion.type = getTorsionType(atoms);

						// search torsion parameters for (a1,a2,a3,a4)
						Atom::Type type_a1 = atoms[0]->getType();
						Atom::Type type_a2 = atoms[1]->getType();
						Atom::Type type_a3 = atoms[2]->getType();
						Atom::Type type_a4 = atoms[3]->getType();

#ifdef BALL_DEBUG_MMFF
						Log.info() << "MMFF94 Torsion: Searching parameters for type " << this_torsion.type << "   "
								<< atoms[0]->getFullName() << " " << atoms[1]->getFullName() << " " 
								<< atoms[2]->getFullName() << " " << atoms[3]->getFullName() << "  " 
								<< atoms[0]->getType() << " " << atoms[1]->getType() << " " 
								<< atoms[2]->getType() << " " << atoms[3]->getType() << " " << std::endl;
#endif

						bool found = parameters_.assignParameters(this_torsion.type, type_a1, type_a2, type_a3, type_a4,
																									 this_torsion.v1, this_torsion.v2, this_torsion.v3);
						

						this_torsion.heuristic = !found;

						if (!found)
						{
							if (!calculateHeuristic_(*atoms[1], *atoms[2], this_torsion.v1, this_torsion.v2, this_torsion.v3))
							{
								//
								// didnt found torsion parameters
								getForceField()->getUnassignedAtoms().insert(atoms[0]);
								getForceField()->getUnassignedAtoms().insert(atoms[1]);
								getForceField()->getUnassignedAtoms().insert(atoms[2]);
								getForceField()->getUnassignedAtoms().insert(atoms[3]);

								getForceField()->error() << "MMFF94 Torsion: Could not find parameters for type " << this_torsion.type << "   "
									<< atoms[0]->getFullName() << " " << atoms[1]->getFullName() << " " 
									<< atoms[2]->getFullName() << " " << atoms[3]->getFullName() << "  " 
									<< atoms[0]->getType() << " " << atoms[1]->getType() << " " 
									<< atoms[2]->getType() << " " << atoms[3]->getType() << " " << std::endl;

								continue;
							}
						}

						// do nothing if all three constants are zeros:
						if (this_torsion.v1 == 0.0 &&
								this_torsion.v2 == 0.0 &&
								this_torsion.v3 == 0.0)
						{
							continue;
						}

						// finaly: we really have a valid torsion: store it
						this_torsion.atom1 = atoms[0];
						this_torsion.atom2 = atoms[1];
						this_torsion.atom3 = atoms[2];
						this_torsion.atom4 = atoms[3];

						torsions_.push_back(this_torsion);

					} // it3
				} // it2
			} // it1
		} // atom_it

		return true;
	}

	// calculates the current energy of this component
	double MMFF94Torsion::updateEnergy() 
	{
		double cosphi;

		Vector3	a21;
		Vector3 a23;
		Vector3 a34;
		Vector3 cross2321;
		Vector3 cross2334;

		energy_ = 0;

		vector<Torsion>::const_iterator it = torsions_.begin(); 

		bool use_selection = getForceField()->getUseSelection();

		for (; it != torsions_.end(); it++) 
		{
			if (!use_selection ||
					(it->atom1->isSelected() || 
					 it->atom2->isSelected() || 
					 it->atom3->isSelected() || 
					 it->atom4->isSelected()))
			{
				a21 = it->atom1->getPosition() - it->atom2->getPosition();
				a23 = it->atom3->getPosition() - it->atom2->getPosition();
				a34 = it->atom4->getPosition() - it->atom3->getPosition();

				cross2321 = a23 % a21;
				cross2334 = a23 % a34;

				double length_cross2321 = cross2321.getLength();
				double length_cross2334 = cross2334.getLength();

				if (length_cross2321 != 0 && length_cross2334 != 0) 
				{
					cross2321 /= length_cross2321;
					cross2334 /= length_cross2334;

					cosphi = cross2321 * cross2334;

					if (cosphi > 1.0)
					{
						cosphi = 1.0;
					}
					if (cosphi < -1.0)
					{
						cosphi = -1.0;
					}

					const double phi = acos(cosphi);

					const double e = K_TORSION * (it->v1 * (1.0 + cosphi) +
																				it->v2 * (1.0 - cos(phi * 2.0)) +
																				it->v3 * (1.0 + cos(phi * 3.0)));

					energy_ += e;

#ifdef BALL_DEBUG_TEST
					(*(Torsion*)&*it).energy = e;
					(*(Torsion*)&*it).angle = phi * (double)180.0 / Constants::PI;
#endif
				}
			}
		}

		return energy_;
	}

	// calculates and adds its forces to the current forces of the force field
	void MMFF94Torsion::updateForces()
	{
		// Bond vectors of the three atoms
		DVector3 ij, jk, kl;

		// length of the three bonds
		double l_ij, l_jk, l_kl;

		// angle between ijk and jkl:
		double angle_ijk, angle_jkl;

		bool us = getForceField()->getUseSelection();

		for (Position t = 0; t < torsions_.size(); t++)
		{
			const Torsion& torsion = torsions_[t];
			Atom& a1 = *torsion.atom1;
			Atom& a2 = *torsion.atom2;
			Atom& a3 = *torsion.atom3;
			Atom& a4 = *torsion.atom4;

			if (us && !a1.isSelected() &&
								!a2.isSelected() &&
								!a3.isSelected() &&
								!a4.isSelected())
			{
				continue;
			}

			ij.set(a2.getPosition().x - a1.getPosition().x,
						 a2.getPosition().y - a1.getPosition().y,
						 a2.getPosition().z - a1.getPosition().z);
			jk.set(a3.getPosition().x - a2.getPosition().x,
						 a3.getPosition().y - a2.getPosition().y,
						 a3.getPosition().z - a2.getPosition().z);
			kl.set(a4.getPosition().x - a3.getPosition().x,
						 a4.getPosition().y - a3.getPosition().y,
						 a4.getPosition().z - a3.getPosition().z);

			l_ij = ij.getLength();
			l_jk = jk.getLength();
			l_kl = kl.getLength();

			if (Maths::isZero(l_ij) || 
					Maths::isZero(l_jk) || 
					Maths::isZero(l_kl) )
			{
				continue;
			}

			angle_ijk = ij.getAngle(jk);
			angle_jkl = jk.getAngle(kl);

			// normalize the bond vectors:
			ij /= l_ij;
			jk /= l_jk;
			kl /= l_kl;

			double sin_j = sin(angle_ijk);
			double sin_k = sin(angle_jkl);

			double rsj = l_ij * sin_j;
			double rsk = l_kl * sin_k;

			double rs2j = 1. / (rsj * sin_j);
			double rs2k = 1. / (rsk * sin_k);

			double rrj = l_ij / l_jk;
			double rrk = l_kl / l_jk;

			double rrcj = rrj * (-cos(angle_ijk));
			double rrck = rrk * (-cos(angle_jkl));

			DVector3 a = ij % jk;
			DVector3 b = jk % kl;
			DVector3 c = a % b;
			double d1 = c * jk;
			double d2 = a * b;
			double angle = atan2(d1, d2);

			DVector3 di = -a * rs2j;
			DVector3 dl = b * rs2k;
			DVector3 dj = di * (rrcj - 1.) - dl * rrck;
			DVector3 dk = -(di + dj + dl);

			double c1 =     torsion.v1 * sin(angle) -
				          2. * torsion.v2 * sin(angle * 2.) +
									3. * torsion.v3 * sin(angle * 3.);
			c1 *= 0.5 * FORCES_FACTOR * Constants::JOULE_PER_CAL;

			if (!us || a1.isSelected()) AddDV3_(a1.getForce(), di * c1);
			if (!us || a2.isSelected()) AddDV3_(a2.getForce(), dj * c1);
			if (!us || a3.isSelected()) AddDV3_(a3.getForce(), dk * c1);
			if (!us || a4.isSelected()) AddDV3_(a4.getForce(), dl * c1);
		}
	}


	/* from Paper IV:
			- This index normally takes the value "0," 
			- "1" when the J-K bond has a bond type index of 1; 
			- "2" when BTJK is "0" but BTIJ and/or BTKL is "1"
			- "4" when i, j, k, and I are all members of the same four-membered ring
			- "5" when the four atoms are members of a five-membered ring
				and at least one is a sp3-hybridized carbon (MMFF atom type 1).
	*/
	Position MMFF94Torsion::getTorsionType(const vector<Atom*>& atoms) const
	{
		MMFF94* mmff = dynamic_cast<MMFF94*>(getForceField());

		// in a ring of 4?
		if (mmff->areInOneRing(atoms, 4))
		{
			// make sure we dont also have 2 rings of 3!
			if (atoms[0]->getBond(*atoms[2]) == 0 &&
					atoms[1]->getBond(*atoms[3]) == 0)
			{
				return 4;
			}
		}

		const Bond* bond1 = atoms[0]->getBond(*atoms[1]);
		const Bond* bond2 = atoms[1]->getBond(*atoms[2]);
		const Bond* bond3 = atoms[2]->getBond(*atoms[3]);

#ifdef BALL_DEBUG_MMFF
Log.error() << "# " << atoms[0]->getName() << " " 
										<< atoms[1]->getName() << " "
										<< atoms[2]->getName() << " "
										<< atoms[3]->getName() << " "   	
										<< bond1->hasProperty("MMFF94SBMB") << " "
										<< bond2->hasProperty("MMFF94SBMB") << " "
										<< bond3->hasProperty("MMFF94SBMB") << " # "
										<< bond1->getOrder() << " " 
										<< bond2->getOrder() << " " 
										<< bond3->getOrder() << " " 
										<< std::endl;
#endif

		vector<Atom*> as;
		as.push_back(atoms[1]);
		as.push_back(atoms[2]);

		if (bond2->getOrder() == Bond::ORDER__SINGLE &&
				!mmff->areInOneAromaticRing(as))
		{
			if (bond2->hasProperty("MMFF94SBMB")) return 1;

			if (bond1->hasProperty("MMFF94SBMB") ||
			    bond3->hasProperty("MMFF94SBMB"))
			{
				return 2;
			}
		}


		// in one nonaromatic ring 
		if (mmff->areInOneRing(atoms, 5))
		{
			// at least one atom has type 1?
			for (Position as = 0; as < atoms.size(); as++)
			{
				if (atoms[as]->getType() == 1) return 5;
			}
		}

		// default value: 0
		return 0;
	}


	double MMFF94Torsion::getU_(Position e)
	{
		// values taken from paper IV
		if (e > 5  && e <  9) return 2.0;   // C N O
		if (e > 13 && e < 17) return 1.25;  // Si P S
		
		// values taken from the CHARMM implementation:
		if (e > 31 && e < 35) return 0.7; 
		if (e > 49 && e < 53) return 0.2; 
		if (e < 86) return 0.1; 
		return -1;
	}

	double MMFF94Torsion::getV_(Position e)
	{
		// values taken from paper IV
		if (e == 6) return 2.12;   // C
		if (e == 7) return 1.50;   // N
		if (e == 8) return 0.20;   // O
		if (e == 14) return 1.22;  // Si
		if (e == 15) return 2.40;  // P
		if (e == 16) return 0.49;  // S

		// values taken from the CHARMM implementation:
		if (e == 32) return 0.7;  // GE
		if (e == 33) return 1.5;  // As
		if (e == 34) return 0.34; // Se
		if (e == 50) return 0.2;  // Sn
		if (e == 51) return 1.1;  // Sb
		if (e == 52) return 0.3;  // Te
		if (e  < 86) return 0.3; 

		return -1;
	}

	bool MMFF94Torsion::calculateHeuristic_(const Atom& aj, const Atom& ak, double& v1, double& v2, double& v3)
	{
		MMFF94* mmff = dynamic_cast<MMFF94*>(getForceField());
		const vector<MMFF94AtomType>& atom_types = mmff->getAtomTypes();

		v1 = v2 = v3 = 0.0;

		const Index tj = aj.getType();
		const Index tk = ak.getType();

		if (tj == -1 || tk == -1) return false;
		
		const MMFF94AtomType& atj = atom_types[tj];
		const MMFF94AtomType& atk = atom_types[tk];

		////////////////////////////////////////////
		// rule a) linear?
		////////////////////////////////////////////
		if (atj.lin || atk.lin) return true;

		const Position ej = aj.getElement().getAtomicNumber();
		const Position ek = ak.getElement().getAtomicNumber();
		const double uj = getU_(ej);
		const double uk = getU_(ek);
		const double vj = getV_(ej);
		const double vk = getV_(ek);
		// n in equation 22:
		const double n = (atj.crd - 1) * (atk.crd - 1);
		// beta in equation 21:
		double beta = 6;

		////////////////////////////////////////////
		// rule b) both atom types are aromatic
		////////////////////////////////////////////
		vector<Atom*> av;
		av.push_back((Atom*) &aj);
		av.push_back((Atom*) &ak);

		// both atoms must be in one aromatic ring
		if (mmff->areInOneAromaticRing(av))
		{
			if (uj == -1 || uk == -1) return false;

			// one trivalent and one tetravalent?
			if (atj.val * atk.val == 12) beta = 3;

			double l = 0.5;

			// one atom can contribute a pi lone pair
			if (atj.pilp || atk.pilp) l = 0.3;

			// equation 21
			v2 = beta * l * sqrt(uj * uk);
			return true;
		}

		////////////////////////////////////////////
		// rule c) double bond
		////////////////////////////////////////////
		const Bond& bond = *aj.getBond(ak);
		Position bond_order = bond.getOrder();
			
		if (bond_order == Bond::ORDER__DOUBLE)
		{
			if (uj == -1 || uk == -1) return false;

			double l = 0.4;

			if (atj.mltb * atk.mltb == 4) l = 1;

			// equation 21
			v2 = beta * l * sqrt(uj * uk);
			return true;
		}
	
		////////////////////////////////////////////
		// rule d) both atoms must be tetrcoordinate
		////////////////////////////////////////////
		if (atj.crd * atk.crd == 16)
		{
			if (vj == -1 || vk == -1) return false;
			// equation 22
			v3 = sqrt(vj * vk) / n;
			return true;
		}

		////////////////////////////////////////////
		// rule e) j tetravalent
		////////////////////////////////////////////
		if (atj.crd == 4)
		{
			if (atk.crd == 3  &&  (atk.val == 4 || atk.val == 34 || atk.mltb))
			{
				return true; // zero values
			}

			if (atk.crd == 2  &&  (atk.val == 3 || atk.mltb))
			{
				return true; // zero values
			}

			if (vj == -1 || vk == -1) return false;
			// equation 22
			v3 = sqrt(vj * vk) / n;
			return true;
		}

		////////////////////////////////////////////
		// rule f) k tetravalent
		////////////////////////////////////////////
		if (atk.crd == 4)
		{
			if (atj.crd == 3  &&  (atj.val == 4 || atj.val == 34 || atj.mltb))
			{
				return true; // zero values
			}

			if (atj.crd == 2  &&  (atj.val == 3 || atj.mltb))
			{
				return true; // zero values
			}

			if (vj == -1 || vk == -1) return false;
			// equation 22
			v3 = sqrt(vj * vk) / n;
			return true;
		}

		////////////////////////////////////////////
		// rule g) k trivalent
		////////////////////////////////////////////
		if ((bond_order == Bond::ORDER__SINGLE && atj.mltb && atk.mltb) ||
				(atj.pilp && atk.mltb) ||
				(atk.pilp && atj.mltb))
		{
			if (atj.pilp && atk.pilp)
			{
				return true; // zero values
			}

			if (uk == -1 || uj == -1) return false;

			double l = 0.15;

			if (atj.pilp && atk.mltb)
			{
				if (atj.mltb == 1) 
				{
					l = 0.5;
				}
				else
				{
					if (aj.getElement().getPeriod() == 2 &&
							ak.getElement().getPeriod() == 2)
					{
						l = 0.3;
					}
				}
			}
			else if (atk.pilp && atj.mltb)
			{
				if (atk.mltb == 1) 
				{
					l = 0.5;
				}
				else
				{
					if (aj.getElement().getPeriod() == 2 &&
							ak.getElement().getPeriod() == 2)
					{
						l = 0.3;
					}
				}
			}
			// strongly delocalized "single" bond between non-carbons:
			else if ((atj.mltb == 1 || atk.mltb == 1) && (ej != 6  || ek != 6))
			{
				l = 0.4;
			}
			else 
			{
				// default l value
			}

			// equation 21
			v2 = beta * l * sqrt(uj * uk);
			return true;
		}

		///////////////////////////////////////////////
		// rule h) saturated centers, at most trivalent
		///////////////////////////////////////////////
		// both atoms are either oxygen or sulfur?
		if ((ej == 8 || ej == 16) && (ek == 8 || ek == 16))
		{
		 	const Size es = ej + ek;

			v2 = -2;										// both oxygen
			if      (es == 24) v2 = -4; // one oxygen, one sulfur
			else if (es == 32) v2 = -8; // both sulfur

			return true;
		}

		// equation 22
		if (vk == -1 || vj == -1) return false;

		v3 = sqrt(vj * vk) / n;
		return true;
	}

} // namespace BALL
