// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MMFF94Torsion.C,v 1.1.2.5 2006/02/12 15:50:28 amoll Exp $
//

#include <BALL/MOLMEC/MMFF94/MMFF94Torsion.h>
#include <BALL/MOLMEC/MMFF94/MMFF94.h>
#include <BALL/MOLMEC/COMMON/forceFieldComponent.h>
#include <BALL/MOLMEC/COMMON/forceField.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/SYSTEM/path.h>

#include <algorithm>

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

		// a working instance to put the current values in and push it back
		Torsion this_torsion;

		MMFF94* mmff = dynamic_cast<MMFF94*>(getForceField());
		const vector<MMFF94AtomTypeData>& atom_types = mmff->getAtomTypes();
		const MMFF94AtomTypeEquivalences& equivalences = mmff->getEquivalences();

		Atom*	a1;
		Atom*	a2;
		Atom*	a3;
		Atom*	a4;

		// calculate the torsions
		vector<Atom*>::const_iterator atom_it = getForceField()->getAtoms().begin();
		Atom::BondIterator it1;
		Atom::BondIterator it2;
		Atom::BondIterator it3;

		bool use_selection = getForceField()->getUseSelection();

		// proper torsion will be added to the torsion vector
		for (; atom_it != getForceField()->getAtoms().end(); ++atom_it) 
		{
			for (it1 = (*atom_it)->beginBond(); +it1 ; ++ it1) 
			{
				if (it1->getType() == Bond::TYPE__HYDROGEN) continue; // ignore H -bonds
				if (*atom_it != it1->getFirstAtom()) continue;
			
				// central atoms
				a2 = *atom_it;
				a3 = const_cast<Atom*>(it1->getSecondAtom());

				for (it2 = (*atom_it)->beginBond(); +it2 ; ++it2) 
				{
					if (it2->getType() == Bond::TYPE__HYDROGEN) continue; // ignore H -bonds
					if (it2->getSecondAtom() == it1->getSecondAtom()) continue;
				
					// determine the first atom
					if (it2->getFirstAtom() == *atom_it) 
					{
						a1 = const_cast<Atom*>(it2->getSecondAtom());
					} 
					else 
					{
						a1 = const_cast<Atom*>(it2->getFirstAtom());
					}

					for (it3 = const_cast<Atom*>(it1->getSecondAtom())->beginBond(); +it3 ; ++it3) 
					{
						if (it3->getType() == Bond::TYPE__HYDROGEN) continue; // ignore H -bonds
						if (it3->getFirstAtom() == a2 ) continue;

						// determine the fourth atom a4
						if (it3->getFirstAtom() == a3)
						{
							a4 = const_cast<Atom*>(it3->getSecondAtom());
						} 
						else 
						{
							a4 = const_cast<Atom*>(it3->getFirstAtom());
						}

						if (use_selection && (!a1->isSelected() ||
																	!a2->isSelected() ||
																	!a3->isSelected() ||
																	!a4->isSelected()))
						{
							continue;
						}

						////////////////////////////////////////
						// ok, we have the torsion
						////////////////////////////////////////
						
						// if a2 or a3 is a linear type, do nothing!
						if (atom_types[a2->getType()].lin ||
								atom_types[a3->getType()].lin)
						{
							continue;
						}

						// a2 type must always be smaller or equal than a3 type
						//
						// if a2 type == a3 type: a1 type must always be smaller or equal a4 type
						//
						// if not: swap direction
						if ((a2->getType() >  a3->getType()) ||
							  (a2->getType() == a3->getType() && a1->getType() > a4->getType()))
						{
							Atom* temp;
							temp = a1; a1 = a4; a4 = temp; 
							temp = a2; a2 = a3; a3 = temp; 
						}

						// if we have only 3 different atoms in one ring, do nothing!
						vector<Atom*> atoms;
						atoms.push_back(a1);
						atoms.push_back(a2);
						atoms.push_back(a3);
						atoms.push_back(a4);

						if (mmff->areInOneRing(atoms, 3)) continue;

						// search torsion parameters for (a1,a2,a3,a4)
						Atom::Type type_a1 = a1->getType();
						Atom::Type type_a2 = a2->getType();
						Atom::Type type_a3 = a3->getType();
						Atom::Type type_a4 = a4->getType();

						this_torsion.atom1 = &Atom::getAttributes()[a1->getIndex()];
						this_torsion.atom2 = &Atom::getAttributes()[a2->getIndex()];
						this_torsion.atom3 = &Atom::getAttributes()[a3->getIndex()];
						this_torsion.atom4 = &Atom::getAttributes()[a4->getIndex()];

						this_torsion.type = getTorsionType(atoms);

						if (this_torsion.type == 99) continue;

						// check for parameters in a step down procedure
						// full parameters
						if (parameters_.getParameters(this_torsion.type, 
																					type_a1, type_a2, type_a3, type_a4, 
																					this_torsion.v1, this_torsion.v2, this_torsion.v3)
							|| // try wildcard matching
							parameters_.getParameters(this_torsion.type, 
																				0,
																				equivalences.getEquivalence(type_a2, 1),
																				equivalences.getEquivalence(type_a3, 1),
																				equivalences.getEquivalence(type_a4, 1),
																				this_torsion.v1, this_torsion.v2, this_torsion.v3)
							|| // try wildcard matching
							parameters_.getParameters(this_torsion.type, 
																				equivalences.getEquivalence(type_a1, 1),
																				equivalences.getEquivalence(type_a2, 1),
																				equivalences.getEquivalence(type_a3, 1),
																				0,
																				this_torsion.v1, this_torsion.v2, this_torsion.v3)
							|| // try full wildcard matching
							parameters_.getParameters(this_torsion.type, 
																				0,
																				equivalences.getEquivalence(type_a2, 1),
																				equivalences.getEquivalence(type_a3, 1),
																				0,
																				this_torsion.v1, this_torsion.v2, this_torsion.v3)
							)
						{
							torsions_.push_back(this_torsion);
							continue;
						}

						// didnt found torsion parameters
						getForceField()->getUnassignedAtoms().insert(a1);
						getForceField()->getUnassignedAtoms().insert(a2);
						getForceField()->getUnassignedAtoms().insert(a3);
						getForceField()->getUnassignedAtoms().insert(a4);
					} // it3
				} // it2
			} // it1
		} // atom_it

		return true;
	}

	// calculates the current energy of this component
	double MMFF94Torsion::updateEnergy() 
	{
		/*
		double cosphi;

		Vector3	a21;
		Vector3 a23;
		Vector3 a34;
		Vector3 cross2321;
		Vector3 cross2334;

		energy_ = 0;

		vector<SingleMMFF94Torsion>::const_iterator it = torsion_.begin(); 

		bool use_selection = getForceField()->getUseSelection();

		for (; it != torsion_.end(); it++) 
		{
			if (!use_selection ||
					(use_selection  &&
					(it->atom1->ptr->isSelected() || it->atom2->ptr->isSelected() 
					 || it->atom3->ptr->isSelected() || it->atom4->ptr->isSelected())))
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

//   					energy_ += it->V * (1 + cos(it->f * acos(cosphi) - it->phase));
				}
			}
		}
*/
		return energy_;
	}

	// calculates and adds its forces to the current forces of the force field
	void MMFF94Torsion::updateForces()
	{
//   		bool use_selection = getForceField()->getUseSelection();
	}


	// The first column gives the value of the torsion type index, TTIJKL. This 
	// index normally takes the value "0", but is "1" when the j-k bond has a bond 
	// type index BTJK of 1, is "2" when BTJK is "0" but BTIJ and/or BTKL is "1", is 
	// "4" when i, j, k, and l are all members of the same 4-membered ring, and 
	// is "5" when the four atoms are members of a 5-membered ring which is 
	// not aromatic and contains no unsaturation.
	Position MMFF94Torsion::getTorsionType(const vector<Atom*>& atoms) const
	{
		MMFF94* mmff = dynamic_cast<MMFF94*>(getForceField());

		const Bond* bond1 = atoms[0]->getBond(*atoms[1]);
		const Bond* bond2 = atoms[1]->getBond(*atoms[2]);
		const Bond* bond3 = atoms[2]->getBond(*atoms[3]);

		HashSet<Atom*> set;
		set.insert(atoms[0]);
		set.insert(atoms[1]);
		set.insert(atoms[2]);
		set.insert(atoms[3]);

		if (set.size() < 4) return 99;

		if (!bond1 || !bond2 || !bond3)
		{
			Log.error() << "Problem in " << __FILE__ << __LINE__ << std::endl;
			Log.error() << atoms[0]->getName() << " " 
									<< atoms[1]->getName() << " "
									<< atoms[2]->getName() << " "
									<< atoms[3]->getName() << std::endl;
			return 99;
		}
		
		if (bond2->hasProperty("MMFF94SBMB")) return 1;

		if (bond1->hasProperty("MMFF94SBMB") ||
				bond3->hasProperty("MMFF94SBMB"))
		{
			return 2;
		}

		if (mmff->areInOneRing(atoms, 4)) return 4;

		if (mmff->areInOneRing(atoms, 5))
		{
			// ???? saturation
			return 5;
		}

		// default value: 0
		return 0;
	}


} // namespace BALL
