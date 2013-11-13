// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//


#include <BALL/FORMAT/molFileFactory.h>
#include <BALL/FORMAT/genericMolFile.h>
#include <BALL/KERNEL/system.h>
#include <BALL/FORMAT/commandlineParser.h>

#include "version.h"

using namespace BALL;

int main(int argc, char* argv[])
{
	// instantiate CommandlineParser object
	CommandlineParser parpars("MolecularFileConverter", "converts molecular file-formats", VERSION, String(__DATE__), "File conversion");
	parpars.registerParameter("i",  "input file",    INFILE,  true);
	parpars.registerParameter("of", "output format", STRING,  false);
	parpars.registerParameter("o",  "output file",   OUTFILE, true);

	// the available formats
	String supp_formats = MolFileFactory::getSupportedFormats();
	// we support: mol2, sdf, drf, pdb, ac, ent, brk, hin, mol, xyz, 
	// mol2.gz, sdf.gz, drf.gz, pdb.gz, ac.gz, ent.gz, brk.gz, hin.gz, mol.gz, xyz.gz.

	// the manual
	String man = String("This tool converts different molecular file formats.\nSupported formats are ") + supp_formats + String(".");

	parpars.setToolManual(man);

	parpars.setSupportedFormats("i", supp_formats);
	parpars.setSupportedFormats("o", supp_formats);

	// set supported formats as options for parameter of
	String formats = supp_formats;
	vector<String> temp;
	formats.split(temp,",");
	list<String> supp_format_list;
	for (Size i=0; i<temp.size(); i++)
	{
		supp_format_list.push_back(temp[i]);
	}
	parpars.setParameterRestrictions("of", supp_format_list);

	// parse the command line
	parpars.parse(argc, argv);

	// check if optional parameter 'of' is given
	String default_format = "mol2";
	if (parpars.has("of"))
		default_format = parpars.get("of");

	// read input/output file
	GenericMolFile* in_file  = MolFileFactory::open(parpars.get("i"), std::ios::in);
	GenericMolFile* out_file = MolFileFactory::open(parpars.get("o"), std::ios::out, default_format);

	if (!in_file)
	{
		// if file does not exist: complain and abort
		Log.error() << "error opening " << parpars.get("i") << " for input." << std::endl;
		exit(2);
	}
	if (!out_file)
	{
		// if file does not exist: complain and abort
		Log.error() << "error opening " << parpars.get("o") << " for output." << std::endl;
		exit(2);
	}

	// create a system and read the contents of the molecular file
	System S;
	in_file->read(S);
	in_file->close();

	// and write
	if (out_file)
	{
		out_file->write(S);
		out_file->close();
		Log.info() << "Wrote converted file " << parpars.get("o") << std::endl;
	}

	delete in_file;
	delete out_file;
}

