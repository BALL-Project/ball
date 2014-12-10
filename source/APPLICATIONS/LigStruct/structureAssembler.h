#ifndef STRCUTUREASSEMBLER_H
#define STRCUTUREASSEMBLER_H

#include "basic.h"
#include "fragmenter.h"
#include "matcher.h"
#include "assembler.h"

using namespace OpenBabel;
using namespace BALL;
using namespace std;

typedef boost::unordered_map <String, TemplateCoord*> CoordinateMap;// String == UCK key
typedef boost::unordered_map <String, float >         BondLengthMap;
typedef boost::unordered_map <String, Fragment* >     ConnectionMap;

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
	
	void assemble_ (Molecule* mol, 
									boost::unordered_map< Fragment*, list<Bond*> > fragment_connections,
									list< Fragment* > linker_lst,
									list< Bond* > rotor_lst);
	
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
	 * !Brocken, TODO: convert from 'BALL::Molecule' to 'TemplateCoord'
	 * !Deprecated: former FragmentLib reader for fragmentLibs that are in SDF
	 * Format
	 */
	void readOldFragmentLib(boost::unordered_map <String, Molecule*>& fragmentLib);
	
	/*
	 * Get all needed library information from a config file
	 * that lists the paths to the respective lib files
	 */
	void libraryPathesFromConfig(const String& config_path);
};
#endif // STRCUTUREASSEMBLER_H
