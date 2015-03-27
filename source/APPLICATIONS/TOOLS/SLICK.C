// $Id: SLICK.C,v 1.1 2006/05/21 18:17:15 anker Exp $
//
// Implement the SLICK scoring function

#include <BALL/SCORING/FUNCTIONS/slickEnergy.h>
#include <BALL/SCORING/FUNCTIONS/slickScore.h>
#include <BALL/SCORING/COMPONENTS/vanDerWaalsSlick.h>
#include <BALL/SCORING/COMPONENTS/polarSolvation.h>
#include <BALL/SCORING/COMPONENTS/nonpolarSolvation.h>

#include <BALL/FORMAT/commandlineParser.h>
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
#include "version.h"

#include <BALL/KERNEL/PTE.h>

#include <ctime>

#define OLD_DATAFILE_FORMAT 1


using namespace BALL;
using namespace std;


int main(int argc, char** argv)
{
	Path path;
	String tmp;
	String configuration_string = "";

	CommandlineParser parpars("SLICK", "scoring protein-carbohydrate interactions", VERSION, String(__DATE__), "Scoring");
	parpars.registerFlag("E", "compute only SLICKEnergy");
	parpars.registerFlag("S", "compute only SLICKScore");
	parpars.registerFlag("u", "unite atoms");
	parpars.registerFlag("n", "read radius rules for the nonpolar solvation from FILE");
	parpars.registerFlag("N", "scale nonpolar radii by FACTOR");
	parpars.registerFlag("log", "write log file");
	parpars.registerMandatoryInputFile("rec", "input receptor file");
	parpars.registerMandatoryInputFile("lig", "input ligand file");
	parpars.registerOptionalInputFile("cr", "charge rules");
	parpars.registerOptionalInputFile("pr", "radius rules for the polar solvation");
	parpars.registerOptionalInputFile("lj", "use FILE for LJ parameters");
	parpars.registerOptionalInputFile("op", "read options from FILE (command line overrides!)");
	parpars.registerOptionalIntegerParameter("v", "verbosity to level");
	parpars.registerOptionalDoubleParameter("s", "scaling factor for receptor charges");
	parpars.registerOptionalDoubleParameter("t", "scaling factor for ligand charges");
	parpars.setSupportedFormats("rec","PDB");
	parpars.setSupportedFormats("lig","HIN");
	parpars.setSupportedFormats("cr","rul");
	parpars.setSupportedFormats("pr","rul");
	parpars.setSupportedFormats("lj","rul");
	parpars.setSupportedFormats("op", "ini");

	String man = "This tool calculates the SLICKEnergy / SLICK Score for protein-carbohydrate complexes.";
	parpars.setToolManual(man);

	parpars.parse(argc, argv);

	Options options;
	ScoringFunction::getDefaultOptions(options);

	unsigned int verbosity = 0;
	if (parpars.get("v") != CommandlineParser::NOT_FOUND)
	{
		verbosity = parpars.get("v").toInt();
		options.setInteger("verbosity", verbosity);
	}

	float receptor_charge_scaling_factor = 1.0f;
	if (parpars.get("s") != CommandlineParser::NOT_FOUND)
	{
		receptor_charge_scaling_factor = parpars.get("s").toFloat();
		configuration_string += "scale_receptor_charges " + parpars.get("s") + "\n";
	}

	float ligand_charge_scaling_factor = 1.0f;
	if (parpars.get("t") != CommandlineParser::NOT_FOUND)
	{
		ligand_charge_scaling_factor = parpars.get("t").toFloat();
		configuration_string += "scale_ligand_charges " + parpars.get("t") + "\n";
	}

	String protein_file_name = parpars.get("rec");
	if (protein_file_name == CommandlineParser::NOT_FOUND)
	{
		Log.error() << "Missing protein file name." << endl;
		return 1;
	}
	configuration_string += "protein_file_name " + protein_file_name + "\n";

	String ligand_file_name = parpars.get("lig");
	if (ligand_file_name == CommandlineParser::NOT_FOUND)
	{
		Log.error() << "Missing ligand file name." << endl;
		return 1;
	}
	configuration_string += "ligand_file_name " + ligand_file_name + "\n";

	if (verbosity > 0)
	{
		Log.info() << "Initializing residue checker." << endl;
	}
	FragmentDB db("fragments/Fragments.db");
	ResidueChecker check(db);


	time_t rawtime;
	time(&rawtime);
	String time_string("start: " + String(asctime(localtime(&rawtime))));

	// now load the molecules and assign radii and charges depending on
	// what the user wants
	System system;

	PDBFile protein_file(protein_file_name);
	protein_file >> system;
	protein_file.close();
	Molecule* protein = system.getMolecule(0);

	if (verbosity > 0)
	{
		Log.info() << "Normalizing names (protein)." << endl;
	}
	system.apply(db.normalize_names);

	if (verbosity > 0)
	{
		Log.info() << "Building bonds (protein)." << endl;
	}
	system.apply(db.build_bonds);

	HINFile ligand_hin_file;
	ligand_hin_file.open(ligand_file_name);
	ligand_hin_file >> system;
	ligand_hin_file.close();
	Molecule* ligand = system.getMolecule(1);


	// Read and apply charge rules
	String charge_rule_file_name = parpars.get("cr");
	if (charge_rule_file_name == CommandlineParser::NOT_FOUND)
	{
		charge_rule_file_name = "solvation/PARSE+ions.rul";
	}
	else
	{
		configuration_string += "use_charge_rules " + charge_rule_file_name + "\n";

		tmp = path.find(charge_rule_file_name);
		if (tmp != "")
		{
			charge_rule_file_name = tmp;
		}

		if (verbosity > 0)
		{
			Log.info() << "Using " << charge_rule_file_name << " as charge rule file" << endl;
		}

		INIFile charge_ini(charge_rule_file_name);
		charge_ini.read();
		ChargeRuleProcessor charge_rules(charge_ini);
		system.apply(charge_rules);
	}


	// Read and apply polar charge rules
	String polar_radius_rule_file_name = parpars.get("pr");
	if (polar_radius_rule_file_name == CommandlineParser::NOT_FOUND)
	{
		polar_radius_rule_file_name = "solvation/PARSE+ions.rul";
	}
	else
	{
		configuration_string += "use_polar_radius_rules " + polar_radius_rule_file_name + "\n";

		tmp = path.find(polar_radius_rule_file_name);
		if (tmp != "")
		{
			polar_radius_rule_file_name = tmp;
		}
		if (verbosity > 0)
		{
			Log.info() << "Using " << polar_radius_rule_file_name << " as polar radius rule file" << endl;
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
				<< it->getFullName() << "\t" << count << "\t" << it->getElement().getSymbol() << endl;
		}
	}

	// scale charges
	if (verbosity > 0)
	{
		Log.info() << "Scaling receptor charges by " << receptor_charge_scaling_factor << endl;
	}
	if (parpars.has("s"))
	{
		for (it = protein->beginAtom(); +it; ++it)
		{
			it->setCharge(it->getCharge() * receptor_charge_scaling_factor);
		}
	}

	if (verbosity > 0)
	{
		Log.info() << "Scaling ligand charges by " << ligand_charge_scaling_factor << endl;
	}
	if (parpars.has("t"))
	{
		for (it = ligand->beginAtom(); +it; ++it)
		{
			it->setCharge(it->getCharge() * ligand_charge_scaling_factor);
		}
	}


	String options_file_name = parpars.get("op");
	if (options_file_name != CommandlineParser::NOT_FOUND)
	{
		options.readOptionFile(options_file_name);
		configuration_string += "read_options_file " + options_file_name;
	}

	String lj_parameter_file_name = parpars.get("lj");
	if (lj_parameter_file_name == CommandlineParser::NOT_FOUND)
	{
		lj_parameter_file_name = "Amber/amber94gly.ini";
	}
	else
	{
		configuration_string = configuration_string + "lj_parameter_file_name " + lj_parameter_file_name;
	}

	options.set(VanDerWaalsSlick::Option::LENNARD_JONES_FILE, lj_parameter_file_name);

	if (parpars.has("u"))
	{
		options.setBool(PolarSolvation::Option::UNITE_ATOMS, true);
		cout << "Uniting atoms." << endl;
		configuration_string += "unite_atoms true\n";
	}
	else
	{
		options.setBool(PolarSolvation::Option::UNITE_ATOMS, false);
		cout << "Not uniting atoms." << endl;
	}

	String nonpolar_radius_rule_file_name = parpars.get("n");
	if (nonpolar_radius_rule_file_name == CommandlineParser::NOT_FOUND)
	{
		nonpolar_radius_rule_file_name = "solvation/bondi.rul";
	}
	else
	{
		cout << "Using " << nonpolar_radius_rule_file_name << " for nonpolar radii" << endl;
		configuration_string += "use_nonpolar_radius_rules " + nonpolar_radius_rule_file_name + "\n";

		options.setBool(NonpolarSolvation::Option::NONPOLAR_OVERWRITE_RADII, true);
		options.set(NonpolarSolvation::Option::NONPOLAR_RADIUS_RULES, nonpolar_radius_rule_file_name);
	}



	float score = 0.0f;

	if (!parpars.has("S"))
	{
		configuration_string = configuration_string + "calculate_energy\n";

		SLICKEnergy slick(*protein, *ligand, options);
		score = slick.calculateScore();

		cout << endl << "SLICK/energy is " << score << " kJ/mol" << endl;

		if (verbosity > 1)
		{
			cout << endl << "Scores (w/o coefficients)" << endl << endl;
			cout << "Hydrogen bonds     " << slick.getHydrogenBondScore() << endl;
			cout << "CHPI               " << slick.getCHPIScore() << endl;
			cout << "VanDerWaalsSlick   " << slick.getVDWScore() << endl;
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
		datfile << protein_file_name << " " << ligand_file_name << " " << score << " " << component << endl;
		datfile.close();

		component = "HB";
		dat_file_name = component + ".dat";
		datfile.open(dat_file_name, std::ios::out);
		datfile << protein_file_name << " " << ligand_file_name << " " << slick.getHydrogenBondScore()
			<< " " << component << endl;
		datfile.close();

		component = "CHPISlick";
		dat_file_name = component + ".dat";
		datfile.open(dat_file_name, std::ios::out);
		datfile << protein_file_name << " " << ligand_file_name << " " << slick.getCHPIScore()
			<< " " << component << endl;
		datfile.close();

		component = "VDW5";
		dat_file_name = component + ".dat";
		datfile.open(dat_file_name, std::ios::out);
		datfile << protein_file_name << " " << ligand_file_name << " " << slick.getVDWScore()
			<< " " << component << endl;
		datfile.close();

		component = "NONPOLAR2";
		dat_file_name = component + ".dat";
		datfile.open(dat_file_name, std::ios::out);
		datfile << protein_file_name << " " << ligand_file_name << " " << slick.getNonpolarSolvationScore()
			<< " " << component << endl;
		datfile.close();

		component = "DESOLV4";
		dat_file_name = component + ".dat";
		datfile.open(dat_file_name, std::ios::out);
		datfile << protein_file_name << " " << ligand_file_name << " " << slick.getPolarSolvationScore()
			<< " " << component << endl;
		datfile.close();
#endif

		time(&rawtime);
		time_string = time_string + String("stop:  " + String(asctime(localtime(&rawtime))));

		if (parpars.has("log"))
		{
			File logfile("SLICKenergy.log", std::ios::out);
			logfile << configuration_string << endl;
			logfile << time_string << endl;
			logfile.close();
		}
	}


	if (!parpars.has("E"))
	{
		configuration_string = configuration_string + "calculate_score\n";

		SLICKScore slick(*protein, *ligand, options);
		score = slick.calculateScore();

		cout << endl << "SLICK/score is " << score << " (arb. units)" << endl;

		if (verbosity > 1)
		{
			cout << endl << "Scores (w/o coefficients)" << endl << endl;
			cout << "Hydrogen bonds     " << slick.getHydrogenBondScore() << endl;
			cout << "CHPI               " << slick.getCHPIScore() << endl;
			cout << "VanDerWaalsSlick   " << slick.getVDWScore() << endl;
			cout << "Electrostatic int. " << slick.getPolarSolvationScore() << endl;
		}

#ifdef OLD_DATAFILE_FORMAT
		String dat_file_name;
		String component;
		File datfile;

		component = "SLICKenergy";
		dat_file_name = component + ".dat";
		datfile.open(dat_file_name, std::ios::out);
		datfile << protein_file_name << " " << ligand_file_name << " " << score << " " << component << endl;
		datfile.close();

		component = "HB";
		dat_file_name = component + ".dat";
		datfile.open(dat_file_name, std::ios::out);
		datfile << protein_file_name << " " << ligand_file_name << " " << slick.getHydrogenBondScore() << " " << component << endl;
		datfile.close();

		component = "CHPISlick";
		dat_file_name = component + ".dat";
		datfile.open(dat_file_name, std::ios::out);
		datfile << protein_file_name << " " << ligand_file_name << " " << slick.getCHPIScore() << " " << component << endl;
		datfile.close();

		component = "VDW5";
		dat_file_name = component + ".dat";
		datfile.open(dat_file_name, std::ios::out);
		datfile << protein_file_name << " " << ligand_file_name << " " << slick.getVDWScore() << " " << component << endl;
		datfile.close();

		component = "COULOMB";
		dat_file_name = component + ".dat";
		datfile.open(dat_file_name, std::ios::out);
		datfile << protein_file_name << " " << ligand_file_name << " " << slick.getPolarSolvationScore() << " " << component << endl;
		datfile.close();
#endif

		time(&rawtime);
		time_string = time_string + String("stop:  " + String(asctime(localtime(&rawtime))));

		if (parpars.has("log"))
		{
			File logfile("SLICKscore.log", std::ios::out);
			logfile << configuration_string << endl;
			logfile << time_string << endl;
			logfile.close();
		}
	}
}
