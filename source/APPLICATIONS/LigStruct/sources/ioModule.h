// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef IOMODULE_H
#define IOMODULE_H

#include "base.h"

#include <BALL/DATATYPE/string.h>

#include <BALL/FORMAT/SDFile.h>

#include <openbabel/obconversion.h>
#include <openbabel/mol.h>

using namespace OpenBabel;
using namespace BALL;
//using namespace std;

/// T e m p l a t e L i b r a r y M a n a g e r
/// ############################################################################
/**
 * @brief The IOModule is a class to read, store and mange any library
 * 
 */
class TemplateLibraryManager
{
public:
	
	TemplateLibraryManager();
	
	~TemplateLibraryManager();
	
	/**
	 * @brief readAll reads all libs FOR WHICH A PATH WAS SET
	 */
	void readAll();
	
	/*
	 * Get all needed library information from a config file
	 * that lists the paths to the respective lib files
	 */
	void libraryPathesFromConfig(const String& config_path);
	
	void readBondLib();
	
	void readConnectionLib();
	
	void readFragmentLib();
	
	/*
	 * fragment_lib reader for fragmentLibs that are in SDF Format, converts
	 * the data to unordered_map <String, TemplateCoord*> for efficient internal
	 * representation
	 */
	void readSDFFragmentLib();
	
	CoordinateMap& getFragmentLib();
	BondLengthMap& getBondLengthlib();
	ConSiteMap& getConnectionsLib();
	
private:
	CoordinateMap _fragment_lib;
	BondLengthMap _bond_lib;
	ConSiteMap _connect_lib;
	
	String _fragment_lib_path;
	String _bondlenth_lib_path;
	String _connection_lib_path;
};

/// C o m b i L i b M a n a g e r
/// ############################################################################
class CombiLibManager
{
public:
	CombiLibManager();
	~CombiLibManager();
	
	void readCombiLib(const String& file_name);
	
	const vector<String>& operator[] (Index index);
	
	const Size size();
private:
	String      _scaffold;
	CombiLibMap _lib;
};


/// S m i l e s P a r s e r
/// ############################################################################
class SmilesParser
{
public:
	SmilesParser();
	~SmilesParser();
	
	Molecule *fromSMILEStoMolecule(const String& smiles_string);
	
	GroupFragment* fromSMILEStoGroupfragment(const String& smiles_string);
	
private:
	OBConversion _babel_conv;
	OBMol        _babel_mol;
};


/// (static) L i g I O
/// ############################################################################
class LigIO
{
public:
	static void writeMolVec(vector<AtomContainer* >& input, SDFile &handle);
	static void writeMolVec(vector<AtomContainer*>& input, LineBasedFile& handle);
	
	static void readOBMolecule(const String& path, OBMol& mol);
	
private:
	
	static void writePositionLines(AtomContainer& mol, LineBasedFile &handle);

};

#endif // IOMODULE_H
