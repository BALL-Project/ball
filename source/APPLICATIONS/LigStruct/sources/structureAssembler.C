// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include "structureAssembler.h"

#include <BALL/KERNEL/fragment.h>


using namespace BALL;
using namespace std;

StructureAssembler::StructureAssembler( TemplateDatabaseManager& libs )
	: _libs( libs ), 
		_matcher( libs.getRigidTemplates() ),
		_linker_builder( libs.getSiteTemplates(), libs.getBondLengthData() ),
		_clash_resolver(1.3, 3)
{
	_connector.setLibs(libs.getSiteTemplates(), libs.getBondLengthData());
}
	
StructureAssembler::~StructureAssembler()
{
	
}

void StructureAssembler::assembleStructure(AtomContainer& mol)
{
	// create fragments from query molecule:
	ACVec rigids, linker;
	ConnectList connections;
	
	_fragmenter.setMolecule(mol);
	_fragmenter.fragment(rigids, linker, connections);

	// canonicalize, generate UCK-key and match the rigid fragments
	for( AtomContainer*& tmp: rigids )
	{
		_canoicalizer.canonicalize( *tmp );
		_matcher.matchFragment( *tmp );
	}
	
	// build linker fragments
	ConnectList linker_rotors;
	for( AtomContainer*& tmp: linker )
	{
		linker_rotors.splice(linker_rotors.end(), _linker_builder.buildLinker(*tmp) ); 
	}
	
	// connect the ready-made fragments to a single molecule
	for( auto& atm_pair : connections )
	{
		connectClashFree( * atm_pair.first, * atm_pair.second, connections, linker_rotors);
	}
	
	// re-insert all fragments into the original molecule
	//#TODO:####: perhaps it makes more sense to splice all atoms, or only insert the root of one fragment
	for(AtomContainer*& f : rigids)
	{
		mol.insert( *f );
	}
	for(AtomContainer*& f : linker)
	{
		mol.insert( *f );
	}
}

void StructureAssembler::connectClashFree(Atom& at1, Atom& at2, ConnectList& connections, ConnectList& linker_rotors)
{
	//1.) select biggest molecule as 1st connection partner. 
	//    REASON: we assume that our connection method keeps the first fragment 
	//    in place and only transforms the second fragment!

	// get root container to be able to identify all connected atoms of the
	// respective connection-atom:
	AtomContainer* root_1 = (AtomContainer*) &at1.getRoot();
	AtomContainer* root_2 = (AtomContainer*) &at2.getRoot();
	
//	cout<<endl<<endl<<"#### Connecting:"<<endl;
	int c_cnt;
	if( root_1->countAtoms() > root_2->countAtoms() )
	{
//		cout<<LigBase::printInlineMol(root_1)<<endl;
//		cout<<LigBase::printInlineMol(root_2)<<endl;

		_connector.connect( &at1, &at2 );

		// 2.) detect and resolve clashes:
		_clash_resolver.setMolecule(at1, at2, connections, &linker_rotors);
		c_cnt = _clash_resolver.detect();
		
		if( c_cnt != 0 )
		{
//			cout<<"Resolving clash, got: "<<c_cnt<<endl;
			c_cnt = _clash_resolver.resolve().first;
//			cout<<"Resolving finished with: "<<c_cnt<<endl;
		}
		
		root_1->insert( *root_2 );
	}
	else
	{
//		cout<<LigBase::printInlineMol(root_2)<<endl;
//		cout<<LigBase::printInlineMol(root_1)<<endl;
		_connector.connect( &at2, &at1 );
		
		// 2.) detect and resolve clashes:
		_clash_resolver.setMolecule(at2, at1, connections, &linker_rotors);
		c_cnt = _clash_resolver.detect();
		
		if( c_cnt != 0 )
		{
//			cout<<"Resolving clash, got: "<<c_cnt<<endl;
			c_cnt = _clash_resolver.resolve().first;
//			cout<<"Resolving finished with: "<<c_cnt<<endl;
		}
		
		root_2->insert( *root_1 );
	}
}



