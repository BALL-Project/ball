// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/FORMAT/molFileFactory.h>
#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/FORMAT/SDFile.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/PTE.h> // for element

#include <BALL/STRUCTURE/defaultProcessors.h>
#include <BALL/STRUCTURE/connectedComponentsProcessor.h>
#include <BALL/STRUCTURE/molecularSimilarity.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/bond.h>

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
// check if the atom is a rigid one:
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

// TODO
// cant get the bond of the last fragment of a test molecule deleted
// is suppose we are somehow loosing the atom on the other side and  thus 
// cant delete it any more.
void clearExternalBonds(Molecule* mol)
{
	Atom::BondIterator bit;
	AtomIterator ait;

//	BALL_FOREACH_INTERBOND(*mol, ait, bit)
//	{
//		bit->destroy();
//	}
	for (ait = mol->beginAtom(); !ait.isEnd(); ait++)
	{
		for (bit=ait->beginBond(); !bit.isEnd(); bit++)
		{
			Atom* at1 = bit->getFirstAtom();
			Atom* at2 = bit->getSecondAtom();
			
			cout<< at1->getElement().getSymbol() <<" - " <<at2->getElement().getSymbol()<<endl;

			
			if ( at2->getParent() != mol) 
			{
				at1->destroyBond(*at2);
			}
			
			if (at1->getParent()!= mol)
			{
				at2->destroyBond(*at1);
			}
		}
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
	DisjointSet dset_rigid(&fixed_rank[0], &fixed_parent[0]);
	
	std::vector <int> linker_rank(num_atoms);
	std::vector <int> linker_parent(num_atoms);
	DisjointSet dset_linker(&linker_rank[0], &linker_parent[0]);
	
	std::vector <bool> is_rigid(num_atoms, 0); // bitVec indicating rigid atoms
	std::vector <bool> is_linker(num_atoms, 0); // bitVec indicating linker atoms
	
	// init ring and rotable information:
	obMol.FindRingAtomsAndBonds();
/// TODO: add OBRotorList object to use custom torlib!

	/// iterate over all bonds and split into 2 groups of fragments:
	OBBondIterator b_it = obMol.BeginBonds();
	std::vector< std::pair<int, int> > connections;
	for (; b_it != obMol.EndBonds(); b_it++)
	{
		int id1 = (*b_it)->GetBeginAtom()->GetId();
		int id2 = (*b_it)->GetEndAtom()->GetId();
		// for all rotable bonds:
		if ( (*b_it)->IsRotor() )
		{
			if( !is_linker[id1] )
			{
				dset_linker.make_set(id1); 
				is_linker[id1]=1;
			}
			if( !is_linker[id2] )
			{
				dset_linker.make_set(id2); 
				is_linker[id2]=1;
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
			if( !is_rigid[id1] )
			{
				dset_rigid.make_set(id1); 
				is_rigid[id1]=1;
			}
			if( !is_rigid[id2] )
			{
				dset_rigid.make_set(id2); 
				is_rigid[id2]=1;
			}
			dset_rigid.union_set(id1,id2);
		}
	}

	/// convert from Babel to BALL:
	Molecule* ball_rigid_mol = MolecularSimilarity::createMolecule(obMol, true);
	Molecule* ball_linker_mol = new Molecule(*ball_rigid_mol); // make a copy
	ball_rigid_mol->setName("original_rigid");
	ball_linker_mol->setName("original_linker");
	
	///	iterate over all atoms, sorting out the fragments
	std::vector <int> link_groups(num_atoms, -1);
	std::vector <int> fixe_groups(num_atoms, -1);
	std::vector <Molecule*> fragments;
	
	int frag_id=0;
	int parent_id = -1;
	
	/// create fixed fragments:
	for(int i = 0 ; i < num_atoms; i++)
	{
		if ( ! is_rigid[i] ) // loop only over rigid atoms:
		{
			ball_rigid_mol->remove(*ball_rigid_mol->getAtom(0));
			continue;
		}

		/// Atom is a fixed one:
		Atom* atm = ball_rigid_mol->getAtom(0);

		parent_id = dset_rigid.find_set(i);
		// if fragment does not exist, create it:
		if(fixe_groups[parent_id]<0)
		{
			fixe_groups[parent_id] = frag_id;
			Molecule* dummy = new Molecule();
			dummy->insert(*atm);
			dummy->setName("Fragment_"+toString(frag_id));
			dummy->setProperty("isRigid", true);
			frag_id++;
			fragments.push_back(dummy);
		}
		else
		{
			fragments[ fixe_groups[parent_id] ]->insert(*atm);
		}
	}
	
	/// create linker fragments:
	for(int i = 0 ; i < num_atoms; i++)
	{
		if(!is_linker[i]) // loop only over linker atoms
		{
			ball_linker_mol->remove(*ball_linker_mol->getAtom(0));
			continue;
		}
		Atom* atm = ball_linker_mol->getAtom(0);
		parent_id = dset_linker.find_set(i);
		
		// if the atoms fragment does not exist, create it:
		if(link_groups[parent_id]<0)
		{
			link_groups[parent_id] = frag_id;
			Molecule* dummy = new Molecule();
			dummy->insert(*atm);
			dummy->setName("Fragment_"+toString(frag_id));
			frag_id++;
			dummy->setProperty("isRigid", false);
			
			fragments.push_back(dummy);
		}
		// atom is part of existing fragment:
		else
		{
			fragments[ link_groups[parent_id] ]->insert(*atm);
		}
	}
	
	/// iterate over all connections and assign connections to fragments:
	//	std::vector< std::pair<int, int> >::iterator it;
	//	cout << endl;
	//	cout << "Intra fragment Bonds:"<<endl;
	//	for(it=connections.begin(); it != connections.end();it++)
	//	{
	//		cout << it->first<< " - "<< it->second<< endl;
	//	}

	/// write output:
	String outfile_name = String(parpars.get("o"));
	SDFile outfile(outfile_name, ios::out);
	for(int i = 0; i < fragments.size(); i++)
	{
		clearExternalBonds(fragments[i]);
		//cout<<"fragment "<<i << " #bonds: "<<fragments[i]->countBonds()<<endl;
		outfile << *fragments[i];
	}
	
	Log << "wrote " << fragments.size() <<" fragments to file " << outfile_name << endl;
	
	// clean up:
	fragments.clear();
	delete ball_rigid_mol;
	delete ball_linker_mol;
	outfile.close();
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

