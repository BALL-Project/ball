#ifndef STRCUTUREASSEMBLER_H
#include "structureAssembler.h"
#endif

#ifndef MATCHER_H
#include "matcher.h"
#endif

using namespace OpenBabel;
using namespace BALL;
using namespace std;


StructureAssembler::StructureAssembler()
{
	
}

// TODO: delete the libs!
StructureAssembler::~StructureAssembler()
{
	// clean db
	Log << "Removing loaded libs from memory...";
	CoordinateMap::iterator tmp_it;
	for(tmp_it = fragment_lib.begin(); tmp_it!=fragment_lib.end(); tmp_it++)
		delete tmp_it->second; // TODO: this line does not delete the contained molecules!!!!
	
	// bonds are of primitive type and on the stack
	
	ConnectionMap::iterator mit;
	for(mit = connect_lib.begin(); mit != connect_lib.end(); mit++)
		delete mit->second;
}


void StructureAssembler::assembleStructure(GroupFragment* gmol, OBMol* ob_mol)
{
	assemble_(gmol->molecule, ob_mol, gmol->frag_con2, gmol->linker_lst, gmol->rigid_lst);
}

void StructureAssembler::assembleStructure(Molecule* mol, OBMol* ob_mol)
{
	list< pair < Atom*, Atom* > > dummy_connections;
	vector< Fragment* > dummy_linker;
	vector< Fragment* > dummy_rigids;
//	list< Bond* > dummy_rotors;
	
	assemble_(mol, ob_mol, dummy_connections, dummy_linker, dummy_rigids);
}

/*
 * Private underlying method to the public assembly methods
 */
void StructureAssembler::assemble_ (Molecule* mol, OBMol* ob_mol,
																		list< pair < Atom*, Atom* > >& connections,
																		vector< Fragment* >& linker_lst, vector<Fragment *> &rigid_lst)
{
//	// DEBUG
//	OBAtom* at = ob_mol->GetAtom(1);
//	cout<<at->GetType()<<endl;
//	cout <<at->GetHyb()<<endl;
//	OBBondIterator obit = at->BeginBonds();
//	for (; obit != at->EndBonds(); ++obit)
//	{
//		cout<< (*obit)->GetBO()<<endl;
//	}
//	//DEBUG
//	fragmentMolecule(*ob_mol, *mol, rigid_lst, linker_lst, connections);
	cout<<"rigids: "<<rigid_lst.size()<<" inker:"<<linker_lst.size()<<endl;
	
	// canonicalize and match rigid fragments
	cout<<"canonicalizing rigid fragments"<<endl;
	canonicalize(rigid_lst);
	cout<<"matching rigid fragments"<<endl;
	matchRigidFragments(fragment_lib, rigid_lst);
	// DEBUG - start
	for(int i = 0; i< rigid_lst.size(); i++)
	{
		mol->append( *(rigid_lst[i]) );
	}
	SDFile debugfile("/Users/pbrach/debug_file.sdf", ios::out);
	debugfile << *mol;
	debugfile.close();
	// DEBUG - end
	
//	cout<<"building linker frgaments"<<endl;
//	// build every linker fragment from standard torsions
//	vector< Fragment* >::iterator lit = linker_lst.begin();
//	for(; lit != linker_lst.end(); ++lit)
//	{
//		AssemblerFunctions::buildLinker(**lit, connect_lib);
//	}

	// connect the individual fragments
	list< pair < Atom*, Atom* > >::iterator cit = connections.begin();
//	int cnt = 1;
//	for(; cit != connections.end(); ++cit)
//	{
////		cout<< "connecting: "<<printInlineMol(cit->first->getParent()) << " - " << printInlineMol( cit->second->getParent() ) <<endl; //DEBUG
//		AssemblerFunctions::connectFragments( cit->first, cit->second, connect_lib, bond_lib);
//		SDFile tmp_out("/Users/pbrach/in_between_"+String(cnt)+".sdf", ios::out);
//		tmp_out << *mol;
//		tmp_out.close();
//		++cnt;
//	}
	
	AssemblerFunctions::connectFragments( cit->second, cit->first, connect_lib, bond_lib);
	
	SDFile tmp_out("/Users/pbrach/in_between_1.sdf", ios::out);
	tmp_out << *mol;
	tmp_out.close();
	
	++cit;
	AssemblerFunctions::connectFragments( cit->first, cit->second, connect_lib, bond_lib);
	
	SDFile tmp_out2("/Users/pbrach/in_between_2.sdf", ios::out);
	tmp_out2 << *mol;
	tmp_out2.close();
	
	++cit;
	AssemblerFunctions::connectFragments( cit->first, cit->second, connect_lib, bond_lib);
	SDFile tmp_out3("/Users/pbrach/in_between_3.sdf", ios::out);
	tmp_out3 << *mol;
	tmp_out3.close();
	
	
	SDFile debugfile2("/Users/pbrach/debug_file2.sdf", ios::out);
	debugfile2 << *mol;
	debugfile2.close();
}


