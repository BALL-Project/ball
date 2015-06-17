// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef STRCUTUREASSEMBLER_H
#define STRCUTUREASSEMBLER_H

#include <BALL/STRUCTURE/fragmenter.h>
#include <BALL/STRUCTURE/LIGAND3DGEN/linkerBuilder.h>
#include <BALL/STRUCTURE/LIGAND3DGEN/ioModule.h>
#include <BALL/STRUCTURE/clashResolver.h>
#include <BALL/STRUCTURE/LIGAND3DGEN/moleculeConnector.h>

namespace BALL 
{

/// C l a s s  S t r u c t u r e A s s e m b l e r
/// ############################################################################
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
	BALL::ConnectList *assembleStructure(BALL::AtomContainer& mol);
	
private:
	
	void insertAll(BALL::ACVec& linker, BALL::ACVec& rigids, BALL::AtomContainer& mol);
	
	void connectClashFree(BALL::Atom &at1, BALL::Atom &at2, 
												BALL::ConnectList& connections);
	
	TemplateDatabaseManager& _libs; // needed fragment libs
	
	// Tool-Classes needed for structure fragmenting and reassembly
	BALL::MoleculeFragmenter _fragmenter;
	BALL::Canonicalizer      _canoicalizer;
	BALL::Matcher            _matcher;
	BALL::LinkerBuilder      _linker_builder;
	BALL::MoleculeConnector  _connector;
	BALL::ConnectionResolver _clash_resolver;
};

} // End Namespace "BALL"
#endif // STRCUTUREASSEMBLER_H
