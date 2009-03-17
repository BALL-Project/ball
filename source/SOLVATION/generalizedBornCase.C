// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: generalizedBornCase.C,v 1.3.10.1 2007/03/25 22:00:32 oliver Exp $
//

#include <BALL/SOLVATION/generalizedBornCase.h>
// #include "generalizedBornCase.h"

#include <BALL/SYSTEM/path.h>
#include <BALL/SYSTEM/timer.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/FORMAT/parameters.h>
#include <BALL/COMMON/exception.h>
#include <BALL/KERNEL/residue.h>
#include <BALL/KERNEL/bond.h>

using namespace std;

#define UNITED_ATOMS
// #define TSUI true

namespace BALL
{

	const String GeneralizedBornModel::Option::VERBOSITY = "verbosity";
	const String GeneralizedBornModel::Option::LAMBDA = "lambda";
	const String GeneralizedBornModel::Option::DELTA = "delta";
	const String GeneralizedBornModel::Option::KAPPA = "kappa";
	const String GeneralizedBornModel::Option::RHO_0 = "rho_0";
	const String GeneralizedBornModel::Option::DC_SOLVENT = "dc_solvent";
	const String GeneralizedBornModel::Option::DC_SOLUTE = "dc_solute";
	const String GeneralizedBornModel::Option::FILENAME = "filename";

	const Size GeneralizedBornModel::Default::VERBOSITY = 10;
	const float GeneralizedBornModel::Default::LAMBDA = 1.4f;
	const float GeneralizedBornModel::Default::DELTA = 0.15f;
	const float GeneralizedBornModel::Default::KAPPA = 0.1f;
	const float GeneralizedBornModel::Default::RHO_0 = 0.09f;
	const float GeneralizedBornModel::Default::DC_SOLVENT = 80.0f;
	const float GeneralizedBornModel::Default::DC_SOLUTE = 2.0f;
	const String GeneralizedBornModel::Default::FILENAME = "solvation/gb_scaling.ini";

	/*
	const Size GeneralizedBornModel::Default::VERBOSITY = 10;
	const float GeneralizedBornModel::Default::LAMBDA = 1.0f;
	const float GeneralizedBornModel::Default::DELTA = 0.00f;
	const float GeneralizedBornModel::Default::KAPPA = 0.1f;
	const float GeneralizedBornModel::Default::RHO_0 = 0.00f;
	const float GeneralizedBornModel::Default::DC_SOLVENT = 80.0f;
	const float GeneralizedBornModel::Default::DC_SOLUTE = 2.0f;
	const String GeneralizedBornModel::Default::FILENAME = "solvation/GB.ini";
	*/

	GeneralizedBornModel::GeneralizedBornModel()
		throw(Exception::FileNotFound)
		: ac_(0),
			born_radii_(),
			pair_list_(),
			pair_distances_(),
			pair_charges_(),
			dc_solute_(0.0f),
			dc_solvent_(0.0f),
			scaling_factors_(),
			scaling_factor_filename_(""),
			lambda_(0.0f),
			delta_(0.0f),
			kappa_(0.0f),
			rho_0_(0.0f)
	{
		// set the default values for the parameters
		lambda_ = Default::LAMBDA;
		delta_ = Default::DELTA;
		kappa_ = Default::KAPPA;
		rho_0_ = Default::RHO_0;
		dc_solvent_ = Default::DC_SOLVENT;
		dc_solute_ = Default::DC_SOLUTE;

		// Compute the factor for converting values to kJ/mol

		// Multiply charge unit
		to_kJ_mol_ = Constants::e0 * Constants::e0;

		// Divide by vacuum permittivity
		to_kJ_mol_ /= Constants::VACUUM_PERMITTIVITY;

		// Divide by length unit (Angstrom)
		to_kJ_mol_ /= 1e-10;

		// Divide by 1000 in order to resturn kJ/mol
		to_kJ_mol_ /= 1e3;

		// Now multiply with N_A for obtaining molar values
		to_kJ_mol_ *= Constants::AVOGADRO;

		// Divide by 4\pi
		to_kJ_mol_ /= (4.0f * Constants::PI);

	}


	GeneralizedBornModel::~GeneralizedBornModel()
	{
	}


	void GeneralizedBornModel::clear()
		
	{
		// Clear the AtomContainer pointer
		ac_ = 0;

		// Don't clear the scaling factors, although this is a clear() function.
		// This is obviously inconsistent, but should make sense in this case.
		// Maybe we should rename this method�

		// Clear all the lists we built for fast calculations
		atoms_.clear();
		scaling_factor_.clear();
		pair_list_.clear();
		pair_distances_.clear();
		pair_charges_.clear();
		squared_distances_.clear();
	}


	bool GeneralizedBornModel::setup(const AtomContainer& ac)
		throw(Exception::FileNotFound)
	{
		
		Timer timer;
		timer.start();
		// save the AtomContainer
		clear();
		ac_ = const_cast<AtomContainer*>(&ac);

		// Set the filename for the scaling factors
		if (scaling_factor_filename_ == String(""))
		{
			scaling_factor_filename_ = Default::FILENAME;
		}

		// Go through the atoms of the molecule and find those charged
		// hydrogens which have to be condensed on heavy atoms, because they
		// bear partial charges without havin a radius.

		// Note: Condensing hydrogens may lead to single atoms (NHx)!

		// Store the atoms that have to be removed after condensation in a hash
		// set.
		HashSet<Atom*> remove_me;

		// Iterate over the system
		AtomIterator it = ac_->beginAtom();
		for (; +it; ++it)
		{
			String resid;
			if (it->getResidue() != 0)
			{
				resid = it->getResidue()->getID() + ":";
			}
			else
			{
				resid = "";
			}

			// DEBUG
			/*
			Log.info() << resid << it->getFullName() << "@"
				<< it->getPosition() << " (r=" << it->getRadius() << ", c="
				<< it->getCharge() << ", e=" << it->getElement().getSymbol() << ")" 
				<< endl;
			*/
			// DEBUG

			if (it->getRadius() == 0.0 && it->getCharge() != 0.0)
			{
				if (it->getElement() != PTE[Element::H])
				{
					Log.error() << "Found non-hydrogen atom with non-zero charge and zero radius, aborting." 
						<< endl;
					Log.error() << it->getResidue()->getID() << ":" << it->getFullName() 
						<< "@" << it->getPosition() << " (r=" << it->getRadius() << ", c="
						<< it->getCharge() << ", e=" << it->getElement().getSymbol() << ")" 
						<< endl;
					return(1);
				}
				else
				{
					Log.info() << "Found charged hydrogen with zero radius: "
						<< it->getFullName() << endl;
#ifdef UNITED_ATOMS
					Atom* hydrogen_partner = it->getBond(0)->getPartner(*it);
					Log.info() << "Condensing it onto the bound heavy atom "
						<< hydrogen_partner->getFullName() << endl;
					hydrogen_partner->setCharge(hydrogen_partner->getCharge()
							+ it->getCharge());
					it->setCharge(0.0);
					remove_me.insert(&*it);
#else
					cout << "Setting radius to 1.30 A" << endl;
					it->setRadius(1.30f);
#endif
				}
			}
		}

		// Now remove the atoms from the system
		HashSet<Atom*>::Iterator remove_it = remove_me.begin();
		for (; +remove_it; ++remove_it)
		{
			Atom* ptr = *remove_it;
			delete(ptr);
		}
		Log.info() << "United atoms: Removed " << remove_me.size() 
			<< " atoms from the system" << endl;

		if (ac_->countAtoms() < 2)
		{
			Log.error() << "Need at least two atoms for this method!"
				<< endl;
		}

		// if there are scaling factors stored here, we're fine. If not, try to
		// load them from the default file
		if (scaling_factors_.size() == 0)
		{
			// Read scaling factors from standard INIfile
			if (readScalingFactors(scaling_factor_filename_) == false)
			{
				return(false);
			}
		}

		// Set scaling factor for each atom
		AtomConstIterator it1;
		for (it1 = ac_->beginAtom(); +it1; ++it1)
		{
			atoms_.push_back(&*it1);

			float S_j = 0.0f;

			if (scaling_factors_.has(it1->getElement().getSymbol()))
			{
				S_j = scaling_factors_[it1->getElement().getSymbol()];
			}
			else
			{
				Log.warn() << "Unknown element " << it1->getElement().getSymbol()
					<< ". Using unscaled radius for calculating eff. Born radius." 
					<< std::endl;
				S_j = 1.0f;
			}
			scaling_factor_.push_back(S_j);
		}

		Size N = atoms_.size();

		// Allocate memory
		pair_list_.resize(N*N / 2 + 1);
		pair_distances_.resize(N*N / 2 + 1);
		pair_charges_.resize(N*N / 2 + 1);

		squared_distances_.resize(N);

		// Build pair list and calculate distances
		Size k = 0;
		for (Size i = 0; i < N; ++i)
		{
			// Allocate memory
			squared_distances_[i].resize(N);

			for (Size j = 0; j < i; ++j)
			{
				float distance = atoms_[i]->getPosition().getDistance(atoms_[j]->getPosition());
				if (distance < 0.1f)
				{
					Log.warn() << "Atoms too close: " 
						<< atoms_[i]->getResidue()->getFullName()
						<< ":" << atoms_[i]->getResidue()->getID()
						<< ":" << atoms_[i]->getName()
						<< " - " 
						<< atoms_[j]->getResidue()->getFullName()
						<< ":" << atoms_[j]->getResidue()->getID()
						<< ":" << atoms_[j]->getName()
						<< std::endl;
				}

				pair_list_[k] = std::pair<Size, Size>(i, j);
				pair_distances_[k] = distance;
				// DEBUG
				// cout << "pair_distances_[" << k << "]: " << pair_distances_[k] << endl;
				// /DEBUG
				pair_charges_[k] = atoms_[i]->getCharge() * atoms_[j]->getCharge();
				squared_distances_[i][j] = squared_distances_[j][i] 
					= distance * distance;

				// DEBUG
				/*
				cout << i << " " << j << " " << k << " " 
					<< (Size) (i/2.0f * (i-1) + j) << endl;
				*/
				// /DEBUG

				// increase index counter
				++k;
			}
		}


		timer.stop();
		Log.info() << "setup(): " << timer.getCPUTime() << "s" << std::endl;
		return(true);
	}


	void GeneralizedBornModel::calculateBornRadii_()
		
	{
		born_radii_.clear();
		born_radii_.resize(atoms_.size());
		for (Size i = 0; i < atoms_.size(); ++i)
		{
			// This is what we want to know for atom i
			float effective_born_radius = 0.0f;
			// This variable is used for summing up the actual contributions to the
			// coulomb integral approximation
			float tmp = 0.0f;
			// The actual "radius" of atom i
			float radius = atoms_[i]->getRadius();

			Size n = atoms_.size();
			for (Size j = 0; j < n; ++j)
			{
				if (j != i)
				{
					Size k;
					if (i > j)
					{
						k = (Size) (i/2.0f * (i-1) + j);
					}
					else
					{
						k = (Size) (j/2.0f * (j-1) + i);
					}
					// DEBUG
					// cout << i << " " << j << " --> " << k << endl;
					// /DEBUG

					// sum up the contribution of every atom of the system
					tmp += coulombIntegralApproximation_(pair_distances_[k],
							scaling_factor_[j] * (atoms_[j]->getRadius() - rho_0_), 
							radius - rho_0_);
				}
			}

			// DEBUG
			// float one_over_B = 1.0f / (radius - rho_0_) - 0.5f * tmp;
			// std::cout << "HCT radius = " << (1.0f / one_over_B) << std::endl;
			// /DEBUG

#ifdef TSUI

			// These are the original calculations 
			// See Tsui et al., 

			float S = 1.0f / ( radius - rho_0_ ) - 0.5f * lambda_ * tmp;
			effective_born_radius = 1.0f / S - delta_;

			// DEBUG
			// std::cout << "reduced HCT radius = " << effective_born_radius << std::endl;
			// /DEBUG
#else

			// In Proteins 55:383-394, 2004 Onufriev et al. propose another term

			// OBC I
			float alpha = 0.8f;
			float beta = 0.0f;
			float gamma = 2.91f;

			// OBC II
			/*
				 float alpha = 1.0f;
				 float beta = 0.8f;
				 float gamma = 4.85f;
			 */

			float psi = 0.5f * (radius - rho_0_) * tmp;
			float psi2 = psi * psi;
			float psi3 = psi2 * psi;

			float S = 1.0f / ( radius - rho_0_ ) 
				- 1.0f / radius * tanhf( alpha * psi - beta * psi2 + gamma * psi3 );
			effective_born_radius = 1.0f / S;

			// DEBUG
			// std::cout << "OCB I radius = " << effective_born_radius << std::endl;
			// /DEBUG
#endif
			// DEBUG
			// cout << "born_radii_[" << i << "]: " << born_radii_[i] << endl;
			// /DEBUG

			born_radii_[i] = effective_born_radius;
		}
	}


	float GeneralizedBornModel::calculateEnergy()
		
	{
		// We have to calculate Born radii every time we do the calculation of
		// the energy because the distances between atoms does influence the
		// effedctive radii. This could be done more sophisticated in order to
		// speed up the calculation, no doubt.
		Timer timer;
		timer.start();
		calculateBornRadii_();
		timer.stop();
		Log.info() << "calculateBornRadii_(): " << timer.getCPUTime() 
			<< "s" << std::endl;

		return(calculateTransferEnergy_() + calculateCreationEnergy_());
	}


	float GeneralizedBornModel::calculateSolvationEnergy()
		
	{
		// We have to calculate Born radii every time we do the calculation of
		// the energy because the distances between atoms does influence the
		// effective radii. This could be done more sophisticated in order to
		// speed up the calculation, no doubt.
		Timer timer;
		timer.start();
		calculateBornRadii_();
		timer.stop();
		Log.info() << "calculateBornRadii_(): " << timer.getCPUTime() 
			<< "s" << std::endl;

		// Solvation energy is just the transfer energy with an internal
		// dieelectric constant of 1. The Coulomb part is constant and vanishes
		// in the difference.
		setSoluteDC(1.0f);
		return(calculateTransferEnergy_());
	}


	bool GeneralizedBornModel::readScalingFactors(const String& filename)
		throw(Exception::FileNotFound)
	{

		Path path;
		String tmp = path.find(filename);
		if (tmp == "") tmp = filename;

		Parameters gb_parameter_file(tmp);
		GeneralizedBornModel::GBParameters gb_parameters;
		if (!gb_parameters.extractSection(gb_parameter_file, "ScalingFactors"))
		{
			Log.error() << "Error reading paramters" << std::endl;
			return(false);
		}
		scaling_factors_ = gb_parameters.getScalingFactors();

		dc_solute_ = gb_parameters.getSoluteDC();
		dc_solvent_ = gb_parameters.getSolventDC();
		kappa_ = gb_parameters.getKappa();

		return(true);
	}


	float GeneralizedBornModel::calculateCreationEnergy_() 
		const
		
	{
		Timer timer;
		timer.start();

		// This is just a simple Coulomb-Calculation. 
		
		float energy = 0.0f;

		// iterate over all atom pairs
		for (Size i = 0; i < pair_list_.size(); i++)
		{
			float contribution = 0.0f;
			float c = pair_charges_[i];
			if (c != 0.0f) contribution = c / pair_distances_[i];
			energy += contribution;
		}

		// The pair list delivers every pair only once! So we don't
		// have to divide by two like given in the formula 
		/*
		  $$
				W = \frac{1}{2} \frac{1}{4 \pi \varepsilon_0 \varepsilon_r} 
					\sum_{i \neq j} \frac{q_i q_j}{r_{ij}}
			$$
		*/

		// Divide relative permittivity
		energy /= dc_solute_;

		// Convert to the correct dimension kJ/mol
		energy *= to_kJ_mol_;

		// DEBUG
		cout << "calculateCreationEnergy_(): " << energy << endl;
		// /DEBUG

		timer.stop();
		Log.info() << "calculateCreationEnergy_(): " << timer.getCPUTime() 
			<< "s" << std::endl;

		return(energy);

	}


	float GeneralizedBornModel::calculateTransferEnergy_() 
		const
		
	{
		Timer timer;
		timer.start();

		// This is a standard Generalized Born (see Still et al.) plus the salt
		// effect calculations developed by Srinivasan et al. (Theor. Chem.
		// Acc. 101:426-434, 1999)

		float energy = 0.0f;

		// inverse dcs
		float inv_dc_solute = 1.0f/dc_solute_;
		float inv_dc_solvent = 1.0f/dc_solvent_;

		// First calculate those terms considering different atoms
		for (Size k = 0; k < pair_list_.size(); ++k)
		{
			Size i = pair_list_[k].first;
			Size j = pair_list_[k].second;
			float c = pair_charges_[k];
			if (c != 0.0)
			{
				// Calculate the evil f term and the resulting energy
				// contribution
				float f_GB = f_GB_(i, j);
				float e = c / f_GB;

				// DEBUG
				// cout << "e: " << e << endl;
				// /DEBUG

				// Include salt effects if necessary
				// Maybe this could be done more efficiently
				if (kappa_ != 0.0f)
				{
					e *= (inv_dc_solute - inv_dc_solvent * exp(-kappa_ * f_GB));
				}

				energy += e;
			}
		}

		// Now calculate self energies
		for (Size i = 0; i < atoms_.size(); ++i)
		{
			float c = atoms_[i]->getCharge();
			if (c != 0.0f)
			{
				float e = c * c / born_radii_[i];
				
				// DEBUG
				// cout << "e self: " << 0.5f * e << endl;
				// /DEBUG

				energy += 0.5f * e;
			}
		}

		// This is the simple term without salt effects
		if (kappa_ == 0.0f)
		{
			energy *= (inv_dc_solute - inv_dc_solvent);
		}

		// Change sign (this contribution is negative) and Convert to the unit
		// of kJ/mol
		energy *= - to_kJ_mol_;

		// DEBUG
		cout << "calculateTransferEnergy_(): " << energy << endl;
		// /DEBUG

		timer.stop();
		Log.info() << "calculateTransferEnergy_(): " << timer.getCPUTime() 
			<< "s" << std::endl;

		return(energy);

	}


	float GeneralizedBornModel::f_GB_(Size i, Size j) const
		
	{
		// we need the effective Born radii for computing this smooth volume
		// function
		float R_iR_j = born_radii_[i] * born_radii_[j];
		
		// we need the squared distance
		float squared_distance = squared_distances_[i][j];

		// This is the term derived by Still et al. (JACS 112:6127-6129, 1990)
		float result = sqrt(squared_distance 
				+ R_iR_j * exp(-squared_distance / (4.0f * R_iR_j)));

		// There are other terms possible. Onufriew et al. propose 
		// (J Comput Chem 23:1297-1304, 2002) several other terms:

		/*
		float beta = 0.0f;
		float S;

		S = 1.0f / (1 + squared_distance/(4.0f * R_iR_j));
		S = 1.0f / sqrt(1 + squared_distance/(4.0f * R_iR_j));
		S = 1.0f / (1 + log(1.0f + squared_distance/(4.0f * R_iR_j)));

		result = sqrt(squared_distance 
				+ beta * sqrt(R_iR_j)
				+ R_iR_j * S);
		*/

		// DEBUG
		// cout << "f_GB_(" << i << ", " << j << "):" << result << endl;
		// /DEBUG

		return(result);
	}


	float GeneralizedBornModel::coulombIntegralApproximation_(float r_ij,
			float scaled_radius_j, float radius_i) const
		
	{
		// This is the approximation by Hawkins, Cramer, Truhlar (based on the
		// calculations of Schaefer and Froemmel
		//
		// Hawkins et al,, Chem Phys Lett 246:122-129, 1995
		// Hawkins et al., J Phys Chem 100:19824-19839, 1996
		// Schaefer and Froemmel, J Mol Biol 216:1045-1066, 1990

		// helper variables
		float L_ij;
		float U_ij;

		float r_plus_Salpha = r_ij + scaled_radius_j;
		float r_minus_Salpha = r_ij - scaled_radius_j;
		
		if (r_plus_Salpha <= radius_i)
		{
			L_ij = 1.0f;
			U_ij = 1.0f;
		}
		else
		{
			if (r_minus_Salpha <= radius_i)
			{
				L_ij = radius_i;
			}
			else
			{
				L_ij = r_minus_Salpha;
			}

			U_ij = r_plus_Salpha;
		}

		// Difference of the squared limits
		float D = 1.0f / ( L_ij * L_ij ) - 1.0f / ( U_ij * U_ij );

		float tmp 
			= 1.0f / L_ij 
			- 1.0f / U_ij 
			+ r_ij / 4.0f * -D
			+ 1.0f / ( 2.0f * r_ij ) * log( L_ij / U_ij )
			+ ( scaled_radius_j * scaled_radius_j )
			/ ( 4.0f * r_ij ) * D;

		return(tmp);
	}

	
	void GeneralizedBornModel::setScalingFactorFile(const String& filename)
		
	{
		scaling_factor_filename_ = filename;
	}


	void GeneralizedBornModel::setSolventDC(float solvent_dc)
		
	{
		dc_solvent_ = solvent_dc;
	}


	void GeneralizedBornModel::setSoluteDC(float solute_dc)
		
	{
		dc_solute_ = solute_dc;
	}


	void GeneralizedBornModel::calculatePotential(HashMap<const Atom*,
			float>& p_hash) const
		
	{
		p_hash.clear();

		AtomConstIterator it = ac_->beginAtom();
		for (; +it; ++it)
		{
			p_hash[&*it] = calculatePotential(*it);
		}
	}


	float GeneralizedBornModel::calculatePotential(const Atom& atom_i) const
		
	{

		Timer timer;
		timer.start();

		// We have to find the index of the atom in question
		Index index = -1;
		for (Size i = 0; i < atoms_.size(); ++i)
		{
			if (atoms_[i] == &atom_i) index = i;
		}
		if (index < 0)
		{
			Log.error() << "calculatePotential(): Could not find atom index!"
				<< std::endl;
			return(0.0f);
		}

		// the potential
		float potential = 0.0f;

		// inverse dcs
		float inv_dc_solute = 1.0f/dc_solute_;
		float inv_dc_solvent = 1.0f/dc_solvent_;

		// Now iterate over all atoms and calculate the potential at the
		// position of atom_i
		for (Index j = 0; (Size) j < atoms_.size(); ++j)
		{
			// Get the charge of the atom
			float c = atoms_[j]->getCharge();

			// compute potential contribution only if charge is not zero
			if (c != 0.0)
			{
				// Calculate the evil f term and the resulting energy
				// contribution
				float f_GB;

				// If this is the self-energy part, take the effective Born radius
				// which is equal to f_GB_(i, i). Otherwise compute f_GB_().
				if (index == j)
				{
					f_GB = born_radii_[index];
				}
				else
				{
					f_GB = f_GB_(index, j);
				}

				// Add up this contribution to the potential.
				float p = - c / f_GB;

				// DEBUG
				// cout << "p: " << p << " c: " << c << " f_GB: " << f_GB << endl;
				// /DEBUG

				// Include salt effects if necessary
				// Maybe this could be done more efficiently
				if (kappa_ != 0.0f)
				{
					p *= (inv_dc_solute - inv_dc_solvent * exp(-kappa_ * f_GB));
				} 
				potential += p;

				// pc is the coulomb part of the potential
				float pc = 0.0f;

				// Compute the Coulomb part of the potential
				if (index != j)
				{
					pc = inv_dc_solute * c / sqrt(squared_distances_[index][j]);
					potential += pc;
					// DEBUG
					// cout << "pc: " << pc << endl;
					// /DEBUG
				}
			}
		}

		// Multiply with DC effect term. This is the simple term without salt
		// effects.
		if (kappa_ == 0.0f)
		{
			potential *= (inv_dc_solute - inv_dc_solvent);
		}

		// Multiply charge unit
		potential *= to_kJ_mol_ / Constants::e0;

		// Stop timer
		timer.stop();

		/*
		Log.info() << "calculatePotential(): " << timer.getCPUTime() 
			<< "s" << std::endl;
		*/

		return(potential);


	}


	GeneralizedBornModel::GBParameters::~GBParameters()
		
	{
	}


	GeneralizedBornModel::GBParameters::GBParameters()
		
		: ParameterSection(),
			scaling_factors_()
	{
	}


	const StringHashMap<float>& GeneralizedBornModel::GBParameters::getScalingFactors() const
		
	{
		return(scaling_factors_);
	}


	float GeneralizedBornModel::GBParameters::getSoluteDC() const
		
	{
		return(dc_solute_);
	}


	float GeneralizedBornModel::GBParameters::getSolventDC() const
		
	{
		return(dc_solvent_);
	}


	float GeneralizedBornModel::GBParameters::getKappa() const
		
	{
		return(kappa_);
	}


	bool GeneralizedBornModel::GBParameters::extractSection(Parameters&
			parameters, const String& section_name)
		
	{
		if (!parameters.isValid())
		{
			Log.error() << "Parameters are invalid." << std::endl;
			return(false);
		}

		ParameterSection::extractSection(parameters, section_name);

		if (!hasVariable("factor"))
		{
			Log.error() << "Missing variables" << std::endl;
			return(false);
		}

		Size number_of_keys = getNumberOfKeys();
		Index factor_index = getColumnIndex("factor");

		// clear the HashMap and fill it with the new values
		scaling_factors_.clear();

		for (Size i = 0; i < number_of_keys; ++i)
		{
			String element_symbol = getKey(i);
			
			if (scaling_factors_.has(element_symbol))
			{
				Log.error() << "Duplicate scaling factor for element " 
					<< element_symbol << std::endl;
				return(false);
			}

			// DEBUG
			std::cout << "inserting " << element_symbol << " with factor "
				<< getValue(i, factor_index).toFloat() << std::endl;
			// /DEBUG
			scaling_factors_[element_symbol] = getValue(i, factor_index).toFloat();

		}

		if (options.has("dc_solute"))
		{
			dc_solute_ = options.getReal("dc_solute");
			Log.info() << "Setting solute dc to " << dc_solute_ << std::endl;
		}

		if (options.has("dc_solvent"))
		{
			dc_solvent_ = options.getReal("dc_solvent");
			Log.info() << "Setting solvent dc to " << dc_solvent_ << std::endl;
		}

		if (options.has("lambda"))
		{
			lambda_ = options.getReal("lambda");
			Log.info() << "(still disabled) Setting lambda to " 
				<< lambda_ << std::endl;
		}

		if (options.has("delta"))
		{
			delta_ = options.getReal("delta");
			Log.info() << "(still disabled) Setting delta to " 
				<< delta_ << std::endl;
		}

		if (options.has("kappa"))
		{
			kappa_ = options.getReal("kappa");
			Log.info() << "Setting kappa to " 
				<< kappa_ << std::endl;
		}

		if (options.has("rho_0"))
		{
			rho_0_ = options.getReal("rho_0");
			Log.info() << "(still disabled) Setting rho_0 to " 
				<< rho_0_ << std::endl;
		}

		valid_ = true;
		return(true);

	}

}

