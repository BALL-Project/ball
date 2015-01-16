#ifndef STRCUTUREASSEMBLER_H
#include "structureAssembler.h"
#endif

#include "canonicalizer.h"

using namespace BALL;
using namespace std;

StructureAssembler::StructureAssembler(){}

StructureAssembler::~StructureAssembler(){}

void StructureAssembler::assembleStructure(GroupFragment* gmol)
{
	assemble_(gmol->molecule, gmol->frag_con2, gmol->linker_lst, gmol->rigid_lst);
}

void StructureAssembler::assembleStructure(Molecule* mol)
{
	list< pair < Atom*, Atom* > > dummy_connections;
	vector< Fragment* > dummy_linker;
	vector< Fragment* > dummy_rigids;
//	list< Bond* > dummy_rotors;
	
	assemble_(mol, dummy_connections, dummy_linker, dummy_rigids);
}

/*
 * Private underlying method to the public assembly methods
 */
void StructureAssembler::assemble_ (Molecule* mol, ConnectList& connections,
																		FragVec& linker_lst, FragVec &rigid_lst)
{
//	fragmentMolecule(*ob_mol, *mol, rigid_lst, linker_lst, connections);
	cout<<"rigids: "<<rigid_lst.size()<<" inker:"<<linker_lst.size()<<endl;
	
	// canonicalize and match rigid fragments
	Canonicalizer cano;
	for(FGVIter fit = rigid_lst.begin(); fit != rigid_lst.end(); ++fit)
	{
		cano.canonicalize( (AtomContainer*)*fit );
	}
//	matchRigidFragments(fragment_lib, rigid_lst);
	
	// build every linker fragment from standard torsions
	vector< Fragment* >::iterator lit = linker_lst.begin();
	for(; lit != linker_lst.end(); ++lit)
	{
//		AssemblerFunctions::buildLinker(**lit, connect_lib);
	}

	// connect the individual fragments
	list< pair < Atom*, Atom* > >::iterator cit = connections.begin();
//	int cnt = 1;
//	for(; cit != connections.end(); ++cit)
//	{
//		AssemblerFunctions::connectFragments( cit->first, cit->second, connect_lib, bond_lib);
//		SDFile tmp_out("/Users/pbrach/in_between_"+String(cnt)+".sdf", ios::out);
//		tmp_out << *mol;
//		tmp_out.close();
//		++cnt;
//	}
	
//	AssemblerFunctions::connectFragments( cit->second, cit->first, connect_lib, bond_lib);
}


