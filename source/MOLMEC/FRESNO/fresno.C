// $Id: fresno.C,v 1.1.2.10 2002/04/24 09:19:53 anker Exp $
// Molecular Mechanics: Fresno force field class

#include <BALL/SYSTEM/path.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/standardPredicates.h>
#include <BALL/KERNEL/bondIterator.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/MOLMEC/PARAMETER/templates.h>

#include <BALL/MOLMEC/FRESNO/fresno.h>
#include <BALL/MOLMEC/FRESNO/fresnoConstant.h>
#include <BALL/MOLMEC/FRESNO/fresnoHydrogenBond.h>
#include <BALL/MOLMEC/FRESNO/fresnoBuriedPolar.h>
#include <BALL/MOLMEC/FRESNO/fresnoLipophilic.h>
#include <BALL/MOLMEC/FRESNO/fresnoRotation.h>
#include <BALL/MOLMEC/FRESNO/fresnoDesolvation.h>
#include <BALL/MOLMEC/FRESNO/chemScoreMetal.h>

using namespace std;

namespace BALL
{


	const char* FresnoFF::Option::CONST = "const";
	const char* FresnoFF::Option::HB = "hb";
	const char* FresnoFF::Option::LIPO = "lipo";
	const char* FresnoFF::Option::ROT = "rot";
	const char* FresnoFF::Option::BP = "bp";
	const char* FresnoFF::Option::DESOLV = "desolv";
	const char* FresnoFF::Option::METAL = "metal";
	const char* FresnoFF::Option::HB_IDEAL_LENGTH = "hb_ideal_length";
	const char* FresnoFF::Option::HB_IDEAL_ANGLE = "hb_ideal_angle";
	const char* FresnoFF::Option::HB_DIST_LOWER = "hb_dist_lower";
	const char* FresnoFF::Option::HB_DIST_UPPER = "hb_dist_upper";
	const char* FresnoFF::Option::HB_ANG_LOWER = "hb_ang_lower";
	const char* FresnoFF::Option::HB_ANG_UPPER = "hb_ang_upper";
	const char* FresnoFF::Option::LIPO_R1_OFFSET = "lipo_r1_offset";
	const char* FresnoFF::Option::LIPO_R2_OFFSET = "lipo_r2_offset";
	const char* FresnoFF::Option::BP_R1_OFFSET = "bp_r1_offset";
	const char* FresnoFF::Option::BP_R2_OFFSET = "bp_r2_offset";
	const char* FresnoFF::Option::ROT_BIND_OFFSET = "rot_bind_offset";
	const char* FresnoFF::Option::ROT_GRID_SPACING = "rot_grid_spacing";
	const char* FresnoFF::Option::ROT_ALGORITHM = "rot_algorithm";
	const char* FresnoFF::Option::METAL_R1 = "metal_r1";
	const char* FresnoFF::Option::METAL_R2 = "metal_r2";

	const float FresnoFF::Default::CONST = -33.614;
	const float FresnoFF::Default::HB = -0.014;
	const float FresnoFF::Default::LIPO = -0.076;
	const float FresnoFF::Default::ROT = 0.017;
	const float FresnoFF::Default::BP = 0.021;
	const float FresnoFF::Default::DESOLV = 0.026;
	const float FresnoFF::Default::METAL = -6.03;
	const float FresnoFF::Default::HB_IDEAL_LENGTH = 1.85;
	const float FresnoFF::Default::HB_IDEAL_ANGLE = 180;
	const float FresnoFF::Default::HB_DIST_LOWER = 0.25;
	const float FresnoFF::Default::HB_DIST_UPPER = 0.65;
	const float FresnoFF::Default::HB_ANG_LOWER = 30;
	const float FresnoFF::Default::HB_ANG_UPPER = 80;
	const float FresnoFF::Default::LIPO_R1_OFFSET = 0.5;
	const float FresnoFF::Default::LIPO_R2_OFFSET = 3.0;
	const float FresnoFF::Default::BP_R1_OFFSET = 0.5;
	const float FresnoFF::Default::BP_R2_OFFSET = 3.0;
	const float FresnoFF::Default::ROT_BIND_OFFSET = 0.5;
	const float FresnoFF::Default::ROT_GRID_SPACING = 5.0;
	const Size FresnoFF::Default::ROT_ALGORITHM =
		FresnoRotation::ALGORITHM__GUESS;
	const float FresnoFF::Default::METAL_R1 = 2.2;
	const float FresnoFF::Default::METAL_R2 = 2.6;

	void FresnoFF::registerComponents_()
		throw()
	{
		// create the component list
		insertComponent(new FresnoConstant(*this));
		insertComponent(new FresnoHydrogenBond(*this));
		insertComponent(new FresnoBuriedPolar(*this));
		insertComponent(new FresnoLipophilic(*this));
		insertComponent(new FresnoRotation(*this));
		insertComponent(new FresnoDesolvation(*this));
		insertComponent(new ChemScoreMetal(*this));
	}


	FresnoFF::FresnoFF()
		throw()
		:	ForceField()
	{
		// register all components of the force field
		registerComponents_();

		// set the name
		setName("Fresno");
	}


	FresnoFF::FresnoFF(System& system)
		throw()
		:	ForceField()
	{
		// register all components of the force field
		registerComponents_();

		// set up with the given system
		bool result = setup(system);

    if (!result)
    {
			Log.error() << "FresnoFF::FresnoFF(System&): "
				<< "Force Field setup failed! " << endl;
      valid_ = false;
		}
	}


	FresnoFF::FresnoFF(System& system, const Options& new_options)
		throw()
		:	ForceField()
	{
		// register all components of the force field
		registerComponents_();

		// set up with the given system
		bool result = setup(system, new_options);

    if (!result)
    {
			Log.error() << "FresnoFF::FresnoFF(System&): "
				<< "Force Field setup failed! " << endl;
      valid_ = false;
		}
	}


	FresnoFF::FresnoFF(const FresnoFF& force_field)
		throw()
		:	ForceField(force_field)
	{
	}


	FresnoFF::~FresnoFF()
		throw()
	{
		clear();
	}


	void FresnoFF::clear()
		throw()
	{
		ForceField::clear();
	}


	bool FresnoFF::specificSetup()
		throw()
	{
		// check whether the system is assigned
		if (getSystem() == 0)
		{
			return false;
		}

		if (getSystem()->countMolecules() != 2)
		{
			Log.error() << "FresnoFF::specificSetup(): "
				<< "Fresno is only defined for systems with 2 molecules" << endl;
			return false;
		}
 
		// we have to assign those atom types defined in 
		// Eldrigde et al. JCAMD 11:425-445, 1997, p 431
		// or the changed version of Rognan et al.

		AtomIterator atom_it = getSystem()->beginAtom();
		for (; +atom_it; ++atom_it)
		{
			fresno_types_.insert(pair<Atom*, short>(&*atom_it, FresnoFF::UNKNOWN));
		}

		HashMap<const Atom*, short>::Iterator it = fresno_types_.begin();
		ConnectedToPredicate connectedTo;
		String symbol;
		const Atom* atom;

		Size lipo_counter = 0;
		Size donor_counter = 0;
		Size acc_don_counter = 0;
		Size acc_counter = 0;
		Size polar_counter = 0;
		Size hyd_counter = 0;
		Size metal_counter = 0;

		// STEP 1
		// ======
		// first assign those types that do not depend on others

		Size assignment_type = FresnoFF::ASSIGNMENT__ELDRIDGE;

		// set up a hashset containing metal symbols
		HashSet<String> metals;
		// ?????
		// For the moment just take those metals we want to use in our
		// calculations
		metals.insert("Ca");
		metals.insert("Mg");
		metals.insert("Mn");
		metals.insert("Zn");
		metals.insert("Fe");

		for (; +it; ++it)
		{
			atom = it->first;
			symbol = atom->getElement().getSymbol();

			// first assign the metals.
			if (metals.has(symbol))
			{
				it->second = FresnoFF::METAL;
				++metal_counter;
			}

			if (symbol == "N")
			{
				connectedTo.setArgument("(H)");
				if (connectedTo(*atom))
				{
					if (assignment_type == FresnoFF::ASSIGNMENT__ELDRIDGE)
					{
						connectedTo.setArgument("(=C)");
						if (connectedTo(*atom))
						{
							it->second = FresnoFF::HBOND_ACCEPTOR_DONOR;
							++acc_don_counter;
							// DEBUG
							// cout << it->first->getFullName() << ": ACC_DON" << endl;
							// /DEBUG
						}
						else
						{
							it->second = FresnoFF::HBOND_DONOR;
							++donor_counter;
							// DEBUG
							// cout << it->first->getFullName() << ": DON" << endl;
							// /DEBUG
						}
					}
				}
				else
				{
					if (atom->countBonds() < 3)
					{
						it->second = FresnoFF::HBOND_ACCEPTOR;
						++acc_counter;
						// DEBUG
						// cout << it->first->getFullName() << ": ACC" << endl;
						// /DEBUG
					}
					else
					{
						it->second = FresnoFF::POLAR;
						++polar_counter;
						// DEBUG
						// cout << it->first->getFullName() << ": POL" << endl;
						// /DEBUG
					}
				}
			}
			else
			{
				if (symbol == "H")
				{
					connectedTo.setArgument("(O)");
					if (connectedTo(*atom))
					{
						it->second = FresnoFF::HBOND_HYDROGEN;
						++hyd_counter;
						// DEBUG
						// cout << it->first->getFullName() << ": HYD" << endl;
						// /DEBUG
					}
					connectedTo.setArgument("(N)");
					if (connectedTo(*atom))
					{
						it->second = FresnoFF::HBOND_HYDROGEN;
						++hyd_counter;
						// DEBUG
						// cout << it->first->getFullName() << ": HYD" << endl;
						// /DEBUG
					}
				}
				else
				{
					if (symbol == "O")
					{
						connectedTo.setArgument("(H)");
						if (connectedTo(*atom))
						{
							if (assignment_type == FresnoFF::ASSIGNMENT__ELDRIDGE)
							{
								it->second = FresnoFF::HBOND_ACCEPTOR_DONOR;
								++acc_don_counter;
								// DEBUG
								// cout << it->first->getFullName() << ": DON" << endl;
								// /DEBUG
							}
						}
						else
						{
							it->second = FresnoFF::HBOND_ACCEPTOR;
							++acc_counter;
							// DEBUG
							// cout << it->first->getFullName() << ": ACC" << endl;
							// /DEBUG
						}
					}
					else
					{
						if ((symbol == "Fl") || (symbol == "P"))
						{
							it->second = FresnoFF::POLAR;
							// DEBUG
							// cout << it->first->getFullName() << ": POL" << endl;
							// /DEBUG
							++polar_counter;
						}
						else
						{
							if (symbol == "S")
							{
								if (atom->countBonds() == 1)
								{
									it->second = FresnoFF::POLAR;
									// DEBUG
									// cout << it->first->getFullName() << ": POL" << endl;
									// /DEBUG
									++polar_counter;
								}
							}
							else
							{
								if (symbol == "C")
								{
									// ????? nitrile/carbonyl
									connectedTo.setArgument("(=O)");
									if (connectedTo(*atom))
									{
										it->second = FresnoFF::POLAR;
										// DEBUG
										// cout << it->first->getFullName() << ": POL" << endl;
										// /DEBUG
										++polar_counter;
									}
									else
									{
										connectedTo.setArgument("(#N)");
										if (connectedTo(*atom))
										{
											it->second = FresnoFF::POLAR;
											// DEBUG
											// cout << it->first->getFullName() << ": POL" << endl;
											// /DEBUG
											++polar_counter;
										}
									}
								}
								else
								{
									if ((symbol == "Cl")
											|| (symbol == "Br")
											|| (symbol == "I"))
									{
										if (atom->getCharge() == 0.0)
										{
											it->second = FresnoFF::LIPOPHILIC;
											// DEBUG
											// cout << it->first->getFullName() << ": LIP" << endl;
											// /DEBUG
											++lipo_counter;
											++lipo_counter;
										}
										else
										{
											it->second = FresnoFF::HBOND_ACCEPTOR;
											// DEBUG
											// cout << it->first->getFullName() << ": ACC" << endl;
											// /DEBUG
											++acc_counter;
											++acc_counter;
										}
									}
								}
							}
						}
					}
				}
			}
		}

		// DEBUG
		cout << "STEP 1 statistics:" << endl << endl;
		cout << "lipophilic atoms:        " << lipo_counter << endl;
		cout << "h bond acceptors:        " << acc_counter << endl;
		cout << "h bond acceptors/donors: " << acc_don_counter << endl;
		cout << "h bond donors:           " << donor_counter << endl;
		cout << "h bond hydrogens:        " << hyd_counter << endl;
		cout << "polar atoms:             " << polar_counter << endl;
		cout << "metal atoms:             " << metal_counter << endl;
		cout << "remaining atoms:         " 
			<< getSystem()->countAtoms() - lipo_counter - acc_counter 
			- acc_don_counter - donor_counter - polar_counter - hyd_counter
			- metal_counter
			<< endl << endl;
		// /DEBUG

		// STEP 2
		// ======
		// now assign those types that depend on connections to already
		// assigned POLAR atoms

		AtomBondConstIterator bond_it;
		Atom* partner;

		Size old_polar_counter = 9999999;

		while (polar_counter != old_polar_counter)
		{
			old_polar_counter = polar_counter;

			for (it = fresno_types_.begin(); +it; ++it)
			{
				atom = it->first;
				symbol = atom->getElement().getSymbol();
				if (symbol == "S")
				{
					if (it->second == FresnoFF::UNKNOWN)
					{
						BALL_FOREACH_ATOM_BOND(*atom, bond_it)
						{
							partner = bond_it->getPartner(*atom);
							if (fresno_types_.has(partner))
							{
								if (((fresno_types_[partner] == FresnoFF::POLAR)
											|| (fresno_types_[partner] == FresnoFF::HBOND_ACCEPTOR)
											|| (fresno_types_[partner] == FresnoFF::HBOND_ACCEPTOR_DONOR)
											|| (fresno_types_[partner] == FresnoFF::HBOND_DONOR)
											|| (fresno_types_[partner] == FresnoFF::HBOND_HYDROGEN)
										) && (
											!((partner->getElement().getSymbol() == "C") 
												&& (fresno_types_[partner] == FresnoFF::POLAR))
											)
									 )
								{
									it->second = FresnoFF::POLAR;
									// DEBUG
									// cout << it->first->getFullName() << ": POL" << endl;
									// /DEBUG
									++polar_counter;
									break;
								}
							}
							else
							{
								Log.error() << "FresnoFF::specificSetup(): "
									<< "Referencing an unknown atom." << endl;
								return false;
							}
						}
					}
					// PARANOIA
					else
					{
						if (it->second != FresnoFF::POLAR)
						{
							Log.warn() << "FresnoFF:specificSetup(): "
								<< "Trying to overwrite already assigned Fresno type: "
								<< atom->getFullName()
								<< " (Step 2, Symbol " << symbol
								<< ", old type " << it->second << ")"
								<< endl;
						}
					}
					// /PARANOIA
				}
				else
				{
					if (symbol == "C")
					{
						if (it->second == FresnoFF::UNKNOWN)
						{
							bool already_found_one = false;
							BALL_FOREACH_ATOM_BOND(*atom, bond_it)
							{
								partner = bond_it->getPartner(*atom);
								if (fresno_types_.has(partner))
								{
									if (((fresno_types_[partner] == FresnoFF::POLAR)
												|| (fresno_types_[partner] == FresnoFF::HBOND_ACCEPTOR)
												|| (fresno_types_[partner] == FresnoFF::HBOND_ACCEPTOR_DONOR)
												|| (fresno_types_[partner] == FresnoFF::HBOND_DONOR)
												|| (fresno_types_[partner] == FresnoFF::HBOND_HYDROGEN)
											) && (
												!((partner->getElement().getSymbol() == "C") 
													&& (fresno_types_[partner] == FresnoFF::POLAR))
												)
										 )
									{
										if (already_found_one)
										{
											it->second = FresnoFF::POLAR;
											// DEBUG
											// cout << it->first->getFullName() << ": POL" << endl;
											// /DEBUG
											++polar_counter;
											break;
										}
										else
										{
											already_found_one = true;
										}
									}
								}
								else
								{
									Log.error() << "FresnoFF::specificSetup(): "
										<< "Referencing an unknown atom." << endl;
									return false;
								}
							}
						}
						// PARANOIA
						else
						{
							if (it->second != FresnoFF::POLAR)
							{
								Log.warn() << "FresnoFF:specificSetup(): "
									<< "Trying to overwrite already assigned Fresno type: "
									<< atom->getFullName()
									<< " (Step 2, Symbol " << symbol 
									<< ", old type " << it->second << ")"
									<< endl;
							}
						}
						// /PARANOIA
					}
				}
			}
		}

		// DEBUG
		cout << "STEP 2 statistics:" << endl << endl;
		cout << "lipophilic atoms:        " << lipo_counter << endl;
		cout << "h bond acceptors:        " << acc_counter << endl;
		cout << "h bond acceptors/donors: " << acc_don_counter << endl;
		cout << "h bond donors:           " << donor_counter << endl;
		cout << "h bond hydrogens:        " << hyd_counter << endl;
		cout << "polar atoms:             " << polar_counter << endl;
		cout << "metal atoms:             " << metal_counter << endl;
		cout << "remaining atoms:         " 
			<< getSystem()->countAtoms() - lipo_counter - acc_counter 
			- acc_don_counter - donor_counter - polar_counter - hyd_counter
			- metal_counter
			<< endl << endl;
		// /DEBUG

		// STEP 3
		// ======
		//

		for (it = fresno_types_.begin(); +it; ++it)
		{
			atom = it->first;
			symbol = atom->getElement().getSymbol();
			if (symbol == "S")
			{
				if ((it->second != FresnoFF::HBOND_ACCEPTOR)
					&& (it->second != FresnoFF::POLAR))
				{
					// PARANOIA
					if (it->second != FresnoFF::UNKNOWN)
					{
						Log.warn() << "FresnoFF:specificSetup(): "
							<< "Trying to overwrite already assigned Fresno type: "
							<< atom->getFullName()
							<< " (Step 3, Symbol " << symbol 
							<< ", old type " << it->second << ")"
							<< endl;
					}
					// /PARANOIA
					it->second = FresnoFF::LIPOPHILIC;
					// DEBUG
					// cout << it->first->getFullName() << ": LIP" << endl;
					// /DEBUG
					++lipo_counter;
				}
			}
			else
			{
				if (symbol == "C")
				{
					if (it->second != FresnoFF::POLAR)
					{
						// PARANOIA
						if (it->second != FresnoFF::UNKNOWN)
						{
							Log.warn() << "FresnoFF:specificSetup(): "
								<< "Trying to overwrite already assigned Fresno type: "
								<< atom->getFullName()
								<< " (Step 3, Symbol " << symbol 
								<< ", old type " << it->second << ")"
								<< endl;
						}
						// /PARANOIA
						it->second = FresnoFF::LIPOPHILIC;
						// DEBUG
						// cout << it->first->getFullName() << ": LIP" << endl;
						// /DEBUG
						++lipo_counter;
					}
				}
			}
		}

		// DEBUG
		cout << "STEP 3 statistics:" << endl << endl;
		cout << "lipophilic atoms:        " << lipo_counter << endl;
		cout << "h bond acceptors:        " << acc_counter << endl;
		cout << "h bond acceptors/donors: " << acc_don_counter << endl;
		cout << "h bond donors:           " << donor_counter << endl;
		cout << "h bond hydrogens:        " << hyd_counter << endl;
		cout << "polar atoms:             " << polar_counter << endl;
		cout << "metal atoms:             " << metal_counter << endl;
		cout << "remaining atoms:         " 
			<< getSystem()->countAtoms() - lipo_counter - acc_counter 
			- acc_don_counter - donor_counter - polar_counter - hyd_counter
			- metal_counter
			<< endl << endl;
		// /DEBUG

		return true;

	}


	const FresnoFF& FresnoFF::operator = (const FresnoFF& force_field)
		throw()
	{
		// avoid self assignment
		if (&force_field != this)
		{
			ForceField::operator = (force_field);
		}
		
		return *this;
	}


	double FresnoFF::getHydrogenBondEnergy() const
		throw()
	{
		ForceFieldComponent* component = getComponent("Fresno Hydrogen Bond");
		if (component != 0)
		{
			return component->getEnergy();
		} 
		else 
		{
			return 0;
		}
	}


	double FresnoFF::getLipophilicEnergy() const
		throw()
	{
		ForceFieldComponent* component = getComponent("Fresno Lipophilic");
		if (component != 0)
		{
			return component->getEnergy();
		} 
		else 
		{
			return 0;
		}
	}


	double FresnoFF::getRotationalEnergy() const
		throw()
	{
		ForceFieldComponent* component = getComponent("Fresno Rotational");
		if (component != 0)
		{
			return component->getEnergy();
		} 
		else 
		{
			return 0;
		}
	}


	double FresnoFF::getBuriedPolarEnergy() const
		throw()
	{
		ForceFieldComponent* component = getComponent("Fresno Buried Polar");
		if (component != 0)
		{
			return component->getEnergy();
		} 
		else 
		{
			return 0;
		}
	}


	double FresnoFF::getDesolvationEnergy() const
		throw()
	{
		ForceFieldComponent* component = getComponent("Fresno Desolvation");
		if (component != 0)
		{
			return component->getEnergy();
		} 
		else 
		{
			return 0;
		}
	}


	const HashMap<const Atom*, short>& FresnoFF::getFresnoTypes() const
		throw()
	{
		return fresno_types_;
	}


} // namespace BALL
