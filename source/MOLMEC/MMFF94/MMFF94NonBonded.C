// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MMFF94NonBonded.C,v 1.1.8.6 2007/07/13 22:12:25 amoll Exp $
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
		
		:	ForceFieldComponent(),
			algorithm_type_(MolmecSupport::BRUTE_FORCE),
			cut_off_(20),
			vdw_cut_on_(13),
			vdw_cut_off_(15),
			es_cut_on_(13),
			es_cut_off_(15),
			dc_(1),
			n_(1),
			es_enabled_(true),
			vdw_enabled_(true)
	{	
		setName("MMFF94 NonBonded");
	}

	MMFF94NonBonded::MMFF94NonBonded(ForceField& force_field)
		
		:	ForceFieldComponent(force_field),
			algorithm_type_(MolmecSupport::BRUTE_FORCE),
			cut_off_(20),
			vdw_cut_on_(13),
			vdw_cut_off_(15),
			es_cut_on_(13),
			es_cut_off_(15),
			dc_(1),
			n_(1),
			es_enabled_(true),
			vdw_enabled_(true)
	{
		setName("MMFF94 NonBonded");
	}

	MMFF94NonBonded::MMFF94NonBonded(const MMFF94NonBonded& component)
		
		:	ForceFieldComponent(component),
			algorithm_type_(component.algorithm_type_),
			cut_off_(component.cut_off_),
			vdw_cut_on_(component.vdw_cut_on_),
			vdw_cut_off_(component.vdw_cut_off_),
			es_cut_on_(component.es_cut_on_),
			es_cut_off_(component.es_cut_off_),
			dc_(component.dc_),
			n_(component.n_),
			es_enabled_(component.es_enabled_),
			vdw_enabled_(component.vdw_enabled_)
	{
	}

	MMFF94NonBonded::~MMFF94NonBonded()
		
	{
		clear();
	}

	const MMFF94NonBonded& MMFF94NonBonded::operator = (const MMFF94NonBonded& anb)
		
	{
		// catch self assignment
		if (&anb == this) return *this;

		// call the base class operator
		ForceFieldComponent::operator = (anb);
		algorithm_type_ = anb.algorithm_type_;
		return *this;
	}

	void MMFF94NonBonded::clear()
		
	{
		algorithm_type_ = MolmecSupport::BRUTE_FORCE;
	}


	bool MMFF94NonBonded::operator == (const MMFF94NonBonded& anb)
		
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

			vdw_parameters_.assignParameters(atom1->getType(), atom2->getType(), data.rij, data.rij_7, data.eij);

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

		if (options.has(MMFF94::Option::NONBONDED_CUTOFF))
		{
			cut_off_ = options.getReal(MMFF94::Option::NONBONDED_CUTOFF);
		}
		if (options.has(MMFF94::Option::VDW_CUTOFF))
		{
			vdw_cut_off_ = options.getReal(MMFF94::Option::VDW_CUTOFF);
		}
		if (options.has(MMFF94::Option::VDW_CUTON))
		{
			vdw_cut_on_ = options.getReal(MMFF94::Option::VDW_CUTON);
		}
		if (options.has(MMFF94::Option::ELECTROSTATIC_CUTOFF))
		{
			es_cut_off_ = options.getReal(MMFF94::Option::ELECTROSTATIC_CUTOFF);
		}
		if (options.has(MMFF94::Option::ELECTROSTATIC_CUTON))
		{
			es_cut_on_ = options.getReal(MMFF94::Option::ELECTROSTATIC_CUTON);
		}
		if (options.has(MMFF94::Option::DISTANCE_DEPENDENT_DIELECTRIC))
		{
			bool ddd = options.getBool(MMFF94::Option::DISTANCE_DEPENDENT_DIELECTRIC);
			if (ddd)  n_ = 2;
			else 			n_ = 1;
		}


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

			String  filename(path.find("MMFF94/mmff94.ini"));
			if (filename == "") throw Exception::FileNotFound(__FILE__, __LINE__, "mmff94.ini");
			Parameters p(filename);
			vdw_parameters_.readParameters(p, "VanDerWaals");
		}

		// Determine the most efficient way to calculate all non bonded atom pairs
		algorithm_type_ = determineMethodOfAtomPairGeneration();

		if (es_enabled_ && es_cut_off_ > 0)
		{
			bool ok = es_cut_off_ > es_cut_on_ &&
								cut_off_ >= es_cut_off_;
			if (!ok ) 
			{
				Log.error() << "Invalid ES cuton/cutoff values!" << std::endl;
				Log.error() << "ES cuton: "<< es_cut_on_ << " off: " << es_cut_off_ 
					          << " NB cut " << cut_off_ << std::endl;
			}
			else
			{
				es_d_on_ = es_cut_on_ + 0.05;
				es_d_on2_ = es_d_on_ * es_d_on_;
				es_d_off_ = es_cut_off_ + 0.05;
				es_d_off2_ = es_d_off_ * es_d_off_;
				es_denom_ = pow((es_d_off2_ - es_d_on2_), 3);
				es_cover3_ = -(es_d_on2_ + es_d_off2_) / es_denom_;
				es_ac_ = es_d_off2_ *es_d_off2_ * (es_d_off2_ - 3. * es_d_on2_ ) 
					       / es_denom_;
				es_bc_ = 6. * es_d_on2_ * es_d_off2_ / es_denom_;
				es_cc_ = 3. * es_cover3_;
				es_dc_ = 2. / es_denom_;
				es_dover5_ = es_dc_ / 5.;
				es_eadd_ = (es_d_off2_ * es_d_on2_ * (es_d_off_ - es_d_on_) -
									 (pow(es_d_off_, 5.) - pow(es_d_on_, 5.)) / 5.) * 8. / es_denom_;
				es_const_ = es_bc_ * es_d_off_ - 
										es_ac_ / es_d_off_ +
										es_cover3_ * pow(es_d_off_, 3.) +
										es_dover5_ * pow(es_d_off_, 5.);
				// CONSTR=TWOB*LOG(CTOFM)-ACOEF/C2OFM+CCOEF*C2OFM+OFF4M*DENOM
				es_constr_ = 2. * es_bc_ * log(es_d_off_) - es_ac_ / es_d_off2_ +
					           es_cc_ * es_d_off2_ + pow(es_d_off2_, 2.) / es_denom_;
				// EADDR  = (TWELVE*ONOFF2M*LOG(CTOFM/CTONM) - THREE*(OFF4M-C2ONM*C2ONM))*DENOM
				es_eaddr_ = (12. * es_d_on2_ * es_d_off2_ * log(es_d_off_ / es_d_on_) -
						         3. * (pow(es_d_off2_, 2.) - pow(es_d_on2_, 2.))) / es_denom_;
			}

			enable_es_switch_ = ok;
		}
		else
		{
			enable_es_switch_ = false;
		}

		if (vdw_enabled_ && vdw_cut_off_ > 0)
		{
			bool ok = vdw_cut_off_ > vdw_cut_on_ &&
								cut_off_ >= vdw_cut_off_;
			if (!ok ) 
			{
				Log.error() << "Invalid vdw cuton/cutoff values!" << std::endl;
			}

			enable_vdw_switch_ = ok;
		}
		else
		{
			enable_vdw_switch_ = false;
		}

		update();

		return true;
	}


	double MMFF94NonBonded::updateEnergy()
		
	{
		energy_ 		= 0;
		vdw_energy_ = 0;
		es_energy_  = 0;

 		Options& options = getForceField()->options;
		bool ddd = options.getBool(MMFF94::Option::DISTANCE_DEPENDENT_DIELECTRIC);

		bool use_selection = getForceField()->getUseSelection();

		for (Position p = 0; p < atom_pair_vector_.size(); p++)
		{
			double d = (atom_pair_vector_[p].first->getPosition() - 
									atom_pair_vector_[p].second->getPosition()).getSquareLength();

			if (Maths::isZero(d)) 
			{
				getForceField()->error() << "Error: Two atoms on the same position!" << std::endl;
				continue;
			}

			d = sqrt(d);

			NonBondedPairData& data = non_bonded_data_[p];
			if (use_selection && 
					!atom_pair_vector_[p].first->isSelected() &&
					!atom_pair_vector_[p].second->isSelected())
			{
				continue;
			}

			const double first = data.eij * pow( 1.07 * data.rij / (d + 0.07 * data.rij), 7);

			const double sec = ((1.12 * data.rij_7) / (pow(d, 7) + 0.12 * data.rij_7))  - 2;

			const double e = first * sec * Constants::JOULE_PER_CAL;
			vdw_energy_ += e;

			double es = 0.;

			if (es_enabled_)
			{
				if (enable_es_switch_)
				{
					if (d >= es_cut_off_) continue;

					const double s2d = pow(d + 0.05, 2.);

					if (!ddd)
					{
						// constant dielectric constant
						if (d > es_cut_on_)
						{
							// G1*(R1D*(ACOEF-S2D*(BCOEF+S2D*(COVER3+DOVER5*S2D))) + CONST)
							es =	es_ac_ - s2d * (es_bc_ + s2d *
													(es_cover3_ + es_dover5_ * s2d));
							es /= (d + 0.05);
							es +=	es_const_;
						}
						else
						{
							//EELPR = G1*(R1D+EADD)
							es =  (1. / (d + 0.05)) + es_eadd_;
						}
					}
					else
					{
						// distance dependend dielectric constant
						double r2d = pow(1. / (d + 0.05), 2.);
						// distance dependent dielectric constant
						if (d > es_cut_on_)
						{
              // EELPR = G2*(ACOEF*R2D + TWOB*LOG(R1D) - S2D*(CCOEF + S2D*DENOM) + CONSTR)
							es = es_ac_ * r2d +
								   2. * es_bc_ * log(1. / (d + 0.05)) - 
									 s2d * (es_cc_ + s2d / es_denom_) + es_constr_;
						}
						else
						{
							// EELPR = G2*(R2D + EADDR)
							es = r2d + es_eaddr_;
						}
					}

					es *= ES_CONSTANT * data.qi * data.qj;
				}
				else
				{
					// no switching
					es = ES_CONSTANT * data.qi * data.qj / (dc_ * pow(d + 0.05, n_));
				}

				if (data.is_1_4) es *= 0.75;

				es_energy_ += es;
			}


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
		
	{
		double dbuf = 0.07;
		double gbuf = 0.12;

		bool use_selection = getForceField()->getUseSelection();

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
				const double q = r / nbd.rij;

				const double q6 = pow(q, 6.);
				const double q7 = q6 * q;
				const double rpe = 1. / (q + dbuf);
				const double pe = (1. + dbuf) * rpe;
				const double rp7g = 1. / (q7 + gbuf);
				const double h = (1. + gbuf) * rp7g;
				const double gh = h - 2.;
				const double dgdp = -7. * q6 * h * rp7g;
				const double p2 = pe * pe;
				const double p4 = p2 * p2;
				const double f = nbd.eij * p4 * p2 * pe;
				const double dfdp = -7. * f * rpe;
				const double dgedp = f * dgdp + gh * dfdp;
				const double vdw_factor = dgedp / nbd.rij;
																							
				force = direction * vdw_factor * FORCES_FACTOR * Constants::JOULE_PER_CAL;

				if (!use_selection || a1.isSelected()) a1.getForce() -= force;
				if (!use_selection || a2.isSelected()) a2.getForce() += force;
			}

			if (es_enabled_)
			{
				double es_factor;

				if (enable_es_switch_)
				{
					if (r >= es_cut_off_) continue;

					Options& options = getForceField()->options;
					bool ddd = options.getBool(MMFF94::Option::DISTANCE_DEPENDENT_DIELECTRIC);

					if (!ddd)
					{
						if (r > es_cut_on_)
						{
							const double s2d = pow(r + 0.05, 2.);

							// DF-G1*R1*(ACOEF*R2D+BCOEF+S2D*(CCOEF+DCOEF*S2D))
							es_factor = es_ac_ * pow(1./(r + 0.05), 2.) + 
													es_bc_ +  
													s2d * (es_cc_ + es_dc_ * s2d);
							es_factor *= ES_CONSTANT * nbd.qi * nbd.qj;
						}
						else
						{
							// ES: -  332.0716 * qi *qj * n / (D * (R + delta )^n *(R + delta))
							es_factor = ES_CONSTANT * nbd.qi * nbd.qj * n_ / (dc_ * pow(r + 0.05, n_ + 1));
						}
					}
					else
					{
						double r3d = pow(1. / (r + 0.05), 3.);
						// distance dependent dielectric constant
						if (r > es_cut_on_)
						{
							//DF = DF-G2*R1*(TWOA*R3D+TWOB*R1D+
							//				       TWOC*S1D+TWOD*S3D)
							es_factor = 2. * es_ac_ * r3d + 2. * es_bc_ * (1. / (r + 0.05)) +
													2. * es_cc_ * (r + 0.05) + 2. * es_dc_ * pow(r + 0.05, 3.);
							es_factor *= ES_CONSTANT * nbd.qi * nbd.qj;
						}
						else
						{
							//DF = DF - TWO*G2*R1*R3D
							es_factor = 2. * r3d;
							es_factor *= ES_CONSTANT * nbd.qi * nbd.qj;
						}
					}
		
				} // switching end
				else
				{
					// ES: -  332.0716 * qi *qj * n / (D * (R + delta )^n *(R + delta))
					es_factor = ES_CONSTANT * nbd.qi * nbd.qj * n_ / (dc_ * pow(r + 0.05, n_ + 1));
				}
					
				if (nbd.is_1_4) es_factor *= 0.75;
				force = direction * es_factor * FORCES_FACTOR;
				// scale 1-4 interactions:
				if (!use_selection || a1.isSelected()) a1.getForce() += force;
				if (!use_selection || a2.isSelected()) a2.getForce() -= force;
			} // ES end
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
