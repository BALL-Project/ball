// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/STRUCTURE/LIGAND3DGEN/structureAssembler.h>

using namespace BALL;
using namespace std;

StructureAssembler::StructureAssembler( TemplateDatabaseManager& libs )
	: _libs( libs ), 
		_matcher( libs.getRigidTemplates() ),
		_linker_builder( libs.getSiteTemplates()),
		_clash_resolver(1.2, 3)
{
	_connector.setConnectionLib(libs.getSiteTemplates());
}
	
StructureAssembler::~StructureAssembler()
{
	
}

void StructureAssembler::insertAll(ACVec &linker, ACVec &rigids, AtomContainer& mol)
{
	for(ACVecIter rig_frag = rigids.begin(); rig_frag != rigids.end(); ++rig_frag)
		mol.insert( **rig_frag );
		
	for(ACVecIter lin_frag = linker.begin(); lin_frag != linker.end(); ++lin_frag)
		mol.insert( **lin_frag );	
}

ConnectList* StructureAssembler::assembleStructure(AtomContainer& mol)
{
	// create fragments from query molecule:
	ACVec rigids, linker;
	ConnectList* connections = new ConnectList;

	_fragmenter.setMolecule(mol);
	
	_fragmenter.fragment(rigids, linker, *connections);

	try
	{
		// canonicalize, generate UCK-key and match the rigid fragments
		for(ACVecIter rig_frag = rigids.begin(); rig_frag != rigids.end(); ++rig_frag)
		{
			_canoicalizer.canonicalize( **rig_frag );
			_matcher.matchFragment( **rig_frag );
		}
		
		// build linker fragments
		for(ACVecIter lin_frag = linker.begin(); lin_frag != linker.end(); ++lin_frag)
		{
			// build linker, and insert rotors into 'connections'
			_linker_builder.buildLinker(**lin_frag, *connections);
		}
		
		// connect the ready-made fragments to a single molecule
		for(ConnectList::iterator atm_pair = connections->begin();
				atm_pair != connections->end(); ++atm_pair )
		{
			// only connect if this is not a linker-rotor bond
			if( &atm_pair->first->getRoot() != &atm_pair->second->getRoot() )
			{
				connectClashFree( * atm_pair->first, * atm_pair->second, *connections);
			}
		}
	}
	catch (const Exception::GeneralException& e)
	{
		insertAll(linker, rigids, mol);
		delete connections;
		throw Exception::StructureNotGenerated("structureAssembler.C", 52,
																					 mol, e);
	}
	
	// re-insert all fragments into the original molecule
	//#TODO: perhaps it makes more sense to insert only the "master" root fragment
	insertAll(linker, rigids, mol);
	
	return connections;
}

void StructureAssembler::connectClashFree(Atom& at1, Atom& at2, ConnectList& connections)
{
	//1.) select biggest molecule as 1st connection partner. 
	//    REASON: we assume that our connection method keeps the first fragment 
	//    in place and only transforms the second fragment!

	// get root container to be able to identify all connected atoms of the
	// respective connection-atom:
	AtomContainer* root_1 = (AtomContainer*) &at1.getRoot();
	AtomContainer* root_2 = (AtomContainer*) &at2.getRoot();
	
	int c_cnt;
	if( root_1->countAtoms() > root_2->countAtoms() )
	{
		_connector.connect( &at1, &at2 );

		// 2.) detect and resolve clashes:
		_clash_resolver.setMolecule(at1, at2, connections);
		c_cnt = _clash_resolver.detect();
		
		if( c_cnt != 0 )
		{
			c_cnt = _clash_resolver.resolve().first;
		}
		
		root_1->insert( *root_2 );
	}
	else
	{
		_connector.connect( &at2, &at1 );

		// 2.) detect and resolve clashes:
		_clash_resolver.setMolecule(at2, at1, connections);
		c_cnt = _clash_resolver.detect();
		
		if( c_cnt != 0 )
		{
			c_cnt = _clash_resolver.resolve().first;
		}
		
		root_2->insert( *root_1 );
	}
}



