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

/**
 * @brief The StructureAssembler class - create valid 3D coordinates for
 * a single molecule
 */
class StructureAssembler
{
public:
	StructureAssembler( RigidFragmentDB& rigid_db, SiteFragmentDB& site_db);

	~StructureAssembler();

	/**
	 * Takes a molecule as input and splits it to several fragments
	 * (either rigid or linker fragments). For every fragment coordinates are
	 * generated and connected fragments are transformed in such a way that
	 * they give the correct 3D structure of the whole molecule.
	 *
	 * @brief assembleStructure
	 * @param mol
	 * @return - returns the found list of rotable atom pairs
	 */
	BALL::ConnectList *assembleStructure(BALL::AtomContainer& mol);
	
private:
	
	/*
	 * Insert all fragments ('linker' and 'rigids' back into the molecule 'mol'
	 * to re-unite all atom coordinates in the molecule entry
	 */
	void insertAll(BALL::ACVec& linker, BALL::ACVec& rigids, BALL::AtomContainer& mol);
	
	/*
	 * Use a BALL::MoleculeConnector to connect 2 fragments and make sure via
	 * BALL::ClashResolver, that the connection does not cause any clash.
	 *
	 * @param at1 - the atom from the first fragment
	 * @param at2 - atom of the second fragment
	 * @param connections - list of known connecting bonds, that are also rotable
	 *                      and thus important for resolving clashes by rotating
	 *                      along those bonds.
	 */
	void connectClashFree(BALL::Atom &at1, BALL::Atom &at2, 
												BALL::ConnectList& connections);
	
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
