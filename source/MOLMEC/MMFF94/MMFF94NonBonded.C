// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MMFF94NonBonded.C,v 1.1.6.2 2006/08/31 14:05:50 leonhardt Exp $
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

	// Conversion from kJ / (mol A) into Newton
	double FORCES_FACTOR = 1000 * 1E10 / Constants::AVOGADRO;


	// ES_CONSTANT
	double ES_CONSTANT = 332.0716 * Constants::JOULE_PER_CAL;


	MMFF94NonBonded::NonBondedPairData::NonBondedPairData()
		: eij(0),
			rij(0),
			rij_7(0),
			vdw_energy(0),
			is_1_4(false),
			qi(0),
			qj(0),
			es_energy(0)
	{
	}

	MMFF94NonBonded::MMFF94NonBonded()
		throw()
		:	ForceFieldComponent(),
			algorithm_type_(MolmecSupport::BRUTE_FORCE),
			cut_off_(20),
			dc_(1),
			n_(1),
			es_enabled_(true),
			vdw_enabled_(true)
	{	
		setName("MMFF94 NonBonded");
	}

	MMFF94NonBonded::MMFF94NonBonded(ForceField& force_field)
		throw()
		:	ForceFieldComponent(force_field),
			algorithm_type_(MolmecSupport::BRUTE_FORCE),
			cut_off_(20),
			dc_(1),
			n_(1),
			es_enabled_(true),
			vdw_enabled_(true)
	{
		setName("MMFF94 NonBonded");
	}

	MMFF94NonBonded::MMFF94NonBonded(const MMFF94NonBonded& component)
		throw()
		:	ForceFieldComponent(component),
			algorithm_type_(component.algorithm_type_),
			dc_(component.dc_),
			n_(component.n_),
			es_enabled_(component.es_enabled_),
			vdw_enabled_(component.vdw_enabled_)
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
		dc_ = anb.dc_;
		n_  = anb.n_;
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
			data.qi = atom1->getCharge();
			data.qj = atom2->getCharge();

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

		es_energy_ = 0;
		vdw_energy_ = 0;

 		Options& options = getForceField()->options;
		vdw_enabled_ = !options.has(MMFF94_VDW_ENABLED) || options.getBool(MMFF94_VDW_ENABLED);
		es_enabled_  = !options.has(MMFF94_ES_ENABLED)  || options.getBool(MMFF94_ES_ENABLED);
		bool disabled = !vdw_enabled_ && !es_enabled_;

		if (disabled)
		{
			setEnabled(false);
			return true;
		}

		setEnabled(true);

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
		}

		// Determine the most efficient way to calculate all non bonded atom pairs
		algorithm_type_ = determineMethodOfAtomPairGeneration();

		update();

		return true;
	}


	double MMFF94NonBonded::updateEnergy()
		throw()
	{
		energy_ 		= 0;
		vdw_energy_ = 0;
		es_energy_  = 0;

		for (Position p = 0; p < atom_pair_vector_.size(); p++)
		{
			double d = (atom_pair_vector_[p].first->getPosition() - 
									atom_pair_vector_[p].second->getPosition()).getSquareLength();

			if (Maths::isZero(d)) 
			{
				getForceField()->error() << "Error: Bond with lenght 0!" << std::endl;
				continue;
			}

			d = sqrt(d);

			NonBondedPairData& data = non_bonded_data_[p];

			const double first = data.eij * pow( 1.07 * data.rij / (d + 0.07 * data.rij), 7);

			const double sec = ((1.12 * data.rij_7) / (pow(d, 7) + 0.12 * data.rij_7))  - 2;

			const double e = first * sec * Constants::JOULE_PER_CAL;
			vdw_energy_ += e;

			double es = ES_CONSTANT * data.qi * data.qj / (dc_ * pow(d + 0.05, n_));

			if (data.is_1_4) 
			{
				es *= 0.75;
			}

			es_energy_ += es;


#ifdef BALL_MMFF94_TEST
			data.vdw_energy = e;
			data.es_energy = es;
#endif
#ifdef BALL_MMFF94_DEBUG
			Log.info() << "ES " << atom_pair_vector_[p].first->getName() << " " 
													<< atom_pair_vector_[p].second->getName() << " qi " 
													<< data.qi << " qj " << data.qj 
													<< " Ees " << es << "  d " << d << std::endl;
			Log.info() << "VDW " << atom_pair_vector_[p].first->getName() << " " 
													 << atom_pair_vector_[p].second->getName() << " e " 
													 << data.eij << " r " << data.rij << " Evdw " << e << " d " 
													 << d << std::endl;
#endif
		}

		if (vdw_enabled_) energy_ += vdw_energy_;
		if (es_enabled_)  energy_ += es_energy_;

		return energy_; 
  } 
	

	void MMFF94NonBonded::updateForces()
		throw()
	{
		// constants for VDW forces formular:
		double a7 = pow(1.07,7.0);
		double b = 0.07;
		double c = 1.12;
		double c2 = c * 2.;
		double d = 0.12;
		double d_2_2 = d * d * 2.;
		double k_vdw = -7.0 * a7;
		double cb = c * b;

		for (Position p = 0; p < atom_pair_vector_.size(); p++)
		{
			Atom& a1 = *atom_pair_vector_[p].first;
			Atom& a2 = *atom_pair_vector_[p].second;
			Vector3 direction(a1.getPosition() - a2.getPosition());
			double r = direction.getSquareLength();
			if (Maths::isZero(r)) 
			{
				getForceField()->error() << "Error: Two atoms at exactly the same position! " 
																 << a1.getFullName() << " " << a2.getFullName() << std::endl;
				continue;
			}

			r = sqrt(r);

			const NonBondedPairData& nbd = non_bonded_data_[p];
			direction /= r;
			Vector3 force;

			if (vdw_enabled_)
			{
				// VDW: e * -7 * a^7 * R^7 * (2 * c * R^7 * r^7 +
				//                            c * R^14 * d - 
				//                            2 * r^14 - 
				//                            4 * r^7 * d * R^7 - 
				//                            2 * d^2 * R^14 +
				//                            R^8 * c * r^6 * b)
				//                    /
				//                       ((r + b * R)^8 * (r^7 + d * R^7)^2)
				//
				//    -7 * a^7 = k_vdw
				
				const double R_7 = pow(nbd.rij, 7.);
				const double R_8 = R_7 * nbd.rij;
				const double R_14 = R_7 * R_7;

				const double r_6 = pow(r, 6.);
				const double r_7 = r_6 * r;
				const double r_14 = r_7 * r_7;

				double vdw_factor = nbd.eij * k_vdw * R_7 * (c2 * R_7 * r_7 +
																										 c * R_14 * d -
																										 2. * r_14 -
																										 4. * r_7 * d * R_7 -
																										 d_2_2 * R_14 +
																										 R_8 * r_6 * cb)
																					/
																							(pow(r + b * nbd.rij, 8.) * pow(r_7 + d * R_7, 2.)); 
																							
				force = direction * vdw_factor * FORCES_FACTOR * Constants::JOULE_PER_CAL;

				a1.getForce() += force;
				a2.getForce() -= force;
			}

			if (es_enabled_)
			{
				// ES: -  332.0716 * qi *qj * n / (D * (R + delta )^n *(R + delta))
				double es_factor = FORCES_FACTOR * (ES_CONSTANT * nbd.qi * nbd.qj * n_ / (dc_ * pow(r + 0.05, n_ + 1)));
				force = direction * es_factor;
				// scale 1-4 interactions:
				if (nbd.is_1_4) force *= 0.75;
				a1.getForce() += force;
				a2.getForce() -= force;
			}
		}   
	} 

	double MMFF94NonBonded::getVDWEnergy() const 
	{ 
		if (!vdw_enabled_) return 0;
		return vdw_energy_;
	}

	double MMFF94NonBonded::getESEnergy() const 
	{ 
		if (!es_enabled_) return 0;
		return es_energy_;
	}

	
} // namespace BALL
