// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/FORMAT/SDFile.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/atomContainer.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/STRUCTURE/UCK.h>
#include <BALL/STRUCTURE/molecularSimilarity.h>

#include <openbabel/obconversion.h>
#include <openbabel/mol.h>
#include <openbabel/canon.h>
#include <openbabel/graphsym.h>

#include <vector>

#include <boost/unordered_map.hpp>
#include <boost/pending/disjoint_sets.hpp>

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

// cut bonds that are shared with atoms from other fragments:
void clearExternalBonds(Molecule* mol)
{
	Atom::BondIterator bit;
	AtomIterator ait;

	BALL_FOREACH_INTERBOND(*mol, ait, bit)
	{
		bit->destroy();
	}
}

// Copy properties from an original molecule to a copy molecule
void copyMoleculeProperies(Molecule &orig, Molecule &cop)
{
	NamedPropertyIterator  it;
	for(it = orig.beginNamedProperty (); it !=orig.endNamedProperty(); it++)
	{
		cop.setProperty(*it);
	}
	cop.setName(orig.getName());
}

void setCoordinates(Molecule* query, Molecule* templat)
{
	AtomIterator qit = query->beginAtom();
	AtomIterator tit = templat->beginAtom();
	for (; qit != query->endAtom(); qit++, tit++)
	{
		qit->setPosition( tit->getPosition() );
	}
}

/// ################# M A I N #################
int main(int argc, char* argv[])
{
	CommandlineParser parpars("queryFragments", " generate query fragments and connections", 0.1, String(__DATE__), "Preparation");
	parpars.registerParameter("i", "input SDF", INFILE, true);
	parpars.registerParameter("o", "output SDF", OUTFILE, true);
	parpars.registerParameter("l", "in SDF fragmentLib", INFILE, true);
	
	parpars.setSupportedFormats("i","sdf");
	parpars.setSupportedFormats("o","sdf");
	parpars.setSupportedFormats("l","sdf");
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
	
	obMol.FindRingAtomsAndBonds(); // find rings

	// --------------------did parameter parsing----------------------------------
///#######################    F R A G M E N T I N G    #######################
	/// convert from Babel to BALL:
	Molecule* ball_mol = MolecularSimilarity::createMolecule(obMol, true);
	ball_mol->setName("original_mol");
	
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

	/// iterate over all bonds and split into rigid and linker fragments:
	OBBondIterator b_it = obMol.BeginBonds();
	std::list< pair<Atom*, Atom*> > connections;
	std::map< Atom*, int> atm_to_pos;
	
	cout<<"Creating fragments from your queryStructure..."<<endl;
	for (; b_it != obMol.EndBonds(); b_it++)
	{
		OBAtom* atm1 = (*b_it)->GetBeginAtom();
		OBAtom* atm2 =  (*b_it)->GetEndAtom();
		int id1 = atm1->GetId();
		int id2 = atm2->GetId();

		// for all rotable bonds:
		if ( (*b_it)->IsRotor() )
		{
			bool isRigid_atm1 = isAtomRigid(atm1);
			bool isRigid_atm2 = isAtomRigid(atm2);
			if( !(is_linker[id1] || isRigid_atm1) ) // do not double assign (deletes disjointset-parent status)
			{
				dset_linker.make_set(id1);
				is_linker[id1]=1;
			}
			if( !(is_linker[id2] || isRigid_atm2) ) // do not double assign (deletes disjointset-parent status)
			{
				dset_linker.make_set(id2);
				is_linker[id2]=1;
			}
			// add union atoms if both are not fixed:
			if( !(isRigid_atm1 || isRigid_atm2) )// both need to be linker atoms (checked via isAtomRigid(atm) )
			{
				dset_linker.union_set(id1, id2);
			}
			
			// rotables to a rigid fragment define connection points:
			if( isAtomRigid((*b_it)->GetBeginAtom()) || isAtomRigid((*b_it)->GetEndAtom()) )
			{
				Atom* atm1 = ball_mol->getAtom(id1);
				Atom* atm2 = ball_mol->getAtom(id2);
				connections.push_back(make_pair(atm1, atm2));
			}
		}
		// for all fixed bonds:
		else
		{
			if( !is_rigid[id1] ) // do not double assign (deletes disjointset-parent status)
			{
				dset_rigid.make_set(id1); 
				is_rigid[id1]=1;
			}
			if( !is_rigid[id2] ) // do not double assign (deletes disjointset-parent status)
			{
				dset_rigid.make_set(id2); 
				is_rigid[id2]=1;
			}
			dset_rigid.union_set(id1,id2);
		}
	}
	
	///	iterate over all atoms, sorting out the  fixed and linker fragments
	std::vector <int> link_groups(num_atoms, -1); // map parent id to vector pos in variable 'fragments'
	std::vector <int> rigid_groups(num_atoms, -1); // map parent id to vector pos in variable 'fragments'
	std::vector <Molecule*> fragments;
	
	int parent_id = -1;
	// loop over the atoms, sort fragments out
	for(int i = 0 ; i < num_atoms; i++)
	{
		Atom* atm = ball_mol->getAtom(0);
		
		// TODO: remove as atm_to_pos is only for debugging:
		atm_to_pos[atm]=i;
		// atom is linker, extend a linker fragment
		if ( is_linker[i] )
		{
			parent_id = dset_linker.find_set(i);
			
			// if the atoms fragment does not exist, create it:
			if(link_groups[parent_id]<0)
			{
				link_groups[parent_id] = fragments.size();
				Molecule* dummy = new Molecule();
				
				dummy->insert(*atm);
				dummy->setName("Fragment_"+toString(fragments.size()));
				dummy->setProperty("isRigid", false);
				
				fragments.push_back(dummy);
			}
			// atom is part of existing fragment:
			else
			{
				fragments[ link_groups[parent_id] ]->insert(*atm);
			}
		}
		// Atom is a rigid, extend a rigid fragment
		else
		{
			parent_id = dset_rigid.find_set(i);
			// if fragment does not exist, create it:
			if(rigid_groups[parent_id]<0)
			{
				rigid_groups[parent_id] = fragments.size();
				Molecule* dummy = new Molecule();
				
				dummy->insert(*atm);
				dummy->setName("Fragment_"+toString(fragments.size()));
				dummy->setProperty("isRigid", true);
				fragments.push_back(dummy);
			}
			else
			{
				fragments[ rigid_groups[parent_id] ]->insert(*atm);
			}
		}
	}
	
	/// iterate over all connections and assign connections to fragments:
	cout << endl;
	cout << "following inter fragment bonds exist:"<<endl;
	list< pair<Atom*, Atom*> >::iterator coit;
	for(coit=connections.begin(); coit!=connections.end(); coit++)
	{
		Atom* atm1 = coit->first;
		Atom* atm2 = coit->second;
		cout<<((Molecule*)atm1->getParent())->getName();
		cout<<" atm "<<atm_to_pos[atm1];
		cout<< " to "<< ((Molecule*)atm2->getParent())->getName();
		cout<<" atm "<<atm_to_pos[atm2];
		cout<<endl;
	}
	cout<<endl;

///#######################    M A T C H I N G    #######################
///---------------make atomlists of the fragments canonical---------------------
	cout<<"Canonicalising the atomlists..."<<endl;
	std::vector< Molecule* >::iterator it;
	Molecule* new_mol;
	for(it=fragments.begin(); it != fragments.end(); it++)
	{
		clearExternalBonds(*it);
		OBMol* temp = MolecularSimilarity::createOBMol(**it, true);
		
		OBGraphSym grsym(temp);
		std::vector<unsigned int> sym;
		grsym.GetSymmetry(sym);
		
		std::vector<unsigned int> clabels;
		CanonicalLabels(temp, sym, clabels);
		
		new_mol = new Molecule;
		std::vector <Atom*> aList(num_atoms);
		for(int i=0; i<clabels.size(); i++)
			aList[clabels[i]-1]=( (*it)->getAtom(i) );
		
		for(int i=0; i<clabels.size(); i++)
			new_mol->append(*aList[i]);
		
		copyMoleculeProperies(**it, *new_mol);
		(*it)->swap(*new_mol);
		
		delete new_mol;
	}

///-------------------match queryFragments to libFragments----------------------
	// Load fragment lib:
	boost::unordered_map <BALL::String, Molecule*> fragmentLib;
	
	String libfile_name = String(parpars.get("l"));
		cout<<"Reading fragmentLib from: "<< libfile_name<<" ..."<<endl;
	SDFile libFile(libfile_name, ios::in);
	
	Molecule* tmp_mol;
	tmp_mol = libFile.read();
	
	// read in fragmentLib and create hash-map from that:
	while(tmp_mol)
	{
		BALL::String key = tmp_mol->getProperty("key").getString();
		fragmentLib[key] = tmp_mol;
		
		tmp_mol = libFile.read();
	}


	// get coordinates for rigid fragments
	cout<<"Assigning template coordinates to rigid fragments..."<<endl;
	std::vector< Molecule* >::iterator it2;
	for(it2=fragments.begin(); it2 != fragments.end(); it2++)
	{
		// for all rigid fragments, match these against the lib:
		if( (*it2)->getProperty("isRigid").getBool() )
		{
			UCK keyGen(**it2, true, 5);
			Molecule* templat = fragmentLib[ keyGen.getUCK() ];
			
			if(templat && (templat->countAtoms() == (*it2)->countAtoms()) )
				setCoordinates(*it2, templat);
			else
				cout<<"Warning: could not find a template for "<< (*it2)->getName()<<endl;
		}
	}
	
	
	
///#######################    A S S E M B L E   3 D    #######################
/// ??? Nothing so far

/// #######################    F I N I S H    #######################
///-------------------------------write output----------------------------------
	String outfile_name = String(parpars.get("o"));
	SDFile outfile(outfile_name, ios::out);
	for(int i = 0; i < fragments.size(); i++)
	{
		outfile << *fragments[i];
	}
	
	Log << "wrote " << fragments.size() <<" fragments to file " << outfile_name << endl;
///---------------------------------Clean up------------------------------------
	// clean up:
	delete ball_mol;
	outfile.close();
	
	// clean db
	boost::unordered_map <BALL::String, Molecule*>::iterator map_it;
	for(map_it = fragmentLib.begin(); map_it!=fragmentLib.end(); map_it++)
		delete map_it->second;
}
