// $Id: charmmNonBonded.C,v 1.6 2000/02/22 12:33:26 oliver Exp $

#include <BALL/MOLMEC/CHARMM/charmmNonBonded.h>
#include <BALL/MOLMEC/CHARMM/charmm.h>
#include <BALL/MOLMEC/COMMON/forceField.h>
#include <BALL/MOLMEC/COMMON/support.h>
#include <BALL/KERNEL/PSE.h>


using namespace std;

namespace BALL 
{

	// default constructor
	CharmmNonBonded::CharmmNonBonded()
		:	ForceFieldComponent()
	{	
		// set component name
		setName("CHARMM NonBonded");

		setForceField(0);	

	}


	// constructor
	CharmmNonBonded::CharmmNonBonded(ForceField* force_field)
		:	ForceFieldComponent(force_field)
	{
		// set component name
		setName("CHARMM NonBonded");
	}


	// copy constructor
	CharmmNonBonded::CharmmNonBonded(const CharmmNonBonded&	component, bool clone_deep)
		:	ForceFieldComponent(component, clone_deep)
	{
		non_bonded_ = component.non_bonded_;
		number_of_1_4_ = component.number_of_1_4_;
		solvation_ = component.solvation_;

		electrostatic_energy_ = component.electrostatic_energy_;
		vdw_energy_ = component.vdw_energy_;
		solvation_energy_ = component.solvation_energy_;
		algorithm_type_ = component.algorithm_type_;

		cut_off_ = component.cut_off_;
		cut_off_electrostatic_ = component.cut_off_electrostatic_;
		cut_off_vdw_ = component.cut_off_vdw_;
		cut_on_vdw_ = component.cut_on_vdw_;
		scaling_vdw_1_4_ = component.scaling_vdw_1_4_;
		scaling_electrostatic_1_4_ = component.scaling_electrostatic_1_4_;
		use_solvation_component_ = component.use_solvation_component_;
	}

	// destructor
	CharmmNonBonded::~CharmmNonBonded()
	{
	}

	// This function determines the most efficient way to calculate all non-bonded atom pairs
	// that depends on the number of atoms of the system.
	// The function return value 0 if the number of atoms is so small that the brute force all against
	// all comparison is the most efficient way. Otherwise it returns 1. 

	MolmecSupport::PairListAlgorithmType	
		CharmmNonBonded::determineMethodOfAtomPairGeneration()
	{
		MolmecSupport::PairListAlgorithmType algorithm_type = MolmecSupport::HASH_GRID;
		if (force_field_->getAtoms().size() < 200) 
		{ 
			algorithm_type = MolmecSupport::BRUTE_FORCE;
		} 

		return algorithm_type;
	}


	// setup the internal datastructures for the component
	bool CharmmNonBonded::setup()
	{
		if (getForceField() == 0) 
		{
			Log.error() << "CharmmNonBonded::setup(): component not bound to a force field" << endl;
			return false;
		}


		// clear vector of non-bonded atom pairs
		non_bonded_.clear();
 
		// Set the options for the non-bonded atom pairs

		getForceField()->options.setDefaultReal(CharmmFF::Option::NONBONDED_CUTOFF, CharmmFF::Default::NONBONDED_CUTOFF);
		cut_off_ = getForceField()->options.getReal(CharmmFF::Option::NONBONDED_CUTOFF);

		getForceField()->options.setDefaultReal(CharmmFF::Option::ELECTROSTATIC_CUTOFF, CharmmFF::Default::ELECTROSTATIC_CUTOFF);
		cut_off_electrostatic_ = getForceField()->options.getReal(CharmmFF::Option::ELECTROSTATIC_CUTOFF);

		getForceField()->options.setDefaultReal(CharmmFF::Option::VDW_CUTOFF, CharmmFF::Default::VDW_CUTOFF);
		cut_off_vdw_ = getForceField()->options.getReal(CharmmFF::Option::VDW_CUTOFF);

		getForceField()->options.setDefaultReal(CharmmFF::Option::VDW_CUTON, CharmmFF::Default::VDW_CUTON);
		cut_on_vdw_ = getForceField()->options.getReal(CharmmFF::Option::VDW_CUTON);

		getForceField()->options.setDefaultReal(CharmmFF::Option::SCALING_ELECTROSTATIC_1_4, CharmmFF::Default::SCALING_ELECTROSTATIC_1_4);
		scaling_electrostatic_1_4_ = 1 / getForceField()->options.getReal(CharmmFF::Option::SCALING_ELECTROSTATIC_1_4);

		getForceField()->options.setDefaultReal(CharmmFF::Option::SCALING_VDW_1_4,CharmmFF::Default::SCALING_VDW_1_4);
		scaling_vdw_1_4_ = 1 / getForceField()->options.getReal(CharmmFF::Option::SCALING_VDW_1_4);

		// set the option for using a constant dielectric constant (default) or
		// or distance dependent one 
		getForceField()->options.setDefaultBool
			(CharmmFF::Option::DISTANCE_DEPENDENT_DIELECTRIC,
       CharmmFF::Default::DISTANCE_DEPENDENT_DIELECTRIC);

		use_dist_depend_dielectric_ = getForceField()->options.getBool
			(CharmmFF::Option::DISTANCE_DEPENDENT_DIELECTRIC);

		// set the option for using solvation energy or not
		getForceField()->options.setDefaultBool
			(CharmmFF::Option::USE_EEF1, CharmmFF::Default::USE_EEF1);
		use_solvation_component_ = getForceField()->options.getBool
			(CharmmFF::Option::USE_EEF1);

		// extract the Lennard-Jones parameters
		CharmmFF* charmm_force_field = dynamic_cast<CharmmFF*>(force_field_);
		bool has_initialized_parameters = false;
		if ((charmm_force_field != 0) && (charmm_force_field->hasInitializedParameters()))
		{
			has_initialized_parameters = true;
		}
			
		if (!has_initialized_parameters)
		{
			bool result = van_der_waals_parameters_.extractSection(getForceField()->getParameters(), "LennardJones");

			if (result == false) 
			{	
				Log.error() << "CharmmNonBonded::setup: cannot find section LennardJones" << endl;
				return false;
			}

			// read 1-4 lennard jones parameters
			result = van_der_waals_parameters_14_.extractSection(getForceField()->getParameters(), "LennardJones14");

			if (result == false) 
			{	
				Log.error() << "CharmmNonBonded::setup: cannot find section LennardJones14" << endl;
				return false;
			}

			// check for options defined in the nonbonded section
			
			// the cut off for the pair lists
			if (van_der_waals_parameters_.options.has("CUTNB"))
			{    
				cut_off_ = van_der_waals_parameters_.options.getReal("CUTNB");
				getForceField()->options[CharmmFF::Option::NONBONDED_CUTOFF] = cut_off_;
			}

			// the cut on for the switch fct.
			if (van_der_waals_parameters_.options.has("CTONNB"))
			{
				cut_on_vdw_ = van_der_waals_parameters_.options.getReal("CTONNB");
				getForceField()->options[CharmmFF::Option::VDW_CUTOFF] = cut_off_;
			}
			
			// the cut off for the switch fct.
			if (van_der_waals_parameters_.options.has("CTOFNB"))
			{
				cut_off_electrostatic_ = van_der_waals_parameters_.options.getReal("CTOFNB");
				cut_off_vdw_ = cut_off_electrostatic_;
				getForceField()->options[CharmmFF::Option::ELECTROSTATIC_CUTOFF] = cut_off_electrostatic_;
				getForceField()->options[CharmmFF::Option::VDW_CUTOFF] = cut_off_vdw_;
			}

			// electrostatic 1-4 scaling factor
			if (van_der_waals_parameters_.options.has("E14FAC"))
			{
				scaling_electrostatic_1_4_ = van_der_waals_parameters_.options.getReal("E14FAC");
				getForceField()->options[CharmmFF::Option::SCALING_ELECTROSTATIC_1_4] = scaling_electrostatic_1_4_;
			}

			// electrostatic 1-4 scaling factor
			if (van_der_waals_parameters_.options.has("ATOM"))
			{
				// the ATOM option either takes CDIEL or RDIEL as an argument
				// meaning constant DC or distance-dependent DC
				String value = van_der_waals_parameters_.options["ATOM"];
				if ((value != "CDIEL") && (value != "RDIEL"))
				{
					Log.warn() << "CharmmNonBonded::setup: unknown CHARMM argument for ATOM: " << value 
							<< "   - using distance dependent electrostatics." << endl;
				}
				
				if (value == "CDIEL")
				{
					use_dist_depend_dielectric_ = false;
				} else {
					use_dist_depend_dielectric_ = true;
				}

				// store the value back in the options
				getForceField()->options[CharmmFF::Option::DISTANCE_DEPENDENT_DIELECTRIC] = use_dist_depend_dielectric_;
			}
		}

		// extract the solvation parameters if use_solvation_component_ true
		if (!has_initialized_parameters && use_solvation_component_)
		{
			use_solvation_component_ =  getForceField()->getParameters().getParameterFile().hasSection("EEF1Solvation");
			if (use_solvation_component_)
			{
				bool result = solvation_parameters_.extractSection(getForceField()->getParameters(), "EEF1Solvation");

				if (result == false)
				{
					Log.error() << "CharmmNonBonded::setup: cannot setup EEF1 solvation component." << endl;
					return false;
				}
			}
		}

		// Determine the most efficient way to calculate all non bonded atom pairs
		algorithm_type_ = determineMethodOfAtomPairGeneration();

		// Calculate all non bonded atom pairs
		ForceField::PairVector atom_pair_vector;
		Size number_of_non_bonded_interactions = MolmecSupport::calculateNonBondedAtomPairs
																								(atom_pair_vector, getForceField()->getAtoms(), 
																								 getForceField()->periodic_boundary.getBox(),
																								 cut_off_, force_field_->periodic_boundary.isEnabled(), 
																								 algorithm_type_); 

		// Reserve space for non-bonded vector
		non_bonded_.reserve(number_of_non_bonded_interactions + (Size)(number_of_non_bonded_interactions / 5));

		// Build the vector "non_bonded_" with the atom pairs and parameters
		buildVectorOfNonBondedAtomPairs(atom_pair_vector);

		// initialize vector of parameter structures

		if (use_solvation_component_)
		{
			solvation_.resize(getForceField()->getParameters().getAtomTypes().getNumberOfTypes());

			for (Atom::Type i = 1; i < (Atom::Type)getForceField()->getParameters().getAtomTypes().getNumberOfTypes(); i++)
			{
				if (!solvation_parameters_.assignParameters(solvation_[i], i)) 
				{
					Log.warn() << "CharmmNonBonded::setup: no solvation parameters for atom type "
											<< i << " (" << getForceField()->getParameters().getAtomTypes().getTypeName(i) << ")" << endl;
				}
			} 
		}

		return true;
	}


	// Build a vector of non-bonded atom pairs with the vdw parameters 
	// The vector starts with 1-4 interactions
	void CharmmNonBonded::buildVectorOfNonBondedAtomPairs
		(const vector< pair<Atom*, Atom*> >& atom_vector)
	{
		// bool vector for storing torsion information
		vector<bool> is_torsion;
		is_torsion.reserve(atom_vector.size());

		// Iterate over all atom pairs in atom_vector and test if the atoms build a torsion
		vector< pair <Atom*, Atom*> >::const_iterator pair_it = atom_vector.begin();

		for ( ; pair_it != atom_vector.end(); ++pair_it) 
		{
			is_torsion.push_back(pair_it->first->isVicinal(*pair_it->second));
		}

		vector< bool >::iterator bool_it = is_torsion.begin(); 
		LennardJones::Data tmp;
		Atom*	atom1;
		Atom* atom2;
		Atom::Type	type_atom1;
		Atom::Type  type_atom2;

		// Iterate and search torsions, fill the atom pairs that have a torsion in non_bonded_

		for (pair_it = atom_vector.begin(); pair_it != atom_vector.end(); ++pair_it, ++bool_it) 
		{
			if (*bool_it) 
			{
				atom1 = pair_it->first;
				atom2 = pair_it->second;
				type_atom1 = atom1->getType();
				type_atom2 = atom2->getType();
				tmp.atom1 = atom1;
				tmp.atom2 = atom2;

				if (!van_der_waals_parameters_14_.assignParameters(tmp.values, type_atom1, type_atom2))
				{
					Log.info() << "cannot find 1-4 parameter for " << atom1->getTypeName() << "/" << atom2->getTypeName() << endl;
					if (!van_der_waals_parameters_.assignParameters(tmp.values, type_atom1, type_atom2))
					{
						Log.error() << "cannot find vdw parameters for:"
												<< atom1->getTypeName() << "-" << atom2->getTypeName() << endl;
						tmp.values.A = 0;
						tmp.values.B = 0;
					}
				}
				
				// nasty: check for diagonal 1-4 interactions in sex membered rings
				// (PHE/TYR) - here the interaction is omitted
				Residue* res1 = atom1->getAncestor(RTTI::getDefault<Residue>());
				Residue* res2 = atom2->getAncestor(RTTI::getDefault<Residue>());
				if (res1 == res2 && res1 != 0 && res2 != 0
						&& (res1->getName() == "TYR" || res1->getName() == "PHE")
						&& (res2->getName() == "TYR" || res2->getName() == "PHE")
						&& ((atom1->getName() == "CG" && atom2->getName() == "CZ")
								|| (atom1->getName() == "CZ" && atom2->getName() == "CG")
								|| (atom1->getName() == "CD1" && atom2->getName() == "CE2")
								|| (atom1->getName() == "CE2" && atom2->getName() == "CD1")
								|| (atom1->getName() == "CD2" && atom2->getName() == "CE1")
								|| (atom1->getName() == "CE1" && atom2->getName() == "CD2")))
				{
					// OK - we found a PHE/TYR ring diagonal interaction. 
					// set its parameters to zero
					tmp.values.A = 0;
					tmp.values.B = 0;
				}
				

				non_bonded_.push_back(tmp);
			}
		}

		// Determine and set the number of 1-4 interactions (torsions)
		number_of_1_4_ = non_bonded_.size();

		// Iterate and search non torsions, fill them in the vector non_bonded_
		bool_it = is_torsion.begin();
 
		for (pair_it = atom_vector.begin(); pair_it != atom_vector.end(); ++pair_it, ++bool_it) 
		{
			if (!(*bool_it)) 
			{
				atom1 = pair_it->first;
				atom2 = pair_it->second;

				type_atom1 = atom1->getType();
				type_atom2 = atom2->getType();
				tmp.atom1 = atom1;
				tmp.atom2 = atom2;

				if (!van_der_waals_parameters_.assignParameters(tmp.values, type_atom1,type_atom2)) 
				{
					Log.error() << "CharmmNonBonded::setup: cannot find Lennard Jones parameters for:"
						<< getForceField()->getParameters().getAtomTypes().getTypeName(type_atom1) << "-"
						<< getForceField()->getParameters().getAtomTypes().getTypeName(type_atom2) 
						<< " (" << atom1->getFullName() << "-" << atom2->getFullName() << ")" << endl;

					tmp.values.A = 0;
					tmp.values.B = 0;
				}

				non_bonded_.push_back(tmp);
			}
		}
	}


	BALL_INLINE 
	void CHARMMcalculateMinimumImage
		(Vector3& difference, Vector3& period, Vector3& half_period)
	{
		if (difference.x < -half_period.x) 
		{	
			difference.x += period.x;
		}
		else if (difference.x > half_period.x)
		{
			difference.x -= period.x;
		}
	 
	 if (difference.y < -half_period.y) 
		{
			difference.y += period.y;
		}
		else if (difference.y > half_period.y)
		{
			difference.y -= period.y;
		}

		if (difference.z < -half_period.z)
		{
			difference.z += period.z;
		}
		else if (difference.z > half_period.z)
		{
			difference.z -= period.z;
		}
	}


	// This function calculates the energies resulting from Van-der-
	// Waals and electrostatic interactions between a pair of non-bonded
	// atoms.

	BALL_INLINE 
	void CHARMMcalculateVdWAndElectrostaticEnergy
		(vector<LennardJones::Data>::const_iterator it,
		 Vector3& period, Vector3& half_period,
		 float& cut_off_vdw_2, float& cut_off_electrostatic_2,
		 float& vdw_energy, float& electrostatic_energy, 
		 float& cut_on_vdw_2, float& inverse_difference_on_off_3,
		 bool use_periodic_boundary, 
		 bool use_dist_depend, 
		 bool use_solvation, 
		 vector<CharmmEEF1::Values>& solvation,
		 float& solvation_energy)
	{
		Vector3 difference = it->atom1->getPosition() - it->atom2->getPosition();      

		if (use_periodic_boundary == true)
		{
			// calculate the minimum image 
			CHARMMcalculateMinimumImage(difference,period,half_period); 
		}
	 
		// the squared distance between the two atoms 
		float distance_2 = difference.getSquareLength();

		if (distance_2 > 0 && distance_2 <= cut_off_electrostatic_2) 
		{
			float inverse_distance_2 = 1 / distance_2;

			// differentiate between constant dielectric and distance dependent
			float tmp_energy; 

			if (use_dist_depend)
			{
				// use distance dependent  dielectric 
				tmp_energy = (it->atom1->getCharge() * it->atom2->getCharge()) * inverse_distance_2; 
			} else {
				// use constant dielectric constant  
				tmp_energy = (it->atom1->getCharge() * it->atom2->getCharge()) * sqrt(inverse_distance_2);
			}

			electrostatic_energy += tmp_energy;


			// calculate vdw energy
			if (distance_2 <= cut_off_vdw_2) 
			{
				float inverse_distance_6 = inverse_distance_2 * inverse_distance_2 * inverse_distance_2;
				tmp_energy = inverse_distance_6 * (it->values.A * inverse_distance_6 - it->values.B );          

				// the switch function must be used
				if (distance_2 > cut_on_vdw_2)
				{
					float difference_off_2 = (cut_off_vdw_2 - distance_2);
					difference_off_2 *= difference_off_2;
					tmp_energy *= difference_off_2 * (cut_off_vdw_2 + 2 * distance_2 - 3 * cut_on_vdw_2) *
												inverse_difference_on_off_3;
				}
					 
				vdw_energy += tmp_energy;
					
			}
		} 

		// Calculate the solvation energy contribution
		if (use_solvation && it->atom1->getElement() !=  PSE[Element::H] && it->atom2->getElement() !=  PSE[Element::H])
		{
			CharmmEEF1::Values a1 = solvation[it->atom1->getType()];
			CharmmEEF1::Values a2 = solvation[it->atom2->getType()];

			float factor = BALL::Constants::PI * sqrt(BALL::Constants::PI) * distance_2;
			float distance = sqrt(distance_2);

			// contribution of atom1
			float factor_exp = (distance - a1.r_min) / a1.sig_w; 
			factor_exp *= factor_exp;

			solvation_energy -= 0.5 * a2.V * a1.dG_free * exp(-factor_exp) / (a1.sig_w * factor);

			// contribution of atom2
			factor_exp = (distance - a2.r_min) / a2.sig_w;
			factor_exp *= factor_exp;
			
			solvation_energy -= 0.5 * a1.V * a2.dG_free * exp(-factor_exp) / (a2.sig_w * factor);
		}
	} // end  of function calculateVdWAndElectrostaticEnergy() 


	// This  function calculates the  force vector
	// resulting from non-bonded interactions between two atoms 
	BALL_INLINE 
	void CHARMMcalculateVdWAndElectrostaticForce
		(vector<LennardJones::Data>::iterator it, 
		 Vector3& period, Vector3& half_period, 
		 bool use_selection,		
		 const float& e_scaling_factor, const float& vdw_scaling_factor, 
		 const float& cut_off_electrostatic_2, const float& cut_off_vdw_2, 
		 const float& cut_on_vdw_2, const float& inverse_difference_on_off_3,
		 bool use_periodic_boundary, 
		 bool use_dist_depend, 
		 bool use_solvation, vector<CharmmEEF1::Values>& solvation)
	{
		float factor = 0;
		float distance;
		float inverse_distance;
		float inverse_distance_2;

		// calculate the difference vector between the two atoms
		Vector3 direction = it->atom1->getPosition() - it->atom2->getPosition(); 

		// choose the nearest image if period boundary is enabled 
		if (use_periodic_boundary == true)
		{
			CHARMMcalculateMinimumImage(direction,period,half_period); 
		}

		float distance_2 = direction.getSquareLength(); 

		if (distance_2 > 0 && distance_2 <= cut_off_electrostatic_2) 
		{ 
			inverse_distance_2 = 1 / distance_2;
			inverse_distance = sqrt(inverse_distance_2);

			// distinguish between constant and distance dependent dielectric 
			if (use_dist_depend)	
			{
				// distance dependent dielectric:  epsilon = 4 * epsilon_0 * r_ij
				// factor 4 reduces to 2 because of derivative 
				factor = it->atom1->getCharge() * it->atom2->getCharge() * inverse_distance_2 * inverse_distance_2 * e_scaling_factor * 2.0 ;
			} else {
				// constant dielectric 
				factor = it->atom1->getCharge() * it->atom2->getCharge() * inverse_distance_2 * inverse_distance * e_scaling_factor;
			}

			// calculate the forces caused by the vdw interactions
			if (distance_2 <= cut_off_vdw_2) 
			{
				float inverse_distance_4 = inverse_distance_2 * inverse_distance_2;
				float inverse_distance_8 = inverse_distance_4 * inverse_distance_4;
				// conversion from kJ/(mol*A) -> J/m
				//    1e3 (kJ -> J)
				//    1e10 (A -> m)
				//    1/NA (J/mol -> J)
				factor += 1e13 / Constants::AVOGADRO * inverse_distance_8 * 
											 (12 * it->values.A * inverse_distance_2 * inverse_distance_4 - 6 * it->values.B); 

				// the switch function must be used
				if (distance_2 > cut_on_vdw_2)
				{
					float difference_to_off = (cut_off_vdw_2 - distance_2);
					float difference_to_off_2 = difference_to_off * difference_to_off;
					float triple = cut_off_vdw_2 - 2 * distance_2 - 3 * cut_on_vdw_2;
					factor *= difference_to_off_2 * triple * inverse_difference_on_off_3; 

					float derivative_of_switch = inverse_difference_on_off_3 * 2.0 * 
																				(difference_to_off * triple - difference_to_off_2);	

					float inverse_distance_6 = inverse_distance_2 * inverse_distance_4;
					factor += inverse_distance * derivative_of_switch * (1e13 / Constants::AVOGADRO) * inverse_distance_6 *
										(inverse_distance_6 * it->values.A - it->values.B);
				}
			}

			// Calculate the forces that are caused by the solvation component

			if (use_solvation && it->atom1->getElement() !=  PSE[Element::H] && it->atom2->getElement() !=  PSE[Element::H])
			{
				CharmmEEF1::Values a1 = solvation[it->atom1->getType()];
				CharmmEEF1::Values a2 = solvation[it->atom2->getType()];

				distance = sqrt(distance_2);
				inverse_distance = 1/distance;

				// contribution of atom1
				float factor_exp = (distance - a1.r_min) / a1.sig_w; 
				float factor_exp_2 = factor_exp * factor_exp;

				float tmp =  a2.V * a1.dG_free * exp(-factor_exp_2) * ((factor_exp/a1.sig_w) + inverse_distance) / a1.sig_w;

				// contribution of atom2
				factor_exp = (distance - a2.r_min) / a2.sig_w;
				factor_exp_2 = factor_exp * factor_exp;
		
				tmp +=  a1.V * a2.dG_free * exp(-factor_exp_2) * ((factor_exp/a2.sig_w) + inverse_distance) / a2.sig_w;

				// units:  conversion from kJ/mol/A -> J/m (N)
				//   AVOGADRO: J/mol -> J
				//   1e3:      kJ -> J
        //   1e10:     Angstrom -> m
				tmp *= (1e13 / Constants::AVOGADRO);
				
				factor -= tmp / (Constants::PI * sqrt(Constants::PI) * distance_2 * distance);

			}
		} 

		// now apply the force to the atoms
		Vector3 force = factor * direction; 

		if (use_selection == false)
		{
			it->atom1->setForce(it->atom1->getForce() + force);
			it->atom2->setForce(it->atom2->getForce() - force);
		} else {
			if (it->atom1->isSelected()) 
			{
				it->atom1->setForce(it->atom1->getForce() + force);
			}

			if (it->atom2->isSelected()) 
			{
				it->atom2->setForce(it->atom2->getForce() - force);
			}
		}
	} // end of function 	calculateVdWAndElectrostaticForce()




	// This method calculates the current energy resulting from non-bonded interactions 
	float CharmmNonBonded::updateEnergy()
	{
		// Calculate squared cut_off values
		float	cut_off_electrostatic_2 = cut_off_electrostatic_ * cut_off_electrostatic_;
		float	cut_off_vdw_2 = cut_off_vdw_ * cut_off_vdw_;
		float cut_on_vdw_2 = cut_on_vdw_ * cut_on_vdw_;
		float inverse_difference_on_off_3 = ( cut_off_vdw_2 - cut_on_vdw_2);
		inverse_difference_on_off_3 *= ( cut_off_vdw_2 - cut_on_vdw_2) * ( cut_off_vdw_2 - cut_on_vdw_2);
		if (inverse_difference_on_off_3 == 0)
		{
			// BAUSTELLE: cut_off_vdw_ and cut_on_vdw_ are equal
		}
		else
		{
			inverse_difference_on_off_3 = 1/ inverse_difference_on_off_3;
		}


		// Define the different components of the non-bonded energy
		float vdw_energy = 0;
		float vdw_energy_1_4 = 0;
		float electrostatic_energy = 0;
		float electrostatic_energy_1_4 = 0;
		solvation_energy_ = 0;

		Vector3 difference,period,half_period;
		vector<LennardJones::Data>::iterator it; 
		Size i;                                                                                       

		bool use_periodic_boundary = force_field_->periodic_boundary.isEnabled(); 
		bool use_selection = getForceField()->getUseSelection(); 

		// if the solvation component has to be computed, then initialize the solvation energy 
		if (use_solvation_component_)
		{
			vector<Atom*>::const_iterator vector_it = getForceField()->getAtoms().begin();

			for ( ; vector_it != getForceField()->getAtoms().end(); ++vector_it)	
			{
				if (use_selection == false || (use_selection == true  && (*vector_it)->isSelected()))
				{
					solvation_energy_ += solvation_[(*vector_it)->getType()].dG_ref;
				}
			}
		}
		
		
		// calculate energies arising from 1-4 interaction pairs 
		// and remaining non-bonded interaction pairs 
		if (use_periodic_boundary == true && use_dist_depend_dielectric_ == true)
		{
			// Periodic boundary is enabled and use distance dependent dielectric 

			// calculate the box period (half of the box period)
			Box3 box = force_field_->periodic_boundary.getBox();
			period = box.b - box.a;
			half_period = period * 0.5; 

			// first evaluate 1-4 non-bonded pairs 
			for (i = 0, it = non_bonded_.begin(); i < number_of_1_4_; ++it, i++)  
			{                                                                                            
				if(use_selection == false
					 || (use_selection == true && (it->atom1->isSelected() || it->atom2->isSelected()))) 
				{                                                                                          
					CHARMMcalculateVdWAndElectrostaticEnergy
						(it, period, half_period, cut_off_vdw_2, 
						 cut_off_electrostatic_2, vdw_energy_1_4,                                
						 electrostatic_energy_1_4, cut_on_vdw_2, 
						 inverse_difference_on_off_3, true, true, 
						 use_solvation_component_, solvation_, solvation_energy_);
				}                                                                                          
			}  

			// evaluate remaining non-bonded pairs (also in the same vector) 
			for (i = 0; it != non_bonded_.end(); ++it, i++)  
			{                                                                                            
				if(use_selection == false || (use_selection == true  && (it->atom1->isSelected() || it->atom2->isSelected()))) 
				{                                                                                          
					CHARMMcalculateVdWAndElectrostaticEnergy
						(it, period, half_period, cut_off_vdw_2, 
						 cut_off_electrostatic_2,vdw_energy, electrostatic_energy,cut_on_vdw_2,
						 inverse_difference_on_off_3, true, true, 
						 use_solvation_component_, solvation_, solvation_energy_);
				}                                                                                          
			}
		} 
		else if (use_periodic_boundary && !use_dist_depend_dielectric_)
		{
			// Periodic boundary is enabled and use a constant dielectric 
			// calculate the box period (half of the box period)
			Box3 box = force_field_->periodic_boundary.getBox();
			period = box.b - box.a;
			half_period = period * 0.5; 

			// first evaluate 1-4 non-bonded pairs 
			for (i = 0, it = non_bonded_.begin(); i < number_of_1_4_; ++it, i++)  
			{                                                                                            
				if (use_selection == false 
						|| (use_selection == true  && (it->atom1->isSelected() || it->atom2->isSelected()))) 
				{                                                                                          
					CHARMMcalculateVdWAndElectrostaticEnergy
						(it, period,half_period, cut_off_vdw_2, cut_off_electrostatic_2,vdw_energy_1_4,                                
						 electrostatic_energy_1_4, cut_on_vdw_2,
						 inverse_difference_on_off_3, true, false, 
						 use_solvation_component_, solvation_, solvation_energy_);            
				}                                                                                   
			}                                                                                    

			// evaluate remaining non-bonded pairs (also in the same vector) 
			for (i = 0; it != non_bonded_.end(); ++it, i++)  
			{                                                                                    
				if (use_selection == false 
						|| (use_selection == true  && (it->atom1->isSelected() || it->atom2->isSelected()))) 
				{                                                                                 
					CHARMMcalculateVdWAndElectrostaticEnergy
						(it, period,half_period,   
						 cut_off_vdw_2, cut_off_electrostatic_2,vdw_energy,                        
						 electrostatic_energy,cut_on_vdw_2,
						 inverse_difference_on_off_3, true, false, 
						 use_solvation_component_, solvation_, solvation_energy_);
				}                                                                                   
			}
		}
		else if (!use_periodic_boundary && use_dist_depend_dielectric_)
		{
			// no periodic boundary enabled but use distance dependent dielectric  
			// first evaluate 1-4 non-bonded pairs 
			for (i = 0, it = non_bonded_.begin(); i < number_of_1_4_; ++it, i++)  
			{                                                                                            
				if (use_selection == false 
						|| (use_selection == true  && (it->atom1->isSelected() || it->atom2->isSelected())))
				{                                                                                          
					CHARMMcalculateVdWAndElectrostaticEnergy
						(it, period,half_period, cut_off_vdw_2, cut_off_electrostatic_2,vdw_energy_1_4,                                
						 electrostatic_energy_1_4, cut_on_vdw_2, 
						 inverse_difference_on_off_3, false, true, 
						 use_solvation_component_, solvation_, solvation_energy_);
				}                                                                                          
			}                                                                                          

			// evaluate remaining non-bonded pairs (also in the same vector) 
			for (i = 0; it != non_bonded_.end(); ++it, i++)  
			{                                                                                            
				if (use_selection == false 
						|| (use_selection == true  && (it->atom1->isSelected() || it->atom2->isSelected()))) 
				{                                                                                          
					CHARMMcalculateVdWAndElectrostaticEnergy
						(it, period,half_period,   
						 cut_off_vdw_2, cut_off_electrostatic_2,vdw_energy,                                
						 electrostatic_energy,cut_on_vdw_2,
						 inverse_difference_on_off_3, false, true, 
						 use_solvation_component_, solvation_, solvation_energy_);
				}                                                                                          
			}                                                                                          
		}
		else
		{
			// no periodic boundary enabled and use a constant dielectric 

			// first evaluate 1-4 non-bonded pairs 
			for (i = 0, it = non_bonded_.begin(); i < number_of_1_4_; ++it, i++)  
			{                                                                                            
				if (use_selection == false || (use_selection == true  && (it->atom1->isSelected() || it->atom2->isSelected()))) 
				{                                                                                          
					CHARMMcalculateVdWAndElectrostaticEnergy
						(it,period,half_period,   
						 cut_off_vdw_2, cut_off_electrostatic_2,vdw_energy_1_4,                                
						 electrostatic_energy_1_4, cut_on_vdw_2,
						 inverse_difference_on_off_3, false, false, 
						 use_solvation_component_, solvation_, solvation_energy_);
				}                                                                                          
			}                                                                                          

			// evaluate remaining non-bonded pairs (also in the same vector) 
			for (i = 0; it != non_bonded_.end(); ++it, i++)  
			{                                                                                            
				if (use_selection == false 
						|| (use_selection == true  && (it->atom1->isSelected() || it->atom2->isSelected()))) 
				{                                                                                          
					CHARMMcalculateVdWAndElectrostaticEnergy
						(it,period,half_period,   
						 cut_off_vdw_2, cut_off_electrostatic_2,vdw_energy,                                
						 electrostatic_energy, cut_on_vdw_2, 
						 inverse_difference_on_off_3, false, false, 
						 use_solvation_component_, solvation_, solvation_energy_);
				}                                                                                          
			}                                                                                          
		}
																																																	 
		// Finally, calculate the factors to get the right units (kJ/mol)
		using namespace Constants;
		const float vdw_factor = 1.0;

		// differentiate between constant dielectric and distance dependent
		float electrostatic_factor;

		if (use_dist_depend_dielectric_)
		{
			// distance dependent dielectric: epsilon = 4 * epsilon_0 * r_ij
			// r_ij is already incorporated in electrostatic_energy and electrostatic_energy_1_4
			electrostatic_factor = NA * e0 * e0 / (PI * 4 * VACUUM_PERMITTIVITY * 1000 * 1e-10);
		} else {
			// constant dielectric 
			electrostatic_factor = NA * e0 * e0 / (4 * PI * VACUUM_PERMITTIVITY * 1000 * 1e-10);
		}

		// calculate the total energy and its contributions
		vdw_energy_ = vdw_factor * (vdw_energy + scaling_vdw_1_4_ * vdw_energy_1_4);

		electrostatic_energy_ = electrostatic_factor 
							* (electrostatic_energy + scaling_electrostatic_1_4_ * electrostatic_energy_1_4);
		energy_ =  vdw_energy_ + electrostatic_energy_ + solvation_energy_;

		return energy_; 
	} // end of CharmmNonBonded::updateEnergy 



	// This method calculates the current forces resulting from Van-der-Waals
	// and electrostatic interactions 
	void CharmmNonBonded::updateForces()
	{
		// Define variables for the squared cut_offs, the unit factors and so on
		float	cut_off_electrostatic_2 = cut_off_electrostatic_ *cut_off_electrostatic_;
		float	cut_off_vdw_2 = cut_off_vdw_ * cut_off_vdw_;
		float cut_on_vdw_2 = cut_on_vdw_ * cut_on_vdw_;
		float inverse_difference_on_off_3 = (cut_off_vdw_2 - cut_on_vdw_2) * (cut_off_vdw_2 - cut_on_vdw_2);
		inverse_difference_on_off_3 *= (cut_off_vdw_2 - cut_on_vdw_2);
		
		if (inverse_difference_on_off_3 == 0)
		{
			// BAUSTELLE: Division durch 0
		} else{
			inverse_difference_on_off_3 = 1/ inverse_difference_on_off_3;
		}
		
		// e_scaling_factor contains the unit conversions und the constants appearing in
		// Coulomb's law:
		//               1        q1 * e0 * q2 * e0
		//     F = ------------- ------------------
		//         4 PI epsilon0       r * r
		// Conversion factors are 1e-10 for Angstrom -> m
		// and Constants::e0 for the proton charge

		const float	e_scaling_factor = Constants::e0 * Constants::e0 / 
									(4 * Constants::PI * Constants::VACUUM_PERMITTIVITY * 1e-20); 

		const float e_scaling_factor_1_4 = e_scaling_factor * scaling_electrostatic_1_4_;
		const float vdw_scaling_factor = 1.0;
		float vdw_scaling_factor_1_4 = vdw_scaling_factor * scaling_vdw_1_4_;

		Size i;
		vector<LennardJones::Data>::iterator it;  
		Vector3 period; 
		Vector3 half_period; 

		bool use_periodic_boundary = force_field_->periodic_boundary.isEnabled(); 
		bool use_selection = getForceField()->getUseSelection(); 

		// calculate forces arising from 1-4 interaction pairs
		// and remaining non-bonded interaction pairs
		if(use_periodic_boundary == true && use_dist_depend_dielectric_ == true)
		{
			// periodic boundary is enabled; use a distance dependent dielectric constant 
			// Calculate periods and half periods
			Box3 box = force_field_->periodic_boundary.getBox();
			period = box.b - box.a; 
			half_period = period * 0.5; 

			 
			// first deal with 1-4 non-bonded pairs 
			for (i = 0, it = non_bonded_.begin(); i < number_of_1_4_; i++, ++it) 
			{
				if (use_selection == false 
						|| ( use_selection == true  && ( it->atom1->isSelected() || it->atom2->isSelected())))
				{
					CHARMMcalculateVdWAndElectrostaticForce
						(it, period, half_period, use_selection,
						 e_scaling_factor_1_4, vdw_scaling_factor_1_4, cut_off_electrostatic_2, 
						 cut_off_vdw_2, cut_on_vdw_2, inverse_difference_on_off_3,
						 true, true, use_solvation_component_, solvation_);
				}
			}

			// now deal with 'real' non-bonded pairs (in the same vector non_bonded_) 
			for (i = 0; it != non_bonded_.end(); i++, ++it) 
			{
				if (use_selection == false 
						|| ( use_selection == true  && ( it->atom1->isSelected() || it->atom2->isSelected())))
				{
					CHARMMcalculateVdWAndElectrostaticForce
						(it, period, half_period, use_selection,
						 e_scaling_factor, vdw_scaling_factor, cut_off_electrostatic_2, 
						 cut_off_vdw_2, cut_on_vdw_2, inverse_difference_on_off_3,
						 true,true , use_solvation_component_, solvation_);
				}
			}
		}
		else if (use_periodic_boundary && !use_dist_depend_dielectric_)
		{
			// periodic boundary is enabled; use a distance dependent dielectric constant 
			// Calculate periods and half periods
			Box3 box = force_field_->periodic_boundary.getBox();
			period = box.b - box.a; 
			half_period = period * 0.5; 
			
			// first deal with 1-4 non-bonded pairs
			for (i = 0, it = non_bonded_.begin(); i < number_of_1_4_; i++, ++it) 
			{
				if (use_selection == false 
						|| ( use_selection == true  && ( it->atom1->isSelected() || it->atom2->isSelected())))
				{
					CHARMMcalculateVdWAndElectrostaticForce
						(it, period, half_period, use_selection, 
						 e_scaling_factor_1_4, vdw_scaling_factor_1_4, cut_off_electrostatic_2, 
						 cut_off_vdw_2, cut_on_vdw_2, inverse_difference_on_off_3,
						 true, false, use_solvation_component_, solvation_);
				}
			}

			// now deal with 'real' non-bonded pairs (in the same vector non_bonded_) 
			for (i = 0; it != non_bonded_.end(); i++, ++it) 
			{
				if (use_selection == false 
						|| (use_selection == true  && (it->atom1->isSelected() || it->atom2->isSelected())))
				{
					CHARMMcalculateVdWAndElectrostaticForce
						(it, period,half_period, use_selection, 
						 e_scaling_factor, vdw_scaling_factor, cut_off_electrostatic_2, 
						 cut_off_vdw_2, cut_on_vdw_2, inverse_difference_on_off_3,
						 true, false, use_solvation_component_, solvation_);
				}
			}
		}
		else if (use_periodic_boundary == false && use_dist_depend_dielectric_ == true)
		{
			// periodic boundary not enabled; use a distance dependent dielectric constant 

			// first deal with 1-4 non-bonded pairs
			for (i = 0, it = non_bonded_.begin(); i < number_of_1_4_; i++, ++it) 
			{
				if (use_selection == false 
						|| ( use_selection == true  && ( it->atom1->isSelected() || it->atom2->isSelected())))
				{
					CHARMMcalculateVdWAndElectrostaticForce
						(it, period,half_period, use_selection, 
						 e_scaling_factor_1_4, vdw_scaling_factor_1_4, cut_off_electrostatic_2, 
						 cut_off_vdw_2, cut_on_vdw_2, inverse_difference_on_off_3,
						 false, true, use_solvation_component_, solvation_);
				}
			}
		 
			// now deal with 'real' non-bonded pairs (in the same vector non_bonded_)
			for (i = 0; it != non_bonded_.end(); i++, ++it) 
			{
				if (use_selection == false || ( use_selection == true  && ( it->atom1->isSelected() || it->atom2->isSelected())))
				{
					CHARMMcalculateVdWAndElectrostaticForce
						(it, period,half_period, use_selection, 
						 e_scaling_factor, vdw_scaling_factor, cut_off_electrostatic_2, 
						 cut_off_vdw_2, cut_on_vdw_2, inverse_difference_on_off_3,
						 false, true, use_solvation_component_, solvation_);
				}
			}
		}
		else
		{
			// periodic boundary is not enabled; use a constant dielectric 

			// first deal with 1-4 non-bonded pairs
			for (i = 0, it = non_bonded_.begin(); i < number_of_1_4_; i++, it++) 
			{
				if (use_selection == false || ( use_selection == true  && ( it->atom1->isSelected() || it->atom2->isSelected())))
				{
					CHARMMcalculateVdWAndElectrostaticForce
						(it, period,half_period, use_selection, 
						 e_scaling_factor_1_4, vdw_scaling_factor_1_4, cut_off_electrostatic_2, 
						 cut_off_vdw_2, cut_on_vdw_2, inverse_difference_on_off_3,
						 false, false, use_solvation_component_, solvation_);
				}
			}

			// now deal with 'real' non-bonded pairs (in the same vector non_bonded_)
			for (i = 0; it != non_bonded_.end(); i++, ++it) 
			{
				if (use_selection == false 
						|| ( use_selection == true  && ( it->atom1->isSelected() || it->atom2->isSelected())))
				{
					CHARMMcalculateVdWAndElectrostaticForce
						(it, period,half_period, use_selection, 
						 e_scaling_factor, vdw_scaling_factor, cut_off_electrostatic_2, 
						 cut_off_vdw_2, cut_on_vdw_2, inverse_difference_on_off_3,
						 false, false, use_solvation_component_, solvation_);
				}
			}
		}

	} // end of method CharmmNonBonded::updateForces()


	float CharmmNonBonded::getElectrostaticEnergy() const
	{
		return electrostatic_energy_;
	}

	float CharmmNonBonded::getVdwEnergy() const
	{
		return vdw_energy_;
	}
 
	float CharmmNonBonded::getSolvationEnergy() const
	{
		return solvation_energy_;
	}

} // namespace BALL
