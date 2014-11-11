// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/FORMAT/SDFile.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/STRUCTURE/molecularSimilarity.h>
#include <BALL/STRUCTURE/connectedComponentsProcessor.h>
#include <BALL/STRUCTURE/UCK.h>

#include <openbabel/mol.h>
#include <openbabel/obconversion.h>
#include <openbabel/canon.h>
#include <openbabel/graphsym.h>


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
	
	int cntr=0;
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
///----------------------Refine Fragments---------------------------
/// CANONICAL Rows:
		vector<Molecule>::iterator fit;
		for(fit = fragments.begin(); fit!=fragments.end(); fit++)
		{
			// get babel mol:
			OBMol* temp = MolecularSimilarity::createOBMol(*fit, true);
			
			// get helper for canonicalisation:
			OBGraphSym grsym(temp);
			std::vector<unsigned int> sym;
			grsym.GetSymmetry(sym);
			
			// get mapping for canonical labels:
			std::vector<unsigned int> clabels;
			CanonicalLabels(temp, sym, clabels);
			
			Molecule* new_mol = new Molecule;
			std::vector <Atom*> aList( (*fit).countAtoms() );
			for(int i=0; i<clabels.size(); i++)
			{
				aList[clabels[i]-1]=( (*fit).getAtom(i) );
			}

			for(int i=0; i<clabels.size(); i++)
				new_mol->append(*aList[i]);
			
			(*fit).swap(*new_mol);
			
/// generate UCK Keys:
			UCK keyGen(*fit, true, 5);
			
			(*fit).setProperty("key", keyGen.getUCK());
		}
		
/// write to output-------------------------------------------------------------
		writeMolVec(fragments, &outfile);
		delete ball_mol;
		obMol.Clear();
		cntr++;
	}
	outfile.close();
	ifs.close();
	Log << "read "<< cntr<<" input structures, wrote "<< fragments.size()<<" fragments to: " << outfile_name << endl;
}
