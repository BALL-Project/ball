// $Id: reading.C,v 1.1 2000/05/05 12:33:39 oliver Exp $

#include "global.h"
#include "reading.h"
#include "assignment.h"
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/FORMAT/HINFile.h>

void addToSystem(System& system)
{
	// splice the system into the global system
	S.splice(system);
}

void readPDBFile(const String& filename)
{
	// read a PDB file
	PDBFile f;
	System system;
	f.open(filename);
	if (f.bad())
	{
		Log.error() << "Could not open HIN file " << filename << " for reading." << endl;
		Log.error() << "Aborted." << endl;
		exit(4);
	}
	f >> system;
	f.close();
	
	// print the number of atoms read in verbose mod
	if (verbose)
	{
		Log.info() << "read " << system.countAtoms() << " atoms from " << filename << endl;
	}
	
	// normalize the names
	normalizeNames(system);

	// assign charges and radii
	assignCharges(system);
	assignRadii(system);
	
	// insert into the system
	addToSystem(system);
}

void readSystemFromHINFile(const String& filename, System& system)
{
	// read a HIN file
	HINFile f;
	f.open(filename);
	if (f.bad())
	{
		Log.error() << "Could not open HIN file " << filename << " for reading." << endl;
		Log.error() << "Aborted." << endl;
		exit(4);
	}
	f >> system;
	f.close();
	
	// print the number of atoms read in verbose mod
	if (verbose)
	{
		Log.info() << "read " << system.countAtoms() << " atoms from " << filename << endl;
	}

	// normalize the names
	normalizeNames(system);

	// assign radii
	assignRadii(system);
}

void readHINFile(const String& filename)
{
	System system;
	readSystemFromHINFile(filename, system);
	assignCharges(system);
	addToSystem(system);
}

void readHINFileNoAssignment(const String& filename)
{
	System system;
	readSystemFromHINFile(filename, system);
	addToSystem(system);
}

void readOptionFile(const String& filename)
{
	if (verbose)
	{
		Log.info() << "reading options from " << filename << endl;
	}
	options.readOptionFile(filename);
}

void readChargeFile(const String& filename)
{
	if (verbose)
	{
		Log.info() << "reading charges from " << filename << endl;
	}
	charges.setFilename(filename);
}

void readRadiusFile(const String& filename)
{
	if (verbose)
	{
		Log.info() << "reading charges from " << filename << endl;
	}
	radii.setFilename(filename);
}

