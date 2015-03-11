// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef STRCUTUREASSEMBLER_H
#define STRCUTUREASSEMBLER_H

#include "fragmenter.h"
#include "linkerBuilder.h"
#include "ioModule.h"
#include "clashResolver.h"
#include "moleculeConnector.h"


class StructureAssembler
{
public:
	StructureAssembler( TemplateDatabaseManager& libs );

	~StructureAssembler();

	/**
	 * Takes a read molecule (with a normal atomlist only) and splits the former
	 * molecule into several fragments (rigid or linker) that are then back 
	 * inserted into the parent molecule.
	 * Coordinates are generated to yield the complete structure for the molecule
	 * 
	 * @brief assembleStructure
	 * @param mol
	 * @return - returns the found list of rotable atom pairs
	 */
	ConnectList *assembleStructure(BALL::AtomContainer& mol);
	
private:
	
	void insertAll(ACVec& linker, ACVec& rigids, BALL::AtomContainer& mol);
	
	void connectClashFree(BALL::Atom &at1, BALL::Atom &at2, 
												ConnectList& connections);
	
	TemplateDatabaseManager& _libs; // needed fragment libs
	
	// Tool-Classes needed for structure fragmenting and reassembly
	MoleculeFragmenter _fragmenter;
	Canonicalizer      _canoicalizer;
	Matcher            _matcher;
	LinkerBuilder      _linker_builder;
	MoleculeConnector  _connector;
	ConnectionResolver _clash_resolver;

	
	//
	
};
#endif // STRCUTUREASSEMBLER_H
