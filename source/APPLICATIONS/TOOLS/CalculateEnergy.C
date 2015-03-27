// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include <BALL/MOLMEC/AMBER/amber.h>
#include <BALL/MOLMEC/MMFF94/MMFF94.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/STRUCTURE/residueChecker.h>

#include <BALL/FORMAT/commandlineParser.h>

#include "version.h"

using namespace std;
using namespace BALL;

int main(int argc, char* argv[])
{
	// instantiate CommandlineParser object
	CommandlineParser parpars("CalculateEnergy", "calculate free energy of a protein ", VERSION, String(__DATE__), "ForceFields");
	parpars.registerMandatoryInputFile("pdb", "input pdb file ");

	// TODO: offer upload of a distinguished fragDB file?

	// choice of force field
	parpars.registerOptionalStringParameter("force_field", "force field (AMBER, MMFF94)", "AMBER");
	list<String> force_fields;
	force_fields.push_back("AMBER");
	force_fields.push_back("MMFF94");
	// TODO: shall we offer CHARM as well?
	parpars.setParameterRestrictions("force_field", force_fields);

	// TODO: shall we offer a force field parameter file upload?

	// TODO: check the naming!
	parpars.registerOptionalDoubleParameter("non_bond_cutoff",  "cutoff radius in calculations of nonbonded interactions", 20.0);
	parpars.registerOptionalDoubleParameter("elec_stat_cuton", "electrostatic cuton", 13.0);
	parpars.registerOptionalDoubleParameter("elec_stat_cutoff", "electrostatic cutoff", 15.0);
	parpars.registerFlag("dist_dep_dielec", "apply distance dependent dielectric constant", false);
	// NOTE: assign is the default
	//parpars.registerFlag("assign_typenames","automatically assign type names to the system", false);
	//parpars.registerFlag("assign_types","automatically assign types to the system if missing", false);
	//parpars.registerFlag("assign_charges", "automatically assign charges to the system if missing", false);
	// TODO: if we only allow PDBFile to upload then from where do we get stuff to overwrite?? 
	parpars.registerFlag("overwrite_types", "overwrite even non-empty type names", false);
	parpars.registerFlag("overwrite_charges","overwrite even non-zero charges", false);

	// the manual
	String man = String("This tool computes the free energy of a pdb file using a specified force field (-force_field) and force field parameters (-non_bond_cutoff, -elec_stat_cuton ... ).");

	parpars.setToolManual(man);

	parpars.setSupportedFormats("pdb", "pdb");

	// parse the command line
	parpars.parse(argc, argv);


	PDBFile pdb;
	pdb.open(parpars.get("pdb"), std::ios::in);

	if (!pdb)
	{
		// if file does not exist: complain and abort
		Log.error() << "error opening " << parpars.get("pdb") << " for input." << std::endl;
		exit(2);
	}

	System sys;
	pdb >> sys;
	pdb.close();

	// normalize the names and build all bonds
	FragmentDB db("");
	sys.apply(db.normalize_names);
	sys.apply(db.build_bonds);

	// check the structure
	Log.info() << " checking residues..." << std::endl;
	ResidueChecker rc(db);
	sys.apply(rc);

	if (!rc.getStatus())
	{
		Log.error() << "There are errors in the given structure. Use the ResidueChecker tool for further investigation." << std::endl;
		exit(2);
	}

	// create a force field	
	AmberFF* amber_force_field = NULL;
	MMFF94*  mmff_force_field = NULL;
	ForceField* force_field = NULL;

	if (parpars.has("force_field"))
	{
		String penalty_table = parpars.get("force_field");
		if (penalty_table == "AMBER")
		{
			amber_force_field = new AmberFF();
			force_field = amber_force_field;
			Log << " using the amber force field" << std::endl;
		}
		else if (penalty_table == "MMFF94")
		{
			mmff_force_field = new MMFF94();
			force_field = mmff_force_field;
			Log << " using the MMFF94 force field" << std::endl;
		}
		else
		{
			Log.error() << "Unknown force field " << parpars.get("force_field") << " Abort." << std::endl;
			exit(2);
		}
	}

	if (!amber_force_field && !mmff_force_field)
	{
		Log.error() << "Invalid force field. Abort." << std::endl;
		exit(2);
	}

	// set the ff options according the parameters
	float non_bonded_cutoff = 0;
	if (parpars.has("non_bond_cutoff"))
	{
		non_bonded_cutoff = parpars.get("non_bond_cutoff").toFloat();

		if (amber_force_field)
		{
			//TODO: something wents awfully wrong with the FF default options!
//	cout  << "non_bond_cutoff " << amber_force_field->options.getReal(AmberFF::Option::NONBONDED_CUTOFF) << " "<< amber_force_field->options.getReal(AmberFF::Default::NONBONDED_CUTOFF)  << endl;
			amber_force_field->options[AmberFF::Option::NONBONDED_CUTOFF] = non_bonded_cutoff;
		}
		else if (mmff_force_field)
		{
			mmff_force_field->options[MMFF94::Option::NONBONDED_CUTOFF] = non_bonded_cutoff;
		}
		Log << " used non bonded cutoff: " << non_bonded_cutoff << std::endl;
	}

	float elec_stat_cuton = 0;
	if (parpars.has("elec_stat_cuton"))
	{
		elec_stat_cuton = parpars.get("elec_stat_cuton").toFloat();

		if (amber_force_field)
		{
//	cout  << "elec_stat_cuton" << force_field->options[AmberFF::Option::ELECTROSTATIC_CUTON]  << endl;
			amber_force_field->options[AmberFF::Option::ELECTROSTATIC_CUTON] = elec_stat_cuton;
		}
		else if (mmff_force_field)
		{
//	cout  << "elec_stat_cuton" << force_field->options[MMFF94::Option::ELECTROSTATIC_CUTON]  << endl;
			mmff_force_field->options[MMFF94::Option::ELECTROSTATIC_CUTON] = elec_stat_cuton;
		}
		Log << " used electrostatic cuton: " << elec_stat_cuton << std::endl;
	}

	float elec_stat_cutoff = 0;
	if (parpars.has("elec_stat_cutoff"))
	{
		elec_stat_cutoff = parpars.get("elec_stat_cutoff").toFloat();

		if (amber_force_field)
		{
			amber_force_field->options[AmberFF::Option::ELECTROSTATIC_CUTOFF] = elec_stat_cutoff;
		}
		else if (mmff_force_field)
		{
			mmff_force_field->options[MMFF94::Option::ELECTROSTATIC_CUTOFF] = elec_stat_cutoff;
		}
		Log << " used electrostatic cutoff: " << elec_stat_cutoff << std::endl;
	}

	bool dist_dep_dielectric = true;
	if (parpars.has("dist_dep_dielec"))
	{
		dist_dep_dielectric = parpars.get("dist_dep_dielec").toBool();

		if (amber_force_field)
		{
			amber_force_field->options[AmberFF::Option::DISTANCE_DEPENDENT_DIELECTRIC] = dist_dep_dielectric;
		}
		else if (mmff_force_field)
		{
			mmff_force_field->options[MMFF94::Option::DISTANCE_DEPENDENT_DIELECTRIC] = dist_dep_dielectric;
		}
	}
	Log << " distance dependent dielectric constant " << (dist_dep_dielectric ? "on" : "off") << std::endl;

	// we handle ASSIGN_TYPES and ASSIGN_TYPENAMES together
	bool assign_types = true;
	if (parpars.has("assign_types"))
	{
		assign_types = parpars.get("assign_types").toBool();

		if (amber_force_field)
		{
			amber_force_field->options[AmberFF::Option::ASSIGN_TYPENAMES] = assign_types;
			amber_force_field->options[AmberFF::Option::ASSIGN_TYPES] = assign_types;
		}
		else if (mmff_force_field)
		{
			mmff_force_field->options[MMFF94::Option::ASSIGN_TYPENAMES] = assign_types;
			mmff_force_field->options[MMFF94::Option::ASSIGN_TYPES] = assign_types;
		}
	}
	Log << " assignment of missing types " << (assign_types ? "on" : "off") << std::endl;


	bool overwrite_types = true;
	if (parpars.has("overwrite_types"))
	{
		overwrite_types = parpars.get("overwrite_types").toBool();

		if (amber_force_field)
		{
			amber_force_field->options[AmberFF::Option::OVERWRITE_TYPENAMES] = overwrite_types;
		}
		else if (mmff_force_field)
		{
			mmff_force_field->options[MMFF94::Option::OVERWRITE_TYPENAMES] = overwrite_types;
		}
	}
	Log << " overwrite types " << (overwrite_types ? "on" : "off") << std::endl;


	bool assign_charges = true;
	if (parpars.has("assign_charges"))
	{
		assign_charges = parpars.get("assign_charges").toBool();

		if (amber_force_field)
		{
			amber_force_field->options[AmberFF::Option::ASSIGN_CHARGES] = assign_charges;
		}
		else if (mmff_force_field)
		{
			mmff_force_field->options[MMFF94::Option::ASSIGN_CHARGES] = assign_charges;
		}
	}
	Log << " assignment of missing charges " << (assign_charges ? "on" : "off") << std::endl;

	bool overwrite_charges = true;
	if (parpars.has("overwrite_charges"))
	{
		overwrite_charges = parpars.get("overwrite_charges").toBool();

		if (amber_force_field)
		{
			amber_force_field->options[AmberFF::Option::OVERWRITE_CHARGES] = overwrite_charges;
		}
		else if (mmff_force_field)
		{
			mmff_force_field->options[MMFF94::Option::OVERWRITE_CHARGES] = overwrite_charges;
		}
	}
	Log << " overwrite charges " << (overwrite_charges ? "on" : "off") << std::endl;

	// setup the force field
	Log.info() << " setting up the force field..." << endl;
	force_field->setup(sys);

	// setup successful?
	if (   (force_field->getUnassignedAtoms().size() > 0)
		  || !force_field->isValid())
	{
		Log.error() << "There are parameterless atoms in the structure. Use the ResidueChecker tool for further investigation." << std::endl;
		exit(2);
	}

	// trigger the energy computations
	force_field->updateEnergy();

	// print the result
	Log.info() << force_field->getResults() << endl;

	return 0;
}
