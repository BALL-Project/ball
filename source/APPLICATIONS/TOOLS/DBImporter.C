// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/FORMAT/dockResultFile.h>
#include <BALL/FORMAT/molFileFactory.h>
#include <BALL/FORMAT/molDB.h>
#include <BALL/FORMAT/genericMolFile.h>
#include <BALL/FORMAT/INIFile.h>
#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/DATATYPE/options.h>
#include "version.h"


using namespace BALL;
using namespace std;

void deleteUnwantedProperties(Molecule* mol, const String& id_prop_name)
{
	Size no = mol->countNamedProperties();
	Size no_deleted = 0;
	for (Size i = 0; i < no; i++)
	{
		Size index = i-no_deleted;
		const String& name = mol->getNamedProperty(index).getName();
		if (name != id_prop_name && !name.hasSubstring("ID") && !name.hasSubstring("id")
			&& !name.hasSubstring("score") && !name.hasSubstring("Class") && !name.hasSubstring("class")
			&& !name.hasSubstring("binding_free_energy")	&& !name.hasSubstring("IC50") && !name.hasSubstring("LOGP"))
		{
			mol->clearProperty(name);
			no_deleted++;
		}
	}
}


int main(int argc, char* argv[])
{
	#if not defined BALL_HAS_OPENBABEL && not defined BALL_HAS_OPENEYE
		Log.level(20) << endl << "[Warning:] Please note that this version has been build without support for OpenBabel or OpenEye.\nThus, the following information canNOT be generated and stored to the database automatically: smiles, functional group counts, logP !\nIf you nevertheless create/fill a database using this tool, you will NOT be able to filter compounds according to those criteria or by SMARTS matching when trying to export molecules from the database." << endl << endl;
	#endif

	String version = VERSION;
	#if defined BALL_HAS_OPENBABEL
		version += " (ob)";
	#elif defined BALL_HAS_OPENEYE
		version += " (oe)";
	#endif

	CommandlineParser parpars("DBImporter", "import molecules into data base", version, String(__DATE__), "Convert, combine and store");
	parpars.registerParameter("i", "input file", INFILE, true);
	parpars.registerParameter("o", "log file", OUTFILE);
	parpars.registerParameter("target", "target/dataset name whose molecules should be exported", STRING);
	parpars.registerParameter("d", "database name", STRING, true);
	parpars.registerParameter("u", "database username", STRING, true);
	parpars.registerParameter("host", "database host", STRING, true);
	parpars.registerParameter("port", "database port", BALL::INT, false, "3306");
	//parpars.registerParameter("id", "molecule ID property-tag name", STRING);
	parpars.registerParameter("p", "database password", STRING, true);
	parpars.registerParameter("vn", "name of compound vendor", STRING);
	parpars.registerParameter("vid", "name of vendor's compound-ID tag in sd-file", STRING);
	parpars.registerParameter("vd", "vendor library date or version", STRING);
	parpars.registerParameter("vu", "url from wich vendor library was obtained", STRING);
	String man = "This tool imports molecules into a database.\nAs input we need the file containing the molecules to be imported in sd-, mol2- or drf-format.";
	parpars.setToolManual(man);
	parpars.setSupportedFormats("i",MolFileFactory::getSupportedFormats());
	parpars.setSupportedFormats("o","txt");
	parpars.parse(argc, argv);

	String user = parpars.get("u");
	String db_name = parpars.get("d");
	String host = parpars.get("host");
	String password = parpars.get("p");
	int port = 3306;
	String id_tag = "";
	if (parpars.get("port") != CommandlineParser::NOT_FOUND)
	{
		port = parpars.get("port").toInt();
	}
	if (parpars.get("id") != CommandlineParser::NOT_FOUND)
	{
		id_tag = parpars.get("id");
	}

	MolDB* output = 0;
	try
	{
		output = new MolDB(host, user, password, db_name, port);
	}
	catch(BALL::Exception::GeneralException e)
	{
		if (e.getName() == String("MolDBConnector connect-error"))
		{
			delete output;
			Log.level(10) << "Database '" << db_name << "' does not seem to exist, will now try to create a new one ..." << endl;
			MolDB::createNewDatabase(host, user, password, db_name, port);
			output = new MolDB(host, user, password, db_name, port);
		}
		else
		{
			throw e;
		}
	}

// 	String s = options.setDefault("disable_fg", "");
// 	s.trim();
// 	if (s == "1" || s == "yes" || s == "y")
// 	{
// 		output->disableFunctionalGroupCounting();
// 	}

	if (parpars.get("vn") != CommandlineParser::NOT_FOUND ||
		parpars.get("vid")!=CommandlineParser::NOT_FOUND ||
		parpars.get("vd")!=CommandlineParser::NOT_FOUND ||
		parpars.get("vu")!=CommandlineParser::NOT_FOUND)
	{
		String vendor_name = parpars.get("vn");
		String vendor_id = parpars.get("vid");
		String vendor_date = parpars.get("vd");
		String vendor_url = parpars.get("vu");
		if (vendor_id == CommandlineParser::NOT_FOUND || vendor_name == CommandlineParser::NOT_FOUND ||
			vendor_date == CommandlineParser::NOT_FOUND)
		{
			cerr<<"Error: If vendor-information is to be imported, parameters 'vn', 'vid' and 'vd' must be specified (see help for more information)."<<endl;
			exit(1);
		}
		if (vendor_url == CommandlineParser::NOT_FOUND)
		{
			vendor_url="";
		}
		output->setVendorPropertyNames(vendor_name, vendor_id, false, vendor_date, vendor_url);
	}

	String target_name="";
	GenericMolFile* input = MolFileFactory::open(parpars.get("i"));
	Result* new_result = 0;
	DockResultFile* drf_input = dynamic_cast<DockResultFile*>(input);
	String t = parpars.get("target");
	if (drf_input)
	{
		drf_input->selectAllResultsForInput();
		if (t == CommandlineParser::NOT_FOUND)
		{
			Log.error() << "[Error:] target-name must be specified when using drf-files!" << endl;
			delete input;
			delete output;
			return 1;
		}
		else target_name = t;
	}
	else if (t != CommandlineParser::NOT_FOUND)
	{
		new_result = Result::makeLigandImportResult();
		target_name = t;
	}

	int no_mols = 0;
	for (Molecule* mol = input->read(); mol; mol = input->read())
	{
		if (no_mols%50 == 0)
		{
			Log.level(20) << "\r" << no_mols << " molecules";
			Log.flush();
		}
		if (!mol) break;

		//deleteUnwantedProperties(mol, id_tag);

		if (new_result)
		{
			String conf_hash;
			output->storeMolecule(mol, &conf_hash);
			Result::ResultData rd(Result::LIGANDIMPORT);
			Conformation conformation(mol, mol);
			conformation.setID(conf_hash);
			rd.setLigandConformation(&conformation);
			new_result->add(&conformation, rd);
			if (new_result && no_mols%10000 == 0)
			{
				output->storeResult(new_result, target_name);
				new_result->clear();
			}
		}
		else
		{
			output->storeMolecule(mol);
		}

		delete mol;
		no_mols++;
	}
	Log.level(20) << "\rImported " << no_mols << " molecules." << endl;

	if (drf_input)
	{
		const vector<Result*>* results = drf_input->getResults();
		for (Size i = 0; i < results->size(); i++)
		{
			output->storeResult((*results)[i], target_name);
		}
	}
	else if (new_result)
	{
		output->storeResult(new_result, target_name);
		delete new_result;
	}

	if (parpars.has("o"))
	{
		ofstream out(parpars.get("o").c_str());
		out << "Imported " << no_mols << " molecules into the data base." << endl << endl;
		out << "data base name: " << db_name << endl;
		out << "data base host: " << host << endl;
		if (t != CommandlineParser::NOT_FOUND)
		{
			out << "dataset description: " << t << endl;
		}
	}

	delete input;
	delete output;

}
