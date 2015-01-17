// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef STRCUTUREASSEMBLER_H
#define STRCUTUREASSEMBLER_H

#include "ioModule.h"
#include "fragmenter.h"
#include "fragmentBuilder.h"
#include "moleculeConnector.h"
#include "clashResolver.h"
#include "canonicalizer.h"

//using namespace OpenBabel;
//using namespace BALL;
//using namespace std;

class StructureAssembler
{
public:
	StructureAssembler( TemplateLibraryManager& libs );

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
	
	void connectClashFree(Atom at1, Atom at2);
	
	TemplateLibraryManager& _libs; // needed fragment libs
	
	// Tool-Classes needed for structure fragmenting and reassembly
	MoleculeFragmenter _fragmenter;
	Canonicalizer      _canoicalizer;
	Matcher            _matcher;
	FragmentBuilder    _linker_builder;
	MoleculeConnector  _connector;
	ClashResolver      _clash_resolver;

	
	//
	
};
#endif // STRCUTUREASSEMBLER_H
