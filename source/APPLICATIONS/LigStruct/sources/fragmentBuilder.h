// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#ifndef FRAGMENTBUILDER_H
#define FRAGMENTBUILDER_H

#include "base.h"

#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/fragment.h>

using namespace BALL;
using namespace std;

class FragmentBuilder{
public:
	FragmentBuilder();
	
	~FragmentBuilder();
	/**
	 *
	 * @brief buildLinker a flexible linker fragment
	 * @param linker_lst
	 */
	void buildLinker(Fragment &linker_frag, ConSiteMap &link_lib);
	
private:
	void recurLinkerConnect(Atom* at, Composite * const parent, ConSiteMap& link_lib);
	void connectAtomToSite(AtmVec& site, AtomContainer& temp, Atom* partner);
};



#endif // FRAGMENTBUILDER_H
