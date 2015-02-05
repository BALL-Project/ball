// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef STRCUTUREASSEMBLER_H
#define STRCUTUREASSEMBLER_H

#include "ioModule.h"
#include "fragmenter.h"
#include "linkerBuilder.h"
#include "moleculeConnector.h"
#include "clashResolver.h"
#include "canonicalizer.h"

using namespace BALL;
using namespace std;

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
	 */
	void assembleStructure(AtomContainer& mol);
	
private:
	
	void connectClashFree(Atom &at1, Atom &at2, ConnectList& connections, ConnectList &linker_rotors);
	
	TemplateDatabaseManager& _libs; // needed fragment libs
	
	// Tool-Classes needed for structure fragmenting and reassembly
	MoleculeFragmenter _fragmenter;
	Canonicalizer      _canoicalizer;
	Matcher            _matcher;
	LinkerBuilder      _linker_builder;
	MoleculeConnector  _connector;
	ConnectionClashResolver _clash_resolver;

	
	//
	
};
#endif // STRCUTUREASSEMBLER_H
