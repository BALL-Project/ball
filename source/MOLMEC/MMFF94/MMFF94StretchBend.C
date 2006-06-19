// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MMFF94StretchBend.C,v 1.1.4.7 2006/06/19 14:56:39 amoll Exp $
//

#include <BALL/MOLMEC/MMFF94/MMFF94StretchBend.h>
#include <BALL/MOLMEC/MMFF94/MMFF94.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/KERNEL/PTE.h>

//      #define BALL_DEBUG_MMFF
#define BALL_MMFF_TEST

using namespace std;

namespace BALL 
{

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

	// Constant 
	#define STRETCH_BEND_K0 2.51210 * Constants::JOULE_PER_CAL

	/// 0.043844 / 2
	#define BEND_K0 0.021922 * Constants::JOULE_PER_CAL
	
	// -0.007 degree^-1
	#define BEND_K1 -0.007

	//
  #define BEND_KX 143.9325 * Constants::JOULE_PER_CAL

	// Constant CS
	#define STRETCH_CUBIC_STRENGTH_CONSTANT -2.0

	// Constant 7 / 12 * CS^2
	#define STRETCH_KCS 7.0 / 3.0

	// mdyne * A -> kJ / mol
	// Constant * 4.1868 * 143.9325 / 2 
	#define STRETCH_K0 301.3082955

	#define DEGREE_TO_RADIAN  (Constants::PI / (double)180.0)

	// default constructor
	MMFF94StretchBend::MMFF94StretchBend()
		:	ForceFieldComponent()
	{	
		// set component name
		setName("MMFF94 StretchBend");
	}


	// constructor
	MMFF94StretchBend::MMFF94StretchBend(ForceField& force_field)
		: ForceFieldComponent(force_field)
	{
		// set component name
		setName("MMFF94 StretchBend");
	}


	// copy constructor
	MMFF94StretchBend::MMFF94StretchBend(const MMFF94StretchBend&	component)
		:	ForceFieldComponent(component)
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

		if (!bend_parameters_.isInitialized())
		{
			Path    path;
			String  filename(path.find("MMFF94/MMFFANG.PAR"));
			String  filename1(path.find("MMFF94/MMFFSTBN.PAR"));
			String  filename2(path.find("MMFF94/MMFFDFSB.PAR"));

			if (filename == "") throw Exception::FileNotFound(__FILE__, __LINE__, filename);
			if (filename1 == "") throw Exception::FileNotFound(__FILE__, __LINE__, filename1);
			if (filename2 == "") throw Exception::FileNotFound(__FILE__, __LINE__, filename2);

			const MMFF94AtomTypeEquivalences& equivalences = mmff94_->getEquivalences();
			bend_parameters_.setEquivalences(equivalences);
			bend_parameters_.readParameters(filename);

			sb_parameters_.setEquivalences(equivalences);
			sb_parameters_.readParameters(filename1);
			sb_parameters_.readEmpericalParameters(filename2);
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

					if (use_selection && (!atom1.isSelected() || !atom2.isSelected() || !atom3.isSelected()))
					{
						continue;
					}

					Atom::Type atom_type_a1 = atom1.getType();
					Atom::Type atom_type_a2 = atom2.getType();
					Atom::Type atom_type_a3 = atom3.getType();

					this_bend.ATIJK = getBendType(*it1, *it2, atom1, atom2, atom3);
					this_bend.is_linear = atom_types[atom_type_a2].lin;

					if (bend_parameters_.getParameters(this_bend.ATIJK, 
																			  atom_type_a1, 
																				atom_type_a2, 
																				atom_type_a3, 
																				this_bend.ka, this_bend.theta0))
					{
						// sometimes the ka values are lacking, try the emperical rule
						if (this_bend.ka == 0)
						{
							this_bend.ka = calculateBendEmpericalForceConstant(atom1, atom2, atom3, this_bend.theta0);
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
					this_bend.theta0 = calculateBendEmpericalReferenceAngle(atom1, atom2, atom3);
					this_bend.ka = calculateBendEmpericalForceConstant(atom1, atom2, atom3, this_bend.theta0);

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

			stretch_it = stretch_parameters_->getParameters(atom1.getType(), atom2.getType());
			
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
				stretches_.push_back(dummy_stretch);

				continue;
			}

			// try emperical values
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

			if (!+stretch_it1 || !+stretch_it2)
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
			    !sb_parameters_.getParameters(sb.sbtijk, *a1, *a2, *a3, sb.kba_ijk, sb.kba_kji))
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

		const double degree_to_radian= Constants::PI / (double)180.0;

		for (; bend_it != bends_.end(); ++bend_it) 
		{
			Bend& bend = *bend_it;
			const Vector3& a1 = bend.atom1->getPosition();
			const Vector3& a2 = bend.atom2->getPosition();
			const Vector3& a3 = bend.atom3->getPosition();

			v1.set(a1.x - a2.x, a1.y - a2.y, a1.z - a2.z);
			v2.set(a3.x - a2.x, a3.y - a2.y, a3.z - a2.z);
		
			const volatile double square_length = v1.getSquareLength() * v2.getSquareLength();

			if (Maths::isZero(square_length)) continue;

			const volatile double costheta = v1 * v2 / sqrt(square_length);
			volatile double theta;
			if (costheta > 1.0) 
			{	
				theta = 0.0;
			}
			else if (costheta < -1.0) 
			{
				theta = Constants::PI;
			}
			else 
			{
				theta = acos(costheta);
			}

			// radian to degree
			theta *= 180.;
			theta /= Constants::PI;

			const double& ka = bend_it->ka;
			const double& theta0 = bend_it->theta0;

#ifdef BALL_DEBUG_MMFF
Log.info() << "Bend " << bend.atom1->getName() << " " 
											<< bend.atom2->getName() << " " 
											<< bend.atom3->getName() << " " 
											<< bend.atom1->getType() << " "
											<< bend.atom2->getType() << " "
											<< bend.atom3->getType() << " "
											<< "ATIJK: " << bend_it->ATIJK << "  T: "
											<< theta << "  T0: " << theta0 << " ka " << ka << std::endl;
#endif

			double energy;
			if (bend.is_linear)
			{ 
				energy = BEND_KX * ka * (1.0 + cos(theta * degree_to_radian));

				// we needed the absolute angle, now calculate the delta
				theta -= theta0;
			}
			else
			{
				theta -= theta0;
				energy = BEND_K0 * ka * theta * theta * (1.0 + BEND_K1 * theta);
			}

			bend.delta_theta = theta;
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

		for (Size i = 0; i < stretch_bends_.size(); i++)
		{
			StretchBend& sb = stretch_bends_[i];
			const Bend& bend = bends_[sb.bend_index];
			sb.energy = (double)STRETCH_BEND_K0 * (sb.kba_ijk * stretches_[sb.stretch_i_j].delta_r +
													  				sb.kba_kji * stretches_[sb.stretch_j_k].delta_r) *
										 	   					  bend.delta_theta;
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
		
		Options& options = mmff94_->options;
		if (!options.has(MMFF94::Option::BENDS_ENABLED) || options.getBool(MMFF94::Option::STRETCHES_ENABLED))
		{
			updateBendForces();
		}

		if (!options.has(MMFF94::Option::STRETCHES_ENABLED) || options.getBool(MMFF94::Option::STRETCHES_ENABLED))
		{
			updateStretchForces();
		}

		if (!options.has(MMFF94::Option::STRETCHBENDS_ENABLED) || options.getBool(MMFF94::Option::STRETCHBENDS_ENABLED))
		{
			updateStretchBendForces();
		}
	}

	void MMFF94StretchBend::calculateDeltas_()
	{
		bool use_selection = mmff94_->getUseSelection();

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

			Vector3 v1 = bend.atom1->getPosition() - bend.atom2->getPosition();
			Vector3 v2 = bend.atom3->getPosition() - bend.atom2->getPosition();
			double length = v1.getLength();

			if (length == 0) continue;
			double inverse_length_v1 = 1.0 / length;
			v1 *= inverse_length_v1 ;

			// Calculate the vector between atom3 and atom2,
			// test if the vector has length larger than 0 and normalize it

			length = v2.getLength();
			if (length == 0.0) continue;
			double inverse_length_v2 = 1/length;
			v2 *= inverse_length_v2;

			// Calculate the cos of theta and then theta
			double costheta = v1 * v2;
			double theta;
			if (costheta > 1.0) 
			{
				theta = 0.0;
			}
			else if (costheta < -1.0) 
			{
				theta = Constants::PI;
			}
			else 
			{
				theta = acos(costheta);
			}

			// unit conversion: kJ/(mol A) -> N
			// kJ -> J: 1e3
			// A -> m : 1e10
			// J/mol -> mol: Avogadro
			bend.delta_theta = theta - bend.theta0;
			double factor;
			
			if (!bend.is_linear) 
			{
				factor = -BEND_K0 * bend.ka * (2 * bend.delta_theta * + 3 * BEND_K1 * bend.delta_theta * bend.delta_theta);
			}
			else
			{
				factor = -BEND_KX * bend.ka * sin(bends_[i].theta0 * DEGREE_TO_RADIAN);
			}

			// Calculate the cross product of v1 and v2, test if it has length unequal 0,
			// and normalize it.
			Vector3 cross = v1 % v2;
			if ((length = cross.getLength()) != 0) 
			{
				cross *= (1.0 / length);
			} 
			else 
			{
				bend.n1 = Vector3(0.);
				bend.n2 = Vector3(0.);
				continue;
			}

			bend.n1 = (v1 % cross) * inverse_length_v1 * factor;
			bend.n2 = (v2 % cross) * inverse_length_v2 * factor;
		}

		// stretches:
		for (Size i = 0 ; i < stretches_.size(); i++)
		{
			Vector3 direction(stretches_[i].atom1->getPosition() - stretches_[i].atom2->getPosition());
			const double distance = direction.getLength(); 
			stretches_[i].delta_r = distance - (double) stretches_[i].r0;
			direction /= distance;
			stretches_[i].n = direction;
		}
	}

	double MMFF94StretchBend::updateEnergy()
	{
		if (!mmff94_) return 0;

		energy_ = 0;

		calculateDeltas_();
 
		Options& options = mmff94_->options;
		if (!options.has(MMFF94::Option::BENDS_ENABLED) || options.getBool(MMFF94::Option::BENDS_ENABLED))
		{
			energy_ += updateBendEnergy();
		}

		if (!options.has(MMFF94::Option::STRETCHES_ENABLED) || options.getBool(MMFF94::Option::STRETCHES_ENABLED))
		{
			energy_ += updateStretchEnergy();
		}

		if (!options.has(MMFF94::Option::STRETCHBENDS_ENABLED) || options.getBool(MMFF94::Option::STRETCHBENDS_ENABLED))
		{
			// stretchbends need data, that is calculated in the bend section
			if (options.has(MMFF94::Option::BENDS_ENABLED) && !options.getBool(MMFF94::Option::BENDS_ENABLED))
			{
				updateBendEnergy();
			}

			energy_ += updateStretchBendEnergy();
		}

		return energy_;
	}

	void MMFF94StretchBend::updateStretchBendForces()
	{
		bool use_selection = mmff94_->getUseSelection();

		for (Size i = 0; i < stretch_bends_.size(); i++)
		{
			const StretchBend& sb = stretch_bends_[i];
			const Bend& bend = bends_[sb.bend_index];
			const Stretch& stretch1 = stretches_[sb.stretch_i_j];
			const Stretch& stretch2 = stretches_[sb.stretch_j_k];

			if (use_selection &&
					!bend.atom1->isSelected() &&
					!bend.atom2->isSelected() &&
					!bend.atom3->isSelected())
			{
				continue;
			}

			double bend_factor = (double)STRETCH_BEND_K0 * 
														(sb.kba_ijk * stretches_[sb.stretch_i_j].delta_r +
														 sb.kba_kji * stretches_[sb.stretch_j_k].delta_r);

			double stretch_factor1 = STRETCH_BEND_K0 * sb.kba_ijk;
			double stretch_factor2 = STRETCH_BEND_K0 * sb.kba_kji;
			
			const Vector3 n1 = bend.n1 * bend_factor;
			const Vector3 n2 = bend.n2 * bend_factor;

			if (!use_selection)
			{
				bend.atom1->getForce() -= n1;
				bend.atom2->getForce() += n1;
				bend.atom2->getForce() -= n2;
				bend.atom3->getForce() += n2;
				bend.atom1->getForce()-= stretch1.n * stretch_factor1;
				bend.atom2->getForce()+= stretch1.n * stretch_factor1;
				bend.atom2->getForce()+= stretch2.n * stretch_factor2;
				bend.atom3->getForce()+= stretch2.n * stretch_factor2;
			} 
			else 
			{
				if (bend.atom1->isSelected()) 
				{
					bend.atom1->getForce() -= n1;
					bend.atom1->getForce()-= stretch1.n * stretch_factor1;
				}

				if (bend.atom2->isSelected())
				{
					bend.atom2->getForce() += n1;
					bend.atom2->getForce() -= n2;
					bend.atom2->getForce()+= stretch1.n * stretch_factor1;
					bend.atom2->getForce()+= stretch2.n * stretch_factor2;
				}
				if (bend.atom3->isSelected())
				{
					bend.atom3->getForce() += n2;
					bend.atom3->getForce()+= stretch2.n * stretch_factor2;
				}
			}
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
		bool use_selection = mmff94_->getUseSelection();
		for (Size i = 0; i < bends_.size(); i++) 
		{
			Bend& bend = bends_[i];

			if (!use_selection)
			{
				bend.atom1->getForce() -= bend.n1;
				bend.atom2->getForce() += bend.n1;
				bend.atom2->getForce() -= bend.n2;
				bend.atom3->getForce() += bend.n2;
			} 
			else 
			{
				if (bend.atom1->isSelected()) 
				{
					bend.atom1->getForce() -= bend.n1;
				}

				if (bend.atom2->isSelected())
				{
					bend.atom2->getForce() += bend.n1;
					bend.atom2->getForce() -= bend.n2;
				}
				if (bend.atom3->isSelected())
				{
					bend.atom3->getForce() += bend.n2;
				}
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

	double MMFF94StretchBend::calculateBendEmpericalReferenceAngle(Atom& atom1, Atom& atom2, Atom& atom3) const
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
		
	double MMFF94StretchBend::calculateBendEmpericalForceConstant(Atom& atom1, Atom& atom2, Atom& atom3, double angle_0) const
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
		
		if 			(mmff94_->areInOneRing(atoms, 3))  beta *= 0.05;
		else if (mmff94_->areInOneRing(atoms, 4))  beta *= 0.85;

		double r01 = atom1.getBond(atom2)->getProperty("MMFF94RBL").getDouble();
		double r02 = atom2.getBond(atom3)->getProperty("MMFF94RBL").getDouble();

		double D = pow(r01 - r02, 2) / pow (r01 + r02, 2);

		double k = beta * zcz * pow(angle_0, -2) * exp(-2 * D) / (r01 + r02);

		return k;
	}

	// original values from Paper V
	double MMFF94StretchBend::bend_z_[] = 				{ 1.395, 0. ,   2.494, 2.711, 3.045, 2.847, 2.350, 2.350, 2.980, 2.909, 3.017, 0.,    3.086 };
	double MMFF94StretchBend::bend_c_[] = 				{ 0.,    0.704, 1.016, 1.113, 1.337, 0.,    0.811, 1.068, 1.249, 1.078, 0.,    0.825, 0.  };
	String MMFF94StretchBend::bend_elements_[] = { "H",    "B",   "C",   "N",   "O",   "F",   "Si",  "P",   "S",   "Cl",  "Br", "As",   "I" };

	double MMFF94StretchBend::updateStretchEnergy()
	{
		stretch_energy_ = 0;

		for (Size i = 0 ; i < stretches_.size(); i++)
		{
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
		// iterate over all bonds, update the forces
		for (Size i = 0 ; i < stretches_.size(); i++)
		{
			Stretch& stretch = stretches_[i];
			const double& delta = stretch.delta_r;

			const double a = STRETCH_K0  * stretch.kb * delta;

			const double dd = delta * delta;

			double force = (2 + 
										  3 * STRETCH_CUBIC_STRENGTH_CONSTANT * delta + 
										  4 * STRETCH_KCS * dd) * a;

			const Vector3 direction = stretch.n * force;

			stretch.atom1->getForce()-= direction;
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

		// calculate shrink factor
		double d = 0.008; 

		// for hyrogen atoms no shrinking , found in CHARMM, not in original paper?
//   		if (e1 == 1 || e2 == 1) d = 0.0;

		// for atoms > neon no shrinking , found in CHARMM, not in original paper?
//   		if (e1 > 10 || e2 > 10) d = 0.0;

		// calculate proportionality constant c
		double c = 0.085;

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

		if (stretch_parameters_->getEmpericalParameters().has(ij))
		{
			const MMFF94StretchParameters::EmpericalBondData& bd = stretch_parameters_->getEmpericalParameters()[ij];
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


} // namespace BALL
