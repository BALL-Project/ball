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

#include <boost/unordered_map.hpp>

namespace BALL 
{

/// S m i l e s P a r s e r
/// ############################################################################
/**
 * @brief The SmilesParserOB class - Wrapper for the OpenBabel SMILES parser
 * to work with normal BALL::AtomContainer. It is necessary as BALLs parser
 * fails in many cases.
 * #TODO#: write working SMILES parser in BALL
 */
class SmilesParserOB
{
public:
	SmilesParserOB();
	~SmilesParserOB();
	
	/**
	 * @brief fromSMILEStoMolecule - takes any valid smiles input string
	 * to convert it to a BALL::AtomContainer
	 * @param smiles_string
	 * @return
	 */
	BALL::AtomContainer *fromSMILEStoMolecule(const BALL::String& smiles_string);
	
	/**
	 * @brief fromSMILEStoRFragment - interprets the SMILES string differently.
	 * The string needs to be in the format of a combilib-file-SMILES. Thus R-atom
	 * and group-atom ids are searched and assigned. DOES NOT assign the rotatable
	 * bonds!!!
	 * @param smiles_string
	 * @param g_id
	 * @return
	 */
	BALL::RFragment* fromSMILEStoRFragment(const BALL::String& smiles_string, const int& g_id = -1);
	
private:
	OpenBabel::OBConversion _babel_conv;
	OpenBabel::OBMol        _babel_mol;
	BALL::Canonicalizer     _cano;
};


/// C o m b i L i b M a n a g e r
/// ############################################################################
/**
 * @brief The CombiLibManager class - Parses a .combi file and returns a
 * CombiLibMap.
 */
class CombiLibManager
{
public:
	CombiLibManager(BALL::LineBasedFile& combilib_file);
	~CombiLibManager();

	/**
	 * @brief getScaffold - return the central scaffold of the combilib.
	 * @return
	 */
	BALL::RFragment& getScaffold();

	/**
	 * @brief getCombiLib - return the complete combilib
	 * @return
	 */
	BALL::CombiLibMap& getCombiLib();
	
	/**
	 * @brief generateCombinationsSMILES - generate a list of all possible
	 * SMILES from the input combilib.
	 * @param out_SMILES
	 */
	void generateCombinationsSMILES( std::list<BALL::String>& out_SMILES);

	/**
	 * @brief generateCombinationsAtomContainer - generate all possible
	 * combinations as BALL::AtomContainers without any coordinates (all are
	 * set to zero)
	 * @param out_molecules
	 */
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
	
	BALL::LineBasedFile*      _combilib_file;
	BALL::RFragment*          _scaffold;
	BALL::CombiLibMap         _lib;
	std::list< BALL::RAtom* > _r_atms;
	
	bool                             _lib_is_generated;
	BALL::SmilesParserOB             _smi_parser;
	boost::unordered_map< int, int > _id_mapping;
};

} // End Namespace "BALL"
#endif // IOMODULE_H
