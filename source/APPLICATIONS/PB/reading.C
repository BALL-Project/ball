// $Id: reading.C,v 1.4 2000/06/02 09:32:54 oliver Exp $

#include <BALL/FORMAT/PDBFile.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/FORMAT/INIFile.h>
#include "global.h"
#include "reading.h"
#include "assignment.h"
#include <fstream>

using namespace std;

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

	// build the bonds
	buildBonds(system);

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
	
	// ignore the rule-based processors
	use_charge_rules = false;
}

void readRadiusFile(const String& filename)
{
	if (verbose)
	{
		Log.info() << "reading radii from " << filename << endl;
	}
	radii.setFilename(filename);
	
	// ignore the rule-based processors
	use_radius_rules = false;
}

void readRuleFile(const String& filename, RuleType rule_type)
{
	if (verbose)
	{
		switch (rule_type)
		{
			case CHARGES_AND_RADII:
				Log.info() << "reading charge and radius rules from " << filename << endl;
				break;
			case CHARGES:
				Log.info() << "reading charge rules from " << filename << endl;
				break;
			case RADII:
				Log.info() << "reading radius rules from " << filename << endl;
		}
	}

	// open the rules file
	INIFile ini(filename);
	
	// read the rules
	ini.read();

	switch (rule_type)
	{
		case CHARGES_AND_RADII:
			charge_rules.initialize(ini, "ChargeRules");
			radius_rules.initialize(ini, "RadiusRules");
			use_charge_rules = true;
			use_radius_rules = true;
			break;

		case RADII:
			radius_rules.initialize(ini, "RadiusRules");
			use_radius_rules = true;
			break;

		case CHARGES:
			charge_rules.initialize(ini, "ChargeRules");
			use_charge_rules = true;
	}
}

void dumpFile()
{
	if (verbose)
	{
		Log.info() << "dumping atom positions, charges, and radii to " << dump_file << endl;
	}

	ofstream outfile(dump_file.c_str(), ios::out);
	
	outfile << "#   PB dump file" << endl;
	outfile << "# atom      x[A]   y[A]   z[A]   charge[e0]  radius[A]" << endl;
	outfile << "#------------------------------------------------------" << endl;
	
	AtomIterator it = S.beginAtom();
	double total_charge = 0.0;
	for (; +it; ++it)
	{
		total_charge += it->getCharge();
		outfile << it->getFullName() 
						<< " " << it->getPosition().x
						<< " " << it->getPosition().y
						<< " " << it->getPosition().z
						<< " " << it->getCharge()
						<< " " << it->getRadius() << endl;
	}
	outfile << "# total charge: " << total_charge << " e0" << endl;
	outfile.close();
}
