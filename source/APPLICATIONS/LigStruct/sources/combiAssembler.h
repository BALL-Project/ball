#ifndef COMBIASSEMBLER_H
#define COMBIASSEMBLER_H

#include "structureAssembler.h"

class CombiAssembler
{
public:
	CombiAssembler(TemplateDatabaseManager& data, CombiLibMap* clib);
	
	~CombiAssembler();
	
	void setScaffold(RFragment& scaffold);
	void setCombiLib(CombiLibMap& clib);
	
	void writeCombinations(BALL::SDFile& handle);
	
private:
	
	bool _connectClashFree(BALL::Atom &at1, BALL::Atom &at2, ConnectList& connections);
	void _checkAndConnect(RAtom& acceptor, RFragment& donor);
	void _addSet(RFragment& mol);
	
	RFragment*          _work_mol;
	CombiLibMap*        _r_groups;
	std::list< RAtom* > _r_atms;
	
	MoleculeConnector  _connector;
	ConnectionResolver _cresolv;

	std::list< RFragment* >            _current_combination;
	std::list< std::pair<int*, int*> > _work_trace;
	void _combineRecur(BALL::SDFile& handle);
	
	void newSetForCurrentCombination();
	
	int total_reuse; //#DEBUG
};

#endif // COMBIASSEMBLER_H
