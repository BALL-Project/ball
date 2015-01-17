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
	FragmentBuilder(ConSiteMap& connection_templates);
	
	~FragmentBuilder();
	/**
	 *
	 * @brief buildLinker a flexible linker fragment
	 * @param linker_lst
	 */
	void buildLinker(Fragment &linker_frag);
	
private:
	void recurLinkerConnect(Atom* at, Composite * const parent);
	
	void connectAtomToSite(AtmVec& site, AtomContainer& temp, Atom* partner);
	
	ConSiteMap& _connection_templates;
};



#endif // FRAGMENTBUILDER_H
