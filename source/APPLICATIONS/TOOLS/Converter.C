// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/FORMAT/molFileFactory.h>
#include <BALL/FORMAT/dockResultFile.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/FORMAT/commandlineParser.h>
#include "version.h"

using namespace BALL;
using namespace std;

void validateParameters(CommandlineParser& params)
{
	String formats = MolFileFactory::getSupportedFormats();
	vector<String> v;
	formats.split(v,",");
	
	String input_format = params.get("if");
	String output_format = params.get("of");
	bool correct_input_format = false;
	bool correct_output_format = false;
	for (Size i=0; i<v.size(); i++)
	{
		if (input_format == v[i])
		{
			correct_input_format = true;
		}
		if (output_format == v[i])
		{
			correct_output_format = true;
		}
	}
	if (!correct_input_format)
	{
		Log.error() << "Input file format " << input_format << " is not supported\nSupported formats are " << formats << endl;
		exit(1);
	}
	
	if (!correct_output_format)
	{
		Log.error() << "Output file format " << output_format << " is not supported\nSupported formats are " << formats << endl;
		exit(1);
	}
}  

int main(int argc, char* argv[])
{
	CommandlineParser parpars("Converter", "interconvert molecular file-formats", VERSION, String(__DATE__), "Convert, combine and store");
	parpars.registerMandatoryInputFile("i", "input filename");
	parpars.registerMandatoryStringParameter("if", "input format");
	parpars.registerMandatoryOutputFile("o", "output filename");
	parpars.registerMandatoryStringParameter("of", "output format");
	parpars.registerFlag("rm", "remove input file when finished");
	
	// the available formats
	String formats = MolFileFactory::getSupportedFormats();
	// we support: mol2, sdf, drf, pdb, ac, ent, brk, hin, mol, xyz, 
	// mol2.gz, sdf.gz, drf.gz, pdb.gz, ac.gz, ent.gz, brk.gz, hin.gz, mol.gz, xyz.gz.
	
	String man = String("This tool can be used to convert between different molecular file-formats.\nSupported formats are ") + formats + String(". File extensions of input and output filenames are ignored.");
	parpars.setToolManual(man);
	parpars.setSupportedFormats("i",MolFileFactory::getSupportedFormats());
	parpars.setSupportedFormats("o",MolFileFactory::getSupportedFormats());
	
	vector<String> v;
	formats.split(v,",");
	list<String> format_list;
	for (Size i=0; i<v.size(); i++)
	{
		format_list.push_back(v[i]);
	}
	parpars.setParameterRestrictions("if",format_list);
	parpars.setParameterRestrictions("of",format_list);
	parpars.parse(argc, argv);
	
	validateParameters(parpars);
	
	//String default_format = "mol2";
	String input_format = parpars.get("if");
	String output_format = parpars.get("of");
	GenericMolFile* input  = MolFileFactory::open(parpars.get("i"), ios::in, input_format, true);
	GenericMolFile* output = MolFileFactory::open(parpars.get("o"), ios::out, output_format, true);
	DockResultFile* drf_output = dynamic_cast<DockResultFile*>(output);
	if (drf_output)
	{
		drf_output->setToolInfo(parpars.getStartCommand(), parpars.getStartTime());
	}

	Molecule* mol;
	int no_written = 0;
	int no_ignored = 0;
	while ((mol = input->read()))
	{
		bool b;
		if (output_format == "pdb")
		{
			PDBInfo *pdbi = new PDBInfo();
			System* mol_sys = new System();
			pdbi->setName(mol->getName());
			String compnd_line = String(80, PDB::FORMAT_COMPND, 1, mol->getName().c_str());
			pdbi->getSkippedRecords().push_back(String("COMPND") + compnd_line);
			mol_sys->insert(*mol);
			b = (dynamic_cast<PDBFile*>(output))->write(*mol_sys, *pdbi);
		}
		else
		{
			b = output->write(*mol);
		}
		
		if (b) no_written++;
		else no_ignored++;
		delete mol;
		
		String smf[] = {"pdb", "pdb.gz", "ac", "ac.gz", "brk", "brk.gz", "mol", "mol.gz"};
		set<String> single_molecule_formats (smf,smf+8);
		
		if (single_molecule_formats.count(output_format) > 0)
		{
			
			if (no_written > 0)
			{
				Log.error() << "Output format " << output_format << " only writes one molecule per file. Only first molecule written.\nYou can use LigandFileSplitter to split input file first." << endl;
				input->close();
				output->close();

				delete input;
				delete output;

				exit(1);
			}
		}
		
		if (no_written%50 == 0)
		{
			Log.level(5) << "\r" << no_written << "molecules";
			Log.flush();
		}
	}

	Log.level(20) << "\r";
	if (no_ignored > 0) Log.level(20) << "ignored " << no_ignored << " identical molecules!" << endl;
	Log.level(20) << "wrote " << no_written << " molecules." << endl;

	input->close();
	output->close();

	delete input;
	delete output;

	if (parpars.has("rm"))
	{
		File::remove(parpars.get("i"));
		Log.level(20) << parpars.get("i") << " removed." << endl;
	}
}
