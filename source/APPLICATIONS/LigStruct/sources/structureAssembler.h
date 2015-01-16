#ifndef STRCUTUREASSEMBLER_H
#define STRCUTUREASSEMBLER_H

#ifndef BASIC_H
#include "basic.h"
#endif

//#include "fragmenter.h"

#ifdef MATCHER_H
#include "matcher.h"
#endif

#ifndef ASSEMBLER_H
#include "assembler.h"
#endif

using namespace OpenBabel;
using namespace BALL;
using namespace std;

class StructureAssembler
{
public:
	StructureAssembler();

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
	void assembleStructure(Molecule* mol, OBMol *ob_mol);
	
	/**
	 * Same as the variant for BALL::Molecule but also assigns the internal 
	 * conncetions between the internal fragments of the contained molecule.
	 * 
	 * @brief assembleStructure
	 * @param mol
	 */
	void assembleStructure(GroupFragment* gmol, OBMol* ob_mol);

	CoordinateMap fragment_lib;
	BondLengthMap bond_lib;
	ConnectionMap connect_lib;
private:
	String fragment_lib_path;
	String bondlenth_lib_path;
	String connection_lib_path;
	
	
	void assemble_ (Molecule* mol, OBMol *ob_mol, 
									list<pair<Atom *, Atom *> > &connections,
									vector<Fragment *> &linker_lst,
									vector<Fragment *> &rigid_lst);
	
};
#endif // STRCUTUREASSEMBLER_H
