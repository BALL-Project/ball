// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MMFF94Stretch.C,v 1.1.2.6 2005/03/22 18:27:25 amoll Exp $
//

#include <BALL/MOLMEC/MMFF94/MMFF94Stretch.h>
#include <BALL/MOLMEC/MMFF94/MMFF94.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/QSAR/ringPerceptionProcessor.h>

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

		RingPerceptionProcessor rpp;
		vector< vector<Atom*> > rings;
		rpp.calculateSSSR(rings, *getForceField()->getSystem());

		for (Position pos = 0; pos < rings.size(); pos++)
		{
			if (rings[pos].size() >= 5 && rings[pos].size() <= 6)
			{
				rings_.push_back(HashSet<Atom*>());
				Position ring_pos = rings_.size() - 1;
				for (Position atom_pos = 0; atom_pos < rings[pos].size(); atom_pos++)
				{
					rings_[ring_pos].insert(rings[pos][atom_pos]);
				}
			}
		}

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
				const bool get_sbmb = 
					parameters_.hasOptionalSBMBParameter(atom_type_A, atom_type_B) &&
					bond.getOrder() == 1   									&&
					(isSp_(atom1) || isSp2_(atom1)) 				&&
					(isSp_(atom2) || isSp2_(atom2))					&&
					!isInOneRing_(bond);

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

	bool MMFF94Stretch::isInOneRing_(const Bond& bond)
	{
		Atom* atom1 = (Atom*) bond.getFirstAtom();
		Atom* atom2 = (Atom*) bond.getSecondAtom();
		for (Position pos = 0; pos < rings_.size(); pos++)
		{
			if (rings_[pos].has(atom1) &&
					rings_[pos].has(atom2))
			{
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
			const double delta(::std::fabs(distance - (double) stretch_[i].r0));
			const double delta_2(delta * delta);

			double eb_ij = K0 * (double) stretch_[i].kb * delta_2 *
				            (1.0L + CUBIC_STRENGTH_CONSTANT * delta + KCS * delta_2);

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

//   		bool use_selection = getForceField()->getUseSelection();

		/*
		// iterate over all bonds, update the forces
		for (Size i = 0 ; i < stretch_.size(); i++)
		{
			Atom::StaticAtomAttributes& atom1(*stretch_[i].atom1);
			Atom::StaticAtomAttributes& atom2(*stretch_[i].atom2);
			Vector3 direction(atom1.position - atom2.position);
			double distance = direction.getLength(); 

			if (distance != 0.0) 
			{
				// unit conversion: from kJ/(mol A) -> N
				//   kJ -> J: 1e3
				//   A  -> m: 1e10
				//   J/mol -> J: Avogadro
//   				direction *= 1e13 / Constants::AVOGADRO * 2 * stretch_[i].values.k * (distance - stretch_[i].values.r0) / distance;

				if (!use_selection || atom1.ptr->isSelected()) 
				{
					atom1.force -= direction;
				}
				if (!use_selection || atom2.ptr->isSelected()) 
				{
					atom2.force += direction;
				}
			}
		}                                                                                                          
		*/
	}

} // namespace BALL
