// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Slick-development Team, Marcel Schumann $
// ----------------------------------------------------

#include <BALL/SCORING/COMPONENTS/fresnoTypes.h>
#include <BALL/KERNEL/standardPredicates.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/SCORING/COMMON/scoringFunction.h>


using namespace BALL;
using namespace std;

FresnoTypes::FresnoTypes(AtomContainer* container)
{
	container_ = container;
	setup();
}


bool FresnoTypes::setup()
{
	Size verbosity = 0;

	if (container_ == 0)
	{
		std::cerr << "No system set." << std::endl;
		return false;
	}

	type_map_.clear();
	for (AtomIterator atom_it = container_->beginAtom(); atom_it != container_->endAtom(); ++atom_it)
	{
		type_map_.insert(make_pair(&*atom_it, FresnoTypes::UNKNOWN));
	}

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

	Size assignment_type = FresnoTypes::ASSIGNMENT__ELDRIDGE;

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

	HashMap<const Atom*, Size>::Iterator it = type_map_.begin();
	for (; it != type_map_.end(); ++it)
	{
		atom = it->first;
		symbol = atom->getElement().getSymbol();

		// first assign the metals.
		if (metals.has(symbol))
		{
			it->second = FresnoTypes::METAL;
			++metal_counter;
		}

		if (symbol == "N")
		{
			connectedTo.setArgument("(H)");
			if (connectedTo(*atom))
			{
				if (assignment_type == FresnoTypes::ASSIGNMENT__ELDRIDGE)
				{
					connectedTo.setArgument("(=C)");
					if (connectedTo(*atom))
					{
						it->second = FresnoTypes::HBOND_ACCEPTOR_DONOR;
						++acc_don_counter;
						if (verbosity >= 90)
						{
							Log.info() << it->first->getFullName() << ": ACC_DON"
								<< std::endl;
						}
					}
					else
					{
						it->second = FresnoTypes::HBOND_DONOR;
						++donor_counter;
						if (verbosity >= 90)
						{
							Log.info() << it->first->getFullName() << ": DON" << std::endl;
						}
					}
				}
			}
			else
			{
				if (atom->countBonds() < 3)
				{
					it->second = FresnoTypes::HBOND_ACCEPTOR;
					++acc_counter;
					if (verbosity >= 90)
					{
						Log.info() << it->first->getFullName() << ": ACC" << std::endl;
					}
				}
				else
				{
					it->second = FresnoTypes::POLAR;
					++polar_counter;
					if (verbosity >= 90)
					{
						Log.info() << it->first->getFullName() << ": POL" << std::endl;
					}
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
					it->second = FresnoTypes::HBOND_HYDROGEN;
					++hyd_counter;
					if (verbosity >= 90)
					{
						Log.info() << it->first->getFullName() << ": HYD" << std::endl;
					}
				}
				connectedTo.setArgument("(N)");
				if (connectedTo(*atom))
				{
					it->second = FresnoTypes::HBOND_HYDROGEN;
					++hyd_counter;
					if (verbosity >= 90)
					{
						Log.info() << it->first->getFullName() << ": HYD" << std::endl;
					}
				}
			}
			else
			{
				if (symbol == "O")
				{
					connectedTo.setArgument("(H)");
					if (connectedTo(*atom))
					{
						if (assignment_type == FresnoTypes::ASSIGNMENT__ELDRIDGE)
						{
							it->second = FresnoTypes::HBOND_ACCEPTOR_DONOR;
							++acc_don_counter;
							if (verbosity >= 90)
							{
								Log.info() << it->first->getFullName() << ": DON" << std::endl;
							}
						}
					}
					else
					{
						it->second = FresnoTypes::HBOND_ACCEPTOR;
						++acc_counter;
						if (verbosity >= 90)
						{
							Log.info() << it->first->getFullName() << ": ACC" << std::endl;
						}
					}
				}
				else
				{
					if ((symbol == "F") || (symbol == "P"))
					{
						it->second = FresnoTypes::POLAR;
						if (verbosity >= 90)
						{
							Log.info() << it->first->getFullName() << ": POL" << std::endl;
						}
						++polar_counter;
					}
					else
					{
						if (symbol == "S")
						{
							if (atom->countBonds() == 1)
							{
								it->second = FresnoTypes::POLAR;
								if (verbosity >= 90)
								{
									Log.info() << it->first->getFullName() << ": POL" << std::endl;
								}
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
									it->second = FresnoTypes::POLAR;
									if (verbosity >= 90)
									{
										Log.info() << it->first->getFullName() << ": POL" << std::endl;
									}
									++polar_counter;
								}
								else
								{
									connectedTo.setArgument("(#N)");
									if (connectedTo(*atom))
									{
										it->second = FresnoTypes::POLAR;
										if (verbosity >= 90)
										{
											Log.info() << it->first->getFullName() << ": POL" << std::endl;
										}
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
										it->second = FresnoTypes::LIPOPHILIC;
										if (verbosity >= 90)
										{
											Log.info() << it->first->getFullName() << ": LIP" << std::endl;
										}
										++lipo_counter;
									}
									else
									{
										it->second = FresnoTypes::HBOND_ACCEPTOR;
										if (verbosity >= 90)
										{
											Log.info() << it->first->getFullName() << ": ACC" << std::endl;
										}
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

	if (verbosity > 8)
	{
		Log.info() << "STEP 1 statistics:" << std::endl << std::endl;
		Log.info() << "lipophilic atoms:        " << lipo_counter << std::endl;
		Log.info() << "h bond acceptors:        " << acc_counter << std::endl;
		Log.info() << "h bond acceptors/donors: " << acc_don_counter << std::endl;
		Log.info() << "h bond donors:           " << donor_counter << std::endl;
		Log.info() << "h bond hydrogens:        " << hyd_counter << std::endl;
		Log.info() << "polar atoms:             " << polar_counter << std::endl;
		Log.info() << "metal atoms:             " << metal_counter << std::endl;
		Log.info() << "remaining atoms:         "
			<< container_->countAtoms() - lipo_counter - acc_counter
			- acc_don_counter - donor_counter - polar_counter - hyd_counter
			- metal_counter
			<< std::endl << std::endl;
	}

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

		for (it = type_map_.begin(); it != type_map_.end(); ++it)
		{
			atom = it->first;
			symbol = atom->getElement().getSymbol();
			if (symbol == "S")
			{
				if (it->second == FresnoTypes::UNKNOWN)
				{
					BALL_FOREACH_ATOM_BOND(*atom, bond_it)
					{
						partner = bond_it->getPartner(*atom);
						if (type_map_.has(partner))
						{
							if (((type_map_[partner] == FresnoTypes::POLAR)
										|| (type_map_[partner] == FresnoTypes::HBOND_ACCEPTOR)
										|| (type_map_[partner] == FresnoTypes::HBOND_ACCEPTOR_DONOR)
										|| (type_map_[partner] == FresnoTypes::HBOND_DONOR)
										|| (type_map_[partner] == FresnoTypes::HBOND_HYDROGEN)
									) && (
										!((partner->getElement().getSymbol() == "C")
											&& (type_map_[partner] == FresnoTypes::POLAR))
										)
									)
							{
								it->second = FresnoTypes::POLAR;
								if (verbosity >= 90)
								{
									Log.info() << it->first->getFullName() << ": POL" << std::endl;
								}
								++polar_counter;
								break;
							}
						}
						else
						{
							Log.error() << "FresnoTypes::specificSetup(): "
								<< "Referencing an unknown atom." << std::endl;
							return false;
						}
					}
				}
				// PARANOIA
				else
				{
					if (it->second != FresnoTypes::POLAR)
					{
						Log.warn() << "FresnoTypes:specificSetup(): "
							<< "Trying to overwrite already assigned Fresno type: "
							<< atom->getFullName()
							<< " (Step 2, Symbol " << symbol
							<< ", old type " << it->second << ")"
							<< std::endl;
					}
				}
				// /PARANOIA
			}
			else
			{
				if (symbol == "C")
				{
					if (it->second == FresnoTypes::UNKNOWN)
					{
						bool already_found_one = false;
						BALL_FOREACH_ATOM_BOND(*atom, bond_it)
						{
							partner = bond_it->getPartner(*atom);
							if (type_map_.has(partner))
							{
								if (((type_map_[partner] == FresnoTypes::POLAR)
											|| (type_map_[partner] == FresnoTypes::HBOND_ACCEPTOR)
											|| (type_map_[partner] == FresnoTypes::HBOND_ACCEPTOR_DONOR)
											|| (type_map_[partner] == FresnoTypes::HBOND_DONOR)
											|| (type_map_[partner] == FresnoTypes::HBOND_HYDROGEN)
										) && (
											!((partner->getElement().getSymbol() == "C")
												&& (type_map_[partner] == FresnoTypes::POLAR))
											)
										)
								{
									if (already_found_one)
									{
										it->second = FresnoTypes::POLAR;
										if (verbosity >= 90)
										{
											Log.info() << it->first->getFullName() << ": POL" << std::endl;
										}
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
								Log.error() << "FresnoTypes::specificSetup(): "
									<< "Referencing an unknown atom." << std::endl;
								return false;
							}
						}
					}
					// PARANOIA
					else
					{
						if (it->second != FresnoTypes::POLAR)
						{
							Log.warn() << "FresnoTypes:specificSetup(): "
								<< "Trying to overwrite already assigned Fresno type: "
								<< atom->getFullName()
								<< " (Step 2, Symbol " << symbol
								<< ", old type " << it->second << ")"
								<< std::endl;
						}
					}
					// /PARANOIA
				}
			}
		}
	}

	if (verbosity > 8)
	{
		Log.info() << "STEP 2 statistics:" << std::endl << std::endl;
		Log.info() << "lipophilic atoms:        " << lipo_counter << std::endl;
		Log.info() << "h bond acceptors:        " << acc_counter << std::endl;
		Log.info() << "h bond acceptors/donors: " << acc_don_counter << std::endl;
		Log.info() << "h bond donors:           " << donor_counter << std::endl;
		Log.info() << "h bond hydrogens:        " << hyd_counter << std::endl;
		Log.info() << "polar atoms:             " << polar_counter << std::endl;
		Log.info() << "metal atoms:             " << metal_counter << std::endl;
		Log.info() << "remaining atoms:         "
			<< container_->countAtoms() - lipo_counter - acc_counter
			- acc_don_counter - donor_counter - polar_counter - hyd_counter
			- metal_counter
			<< std::endl << std::endl;
	}

	// STEP 3
	// ======
	//

	for (it = type_map_.begin(); it != type_map_.end(); ++it)
	{
		atom = it->first;
		symbol = atom->getElement().getSymbol();
		if (symbol == "S")
		{
			if ((it->second != FresnoTypes::HBOND_ACCEPTOR)
				&& (it->second != FresnoTypes::POLAR))
			{
				// PARANOIA
				if (it->second != FresnoTypes::UNKNOWN)
				{
					Log.warn() << "FresnoTypes:specificSetup(): "
						<< "Trying to overwrite already assigned Fresno type: "
						<< atom->getFullName()
						<< " (Step 3, Symbol " << symbol
						<< ", old type " << it->second << ")"
						<< std::endl;
				}
				// /PARANOIA
				it->second = FresnoTypes::LIPOPHILIC;
				if (verbosity >= 90)
				{
					Log.info() << it->first->getFullName() << ": LIP" << std::endl;
				}
				++lipo_counter;
			}
		}
		else
		{
			if (symbol == "C")
			{
				if (it->second != FresnoTypes::POLAR)
				{
					// PARANOIA
					if (it->second != FresnoTypes::UNKNOWN)
					{
						Log.warn() << "FresnoTypes:specificSetup(): "
							<< "Trying to overwrite already assigned Fresno type: "
							<< atom->getFullName()
							<< " (Step 3, Symbol " << symbol
							<< ", old type " << it->second << ")"
							<< std::endl;
					}
					// /PARANOIA
					it->second = FresnoTypes::LIPOPHILIC;
					if (verbosity >= 90)
					{
						Log.info() << it->first->getFullName() << ": LIP" << std::endl;
					}
					++lipo_counter;
				}
			}
		}
	}

	if (verbosity > 8)
	{
		Log.info() << "STEP 3 statistics:" << std::endl << std::endl;
		Log.info() << "lipophilic atoms:        " << lipo_counter << std::endl;
		Log.info() << "h bond acceptors:        " << acc_counter << std::endl;
		Log.info() << "h bond acceptors/donors: " << acc_don_counter << std::endl;
		Log.info() << "h bond donors:           " << donor_counter << std::endl;
		Log.info() << "h bond hydrogens:        " << hyd_counter << std::endl;
		Log.info() << "polar atoms:             " << polar_counter << std::endl;
		Log.info() << "metal atoms:             " << metal_counter << std::endl;
		Log.info() << "remaining atoms:         "
			<< container_->countAtoms() - lipo_counter - acc_counter
			- acc_don_counter - donor_counter - polar_counter - hyd_counter
			- metal_counter
			<< std::endl << std::endl;
	}

	return true;
} // end of setup()


const HashMap<const Atom*, Size>* FresnoTypes::getTypeMap() const
{
	return &type_map_;
}
