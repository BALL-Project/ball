#ifndef COMBIASSEMBLER_H
#define COMBIASSEMBLER_H

#include <BALL/STRUCTURE/LIGAND3DGEN/structureAssembler.h>

namespace BALL 
{


class CombiAssembler
{
public:
	CombiAssembler(TemplateDatabaseManager& data, BALL::CombiLibMap* clib);
	
	~CombiAssembler();
	
	void setScaffold(BALL::RFragment& scaffold);
	void setCombiLib(BALL::CombiLibMap& clib);
	
	void writeCombinations(BALL::SDFile& handle);
	
	
private:
	
	bool _connectClashFree(BALL::Atom &at1, BALL::Atom &at2, BALL::ConnectList& connections);
	void _checkAndConnect(BALL::RAtom& acceptor, BALL::RFragment& donor);
	void _addSet(BALL::RFragment& mol);
	
	BALL::RFragment*          _work_mol;
	BALL::CombiLibMap*        _r_groups;
	std::list< BALL::RAtom* > _r_atms;
	
	MoleculeConnector  _connector;
	ConnectionResolver _cresolv;

	std::list< BALL::RFragment* >            _current_combination;
	void _combineRecur(BALL::SDFile& handle);
	
	void newSetForCurrentCombination();
};

}// End Namespace BALL
#endif // COMBIASSEMBLER_H
