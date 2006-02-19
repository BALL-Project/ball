// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MMFF94NonBonded.C,v 1.1.2.6 2006/02/19 22:38:12 amoll Exp $
//

#include <BALL/MOLMEC/MMFF94/MMFF94NonBonded.h>
#include <BALL/MOLMEC/MMFF94/MMFF94.h>
#include <BALL/MOLMEC/COMMON/forceField.h>
#include <BALL/MOLMEC/COMMON/support.h>
#include <BALL/SYSTEM/path.h>

using namespace std;

// define a macro for the square function
#define SQR(x) ((x) * (x))

namespace BALL 
{

	// default constructor
	MMFF94NonBonded::MMFF94NonBonded()
		throw()
		:	ForceFieldComponent(),
			algorithm_type_(MolmecSupport::BRUTE_FORCE)
	{	
		// set component name
		setName("MMFF94 NonBonded");
	}


	// constructor
	MMFF94NonBonded::MMFF94NonBonded(ForceField& force_field)
		throw()
		:	ForceFieldComponent(force_field),
			algorithm_type_(MolmecSupport::BRUTE_FORCE)
	{
		// set component name
		setName("MMFF94 NonBonded");
	}


	// copy constructor
	MMFF94NonBonded::MMFF94NonBonded(const MMFF94NonBonded& component)
		throw()
		:	ForceFieldComponent(component),
			algorithm_type_(component.algorithm_type_)
	{
	}


	// destructor
	MMFF94NonBonded::~MMFF94NonBonded()
		throw()
	{
		clear();
	}

	const MMFF94NonBonded& MMFF94NonBonded::operator = (const MMFF94NonBonded& anb)
		throw()
	{
		// catch self assignment
		if (&anb == this) return *this;

		// call the base class operator
		ForceFieldComponent::operator = (anb);
		algorithm_type_ = anb.algorithm_type_;
		return *this;
	}

	void MMFF94NonBonded::clear()
		throw()
	{
		algorithm_type_ = MolmecSupport::BRUTE_FORCE;
	}


	bool MMFF94NonBonded::operator == (const MMFF94NonBonded& anb)
		throw()
	{
		return (this == &anb);
	}

	// This function determines the most efficient way to calculate all
	// non-bonded atom pairs that depends on the number of atoms of the
	// system. The function return value 0 if the number of atoms is so
	// small that the brute force all against all comparison is the most
	// efficient way. Otherwise it returns 1. 
	MolmecSupport::PairListAlgorithmType
	MMFF94NonBonded::determineMethodOfAtomPairGeneration()
		throw()
	{
		if (force_field_->getAtoms().size() > 900) 
		{ 
			return MolmecSupport::HASH_GRID;
		} 
		else
		{
			return MolmecSupport::BRUTE_FORCE;
		}
	}

	void MMFF94NonBonded::update()
		throw(Exception::TooManyErrors)
	{
		if (getForceField() == 0) 
		{
			Log.error() << "MMFF94NonBonded::update(): component not bound to a force field" << endl;
			return;
		}

		// Calculate all non bonded atom pairs

		cut_off_ = 100;

		MolmecSupport::calculateNonBondedAtomPairs
			(atom_pair_vector_, getForceField()->getAtoms(), 
			 getForceField()->periodic_boundary.getBox(),
			 cut_off_, force_field_->periodic_boundary.isEnabled(), 
			 algorithm_type_);

		if (getForceField()->getSystem()->containsSelection())
		{
			// eliminate all those pairs where none of the two atoms is selected
			Size number_of_selected_pairs = MolmecSupport::sortNonBondedAtomPairsAfterSelection(atom_pair_vector_);
			atom_pair_vector_.resize(number_of_selected_pairs);
		}

		eijs_.resize(atom_pair_vector_.size());
		rijs_.resize(atom_pair_vector_.size());
		rijs_7_.resize(atom_pair_vector_.size());

		for (Position p = 0; p < atom_pair_vector_.size(); p++)
		{
			parameters_.getParameters(atom_pair_vector_[p].first->getType(),
																atom_pair_vector_[p].second->getType(),
																rijs_[p], rijs_7_[p], eijs_[p]);
		}
	}

	// setup the internal datastructures for the component
	bool MMFF94NonBonded::setup()
		throw(Exception::TooManyErrors)
	{
		if (getForceField() == 0 ||
				!dynamic_cast<MMFF94*>(getForceField()))
		{
			Log.error() << "MMFF94NonBonded::setup(): component not bound to a force field" << endl;
			return false;
		}

		// clear vector of non-bonded atom pairs
		clear();
 
		// when using periodic boundary conditions, all
		// cutoffs must be smaller than the smallest linear extension of
		// the box - we use the minimum image convention!
		if (getForceField()->periodic_boundary.isEnabled())
		{
			SimpleBox3 box = getForceField()->periodic_boundary.getBox();
			double max_cut_off = 0.5 * Maths::min(box.getWidth(), box.getHeight(), box.getDepth());

			if (cut_off_> max_cut_off)
			{
				Log.error() << "MMFF94NonBonded::setup(): "
										<< "electrostatic cutoff may not exceed half" << endl
										<< "the box dimension when using periodic boundary conditions!" << endl;
				return false;
			}
		}
				
		if (!parameters_.isInitialized())
		{
			Path    path;
			String  filename(path.find("MMFF94/MMFFVDW.PAR"));

			if (filename == "") throw Exception::FileNotFound(__FILE__, __LINE__, "[empty]");

			parameters_.readParameters(filename);
		}

		// Determine the most efficient way to calculate all non bonded atom pairs
		algorithm_type_ = determineMethodOfAtomPairGeneration();

		// build the nonbonded pairs
		update();

		return true;
	}


	double MMFF94NonBonded::updateEnergy()
		throw()
	{

		energy_ = 0;
		vdw_energy_ = 0;

		for (Position p = 0; p < atom_pair_vector_.size(); p++)
		{
			double d = (atom_pair_vector_[p].first->getPosition() - 
									atom_pair_vector_[p].second->getPosition()).getSquareLength();

			if (Maths::isZero(p)) continue;

			d = sqrt(d);

			const double& rij = rijs_[p];
			const double& eij = eijs_[p];
			const double& rij7 = rijs_7_[p];

			const double first = eij * pow( 1.07 * rij / (d + 0.07 * rij), 7);

			const double sec = ((1.12 * rij7) / ( pow(d, 7) + 0.12 * rij7))  - 2;

			vdw_energy_ += first * sec;

			Log.info() << "VDW " << atom_pair_vector_[p].first->getName() << " " 
													 << atom_pair_vector_[p].second->getName() << " e " 
													 << eij << " r " << rij << " "
													 << first * sec 
													 << " " << first << " " << sec << " " << d
													 << std::endl;
		}

		energy_ += vdw_energy_;

		return energy_; 
  } 
	

	void MMFF94NonBonded::updateForces()
		throw()
	{
		if (getForceField() == 0)
		{
			return;
		}
	} 

	
	double MMFF94NonBonded::getElectrostaticEnergy() const
		throw()
	{
		return electrostatic_energy_;
	}

	double MMFF94NonBonded::getVdwEnergy() const
		throw()
	{
		return vdw_energy_;
	}

} // namespace BALL
