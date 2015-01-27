// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#ifndef FRAGMENTBUILDER_H
#define FRAGMENTBUILDER_H

#include "base.h"
#include "starAligner.h"

#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/fragment.h>

#include <BALL/DATATYPE/hashSet.h>

using namespace BALL;
using namespace std;

/**
 * @brief The FragmentBuilder class
 * A class for calculating coordinates for linker/flexible fragments
 */
class FragmentBuilder{
public:
	FragmentBuilder(ConSiteMap& connection_templates);
	
	~FragmentBuilder();
	/**
	 *
	 * @brief buildLinker a flexible linker fragment
	 * This function only works on cycle free molecules (but molecules with 
	 * cycles are normally not considered a flexible)
	 * @param linker_lst
	 */
	void buildLinker(Fragment &linker_frag);
	
private:
	void recurLinkerConnect(Atom* at, const Composite *parent);
	
	void getPositionFromTemplate(AtmVec& site, AtomContainer& temp, Atom* partner);
	
	// TODO: currently this is exaclty the same as MoleculeConnector::getSite
	// -> deduplicate this
	void getSite(Atom* atm, AtmVec &site, String& key);
	static bool compare(pair<String,Atom*>& a, pair<String,Atom*>& b);
	
	ConSiteMap& _connection_templates;
	StarAligner _aligner;
	HashSet< Atom* > _done;
	
};



#endif // FRAGMENTBUILDER_H
