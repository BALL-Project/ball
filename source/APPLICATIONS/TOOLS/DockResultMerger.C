// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/DOCKING/COMMON/conformation.h>
#include <BALL/FORMAT/molFileFactory.h>
#include <BALL/FORMAT/genericMolFile.h>
#include <BALL/FORMAT/dockResultFile.h>
#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/KERNEL/molecule.h>

#include <map>
#include <set>
#include "version.h"


using namespace BALL;
using namespace std;


void sortMolecules(vector<String>& names, string& output_file, Size& best_k, string& e_property, double& score_cutoff, double& score_cuton)
{
	multimap<double, Molecule*> compounds;

	for (Size file = 0; file < names.size(); file++)
	{
		GenericMolFile* input = MolFileFactory::open(names[file]);

		int mol_no = 0;
		for (Molecule* mol = input->read(); mol; mol = input->read(), mol_no++)
		{
			if (!mol->hasProperty(e_property))
			{
				Log.level(10) << "Compound " << mol->getName() << " in file " << names[file] << " has no score property. Skipping this compound." << endl;
				delete mol;
				continue;
			}

			double score = ((String)mol->getProperty(e_property).toString()).toFloat();

			if (score > score_cutoff || score < score_cuton)
			{
				delete mol;
				continue;
			}

			if ((compounds.size() < best_k || score < compounds.rbegin()->first))
			{
				compounds.insert(make_pair(score, mol));

				if (compounds.size() > best_k)
				{
					delete compounds.rbegin()->second;
					multimap<double, Molecule*>::iterator it = compounds.end();
					it--;
					compounds.erase(it);
				}
			}
			else
			{
				delete mol;
			}
			Log.level(20) << "\r" << names[file] << " : " << mol_no+1 << flush;
		}
		Log.level(20) << endl;
		Log.flush();

		input->close();
		delete input;
	}

	if (compounds.size() < best_k)
	{
		Log.level(20) << "found " << compounds.size() << " compounds matching the given criteria." << endl;
	}

	GenericMolFile* output = MolFileFactory::open(output_file, ios::out, "mol2.gz");

	for (multimap < double, Molecule* > ::iterator it = compounds.begin();
		it!=compounds.end(); it++)
	{
		*output << *it->second;
		delete it->second;
	}

	output->close();
	delete output;
}


void mergeDRFiles(vector<String>& names, string& output_file, Size& best_k, string& e_property, double& score_cutoff, double& score_cuton)
{

	DockResultFile* output = new DockResultFile(output_file, ios::out);
	bool sort_by_scores = 1;
	if (e_property == "") sort_by_scores = 0;

	vector<Result*> new_results;

	/// First of all, copy Result data
	map<Result::Method, Result*> result_map;
	for (Size file = 0; file < names.size(); file++)
	{
		DockResultFile* input = new DockResultFile(names[file]);

		const vector<Result*>* results = input->getResults();
		for (Size i = 0; i < results->size(); i++)
		{
			map<Result::Method, Result*>::iterator it = result_map.find((*results)[i]->getMethod());
			if (it == result_map.end())
			{
				Result* result_copy = new Result(*(*results)[i]);
				if (!sort_by_scores) output->addResult(result_copy);
				else new_results.push_back(result_copy);
				result_map.insert(make_pair(result_copy->getMethod(), result_copy));
			}
			else
			{
				*it->second += *(*results)[i];
			}
		}

		input->close();
		delete input;
	}


	if (e_property != "")
	{
		e_property = "score_"+new_results.back()->getMethodString();
	}

	/// If no sorting is desired, iterate over all input-files and write each input-molecules to output-file
	if (!sort_by_scores)
	{
		output->disableAutomaticResultCreation();

		for (Size file = 0; file < names.size(); file++)
		{
			GenericMolFile* input = MolFileFactory::open(names[file]);

			int mol_no = 0;
			for (Molecule* mol = input->read(); mol; mol = input->read(), mol_no++)
			{
				*output << *mol;
				delete mol;
				Log.level(20) << "\r" << names[file] << " : " << mol_no+1;
				Log.flush();
			}
			Log.level(20)<<endl;
			Log.flush();

			input->close();
			delete input;
		}
	}

	/// If sorting is desired, iterate over all input-files and save each input-molecules to a map.
	/// Then write all FlexibleMolecules in this map to the output file and adapt the Result objects.
	else
	{
		multimap < double, FlexibleMolecule* > compounds; // map containing score and conformation-ID
		set < String > IDs; // IDs of the base-conformations

		for (Size file = 0; file < names.size(); file++)
		{
			DockResultFile* input = new DockResultFile(names[file]);
			int mol_no = 0;
			for (Molecule* mol = input->read(); mol; mol = input->read(), mol_no++)
			{
				if (!mol->hasProperty(e_property))
				{
					Log.level(10) << "Compound " << mol->getName() << " in file " << names[file] << " has no score property. Skipping this compound." << endl;
					for (Size i = 0; i < new_results.size(); i++)
					{
						new_results[i]->erase(input->getCurrentLigand());
					}
					delete mol;
					continue;
				}

				double score = ((String)mol->getProperty(e_property).toString()).toFloat();

				if (score > score_cutoff || score < score_cuton)
				{
					for (Size i = 0; i < new_results.size(); i++)
					{
						new_results[i]->erase(input->getCurrentLigand());
					}
					delete mol;
					continue;
				}

				if ((compounds.size() < best_k || score < compounds.rbegin()->first))
				{
					FlexibleMolecule* flexmol_copy = new FlexibleMolecule(*input->getCurrentLigand());
					compounds.insert(make_pair(score, flexmol_copy));
					IDs.insert(flexmol_copy->getId());

					if (compounds.size() > best_k)
					{
						for (Size i = 0; i < new_results.size(); i++)
						{
							new_results[i]->erase(compounds.rbegin()->second);
						}
						IDs.erase(compounds.rbegin()->second->getId());
						delete compounds.rbegin()->second;
						multimap<double, FlexibleMolecule*>::iterator it = compounds.end();
						it--;
						compounds.erase(it);
					}
				}
				else
				{
					for (Size i = 0; i < new_results.size(); i++)
					{
						new_results[i]->erase(input->getCurrentLigand());
					}
					delete mol;
				}
				Log.level(20) <<"\r"<<names[file]<<" : "<<mol_no+1<<flush;
			}
			Log.level(20)<<endl;

			input->close();
			delete input;
		}

		if (compounds.size() < best_k)
		{
			Log.level(20)<<"found "<<compounds.size()<<" compounds matching the given criteria."<<endl;
		}

		list<String> score_list;

		for (multimap < double, FlexibleMolecule* > ::iterator it = compounds.begin();
			it!=compounds.end(); it++)
		{
			output->writeLigand(it->second);
			score_list.push_back(it->second->getId());
			delete it->second;
		}

		// Remove those ligands from results for which no final result is available (e.g. due to missing atom parameters)
		vector < String > import_IDs = *new_results[0]->getInputConformations();
		for (Size i = 0; i < import_IDs.size(); i++)
		{
			if (IDs.find(import_IDs[i]) == IDs.end())
			{
				vector<vector<Result::ResultData> > data_list;
				for (Size k = 0; k < new_results.size(); k++)
				{
					if (k == 0)
					{
						data_list.push_back(*new_results[k]->get(import_IDs[i]));
					}

					vector<vector<Result::ResultData> > new_data_list;
					for (Size j = 0; j < data_list.size(); j++)
					{
						for (Size l = 0; l < data_list[j].size(); l++)
						{
							String ID = data_list[j][l].getLigandConformationId();
							new_data_list.push_back(*new_results[k]->get(ID));
							new_results[k]->erase(ID);
						}
					}
					data_list = new_data_list;
				}
			}
		}

		for (Size i = 0; i < new_results.size(); i++)
		{
			list<String> new_list;
			new_results[i]->sort(score_list, new_list);
			score_list = new_list;
		}

		output->writeResults(new_results);
	}

	output->close();
	delete output;
}


int main(int argc, char* argv[])
{
	CommandlineParser parpars("DockResultMerger", "merge docking output files", VERSION, String(__DATE__), "Convert, combine and store");
	parpars.registerMandatoryInputFile("i", "input files");
	parpars.registerMandatoryOutputFile("o", "output file");

	parpars.registerOptionalStringParameter("score", "score property name", "score");
	parpars.registerOptionalDoubleParameter("min", "minimal score value", -30.0);
	parpars.registerOptionalDoubleParameter("max", "maximal score value", 0.0);
	parpars.registerOptionalIntegerParameter("k", "number of output molecules", 20);

	parpars.registerFlag("rm", "remove input files after merging");
	parpars.setSupportedFormats("i","mol2,sdf,drf");
	parpars.setSupportedFormats("o","mol2,sdf,drf");
	String manual = "This tool merges and sorts molecule files as generated by docking or rescoring.\n\nYou need to specify the property-tag name of the scores according to which the molecules should be sorted. Optionally you can filter those compounds that were assigned a score above and/or below specified thresholds. If desired, you can furthermore choose to have only the compounds with the k best scores written to the output file.\n\n Output of DockResultMerger is one molecule containing the molecules found in input-files (that matched all filter criteria, if any), sorted ascendingly according to their scores.";
	parpars.setToolManual(manual);
	parpars.parse(argc, argv);

	string e_property="score";
	double energy_cutoff = 1e100;
	double energy_cuton = -1e100;
	Size best_k = 100000000;

	String output = parpars.get("o");
	String s = parpars.get("score");
	if (s != CommandlineParser::NOT_FOUND) e_property = s;
	s = parpars.get("min");
	if (s != CommandlineParser::NOT_FOUND) energy_cuton = s.toDouble();
	s = parpars.get("max");
	if (s != CommandlineParser::NOT_FOUND) energy_cutoff = s.toDouble();
	s = parpars.get("k");
	if (s != CommandlineParser::NOT_FOUND) best_k = s.toInt();

	const list<String>& n = parpars.getList("i");
	vector<String> names;
	for (list<String>::const_iterator it = n.begin(); it != n.end(); it++)
	{
		names.push_back(*it);
	}

	bool drf_merge = 0;
	if (output.hasSuffix(".drf"))
	{
		for (Size i = 0; i < names.size(); i++)
		{
			if (names[i].hasSuffix(".drf")) drf_merge = 1;
			else if (drf_merge)
			{
				Log.error()<<"[Error:] Using drf and non-drf files together as input is not supported."<<endl;
				return 1;
			}
		}
	}

	if (drf_merge)
	{
		Log.error()<<"[Error:] Using DockingFiles (*.drf) is not possible since this version of DockResultMerger has been compiled without QtXML support."<<endl;
		return 1;
	}

	if (!drf_merge)
	{
		sortMolecules(names, output, best_k, e_property, energy_cutoff, energy_cuton);
	}

	else
	{
		mergeDRFiles(names, output, best_k, e_property, energy_cutoff, energy_cuton);
	}


	if (parpars.has("rm"))
	{
		for (Size i = 0; i < names.size(); i++)
		{
			File::remove(names[i]);
		}
	}

	return 0;
}
