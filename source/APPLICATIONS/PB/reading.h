// $Id: reading.h,v 1.2 2000/05/25 11:02:42 oliver Exp $

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

// read a charge and radius rule file
void readRuleFile(const String& filename);

// dump the positions, charges, and radii to a file
void dumpFile();
