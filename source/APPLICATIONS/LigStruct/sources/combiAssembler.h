#ifndef COMBIASSEMBLER_H
#define COMBIASSEMBLER_H

#include "base.h"
#include "clashResolver.h"
#include "moleculeConnector.h"
#include "structureAssembler.h"

class CombiAssembler
{
public:
	CombiAssembler(RFragment* scaffold = 0, CombiLibMap* clib = 0);
	
	~CombiAssembler();
	
	void setScaffold(RFragment& scaffold);
	void setCombiLib(CombiLibMap& clib);
	
	void writeCombinations(SDFile& handle);
	
	void getCombinations( std::list< BALL::AtomContainer*>& result );
private:
	
	void connectClashFree(Atom &at1, Atom &at2, ConnectList& connections);
	
	RFragment*          _work_mol;
	CombiLibMap*        _r_groups;
	std::list< RAtom* > _r_atms;
	
	MoleculeConnector       _connector;
	ConnectionClashResolver _cresolv;

};

#endif // COMBIASSEMBLER_H
