
#include "combiAssembler.h"



BALL::CombiAssembler::CombiAssembler(RFragment* scaffold, CombiLibMap* clib):
	_scaffold(scaffold), _r_groups(clib)
{}

CombiAssembler::~CombiAssembler()
{
	
}

void CombiAssembler::setScaffold(RFragment &scaffold)
{
	_scaffold = &scaffold;
}

void CombiAssembler::setCombiLib(CombiLibMap &clib)
{
	_r_groups = &clib;
}

void CombiAssembler::connectRFragments()
{
	Atom* at1 = 0;
	Atom* at2 = 0;
	
	Bond* tmp_bond = at1->createBond(*at2);
	
	AtomContainer* root_1 = (AtomContainer*) &at1->getRoot();
	AtomContainer* root_2 = (AtomContainer*) &at2->getRoot();
	
	if( root_1->countAtoms() > root_2->countAtoms() )
	{
		
	}
	else
	{
		
	}
	
	// after connecion succeeded, delete the bond again
	tmp_bond->destroy();
}
