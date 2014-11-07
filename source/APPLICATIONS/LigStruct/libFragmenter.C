// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/FORMAT/SDFile.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/STRUCTURE/molecularSimilarity.h>
#include <BALL/STRUCTURE/connectedComponentsProcessor.h>

#include <openbabel/mol.h>
#include <openbabel/shared_ptr.h>
#include <openbabel/obconversion.h>

using namespace OpenBabel;
using namespace BALL;
using namespace std;

/// ################# H E L P E R    F U N C T I O N S #################
// Write several result molecules to one file
void writeMolVec(vector<Molecule> &input, SDFile* handle)
{
	for(int i = 0; i < input.size(); i++)
	{
		(*handle) << input[i];
	}
}

/// ################# M A I N #################
int main(int argc, char* argv[])
{
	CommandlineParser parpars("libFragmenter", "cut a molecule along its rotable bonds, generating fragments", 0.1, String(__DATE__), "Preparation");
	parpars.registerParameter("i", "input SDF", INFILE, true);
	parpars.registerParameter("o", "output SDF", OUTFILE, true);

	parpars.setSupportedFormats("i","sdf");
	parpars.setSupportedFormats("o","sdf");
	parpars.setOutputFormatSource("o","i");

	String manual = "For creating a fragment library.";
	parpars.setToolManual(manual);

	parpars.parse(argc, argv);
	
	// START of CODE#############################################################
	
	OBMol obMol; // working molecule
	
	// Read open-babel molecule as input:
	OBConversion conv;
	OBFormat *format = conv.FormatFromExt(parpars.get("i"));
	
	if (!format || !conv.SetInFormat(format))
		cout << "Could not find input format for file " << parpars.get("i") << endl;

	ifstream ifs(parpars.get("i")); // open file
	
	if (!ifs)
		cout << "Could not open " << parpars.get("i") << " for reading." << endl;
	
	// open output file:
	String outfile_name = String(parpars.get("o"));
	SDFile outfile(outfile_name, ios::out);
	
	// Read all molecules.
	vector<OBBond*> for_deletion;
	vector<OBBond*>::iterator it;
	vector<Molecule> fragments;
	Molecule* ball_mol;
	while ( conv.Read(&obMol, &ifs) )
	{
		// init ring and rotable information:
		obMol.FindRingAtomsAndBonds();
		
		for_deletion.clear();
		fragments.clear();
		
		// Delete rotor-bonds to create fragments (of rings and rigid-groups)
		FOR_BONDS_OF_MOL(b, obMol)
			if (b->IsRotor()) // && !b->IsInRing()) // in babel ring bonds are never rotors!!!!
				for_deletion.push_back(&(*b));
		for(it=for_deletion.begin(); it!=for_deletion.end(); ++it)
			obMol.DeleteBond(*it);
		
		// get everything that is larger than 2 molecules (convert to BALL for that)
		ball_mol = MolecularSimilarity::createMolecule(obMol, true);
		ConnectedComponentsProcessor conpro;
		ball_mol->apply(conpro);
		conpro.getMinAtomsComponents(fragments, 2);
		
		// write to output:
		writeMolVec(fragments, &outfile);
		delete ball_mol;
		obMol.Clear();
	}
	outfile.close();
	ifs.close();
}
