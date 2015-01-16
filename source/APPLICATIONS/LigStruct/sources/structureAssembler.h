#ifndef STRCUTUREASSEMBLER_H
#define STRCUTUREASSEMBLER_H

#ifdef MATCHER_H
#include "matcher.h"
#endif

#include "ioModule.h"

//using namespace OpenBabel;
//using namespace BALL;
//using namespace std;

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
	void assembleStructure(Molecule* mol);
	
	/**
	 * Same as the variant for BALL::Molecule but also assigns the internal 
	 * conncetions between the internal fragments of the contained molecule.
	 * 
	 * @brief assembleStructure
	 * @param mol
	 */
	void assembleStructure(GroupFragment* gmol);

private:
	TemplateLibraryManager _libs;
	
	
	void assemble_ (Molecule* mol, ConnectList &connections,
									FragVec &linker_lst, FragVec &rigid_lst);
	
};
#endif // STRCUTUREASSEMBLER_H
