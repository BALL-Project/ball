// $Id: assignment.h,v 1.1 2000/05/05 12:33:37 oliver Exp $

// assign charges to a system and honour the clear_charge flag
void assignCharges(System& system);

// assign radii to a system
void assignRadii(System& system);

// normalize names (if desired)
// and create the fragment DB if rquired
void normalizeNames(System& system);
