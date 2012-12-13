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
	String ligandsPerFile = params.get("ligands_per_file");
	if (ligandsPerFile != CommandlineParser::NOT_FOUND)
    {
        if (params.get("ligands_per_file").toInt() < 1)
        {
            Log.error() << "The provided value for parameter 'ligands_per_file', " << params.get("ligands_per_file").toInt() << ", is invalid. Values must be greater or equal to 1." << endl;
            exit(1);
        }
	}
	String outputNamePattern = params.get("output_name_pattern");
	if (outputNamePattern != CommandlineParser::NOT_FOUND)
	{
		// count the number of '%d' and exit if the name is different to one
		string::size_type firstPlaceholder = outputNamePattern.find("%d");
		string::size_type lastPlaceholder = outputNamePattern.rfind("%d");
		// if there is more than one %d, it means that the index of the first and last occurrences are different
		// if there is not a single %d, then the index of both first and last occurrences must be npos
		if (firstPlaceholder != lastPlaceholder || firstPlaceholder == String::npos)
		{
			Log.error() << "The provided value for output_name_pattern '" << outputNamePattern << "' is invalid." << endl;
			exit(1);
		}
	}
}

String getFileFormat(String& fileName)
{
    // locate the first dot, from right to left
    int dotIndex = fileName.find_last_of('.');
    if (dotIndex < 0)
    {
        return "";
    }
    return fileName.substr(dotIndex + 1, fileName.length() - dotIndex - 1);
}

String getOutputFileFormat(CommandlineParser& parameters)
{
	// give preference to the 'output_format' parameter,
	// if not found, then use the format of the input file
	String format = parameters.get("output_format");
	if (format != CommandlineParser::NOT_FOUND)
	{
		return format;
	}
	String inputFile = parameters.get("i");
	return getFileFormat(inputFile);
}

String getOutputFileName(CommandlineParser& parameters, int index)
{
	String outputNamePattern = parameters.get("output_name_pattern");
	String outputFileName;
	if (outputNamePattern != CommandlineParser::NOT_FOUND)
    {
		outputNamePattern.substituteAll("%d", String(index));
		outputFileName = outputNamePattern;
    }
    else
    {
		String inputFileName = parameters.get("i");
		String extension = '.' + getFileFormat(inputFileName);

		// if invoked with -i ligands.sdf, output name will be ligands_<index>.sdf
		outputFileName = inputFileName.before(extension) + "_" + String(index) + extension;
    }
	return outputFileName;
}

int main(int argc, char* argv[])
{
    CommandlineParser parpars("LigandFileSplitter", "split molecule files", VERSION, String(__DATE__), "Preparation");
	parpars.registerParameter("i", "input molecule file", INFILE, true);
    parpars.registerParameter("no", "no. of splits to be created", BALL::INT, false);
    parpars.registerParameter("ligands_per_file", "max. number of ligands to output to a file", BALL::INT, false);
	parpars.registerParameter("output_name_pattern", "pattern that will be used to generate the names of the output files, see notes and examples below.", BALL::STRING, false);
	parpars.registerParameter("o", "output filenames; if none are specified, input filename postfixed with IDs will be used", OUTFILELIST, false);
	parpars.registerParameter("output_format", "format of the output filenames, see notes and examples below.", BALL::STRING, false);
	String man =
			"LigandFileSplitter splits a molecule file into a given number of subsets.\n"
			"Note that the molecules are not sorted in any way for this.\n\n"
			"Examples:\n\n"
			"$ LigandFileSplitter -i Trypsin_actives.sdf -no 3\n"
			"    will split the input file Trypsin_actives.sdf in three files named Trypsin_actives_0.sdf, Trypsin_actives_1.sdf and Trypsin_actives_2.sdf\n\n"
			"$ LigandFileSplitter -i ligands.sdf -ligands_per_file 4\n"
			"    will split the input file ligands.sdf in as many files needed to fit at most 4 ligands per file.\n"
			"    The files will be named ligands_0.sdf, ligands_1.sdf ... ligands_N.sdf\n\n"
			"$ LigandFileSplitter -i ligands.sdf -ligands_per_file 5 -output_name_pattern split_ligands-%d.sdf\n"
			"    will split the input file ligands.sdf in as many files needed to fit at most 5 ligands per file.\n"
			"    The files will be named split_ligands-0.sdf, split_ligands-1.sdf, ... , split_ligands-N.sdf\n"
			"    and they will have sdf format.\n\n"
			"$ LigandFileSplitter -i ligands.sdf -output_name_pattern split_ligands.mol2_%d -output_format mol2 -no 100\n"
			"    will split the input file ligands.sdf in 100 files using the following names:\n"
			"    split_ligands.mol2_0, split_ligands.mol2_1, ... , split_ligands.mol2_99\n"
			"    The output files will have mol2 format. If the 'output_format' parameter is not given, then the ouput files\n"
			"    will have the same format as the input file (sdf).\n\n"
			"NOTE:\n"
			"    output_name_pattern accepts a printf-like pattern, expecting exactly one decimal integer placeholder, %d.\n"
			"    The following are valid patterns: output_ligand.sdf_%d, split_%d.mol, %d_lig.drf\n"
			"    The following are invalid patterns: output_%f.sdf, ligands.drf_%u, %d_lig_%d.mol, molecules.sdf\n"
			"    If you want the output files to have a different format than the given input file, use the 'output_format' parameter.";
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
		GenericMolFile* output;
		if ((int)output_files.size() > i)
		{
			filename = output_files[i];
			// honor the use of "output_format"
			String format = parpars.get("output_format");
			if (format != CommandlineParser::NOT_FOUND)
			{
				output = MolFileFactory::open(filename, ios::out, format);
			}
			else
			{
				output = MolFileFactory::open(filename, ios::out, input);
			}

		}
		else
		{
            filename = getOutputFileName(parpars, i);
			output = MolFileFactory::open(filename, ios::out, getOutputFileFormat(parpars));
		}

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
