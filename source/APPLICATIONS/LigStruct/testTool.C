// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/FORMAT/SDFile.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/STRUCTURE/molecularSimilarity.h>

#include <openbabel/obconversion.h>
#include <openbabel/mol.h>
#include <openbabel/canon.h>
#include <openbabel/graphsym.h>

#include <vector>
#include <map>

#include <boost/pending/disjoint_sets.hpp>

using namespace OpenBabel;
using namespace BALL;
using namespace std;

/// ################# M A I N #################
int main(int argc, char* argv[])
{
	CommandlineParser parpars("queryFragments", " generate query fragments and connections", 0.1, String(__DATE__), "Preparation");
	parpars.registerParameter("i", "input SDF", INFILE, true);
	parpars.registerParameter("o", "output SDF", OUTFILE, true);
	
	parpars.setSupportedFormats("i","sdf");
	parpars.setSupportedFormats("o","sdf");
	parpars.setOutputFormatSource("o","i");

	String manual = "...just playing...";
	parpars.setToolManual(manual);

	parpars.parse(argc, argv);
	// ######## START ####################################################
	OBMol obMol; // working molecule
	
	// Read open-babel molecule as input:
	OBConversion conv;
	OBFormat *format = conv.FormatFromExt(parpars.get("i"));
	
	if (!format || !conv.SetInFormat(format))
		cout << "Could not find input format for file " << parpars.get("i") << endl;

	ifstream ifs(parpars.get("i")); // open file
	
	if (!ifs)
		cout << "Could not open " << parpars.get("i") << " for reading." << endl;
	
	conv.Read(&obMol, &ifs); // actual 'read' command
	ifs.close();
	// --------------------did parameter parsing----------------------------------
	
	int num_atoms = obMol.NumAtoms();
///======================Fragment     M A T C H I N G ==========================

	Molecule* ball_mol = MolecularSimilarity::createMolecule(obMol, true);
	
	OBGraphSym grsym(&obMol);
	std::vector<unsigned int> sym;
	grsym.GetSymmetry(sym);
	
	std::vector<unsigned int> clabels;
	CanonicalLabels(&obMol, sym, clabels);
	
	cout<< "canonical labels "<<endl;
	for(int i=0; i<clabels.size(); i++)
		cout<<(i+1) << " - "<< clabels[i]<<endl;
	
	Molecule new_mol;
	std::vector <Atom*> aList(num_atoms);
	for(int i=0; i<clabels.size(); i++)
	{
		aList[clabels[i]-1]=( ball_mol->getAtom(i) );
	}
	cout<<endl;
	
	/// write output:
	String outfile_name = String(parpars.get("o"));
	SDFile outfile(outfile_name, ios::out);
	outfile << new_mol;
	
	Log << "wrote canonical to file " << outfile_name << endl;

	delete ball_mol;
}
