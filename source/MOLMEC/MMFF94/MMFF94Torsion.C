// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MMFF94Torsion.C,v 1.1.2.16 2006/02/15 17:24:00 amoll Exp $
//

#include <BALL/MOLMEC/MMFF94/MMFF94Torsion.h>
#include <BALL/MOLMEC/MMFF94/MMFF94.h>
#include <BALL/MOLMEC/COMMON/forceFieldComponent.h>
#include <BALL/MOLMEC/COMMON/forceField.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/SYSTEM/path.h>

#include <algorithm>

#define BALL_DEBUG_TEST

using namespace std;

namespace BALL 
{

	MMFF94Torsion::Torsion::Torsion()
		: type(-1),
			angle(0),
			atom1(0),
			atom2(0),
			atom3(0),
			atom4(0),
			energy(0),
			v1(-1),
			v2(-1),
			v3(-1)
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

		if (!parameters_.isInitialized())
		{
			Path    path;
			String  filename(path.find("MMFF94/MMFFTOR.PAR"));

			if (filename == "") 
			{
				throw Exception::FileNotFound(__FILE__, __LINE__, filename);
			}

			parameters_.readParameters(filename);
		}

		torsions_.clear();

		MMFF94* mmff = dynamic_cast<MMFF94*>(getForceField());
		const vector<MMFF94AtomType>& atom_types = mmff->getAtomTypes();
		const MMFF94AtomTypeEquivalences& equivalences = mmff->getEquivalences();

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

						// check for parameters in a step down procedure
						Position ic[] = { 0, 1, 2, 4, 4}; // equivalence values for atom type i
						Position lc[] = { 0, 1, 4, 2, 4}; // equivalence values for atom type l

						bool found = false;
						for (Position p = 0; p < 5 && !found; p++)
						{
							found = parameters_.getParameters(this_torsion.type, 
																		equivalences.getEquivalence(type_a1, ic[p]),
																		type_a2, type_a3,
																		equivalences.getEquivalence(type_a4, lc[p]),
																		this_torsion.v1, this_torsion.v2, this_torsion.v3);
						
							if (found) break;
						}

						if (!found)
						{
							// ???? heuristics
//   							const MMFF94AtomType& atj = atom_types[aj.getType()];
//   							const MMFF94AtomType& atk = atom_types[aj.getType()];

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

						// do nothing if all three constants are zeros:
						if (this_torsion.v1 == 0.0 &&
								this_torsion.v2 == 0.0 &&
								this_torsion.v3 == 0.0)
						{
							continue;
						}

						// finaly: we really have a valid torsion: store it
						this_torsion.atom1 = &Atom::getAttributes()[atoms[0]->getIndex()];
						this_torsion.atom2 = &Atom::getAttributes()[atoms[1]->getIndex()];
						this_torsion.atom3 = &Atom::getAttributes()[atoms[2]->getIndex()];
						this_torsion.atom4 = &Atom::getAttributes()[atoms[3]->getIndex()];

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
					(it->atom1->ptr->isSelected() || 
					 it->atom2->ptr->isSelected() || 
					 it->atom3->ptr->isSelected() || 
					 it->atom4->ptr->isSelected()))
			{
				a21 = it->atom1->position - it->atom2->position;
				a23 = it->atom3->position - it->atom2->position;
				a34 = it->atom4->position - it->atom3->position;

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

					const double e = 0.5 * (it->v1 * (1.0 + cosphi) +
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
//   		bool use_selection = getForceField()->getUseSelection();
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
		if (e > 5  && e <  9) return 2.0;   // C N O
		if (e > 13 && e < 17) return 1.25;  // Si P S
		return -1;
	}

	double MMFF94Torsion::getV_(Position e)
	{
		if (e == 6) return 2.12;   // C
		if (e == 7) return 1.50;   // N
		if (e == 8) return 0.20;   // O
		if (e == 14) return 1.22;  // Si
		if (e == 15) return 2.40;  // P
		if (e == 16) return 0.49;  // S
		return -1;
	}

	bool MMFF94Torsion::calculateHeuristicB_(const Atom& aj, const Atom& ak, 
																					 const MMFF94AtomType& atj, const MMFF94AtomType& atk, double& result)
	{
		// both atom types must be aromatic
		if (!atj.arom || ! atk.arom) return false;

		vector<Atom*> av;
		av.push_back((Atom*) &aj);
		av.push_back((Atom*) &ak);

		// both atoms must be in one aromatic ring
		MMFF94* mmff = dynamic_cast<MMFF94*>(getForceField());
		if (!mmff->areInOneAromaticRing(av)) return false;

		const Position ej = aj.getElement().getAtomicNumber();
		const Position ek = ak.getElement().getAtomicNumber();
		const double uj = getU_(ej);
		const double uk = getU_(ek);
		if (uj == -1 || uk == -1) return false;

		double beta = 6;

		// one trivalent and one tetravalent?
		if (atj.val * atk.val == 12) beta = 3;

		double l = 0.5;

		// one atom can contribute a pi lone pair
		if (atj.pilp || atk.pilp) l = 0.3;

		// equation 21
		result = beta * l * pow(uj * uk, -0.5);
		return true;
	}

	bool MMFF94Torsion::calculateHeuristicC_(const Atom& aj, const Atom& ak,
																					 const MMFF94AtomType& atj, const MMFF94AtomType& atk, double& result)
	{
		const Bond& bond = *aj.getBond(ak);
		if (!bond.getOrder() == Bond::ORDER__DOUBLE) return false;

		const Position ej = aj.getElement().getAtomicNumber();
		const Position ek = ak.getElement().getAtomicNumber();
		const double uj = getU_(ej);
		const double uk = getU_(ek);
		if (uj == -1 || uk == -1) return false;

		const double beta = 6.0;

		double l = 1;

		if (atj.mltb != 2 || atk.mltb != 2) l = 0.4;

		// equation 21
		result = beta * l * pow(uj * uk, -0.5);
		return true;
	}

	bool MMFF94Torsion::calculateHeuristicD_(Position anj, Position ank,
																					 const MMFF94AtomType& atj, const MMFF94AtomType& atk, double& result, bool must_be_tetra)
	{
		// both atoms must be tetrcoordinate?
		if (must_be_tetra && atj.crd * atk.crd != 16) return false;
		const double vj = getV_(anj);
		const double vk = getV_(ank);
		if (vj == -1 || vk == -1) return false;

		// equation 22
		// (crd(j) - 1) * (crd(k) - 1) = 9
		result = pow(vj * vk, -0.5) / 9.0;

		return true;
	}

	bool MMFF94Torsion::calculateHeuristicE_(Position anj, Position ank,
																					 const MMFF94AtomType& atj, const MMFF94AtomType& atk, double& result, bool& zero)
	{
		if (atj.crd != 4) return false;

		if (atk.crd == 3 && 
				(atk.val == 4 || atk.val == 34 | atk.mltb))
		{
			zero = true;
			return true;
		}

		if (atk.crd == 2 &&
				(atk.val == 3 || atk.mltb))
		{
			zero = true;
			return true;
		}

		return calculateHeuristicD_(anj, ank, atj, atk, result, false);
	}


	bool MMFF94Torsion::calculateHeuristicF_(Position anj, Position ank,
																					 const MMFF94AtomType& atj, const MMFF94AtomType& atk, double& result, bool& zero)
	{
		if (atk.crd != 4) return false;

		if (atj.crd == 3 && 
				(atj.val == 4 || atj.val == 34 | atj.mltb))
		{
			zero = true;
			return true;
		}

		if (atj.crd == 2 &&
				(atj.val == 3 || atj.mltb))
		{
			zero = true;
			return true;
		}

		return calculateHeuristicD_(anj, ank, atj, atk, result, false);
	}


} // namespace BALL
