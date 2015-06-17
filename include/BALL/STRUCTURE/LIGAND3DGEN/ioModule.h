// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef IOMODULE_H
#define IOMODULE_H

#include <BALL/STRUCTURE/LIGAND3DGEN/ligand3DBase.h>
#include <BALL/STRUCTURE/UCK.h>

#include <BALL/FORMAT/SDFile.h>

#include <openbabel/obconversion.h>
#include <openbabel/mol.h>

namespace BALL 
{
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
	void libraryPathesFromConfig(const BALL::String& config_path);
	
	void readSiteTemplates();
	
	void readRigidTemplates();
	
	/*
	 * fragment_lib reader for fragmentLibs that are in SDF Format, converts
	 * the data to unordered_map <String, TemplateCoord*> for efficient internal
	 * representation
	 */
	void readSDFRigidTemplates();
	
	BALL::RigidsMap& getRigidTemplates();
	BALL::SiteMap& getSiteTemplates();
	
private:
	BALL::RigidsMap _templates_rigids;
	BALL::SiteMap _templates_sites;
	
	BALL::String _path_to_rigids;
	BALL::String _path_to_sites;
};


/// S m i l e s P a r s e r
/// ############################################################################
class SmilesParserOB
{
public:
	SmilesParserOB();
	~SmilesParserOB();
	
	BALL::AtomContainer *fromSMILEStoMolecule(const BALL::String& smiles_string);
	
	BALL::RFragment* fromSMILEStoRFragment(const BALL::String& smiles_string, const int& g_id = -1);
	
private:
	OpenBabel::OBConversion _babel_conv;
	OpenBabel::OBMol        _babel_mol;
	BALL::Canonicalizer           _cano;
	
};


/// C o m b i L i b M a n a g e r
/// ############################################################################
class CombiLibManager
{
public:
	CombiLibManager(BALL::LineBasedFile * combilib_file=0);
	~CombiLibManager();
	
	void setCombiLib(BALL::LineBasedFile &combilib_file);
	
	BALL::RFragment& getScaffold();
	BALL::CombiLibMap& getCombiLib();
	
	void generateCombinationsSMILES( std::list<BALL::String>& out_SMILES);
	void generateCombinationsAtomContainer(
			std::list<BALL::AtomContainer *> &out_molecules);
	
private:
	void _sortRGroups();
	/*
	 * Returns true if frag1 has more atoms than frag2
	 */
	bool static _compareRFrag(BALL::RFragment*& frag1, BALL::RFragment*& frag2 );
	void _parseCombiLibFile();
	void _recurGenerateCombi( std::list<BALL::AtomContainer *> &out_molecules);
	
	BALL::LineBasedFile* _combilib_file;
	BALL::RFragment*           _scaffold;
	BALL::CombiLibMap          _lib;
	std::list< BALL::RAtom* >  _r_atms;
	
	bool         _lib_is_generated;
	BALL::SmilesParserOB _smi_parser;
	boost::unordered_map< int, int > _id_mapping;
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
	static void writeMolVec(std::vector<BALL::AtomContainer* >& input, BALL::SDFile &handle);
	
	/**
	 * @brief writeMolVec write vector of BALL::AtomContainer to a single 
	 * LineBasedFile. This will only write the molecule key and the coordinates to
	 * save space and reading time.
	 * 
	 * @param input
	 * @param handle
	 */
	static void writeMolVec(std::vector<BALL::AtomContainer*>& input, BALL::LineBasedFile& handle);
	
	/**
	 * @brief writeMol same as writeMolVec for a single file
	 * @param mol
	 * @param handle
	 */
	static void writeMol(BALL::AtomContainer& mol, BALL::LineBasedFile &handle);
	
	/**
	 * @brief writeMol same as writeMolVec for a single file
	 * @param mol
	 * @param handle
	 */
	static void writeMol(BALL::AtomContainer& mol, BALL::SDFile &handle);
	
	
private:
	static void writePositionLines(BALL::AtomContainer& mol, BALL::LineBasedFile &handle);
	static void readOBMolecule(const BALL::String& path, OpenBabel::OBMol& mol);

};

} // End Namespace "BALL"
#endif // IOMODULE_H
