#ifndef COMBIASSEMBLER_H
#define COMBIASSEMBLER_H

#include "base.h"
#include "clashResolver.h"
#include "moleculeConnector.h"
#include "structureAssembler.h"

//using namespace std;
namespace BALL
{

class CombiAssembler
{
public:
	CombiAssembler( StructureAssembler& single_assmbler );
	
	~CombiAssembler();
	
	void setCombiLib();
	
private:
	
	RFragment _scaffold;
	vector< vector< RFragment> r_groups;
	
	MoleculeConnector _connector;
	ClashResolver _cresolv;
	
	void connectRFragments();
};

}

#endif // COMBIASSEMBLER_H
