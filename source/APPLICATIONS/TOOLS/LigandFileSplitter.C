// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/FORMAT/molFileFactory.h>
#include <BALL/FORMAT/genericMolFile.h>
#include <BALL/FORMAT/dockResultFile.h>
#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/KERNEL/molecule.h>
#include "version.h"

using namespace BALL;
using namespace std;

void validateParameters(CommandlineParser& params)
{
    // we need at least one of the following: no, ligands_per_file
    // TODO: move this "one of the following parameters" logic to CommandlineParser!
    if (!params.has("no") && !params.has("ligands_per_file"))
    {
        Log.error() << "One of the parameters 'no' and 'ligands_per_file' is required." << endl;
        exit(1);
    }
    if (params.has("no") && params.has("ligands_per_file"))
    {
        Log.error() << "At most one of the parameters 'no' and 'ligands_per_file' is required. You have provided both." << endl;
        exit(1);
    }
    // TODO: move this to CommandlineParser... using 'registerParameterRestrictions' does not seem to be working
    if (params.has("no"))
    {
        if (params.get("no").toInt() < 1)
        {
            Log.error() << "The provided value for parameter 'no', " << params.get("no").toInt() << ", is invalid. Values must be greater or equal to 1." << endl;
            exit(1);
        }
    }
    if (params.has("ligands_per_file"))
    {
        if (params.get("ligands_per_file").toInt() < 1)
        {
            Log.error() << "The provided value for parameter 'ligands_per_file', " << params.get("ligands_per_file").toInt() << ", is invalid. Values must be greater or equal to 1." << endl;
            exit(1);
        }
    }
}

int main(int argc, char* argv[])
{
    CommandlineParser parpars("LigandFileSplitter", "split molecule files", VERSION, String(__DATE__), "Preparation");
	parpars.registerParameter("i", "input molecule file", INFILE, true);
    parpars.registerParameter("no", "no. of splits to be created", BALL::INT, false);
    parpars.registerParameter("ligands_per_file", "max. number of ligands to output to a file", BALL::INT, false);
	parpars.registerParameter("o", "output filenames; if none are specified, input filename postfixed with IDs will be used", OUTFILELIST, false);
	String man = "LigandFileSplitter splits a molecule file into a given number of subsets.\nNote that the molecules are not sorted in any way for this.";
	parpars.setToolManual(man);
	parpars.setSupportedFormats("i","mol2,sdf,drf");
	parpars.setSupportedFormats("o","mol2,sdf,drf");
	parpars.setOutputFormatSource("o","i");
    parpars.parse(argc, argv);

    validateParameters(parpars);

    GenericMolFile* input = MolFileFactory::open(parpars.get("i"));

	// Determine number of molecules in input files.
	// In case of DockResultFiles, we do not need to process all contained molecules in order to achieve this; we can simply count the result-section entries.
    int no_compounds = 0;
	DockResultFile* drf_input = dynamic_cast<DockResultFile*>(input);
	if (drf_input)
	{
		no_compounds = drf_input->countConformations();
		Log.level(10)<<"\rinput-file contains "<<no_compounds<<" conformations."<<endl;
	}
	else
	{
		for (Molecule* mol = input->read(); mol; mol = input->read())
		{
			no_compounds++;
			delete mol;
			if (no_compounds%50 == 0)
			{
				Log.level(10)<<"\r"<<no_compounds<<" molecules";
				Log.flush();
			}
		}
		Log.level(10)<<"\rinput-file contains "<<no_compounds<<" molecules."<<endl;
	}

	input->close();
	delete input;

    // if ligands_per_file was provided, simply insert the parameter 'no' with a proper value
    int numberOfFiles = -1;
    if (parpars.has("ligands_per_file"))
    {
        int ligandsPerFile = parpars.get("ligands_per_file").toInt();
        // we know the number of ligands per file and the number of input ligands, so:
        numberOfFiles = no_compounds / ligandsPerFile;
        if ((numberOfFiles * ligandsPerFile) < no_compounds)
        {
            numberOfFiles++;
        }
    }

	input = MolFileFactory::open(parpars.get("i"));
	drf_input = dynamic_cast<DockResultFile*>(input);
	if (drf_input) drf_input->selectAllResultsForInput();

    // if numberOfFiles != -1, it means that ligands_per_file was set
    int no_splits = numberOfFiles == -1 ? parpars.get("no").toInt() : numberOfFiles;
	vector<String> output_files;
	if (parpars.has("o"))
	{
		list<String> l = parpars.getList("o");
		for (list<String>::iterator it = l.begin(); it!=l.end(); it++)
		{
			output_files.push_back(*it);
		}
	}

	int comp_per_split = no_compounds/no_splits;
	int remainer = no_compounds-(comp_per_split*no_splits);
	int written_compounds = 0;

	Log.level(10)<<endl<<"will now write split-files :"<<endl;

	for (int i = 0; i < no_splits; i++)
	{
		String filename;
		if ((int)output_files.size() > i)
		{
			filename = output_files[i];
		}
		else
		{
			filename = parpars.get("i");
			if (filename.hasSuffix(".mol2"))
			{
				filename = ((String)filename.before(".mol2"))+"_"+String(i)+".mol2";
			}
			else if (filename.hasSuffix(".sdf"))
			{
				filename = ((String)filename.before(".sdf"))+"_"+String(i)+".sdf";
			}
			else if (filename.hasSuffix(".drf"))
			{
				filename = ((String)filename.before(".drf"))+"_"+String(i)+".drf";
			}
			else
			{
				cerr<<"Error: Extension of file '"<<filename<<"' not supported!"<<endl;
				return 1;
			}
		}

		GenericMolFile* output = MolFileFactory::open(filename, ios::out, input);

		DockResultFile* drf_output = dynamic_cast<DockResultFile*>(output);
		if (drf_input && drf_output) drf_output->disableAutomaticResultCreation();

		HashSet < String > conformation_IDs;

		int written_compounds_current_split = 0;
		for (int j = 0; j < comp_per_split ||
			(i == no_splits-1&&written_compounds<no_compounds); j++)
		{
			Molecule* mol = input->read();
			if (!mol) break;
			if (drf_input && drf_output && mol->hasProperty("Conformation_input_UID"))
			{
				conformation_IDs.insert(mol->getProperty("Conformation_input_UID").toString());
			}

			*output << *mol;
			delete mol;
			written_compounds++;
			written_compounds_current_split++;
			if (written_compounds_current_split%50 == 0)
			{
				Log.level(20)<<"\r"<<filename<<": "<<written_compounds_current_split;
				Log.flush();
			}
		}

		if (drf_input && drf_output)
		{
			const vector<Result*>* results = drf_input->getResults();
			for (Size i = 0; i < results->size(); i++)
			{
				Result* new_res = new Result(*(*results)[i], conformation_IDs);
				drf_output->addResult(new_res);
			}
		}

		// evenly distribute remaining compounds
		if (remainer > 1 && i == no_splits-1-remainer)
		{
			comp_per_split++;
		}

		Log.level(20)<<"\r"<<filename<<": "<<written_compounds_current_split<<endl;
		delete output;
	}

	delete input;
	return 0;
}
