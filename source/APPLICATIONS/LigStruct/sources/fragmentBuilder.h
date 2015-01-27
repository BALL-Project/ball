// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#ifndef FRAGMENTBUILDER_H
#define FRAGMENTBUILDER_H

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
 * @brief The FragmentBuilder class
 * A class for calculating coordinates for linker/flexible fragments
 */
class FragmentBuilder{
public:
	FragmentBuilder(ConSiteMap& connection_templates, BondLengthMap& bonds);
	
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
	
	void getSite(Atom* atm, AtmVec &site, String& key);
	static bool compare(pair<String,Atom*>& a, pair<String,Atom*>& b);
	
	void findRotors(Fragment &linker_frag);
	void recurFindRotors(int previous_cnt, Bond& bnd, Atom &curr_atm, Composite *parent );
	
	void setBondTrans( Bond &bnd );
	
	// input libs:
	ConSiteMap& _connection_templates;
	BondLengthMap& _bond_lengths;
	
	// helper structures
	HashSet< Atom* > _done;
	list< Bond* > _rotors;

	// helper objects
	StarAligner _aligner;
	ClashResolver _cresolv; // for rotations
	
};



#endif // FRAGMENTBUILDER_H
