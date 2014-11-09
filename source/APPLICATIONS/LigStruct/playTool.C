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
#include <openbabel/rotor.h>

#include <vector>
#include <unordered_set>
#include <utility> // for 'pair'

//#include <boost/pending/disjoint_sets.hpp>

using namespace OpenBabel;
using namespace BALL;
using namespace std;

/// ################# H E L P E R    F U N C T I O N S #################
bool isAtomRigid(OBAtom* atm)
{
/// TODO: add OBRotorList object to use custom torlib!
	if (atm->IsInRing())
		return true;
	else
	{
		OBBondIterator b_it = atm->BeginBonds();
		for(; b_it!=atm->EndBonds(); b_it++)
		{
			if( ! (*b_it)->IsRotor() )
				return true;
		}
		return false;
	}
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
	// --------------------did molecule reading-----------------------------------
	
	typedef boost::disjoint_sets < int*, int*, boost::find_with_full_path_compression > DisjointSet;
	int num_atoms = obMol.NumAtoms();
	
	// create 2 empty disjoint sets:
	std::vector <int> fixed_rank(num_atoms);
	std::vector <int> fixed_parent(num_atoms);
	DisjointSet dset_fixed(&fixed_rank[0], &fixed_parent[0]);
	
	std::vector <int> linker_rank(num_atoms);
	std::vector <int> linker_parent(num_atoms);
	DisjointSet dset_linker(&linker_rank[0], &linker_parent[0]);
	
	std::vector <bool> fixed_ids(num_atoms, 0);
	std::vector <bool> rotab_ids(num_atoms, 0);
	
	// init ring and rotable information:
	obMol.FindRingAtomsAndBonds();
/// TODO: add OBRotorList object to use custom torlib!

	// iterate over all bonds, split into 2 groups of fragments:
	OBBondIterator b_it = obMol.BeginBonds();
	std::vector< std::pair<int, int> > connections;
	for (; b_it != obMol.EndBonds(); b_it++)
	{
		int id1 = (*b_it)->GetBeginAtom()->GetId();
		int id2 = (*b_it)->GetEndAtom()->GetId();
		// for all rotable bonds:
		if ( (*b_it)->IsRotor() )
		{
			if( !rotab_ids[id1] )
			{
				dset_linker.make_set(id1); 
				rotab_ids[id1]=1;
			}
			if( !rotab_ids[id2] )
			{
				dset_linker.make_set(id2); 
				rotab_ids[id2]=1;
			}
			dset_linker.union_set(id1, id2);
			
			// rotables are also connecting the fragments, find the connections:
			if( isAtomRigid((*b_it)->GetBeginAtom()) || isAtomRigid((*b_it)->GetEndAtom()) )
			{
				connections.push_back( make_pair( id1, id2) );
			}
		}
		// for all fixed bonds:
		else
		{
			if( !fixed_ids[id1] )
			{
				dset_fixed.make_set(id1); 
				fixed_ids[id1]=1;
			}
			if( !fixed_ids[id2] )
			{
				dset_fixed.make_set(id2); 
				fixed_ids[id2]=1;
			}
			dset_fixed.union_set(id1,id2);
		}
	}
	
	cout<<endl;
	for(int i = 0 ; i < num_atoms; i++)
	{
		int pare = -1;
		if (fixed_ids[i])
		{
			pare = dset_fixed.find_set(i);
			cout << "Atom:"<< i<<" "<<obMol.GetAtomById(i)->GetType()<< " is fixed, "<<pare<<endl;
			if(rotab_ids[i])
				cout<<"also linker atom!"<<endl;
		}
		else
		{
			pare = dset_linker.find_set(i);
			cout << "Atom:"<< i<<" "<<obMol.GetAtomById(i)->GetType()<< " is linker, "<<pare<<endl;
		}
	}
	std::vector< std::pair<int, int> >::iterator it;
	cout << endl;
	cout << "Intra fragment Bonds:"<<endl;
	for(it=connections.begin(); it != connections.end();it++)
	{
		cout << it->first<< " - "<< it->second<< endl;
	}
	// convert from Babel to BALL:
	Molecule* ball_mol2 = MolecularSimilarity::createMolecule(obMol, true);

	// write output:
	String outfile_name = String(parpars.get("o"));
	SDFile outfile(outfile_name, ios::out);
	outfile << *ball_mol2;
	Log << "wrote file " << outfile_name << endl;
	
	// clean up:
	delete ball_mol2;
	outfile.close();
	
	// END of program------------------------------------------
	vector <int> rank(5,0);
	vector <int> parent(5,0);
	DisjointSet dset(&rank[0], &parent[0]);
	
	dset.make_set(2);
	dset.make_set(3);
	dset.union_set(2,3);
	//dset.make_set(2);
	cout<<endl;
	for (int i =0; i <5 ;i++)
	{
		cout<< i << " p: "<<parent[i]<<" r:"<< rank[i]<< " finder: "<< dset.find_set(i)<<endl;
	}
	cout<<dset.find_set(1);
	
}

/// Apply ConComp Processor:
//	ConnectedComponentsProcessor conpro;
//	ball_mol2->apply(conpro);
//	vector<Molecule> fragments;
//	conpro.getAllComponents(fragments);
/// Deletion of certain bonds to create ring fragments:
//	vector<OBBond*> for_deletion;
//	FOR_BONDS_OF_MOL(b, rings2)
//		if (b->IsRotor()) // && !b->IsInRing()) // in babel ring bonds are never rotors!!!!
//			for_deletion.push_back(&(*b));
//	for(vector<OBBond*>::iterator it=for_deletion.begin(); it!=for_deletion.end(); ++it)
//		rings2.DeleteBond(*it);
/// delete non ring atoms:
//	vector<OBAtom*> atm_deletion;
//	FOR_ATOMS_OF_MOL(at, rings2)
//			if (!at->IsInRing())
//				atm_deletion.push_back(&(*at));
//	for(vector<OBAtom*>::iterator it2=atm_deletion.begin(); it2!=atm_deletion.end(); ++it2)
//		rings2.DeleteAtom(*it2);
/// write files in babel style:
//	OBConversion conv;
//	conv.SetOutFormat("sdf");
//	ofstream ring_file;
//	ring_file.open (String(parpars.get("o")+"found_rings.sdf"));
//	conv.Write(&rings, &ring_file);
//	ring_file.close();

