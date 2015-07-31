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
 * A class for calculating coordinates for linker/flexible fragments. These
 * need to be cycle free.
 */
class LinkerBuilder{
public:

	/**
	 * @brief LinkerBuilder - needs a map of valid site-templates from which
	 * query fragments will be generated.
	 * @param connection_templates
	 */
	LinkerBuilder(SiteFragmentDB& connection_templates);
	 
	~LinkerBuilder();

	/**
	 *
	 * @brief buildLinker iteratively builds the structure of flexible molecules
	 * (aka linker fragments) and generates coordinates for every atom.
	 * This function only works on cycle free molecules
	 * @param linker_frag
	 * @param result_rotors
	 */
	void buildLinker(BALL::AtomContainer &linker_frag, BALL::ConnectList& result_rotors);
	
private:
	/*
	 * Recursion part of 'buildLinker'
	 */
	void recurLinkerConnect(BALL::Atom &at, const BALL::Composite &parent);
	
	/*
	 * #TODO# Further generalize the MoleculeConnector and thus remove
	 * several functions from here. Also move the SiteTemplateNoFound exception
	 * to MoleculeConnector.
	 *
	 * Set the position of the 'partner' atom to the free position calculated
	 * from the alignment of the 'site' template to the actual star molecule
	 * 'temp'
	 */
	void getPositionFromTemplate(AtmVec& site, BALL::AtomContainer& temp, 
															 BALL::Atom& partner);
	
	/*
	 * Returns the site template for the given key from the internal
	 * '_connection_templates' map, or throws an exception if no template
	 * could be found.
	 */
	BALL::AtomContainer& loadTemplate( BALL::String& key );
	
	/*
	 * From the given input atom 'atm' derive the correct site template
	 * 'site' and its site key 'key'
	 */
	void getSite(BALL::Atom& atm, AtmVec &site, BALL::String& key);

	/*
	 * Compares two <String, Atom* > pairs on the first element (the string)
	 */
	static bool compare(std::pair< BALL::String, BALL::Atom* >& a, 
											std::pair< BALL::String, BALL::Atom* >& b);
	
	/*
	 * Resolve clashes within the initially generated linker-molecule.
	 *
	 * Only certain bonds are used as rotable bonds. E.g.: we do not rotate every
	 * bond of a straight (unbranched) chain, but rotate the bonds around atoms
	 * that mark branching points.
	 *
	 * The list of selected rotors is saved in 'result_rotors', thus it should
	 * be an empty list in the beginning.
	 */
	void resolveLinkerClashes(BALL::AtomContainer &linker_frag, BALL::ConnectList &result_rotors);

	/*
	 * recursion part of resolveLinkerClashes
	 */
	void recurResolveLinker(int previous_cnt, BALL::Bond& bnd, 
													BALL::Atom &curr_atm, int dist, 
													BALL::Composite *parent );
	
	/*
	 * set a (any) dihedral angle to 180 degree that can be formed from the given
	 * bond the first two neigboring bonds that are found.
	 *
	 * (normally suffices here because we only set a chain consisting of no
	 * branches to trans)
	 */
	void setBondTrans( BALL::Bond &bnd );
	
	/*
	 * Indicates if the bond is connected to a terminal atom. "Terminality" is
	 * checked within a certain fragment given by 'parent', thus it is possible
	 * that the bond is only terminal within the selected fragment but not
	 * within the whole molecule.
	 */
	bool isTerminalBond( BALL::Bond& bnd, BALL::Composite& parent);
	
	/*
	 * counts bonded neighbors of 'atm' within a given fragment of the whole
	 * molecule 'parent'
	 */
	static int countBondsInParent( BALL::Atom& atm, const BALL::Composite &parent);
	
	/*
	 * input lib of site-templates. (templates showing how neighbor atoms are
	 * geometrically connected to a central atom of interest)
	 */
	SiteFragmentDB& _connection_templates;
	
	/*
	 * helper fields
	 */
	BALL::HashSet< BALL::Atom* > _done; // set of atoms that got coordinates assigned
	BALL::ConnectList _rotors; // list of rotable bonds that allow us to resolve clashes

	/*
	 * supporting objects/functions:
	 */
	BALL::StarAligner _aligner;
	BALL::ClashResolver _cresolv;
	
	BALL::Rotator _roto;
};


} // End Namespace BALL
#endif // LINKERBUILDER_H
