// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef CANONICALIZER_H
#define CANONICALIZER_H

#include "base.h"

/// C l a s s   C a n o n i c a l i z e r
/// ############################################################################
class Canonicalizer
{
public:
	Canonicalizer();
	~Canonicalizer();

	void canonicalize(BALL::AtomContainer &molecule);
	
private:
	//#TODO: create OBGraphSym and OBmol as private member
};

/// C l a s s   M a t c h e r
/// ############################################################################
class Matcher
{
public:
	Matcher( RigidsMap& coord_map );
	~Matcher();
	
	void matchFragment(BALL::AtomContainer &fragment);
	
	static const BALL::String getUCK(BALL::AtomContainer & mol);
	
private:
	RigidsMap& _coord_lib;
};

#endif // CANONICALIZER_H
