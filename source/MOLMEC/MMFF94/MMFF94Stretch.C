// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MMFF94Stretch.C,v 1.1.2.11 2005/03/23 16:13:23 amoll Exp $
//

#include <BALL/MOLMEC/MMFF94/MMFF94Stretch.h>
#include <BALL/MOLMEC/MMFF94/MMFF94.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/QSAR/ringPerceptionProcessor.h>
#include <BALL/QSAR/aromaticityProcessor.h>

#define BALL_DEBUG_MMFF

using namespace std;

namespace BALL 
{

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
		if (getForceField() == 0) 
		{
			Log.error() << "MMFF94Stretch::setup(): component not bound to force field" << endl;
			return false;
		}

		if (!parameters_.isInitialized())
		{
			Path    path;
			String  filename(path.find("MMFF94/MMFFBOND.PAR"));

			if (filename == "") 
			{
				throw Exception::FileNotFound(__FILE__, __LINE__, filename);
			}

			parameters_.readParameters(filename);
		}

		stretch_.clear();
		rings_.clear();

		vector<vector<Atom*> > rings;

		RingPerceptionProcessor rpp;
		rpp.calculateSSSR(rings, *getForceField()->getSystem());

		AromaticityProcessor ap;
		ap.aromatize(rings, *getForceField()->getSystem());

		for (Position i = 0; i < rings.size(); i++)
		{
			bool ok = true;
			for (Position j = 0; j < rings[i].size(); j++)
			{
				if (!rings[i][j]->hasProperty("IsAromatic"))
				{
					ok = false;
					break;
				}
			}

			if (ok &&
				rings[i].size() >=5 && rings[i].size() <= 6)
			{
				rings_.push_back(HashSet<Atom*>());
				for (Position j = 0; j < rings[i].size(); j++)
				{
					rings_[rings_.size() - 1].insert(rings[i][j]);
				}
			}
		}
		
#ifdef BALL_DEBUG_MMFF
Log.info() << "MMFF94Strech Number of rings: "  << rings_.size() << std::endl;
#endif

		bool use_selection = getForceField()->getUseSelection();

		// a working instance to put the current values in and push it back
		Stretch dummy_stretch;

		AtomVector::ConstIterator atom_it = getForceField()->getAtoms().begin();
		for (; atom_it != getForceField()->getAtoms().end(); ++atom_it)
		{
			for (Atom::BondIterator it = (*atom_it)->beginBond(); +it ; ++it) 
			{
				// make sure we process each bond only once
				if (*atom_it != it->getFirstAtom()) continue;
				
				Bond&	bond = *it;
				if (bond.getType() == Bond::TYPE__HYDROGEN)
				{	
					// Ignore hydrogen bonds!
					continue;
				}

			  Atom& atom1(*(Atom*)bond.getFirstAtom());
			  Atom& atom2(*(Atom*)bond.getSecondAtom());

				const bool make_it = !use_selection || (use_selection && atom1.isSelected() && atom2.isSelected());
				if (!make_it) continue;

				const Atom::Type atom_type_A = atom1.getType();
				const Atom::Type atom_type_B = atom2.getType();

				// take the sbmb value if :

				// is there an optional sbmb value ?
				// is the bond order == 1 ?
				// are both atoms sp or sp2 hypridised?
				bool get_sbmb = 
					parameters_.hasOptionalSBMBParameter(atom_type_A, atom_type_B) &&
					bond.getOrder() == 1   									&&
					(isSp_(atom1) || isSp2_(atom1)) 				&&
					(isSp_(atom2) || isSp2_(atom2))					&&
 					!isInOneAromaticRing_(bond);


int reason = 0;
if (!parameters_.hasOptionalSBMBParameter(atom_type_A, atom_type_B)) reason = 1;
if (bond.getOrder() != 1) reason = 2;
if (! ((isSp_(atom1) || isSp2_(atom1)) && (isSp_(atom2) || isSp2_(atom2)))) reason = 3;
if (isInOneAromaticRing_(bond)) reason = 4;

dummy_stretch.reason = reason;


				dummy_stretch.sbmb = get_sbmb;

				if (get_sbmb)
				{
					parameters_.getOptionalSBMBParameters(bond, dummy_stretch.kb, dummy_stretch.r0);
				}

				else if (!parameters_.getParameters(bond, dummy_stretch.kb, dummy_stretch.r0))
				{
					getForceField()->error() << "cannot find stretch parameters for atom types " 
							<< atom_type_A << " " << atom_type_B 
							<< " (atoms are: " 
							<< atom1.getFullName(Atom::ADD_VARIANT_EXTENSIONS_AND_ID) << " " 
							<< atom2.getFullName(Atom::ADD_VARIANT_EXTENSIONS_AND_ID) << ")" << std::endl;

					getForceField()->getUnassignedAtoms().insert(&atom1);
					getForceField()->getUnassignedAtoms().insert(&atom2);
					continue;
				}

				dummy_stretch.atom1 = &atom1; 
				dummy_stretch.atom2 = &atom2;

#ifdef BALL_DEBUG_MMFF
				Log.info() << atom1.getFullName() << " -> " 
					         << atom2.getFullName() << " : "
									 << atom_type_A << " " << atom_type_B << std::endl;
#endif

				stretch_.push_back(dummy_stretch);
			}
		}

		return true;
	}

	bool MMFF94Stretch::isInOneAromaticRing_(const Bond& bond)
	{
		Atom* atom1 = (Atom*) bond.getFirstAtom();
		Atom* atom2 = (Atom*) bond.getSecondAtom();
		for (Position pos = 0; pos < rings_.size(); pos++)
		{
			if (rings_[pos].has(atom1) &&
					rings_[pos].has(atom2))
			{
#ifdef BALL_DEBUG_MMFF
				Log.info() << atom1->getName() << " " 
									 << atom2->getName() << " "  
									 << " removed atom from sbmbs" << std::endl;
#endif
				return true;
			}
		}
		
		return false;
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

} // namespace BALL
