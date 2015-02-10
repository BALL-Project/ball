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
	
	RFragment*        _scaffold;
	CombiLibMap*      _r_groups;
	
	MoleculeConnector _connector;
	ClashResolver     _cresolv;
	
	void connectRFragments();
};

#endif // COMBIASSEMBLER_H
