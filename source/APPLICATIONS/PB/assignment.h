// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: assignment.h,v 1.2.2.1 2003/01/07 13:20:20 anker Exp $

// assign charges to a system and honour the clear_charge flag
void assignCharges(System& system);

// assign radii to a system
void assignRadii(System& system);

// normalize names (if desired)
// and create the fragment DB if rquired
void normalizeNames(System& system);

// build bonds (if desired)
// and create the fragment DB if rquired
void buildBonds(System& system);

