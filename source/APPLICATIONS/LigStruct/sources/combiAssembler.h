#ifndef COMBIASSEMBLER_H
#define COMBIASSEMBLER_H

#include "base.h"
#include "clashResolver.h"
#include "moleculeConnector.h"
#include "structureAssembler.h"

#include <map>
#include <set>

class CombiAssembler
{
public:
	CombiAssembler(TemplateDatabaseManager& data, CombiLibMap* clib);
	
	~CombiAssembler();
	
	void setScaffold(RFragment& scaffold);
	void setCombiLib(CombiLibMap& clib);
	
	void writeCombinations(SDFile& handle);
	
//	void getCombinations( std::list< BALL::AtomContainer*>& result );
private:
	
	bool _connectClashFree(Atom &at1, Atom &at2, ConnectList& connections);
	void _checkAndConnect(RAtom& acceptor, RFragment& donor);
	void _addSet(RFragment& mol);
	
	RFragment*          _work_mol;
	CombiLibMap*        _r_groups;
	std::list< RAtom* > _r_atms;
	
	MoleculeConnector       _connector;
	ConnectionClashResolver _cresolv;

	void _combineRecur(SDFile& handle);
};

#endif // COMBIASSEMBLER_H
