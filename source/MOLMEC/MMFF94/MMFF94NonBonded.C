// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MMFF94NonBonded.C,v 1.1.2.11 2006/02/21 21:06:51 amoll Exp $
//

#include <BALL/MOLMEC/MMFF94/MMFF94NonBonded.h>
#include <BALL/MOLMEC/MMFF94/MMFF94.h>
#include <BALL/MOLMEC/COMMON/forceField.h>
#include <BALL/MOLMEC/COMMON/support.h>
#include <BALL/SYSTEM/path.h>

using namespace std;

#define BALL_MMFF94_TEST

namespace BALL 
{

	MMFF94NonBonded::NonBondedPairData::NonBondedPairData()
		: eij(0),
			rij(0),
			rij_7(0),
			VDW_energy(0),
			is_1_4(false),
			qi(0),
			qj(0),
			ES_energy(0)
	{
	}

	MMFF94NonBonded::MMFF94NonBonded()
		throw()
		:	ForceFieldComponent(),
			algorithm_type_(MolmecSupport::BRUTE_FORCE),
			cut_off_(10)
	{	
		setName("MMFF94 NonBonded");
	}

	MMFF94NonBonded::MMFF94NonBonded(ForceField& force_field)
		throw()
		:	ForceFieldComponent(force_field),
			algorithm_type_(MolmecSupport::BRUTE_FORCE),
			cut_off_(10)
	{
		setName("MMFF94 NonBonded");
	}

	MMFF94NonBonded::MMFF94NonBonded(const MMFF94NonBonded& component)
		throw()
		:	ForceFieldComponent(component),
			algorithm_type_(component.algorithm_type_)
	{
	}

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
		
		return MolmecSupport::BRUTE_FORCE;
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
		atom_pair_vector_.clear();

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

		non_bonded_data_.resize(atom_pair_vector_.size());

		for (Position p = 0; p < atom_pair_vector_.size(); p++)
		{
			Atom* const atom1 = atom_pair_vector_[p].first;
			Atom* const atom2 = atom_pair_vector_[p].second;

			NonBondedPairData& data = non_bonded_data_[p];

			data.is_1_4 = atom1->isVicinal(*atom2);

			vdw_parameters_.getParameters(atom1->getType(), atom2->getType(), data.rij, data.rij_7, data.eij);

#ifdef BALL_MMFF94_DEBUG
			Log.info() << "NB pair " << atom1->getName() << " " << atom2->getName() << std::endl;
#endif
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
				
		if (!vdw_parameters_.isInitialized())
		{
			Path    path;

			String  filename(path.find("MMFF94/MMFFVDW.PAR"));
			if (filename == "") throw Exception::FileNotFound(__FILE__, __LINE__, "MMFFVDW.PAR");
			vdw_parameters_.readParameters(filename);

			filename = path.find("MMFF94/MMFFCHG.PAR");
			if (filename == "") throw Exception::FileNotFound(__FILE__, __LINE__, "MMFFCHG.PAR");
			es_parameters_.readParameters(filename);

			filename = path.find("MMFF94/MMFFPBCI.PAR");
			if (filename == "") throw Exception::FileNotFound(__FILE__, __LINE__, "MMFFPBCI.PAR");
			es_parameters_.readEmpericalParameters(filename);
		}

		// Determine the most efficient way to calculate all non bonded atom pairs
		algorithm_type_ = determineMethodOfAtomPairGeneration();

		charge_map_.clear();

		vector<Atom*>::const_iterator	ait = getForceField()->getAtoms().begin();
		for (; ait != getForceField()->getAtoms().begin(); ++ait)
		{
			Atom* const atom1 = *ait;
			double charge = atom1->getCharge();

			AtomBondIterator bit = atom1->beginBond();
			for (; +bit; ++bit)
			{
				const Atom* const atom2 = bit->getPartner(*atom1);
				Position bt = bit->hasProperty("MMFF94SBMB"); // ????
				const double pcharge = es_parameters_.getPartialCharge(atom1->getType(), atom2->getType(), bt);
				if (pcharge == -99) 
				{
					getForceField()->getUnassignedAtoms().insert(atom1);
					getForceField()->getUnassignedAtoms().insert(atom2);
					getForceField()->error() << "MMFF94 NonBonded: Could not assign partial charges for " 
																	 << atom1->getName() << " " << atom2->getName() << std::endl;
				}
				charge += pcharge;
			}
		}

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

			if (Maths::isZero(d)) continue;
			d = sqrt(d);

			NonBondedPairData& data = non_bonded_data_[p];

			const double first = data.eij * pow( 1.07 * data.rij / (d + 0.07 * data.rij), 7);

			const double sec = ((1.12 * data.rij_7) / (pow(d, 7) + 0.12 * data.rij_7))  - 2;

			const double e = first * sec;
			vdw_energy_ += e;

#ifdef BALL_MMFF94_TEST
			data.VDW_energy = e;
#endif
#ifdef BALL_MMFF94_DEBUG
			Log.info() << "VDW " << atom_pair_vector_[p].first->getName() << " " 
													 << atom_pair_vector_[p].second->getName() << " e " 
													 << data.eij << " r " << data.rij << " E " << e << " d " << d << std::endl;
#endif
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
