// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/FORMAT/molFileFactory.h>
#include <BALL/FORMAT/commandlineParser.h>

#include <BALL/STRUCTURE/defaultProcessors.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/STRUCTURE/molecularSimilarity.h>
#include <BALL/FORMAT/SDFile.h>

#include <BALL/STRUCTURE/connectedComponentsProcessor.h>
#include <openbabel/obconversion.h>
#include <openbabel/mol.h>
#include <openbabel/shared_ptr.h>
#include <openbabel/ring.h>

#include "testHeader.h"

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

shared_ptr<OBMol> getMol(const std::string &filename)
{
  // Create the OBMol object.
  shared_ptr<OBMol> mol(new OBMol);

  // Create the OBConversion object.
  OBConversion conv;
  OBFormat *format = conv.FormatFromExt(filename.c_str());
  if (!format || !conv.SetInFormat(format)) {
    std::cout << "Could not find input format for file " << filename << std::endl;
    return mol;
  }

  // Open the file.
  std::ifstream ifs(filename.c_str());
  if (!ifs) {
    std::cout << "Could not open " << filename << " for reading." << std::endl;
    return mol;
  }
  // Read the molecule.
  if (!conv.Read(mol.get(), &ifs)) {
    std::cout << "Could not read molecule from file " << filename << std::endl;
    return mol;
  }

  return mol;
}
/// ################# M A I N #################
int main(int argc, char* argv[])
{
	CommandlineParser parpars("Molecule2Fragments", "cut a molecule along its bonds, generating fragments", 0.1, String(__DATE__), "Preparation");
	parpars.registerParameter("i", "input SDF", INFILE, true);
	parpars.registerParameter("o", "output SDF", OUTFILE, true);

	parpars.setSupportedFormats("i","sdf");
	parpars.setSupportedFormats("o","sdf");
	parpars.setOutputFormatSource("o","i");

	String manual = "...just playing...";
	parpars.setToolManual(manual);

	parpars.parse(argc, argv);

	SDFile input(parpars.get("i"), ios::in);
	if (!input)
	{
		Log.error() << "Cannot open SDF input file " << parpars.get("i") << endl;
		return 2;
	}

	shared_ptr<OBMol> obMol = getMol(parpars.get("i"));
	Molecule ball_mol;
	input >> ball_mol;
	input.close();

	// init ring and rotable information:
	obMol->FindRingAtomsAndBonds();
	
	// number of rotables and location of rotables:
	OBBondIterator b_it;
	obMol->BeginBond(b_it);
	for (; b_it != obMol->EndBonds(); b_it++)
	{
		OBBond *bond = (*b_it);
		if (bond->IsRotor())
		{
			cout <<"rotor at: "<< bond->GetBeginAtom()->GetType()<< " - "<< bond->GetEndAtom()->GetType()<< endl;
		}
	}
	cout << "Number of rotables: "<<obMol->NumRotors()<<endl;
	cout <<endl;
	
	// Iterate over ring-atoms:
	OBAtomIterator at_it;
	obMol->BeginAtom(at_it);
	int num_ring_atms = 0;
	OBMol rings;
	OBMol rings2 = *obMol;
	for(; at_it != obMol->EndAtoms(); at_it++)
	{
		OBAtom* at = (*at_it);
		if (at->IsInRing())
		{
			rings.AddAtom(*at);
			num_ring_atms++;
			cout <<"Ring Atm: "<< at->GetType()<<at->GetIndex()<< endl;
		}
		else
		{
			cout <<"NORMAL Atm: "<< at->GetType()<<at->GetIndex()<< endl;
		}
	}
	cout << "Number of ring atoms: "<<num_ring_atms<<endl;
	cout<<endl;
	
	// Deletion of certain bonds to
	// create ring fragments
	vector<OBBond*> for_deletion;
  FOR_BONDS_OF_MOL(b, rings2)
    if (b->IsRotor() && !b->IsInRing())
      for_deletion.push_back(&(*b));
  for(vector<OBBond*>::iterator it=for_deletion.begin(); it!=for_deletion.end(); ++it)
    rings2.DeleteBond(*it);
	// delete non ring atoms:
//	vector<OBAtom*> atm_deletion;
//	FOR_ATOMS_OF_MOL(at, rings2)
//			if (!at->IsInRing())
//				atm_deletion.push_back(&(*at));
//	for(vector<OBAtom*>::iterator it2=atm_deletion.begin(); it2!=atm_deletion.end(); ++it2)
//		rings2.DeleteAtom(*it2);
	
	OBBondIterator rb_it;
	obMol->BeginBond(rb_it);
	int bondcnt = 0;
	for(; rb_it != obMol->EndBonds(); rb_it++)
	{
		OBBond* bn = *rb_it;
		if ((*rb_it)->IsInRing())
		{
			cout<< "adding worked "<<rings.AddBond(**rb_it);
			cout<<" ring bond: "<< bn->GetBeginAtom()->GetType() << bn->GetBeginAtom()->GetIndex()<<" - " << bn->GetEndAtom()->GetType()<< bn->GetEndAtom()->GetIndex()<< endl;
			
			bondcnt++;
			cout<< "total number of bonds in ring file: "<<rings.NumBonds()<<endl;
		}
	}
	cout << "total ring-bonds: "<<bondcnt << endl;
	cout<<endl;
	

//	// write files in babel style:
//	OBConversion conv;
//  conv.SetOutFormat("sdf");
//	ofstream ring_file;
//	ofstream ring_file2;
//	ring_file.open (String(parpars.get("o")+"found_rings.sdf"));
//	ring_file2.open (String(parpars.get("o")+"2found_rings.sdf"));
//	conv.Write(&rings, &ring_file);
//	conv.Write(&rings2, &ring_file2);
//	ring_file.close();
//	ring_file2.close();
	
	cout <<endl;
	
	Molecule* ball_mol2 = MolecularSimilarity::createMolecule(rings2, true);
	ConnectedComponentsProcessor conpro;
	ball_mol2->apply(conpro);
	vector<Molecule> fragments;
	conpro.getAllComponents(fragments);
	
	// write output:
	String outfile_name = String(parpars.get("o"));
	SDFile outfile(outfile_name, ios::out);
	writeMolVec(fragments, &outfile);
	outfile.close();

	Log << "wrote file " << outfile_name << endl;
}


