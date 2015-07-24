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
	BALL::Canonicalizer     _cano;
	
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
