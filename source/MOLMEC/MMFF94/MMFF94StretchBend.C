// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MMFF94StretchBend.C,v 1.1.8.6 2007/08/05 18:52:46 amoll Exp $
//

#include <BALL/MOLMEC/MMFF94/MMFF94StretchBend.h>
#include <BALL/MOLMEC/MMFF94/MMFF94.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/COMMON/macros.h>
#include <BALL/FORMAT/parameters.h>

// #define BALL_DEBUG_MMFF
// #define BALL_MMFF_TEST

using namespace std;

namespace BALL 
{

	void MMFF94StretchBend::AddDV3_(Vector3& f3, const TVector3<double> d3)
	{
		f3.x += d3.x;
		f3.y += d3.y;
		f3.z += d3.z;
	}

	inline TVector3<double> diffV3(const Vector3& a, const Vector3& b)
	{
		return TVector3<double>(a.x - b.x, a.y - b.y, a.z - b.z);
	}

	MMFF94StretchBend::Bend::Bend()
		: theta0(0),
			delta_theta(0),
			ka(0),
			atom1(0),
			atom2(0),
			atom3(0),
			is_linear(false),
			ATIJK(0),
			energy(0),
			emperical(false)
	{
	}

	const double DEGREE_TO_RADIAN = (Constants::PI / (double)180.0);

	// Constant MDyne * A -> kJ / mol | Conversion DEGREE to radian:
	// =~ 2.51210 * Constants::JOULE_PER_CAL;
 	const double STRETCH_BEND_K0 = Constants::JOULE_PER_CAL * (double)143.9325 * DEGREE_TO_RADIAN;

	/// 0.043844 / 2
	const double BEND_K0 = 0.021922 * Constants::JOULE_PER_CAL;

	/// for forces:
	const double BEND_K0_FORCES = BEND_K0 * (180.0 / Constants::PI);
	
	// -0.007 degree^-1
	#define BEND_K1 -0.007

	// mdyne * A -> kJ / mol
  const double BEND_KX = 143.9325 * Constants::JOULE_PER_CAL;

	// Constant CS
	#define STRETCH_CUBIC_STRENGTH_CONSTANT -2.0

	// Constant 7 / 12 * CS^2
	#define STRETCH_KCS 7.0 / 3.0

	// mdyne * A -> kJ / mol
	// Constant * 4.1868 * 143.9325 / 2 
	const double STRETCH_K0 = Constants::JOULE_PER_CAL * 143.9325 / 2.0; 

	// Conversion from kJ / (mol A) into Newton
	const double FORCES_FACTOR = 1000 * 1E10 / Constants::AVOGADRO;


	// default constructor
	MMFF94StretchBend::MMFF94StretchBend()
		:	ForceFieldComponent(),
			stretch_enabled_(true),
			bend_enabled_(true),
			stretchbend_enabled_(true)
	{	
		// set component name
		setName("MMFF94 StretchBend");
	}


	// constructor
	MMFF94StretchBend::MMFF94StretchBend(ForceField& force_field)
		: ForceFieldComponent(force_field),
			stretch_enabled_(true),
			bend_enabled_(true),
			stretchbend_enabled_(true)
	{
		// set component name
		setName("MMFF94 StretchBend");
	}


	// copy constructor
	MMFF94StretchBend::MMFF94StretchBend(const MMFF94StretchBend&	component)
		:	ForceFieldComponent(component),
			stretch_enabled_(component.stretch_enabled_),
			bend_enabled_(component.bend_enabled_),
			stretchbend_enabled_(component.stretchbend_enabled_)
	{
	}

	// destructor
	MMFF94StretchBend::~MMFF94StretchBend()
	{
	}


	// setup the internal datastructures for the component
	bool MMFF94StretchBend::setup()
		throw(Exception::TooManyErrors)
	{
		if (getForceField() == 0 || getForceField()->getSystem() == 0) 
		{
			Log.error() << "MMFF94StretchBend::setup(): component not bound to force field" << endl;
			return false;
		}

		// obtain the Stretch and Bend data from the MMFF94 force field
		mmff94_ = (MMFF94*)getForceField();

 		Options& options = getForceField()->options;
		bend_enabled_ = !options.has(MMFF94_BENDS_ENABLED) || 
										 options.getBool(MMFF94_BENDS_ENABLED);
		stretch_enabled_ = !options.has(MMFF94_STRETCHES_ENABLED) || 
											 options.getBool(MMFF94_STRETCHES_ENABLED);
		stretchbend_enabled_ = !options.has(MMFF94_STRETCHBENDS_ENABLED) || 
													 options.getBool(MMFF94_STRETCHBENDS_ENABLED);
		bool disabled = !bend_enabled_ && !stretchbend_enabled_ && !stretch_enabled_;

		if (disabled)
		{
			setEnabled(false);
			return true;
		}

		setEnabled(true);


		if (!bend_parameters_.isInitialized())
		{
			Path    path;
			String  filename(path.find("MMFF94/mmff94.ini"));

			if (filename == "") throw Exception::FileNotFound(__FILE__, __LINE__, filename);

			const MMFF94AtomTypeEquivalences& equivalences = mmff94_->getEquivalences();
			bend_parameters_.setEquivalences(equivalences);
			Parameters p(filename);
			bend_parameters_.readParameters(p, "AngleBend");

			sb_parameters_.setEquivalences(equivalences);
			sb_parameters_.readParameters(p, "StretchBend");
			sb_parameters_.readEmpiricalParameters(p, "StretchBendEmpirical");
		}

		bool ok = true;
		// this order is important!
		ok &= setupStretches_();
		ok &= setupBends_();
		ok &= setupStretchBends_();
		return ok;
	}

	bool MMFF94StretchBend::setupBends_()
	{
		bends_.clear();

		const vector<MMFF94AtomType>& atom_types = mmff94_->getAtomTypes();
		bool use_selection = getForceField()->getUseSelection();

		vector<Atom*>::const_iterator	atom_it = mmff94_->getAtoms().begin();
		Atom::BondIterator it1;
		Atom::BondIterator it2;
		for ( ; atom_it != mmff94_->getAtoms().end(); ++atom_it) 
		{
			for (it2 = (*atom_it)->beginBond(); +it2 ; ++it2) 
			{
				if (it2->getType() == Bond::TYPE__HYDROGEN) continue; // Skip H-bonds

				for (it1 = it2, ++it1; +it1 ; ++it1) 
				{
					if (it1->getType() == Bond::TYPE__HYDROGEN) continue; // Skip H-Bonds;

					// a working instance to put the current values in and push it back
					Bend this_bend;

					this_bend.atom1 = it2->getPartner(**atom_it);
					this_bend.atom2 = (*atom_it);
					this_bend.atom3 = it1->getPartner(**atom_it);

					// make sure atom type is lower for atom1 than for atom3
					if (this_bend.atom1->getType() > this_bend.atom3->getType())
					{
						Atom* temp = this_bend.atom1;
						this_bend.atom1 = this_bend.atom3;
						this_bend.atom3 = temp;
					}

					Atom& atom1 = *this_bend.atom1;
					Atom& atom2 = *this_bend.atom2;
					Atom& atom3 = *this_bend.atom3;

					if (use_selection && (!atom1.isSelected() && !atom2.isSelected() && !atom3.isSelected()))
					{
						continue;
					}

					Atom::Type atom_type_a1 = atom1.getType();
					Atom::Type atom_type_a2 = atom2.getType();
					Atom::Type atom_type_a3 = atom3.getType();

					this_bend.ATIJK = getBendType(*it1, *it2, atom1, atom2, atom3);
					this_bend.is_linear = atom_types[atom_type_a2].lin;

					if (bend_parameters_.assignParameters(this_bend.ATIJK, 
																			  atom_type_a1, 
																				atom_type_a2, 
																				atom_type_a3, 
																				this_bend.ka, this_bend.theta0))
					{
						// sometimes the ka values are lacking, try the emperical rule
						if (this_bend.ka == 0)
						{
							this_bend.ka = calculateBendEmpiricalForceConstant(atom1, atom2, atom3, this_bend.theta0);
							this_bend.emperical = true;
						}

						if (this_bend.ka > 0)
						{
							// if we have a valid constant: store the bend parameters and we are done for this bend
							bends_.push_back(this_bend);
							continue;
						}
					}

					// ok we will try the emperical rule
					this_bend.emperical = true;
					this_bend.theta0 = calculateBendEmpiricalReferenceAngle(atom1, atom2, atom3);
					this_bend.ka = calculateBendEmpiricalForceConstant(atom1, atom2, atom3, this_bend.theta0);

					if (this_bend.ka != -1)
					{
						bends_.push_back(this_bend);
						continue;
					}

					// complain if nothing was found
					mmff94_->error() << "MMFF94StretchBend::setup: cannot find bend parameters for atom types:"
												<< atom_type_a1 << "-" << atom_type_a2 << "-" << atom_type_a3 
												<< "bend " << this_bend.ATIJK
												<< " (atoms are: " << atom1.getFullName() << "/" 
												<< atom2.getFullName() << "/" 
												<< atom3.getFullName() << ")" << endl;

					mmff94_->getUnassignedAtoms().insert(&atom1);
					mmff94_->getUnassignedAtoms().insert(&atom2);
					mmff94_->getUnassignedAtoms().insert(&atom3);
				}
			}
		}

		return true;
	}

	bool MMFF94StretchBend::setupStretches_()
	{
		stretches_.clear();
		
		// a working instance to put the current values in and push it back
		Stretch dummy_stretch;

		stretch_parameters_ = &mmff94_->getStretchParameters();
		MMFF94StretchParameters::StretchMap::ConstIterator stretch_it;

		const vector<Bond*>& bonds = mmff94_->getBonds();
		
		vector<Bond*>::const_iterator bond_it = bonds.begin();
		for (; bond_it != bonds.end(); bond_it++)
		{
			Atom& atom1 = *(Atom*)(*bond_it)->getFirstAtom();
			Atom& atom2 = *(Atom*)(*bond_it)->getSecondAtom();

			static MMFF94StretchParameters::BondData data;
			const bool is_sbmb = (**bond_it).hasProperty("MMFF94SBMB");
			dummy_stretch.sbmb = is_sbmb;
			dummy_stretch.atom1 = &atom1; 
			dummy_stretch.atom2 = &atom2;

			if (stretch_parameters_->assignParameters(atom1.getType(), atom2.getType(), data))
			{
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

				(**bond_it).setProperty("MMFF94RBL", (double)dummy_stretch.r0);
				stretches_.push_back(dummy_stretch);

				continue;
			}

			// try empirical values
			double r0 = calculateStretchR0(**bond_it);
			double k  = calculateStretchConstant(**bond_it, r0);

			if (r0 != -1 && k != -1)
			{
				dummy_stretch.r0 = r0;
				dummy_stretch.kb = k;
				dummy_stretch.emperical = true;
				(**bond_it).setProperty("MMFF94RBL", (double) dummy_stretch.r0);
				stretches_.push_back(dummy_stretch);
				continue;
			}

			mmff94_->error() << "cannot find stretch parameters for atom types " 
				<< atom1.getType() << " "
				<< atom2.getType() << " "
				<< " (atoms are: " 
				<< atom1.getFullName(Atom::ADD_VARIANT_EXTENSIONS_AND_ID) << " " 
				<< atom2.getFullName(Atom::ADD_VARIANT_EXTENSIONS_AND_ID) 
				<< ")" << std::endl;

			mmff94_->getUnassignedAtoms().insert(&atom1);
			mmff94_->getUnassignedAtoms().insert(&atom2);
			continue;
		}
		return true;
	}


	bool MMFF94StretchBend::setupStretchBends_()
	{
		stretch_bends_.clear();

		// build up a lookup table for the stretch data
		HashMap<long, Position> stretch_map;
		for (Position stretch_pos = 0; stretch_pos < stretches_.size(); stretch_pos++)
		{
			long index = ((long) stretches_[stretch_pos].atom1) * 
									 ((long) stretches_[stretch_pos].atom2);

			stretch_map[index] = stretch_pos;
		}

		// a working instance to put the current values in and push it back
		StretchBend sb;

		// iterator on the stretch search results
		HashMap<long, Position>::Iterator stretch_it1, stretch_it2;
		
		// iterate over all bends and look for the corresponding bends in the lookup table
		for (Position bend_pos = 0; bend_pos < bends_.size(); bend_pos++)
		{
			// citation from original paper:
			// "Currently, stretch-bend interactions are omitted when the 
			// i-j-k interaction corresponds to a linear bond angle."
			if (bends_[bend_pos].is_linear) continue;

			const Bend& bend = bends_[bend_pos];

			sb.bend_index = bend_pos;

			Atom* a1 = bend.atom1;
			Atom* a2 = bend.atom2;
			Atom* a3 = bend.atom3;

			// find the i->j and j->k stretch
			stretch_it1 = stretch_map.find((long) a1 * (long) a2);
			stretch_it2 = stretch_map.find((long) a2 * (long) a3);

			if (stretch_it1 == stretch_map.end() || stretch_it2 == stretch_map.end())
			{
				errorOccured_("stretch", *a1, *a2, *a3);
				continue;
			}

			sb.stretch_i_j = stretch_it1->second;
			sb.stretch_j_k = stretch_it2->second;

			const Stretch& stretch_i_j = stretches_[sb.stretch_i_j];
			const Stretch& stretch_j_k = stretches_[sb.stretch_j_k];

			sb.sbtijk = calculateSBTIJK(bends_[sb.bend_index].ATIJK, stretch_i_j.sbmb, stretch_j_k.sbmb);

			if (sb.sbtijk == 2 && a1->getType() == a3->getType())
			{
				sb.sbtijk = 1;
			}

			// get kba_ijk and kba_kji
			if (sb.sbtijk == -1 ||
			    !sb_parameters_.assignParameters(sb.sbtijk, *a1, *a2, *a3, sb.kba_ijk, sb.kba_kji))
			{
				errorOccured_("stretch-bend", *a1, *a2, *a3);
				continue;
			}

			// we are done for this bend
			stretch_bends_.push_back(sb);
		}

		return true;
	}


	void MMFF94StretchBend::errorOccured_(const String& string, 
																				const Atom& a1, const Atom& a2, const Atom& a3)
	{
		mmff94_->error() << "MMFF94 StretchBend: Could not find " << string << " data! "
														 << a1.getName() << " " << a2.getName() << " " << a3.getName() << "   "
														 << a1.getType() << " " << a2.getType() << " " << a3.getType() 
														 << std::endl;

		mmff94_->getUnassignedAtoms().insert(&a1);
		mmff94_->getUnassignedAtoms().insert(&a2);
		mmff94_->getUnassignedAtoms().insert(&a3);
	}

	// calculates the current energy of this component
	double MMFF94StretchBend::updateBendEnergy()
	{
		bend_energy_ = 0;

		TVector3<double> v1, v2;
		vector<Bend>::iterator bend_it = bends_.begin();

		const double degree_to_radian = Constants::PI / (double)180.0;
		bool use_selection = mmff94_->getUseSelection();

		for (; bend_it != bends_.end(); ++bend_it) 
		{
			Bend& bend = *bend_it;
			if (use_selection &&
					!bend.atom1->isSelected() &&
					!bend.atom2->isSelected() &&
					!bend.atom3->isSelected())
			{
				continue;
			}

			const double& ka = bend_it->ka;
			const double& theta = bend_it->theta;
			const double& delta_theta = bend_it->delta_theta;

#ifdef BALL_DEBUG_MMFF
Log.info() << "Bend " << bend.atom1->getName() << " " 
											<< bend.atom2->getName() << " " 
											<< bend.atom3->getName() << " " 
											<< bend.atom1->getType() << " "
											<< bend.atom2->getType() << " "
											<< bend.atom3->getType() << " "
											<< "ATIJK: " << bend_it->ATIJK << "  dT: "
											<< delta_theta << " ka " << ka << std::endl;
#endif

			double energy;
			if (bend.is_linear)
			{ 
				energy = BEND_KX * ka * (1.0 + cos(theta * degree_to_radian));
			}
			else
			{
				energy = BEND_K0 * ka * delta_theta * delta_theta * (1.0 + BEND_K1 * delta_theta);
			}

			bend.energy = energy;

#ifdef BALL_DEBUG_MMFF
	Log.info() << "  E: "<< energy << std::endl;
#endif

			bend_energy_ += energy;
		}

		return bend_energy_;
	}


	// calculates the current energy of this component
	double MMFF94StretchBend::updateStretchBendEnergy()
	{
		// initial energy is zero
		stretch_bend_energy_ = 0;
		bool use_selection = mmff94_->getUseSelection();

		for (Size i = 0; i < stretch_bends_.size(); i++)
		{
			StretchBend& sb = stretch_bends_[i];
			const Bend& bend = bends_[sb.bend_index];
			if (use_selection &&
					!bend.atom1->isSelected() &&
					!bend.atom2->isSelected() &&
					!bend.atom3->isSelected())
			{
				continue;
			}

			double sb1 = STRETCH_BEND_K0 * (sb.kba_ijk * stretches_[sb.stretch_i_j].delta_r) * bend.delta_theta;
			double sb2 = STRETCH_BEND_K0 * (sb.kba_kji * stretches_[sb.stretch_j_k].delta_r) * bend.delta_theta;

			sb.energy = sb1 + sb2;

      #ifdef BALL_DEBUG_MMFF
			Log.info() << "MMFF94 SB "  
				<< bend.atom1->getName() << " "
				<< bend.atom2->getName() << " "
				<< bend.atom3->getName() << " "
				<< bend.atom1->getType() << " "
				<< bend.atom2->getType() << " "
				<< bend.atom3->getType() << " : "
				<< sb.kba_ijk << " " 
				<< sb.kba_kji  << " r_ij: " 
				<< stretches_[sb.stretch_i_j].delta_r << " r_ik: " 
				<< stretches_[sb.stretch_j_k].delta_r << " d: " 
				<< bends_[sb.bend_index].delta_theta  << "      " 
				<< sb.energy << std::endl;
      #endif

			stretch_bend_energy_ += sb.energy;
		}

		return stretch_bend_energy_;
	}

	// calculates and adds its forces to the current forces of the force field
	void MMFF94StretchBend::updateForces()
	{
		if (!mmff94_) return;

		calculateDeltas_();
		
		if (bend_enabled_) 				updateBendForces();
		if (stretch_enabled_) 		updateStretchForces();
		if (stretchbend_enabled_) updateStretchBendForces();
	}

	void MMFF94StretchBend::calculateDeltas_()
	{
		bool use_selection = mmff94_->getUseSelection();

		TVector3<double> v1, v2;

		for (Size i = 0; i < bends_.size(); i++) 
		{
			Bend& bend = bends_[i];

			if (use_selection &&
					!bend.atom1->isSelected() &&
					!bend.atom2->isSelected() &&
					!bend.atom3->isSelected())
			{
				continue;
			}

			// Calculate the vector between atom1 and atom2,
			// test if the vector has length larger than 0 and normalize it

			v1 = diffV3(bend.atom1->getPosition(), bend.atom2->getPosition());
			v2 = diffV3(bend.atom3->getPosition(), bend.atom2->getPosition());

			double length1 = v1.getLength();
			double length2 = v2.getLength();

			// test if the vector has length larger than 0 and normalize it
			if (Maths::isZero(length1) || Maths::isZero(length2))
			{
				bend.n1.set(0.);
				bend.n2.set(0.);
				continue;
			}

			// Calculate the normalized bond vectors
			double inverse_length_v1 = 1.0 / length1;
			double inverse_length_v2 = 1.0 / length2;
			v1 *= inverse_length_v1 ;
			v2 *= inverse_length_v2;

			// Calculate the cross product of v1 and v2, test if it has length unequal 0,
			// and normalize it.
			TVector3<double> cross = v1 % v2;
			double length = cross.getLength();
			if (Maths::isZero(length))
			{
				bend.n1.set(0.);
				bend.n2.set(0.);
				continue;
			}

			cross /= length;

			// Calculate the cos of theta and then theta
			double costheta = v1 * v2;
			double theta;
			if (costheta > 1.0) 
			{
				theta = 0.0;
				costheta = 1.;
			}
			else if (costheta < -1.0) 
			{
				theta = Constants::PI;
				costheta = -1;
			}
			else 
			{
				theta = acos(costheta);
			}

			if (bend.is_linear)
			{
				bend.n1 = (v2 - (v1 * costheta)) * inverse_length_v1;
				bend.n2 = (v1 + (v2 * costheta)) * inverse_length_v2;
			}

			// radian to degree
			theta *= 180.;
			theta /= Constants::PI;

			bend.delta_theta = theta - bend.theta0;
			bend.theta = theta;

			if (!bend.is_linear)
			{
				TVector3<double> t1 = v1 % cross;
				t1.normalize();
				TVector3<double> t2 = v2 % cross;
				t2.normalize();

				bend.n1 = -t1 * inverse_length_v1;
				bend.n2 =  t2 * inverse_length_v2;
			}
		}

		// stretches:
		for (Size i = 0 ; i < stretches_.size(); i++)
		{
			// WARNING: since we do not yet know whether the stretch will be needed for
			// 				  an enabled StretchBend, we cannot skip the setup for non-selected
			// 					stretches here!
			/*if (use_selection && !stretches_[i].atom1->isSelected()
								 			  && !stretches_[i].atom2->isSelected())
			{
				continue;
			}
			*/

			Vector3 direction(stretches_[i].atom1->getPosition() - stretches_[i].atom2->getPosition());
			double distance = direction.x * direction.x +
												direction.y * direction.y +
												direction.z * direction.z;
	
			distance = sqrt(distance);
			stretches_[i].delta_r = distance - (double) stretches_[i].r0;
			direction /= distance;
			stretches_[i].n = direction;
		}
	}

	double MMFF94StretchBend::updateEnergy()
	{
		if (!mmff94_) return 0;

		energy_ = 0;
		bend_energy_ = 0;
		stretch_energy_ = 0;
		stretch_bend_energy_ = 0;

		calculateDeltas_();
 
		if (bend_enabled_) 				energy_ += updateBendEnergy();
		if (stretch_enabled_) 		energy_ += updateStretchEnergy();
		if (stretchbend_enabled_) energy_ += updateStretchBendEnergy();

		return energy_;
	}

	void MMFF94StretchBend::updateStretchBendForces()
	{
		bool us = mmff94_->getUseSelection();

		const double SB_CONSTANT = STRETCH_BEND_K0 / DEGREE_TO_RADIAN * FORCES_FACTOR;

		TVector3<double> c1, c2, r1, r3;

		for (Size i = 0; i < stretch_bends_.size(); i++)
		{
			const StretchBend& sb = stretch_bends_[i];
			const Bend& bend = bends_[sb.bend_index];

			if (us && !bend.atom1->isSelected() &&
								!bend.atom2->isSelected() &&
								!bend.atom3->isSelected())
			{
				continue;
			}

			// normalized vectors from center atom to outer atoms
			c1 = diffV3(bend.atom1->getPosition(), bend.atom2->getPosition());
			double l = c1.getLength();
			if (!Maths::isZero(l)) c1 /= l;

			c2 = diffV3(bend.atom3->getPosition(), bend.atom2->getPosition());
			l = c2.getLength();
			if (!Maths::isZero(l)) c2 /= l;
			
			// force equation:
			// outer atoms i + k:
			// c * sb_constant * K0 * delta_theta * FORCES_FACTOR +
			// (constant_ijk * delta_bond_length1 + constant_kji * delta_bond_length2) * K0 * FORCES_FACTOR / DEGREE_TO_RADIAN
			//
			// inner atom:
			// minus sum of the forces on the outer atoms

			const double temp = bend.delta_theta * FORCES_FACTOR * STRETCH_BEND_K0;
			r1 = c1 * sb.kba_ijk * temp;
			r3 = c2 * sb.kba_kji * temp;

			const double d_ij = stretches_[sb.stretch_i_j].delta_r;
 			const double d_jk = stretches_[sb.stretch_j_k].delta_r;

			const double sb_scale = -(sb.kba_ijk * d_ij + sb.kba_kji * d_jk) * SB_CONSTANT;
			r1 += bend.n1 * sb_scale;
			r3 += bend.n2 * sb_scale;
				
			if (!us || bend.atom1->isSelected()) AddDV3_(bend.atom1->getForce(), -r1);
			if (!us || bend.atom2->isSelected()) AddDV3_(bend.atom2->getForce(), (r1 + r3));
			if (!us || bend.atom3->isSelected()) AddDV3_(bend.atom3->getForce(), -r3);
		}
	}

	/* "The stretch-bend types are defined in terms of the constituent bond types BTIJ 
		 and BTJK and the angle type ATIJK as shown below:"

	   Stretch-Bend       Angle           ---- Bond Type ----
					 Type         Type            I-J             J-K
	 -----------------------------------------------------------
						0             0               0               0
						1             1               1               0
						2             1(*)            0               1 		* error in CHARMM docu
						3             2               1               1
						4             4               0               0
						5             3               0               0
						6             5               1               0
						7             5               0               1
						8             6               1               1
						9             7               1               0
					 10             7               0               1
					 11             8 (*)           1               1
	*/
	Index MMFF94StretchBend::calculateSBTIJK(Position angle_type, bool bond_type1, bool bond_type2)
	{
		if (angle_type  == 0 || angle_type == 4) return angle_type;

		if (angle_type == 1)
		{
			if (bond_type1) return 1;
			return 2;
		}

		if (angle_type == 2) return 3;
		if (angle_type == 3) return 5;

		if (angle_type == 5)
		{
			if (bond_type1) return 6;
			else 						return 7;
		}

		if (angle_type == 6) return 8;

		if (angle_type == 7)
		{
			if (bond_type1) return 9;
			else 						return 10;
		}

		if (angle_type == 8) return 11;

		Log.error() << "MMFF94 StretchBend: Could not calculate sbtijk " << angle_type << " " 
								<< bond_type1 << " " << bond_type2 << std::endl;

		return -1;
	}
	
	// calculates and adds its forces to the current forces of the force field
	void MMFF94StretchBend::updateBendForces()
	{
		bool us = mmff94_->getUseSelection();

		TVector3<double> n1, n2;

		for (Size i = 0; i < bends_.size(); i++) 
		{
			Bend& bend = bends_[i];
		
			// calculate the bend force vectors, do it here, since they are also needed by the stretchbends
			double factor;
			
			if (!bend.is_linear) 
			{
				factor = BEND_K0_FORCES * bend.ka * bend.delta_theta * (2. + 3. * BEND_K1 * bend.delta_theta);
			}
			else
			{
				factor = -BEND_KX * bend.ka;
			}
			
			n1 = bend.n1 * factor * FORCES_FACTOR;
			n2 = bend.n2 * factor * FORCES_FACTOR;

			if (!us || bend.atom1->isSelected()) AddDV3_(bend.atom1->getForce(), n1);
			if (!us || bend.atom3->isSelected()) AddDV3_(bend.atom3->getForce(), n2);

			if (!us || bend.atom2->isSelected())
			{
				AddDV3_(bend.atom2->getForce(), -n1);
				AddDV3_(bend.atom2->getForce(), -n2);
			}
		}
	}

	Position MMFF94StretchBend::getBendType(const Bond& bond1, const Bond& bond2,
										 					 		 Atom& atom1, Atom& atom2, Atom& atom3) const
	{
		/* 	The angle-bending parameters employ angle-type indices ATIJK ranging 
		  	between 0 and 8.  Their meanings are as defined below:

				ATIJK           Structural significance
				 ---------------------------------------------------------------------------
					 0            The angle i-j-k is a "normal" bond angle
					 1            Either bond i-j or bond j-k has a bond type of 1
					 2            Both i-j and j-k have bond types of 1; the sum is 2.
					 3            The angle occurs in a three-membered ring
					 4            The angle occurs in a four-membered ring
					 5            Is in a three-membered ring and the sum of the bond types is 1
					 6            Is in a three-membered ring and the sum of the bond types is 2
					 7            Is in a four-membered ring and the sum of the bond types is 1
					 8            Is in a four-membered ring and the sum of the bond types is 2 
		*/

		const vector<HashSet<Atom*> >& all_rings = mmff94_->getRings();

		bool in_ring_of_three = false;
		bool in_ring_of_four  = false;

		/// test the rings in which all atoms occur
		for (Position ring_nr = 0; ring_nr < all_rings.size(); ring_nr++)
		{
			Size size = all_rings[ring_nr].size();
			if (size < 3 || size > 4) continue;

			if (!all_rings[ring_nr].has(&atom1) ||
			    !all_rings[ring_nr].has(&atom2) ||
			    !all_rings[ring_nr].has(&atom3))
			{
				continue;
			}

			if (size == 3) in_ring_of_three = true;
			if (size == 4) in_ring_of_four  = true;
		}

		/// calculate sum of bond_types
		Size sum_bond_types = 0;
		if (bond1.hasProperty("MMFF94SBMB")) sum_bond_types ++;
		if (bond2.hasProperty("MMFF94SBMB")) sum_bond_types ++;

		if (in_ring_of_three)
		{
			Position result = 3;
			if (sum_bond_types != 0) result = 4 + sum_bond_types;
			return result;
		}


		if (in_ring_of_four)
		{
			Position result = 4;
			if (sum_bond_types != 0) result = 6 + sum_bond_types;
			return result;
		}

		return sum_bond_types;
	}

	double MMFF94StretchBend::calculateBendEmpiricalReferenceAngle(Atom& atom1, Atom& atom2, Atom& atom3) const
	{
		const vector<MMFF94AtomType>& atd = mmff94_->getAtomTypes();

		vector<Atom*> atoms;
		atoms.push_back(&atom1);
		atoms.push_back(&atom2);
		atoms.push_back(&atom3);

		if (mmff94_->areInOneRing(atoms, 3)) return 60;
		if (mmff94_->areInOneRing(atoms, 4)) return 90;

		const MMFF94AtomType& aj = atd[atom2.getType()];
		if (aj.crd == 4) return 109.45;
		if (aj.crd == 2)
		{
			if (atom2.getElement().getSymbol() == "O") return 105;
			if (atom2.getElement().getAtomicNumber() > 10) return 95;
			if (aj.lin) return 180;
		}

		if (aj.crd == 3 && 
				aj.val == 3 &&
				aj.mltb == 0)
		{
			if (atom2.getElement().getSymbol() == "N") return 107;
			return 92;
		}
			
		// default value
		return 120;
	}
		
	double MMFF94StretchBend::calculateBendEmpiricalForceConstant(Atom& atom1, Atom& atom2, Atom& atom3, double angle_0) const
	{
		double degree_to_radian= Constants::PI / 180.0;
		angle_0 *= degree_to_radian;

		String el[3];
		el[0] = atom1.getElement().getSymbol();
		el[1] = atom2.getElement().getSymbol();
		el[2] = atom3.getElement().getSymbol();

		// look for the constant for every element
		Index ps[3];
		for (Position p = 0; p < 3; p++)
		{
			ps[p] = -1;
			for (Position i = 0; i < 12; i++)
			{
				if (el[p] == bend_elements_[i])
				{
					ps[p] = i;
					break;
				}
			}

			if (ps[p] == -1) 
			{
				return -1;
			}
		}

		double zcz = bend_z_[ps[0]] * bend_c_[ps[1]] * bend_z_[ps[2]];

		// One Parameter lacking?
		if (zcz == 0.0) 
		{
			return -1;
		}

		vector<Atom*> atoms;
		atoms.push_back(&atom1);
		atoms.push_back(&atom2);
		atoms.push_back(&atom3);

		double beta = 1.75;
	Log.precision(39);	
		if 			(mmff94_->areInOneRing(atoms, 3))  beta *= 0.05;
		else if (mmff94_->areInOneRing(atoms, 4))  beta *= 0.85;

		double r01 = atom1.getBond(atom2)->getProperty("MMFF94RBL").getDouble();
		double r02 = atom2.getBond(atom3)->getProperty("MMFF94RBL").getDouble();

		double rr = r01 + r02;

		double D = pow(r01 - r02, (double)2.) / pow (rr, (double)2.);

		double ex = exp(-2 * D);

		double asq = pow (angle_0, (double)-2.);
		double k = beta * zcz * asq * ex / (rr);

		return k;
	}

	// original values from Paper V
	double MMFF94StretchBend::bend_z_[] = 				{ 1.395, 0. ,   2.494, 2.711, 3.045, 2.847, 2.350, 2.350, 2.980, 2.909, 3.017, 0.,    3.086 };
	double MMFF94StretchBend::bend_c_[] = 				{ 0.,    0.704, 1.016, 1.113, 1.337, 0.,    0.811, 1.068, 1.249, 1.078, 0.,    0.825, 0.  };
	String MMFF94StretchBend::bend_elements_[] = { "H",    "B",   "C",   "N",   "O",   "F",   "Si",  "P",   "S",   "Cl",  "Br", "As",   "I" };

	double MMFF94StretchBend::updateStretchEnergy()
	{
		stretch_energy_ = 0;
		bool use_selection = mmff94_->getUseSelection();

		for (Size i = 0 ; i < stretches_.size(); i++)
		{
			if (use_selection &&
					!stretches_[i].atom1->isSelected() &&
					!stretches_[i].atom2->isSelected())
			{
				continue;
			}

			const Vector3 direction(stretches_[i].atom1->getPosition() - stretches_[i].atom2->getPosition());
			const double delta(stretches_[i].delta_r);
			const double delta_2(delta * delta);

			double eb_ij = (double) STRETCH_K0 * (double) stretches_[i].kb * delta_2 *
				            ((double) 1.0 + (double) STRETCH_CUBIC_STRENGTH_CONSTANT * delta + (double) STRETCH_KCS * delta_2);

#ifdef BALL_DEBUG_MMFF
			Log.info() << stretches_[i].atom1->getFullName() << " -> " 
								 << stretches_[i].atom2->getFullName() 
								 << "   r0: " << stretches_[i].r0
								 << "   D: " << delta << "   E: " << eb_ij << std::endl;
#endif

			stretch_energy_ += eb_ij;
		}

		return stretch_energy_;
	}

	void MMFF94StretchBend::updateStretchForces()
	{
		bool use_selection = getForceField()->getUseSelection();

		// iterate over all bonds, update the forces
		for (Size i = 0 ; i < stretches_.size(); i++)
		{
			Stretch& stretch = stretches_[i];
			double& delta = stretch.delta_r;

			const double a = STRETCH_K0 * stretch.kb * delta;

			const double dd = delta * delta;

			double force = (2 + 
										  3 * STRETCH_CUBIC_STRENGTH_CONSTANT * delta + 
										  4 * STRETCH_KCS * dd) * a;
			
			// unit conversion: kJ/(mol A) -> N: FORCES_FACTOR
			const Vector3 direction = stretch.n * force * FORCES_FACTOR;

			if (!use_selection || stretch.atom1->isSelected()) 
				stretch.atom1->getForce()-= direction;
			if (!use_selection || stretch.atom2->isSelected()) 
				stretch.atom2->getForce()+= direction;
		}      
	}

	// Calculate the reference bond length value using a modified Schomaker-Stevenson rule
	double MMFF94StretchBend::calculateStretchR0(const Bond& bond)
	{
		const Atom& atom1 = *bond.getFirstAtom();
		const Atom& atom2 = *bond.getSecondAtom();

		const Position e1 = atom1.getElement().getAtomicNumber();
		const Position e2 = atom2.getElement().getAtomicNumber();

		const Position t1 = atom1.getType();
		const Position t2 = atom2.getType();

		if(t1 == BALL_ATOM_DEFAULT_TYPE || t2 == BALL_ATOM_DEFAULT_TYPE) {
			return -1.0;
		}

		// currently only supports atoms up to Xenon
		if (e1 > 53 || e2 > 53 ||
				e1 == 0 || e2 == 0) 
		{
			return -1;
		}

		// radii
		double r1 = stretch_parameters_->radii[e1 - 1];
		double r2 = stretch_parameters_->radii[e2 - 1];

		// only for stored radii
		if (r1 == 0.0 || r2 == 0.0)
		{
			return -1;
		}

		// dont correct radii if one atom is a hydrogen!
		if (e1 != 1 && e2 != 1)
		{
			Position bo = bond.getOrder();
			if (bo == Bond::ORDER__UNKNOWN || 
					bo == Bond::ORDER__QUADRUPLE ||
					bo == Bond::ORDER__ANY)
			{
				return -1;
			}

			const vector<MMFF94AtomType>& atom_types = mmff94_->getAtomTypes();

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
				if (mmff94_->areInOneAromaticRing(atoms, 0))
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
			else  // bo == 1
			{
				// calculate hybridization index
				Position h1 = 3;
				Position h2 = 3;

				if (b1 == 1 || b1 == 2) h1 = 2;
				else if (b1 == 3) 			h1 = 1;

				if (b2 == 1 || b2 == 2) h2 = 2;
				else if (b2 == 3) 			h2 = 1;

				if 			(h1 == 1) r1 -= 0.08;
				else if (h1 == 2) r1 -= 0.03;

				if 			(h2 == 1) r2 -= 0.08;
				else if (h2 == 2) r2 -= 0.03;
			}
		}

		// calculate shrink factor
		double d = 0.008; 

		// for hyrogen atoms no shrinking , found in CHARMM, not in original paper?
 		if (e1 == 1 || e2 == 1) d = 0.0;

		// for atoms > neon no shrinking , found in CHARMM, not in original paper?
 		if (e1 > 10 || e2 > 10) d = 0.0;

    //  c and n are constants defined in R.Blom and A. Haaland,
    //  J. Molec. Struc, 1985, 128, 21-27.
		// calculate proportionality constant c
		double c = 0.08; // value from CHARMM implementation!, original value in paper: 0.085

		// for hyrogen atoms
		if (e1 == 1 || e2 == 1) c = 0.05;

		// POWER
		const double n = 1.4;

		const double diff_e = fabs((double)(stretch_parameters_->electronegatives[e1 - 1] - 
																				stretch_parameters_->electronegatives[e2 - 1]));

		// FORMULA 
		const double r0 = r1 + r2 - c * pow(diff_e, n) - d;
		
		return r0;
	}


	// calculate force constant:
	// requisite data is not available, use relationship developed by Badger
	// parameters are those described in: D. L. Herschbach and V. W. Laurie, J. Chem.  Phys. 1961, 35, 458-463.
	double MMFF94StretchBend::calculateStretchConstant(const Bond& bond, double r0)
	{
		const Atom& a1 = *bond.getFirstAtom();
		const Atom& a2 = *bond.getSecondAtom();

		Index ij = getMMFF94Index(a1.getElement().getAtomicNumber(), a2.getElement().getAtomicNumber());

		if (stretch_parameters_->getEmpiricalParameters().has(ij))
		{
			const MMFF94StretchParameters::EmpiricalBondData& bd = stretch_parameters_->getEmpiricalParameters()[ij];
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

		// individual values taken from HERSCHBACH and LAURIE 1961
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

	double MMFF94StretchBend::getStretchEnergy() const 
	{
		if (!stretch_enabled_) return 0;
		return stretch_energy_;
	}

	double MMFF94StretchBend::getBendEnergy() const 
	{ 
		if (!bend_enabled_) return 0;
		return bend_energy_; 
	}

	double MMFF94StretchBend::getStretchBendEnergy() const 
	{ 
		if (!stretchbend_enabled_) return 0;
		return stretch_bend_energy_;
	}


} // namespace BALL
