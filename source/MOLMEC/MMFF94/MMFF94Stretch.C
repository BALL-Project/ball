// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MMFF94Stretch.C,v 1.1.2.19 2006/02/09 22:58:00 amoll Exp $
//

#include <BALL/MOLMEC/MMFF94/MMFF94Stretch.h>
#include <BALL/MOLMEC/MMFF94/MMFF94.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/SYSTEM/path.h>

//   #define BALL_DEBUG_MMFF

using namespace std;

namespace BALL 
{

	// Constant CS
	#define CUBIC_STRENGTH_CONSTANT -2.0

	// Constant 7 / 12 * CS^2
	#define KCS 7.0 / 3.0

	// Constant 143.9325 / 2
	#define K0 71.96625


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
		if (getForceField() == 0 ||
				dynamic_cast<MMFF94*>(getForceField()) == 0) 
		{
			Log.error() << "MMFF94Stretch::setup(): component not bound to force field" << endl;
			return false;
		}

		stretch_.clear();
		
		// a working instance to put the current values in and push it back
		Stretch dummy_stretch;

		const MMFF94& mmff = *dynamic_cast<const MMFF94*>(getForceField());

		parameters_ = &mmff.getStretchParameters();
		MMFF94StretchParameters::StretchMap::ConstIterator stretch_it;

		const vector<Bond*>& bonds = mmff.getBonds();
		
		vector<Bond*>::const_iterator bond_it = bonds.begin();
		for (; bond_it != bonds.end(); bond_it++)
		{
			stretch_it = parameters_->getParameters(**bond_it);
			
			Atom& atom1 = *(Atom*)(*bond_it)->getFirstAtom();
			Atom& atom2 = *(Atom*)(*bond_it)->getSecondAtom();

			const bool is_sbmb = (**bond_it).hasProperty("MMFF94SBMB");
			dummy_stretch.sbmb = is_sbmb;
			dummy_stretch.atom1 = &atom1; 
			dummy_stretch.atom2 = &atom2;

			if (+stretch_it)
			{
				const MMFF94StretchParameters::BondData& data = stretch_it->second;

				if (is_sbmb)
				{
					dummy_stretch.r0 = data.r0_sbmb;
					dummy_stretch.kb = data.kb_sbmb;
				}
				else
				{
					dummy_stretch.r0 = data.r0_normal;
					dummy_stretch.kb = data.kb_normal;
				}

				(**bond_it).setProperty("MMFF94RBL", (double) dummy_stretch.r0);
				stretch_.push_back(dummy_stretch);

				continue;
			}

			// try emperical values
			double r0 = calculateR0(**bond_it);
			double k  = calculateStretchConstant(**bond_it, r0);

			if (r0 != -1 && k != -1)
			{
				dummy_stretch.r0 = r0;
				dummy_stretch.kb = k;
				dummy_stretch.emperical = true;
				(**bond_it).setProperty("MMFF94RBL", (double) dummy_stretch.r0);
				stretch_.push_back(dummy_stretch);
				continue;
			}

			getForceField()->error() << "cannot find stretch parameters for atom types " 
				<< atom1.getType() << " "
				<< atom2.getType() << " "
				<< " (atoms are: " 
				<< atom1.getFullName(Atom::ADD_VARIANT_EXTENSIONS_AND_ID) << " " 
				<< atom2.getFullName(Atom::ADD_VARIANT_EXTENSIONS_AND_ID) 
				<< ")" << std::endl;

			getForceField()->getUnassignedAtoms().insert(&atom1);
			getForceField()->getUnassignedAtoms().insert(&atom2);
			continue;
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

		for (Size i = 0 ; i < stretch_.size(); i++)
		{
			const Vector3 direction(stretch_[i].atom1->getPosition() - stretch_[i].atom2->getPosition());
			double distance = direction.getLength(); 
			const double delta(distance - (double) stretch_[i].r0);
			stretch_[i].delta_r = delta;
			const double delta_2(delta * delta);

			double eb_ij = (double) K0 * (double) stretch_[i].kb * delta_2 *
				            ((double) 1.0 + (double) CUBIC_STRENGTH_CONSTANT * delta + (double) KCS * delta_2);

#ifdef BALL_DEBUG_MMFF
			Log.info() << stretch_[i].atom1->getFullName() << " -> " 
								 << stretch_[i].atom2->getFullName() 
								 << "   r0: " << stretch_[i].r0
								 << "   D: " << delta << "   E: " << eb_ij << std::endl;
#endif

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

		// iterate over all bonds, update the forces
		for (Size i = 0 ; i < stretch_.size(); i++)
		{
			Vector3 direction(stretch_[i].atom1->getPosition() - stretch_[i].atom2->getPosition());
			const double r0(stretch_[i].r0);
			const double delta(direction.getLength() - r0);

			const double a(143.9325 / 2  * r0);

			double force = -(2 * a * delta + 
											 3 * a * CUBIC_STRENGTH_CONSTANT * delta * delta + 
											 4 * a * KCS * delta * delta);

			direction.normalize();
			direction *= force;

			stretch_[i].atom1->getForce()-= direction;
			stretch_[i].atom2->getForce()+= direction;
		}                                                                                                          
	}

	// Calculate the reference bond length value using a modified Schomaker-Stevenson rule
	double MMFF94Stretch::calculateR0(const Bond& bond)
	{
		const Atom& atom1 = *bond.getFirstAtom();
		const Atom& atom2 = *bond.getSecondAtom();

		const Position e1 = atom1.getElement().getAtomicNumber();
		const Position e2 = atom2.getElement().getAtomicNumber();

		const Position t1 = atom1.getType();
		const Position t2 = atom2.getType();

		// currently only supports atoms up to Xenon
		if (e1 > 53 || e2 > 53 ||
				e1 == 0 || e2 == 0) 
		{
			return -1;
		}

		// radii
		double r1 = parameters_->radii[e1 - 1];
		double r2 = parameters_->radii[e2 - 1];

		// only for stored radii
		if (r1 == 0.0 || r2 == 0.0)
		{
			return -1;
		}

		Position bo = bond.getOrder();
		if (bo == Bond::ORDER__UNKNOWN || 
				bo == Bond::ORDER__QUADRUPLE ||
				bo == Bond::ORDER__ANY)
		{
			return -1;
		}

		if (getForceField() == 0 ||
				dynamic_cast<MMFF94*>(getForceField()) == 0)
		{
			Log.error() << "No MMFF94 ForceField available " << __FILE__ << " " << __LINE__ << std::endl;
			return -1;
		}

		const MMFF94* mmff = dynamic_cast<MMFF94*>(getForceField());
		const vector<MMFF94AtomTypeData>& atom_types = mmff->getAtomTypes();

		Position b1 = atom_types[t1].mltb;
		Position b2 = atom_types[t2].mltb;

		if (b1 == 1 && b2 == 1) bo = 4;
		else if (b1 + b2 == 3)  bo = 5;
		else
		{
			// if aromatisch and same ring:
			vector <Atom*> atoms;
			atoms.push_back((Atom*)&atom1);
			atoms.push_back((Atom*)&atom2);
			if (mmff->areInOneAromaticRing(atoms, 0))
			{
				if (!atom_types[t1].pilp && !atom_types[t2].pilp)
				{
					bo = 4;
				}
				else 
				{
					bo = 5;
				}
			}
		}

		// calculate corrected radii
		
		if (bo == 5)
		{
			r1 -= 0.04;
			r2 -= 0.04;
		}
		else if (bo == 4)
		{
			r1 -= 0.075;
			r2 -= 0.075;
		}
		else if (bo == 3)
		{
			r1 -= 0.17;
			r2 -= 0.17;
		}
		else if (bo == 2)
		{
			r1 -= 0.1;
			r2 -= 0.1;
		}
		else 
		{
			if (b1 == 1 || b1 == 2) 
			{
				r1 -= 0.03;
			}
			if (b2 == 1 || b2 == 2) 
			{
				r2 -= 0.03;
			}

			if (b1 == 3) r1 -= 0.08;
			if (b2 == 3) r2 -= 0.08;
		}

		// calculate shrink factor
		double d = 0.008; // SHRINK FACTOR

		// for hyrogen atoms no shrinking
		if (e1 == 1 || e2 == 1) d = 0.0;

		// for atoms > neon no shrinking
		if (e1 > 10 || e2 > 10) d = 0.0;

		// calculate SENS c
		double c = 0.085;

		// for hyrogen atoms
		if (e1 == 1 || e2 == 1) c = 0.05;

		// POWER
		const double n = 1.4;

		// FORMULA from CHARMM docu:
		const double r0 = parameters_->radii[e1 - 1] + parameters_->radii[e2 - 1] - c * 
									pow(fabs(parameters_->electronegatives[e1 - 1] - parameters_->electronegatives[e2 - 1]), n) - d;
		
		return r0;
	}


	// calculate force constant:
	// requisite data is not available, use relationship developed by Badger
	// parameters are those described in: D. L. Herschbach and V. W. Laurie, J. Chem.  Phys. 1961, 35, 458-463.
	double MMFF94Stretch::calculateStretchConstant(const Bond& bond, double r0)
	{
		const Atom& a1 = *bond.getFirstAtom();
		const Atom& a2 = *bond.getSecondAtom();

		Index ij = getMMFF94Index(a1.getElement().getAtomicNumber(), a2.getElement().getAtomicNumber());

		if (parameters_->getEmpericalParameters().has(ij))
		{
			const MMFF94StretchParameters::EmpericalBondData& bd = parameters_->getEmpericalParameters()[ij];
			const double kb = bd.kb * pow((bd.r0 / r0), 6);
			return kb;
		}

		Position e1 = a1.getElement().getAtomicNumber();
		Position e2 = a2.getElement().getAtomicNumber();
		Position p1 = BALL_MIN(e1, e2);
		Position p2 = BALL_MAX(e1, e2);

		const Position HELIUM = 2;
		const Position NEON = 10;
		const Position ARGON = 18;
		const Position KRYPTN = 36;
		const Position XENON = 54;
		const Position RADON = 86;

		// from CHARMM implementation
		// default values
		double	AIJ = 3.15;
		double	BIJ = 1.80;

		// special values taken from HERSCHBACH and LAURIE 1961
		if (p1 < HELIUM)
		{
			if      (p2 < HELIUM) { AIJ = 1.26; BIJ = 0.025; } // 0.025 is not an error!
			else if (p2 < NEON)   { AIJ = 1.66; BIJ = 0.30; }
			else if (p2 < ARGON)  { AIJ = 1.84; BIJ = 0.38; }
			else if (p2 < KRYPTN) { AIJ = 1.98; BIJ = 0.49; }
			else if (p2 < XENON)  { AIJ = 2.03; BIJ = 0.51; }
			else if (p2 < RADON)  { AIJ = 2.03; BIJ = 0.25; }
		}
		else if (p1 < NEON)
		{
			if 			(p2 < NEON) 	{ AIJ = 1.91; BIJ = 0.68; }
			else if (p2 < ARGON)	{ AIJ = 2.28; BIJ = 0.74; }
			else if (p2 < KRYPTN) { AIJ = 2.35; BIJ = 0.85; }
			else if (p2 < XENON)  { AIJ = 2.33; BIJ = 0.68; }
			else if (p2 < RADON)  { AIJ = 2.50; BIJ = 0.97; }
		}
		else if (p1 < ARGON)
		{
			if 			(p2 < ARGON)  { AIJ = 2.41; BIJ = 1.18; }
			else if (p2 < KRYPTN) { AIJ = 2.52; BIJ = 1.02; }
			else if (p2 < XENON) 	{ AIJ = 2.61; BIJ = 1.28; }
			else if (p2 < RADON) 	{ AIJ = 2.60; BIJ = 0.84; }
		}
		else if (p1 < KRYPTN)
		{
			if 			(p2 < KRYPTN) { AIJ = 2.58; BIJ = 1.41; }
			else if (p2 < XENON)  { AIJ = 2.66; BIJ = 0.86; }
			else if (p2 < RADON)  { AIJ = 2.75; BIJ = 1.14; }
		}
		else if (p1 < XENON)
		{
			if 			(p2 < XENON) { AIJ = 2.85; BIJ = 1.62; }
			else if (p2 < XENON) { AIJ = 2.76; BIJ = 1.25; }
		}

		double kb = pow(((AIJ - BIJ) / (r0 - BIJ)), 3);
		return kb;
	}


} // namespace BALL
