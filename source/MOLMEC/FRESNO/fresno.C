// $Id: fresno.C,v 1.1.2.3 2002/03/05 22:43:40 anker Exp $
// Molecular Mechanics: Fresno force field class

#include <BALL/SYSTEM/path.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/standardPredicates.h>
#include <BALL/KERNEL/bondIterator.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/MOLMEC/PARAMETER/templates.h>

#include <BALL/MOLMEC/FRESNO/fresno.h>
#include <BALL/MOLMEC/FRESNO/fresnoHydrogenBond.h>
#include <BALL/MOLMEC/FRESNO/fresnoBuriedPolar.h>
#include <BALL/MOLMEC/FRESNO/fresnoLipophilic.h>
#include <BALL/MOLMEC/FRESNO/fresnoRotation.h>
#include <BALL/MOLMEC/FRESNO/fresnoDesolvation.h>

using namespace std;

namespace BALL
{

	const char* FresnoFF::Option::FILENAME = "filename";
	const char* FresnoFF::Option::ASSIGN_CHARGES = "assign_charges"; 
	const char* FresnoFF::Option::ASSIGN_TYPENAMES = "assign_type_names"; 
	const char* FresnoFF::Option::ASSIGN_TYPES = "assign_types"; 
	const char* FresnoFF::Option::OVERWRITE_CHARGES = "overwrite_non-zero_charges"; 
	const char* FresnoFF::Option::OVERWRITE_TYPENAMES = "overwrite_non-empty_typenames"; 


	const char* FresnoFF::Default::FILENAME = "Fresno/fresno.ini";
	const bool	FresnoFF::Default::ASSIGN_CHARGES = true;
	const bool	FresnoFF::Default::ASSIGN_TYPENAMES = true;
	const bool	FresnoFF::Default::ASSIGN_TYPES = true;
	const bool	FresnoFF::Default::OVERWRITE_CHARGES = true;
	const bool	FresnoFF::Default::OVERWRITE_TYPENAMES = false;


	void FresnoFF::registerComponents_()
		throw()
	{
		// create the component list
		insertComponent(new FresnoHydrogenBond(*this));
		insertComponent(new FresnoBuriedPolar(*this));
		insertComponent(new FresnoLipophilic(*this));
		insertComponent(new FresnoRotation(*this));
		// insertComponent(new FresnoDesolvation(*this));
	}


	FresnoFF::FresnoFF()
		throw()
		:	ForceField(),
			filename_(Default::FILENAME),
			parameters_initialized_(false)
	{
		// register all components of the force field
		registerComponents_();

		// set the name
		setName("Fresno [" + filename_ + "]");
	}


	FresnoFF::FresnoFF(System& system)
		throw()
		:	ForceField(),
			filename_(Default::FILENAME),
			parameters_initialized_(false)
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
		:	ForceField(),
			filename_(Default::FILENAME),
			parameters_initialized_(false)
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
		:	ForceField(force_field),
			filename_(force_field.filename_),
			parameters_initialized_(false)
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
		filename_ = Default::FILENAME;
		parameters_initialized_ = false;
	}


	bool FresnoFF::specificSetup()
		throw(Exception::FileNotFound)
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
 
		/* Not needed at the moment. 

		// check whether the parameter file name
		// is set in the options
		if (options.has(Option::FILENAME))
		{
			filename_ = options[Option::FILENAME];
			setName("Fresno [" + filename_ + "]");
		} 
		else 
		{
			// ?????
			// Why would we set an option HERE? Is it really necessary? This
			// code snippet was stolen from amber...
			options[Option::FILENAME] = filename_;
		}

		// open parameter file
		Path    path;
		String  filename(path.find(filename_));

		if (filename == "") 
		{
			throw Exception::FileNotFound(__FILE__, __LINE__, filename_);
		}

		// initialize the force field parameters
		// and retrieve the atom types
		if (parameters_.getFilename() != filename || !parameters_initialized_)
		{
			parameters_.setFilename(filename);
			parameters_.init();

			// this is the first time parameters was initialized
			// tell all components about it
			parameters_initialized_ = false;

			// retrieve global force field options
			ParameterSection global_options;
			global_options.extractSection(parameters_, "Options");
		}

		options.setDefaultBool(Option::ASSIGN_CHARGES, Default::ASSIGN_CHARGES);
		bool assign_charges = options.getBool(Option::ASSIGN_CHARGES);
		options.setDefaultBool(Option::ASSIGN_TYPES, Default::ASSIGN_TYPES);
		bool assign_types = options.getBool(Option::ASSIGN_TYPES);
		options.setDefaultBool(Option::ASSIGN_TYPENAMES, Default::ASSIGN_TYPENAMES);
		bool assign_type_names = options.getBool(Option::ASSIGN_TYPENAMES);
		options.setDefaultBool(Option::OVERWRITE_TYPENAMES, Default::OVERWRITE_TYPENAMES);
		bool overwrite_type_names = options.getBool(Option::OVERWRITE_TYPENAMES);
		options.setDefaultBool(Option::OVERWRITE_CHARGES, Default::OVERWRITE_CHARGES);
		bool overwrite_charges = options.getBool(Option::OVERWRITE_CHARGES);

		// extract template section (containing charges and atom types)
		if (assign_charges || assign_type_names)
		{
			Templates templates;
			templates.extractSection(parameters_, "ChargesAndTypeNames");
			if (assign_charges && assign_type_names)
			{
				templates.assign(*getSystem(), overwrite_type_names, overwrite_charges);
			} 
			else 
			{
				if (assign_type_names)
				{
					templates.assignTypeNames(*getSystem(), overwrite_type_names);
				} 
				else 
				{
					templates.assignCharges(*getSystem(), overwrite_charges);
				}
			}
		}
		*/

		// we have to assign those atom types defined in 
		// Eldrigde et al. JCAMD 11:425-445, 1997, p 431.

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

		// STEP 1
		// ======
		// first assign those types that do not depend on others

		for (; +it; ++it)
		{
			atom = it->first;
			symbol = atom->getElement().getSymbol();

			if (symbol == "N")
			{
				connectedTo.setArgument("(H)");
				if (connectedTo(*atom))
				{
					connectedTo.setArgument("(=C)");
					if (connectedTo(*atom))
					{
						it->second = FresnoFF::HBOND_ACCEPTOR_DONOR;
						++acc_don_counter;
						// DEBUG
						cout << it->first->getFullName() << ": ACC_DON" << endl;
						// /DEBUG
					}
					else
					{
						it->second = FresnoFF::HBOND_DONOR;
						++donor_counter;
						// DEBUG
						cout << it->first->getFullName() << ": DON" << endl;
						// /DEBUG
					}
				}
				else
				{
					if (atom->countBonds() < 3)
					{
						it->second = FresnoFF::HBOND_ACCEPTOR;
						++acc_counter;
						// DEBUG
						cout << it->first->getFullName() << ": ACC" << endl;
						// /DEBUG
					}
					else
					{
						it->second = FresnoFF::POLAR;
						++polar_counter;
						// DEBUG
						cout << it->first->getFullName() << ": POL" << endl;
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
						cout << it->first->getFullName() << ": HYD" << endl;
						// /DEBUG
					}
					connectedTo.setArgument("(N)");
					if (connectedTo(*atom))
					{
						it->second = FresnoFF::HBOND_HYDROGEN;
						++hyd_counter;
						// DEBUG
						cout << it->first->getFullName() << ": HYD" << endl;
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
							it->second = FresnoFF::HBOND_ACCEPTOR_DONOR;
							++acc_don_counter;
							// DEBUG
							cout << it->first->getFullName() << ": DON" << endl;
							// /DEBUG
						}
						else
						{
							it->second = FresnoFF::HBOND_ACCEPTOR;
							++acc_counter;
							// DEBUG
							cout << it->first->getFullName() << ": ACC" << endl;
							// /DEBUG
						}
					}
					else
					{
						if ((symbol == "Fl") || (symbol == "P"))
						{
							it->second = FresnoFF::POLAR;
							// DEBUG
							cout << it->first->getFullName() << ": POL" << endl;
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
									cout << it->first->getFullName() << ": POL" << endl;
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
										cout << it->first->getFullName() << ": POL" << endl;
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
											cout << it->first->getFullName() << ": POL" << endl;
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
											cout << it->first->getFullName() << ": LIP" << endl;
											// /DEBUG
											++lipo_counter;
											++lipo_counter;
										}
										else
										{
											it->second = FresnoFF::HBOND_ACCEPTOR;
											// DEBUG
											cout << it->first->getFullName() << ": ACC" << endl;
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
		cout << "remaining atoms:         " 
			<< getSystem()->countAtoms() - lipo_counter - acc_counter -
			   acc_don_counter - donor_counter - polar_counter
			<< endl << endl;
		// /DEBUG

		// STEP 2
		// ======
		// now assign those types that depend on connections to already
		// assigned POLAR atoms

		AtomBondConstIterator bond_it;
		Atom* partner;

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
									) && (
										!((partner->getElement().getSymbol() == "C") 
											&& (fresno_types_[partner] == FresnoFF::POLAR))
										)
								 )
							{
								it->second = FresnoFF::POLAR;
								// DEBUG
								cout << it->first->getFullName() << ": POL" << endl;
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
				// DEBUG
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
				// /DEBUG
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
										cout << it->first->getFullName() << ": POL" << endl;
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
					// DEBUG
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
					// /DEBUG
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
		cout << "remaining atoms:         " 
			<< getSystem()->countAtoms() - lipo_counter - acc_counter -
			   acc_don_counter - donor_counter - polar_counter
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
					cout << it->first->getFullName() << ": LIP" << endl;
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
						cout << it->first->getFullName() << ": LIP" << endl;
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
		cout << "remaining atoms:         " 
			<< getSystem()->countAtoms() - lipo_counter - acc_counter -
			   acc_don_counter - donor_counter - polar_counter
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
			filename_ = force_field.filename_;
			parameters_initialized_ = force_field.parameters_initialized_;
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
