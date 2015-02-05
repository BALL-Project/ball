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

using namespace BALL;

/// T e m p l a t e L i b r a r y M a n a g e r
/// ############################################################################
/**
 * @brief The IOModule is a class to read, store and mange any library
 * 
 */
class TemplateDatabaseManager
{
public:
	
	TemplateDatabaseManager();
	
	~TemplateDatabaseManager();
	
	/**
	 * @brief readAll reads all libs FOR WHICH A PATH WAS SET
	 */
	void readAll();
	
	/*
	 * Get all needed library information from a config file
	 * that lists the paths to the respective lib files
	 */
	void libraryPathesFromConfig(const String& config_path);
	
	void readBondLenths();
	
	void readSiteTemplates();
	
	void readRigidTemplates();
	
	/*
	 * fragment_lib reader for fragmentLibs that are in SDF Format, converts
	 * the data to unordered_map <String, TemplateCoord*> for efficient internal
	 * representation
	 */
	void readSDFRigidTemplates();
	
	CoordinateMap& getRigidTemplates();
	BondLengthMap& getBondLengthData();
	ConSiteMap& getSiteTemplates();
	
private:
	CoordinateMap _templates_rigids;
	BondLengthMap _bondlengths;
	ConSiteMap _templates_sites;
	
	String _path_to_rigids;
	String _path_to_bondlengths;
	String _path_to_sites;
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
	OpenBabel::OBConversion _babel_conv;
	OpenBabel::OBMol        _babel_mol;
};


/// (static) L i g I O
/// ############################################################################
class LigIO
{
public:
	/**
	 * @brief writeMolVec write vector of BALL::AtomContainer to a single SDFile
	 * @param input
	 * @param handle
	 */
	static void writeMolVec(vector<AtomContainer* >& input, SDFile &handle);
	
	/**
	 * @brief writeMolVec write vector of BALL::AtomContainer to a single 
	 * LineBasedFile. This will only write the molecule key and the coordinates to
	 * save space and reading time.
	 * 
	 * @param input
	 * @param handle
	 */
	static void writeMolVec(vector<AtomContainer*>& input, LineBasedFile& handle);
	
	/**
	 * @brief writeMol same as writeMolVec for a single file
	 * @param mol
	 * @param handle
	 */
	static void writeMol(AtomContainer& mol, LineBasedFile &handle);
	
	/**
	 * @brief writeMol same as writeMolVec for a single file
	 * @param mol
	 * @param handle
	 */
	static void writeMol(AtomContainer& mol, SDFile &handle);
	
	
private:
	static void writePositionLines(AtomContainer& mol, LineBasedFile &handle);
	static void readOBMolecule(const String& path, OpenBabel::OBMol& mol);

};

#endif // IOMODULE_H
