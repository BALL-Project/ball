/* DBExporter.C
*
* Copyright (C) 2011 Marcel Schumann
*
* This program free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or (at
* your option) any later version.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, see <http://www.gnu.org/licenses/>.
*/

// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Marcel Schumann $
// ----------------------------------------------------

#include <BALL/FORMAT/molFileFactory.h>
#include <BALL/FORMAT/molDB.h>
#include <BALL/FORMAT/genericMolFile.h>
#include <BALL/FORMAT/dockResultFile.h>
#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/FORMAT/INIFile.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/STRUCTURE/molecularSimilarity.h>
#include <BALL/DATATYPE/options.h>
#include <BALL/SYSTEM/timer.h>
#include "version.h"

using namespace BALL;
using namespace std;


int main(int argc, char* argv[])
{
	String version = VERSION;
	#if defined BALL_HAS_OPENBABEL
		version += " (ob)";
	#elif defined BALL_HAS_OPENEYE
		version += " (oe)";
	#endif

	CommandlineParser parpars("DBExporter", "export molecules from data base", version, String(__DATE__), "Get Data");

	// -- Register all available parameters and flags here ---
	parpars.registerParameter("o", "output file", OUTFILE, true);
	parpars.registerParameter("start_id", "ID of the first conformation to be exported", BALL::INT);
	parpars.registerParameter("end_id", "ID of the last conformation to be exported", BALL::INT);
	parpars.registerParameter("min_logP", "minimal logP value", DOUBLE);
	parpars.registerParameter("max_logP", "maximal logP value", DOUBLE);
	parpars.registerParameter("min_MW", "minimal molecular weight", DOUBLE);
	parpars.registerParameter("max_MW", "maximal molecular weight", DOUBLE);
	parpars.registerParameter("max_mols", "max. number of molecules to be exported", BALL::INT);
	parpars.registerParameter("target", "target/dataset name whose molecules should be exported", STRING);
	parpars.registerParameter("q", "query molecules for similarity searching", INFILE);
	parpars.registerParameter("min_sim", "minimal average similarity", DOUBLE);
	parpars.registerParameter("max_sim", "maximal similarity", DOUBLE);
	parpars.registerParameter("smarts", "SMARTS pattern", STRING);
	parpars.registerParameter("smarts_file", "SMARTS pattern", INFILE);
	parpars.registerParameter("uck", "UCK key", STRING);
	parpars.registerFlag("s", "show summary of datasets in database");
	parpars.registerParameter("d", "database name", STRING, true);
	parpars.registerParameter("u", "database username", STRING, true);
	parpars.registerParameter("h", "database host", STRING, true);
	parpars.registerParameter("port", "database port", BALL::INT, false, "3306");
//	parpars.registerParameter("id", "molecule ID property-tag name", STRING);
	parpars.registerParameter("p", "database password", STRING, true);
	parpars.setParameterRestrictions("min_sim",0,1);
	parpars.setParameterRestrictions("max_sim",0,1);
	parpars.setParameterRestrictions("min_logP",-10,10);
	parpars.setParameterRestrictions("max_logP",-10,10);
	parpars.setSupportedFormats("o","mol2, sdf, drf");
	parpars.setSupportedFormats("q",MolFileFactory::getSupportedFormats());
	parpars.setSupportedFormats("smarts_file","txt");
	// -------

	String man = "This tool exports compounds from a database to a molecular file. Compounds can be filtered according to (among others) SMARTS expressions, logP, molecular weight, or similarity to query molecule(s) and dataset name.";

	parpars.setToolManual(man);
	parpars.parse(argc, argv);

	String par_start_id = parpars.get("start_id");
	String par_end_id = parpars.get("end_id");
	String par_target = parpars.get("target");
	String par_query = parpars.get("q");
	String par_minsim = parpars.get("min_sim");
	String par_maxsim = parpars.get("max_sim");
	String par_smarts = parpars.get("smarts");
	String par_smartsfile = parpars.get("smarts_file");
	String par_uck    = parpars.get("uck");
	String par_minlogp = parpars.get("min_logP");
	String par_maxlogp = parpars.get("max_logP");
	String par_minmw = parpars.get("min_MW");
	String par_maxmw = parpars.get("max_MW");

	// -- program-specific parameter checks, because there are various ways of invoking this program ---
	const String& nf = CommandlineParser::NOT_FOUND;
	bool use_MW = (par_minmw!=nf || par_maxmw!=nf);
	bool use_ids = (par_start_id!=nf && par_end_id!=nf);
	bool use_target = (par_target!=nf);
	bool use_fingerprints = (par_query!=nf && (par_minsim!=nf || par_maxsim!=nf));
	bool use_smarts = (par_smarts!=nf || par_smartsfile!=nf);
	bool use_uck = (par_uck!=nf);
	bool use_logP = (par_minlogp!=nf||par_maxlogp!=nf);
	bool show_datasets = (parpars.get("s")!=nf);
	int no_selected = use_ids+use_MW+use_target+use_fingerprints+use_smarts+use_logP+use_uck;

	if (!show_datasets)
	{
		if (!use_ids && (par_start_id != nf || par_end_id != nf))
		{
			Log.error() << "[Error:] When using ID-ranges, you need to specify start *and* end ID !" << endl;
			Log.error() << "Use 'DBExporter -help' to see more information about all available parameters." << endl;
			exit(1);
		}
		if (!use_fingerprints && (par_query != nf || par_minsim != nf || par_maxsim != nf))
		{
			Log.error() << "[Error:] When using similarity search, you need to specify query-molecules *and* maximal and/or minimal similarity!" << endl;
			Log.error() << "Use 'DBExporter -help' to see more information about all available parameters." << endl;
			exit(1);
		}
	}

	String user = parpars.get("u");
	String db_name = parpars.get("d");
	String host = parpars.get("h");
	String password = parpars.get("p");
	int port = 3306;
	if (parpars.get("port") != CommandlineParser::NOT_FOUND)
	{
		port = parpars.get("port").toInt();
	}
	String id_tag;
	if (parpars.get("id") != CommandlineParser::NOT_FOUND)
	{
		id_tag = parpars.get("id");
	}
	else
	{
		id_tag = String("ID_") + db_name;
	}


	GenericMolFile* output = MolFileFactory::open(parpars.get("o"), ios::out, "mol2.gz");
	MolDB input(host, user, password, db_name, port);

	if (show_datasets)
	{
		vector<String> names;
		vector<Size> no_isomers;
		vector<Size> no_conformations;
		input.getDatasetSummary(names, no_isomers, no_conformations);
		if (names.size() == 1)
		{
			Log.level(20) << "There are no datasets currently stored in the database." << endl;
			Log.level(20) << "\t  #isomers\t #conformations" << endl;
		}
		else
		{
			Log.level(20) << "dataset description\t #isomers\t #conformations" << endl;
			Log.level(20) << "-------------------------------------------------------------" << endl;
			for (Size i = 0; i < names.size()-1; i++)
			{
				Log.level(20) << names[i] << "\t" << no_isomers[i] << "\t" << no_conformations[i] << endl;
			}
			Log.level(20) << "-------------------------------------------------------------" << endl;
		}
		Log.level(20) << "TOTAL: \t\t" << no_isomers.back() << "\t"<<no_conformations.back() << endl;
		exit(0);
	}
	if (no_selected < 1)
	{
		Log.level(20) << "You did not specify which molecules you would like to export!" << endl;
		Log.level(20) << "Please use DBExporter -help to display available options for specifying/searching molecules." << endl;
		exit(1);
	}

	DockResultFile* drf_output = dynamic_cast<DockResultFile*>(output);
	if (drf_output && input.hasResults()) drf_output->disableAutomaticResultCreation();

	int start_id = 0;
	int end_id = 0;
	list < LongSize > conformation_IDs;

	if (use_ids) // ID-range was specified by user
	{
		start_id = atoi(par_start_id.c_str());
		end_id = atoi(par_end_id.c_str());
		for (int i = start_id; i <= end_id; i++)
		{
			conformation_IDs.push_back(i);
		}
		if (conformation_IDs.empty())
		{
			Log.error() << "[Error:] Specified conformation ID range is empty!" << endl;
			exit(1);
		}
	}
	if (use_logP)
	{
		list<LongSize>* IDs_to_check = 0;
		if (use_ids)
		{
			IDs_to_check = new list<LongSize>;
			*IDs_to_check = conformation_IDs;
		}

		bool input_isomer_ids = 0;
		bool output_isomer_ids = (use_target||use_smarts||use_fingerprints||use_MW);

		float min_logP = -9999;
		float max_logP = 9999;
		if (par_minlogp != CommandlineParser::NOT_FOUND) min_logP = par_minlogp.toFloat();
		if (par_maxlogp != CommandlineParser::NOT_FOUND) max_logP = par_maxlogp.toFloat();

		input.searchMoleculesByLogP(min_logP, max_logP, conformation_IDs, IDs_to_check, input_isomer_ids, output_isomer_ids);

		delete IDs_to_check;
		String s="conformations";
		if (output_isomer_ids) s="isomers";
		if (conformation_IDs.empty())
		{
			Log.level(20) << "-- No " << s << " within the given logP range found!" << endl;
			exit(0);
		}
		else
		{
			Log.level(20) << "-- Found " << conformation_IDs.size() << " " << s << " within the given logP range." << endl << flush;
		}
	}
	if (use_MW)
	{
		list<LongSize>* IDs_to_check = 0;
		if (use_ids)
		{
			IDs_to_check = new list<LongSize>;
			*IDs_to_check = conformation_IDs;
		}

		bool input_isomer_ids = use_logP;
		bool output_isomer_ids = (use_target||use_smarts||use_fingerprints);

		float min_MW = -9999;
		float max_MW = 9999;
		if (par_minmw != CommandlineParser::NOT_FOUND) min_MW = par_minmw.toFloat();
		if (par_maxmw != CommandlineParser::NOT_FOUND) max_MW = par_maxmw.toFloat();

		input.searchMoleculesByMW(min_MW, max_MW, conformation_IDs, IDs_to_check, input_isomer_ids, output_isomer_ids);

		delete IDs_to_check;
		String s="conformations";
		if (output_isomer_ids) s="isomers";
		if (conformation_IDs.empty())
		{
			Log.level(20) << "-- No " << s << " within the given MW range found!" << endl;
			exit(0);
		}
		else
		{
			Log.level(20) << "-- Found " << conformation_IDs.size() << " " << s << " within the given MW range." << endl << flush;
		}
	}
	if (use_target) // target_name was specified by user
	{
		list<LongSize>* IDs_to_check = 0;
		if (use_ids || use_logP)
		{
			IDs_to_check = new list<LongSize>;
			*IDs_to_check = conformation_IDs;
		}

		bool input_isomer_ids = (use_logP||use_MW);
		bool output_isomer_ids = (use_fingerprints||use_smarts);
		input.fetchDatasetConformationIDs(par_target, conformation_IDs, IDs_to_check, input_isomer_ids, output_isomer_ids);

		delete IDs_to_check;
		String s="conformations";
		if (output_isomer_ids) s="isomers";
		if (conformation_IDs.empty())
		{
			Log.level(20) << "-- No " << s << " with the given target description found!" << endl;
			exit(0);
		}
		else
		{
			Log.level(20) << "-- Found " << conformation_IDs.size() << " " << s << " for the given target description." << endl << flush;
		}
	}
	if (use_fingerprints)
	{
		list<LongSize>* IDs_to_check = 0;
		if (use_ids || use_logP || use_target)
		{
			IDs_to_check = new list<LongSize>;
			*IDs_to_check = conformation_IDs;
		}
		MolecularSimilarity molsim("fragments/functionalGroups.smarts");
		GenericMolFile* query = MolFileFactory::open(parpars.get("q"));
		list<vector<bool> > query_fingerprints;
		for (Molecule* mol = query->read(); mol; mol = query->read())
		{
			vector<bool> fp;
			molsim.generatePathFingerprint(*mol, fp);
			query_fingerprints.push_back(fp);
			delete mol;
		}
		float min_sim = 0;
		float max_sim = 1;
		if (par_minsim != CommandlineParser::NOT_FOUND) min_sim = par_minsim.toFloat();
		if (par_maxsim != CommandlineParser::NOT_FOUND) max_sim = par_maxsim.toFloat();

		Timer timer;
		timer.start();

		bool input_isomer_ids = (use_target||use_logP||use_MW);
		bool output_isomer_ids = use_smarts; 	input.searchSimilarMoleculesByFingerprint(query_fingerprints, min_sim, max_sim, conformation_IDs, IDs_to_check, input_isomer_ids, output_isomer_ids);

		timer.stop();
		Log.level(3) << "  Time needed for comparison of fingerprints : " << timer.getClockTime() << endl;
		delete query;
		delete IDs_to_check;

		String s="conformations";
		if (output_isomer_ids) s="isomers";
		if (conformation_IDs.empty())
		{
			Log.level(20) << "-- No " << s << " similar to the given query molecule(s) found!" << endl;
			exit(0);
		}
		else
		{
			Log.level(20) << "-- Found " << conformation_IDs.size() << " " << s << " with the desired similarity to the given query-molecule(s)." << endl << flush;
		}
	}

    #if defined BALL_HAS_OPENBABEL || defined BALL_HAS_OPENEYE
	if (use_smarts)
	{
		list<LongSize>* IDs_to_check = 0;
		if (no_selected > 1)
		{
			IDs_to_check = new list<LongSize>;
			*IDs_to_check = conformation_IDs;
		}
		list<String> smarts;
		vector<String> toks;

		String raw_smarts = "";
		if (par_smarts != nf)
		{
			String separator = String::CHARACTER_CLASS__WHITESPACE;
			separator += ", ";
			par_smarts.splitQuoted(toks, separator.c_str());
			for (Size i = 0; i < toks.size(); i++)
			{
				toks[i].trim();
				if (toks[i] != "")
				{
					smarts.push_back(toks[i]);
				}
			}
		}

		if (par_smartsfile != nf)
		{
			std::ifstream in(par_smartsfile.c_str());
			while (in)
			{
				String tmp;
				in >> tmp;
				tmp.trim();
				if (tmp != "")
				{
					smarts.push_back(tmp);
				}
			}
		}

		Log.level(5) << "Will now search for the following smarts:" << endl;
		for (list < String > ::iterator it = smarts.begin(); it != smarts.end(); it++)
		{
			cout<<*it<<endl<<flush;
		}

		Timer timer;
		timer.start();

		bool input_isomer_ids = (use_target||use_fingerprints||use_MW||use_logP);
		bool output_isomer_ids = 0;
		input.searchMoleculesBySmarts(smarts, conformation_IDs, IDs_to_check, 0, 0, input_isomer_ids, output_isomer_ids);

		timer.stop();
		Log.level(3) << "  Time needed for searching database by use of SMARTS : " << timer.getClockTime() << endl;
		delete IDs_to_check;

		String s="conformations";
		if (output_isomer_ids) s="isomers";
		if (conformation_IDs.empty())
		{
			Log.level(20) << "-- No " << s << " containing the desired SMARTS expression(s) found!" << endl;
			exit(0);
		}
		else
		{
			Log.level(20) << "-- Found " << conformation_IDs.size() << " " << s << " containing the given SMARTS pattern(s)." << endl << flush;
		}
	}
	#else
	if (use_smarts)
	{
		Log.error() << "[Error:] Smarts searching is not supported because this tool has been built without support for OpenBabel or OpenEye." << endl;
		exit(1);
	}
	#endif

	if (use_uck)
	{
		vector<String> ucks;
		vector<String> toks;
		par_uck.splitQuoted(toks, ", ");

		for (Size i = 0; i < toks.size(); i++)
		{
			ucks.push_back(toks[i]);
		}

		for (Size i = 0; i < ucks.size(); i++)
		{
			String uck = ucks[i];
			input.searchMoleculesByUCK(uck, conformation_IDs);
		}
	}

	String m = parpars.get("max_mols");
	if (m != CommandlineParser::NOT_FOUND)
	{
		Size max_mols = m.toUnsignedInt();
		if (conformation_IDs.size() > max_mols)
		{
			conformation_IDs.resize(max_mols);
		}
	}

	list<String> hash_strings;

	int no_mols = 0;
	list < LongSize > ::iterator conf_it = conformation_IDs.begin();
	bool warning_shown = 0;
	int db_chunk_size = 500;
	int no_chunks;
	if (use_ids)
	{
		if (db_chunk_size > (end_id-start_id+1)) db_chunk_size = end_id-start_id+1;
		no_chunks = (end_id-start_id+1)/db_chunk_size;
		if ((end_id-start_id+1)%db_chunk_size != 0) no_chunks++;
	}
	else
	{
		int size = conformation_IDs.size();
		if (db_chunk_size > size) db_chunk_size = size;
		no_chunks = size/db_chunk_size;
		if (size%db_chunk_size != 0) no_chunks++;
	}
	bool end_found = 0;

	for (int i = 0; i < no_chunks && !end_found; i++)
	{
		// generate ID-list
		vector<LongSize> ids;
		ids.reserve(db_chunk_size);
		for (int a = 0; a < db_chunk_size && conf_it != conformation_IDs.end(); a++, conf_it++)
		{
			ids.push_back(*conf_it);
		}
		if (conf_it == conformation_IDs.end())
		{
			end_found = 1;
		}

		vector<Molecule*> molecules = input.fetchMoleculeById(ids);
		bool ok = 1;

		vector<LongSize>::iterator id_it = ids.begin();
		for (Size p = 0; ok && p < molecules.size() && id_it != ids.end(); p++, id_it++)
		{
			Molecule* mol = molecules[p];
			if (!mol)
			{
				ok = 0;
				break;
			}

			if (drf_output)
			{
				if (mol->hasProperty("Conformation_input_UID"))
				{
					hash_strings.push_back(mol->getProperty("Conformation_input_UID").toString());
				}
				else if (!warning_shown)
				{
					Log.level(20) << "\r[Warning:] no conformation-hash property found. This should be set by the database export-function" << endl;
					warning_shown = 1;
				}
			}

			mol->setProperty(id_tag, String(*id_it));
			*output << *mol;

			delete mol;
			no_mols++;
			if (no_mols%50 == 0 || p == molecules.size()-1)
			{
				Log.level(20) << "\r" << no_mols << " molecules";
				Log.flush();
			}
		}
	}
	Log.level(20) << "\rExported " << no_mols << " molecules." << endl;

	if (drf_output && input.hasResults())
	{
		String target_name = "";
		if (par_target != nf) target_name = par_target;
		vector<Result*> results = input.fetchResults(hash_strings, target_name);
		for (Size i = 0; i < results.size(); i++)
		{
			drf_output->addResult(results[i]);
		}
	}

	delete output;
}
