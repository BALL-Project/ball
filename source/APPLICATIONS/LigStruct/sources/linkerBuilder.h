// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#ifndef LINKERBUILDER_H
#define LINKERBUILDER_H

#include "base.h"
#include "starAligner.h"
#include "clashResolver.h"

#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/fragment.h>
#include <BALL/MATHS/analyticalGeometry.h>
#include <BALL/DATATYPE/hashSet.h>

using namespace BALL;
using namespace std;

/**
 * @brief The LinkerBuilder class
 * A class for calculating coordinates for linker/flexible fragments
 */
class LinkerBuilder{
public:
	LinkerBuilder(SiteMap& connection_templates, BondLengthMap& bonds);
	 
	~LinkerBuilder();
	/**
	 *
	 * @brief buildLinker a flexible linker fragment
	 * This function only works on cycle free molecules (but molecules with 
	 * cycles are normally not considered a flexible)
	 * @param linker_lst
	 */
	ConnectList buildLinker(AtomContainer &linker_frag);
	
private:
	void recurLinkerConnect(Atom &at, const Composite &parent);
	
	void getPositionFromTemplate(AtmVec& site, AtomContainer& temp, Atom& partner);
	
	AtomContainer& loadTemplate( String& key );
	
	void getSite(Atom& atm, AtmVec &site, String& key);
	static bool compare(pair<String,Atom*>& a, pair<String,Atom*>& b);
	
	ConnectList resolveLinkerClashes(AtomContainer &linker_frag);
	void recurResolveLinker(int previous_cnt, Bond& bnd, Atom &curr_atm, int dist, Composite *parent );
	
	void setBondTrans( Bond &bnd );
	
	bool isTerminalBond( Bond& bnd, Composite& parent);
	
	// input libs:
	SiteMap& _connection_templates;
	BondLengthMap& _bond_lengths;
	
	// helper structures
	HashSet< Atom* > _done;
	list< Bond* > _rotors;

	// helper objects
	StarAligner _aligner;
	ClashResolver _cresolv; // for rotations
	
};



#endif // LINKERBUILDER_H
