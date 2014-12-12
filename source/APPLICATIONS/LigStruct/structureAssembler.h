#ifndef STRCUTUREASSEMBLER_H
#define STRCUTUREASSEMBLER_H

#ifndef BASIC_H
#include "basic.h"
#endif

#ifndef FRAGMENTER_H
#include "fragmenter.h"
#endif

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

	/**
	 * @brief setLibsFromConfig reads a config file that indicates locations of
	 *				all necessary libary files.
	 * @param path to config file
	 */
	void setLibsFromConfig(const String& path);

private:
	String fragment_lib_path;
	String bondlenth_lib_path;
	String connection_lib_path;
	
	CoordinateMap fragment_lib;
	BondLengthMap bond_lib;
	ConnectionMap connect_lib;
	
	void assemble_ (Molecule* mol, OBMol *ob_mol, 
									list<pair<Atom *, Atom *> > &connections,
									vector<Fragment *> &linker_lst,
									vector<Fragment *> &rigid_lst);
	
	/*
	 * Reads the standard bond lengths from a lib file
	 */
	void readBondLib();
	
	/*
	 * Read the connection site from a library file
	 */
	void readConnectionLib();
	
	/*
	 * Reads the template coordinates for all fragments
	 */
	void readFragmentLib();
	
	/*
	 * fragment_lib reader for fragmentLibs that are in SDF Format, converts
	 * the data to unordered_map <String, TemplateCoord*> for efficient internal
	 * representation
	 */
	void readSDFFragmentLib();
	
	/*
	 * Get all needed library information from a config file
	 * that lists the paths to the respective lib files
	 */
	void libraryPathesFromConfig(const String& config_path);
};
#endif // STRCUTUREASSEMBLER_H
