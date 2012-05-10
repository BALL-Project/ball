// $Id: SLICK.C,v 1.1 2006/05/21 18:17:15 anker Exp $
//
// Implement the SLICK scoring function

#include <BALL/SCORING/FUNCTIONS/slickEnergy.h>
#include <BALL/SCORING/FUNCTIONS/slickScore.h>
#include <BALL/SCORING/COMPONENTS/vanDerWaalsSlick.h>
#include <BALL/SCORING/COMPONENTS/polarSolvation.h>
#include <BALL/SCORING/COMPONENTS/nonpolarSolvation.h>

#include <BALL/FORMAT/HINFile.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/STRUCTURE/residueChecker.h>
#include <BALL/STRUCTURE/defaultProcessors.h>
#include <BALL/MOLMEC/COMMON/radiusRuleProcessor.h>
#include <BALL/MOLMEC/COMMON/chargeRuleProcessor.h>
#include <BALL/MOLMEC/COMMON/assignTypes.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/DATATYPE/stringHashSet.h>

#include <BALL/KERNEL/PTE.h>

#include <ctime>

#define OLD_DATAFILE_FORMAT 1

using namespace BALL;
using namespace std;

void usage(const String& name) 
{
	Log.error() << name << endl
		<< " What do you want to calculate?" << endl
		<< "  -E           compute SLICK/energy" << endl
		<< "  -S           compute SLICK/score" << endl
		<< endl
		<< " Structure handling:" << endl
		<< "  -p <FILE>    use FILE as receptor (PDB format)" << endl
//		<< "  -l <FILE>    use FILE as ligand (PDB format)" << endl
		<< "  -L <FILE>    use FILE as ligand (HIN format)" << endl
		<< endl
		<< " Options for the VDW component:" << endl
		<< "  -w <FILE>    use FILE for LJ parameters" << endl
		<< endl
		<< " Options for electrostatics (including es solvation):" << endl
		<< "  -c <FILE>    read charge rules from FILE" << endl
		<< "  -r <FILE>    read radius rules for the polar solvation from FILE" << endl
		<< "  -s <FACTOR>  use FACTOR as scaling factor for receptor charges" << endl
		<< "  -t <FACTOR>  use FACTOR as scaling factor for ligand charges" << endl
		<< "  -g           use Generalized Born in solvation calc. (only SLICK/energy)" << endl
		<< "  -u           unite atoms" << endl
		<< "  -o           overwrite ligand charges from rules" << endl
		<< endl
		<< " Options for nonpolar solvation:" << endl
		<< "  -n <FILE>    read radius rules for the nonpolar solvation from FILE" << endl
		<< "  -N <FACTOR>  scale nonpolar radii by FACTOR" << endl
		<< endl
		<< " Common options:" << endl
		<< "  -v <VERBOSITY> set the verbosity to level VERBOSITY" << endl
		<< "  -O <FILE>    read options from FILE (command line overrides!)" << endl
		<< endl;
}


int main(int argc, char** argv)
{

	if (argc == 1)
	{
		usage(argv[0]);
		return 1;
	}

	// define all the variables for the UI

	String protein_file_name;
	String ligand_file_name;
	String polar_radius_rule_file_name("solvation/PARSE+ions.rul");
	String nonpolar_radius_rule_file_name("solvation/bondi.rul");
	String charge_rule_file_name("solvation/PARSE+ions.rul");
	String lj_parameter_file_name("Amber/amber94gly.ini");
	String options_file_name;

	String tmp;

	bool calculate_energy = false;
	bool calculate_score = false;
	bool use_hin_ligand = false;
	bool use_polar_radius_rules = false;
	bool use_nonpolar_radius_rules = false;
	bool use_charge_rules = false;
	bool overwrite_ligand_charges = false;
	bool scale_receptor_charges = false;
	bool scale_ligand_charges = false;
	bool scale_nonpolar_radii = false;
	bool write_logfile = true;
	bool use_generalized_born = false;
	bool unite_atoms = false;
	bool read_options_file = false;

	Size verbosity = 0;

	float ligand_charge_scaling_factor = 1.0f;
	float receptor_charge_scaling_factor = 1.0f;
	float nonpolar_radii_scaling_factor = 1.0f;

	Path path;

	String configuration_string("");

	// check all command line options, stolen from PB

	for (int i = 1; i < argc; i++)
	{
		// retrieve the current option
		String option(argv[i]);

		// check for leading "-" and length of argument
		if ((option[0] != '-') || (option.size() != 2))
		{
			// print usage hints and an error message, exit
			usage(argv[0]);
			Log.error() << "Illegal option: " << option << endl;
			return 2;
		}

		// check for another argument for those 
		// options requiring a filename (-h)
		if (String("plLwrcstnmvO").has(option[1]) && (i == (argc - 1)))
		{
			// pring usage hints, an error message, exit
			usage(argv[0]);
			Log.error() << "Option " << option << " requires an additional argument." 
				<< endl;
			return 3;
		}		

		// interpret all command line options
		switch (option[1])
		{

			// Calculate SLICK/energy
			case 'E':
				if (calculate_score == true)
				{
					Log.error() << "Cannot calculate score and energy at the same time"
						<< endl;
					return(1);
				}
				calculate_energy = true;
				configuration_string = configuration_string
					+ "calculate_energy\n";
				break;

			// Calculate SLICK/score
			case 'S':
				if (calculate_energy == true)
				{
					Log.error() << "Cannot calculate score and energy at the same time"
						<< endl;
					return(1);
				}
				calculate_score = true;
				configuration_string = configuration_string
					+ "calculate_score\n";
				break;

			// Read the protein from a PDB file
			case 'p':
				protein_file_name = argv[++i];
				configuration_string = configuration_string
					+ "protein_file_name " + protein_file_name + "\n";
				break;

			// Read the ligand from a PDB file
			case 'l':
				use_hin_ligand = false;
				ligand_file_name = argv[++i];
				configuration_string = configuration_string
					+ "use_hin_ligand false\n"
					+ "ligand_file_name " + ligand_file_name + "\n";
				break;

			// Read the ligand from a HIN file
			case 'L':
				use_hin_ligand = true;
				ligand_file_name = argv[++i];
				configuration_string = configuration_string
					+ "use_hin_ligand true\n"
					+ "ligand_file_name " + ligand_file_name + "\n";
				break;

			// VDW: LJ parameter file
			case 'w': 
				lj_parameter_file_name = argv[++i];
				configuration_string = configuration_string 
					+ "lj_parameter_file_name " + lj_parameter_file_name;
				break;

			// ES: radius rule file
			case 'r':
				use_polar_radius_rules = true;
				polar_radius_rule_file_name = argv[++i];
				configuration_string = configuration_string
					+ "use_polar_radius_rules " + polar_radius_rule_file_name + "\n";
				break;

			// ES: charge rule file
			case 'c':
				use_charge_rules = true;
				charge_rule_file_name = argv[++i];
				configuration_string = configuration_string
					+ "use_charge_rules " + charge_rule_file_name + "\n";
				break;

			// ES: scale receptor charges
			case 's':
				scale_receptor_charges = true;
				receptor_charge_scaling_factor = atof(argv[++i]);
				configuration_string = configuration_string
					+ "scale_receptor_charges " 
					+ String(receptor_charge_scaling_factor) + "\n";
				break;

			// ES: scale ligand charges
			case 't':
				scale_ligand_charges = true;
				ligand_charge_scaling_factor = atof(argv[++i]);
				configuration_string = configuration_string
					+ "scale_ligand_charges " 
					+ String(ligand_charge_scaling_factor) + "\n";
				break;

			// ES: overwrite ligand charges if it's a HIN file
			case 'o':
				overwrite_ligand_charges = true;
				configuration_string = configuration_string
					+ "overwrite_ligand_charges true\n";
				break;

			// ES: overwrite ligand charges if it's a HIN file
			case 'g':
				use_generalized_born = true;
				configuration_string = configuration_string
					+ "use_generalized_born true\n";
				break;

			// ES: unite atoms (eg in the PARSE case)
			case 'u':		// 
				unite_atoms = true;
				configuration_string = configuration_string
					+ "unite_atoms true\n";
				break;

			// NP: read radius rules
			case 'n':
				use_nonpolar_radius_rules = true;
				nonpolar_radius_rule_file_name = argv[++i];
				configuration_string = configuration_string
					+ "use_nonpolar_radius_rules " + nonpolar_radius_rule_file_name + "\n";
				break;

			// NP: scale nonpolar radii
			case 'm':		// 
				scale_nonpolar_radii = true;
				nonpolar_radii_scaling_factor = atof(argv[++i]);
				configuration_string = configuration_string
					+ "scale_nonpolar_radii " 
					+ String(receptor_charge_scaling_factor) + "\n";
				break;

			// set verbosity 
			case 'v':
				verbosity = atoi(argv[++i]);
				Log.info() << "Setting verbosity level to " << verbosity << endl;
				break;

			// define options file
			case 'O':
				read_options_file = true;
				options_file_name = argv[++i];
				configuration_string = configuration_string + "read_options_file"
					+ ' ' + options_file_name;
				break;

			default:		// unknown option
				// print usage hints and an error message, exit
				usage(argv[0]);
				Log.error() << "Illegal option: " << option << endl;
				return 2;
		}

	}

	// ********************************************************************

	time_t rawtime;
	time(&rawtime);
	String time_string("start: " + String(asctime(localtime(&rawtime))));
	
	// We need to know what to do
	/*
	if ((calculate_energy ^ calculate_score) == false)
	{
		Log.error() << "Please select the task: choose -E or -S" << endl;
		return(1);
	}
	*/

	// if we don't have a receptor and/or a ligand, we can't compute something
	if (protein_file_name == "")
	{
		Log.error() << "Missing protein file name." << endl;
		return 1;
	}
	if (ligand_file_name == "")
	{
		Log.error() << "Missing ligand file name." << endl;
		return 1;
	}

	if (verbosity > 0) Log.info() << "Initializing residue checker." << endl;
	FragmentDB db("fragments/Fragments.db");
	ResidueChecker check(db);

	// now load the molecules and assign radii and charges depending on
	// what the user wants
	System system;

	PDBFile protein_file(protein_file_name);
	protein_file >> system;
	protein_file.close();

	Molecule* protein = system.getMolecule(0);
	Molecule* ligand = 0;

	if (verbosity > 0) Log.info() << "Normalizing names (protein)." << endl;
	system.apply(db.normalize_names);
	if (verbosity > 0) Log.info() << "Building bonds (protein)." << endl;
	system.apply(db.build_bonds);

	HINFile ligand_hin_file;
	PDBFile ligand_pdb_file;
	
	if ((use_hin_ligand == true) && (overwrite_ligand_charges == true))
	{
		ligand_hin_file.open(ligand_file_name);
		ligand_hin_file >> system;
		ligand_hin_file.close();
		ligand = system.getMolecule(1);
	}

	if (use_hin_ligand == false)
	{
		ligand_pdb_file.open(ligand_file_name);
		ligand_pdb_file >> system;
		ligand_pdb_file.close();
		ligand = system.getMolecule(1);
		if (verbosity > 0) Log.info() << "Normalizing names (ligand)." << endl;
		system.apply(db.normalize_names);
		if (verbosity > 0) Log.info() << "Building bonds (protein)." << endl;
		system.apply(db.build_bonds);
	}

	if (use_charge_rules == true)
	{
		tmp = path.find(charge_rule_file_name);
		if (tmp != "") charge_rule_file_name = tmp;
		if (verbosity > 0)
		{
			Log.info() << "Using " << charge_rule_file_name 
				<< " as charge rule file" << endl;
		}
		INIFile charge_ini(charge_rule_file_name);
		charge_ini.read();
		ChargeRuleProcessor charge_rules(charge_ini);
		system.apply(charge_rules);
	}

	if ((use_hin_ligand == true) && (overwrite_ligand_charges == false))
	{
		ligand_hin_file.open(ligand_file_name);
		ligand_hin_file >> system;
		ligand_hin_file.close();
		ligand = system.getMolecule(1);
	}

	if (use_polar_radius_rules == true)
	{
		tmp = path.find(polar_radius_rule_file_name);
		if (tmp != "") polar_radius_rule_file_name = tmp;
		if (verbosity > 0)
		{
			Log.info() << "Using " << polar_radius_rule_file_name 
				<< " as polar radius rule file" << endl;
		}
		INIFile radius_ini(polar_radius_rule_file_name);
		radius_ini.read();
		RadiusRuleProcessor radius_rules(radius_ini);
		system.apply(radius_rules);
	}


	if (verbosity > 8)
	{
		Log.info() << "system statistics:" << endl;
		Log.info() << "molecules: " << system.countMolecules() << endl;
		Log.info() << "proteins:  " << system.countProteins() << endl;
		Log.info() << "fragments: " << system.countFragments() << endl;
		Log.info() << "atoms:     " << system.countAtoms() << endl;
	}

	// check for uassigned type names
	AtomIterator it;
	int count = 0;
	for (it = system.beginAtom(); +it; ++it)
	{
		count++;
		if (it->getElement().getSymbol() == "?")
		{
			Log.info() << "Got symbol \"?\": " << it->getFullName() << endl;
		}
		if (it->getCharge() > 8.0)
		{
			Log.error() << "Mysterious charge: " << it->getCharge() << "\t" 
				<< it->getFullName() << "\t" << count 
				<< "\t" << it->getElement().getSymbol() << endl;
		}
	}

	// scale charges
	if (verbosity > 0) 
	{
		Log.info() << "Scaling receptor charges by " 
			<< receptor_charge_scaling_factor << endl;
	}
	if (scale_receptor_charges == true)
	{
		for (it = protein->beginAtom(); +it; ++it)
		{
			it->setCharge(it->getCharge() * receptor_charge_scaling_factor);
		}
	}

	if (verbosity > 0) 
	{
		Log.info() << "Scaling ligand charges by " 
			<< ligand_charge_scaling_factor << endl;
	}
	if (scale_ligand_charges == true)
	{
		for (it = ligand->beginAtom(); +it; ++it)
		{
			it->setCharge(it->getCharge() * ligand_charge_scaling_factor);
		}
	}

	// if (verbosity > 0) { Log.info() << "Checking structure." << endl; }
	// system.apply(check);

	Options options;
	if (read_options_file == true)
	{
		options.readOptionFile(options_file_name);
	}

	options.set(VanDerWaals::Option::LENNARD_JONES_FILE, lj_parameter_file_name);
	options.setInteger(ScoringFunction::Option::VERBOSITY, verbosity);

	if (unite_atoms == true)
	{
		options.setBool(PolarSolvation::Option::UNITE_ATOMS, true);
		cout << "Uniting atoms." << endl;
	}
	else
	{
		options.setBool(PolarSolvation::Option::UNITE_ATOMS, false);
		cout << "Not uniting atoms." << endl;
	}


	if (use_nonpolar_radius_rules == true)
	{
		cout << "Using " << nonpolar_radius_rule_file_name << " for nonpolar radii" << endl;
		options.setBool(NonpolarSolvation::Option::NONPOLAR_OVERWRITE_RADII, true);
		options.set(NonpolarSolvation::Option::NONPOLAR_RADIUS_RULES, nonpolar_radius_rule_file_name);
	}

	
	float score = 0.0f;

	if (calculate_energy == true)
	{
		SLICKEnergy slick(*protein, *ligand, options);
		score = slick.calculateScore();
		cout << endl << "SLICK/energy is " << score << " kJ/mol" << endl;

		if (verbosity > 1)
		{
			cout << endl << "Scores (w/o coefficients)" << endl << endl;
			cout << "Hydrogen bonds     " << slick.getHydrogenBondScore() << endl;
			cout << "CHPI               " << slick.getCHPIScore() << endl;
			cout << "Van-der-Waals      " << slick.getVDWScore() << endl;
			cout << "Nonpolar solvation " << slick.getNonpolarSolvationScore() << endl;
			cout << "Polar Solvation    " << slick.getPolarSolvationScore() << endl;
		}

#ifdef OLD_DATAFILE_FORMAT
		String dat_file_name;
		String component;
		File datfile;

		component = "SLICKenergy";
		dat_file_name = component + ".dat";
		datfile.open(dat_file_name, std::ios::out);
		datfile << protein_file_name << " " << ligand_file_name << " " << score 
			<< " " << component << endl;
		datfile.close();

		component = "HB";
		dat_file_name = component + ".dat";
		datfile.open(dat_file_name, std::ios::out);
		datfile << protein_file_name << " " << ligand_file_name << " " 
			<< slick.getHydrogenBondScore()
			<< " " << component << endl;
		datfile.close();

		component = "CHPI";
		dat_file_name = component + ".dat";
		datfile.open(dat_file_name, std::ios::out);
		datfile << protein_file_name << " " << ligand_file_name << " " 
			<< slick.getCHPIScore() 
			<< " " << component << endl;
		datfile.close();

		component = "VDW5";
		dat_file_name = component + ".dat";
		datfile.open(dat_file_name, std::ios::out);
		datfile << protein_file_name << " " << ligand_file_name << " " 
			<< slick.getVDWScore() 
			<< " " << component << endl;
		datfile.close();

		component = "NONPOLAR2";
		dat_file_name = component + ".dat";
		datfile.open(dat_file_name, std::ios::out);
		datfile << protein_file_name << " " << ligand_file_name << " " 
			<< slick.getNonpolarSolvationScore() 
			<< " " << component << endl;
		datfile.close();

		component = "DESOLV4";
		dat_file_name = component + ".dat";
		datfile.open(dat_file_name, std::ios::out);
		datfile << protein_file_name << " " << ligand_file_name << " " 
			<< slick.getPolarSolvationScore()
			<< " " << component << endl;
		datfile.close();
#endif

		time(&rawtime);
		time_string = time_string
			+ String("stop:  " + String(asctime(localtime(&rawtime))));

		if (write_logfile == true)
		{
			File logfile("SLICKenergy.log", std::ios::out);
			logfile << configuration_string << endl;
			logfile << time_string << endl;
			logfile.close();
		}

	}

	if (calculate_score == true)
	{
		SLICKScore slick(*protein, *ligand, options);
		score = slick.calculateScore();
		cout << endl << "SLICK/score is " << score << " (arb. units)" << endl;

		if (verbosity > 1)
		{
			cout << endl << "Scores (w/o coefficients)" << endl << endl;
			cout << "Hydrogen bonds     " << slick.getHydrogenBondScore() << endl;
			cout << "CHPI               " << slick.getCHPIScore() << endl;
			cout << "Van-der-Waals      " << slick.getVDWScore() << endl;
			cout << "Electrostatic int. " << slick.getPolarSolvationScore() << endl;
		}

#ifdef OLD_DATAFILE_FORMAT
		String dat_file_name;
		String component;
		File datfile;

		component = "SLICKenergy";
		dat_file_name = component + ".dat";
		datfile.open(dat_file_name, std::ios::out);
		datfile << protein_file_name << " " << ligand_file_name << " " << score 
			<< " " << component << endl;
		datfile.close();

		component = "HB";
		dat_file_name = component + ".dat";
		datfile.open(dat_file_name, std::ios::out);
		datfile << protein_file_name << " " << ligand_file_name << " " 
			<< slick.getHydrogenBondScore()
			<< " " << component << endl;
		datfile.close();

		component = "CHPI";
		dat_file_name = component + ".dat";
		datfile.open(dat_file_name, std::ios::out);
		datfile << protein_file_name << " " << ligand_file_name << " " 
			<< slick.getCHPIScore() 
			<< " " << component << endl;
		datfile.close();

		component = "VDW5";
		dat_file_name = component + ".dat";
		datfile.open(dat_file_name, std::ios::out);
		datfile << protein_file_name << " " << ligand_file_name << " " 
			<< slick.getVDWScore() 
			<< " " << component << endl;
		datfile.close();

		component = "COULOMB";
		dat_file_name = component + ".dat";
		datfile.open(dat_file_name, std::ios::out);
		datfile << protein_file_name << " " << ligand_file_name << " " 
			<< slick.getPolarSolvationScore()
			<< " " << component << endl;
		datfile.close();
#endif

		time(&rawtime);
		time_string = time_string
			+ String("stop:  " + String(asctime(localtime(&rawtime))));

		if (write_logfile == true)
		{
			File logfile("SLICKscore.log", std::ios::out);
			logfile << configuration_string << endl;
			logfile << time_string << endl;
			logfile.close();
		}

	}


}
