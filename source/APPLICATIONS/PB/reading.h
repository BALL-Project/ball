// $Id: reading.h,v 1.1 2000/05/05 12:33:39 oliver Exp $

// read the contents of a PDB file and assign
// charges and radii 
void readPDBFile(const String& filename);

// read the contents of a PDB file and assign
// charges and radii 
void readHINFile(const String& filename);

// read the contents of a PDB file and assign radii 
void readHINFileNoAssignment(const String& filename);

// read an option file
void readOptionFile(const String& filename);

// read a radius file
void readRadiusFile(const String& filename);

// read a charge file
void readChargeFile(const String& filename);

