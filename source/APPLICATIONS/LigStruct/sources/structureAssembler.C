// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include "structureAssembler.h"

#include <BALL/KERNEL/fragment.h>


using namespace BALL;
using namespace std;

StructureAssembler::StructureAssembler( TemplateLibraryManager& libs )
	: _libs( libs ), 
		_matcher( libs.getFragmentLib() ),
		_linker_builder( libs.getConnectionsLib() )
{
	
}
	
StructureAssembler::~StructureAssembler()
{
	
}

void StructureAssembler::assembleStructure(AtomContainer& mol)
{
	// create fragments from query molecule:
	FragVec rigids, linker;
	ConnectList connections;
	
	_fragmenter.setMolecule(mol);
	_fragmenter.getMoleculeFragments(rigids, linker, connections);
	
	// canonicalize, generate UCK-key and match the rigid fragments
	for( Fragment*& tmp: rigids )
	{
		_canoicalizer.canonicalize( tmp );
		_matcher.matchFragment( *tmp );
	}
	
	// build linker fragments
	for( Fragment*& tmp: linker )
	{
		// WARNING: not yet completely implemented, won't actually work!
		_linker_builder.buildLinker( *tmp ); 
	}
	
	// connect the ready made fragments to a single molecule
}



