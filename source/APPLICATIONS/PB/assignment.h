// $Id: assignment.h,v 1.2 2000/06/02 09:32:53 oliver Exp $

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

