// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#ifndef LINKERBUILDER_H
#define LINKERBUILDER_H

#include <BALL/STRUCTURE/LIGAND3DGEN/ligand3DBase.h>
#include <BALL/STRUCTURE/LIGAND3DGEN/starAligner.h>
#include <BALL/STRUCTURE/clashResolver.h>

namespace BALL 
{
/**
 * @brief The LinkerBuilder class
 * A class for calculating coordinates for linker/flexible fragments
 */
class LinkerBuilder{
public:
	LinkerBuilder(BALL::SiteMap& connection_templates);
	 
	~LinkerBuilder();
	/**
	 *
	 * @brief buildLinker a flexible linker fragment
	 * This function only works on cycle free molecules (but molecules with 
	 * cycles are normally not considered as flexible)
	 * @param linker_frag
	 * @param result_rotors
	 */
	void buildLinker(BALL::AtomContainer &linker_frag, BALL::ConnectList& result_rotors);
	
private:
	void recurLinkerConnect(BALL::Atom &at, const BALL::Composite &parent);
	
	void getPositionFromTemplate(AtmVec& site, BALL::AtomContainer& temp, 
															 BALL::Atom& partner);
	
	BALL::AtomContainer& loadTemplate( BALL::String& key );
	
	void getSite(BALL::Atom& atm, AtmVec &site, BALL::String& key);
	static bool compare(std::pair< BALL::String, BALL::Atom* >& a, 
											std::pair< BALL::String, BALL::Atom* >& b);
	
	void resolveLinkerClashes(BALL::AtomContainer &linker_frag, BALL::ConnectList &result_rotors);
	void recurResolveLinker(int previous_cnt, BALL::Bond& bnd, 
													BALL::Atom &curr_atm, int dist, 
													BALL::Composite *parent );
	
	void setBondTrans( BALL::Bond &bnd );
	
	bool isTerminalBond( BALL::Bond& bnd, BALL::Composite& parent);
	
	// input libs:
	BALL::SiteMap& _connection_templates;
	
	// helper structures
	BALL::HashSet< BALL::Atom* > _done;
	BALL::ConnectList _rotors;

	// helper objects
	BALL::StarAligner _aligner;
	BALL::ClashResolver _cresolv; // for rotations
	
	BALL::Rotator _roto;
};


} // End Namespace BALL
#endif // LINKERBUILDER_H
