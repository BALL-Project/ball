// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

//
//  SeperateMolecules.C
//------------------------------------------------------------------------------
//
// * take one input file containing molecules that are merged under one entry
// * allow all available BALL-formats that guarantee bond information
//   (currently this is: mol, mol2, sd)
// * output a single file in the same format, but each molecule separated in its
//   own molecular entry
// * standard: output only largest component of each molecule
// * optionally: seperate all connected components (Flag: -all)
// * optionally: output only components of a molecule with min number of atoms
//   (Flag: -min_atoms)
//
//  rewritten
//    by Hans-Philipp Brachvogel, philipp-brachvogel@web.de, 2014/07/30
//------------------------------------------------------------------------------
//

// File formats:
#include <BALL/FORMAT/MOL2File.h>
#include <BALL/FORMAT/MOLFile.h>
#include <BALL/FORMAT/SDFile.h>
#include <BALL/FORMAT/molFileFactory.h>

// To be wrapped:
#include <BALL/STRUCTURE/connectedComponentsProcessor.h>
#include <BALL/CONCEPT/property.h>

// User interface:
#include <BALL/FORMAT/commandlineParser.h>

// Feedback msg
#include <iostream> 
#include <stdlib.h> 

#include "version.h"

using namespace std;
using namespace BALL;

typedef std::vector<Molecule> MolVec;

// -----------------------------------------------------------------------------
// ----------------------- HELPER FUNCTIONS: -----------------------------------
// -----------------------------------------------------------------------------

// Copy properties from an input molecule to an output molecule
void copyMoleculeProperies(Molecule &orig, Molecule &cop)
{
	NamedPropertyIterator  it;
	for(it = orig.beginNamedProperty (); it !=orig.endNamedProperty(); it++)
	{
		cop.setProperty(*it);
	}
	cop.setName(orig.getName());
}

// Write several result molecules to one file
void writeMolVec(MolVec &input, GenericMolFile* handle, Molecule &orig)
{
	for(size_t i = 0; i < input.size(); i++)
	{
		copyMoleculeProperies(orig, input[i]);
		(*handle) << input[i];
	}
}


//##### LARGEST
// Extract only THE LARGEST molecule of a single molecule entry
// and append it to a single file
void looperLARGE( int& total, int& selected, Molecule* in_mol, 
									ConnectedComponentsProcessor& ccp, GenericMolFile* outF)
{
	in_mol->apply(ccp);
	
	Molecule result;
	ccp.getLargestComponent(result);

	copyMoleculeProperies((*in_mol), result);
	(*outF) << result;
	
	total    += ccp.getNumberOfConnectedComponents();
	selected += 1;
}

//##### MINIMUM NUMBER
// Extract all molecules that have a MINIMUM NUMBER OF ATOMS
// and write them to a single file
void looperMINIMUM(int& total, int& selected, Molecule* in_mol, 
									 ConnectedComponentsProcessor& ccp, 
									 GenericMolFile* outF, int min)
{
	in_mol->apply(ccp);
	
	MolVec results;
	ccp.getMinAtomsComponents(results, min);
	writeMolVec(results, outF, *in_mol);
	
	total    += ccp.getNumberOfConnectedComponents();
	selected += results.size();
}

//##### ALL
// Extract ALL contained molecules to their own molecular entries
// and write them to a single file
void looperALL(int& total, int& selected, Molecule* in_mol, 
							 ConnectedComponentsProcessor& ccp, GenericMolFile* outF)
{
	in_mol->apply(ccp);
	MolVec results;
	ccp.getAllComponents(results);
	writeMolVec(results, outF, *in_mol);
	
	total    += ccp.getNumberOfConnectedComponents();
	selected += results.size();
}

// -----------------------------------------------------------------------------
// --------------------------------- MAIN: -------------------------------------
// -----------------------------------------------------------------------------
int main (int argc, char **argv)
{
	
// Setup the parser-------------------------------------------------------------
	CommandlineParser parpars("SeperateMolecules", 
														"Split each multi molecule entry to "
														"separate single molecule entries",
														VERSION, String(__DATE__), "Preparation");

	parpars.registerMandatoryInputFile( "i", "input file (mol mol2 sd sdf)");

	parpars.registerMandatoryOutputFile("o",
														"output file name, type will be set to the same "
														"as the input file");

	// register int parameter for supplying minimal number of atoms
	parpars.registerOptionalIntegerParameter("min_atoms",
														"only keep molecules having at least a minimal "
														"number of atoms",
														0);

	// register flag for outputting all molecules
	parpars.registerFlag("all",
											 "keep all contained molecules, but each in separated "
											 "entries", false);

	String man = 
			"This tool splits each molecule entry contained in a structure file "
			"(mol, sdf, mol2) into possibly multiple molecule entries, so that the "
			"new entries contain only a single connected molecule. "
			"The results will always be written to a single structure file in the "
			"same format as the input format, or a sdf-file in the case of a mol "
			"input file. The tool works with the given bond information, which "
			"needs to be correct.\n\n"
			
			"Standard behaviour: only the largest molecule from each entry is "
			"retained ('largest' according to the number of atoms).\n\n"
			
			"Optional parameters:\n"
			"Retain all molecules contained in a molecule entry ('-all'). "
			"Alternatively the minimal number of atoms required for a molecule "
			"to be kept can be specified ('-min_atoms').";
	parpars.setToolManual(man);

	// here we set the types of I/O files
	parpars.setSupportedFormats("i", "mol,sd,sdf,mol2");
	parpars.setSupportedFormats("o", "mol,sd,sdf,mol2");
// Parse the command line
	parpars.parse(argc, argv);


// Decide on output ------------------------------------------------------------
	// read all input files
	String in_name = parpars.get("i");
	
	// first input shall define the output format:
	GenericMolFile* in_file = MolFileFactory::open(in_name);
	String outfile_name = String(parpars.get("o"));
	GenericMolFile* outfile;
	
	// determine output type
	if (dynamic_cast<MOLFile*>(in_file) !=0)
	{
		outfile = new SDFile(outfile_name, ios::out);
	}
	else if (dynamic_cast<MOL2File*>(in_file) !=0)
	{
		outfile = new MOL2File(outfile_name, ios::out);
	}
	else if (dynamic_cast<SDFile*>(in_file) !=0)
	{
		outfile = new SDFile(outfile_name, ios::out);
	}
	else if (in_file == 0)
	{
		Log.error() << "could not open file <" << in_name << "> as structure file"
								<< endl;
		exit(EXIT_FAILURE);
	}
	else
	{
		Log.error() << "input format of file '" << outfile_name << "'" <<
									 " is not supported!" << endl;
		in_file->close();
		delete in_file;
		exit(EXIT_FAILURE);
	}
	
	// check the resulting out file:
	if (outfile->bad())
	{
		Log.error() << "cannot write file '" << outfile_name << "'" << endl;
		exit(EXIT_FAILURE);
	}
	
// Perform separation action according to user input ---------------------------
	 Molecule* tmp_mol;
	 ConnectedComponentsProcessor concp; // create a connectedComponent processor
	 
	// FOR each molecule in the input file:
	// Decompose the molecule into all contained molecules and...
	int cnt = 0;
	int act_cnt = 0;
	int sel_cnt = 0;
	tmp_mol = in_file->read();
	while(tmp_mol != 0)
	{
		cnt++;
		
		// WARN: NO ATOMS IN MOLECULE
		if (tmp_mol->countAtoms() == 0){
			Log.warn() << "WARNING: in input file '" << in_name << "' the molecule "
								 << tmp_mol->getName() << " did not contain any atoms, the"
										" entry was keept." << endl;
			outfile->write((*tmp_mol));
		}
		// ...keep all molecules that are contained:
		else if ( parpars.has("all") ){
			looperALL(act_cnt, sel_cnt, tmp_mol, concp, outfile);
		}
		// ...keep all molcules with a min number of atoms:
		else if ( parpars.has("min_atoms")){
			looperMINIMUM(act_cnt, sel_cnt, tmp_mol, concp, outfile, 
										parpars.get("min_atoms").toInt());
		}
		// ...keep largest molecule:
		else{
			looperLARGE(act_cnt, sel_cnt, tmp_mol, concp, outfile);
		}
		// clean up the last molecule and get the next one:
		delete tmp_mol;
		tmp_mol = in_file->read();
	}
	in_file->close();
	delete in_file;
	
	// WARN: NO MOLECULES IN FILE
	if (cnt == 0){
		Log.warn() << "WARNING: input file '" << in_name << "' did not contain any "
									"molecule, wrote empty output file '" << outfile_name <<
									"'" << endl;
	}
	// Close the result file
	outfile->close();
	delete outfile;
	
	Log.info() << "++ input molecules    "<< cnt << endl;
	Log.info() << "++ found molecules    "<< act_cnt << endl;
	Log.info() << "++ selected molecules "<< sel_cnt << endl;
	Log << "++ Wrote selected molecules to file '" << outfile_name <<
									 "'" << endl;
	Log << "++ DONE." << endl;
	
	return 0;
}
