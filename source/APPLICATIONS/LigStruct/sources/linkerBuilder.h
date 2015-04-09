// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#ifndef LINKERBUILDER_H
#define LINKERBUILDER_H

#include "base.h"
#include "starAligner.h"
#include "clashResolver.h"
#include "clashBase.h"

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
	 * cycles are normally not considered as flexible)
	 * @param linker_frag
	 * @param result_rotors
	 */
	void buildLinker(BALL::AtomContainer &linker_frag, ConnectList& result_rotors);
	
private:
	void recurLinkerConnect(BALL::Atom &at, const BALL::Composite &parent);
	
	void getPositionFromTemplate(AtmVec& site, BALL::AtomContainer& temp, 
															 BALL::Atom& partner);
	
	BALL::AtomContainer& loadTemplate( BALL::String& key );
	
	void getSite(BALL::Atom& atm, AtmVec &site, BALL::String& key);
	static bool compare(std::pair< BALL::String, BALL::Atom* >& a, 
											std::pair< BALL::String, BALL::Atom* >& b);
	
	void resolveLinkerClashes(BALL::AtomContainer &linker_frag, ConnectList &result_rotors);
	void recurResolveLinker(int previous_cnt, BALL::Bond& bnd, 
													BALL::Atom &curr_atm, int dist, 
													BALL::Composite *parent );
	
	void setBondTrans( BALL::Bond &bnd );
	
	bool isTerminalBond( BALL::Bond& bnd, BALL::Composite& parent);
	
	// input libs:
	SiteMap& _connection_templates;
	BondLengthMap& _bond_lengths;
	
	// helper structures
	BALL::HashSet< BALL::Atom* > _done;
	ConnectList _rotors;

	// helper objects
	StarAligner _aligner;
	ClashResolver _cresolv; // for rotations
	
	Rotator _roto;
};



#endif // LINKERBUILDER_H
