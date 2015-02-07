
#include "combiAssembler.h"



BALL::CombiAssembler::CombiAssembler(StructureAssembler &single_assmbler):
	_single_assmbler(single_assmbler){}

CombiAssembler::~CombiAssembler()
{
	
}

void CombiAssembler::connectRFragments()
{
	Atom* at1 = 0;
	Atom* at2 = 0;
	
	Bond tmp_bond = at1->createBond(*at2);
	
	AtomContainer* root_1 = (AtomContainer*) &at1.getRoot();
	AtomContainer* root_2 = (AtomContainer*) &at2.getRoot();
	
	if( root_1->countAtoms() > root_2->countAtoms() )
	{
		
	}
	else
	{
		
	}
	
	// after connecion succeeded, delete the bond again
	tmp_bond.destroy();
}
