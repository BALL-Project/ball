// $Id: fresno.C,v 1.1.2.24 2005/01/30 14:00:21 anker Exp $
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
#include <BALL/MOLMEC/CHARMM/charmmNonBonded.h>
#include <BALL/MOLMEC/FRESNO/fresnoNonPolar.h>
#include <BALL/MOLMEC/FRESNO/fresnoRingStacking.h>

using namespace std;

namespace BALL
{


	FresnoFF::BaseFunction::BaseFunction()
		throw()
		:	lower_(0.0f),
			upper_(0.0f)
	{
	}

	FresnoFF::BaseFunction::BaseFunction(float lower, float upper)
		throw()
		:	lower_(lower),
			upper_(upper)
	{
	}

	void FresnoFF::BaseFunction::setLower(float lower)
		throw()
	{
		lower_ = lower;
	}

	void FresnoFF::BaseFunction::setUpper(float upper)
		throw()
	{
		upper_ = upper;
	}

	float FresnoFF::BaseFunction::calculate(float /* x */) const 
		throw()
	{
		return(0.0);
	}

	float FresnoFF::BaseFunction::calculate(float /* x */, float /* lower */,
			float /* upper */) 
		throw()
	{
		return(0.0);
	}

	FresnoFF::BaseFunctionLinear::BaseFunctionLinear()
		throw()
	{
	}

	FresnoFF::BaseFunctionLinear::BaseFunctionLinear(float lower, float upper)
		throw()
	{
		// ????? I wanted to just call the constructor of FresnoFF::BaseFunction,
		// but that didn't work, because the private variables were not
		// changed. Maybe I instantiated another one, but I actually believed
		// that the following call should suffice:
		//FresnoFF::BaseFunction::FresnoFF::BaseFunction(lower, upper);
		lower_ = lower;
		upper_ = upper;
	}

	float FresnoFF::BaseFunctionLinear::calculate(float x) const
		throw()
	{

		float return_value;

		// DEBUG
		// cout << "l = " << lower_ << " u = " << upper_ << " x = " << x << endl;
		// /DEBUG

		if (x < 0.0)
		{
			Log.error() << "FresnoFF::BaseFunctionLinear::calculate(): "
				<< "negative score, returning 0." << endl;
			return(0.0);
		}

		if (x <= lower_)
		{
			return_value = 1.0;
		}
		else
		{
			if (x <= upper_)
			{
				return_value = 1.0 - ((x - lower_)/(upper_ - lower_));
			}
			else
			{
				return_value = 0.0;
			}
		}
		return return_value;
	}

	float FresnoFF::BaseFunctionLinear::calculate(float x, float lower,
			float upper) 
		throw()
	{
		setLower(lower);
		setUpper(upper);
		return(calculate(x));
	}

	FresnoFF::BaseFunctionSigmoidal::BaseFunctionSigmoidal()
		throw()
		:	a_(0.0),
			b_(0.0)
	{
	}

	FresnoFF::BaseFunctionSigmoidal::BaseFunctionSigmoidal(float lower,
			float upper)
		throw()
	{
		// ????? see constructor of FresnoFF::BaseFunctionLinear
		// FresnoFF::BaseFunction::FresnoFF::BaseFunction(lower, upper);
		lower_ = lower;
		upper_ = upper;
		computeSigmoidParameters_();
	}

	void FresnoFF::BaseFunctionSigmoidal::setLower(float lower)
		throw()
	{
		FresnoFF::BaseFunction::setLower(lower);
		// computeSigmoidParameters_();
	}

	void FresnoFF::BaseFunctionSigmoidal::setUpper(float upper)
		throw()
	{
		FresnoFF::BaseFunction::setUpper(upper);
		// computeSigmoidParameters_();
	}

	float FresnoFF::BaseFunctionSigmoidal::calculate(float x) const
		throw()
	{
		return(1.0 / (1.0 + exp(-a_ * x + b_)));
	}

	float FresnoFF::BaseFunctionSigmoidal::calculate(float x, float lower,
			float upper)
		throw()
	{
		setLower(lower);
		setUpper(upper);
		computeSigmoidParameters_();
		return(calculate(x));
	}


	void FresnoFF::BaseFunctionSigmoidal::computeSigmoidParameters_()
		throw()
	{
		a_ = 4.0 / (lower_ - upper_);
		b_ = a_ * (lower_ + 0.5 * (upper_ - lower_));
	}


	const char* FresnoFF::Option::CONST = "const";
	const char* FresnoFF::Option::HB = "hb";
	const char* FresnoFF::Option::LIPO = "lipo";
	const char* FresnoFF::Option::ROT = "rot";
	const char* FresnoFF::Option::BP = "bp";
	const char* FresnoFF::Option::DESOLV = "desolv";
	const char* FresnoFF::Option::METAL = "metal";
	const char* FresnoFF::Option::NONPOLAR = "nonpolar";
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
	const char* FresnoFF::Option::ROT_METHOD = "rot_method";
	const char* FresnoFF::Option::METAL_R1 = "metal_r1";
	const char* FresnoFF::Option::METAL_R2 = "metal_r2";
	const char* FresnoFF::Option::DESOLV_METHOD = "desolvation_method";
	const char* FresnoFF::Option::DESOLV_GB = "use_gb";
	const char* FresnoFF::Option::DESOLV_AVG = "desolvation_averaging";
	const char* FresnoFF::Option::DESOLV_FOCUS_GRID_AROUND_LIGAND 
		= "desolv_focus_grid_around_ligand";
	const char* FresnoFF::Option::PROBE_RADIUS = "probe_radius";
	const char* FresnoFF::Option::SURFACE_TENSION = "surface_tesnion";
	const char* FresnoFF::Option::UHLIG_CONSTANT = "uhlig_constant";
	const char* FresnoFF::Option::SOLVENT_NUMBER_DENSITY 
		= "solvent_number_density";
	const char* FresnoFF::Option::ABSOLUTE_TEMPERATURE = "absolute_temperature";
	const char* FresnoFF::Option::NONPOLAR_METHOD = "nonpolar_method";
	const char* FresnoFF::Option::VERBOSITY = "verbosity";
	const char* FresnoFF::Option::SOLVENT_DESCRIPTOR_FILE 
		= "solvent_descriptor_file";
	const char* FresnoFF::Option::LJ_PARAM_FILE 
		= "lj_param_file";
	const char* FresnoFF::Option::ATOM_TYPE_FILE 
		= "atom_types_file";
	const char* FresnoFF::Option::BASE_FUNCTION_TYPE
		= "base_function_type";
	const char* FresnoFF::Option::GB_SCALING_FILE
		= "gb_scaling_file";


	// const float FresnoFF::Default::CONST = -33.614;
	// const float FresnoFF::Default::HB = -0.014;
	// const float FresnoFF::Default::LIPO = -0.076;
	// const float FresnoFF::Default::ROT = 0.017;
	// const float FresnoFF::Default::BP = 0.021;
	// const float FresnoFF::Default::DESOLV = 0.026;
	// const float FresnoFF::Default::METAL = -6.03;

	const float FresnoFF::Default::CONST = 1.0;
	const float FresnoFF::Default::HB = 1.0;
	const float FresnoFF::Default::LIPO = 1.0;
	const float FresnoFF::Default::ROT = 1.0;
	const float FresnoFF::Default::BP = 1.0;
	const float FresnoFF::Default::DESOLV = 1.0;
	const float FresnoFF::Default::METAL = 1.0;
	const float FresnoFF::Default::NONPOLAR = 1.0;

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
	const Size FresnoFF::Default::ROT_METHOD =
		FresnoRotation::CALCULATION__ORIGINAL;
	const float FresnoFF::Default::METAL_R1 = 2.2;
	const float FresnoFF::Default::METAL_R2 = 2.6;
	const Size FresnoFF::Default::DESOLV_METHOD = 0;
	const bool FresnoFF::Default::DESOLV_GB = false;
	const Size FresnoFF::Default::DESOLV_AVG = 0;
	const bool FresnoFF::Default::DESOLV_FOCUS_GRID_AROUND_LIGAND = true;
	const float FresnoFF::Default::PROBE_RADIUS = 0;
	const float FresnoFF::Default::SURFACE_TENSION = 0;
	const float FresnoFF::Default::UHLIG_CONSTANT = 0;
	const float FresnoFF::Default::SOLVENT_NUMBER_DENSITY = 0;
	const float FresnoFF::Default::ABSOLUTE_TEMPERATURE = 0;
	const Size FresnoFF::Default::NONPOLAR_METHOD = 0;
	const Size FresnoFF::Default::VERBOSITY = 0;
	const String FresnoFF::Default::SOLVENT_DESCRIPTOR_FILE 
		= "solvents/PCM-water.ini";
	const String FresnoFF::Default::LJ_PARAM_FILE 
		= "Amber/amber94.ini";
	const String FresnoFF::Default::ATOM_TYPE_FILE 
		= "Amber/amber94.types";
	const Size FresnoFF::Default::BASE_FUNCTION_TYPE
		= BASE_FUNCTION_TYPE__LINEAR;
	const String FresnoFF::Default::GB_SCALING_FILE
		= "gb_scaling.ini";

	void FresnoFF::registerComponents_()
		throw()
	{
		// create the component list
		// insertComponent(new FresnoConstant(*this));
		insertComponent(new FresnoHydrogenBond(*this));
		insertComponent(new FresnoBuriedPolar(*this));
		insertComponent(new FresnoLipophilic(*this));
		insertComponent(new FresnoRotation(*this));
		insertComponent(new FresnoDesolvation(*this));
		insertComponent(new ChemScoreMetal(*this));
		// insertComponent(new CharmmNonBonded(*this));
		insertComponent(new FresnoNonPolar(*this));
		insertComponent(new FresnoRingStacking(*this));
	}


	FresnoFF::FresnoFF()
		throw()
		:	ForceField(),
			protein_(0),
			ligand_(0),
			fresno_types_()
	{
		// register all components of the force field
		registerComponents_();

		// set the name
		setName("Fresno");
	}


	FresnoFF::FresnoFF(System& system)
		throw()
		:	ForceField(),
			protein_(0),
			ligand_(0),
			fresno_types_()
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


	FresnoFF::FresnoFF(System& system, Molecule* protein, Molecule* ligand)
		throw()
		:	ForceField(),
			protein_(protein),
			ligand_(ligand),
			fresno_types_()
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
			protein_(),
			ligand_(),
			fresno_types_()
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


	FresnoFF::FresnoFF(System& system, Molecule* protein, Molecule* ligand,
			const Options& new_options)
		throw()
		:	ForceField(),
			protein_(protein),
			ligand_(ligand),
			fresno_types_()
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
			protein_(),
			ligand_(),
			fresno_types_()
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
		protein_ = 0;
		ligand_ = 0;
		fresno_types_.clear();
		ForceField::clear();
	}


	bool FresnoFF::specificSetup()
		throw()
	{

		Size verbosity
			= options.setDefaultInteger(FresnoFF::Option::VERBOSITY,
					FresnoFF::Default::VERBOSITY);

		Size base_function_type 
			= options.setDefaultInteger(FresnoFF::Option::BASE_FUNCTION_TYPE,
					FresnoFF::Default::BASE_FUNCTION_TYPE);

		// check whether the system is assigned
		System* system = getSystem();
		if (system == 0)
		{
			Log.error() << "FresnoFF::specificSetup(): "
				<< "No system assigned, aborting" << endl;
			return false;
		}

		// check whether we have two molecules
		if (system->countMolecules() != 2)
		{
			Log.error() << "FresnoFF::specificSetup(): "
				<< "Fresno is only defined for systems with 2 molecules, aborting" 
				<< endl;
			return false;
		}

		// check whether we know which molecule is the protein and which is the
		// ligand in this complex. In case we don't know, we have to guess.
		// This can be wrong if we got two PDB files which will both be tagged
		// as protein.
		if ((protein_ == 0) || (ligand_ == 0))
		{
			Log.warn() 
				<< "I don't know what the protein and the ligand is, trying to guess." 
				<< endl;
			protein_ = &*(system->beginProtein());
			ligand_= system->getMolecule(0);
			if (ligand_ == protein_) 
			{
				ligand_ = system->getMolecule(1);
			}
		}
 
 		// Assign base function
		if (base_function_type == BASE_FUNCTION_TYPE__LINEAR)
		{
			base_function = new BaseFunctionLinear;
		}
		else
		{
			if (base_function_type == BASE_FUNCTION_TYPE__SIGMOIDAL)
			{
				base_function = new BaseFunctionSigmoidal;
			}
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
							if (verbosity >= 90)
							{
								Log.info() << it->first->getFullName() << ": ACC_DON" << endl;
							}
						}
						else
						{
							it->second = FresnoFF::HBOND_DONOR;
							++donor_counter;
							if (verbosity >= 90)
							{
								Log.info() << it->first->getFullName() << ": DON" << endl;
							}
						}
					}
				}
				else
				{
					if (atom->countBonds() < 3)
					{
						it->second = FresnoFF::HBOND_ACCEPTOR;
						++acc_counter;
						if (verbosity >= 90) 
						{
							Log.info() << it->first->getFullName() << ": ACC" << endl;
						}
					}
					else
					{
						it->second = FresnoFF::POLAR;
						++polar_counter;
						if (verbosity >= 90) 
						{
							Log.info() << it->first->getFullName() << ": POL" << endl;
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
						it->second = FresnoFF::HBOND_HYDROGEN;
						++hyd_counter;
						if (verbosity >= 90) 
						{
							Log.info() << it->first->getFullName() << ": HYD" << endl;
						}
					}
					connectedTo.setArgument("(N)");
					if (connectedTo(*atom))
					{
						it->second = FresnoFF::HBOND_HYDROGEN;
						++hyd_counter;
						if (verbosity >= 90) 
						{
							Log.info() << it->first->getFullName() << ": HYD" << endl;
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
							if (assignment_type == FresnoFF::ASSIGNMENT__ELDRIDGE)
							{
								it->second = FresnoFF::HBOND_ACCEPTOR_DONOR;
								++acc_don_counter;
								if (verbosity >= 90)
								{
									Log.info() << it->first->getFullName() << ": DON" << endl;
								}
							}
						}
						else
						{
							it->second = FresnoFF::HBOND_ACCEPTOR;
							++acc_counter;
							if (verbosity >= 90)
							{
								Log.info() << it->first->getFullName() << ": ACC" << endl;
							}
						}
					}
					else
					{
						if ((symbol == "Fl") || (symbol == "P"))
						{
							it->second = FresnoFF::POLAR;
							if (verbosity >= 90)
							{
								Log.info() << it->first->getFullName() << ": POL" << endl;
							}
							++polar_counter;
						}
						else
						{
							if (symbol == "S")
							{
								if (atom->countBonds() == 1)
								{
									it->second = FresnoFF::POLAR;
									if (verbosity >= 90)
									{
										Log.info() << it->first->getFullName() << ": POL" << endl;
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
										it->second = FresnoFF::POLAR;
										if (verbosity >= 90)
										{
											Log.info() << it->first->getFullName() << ": POL" << endl;
										}
										++polar_counter;
									}
									else
									{
										connectedTo.setArgument("(#N)");
										if (connectedTo(*atom))
										{
											it->second = FresnoFF::POLAR;
											if (verbosity >= 90)
											{
												Log.info() << it->first->getFullName() << ": POL" << endl;
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
											it->second = FresnoFF::LIPOPHILIC;
											if (verbosity >= 90)
											{
												Log.info() << it->first->getFullName() << ": LIP" << endl;
											}
											++lipo_counter;
										}
										else
										{
											it->second = FresnoFF::HBOND_ACCEPTOR;
											if (verbosity >= 90)
											{
												Log.info() << it->first->getFullName() << ": ACC" << endl;
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
			Log.info() << "STEP 1 statistics:" << endl << endl;
			Log.info() << "lipophilic atoms:        " << lipo_counter << endl;
			Log.info() << "h bond acceptors:        " << acc_counter << endl;
			Log.info() << "h bond acceptors/donors: " << acc_don_counter << endl;
			Log.info() << "h bond donors:           " << donor_counter << endl;
			Log.info() << "h bond hydrogens:        " << hyd_counter << endl;
			Log.info() << "polar atoms:             " << polar_counter << endl;
			Log.info() << "metal atoms:             " << metal_counter << endl;
			Log.info() << "remaining atoms:         " 
				<< getSystem()->countAtoms() - lipo_counter - acc_counter 
				- acc_don_counter - donor_counter - polar_counter - hyd_counter
				- metal_counter
				<< endl << endl;
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
									if (verbosity >= 90)
									{
										Log.info() << it->first->getFullName() << ": POL" << endl;
									}
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
											if (verbosity >= 90)
											{
												Log.info() << it->first->getFullName() << ": POL" << endl;
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

		if (verbosity > 8)
		{
			Log.info() << "STEP 2 statistics:" << endl << endl;
			Log.info() << "lipophilic atoms:        " << lipo_counter << endl;
			Log.info() << "h bond acceptors:        " << acc_counter << endl;
			Log.info() << "h bond acceptors/donors: " << acc_don_counter << endl;
			Log.info() << "h bond donors:           " << donor_counter << endl;
			Log.info() << "h bond hydrogens:        " << hyd_counter << endl;
			Log.info() << "polar atoms:             " << polar_counter << endl;
			Log.info() << "metal atoms:             " << metal_counter << endl;
			Log.info() << "remaining atoms:         " 
				<< getSystem()->countAtoms() - lipo_counter - acc_counter 
				- acc_don_counter - donor_counter - polar_counter - hyd_counter
				- metal_counter
				<< endl << endl;
		}

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
					if (verbosity >= 90)
					{
						Log.info() << it->first->getFullName() << ": LIP" << endl;
					}
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
						if (verbosity >= 90)
						{
							Log.info() << it->first->getFullName() << ": LIP" << endl;
						}
						++lipo_counter;
					}
				}
			}
		}

		if (verbosity > 8)
		{
			Log.info() << "STEP 3 statistics:" << endl << endl;
			Log.info() << "lipophilic atoms:        " << lipo_counter << endl;
			Log.info() << "h bond acceptors:        " << acc_counter << endl;
			Log.info() << "h bond acceptors/donors: " << acc_don_counter << endl;
			Log.info() << "h bond donors:           " << donor_counter << endl;
			Log.info() << "h bond hydrogens:        " << hyd_counter << endl;
			Log.info() << "polar atoms:             " << polar_counter << endl;
			Log.info() << "metal atoms:             " << metal_counter << endl;
			Log.info() << "remaining atoms:         " 
				<< getSystem()->countAtoms() - lipo_counter - acc_counter 
				- acc_don_counter - donor_counter - polar_counter - hyd_counter
				- metal_counter
				<< endl << endl;
		}

		// DEBUG
		/*
		for (it = fresno_types_.begin(); +it; ++it)
		{
			atom = it->first;
			Log.info() << atom->getFullName() << ": ";
			String type;
			switch(it->second)
			{
				case FresnoFF::UNKNOWN: 
					type = "unknown";
					break;
				case FresnoFF::LIPOPHILIC: 
					type = "lipophilic";
					break;
				case FresnoFF::HBOND_DONOR: 
					type = "h bond donor";
					break;
				case FresnoFF::HBOND_ACCEPTOR: 
					type = "h bond acceptor";
					break;
				case FresnoFF::HBOND_ACCEPTOR_DONOR: 
					type = "h bond acceptor/donor";
					break;
				case FresnoFF::HBOND_HYDROGEN: 
					type = "h bond hydrogen";
					break;
				case FresnoFF::POLAR: 
					type = "polar";
					break;
				case FresnoFF::METAL: 
					type = "metal";
					break;
				default: 
					type = "unassigned";
			}
			Log.info() << type << endl;
		}
		*/
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


	void FresnoFF::setProtein(Molecule* protein)
		throw()
	{
		protein_ = protein;
	}


	void FresnoFF::setLigand(Molecule* ligand)
		throw()
	{
		ligand_ = ligand;
	}


	Molecule* FresnoFF::getProtein() const
		throw()
	{
		return protein_;
	}


	Molecule* FresnoFF::getLigand() const
		throw()
	{
		return ligand_;
	}


	double FresnoFF::getHydrogenBondEnergy() const
		throw()
	{
		ForceFieldComponent* component = getComponent("Fresno HydrogenBond");
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
		ForceFieldComponent* component = getComponent("Fresno RotationalEntropyLoss");
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
		ForceFieldComponent* component = getComponent("Fresno BuriedPolar");
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


	double FresnoFF::getNonPolarEnergy() const
		throw()
	{
		ForceFieldComponent* component = getComponent("Fresno NonPolar");
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


	String FresnoFF::getFresnoTypeString(short type) const
		throw()
	{
		String string;
		switch(type)
		{
			case FresnoFF::UNKNOWN: 
				string = "UNKNOWN"; break;
			case FresnoFF::LIPOPHILIC: 
				string = "LIPOPHILIC"; break;
			case FresnoFF::HBOND_DONOR: 
				string = "HBOND_DONOR"; break;
			case FresnoFF::HBOND_ACCEPTOR_DONOR: 
				string = "HBOND_ACCEPTOR_DONOR"; break;
			case FresnoFF::HBOND_ACCEPTOR: 
				string = "HBOND_ACCEPTOR"; break;
			case FresnoFF::HBOND_HYDROGEN: 
				string = "HBOND_HYDROGEN"; break;
			case FresnoFF::POLAR: 
				string = "POLAR"; break;
			case FresnoFF::METAL: 
				string = "METAL"; break;
			default: 
				string = "ARGH";
		}
		return string;
	}

	String FresnoFF::getFresnoTypeString(const Atom* atom) const
		throw()
	{
		return(getFresnoTypeString(fresno_types_[atom]));
	}
	

} // namespace BALL
