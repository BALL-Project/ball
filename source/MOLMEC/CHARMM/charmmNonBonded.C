// $Id: charmmNonBonded.C,v 1.8 2000/03/28 07:53:14 oliver Exp $

#include <BALL/MOLMEC/CHARMM/charmmNonBonded.h>
#include <BALL/MOLMEC/CHARMM/charmm.h>
#include <BALL/MOLMEC/COMMON/forceField.h>
#include <BALL/MOLMEC/COMMON/support.h>
#include <BALL/COMMON/limits.h>
#include <BALL/KERNEL/PSE.h>


// define square function
#define SQR(a) ((a)*(a))

using namespace std;
using namespace BALL::Constants;
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
		cut_on_electrostatic_ = component.cut_on_electrostatic_;
		cut_off_solvation_ = component.cut_off_solvation_;
		cut_on_solvation_ = component.cut_on_solvation_;
		cut_off_vdw_ = component.cut_off_vdw_;
		cut_on_vdw_ = component.cut_on_vdw_;
		inverse_difference_off_on_vdw_3_ = component.inverse_difference_off_on_vdw_3_;
		inverse_difference_off_on_solvation_3_ = component.inverse_difference_off_on_solvation_3_;
		inverse_difference_off_on_electrostatic_3_ = component.inverse_difference_off_on_electrostatic_3_;
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
 
		// create a shorthand for the options
		Options& options = getForceField()->options;

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
				cut_off_ = options.setDefaultReal(CharmmFF::Option::NONBONDED_CUTOFF, cut_off_);
			}

			// the cut on for the switching fct.
			if (van_der_waals_parameters_.options.has("CTONNB"))
			{
				cut_on_vdw_ = van_der_waals_parameters_.options.getReal("CTONNB");
				cut_on_electrostatic_ = cut_on_vdw_;
				cut_on_solvation_ = cut_on_vdw_;

				// user defined values override the parameters from the file
				cut_on_vdw_ = options.setDefaultReal(CharmmFF::Option::VDW_CUTON, cut_on_vdw_);
				cut_on_solvation_ = options.setDefaultReal(CharmmFF::Option::SOLVATION_CUTON, cut_on_solvation_);
				cut_on_electrostatic_ = options.setDefaultReal(CharmmFF::Option::ELECTROSTATIC_CUTON, cut_on_electrostatic_);
			}
			
			// the cut off for the switching fct.
			if (van_der_waals_parameters_.options.has("CTOFNB"))
			{
				cut_off_electrostatic_ = van_der_waals_parameters_.options.getReal("CTOFNB");
				cut_off_vdw_ = cut_off_electrostatic_;
				cut_off_solvation_ = cut_off_electrostatic_;
				
				// user defined values override the parameters from the file
				cut_off_vdw_ = options.setDefaultReal(CharmmFF::Option::VDW_CUTOFF, cut_off_vdw_);
				cut_off_solvation_ = options.setDefaultReal(CharmmFF::Option::SOLVATION_CUTOFF, cut_off_solvation_);
				cut_off_electrostatic_ = options.setDefaultReal(CharmmFF::Option::ELECTROSTATIC_CUTOFF, cut_off_electrostatic_);
			}

			// electrostatic 1-4 scaling factor
			if (van_der_waals_parameters_.options.has("E14FAC"))
			{
				// user defined options override the options from the file
				scaling_electrostatic_1_4_ = van_der_waals_parameters_.options.getReal("E14FAC");
				scaling_electrostatic_1_4_ = options.setDefaultReal(CharmmFF::Option::SCALING_ELECTROSTATIC_1_4, scaling_electrostatic_1_4_);
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
				} 
				else 
				{
					use_dist_depend_dielectric_ = true;
				}

				// user defined options ovverride the options read from the file
				use_dist_depend_dielectric_ = options.setDefaultBool
					(CharmmFF::Option::DISTANCE_DEPENDENT_DIELECTRIC, 
					 use_dist_depend_dielectric_);
			}
		}

		// extract the solvation parameters if possible
		// (not every CHARMM parameter file contains solvation parameters)
		if (!has_initialized_parameters)
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

		// Set the options for the non-bonded atom pairs
		cut_off_ = options.setDefaultReal(CharmmFF::Option::NONBONDED_CUTOFF, CharmmFF::Default::NONBONDED_CUTOFF);

		cut_off_electrostatic_ = options.setDefaultReal(CharmmFF::Option::ELECTROSTATIC_CUTOFF, CharmmFF::Default::ELECTROSTATIC_CUTOFF);
		cut_on_electrostatic_ = options.setDefaultReal(CharmmFF::Option::ELECTROSTATIC_CUTON, CharmmFF::Default::ELECTROSTATIC_CUTON);

		cut_off_vdw_ = options.setDefaultReal(CharmmFF::Option::VDW_CUTOFF, CharmmFF::Default::VDW_CUTOFF);
		cut_on_vdw_ = options.setDefaultReal(CharmmFF::Option::VDW_CUTON, CharmmFF::Default::VDW_CUTON);

		cut_off_solvation_ = options.setDefaultReal(CharmmFF::Option::SOLVATION_CUTOFF, CharmmFF::Default::SOLVATION_CUTOFF);
		cut_on_solvation_ = options.setDefaultReal(CharmmFF::Option::SOLVATION_CUTON, CharmmFF::Default::SOLVATION_CUTON);

		scaling_electrostatic_1_4_ = options.setDefaultReal(CharmmFF::Option::SCALING_ELECTROSTATIC_1_4, CharmmFF::Default::SCALING_ELECTROSTATIC_1_4);
		scaling_vdw_1_4_ = options.setDefaultReal(CharmmFF::Option::SCALING_VDW_1_4,CharmmFF::Default::SCALING_VDW_1_4);

		// set the option for using a constant dielectric constant (default) or
		// or distance dependent one 
		use_dist_depend_dielectric_ = options.setDefaultBool(CharmmFF::Option::DISTANCE_DEPENDENT_DIELECTRIC, CharmmFF::Default::DISTANCE_DEPENDENT_DIELECTRIC);

		// set the option for using the solvation energy
		options.setDefaultBool(CharmmFF::Option::USE_EEF1, CharmmFF::Default::USE_EEF1);
		if (use_solvation_component_
				&& options[CharmmFF::Option::USE_EEF1] == "false")
		{
			// solvation component should be switched off (requested via options)
			use_solvation_component_ = false;
		}

		// consistency check for the cuton/cutoff values
		// and calculation of the inverse cube of their difference 
		// (needed for force update)
		if (cut_off_vdw_ < 0.0)
		{
			Log.warn() << "CharmmNonBonded::setup: vdW cutoff value cannot be negative: " << cut_off_vdw_ 
								 << " (is set to infinite now, switching function disabled)"<< endl;
			cut_off_vdw_ = Limits<float>::max() - 2.0;
			cut_on_vdw_ = cut_off_vdw_ - 1.0;
		}
		if (cut_off_solvation_ < 0.0)
		{
			Log.warn() << "CharmmNonBonded::setup: solvation cutoff value cannot be negative: " << cut_off_solvation_ 
								 << " (is set to infinite now, switching function disabled)"<< endl;
			cut_off_solvation_ = Limits<float>::max() - 2.0;
			cut_on_solvation_ = cut_off_solvation_ - 1.0;
		}
		if (cut_off_electrostatic_ < 0.0)
		{
			Log.warn() << "CharmmNonBonded::setup: electrostatic cutoff value cannot be negative: " << cut_off_electrostatic_ 
								 << " (is set to infinite now, switching function disabled)"<< endl;
			cut_off_electrostatic_ = Limits<float>::max() - 2.0;
			cut_on_electrostatic_ = cut_off_electrostatic_ - 1.0;
		}

		inverse_difference_off_on_vdw_3_ = SQR(cut_off_vdw_) - SQR(cut_on_vdw_);
		inverse_difference_off_on_solvation_3_ = SQR(cut_off_solvation_) - SQR(cut_on_solvation_);
		inverse_difference_off_on_electrostatic_3_ = SQR(cut_off_electrostatic_) - SQR(cut_on_electrostatic_);

		inverse_difference_off_on_vdw_3_ *= SQR(inverse_difference_off_on_vdw_3_);
		inverse_difference_off_on_solvation_3_ *= SQR(inverse_difference_off_on_solvation_3_);
		inverse_difference_off_on_electrostatic_3_ *= SQR(inverse_difference_off_on_electrostatic_3_);

		if (inverse_difference_off_on_vdw_3_ <= 0.0)
		{
			Log.warn() << "CharmmNonBonded::setup: vdW cuton value should be smaller than cutoff. Switching function disabled." << endl;
			cut_on_vdw_ = cut_off_vdw_ + 1.0;
		}
		else
		{
			inverse_difference_off_on_vdw_3_ = 1.0 / inverse_difference_off_on_vdw_3_;
		}
		if (inverse_difference_off_on_electrostatic_3_ <= 0.0)
		{
			Log.warn() << "CharmmNonBonded::setup: electrostatic cuton value should be smaller than cutoff. Switching function disabled." << endl;
			cut_on_electrostatic_ = cut_off_electrostatic_ + 1.0;
		}
		else
		{
			inverse_difference_off_on_electrostatic_3_ = 1.0 / inverse_difference_off_on_electrostatic_3_;
		}
		if (inverse_difference_off_on_solvation_3_ <= 0.0)
		{
			Log.warn() << "CharmmNonBonded::setup: solvation cuton value should be smaller than cutoff. Switching function disabled." << endl;
			cut_on_solvation_ = cut_off_solvation_ + 1.0;
		}
		else
		{
			inverse_difference_off_on_solvation_3_ = 1.0 / inverse_difference_off_on_solvation_3_;
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
		 const double& cut_off_electrostatic_2, const double& cut_on_electrostatic_2, 
		 const double& inverse_difference_off_on_electrostatic_3,
		 const double& cut_off_vdw_2, const double& cut_on_vdw_2, 
		 const double& inverse_difference_off_on_vdw_3,
		 const double& cut_off_solvation_2, const double& cut_on_solvation_2, 
		 const double& inverse_difference_off_on_solvation_3,
		 bool use_solvation, 
		 vector<CharmmEEF1::Values>& solvation,
		 bool use_periodic_boundary, 
		 bool use_dist_depend, 
		 double& electrostatic_energy,
		 double& vdw_energy,
		 double& solvation_energy)
	{
		Vector3 difference = it->atom1->getPosition() - it->atom2->getPosition();      

		if (use_periodic_boundary == true)
		{
			// calculate the minimum image if a periodic boundary is used
			CHARMMcalculateMinimumImage(difference, period, half_period); 
		}
	 
		// the squared distance between the two atoms 
		double distance_2 = difference.getSquareLength();

		if (distance_2 > 0.0)
		{
			double inverse_distance_2 = 1.0 / distance_2;
			if (distance_2 <= cut_off_electrostatic_2) 
			{
				// differentiate between constant dielectric and distance dependent
				double tmp_energy = it->atom1->getCharge() * it->atom2->getCharge();
				if (use_dist_depend)
				{
					// use distance dependent  dielectric 
					tmp_energy *= inverse_distance_2; 
				} 
				else 
				{
					// use constant dielectric constant  
					tmp_energy *= sqrt(inverse_distance_2);
				}

				// check for the switching function	
				if (distance_2 > cut_on_electrostatic_2)
				{
					double difference_off_2 = SQR(cut_off_electrostatic_2 - distance_2);
					tmp_energy *= difference_off_2 * (cut_off_electrostatic_2 + 2 * distance_2 - 3 * cut_on_electrostatic_2) 
												* inverse_difference_off_on_electrostatic_3;
				}

				// add the electrostatic contribution of this pair
				electrostatic_energy += tmp_energy;
			}

			// calculate vdw energy
			if (distance_2 <= cut_off_vdw_2) 
			{
				double inverse_distance_6 = inverse_distance_2 * inverse_distance_2 * inverse_distance_2;
				double tmp_energy = inverse_distance_6 * (it->values.A * inverse_distance_6 - it->values.B); 

				// check for the switching function	
				if (distance_2 > cut_on_vdw_2)
				{
					double difference_off_2 = SQR(cut_off_vdw_2 - distance_2);
					tmp_energy *= difference_off_2 * (cut_off_vdw_2 + 2.0 * distance_2 - 3.0 * cut_on_vdw_2) *
													 inverse_difference_off_on_vdw_3;
				}
				vdw_energy += tmp_energy;
			} 

			// Calculate the solvation energy contribution
			if (use_solvation		
					&& (distance_2 <= cut_off_solvation_2)
					&& it->atom1->getElement() !=  PSE[Element::H] 
					&& it->atom2->getElement() !=  PSE[Element::H])
			{
				CharmmEEF1::Values a1 = solvation[it->atom1->getType()];
				CharmmEEF1::Values a2 = solvation[it->atom2->getType()];

				double factor = PI * sqrt(PI) * distance_2;
				double distance = sqrt(distance_2);

				// contribution of atom1
				float factor_exp = (distance - a1.r_min) / a1.sig_w; 
				factor_exp *= factor_exp;

				double tmp_energy =  - 0.5 * a2.V * a1.dG_free * exp(-factor_exp) / (a1.sig_w * factor);

				// contribution of atom2
				factor_exp = (distance - a2.r_min) / a2.sig_w;
				factor_exp *= factor_exp;
				
				tmp_energy -= 0.5 * a1.V * a2.dG_free * exp(-factor_exp) / (a2.sig_w * factor);

				// check for the switching function	
				if (distance_2 > cut_on_solvation_2)
				{
					double difference_off_2 = SQR(cut_off_solvation_2 - distance_2);
					tmp_energy *= difference_off_2 * (cut_off_solvation_2 + 2 * distance_2 - 3 * cut_on_solvation_2) 
												* inverse_difference_off_on_solvation_3;
				}
				
				solvation_energy += tmp_energy;
			}
		}
	} // end  of function calculateVdWAndElectrostaticEnergy() 

	// This  function calculates the  force vector
	// resulting from non-bonded interactions between two atoms 
	BALL_INLINE 
	void CHARMMcalculateVdWAndElectrostaticForce
		(vector<LennardJones::Data>::iterator it, 
		 const double& e_scaling_factor, const double& vdw_scaling_factor, 
		 Vector3& period, Vector3& half_period, 
		 const double& cut_off_electrostatic_2, const double& cut_on_electrostatic_2, 
		 const double& inverse_difference_off_on_electrostatic_3,
		 const double& cut_off_vdw_2, const double& cut_on_vdw_2, 
		 const double& inverse_difference_off_on_vdw_3,
		 const double& cut_off_solvation_2, const double& cut_on_solvation_2, 
		 const double& inverse_difference_off_on_solvation_3,
		 bool use_solvation, 
		 vector<CharmmEEF1::Values>& solvation,
		 bool use_selection,		
		 bool use_periodic_boundary, 
		 bool use_dist_depend)
	{
		// calculate the difference vector between the two atoms
		Vector3 direction = it->atom1->getPosition() - it->atom2->getPosition(); 

		// choose the nearest image if period boundary is enabled 
		if (use_periodic_boundary == true)
		{
			CHARMMcalculateMinimumImage(direction,period,half_period); 
		}

		double distance_2 = direction.getSquareLength(); 
		if (distance_2 > 0.0)
		{
			// calculate the distance, its inverse,
			// and the square thereof
			double distance = sqrt(distance_2);
			double inverse_distance(1.0 / distance);
			double inverse_distance_2 = SQR(inverse_distance);

			// We multiply the normalized direction of the 
			// forces with this factor
			double factor = 0.0;
			
			// calculate the electrostatic energy
			// if the distance is within the ctoff distance
			//
			if (distance_2 <= cut_off_electrostatic_2) 
			{ 
				// distinguish between constant and distance dependent dielectric 
				// distance dependent dielectric:  epsilon = 4 * epsilon_0 * r_ij
				// factor 4 reduces to 2 because of derivative 

				// the following obscure construction
				// saves one if/then:
				// factor evaluates to 1.0 if the distance dependent electrostatics
				// are used and to 0.0 otherwise
				double dist_depend_factor = (double)(use_dist_depend == true);

				// This expression evaluates to either 
				//     inverse_distance * 2.0  (for distance dependent ES)
				//  or 1.0 (for constant ES)
				factor = dist_depend_factor * inverse_distance * 2.0 + (1.0 - dist_depend_factor);

				// now we multiply with the right constants and we are done.
				factor *=  it->atom1->getCharge() * it->atom2->getCharge() * inverse_distance * inverse_distance_2 * e_scaling_factor;
				
				// we have to use the switching function (cuton <= distance <= cutoff)
				if (distance_2 > cut_on_electrostatic_2)
				{
					// the switching function is defined as follows:
					//         (r_{on}^2 - R^2)^2 (r_{off}^2 + 2 R^2 - 3r_{on}^2)
					// sw(R) = --------------------------------------------------
					//                    (r_{off}^2 - r_{on}^2)^3
					// [Brooks et al., J. Comput. Chem., 4:191 (1983)]
					//
					// the derivative has the following form:
					//                      (r_{off}^2 - R^2)(r_{on}^2 - R^2)
					//   d/dR sw(R) = 12 R -----------------------------------
					//                           (r_{off}^2 - r_{on}^2)^3
					// 
					double difference_to_off = cut_off_electrostatic_2 - distance_2;
					double difference_to_on = cut_on_electrostatic_2 - distance_2;

					// First, multiply the current force with the switching function
					factor *= SQR(difference_to_off) 
										* (cut_off_electrostatic_2 + 2.0 * distance_2 - 3.0 * cut_on_electrostatic_2)
										* inverse_difference_off_on_electrostatic_3; 


					// Second, we add the product of the energy and the derivative 
					// of the switching function (the total force is the derivative of 
					// a product of functions)
					// we divide the derivative of the switching function 
					// by distance to save the normalization of the direction vector
					double derivative_of_switch = 12.0 
																				* difference_to_off * difference_to_on
																				* inverse_difference_off_on_electrostatic_3;

					// calculate the electrostatic energy
					// energy has to be negative since we do not calculate the 
					// force, but the derivative of the energy (negative force) above
					dist_depend_factor = inverse_distance * dist_depend_factor + (1.0 - dist_depend_factor);
					double electrostatic_energy = - e_scaling_factor
																				* dist_depend_factor
																				* inverse_distance * it->atom1->getCharge() * it->atom2->getCharge();
					factor += derivative_of_switch * electrostatic_energy;
				}
			}
			
			// calculate the forces caused by the vdw interactions
			// if we are within the VdW cutoff
			if (distance_2 <= cut_off_vdw_2) 
			{
				double inverse_distance_4 = SQR(inverse_distance_2);
				double inverse_distance_8 = SQR(inverse_distance_4);

				// We divide by the distance for efficiency.
				// This saves the normalization of the direction vector
				// conversion from kJ/(mol*A) -> J/m
				//    1e3 (kJ -> J)
				//    1e10 (A -> m)
				//    1/NA (J/mol -> J)
				double tmp = 1e13 / AVOGADRO * inverse_distance_8 * vdw_scaling_factor
											*  (12 * it->values.A * inverse_distance_2 * inverse_distance_4 - 6 * it->values.B); 

				// we have to use the switching function (cuton <= distance <= cutoff)
				if (distance_2 > cut_on_vdw_2)
				{
					// the switching function is the same function as for
					// electrostatics (see above)
					double difference_to_off = cut_off_vdw_2 - distance_2;
					double difference_to_on = cut_on_vdw_2 - distance_2;

					// First, multiply the current force with the switching function
					tmp *= SQR(difference_to_off)
										* (cut_off_vdw_2 + 2.0 * distance_2 - 3.0 * cut_on_vdw_2)
										* inverse_difference_off_on_vdw_3; 

					// Second, we add the product of the energy and the derivative 
					// of the switching function (the total force is the derivative of 
					// a product of functions)
					// we multiply the derivative by a factor of distance
					// to save the normalization of the direction vector
					double derivative_of_switch = 12.0
																				* difference_to_off * difference_to_on
																				* inverse_difference_off_on_vdw_3;
					// calculate the vdW energy
					// and convert it to units of N
					// (values.A and values.B are in units of kJ/mol, distances in units of Angstrom)
					double inverse_distance_6 = inverse_distance_4 * inverse_distance_2;
					double vdw_energy = - 1e13 / NA * vdw_scaling_factor * inverse_distance_6 * (inverse_distance_6 * it->values.A - it->values.B);
					tmp += derivative_of_switch * vdw_energy;

				}

				// add the vdW contributions to the force factor
				factor += tmp;
			}

			// Calculate the forces that are caused by the solvation component
			// ignore all hydrogen atoms (they are not considered in EEF1)
			if (use_solvation 
					&& (distance_2 <= cut_off_solvation_2) 
					&& it->atom1->getElement() !=  PSE[Element::H] 
					&& it->atom2->getElement() !=  PSE[Element::H])
			{
				CharmmEEF1::Values a1 = solvation[it->atom1->getType()];
				CharmmEEF1::Values a2 = solvation[it->atom2->getType()];

				// contribution of atom1
				float factor_exp = (distance - a1.r_min) / a1.sig_w; 
				float factor_exp_2 = SQR(factor_exp);

				double tmp =  - a2.V * a1.dG_free * exp(-factor_exp_2) * ((factor_exp/a1.sig_w) + inverse_distance) / a1.sig_w;

				// contribution of atom2
				factor_exp = (distance - a2.r_min) / a2.sig_w;
				factor_exp_2 = SQR(factor_exp);
		
				tmp +=  - a1.V * a2.dG_free * exp(-factor_exp_2) * ((factor_exp/a2.sig_w) + inverse_distance) / a2.sig_w;


				// units:  conversion from kJ/mol/A -> J/m (N)
				//   AVOGADRO: J/mol -> J
				//   1e3:      kJ -> J
				//   1e10:     Angstrom -> m
				// 
				tmp *= (1e13 / AVOGADRO) / (PI * sqrt(PI) * distance_2 * distance);
				
				// we have to use the switching function (cuton <= distance <= cutoff)
				if (distance_2 > cut_on_solvation_2)
				{
					// the switching function is the same function as for
					// electrostatics (see above)
					double difference_to_off = cut_off_solvation_2 - distance_2;
					double difference_to_on = cut_on_solvation_2 - distance_2;

					// First, multiply the current force with the switching function
					tmp *= difference_to_off * difference_to_off 
										* (cut_off_solvation_2 + 2.0 * distance_2 - 3.0 * cut_on_solvation_2)
										* inverse_difference_off_on_solvation_3; 

					// Second, we add the product of the energy and the derivative 
					// of the switching function (the total force is the derivative of 
					// a product of functions)
					// we divide the derivative of switch by distance
					// to save the normalization of the direction vector
					double derivative_of_switch = 12.0 
																				* difference_to_off * difference_to_on
																				* inverse_difference_off_on_solvation_3;

					// calculate the solvation energy
					double tmp2 = PI * sqrt(PI) * distance_2;
					
					// contribution of atom1
					float factor_exp = (distance - a1.r_min) / a1.sig_w; 
					factor_exp *= factor_exp;
					double solvation_energy = - 0.5 * a2.V * a1.dG_free * exp(-factor_exp) / (a1.sig_w * tmp2);

					// contribution of atom2
					factor_exp = (distance - a2.r_min) / a2.sig_w;
					factor_exp *= factor_exp;
					solvation_energy += - 0.5 * a1.V * a2.dG_free * exp(-factor_exp) / (a2.sig_w * tmp2);

					// convert to units of N (was: kJ/(mol A))
					// derivative of switch has units of 1/Angstrom
					solvation_energy *= - 1e13 / NA;

					tmp += derivative_of_switch * solvation_energy;
				}

				factor += tmp;
			} 

			// now apply the force to the atoms
			Vector3 force = (float)factor * direction; 

			if (!use_selection)
			{
				it->atom1->setForce(it->atom1->getForce() + force);
				it->atom2->setForce(it->atom2->getForce() - force);
			} 
			else 
			{
				if (it->atom1->isSelected()) 
				{
					it->atom1->setForce(it->atom1->getForce() + force);
				}

				if (it->atom2->isSelected()) 
				{
					it->atom2->setForce(it->atom2->getForce() - force);
				}
			}
		}
	} // end of function 	calculateVdWAndElectrostaticForce()



	// define a conventient shorthand for
	// the rather lenghty collection of parameters
	// used in the inline function	
	#define ENERGY_PARAMETERS\
		period,\
		half_period,\
		cut_off_electrostatic_2,\
		cut_on_electrostatic_2,\
		inverse_difference_off_on_electrostatic_3_,\
		cut_off_vdw_2,\
		cut_on_vdw_2,\
		inverse_difference_off_on_vdw_3_,\
		cut_off_solvation_2,\
		cut_on_solvation_2,\
		inverse_difference_off_on_solvation_3_,\
		use_solvation_component_,\
		solvation_
	

	// This method calculates the current energy resulting from non-bonded interactions 
	double CharmmNonBonded::updateEnergy()
	{
		// Calculate squared cut_off values
		double	cut_off_vdw_2 = SQR(cut_off_vdw_);
		double	cut_on_vdw_2 = SQR(cut_on_vdw_);
		double	cut_off_electrostatic_2 = SQR(cut_off_electrostatic_);
		double	cut_on_electrostatic_2 = SQR(cut_on_electrostatic_);
		double	cut_off_solvation_2 = SQR(cut_off_solvation_);
		double	cut_on_solvation_2 = SQR(cut_on_solvation_);
		
		// Define the different components of the non-bonded energy
		double vdw_energy = 0;
		double vdw_energy_1_4 = 0;
		double electrostatic_energy = 0;
		double electrostatic_energy_1_4 = 0;
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
				if (use_selection == false || (use_selection == true && (it->atom1->isSelected() || it->atom2->isSelected()))) 
				{                                                                                          
					CHARMMcalculateVdWAndElectrostaticEnergy
						(it, ENERGY_PARAMETERS, true, true, 
						 electrostatic_energy_1_4, vdw_energy_1_4, solvation_energy_);
				}                                                                                          
			}  

			// evaluate remaining non-bonded pairs (also in the same vector) 
			for (i = 0; it != non_bonded_.end(); ++it, i++)  
			{                                                                                            
				if (use_selection == false || (use_selection == true  && (it->atom1->isSelected() || it->atom2->isSelected()))) 
				{                                                                                          
					CHARMMcalculateVdWAndElectrostaticEnergy
						(it, ENERGY_PARAMETERS, true, true, 
						 electrostatic_energy, vdw_energy, solvation_energy_);
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
				if (use_selection == false || (use_selection == true  && (it->atom1->isSelected() || it->atom2->isSelected()))) 
				{                                                                                          
					CHARMMcalculateVdWAndElectrostaticEnergy
						(it, ENERGY_PARAMETERS, true, false, 
						 electrostatic_energy_1_4, vdw_energy_1_4, solvation_energy_);
				}                                                                                   
			}                                                                                    

			// evaluate remaining non-bonded pairs (also in the same vector) 
			for (i = 0; it != non_bonded_.end(); ++it, i++)  
			{                                                                                    
				if (use_selection == false || (use_selection == true  && (it->atom1->isSelected() || it->atom2->isSelected()))) 
				{                                                                                 
					CHARMMcalculateVdWAndElectrostaticEnergy
						(it, ENERGY_PARAMETERS, true, false, 
						 electrostatic_energy, vdw_energy, solvation_energy_);
				}                                                                                   
			}
		}
		else if (!use_periodic_boundary && use_dist_depend_dielectric_)
		{
			// no periodic boundary enabled but use distance dependent dielectric  
			// first evaluate 1-4 non-bonded pairs 
			for (i = 0, it = non_bonded_.begin(); i < number_of_1_4_; ++it, i++)  
			{                                                                                            
				if (use_selection == false || (use_selection == true  && (it->atom1->isSelected() || it->atom2->isSelected())))
				{                                                                                          
					CHARMMcalculateVdWAndElectrostaticEnergy
						(it, ENERGY_PARAMETERS, false, true, 
						 electrostatic_energy_1_4, vdw_energy_1_4, solvation_energy_);
				}                                                                                          
			}                                                                                          

			// evaluate remaining non-bonded pairs (also in the same vector) 
			for (i = 0; it != non_bonded_.end(); ++it, i++)  
			{                                                                                            
				if (use_selection == false 
						|| (use_selection == true  && (it->atom1->isSelected() || it->atom2->isSelected()))) 
				{                                                                                          
					CHARMMcalculateVdWAndElectrostaticEnergy
						(it, ENERGY_PARAMETERS, false, true, 
						 electrostatic_energy, vdw_energy, solvation_energy_);
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
						(it, ENERGY_PARAMETERS, false, false, 
						 electrostatic_energy_1_4, vdw_energy_1_4, solvation_energy_);
				}                                                                                          
			}                                                                                          

			// evaluate remaining non-bonded pairs (also in the same vector) 
			for (i = 0; it != non_bonded_.end(); ++it, i++)  
			{                                                                                            
				if (use_selection == false 
						|| (use_selection == true  && (it->atom1->isSelected() || it->atom2->isSelected()))) 
				{                                                                                          
					CHARMMcalculateVdWAndElectrostaticEnergy
						(it, ENERGY_PARAMETERS, false, false, 
						 electrostatic_energy, vdw_energy, solvation_energy_);
				}                                                                                          
			}                                                                                          
		}
																																																	 
    // calculate the total energy and its contributions

    // vdw energy already has the right units -
    // just scale the 1-4-energy
    vdw_energy_ = vdw_energy + scaling_vdw_1_4_ * vdw_energy_1_4;

    // we have to scale the electrostatic energy:
    // we omitted 1/(4 PI epsilon) up to now and we have to
    // convert to kJ/mol
    const double electrostatic_factor = NA * e0 * e0 / (4 * PI * VACUUM_PERMITTIVITY * 1000 * 1e-10);
    electrostatic_energy_
        = electrostatic_factor * (electrostatic_energy + scaling_electrostatic_1_4_ * electrostatic_energy_1_4);

    // add up all contributions
    energy_ =  vdw_energy_ + electrostatic_energy_ + solvation_energy_;
 
		return energy_; 
	} // end of CharmmNonBonded::updateEnergy 



	// This method calculates the current forces resulting from Van-der-Waals
	// and electrostatic interactions 

	// define a conventient shorthand for
	// the rather lenghty collection of parameters
	// used in the inline function
	#define FORCE_PARAMETERS\
		period,\
		half_period,\
		cut_off_electrostatic_2,\
		cut_on_electrostatic_2,\
		inverse_difference_off_on_electrostatic_3_,\
		cut_off_vdw_2,\
		cut_on_vdw_2,\
		inverse_difference_off_on_vdw_3_,\
		cut_off_solvation_2,\
		cut_on_solvation_2,\
		inverse_difference_off_on_solvation_3_,\
		use_solvation_component_,\
		solvation_

	void CharmmNonBonded::updateForces()
	{
		// Define variables for the squared cut_offs, the unit factors and so on
		double cut_off_electrostatic_2 = SQR(cut_off_electrostatic_);
		double cut_on_electrostatic_2 = SQR(cut_on_electrostatic_);
		double cut_off_vdw_2 = SQR(cut_off_vdw_);
		double cut_on_vdw_2 = SQR(cut_on_vdw_);
		double cut_off_solvation_2 = SQR(cut_off_solvation_);
		double cut_on_solvation_2 = SQR(cut_on_solvation_);
		
		// e_scaling_factor contains the unit conversions und the constants appearing in
		// Coulomb's law:
		//               1        q1 * e0 * q2 * e0
		//     F = ------------- ------------------
		//         4 PI epsilon0       r * r
		// Conversion factors are 1e-10 for Angstrom -> m
		// and e0 for the proton charge

		const double	e_scaling_factor = e0 * e0 / 
									(4 * PI * VACUUM_PERMITTIVITY * 1e-20); 

		const double e_scaling_factor_1_4 = e_scaling_factor * scaling_electrostatic_1_4_;
		const double vdw_scaling_factor = 1.0;
		double vdw_scaling_factor_1_4 = vdw_scaling_factor * scaling_vdw_1_4_;

		Size i;
		vector<LennardJones::Data>::iterator it;  
		Vector3 period; 
		Vector3 half_period; 

		bool use_periodic_boundary = force_field_->periodic_boundary.isEnabled(); 
		bool use_selection = getForceField()->getUseSelection(); 

		// calculate forces arising from 1-4 interaction pairs
		// and remaining non-bonded interaction pairs
		if (use_periodic_boundary == true && use_dist_depend_dielectric_ == true)
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
						(it, e_scaling_factor_1_4, vdw_scaling_factor_1_4,
						 FORCE_PARAMETERS, use_selection, true, true);
				}
			}

			// now deal with 'real' non-bonded pairs (in the same vector non_bonded_) 
			for (i = 0; it != non_bonded_.end(); i++, ++it) 
			{
				if (use_selection == false 
						|| ( use_selection == true  && ( it->atom1->isSelected() || it->atom2->isSelected())))
				{
					CHARMMcalculateVdWAndElectrostaticForce
						(it, e_scaling_factor, vdw_scaling_factor,
						 FORCE_PARAMETERS, use_selection, true, true);
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
				if (use_selection == false || ( use_selection == true  && ( it->atom1->isSelected() || it->atom2->isSelected())))
				{
					CHARMMcalculateVdWAndElectrostaticForce
						(it, e_scaling_factor_1_4, vdw_scaling_factor_1_4,
						 FORCE_PARAMETERS, use_selection, true, false);
				}
			}

			// now deal with 'real' non-bonded pairs (in the same vector non_bonded_) 
			for (i = 0; it != non_bonded_.end(); i++, ++it) 
			{
				if (use_selection == false || (use_selection == true  && (it->atom1->isSelected() || it->atom2->isSelected())))
				{
					CHARMMcalculateVdWAndElectrostaticForce
						(it, e_scaling_factor, vdw_scaling_factor,
						 FORCE_PARAMETERS, use_selection, true, false);
				}
			}
		}
		else if (use_periodic_boundary == false && use_dist_depend_dielectric_ == true)
		{
			// periodic boundary not enabled; use a distance dependent dielectric constant 

			// first deal with 1-4 non-bonded pairs
			for (i = 0, it = non_bonded_.begin(); i < number_of_1_4_; i++, ++it) 
			{
				if (use_selection == false || ( use_selection == true  && ( it->atom1->isSelected() || it->atom2->isSelected())))
				{
					CHARMMcalculateVdWAndElectrostaticForce
						(it, e_scaling_factor_1_4, vdw_scaling_factor_1_4,
						 FORCE_PARAMETERS, use_selection, false, true);
				}
			}
		 
			// now deal with 'real' non-bonded pairs (in the same vector non_bonded_)
			for (i = 0; it != non_bonded_.end(); i++, ++it) 
			{
				if (use_selection == false || (use_selection == true  && ( it->atom1->isSelected() || it->atom2->isSelected())))
				{
					CHARMMcalculateVdWAndElectrostaticForce
						(it, e_scaling_factor, vdw_scaling_factor,
						 FORCE_PARAMETERS, use_selection, false, true);
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
						(it, e_scaling_factor_1_4, vdw_scaling_factor_1_4,
						 FORCE_PARAMETERS, use_selection, false, false);
				}
			}

			// now deal with 'real' non-bonded pairs (in the same vector non_bonded_)
			for (i = 0; it != non_bonded_.end(); i++, ++it) 
			{
				if (use_selection == false || (use_selection == true  && ( it->atom1->isSelected() || it->atom2->isSelected())))
				{
					CHARMMcalculateVdWAndElectrostaticForce
						(it, e_scaling_factor, vdw_scaling_factor,
						 FORCE_PARAMETERS, use_selection, false, false);
				}
			}
		}

	} // end of method CharmmNonBonded::updateForces()


	double CharmmNonBonded::getElectrostaticEnergy() const
	{
		return electrostatic_energy_;
	}

	double CharmmNonBonded::getVdwEnergy() const
	{
		return vdw_energy_;
	}
 
	double CharmmNonBonded::getSolvationEnergy() const
	{
		return solvation_energy_;
	}

} // namespace BALL
