// $Id: files.C,v 1.1 2000/06/10 11:23:19 oliver Exp $

#include <BALL/FORMAT/PDBFile.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/FORMAT/INIFile.h>
#include <BALL/MOLMEC/AMBER/amber.h>
#include <BALL/MOLMEC/MINIMIZATION/conjugateGradient.h>
#include <BALL/STRUCTURE/residueChecker.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/selector.h>
#include "global.h"
#include "files.h"
#include <fstream>

using namespace std;

class	SingleFile
{
	public:

	void moveTo(System& new_system);

	void moveBack();

	void readPDB(const String& file);
	void readHIN(const String& file);

	void write() const;
	
	System& getSystem(){return system;}

	
	protected:
	enum Type { PDB, HIN };

	list<Composite*>	composites;
	System						system;
	String						filename;
	Type							type;
};

void SingleFile::moveTo(System& new_system)
{
	Log.info() << "moving all " << system.countAtoms() << " of " << filename << " into new system" << endl;
	while (system.countDescendants() != 0)
	{
		Log.info() << "moving composite..." << endl;
		// save the address of the composite and insert it into the new system
		composites.push_back(system.getChild(0));
		new_system.Composite::prependChild(*system.getChild(0));
	}
}
	
void SingleFile::moveBack()
{
	while (composites.size() != 0)
	{
		// insert all composites back into the system
		system.prependChild(*composites.back());
		composites.pop_back();
	}
}
	

void SingleFile::readPDB(const String& file)
{
	type = PDB;
	filename = file;
	PDBFile pdb_file(filename);
	if (!pdb_file.good())
	{
		Log.error() << "cannot open " << filename << " for reading." << endl;
	} 
	else 
	{
		pdb_file >> system;
	}
	pdb_file.close();
}

void SingleFile::readHIN(const String& file)
{
	type = HIN;
	filename = file;
	HINFile hin_file(filename);
	if (!hin_file.good())
	{
		Log.error() << "cannot open " << filename << " for reading." << endl;
	} 
	else 
	{
		hin_file >> system;
	}
	hin_file.close();
}

void SingleFile::write() const
{
	if (type == HIN)
	{
		HINFile hin_file(filename + "_opt", ios::out);
		if (hin_file.bad())
		{
			Log.error() << "cannot open " << filename << " for writing." << endl;
		} 
		else 
		{
			hin_file << system;
		}
	}
	else 
	{
		PDBFile pdb_file(filename + "_opt", ios::out);
		if (pdb_file.bad())
		{
			Log.error() << "cannot open " << filename << " for writing." << endl;
		} 
		else 
		{
			pdb_file << system;
		}
		pdb_file.close();
	}
}

// create structures for the PDB files and the movable residues
vector<SingleFile>	PDB_files;
AmberFF amber;

void setup()
{
	// fragment DB
	if (frag_db != 0)
	{
		frag_db = new FragmentDB;
	}

	// create force field
	Log.info() << "setting up force field" << endl;

	if (verbose)
	{
		Log.info() << "force field parameters are read from " << FF_filename << endl;
	}
	S.deselect();
	amber.options[AmberFF::Option::FILENAME] = FF_filename;
	if (!amber.setup(S))
	{
		Log.error() << "Setup failed! Abort." << endl;	
		exit(10);
	}
}

void checkStructures()
{
	// fragment DB
	if (frag_db != 0)
	{
		frag_db = new FragmentDB;
	}

	// checking residues
	Log.info() << "checking system" << endl;
	ResidueChecker check(*frag_db);
	S.apply(check);

	Log.info() << "system contains " << S.countAtoms() << " atoms." << endl;
}

void singlePoint()
{
	double energy = amber.updateEnergy();
	Log.info() << "single point energy: " << amber.getEnergy() << " kJ/mol" << endl;
	Log.info() << "  - stretch      :" << amber.getStretchEnergy() << " kJ/mol" << endl;
	Log.info() << "  - bend         :" << amber.getBendEnergy() << " kJ/mol" << endl;
	Log.info() << "  - torsion      :" << amber.getTorsionEnergy() << " kJ/mol" << endl;
	Log.info() << "  - VdW          :" << amber.getVdWEnergy() << " kJ/mol" << endl;
	Log.info() << "  - electrostatic:" << amber.getESEnergy() << " kJ/mol" << endl;
}

void optimize()
{
	if (use_selection)
	{
		Log.info() << "SELECT: " << selection << endl;
		Selector selector(selection);
		S.apply(selector);
		// count selected vs. unselected atoms
		Size selected = 0;
		AtomIterator ai = S.beginAtom();
		for (; +ai; ++ai)
		{
			if (ai->isSelected()) selected++;
		}
		Log.info() << "selected " << selected << " out of " << S.countAtoms() << " atoms." << endl;
	}
	
	// minimize
	Log.info() << "starting minimization" << endl;
	ConjugateGradientMinimizer minimizer(amber);
	if (verbose)
	{
		minimizer.setEnergyOutputFrequency(1);
	} 
	else 
	{
		minimizer.setEnergyOutputFrequency(20);
	}
	minimizer.setEnergyDifferenceBound(1e-9);
	minimizer.setMaxSameEnergy(20);
	minimizer.setMaxGradient(max_gradient);

	Size max_restart = 15;
	while ((amber.getRMSGradient() > minimizer.getMaxGradient())
				 && (max_restart > 0))
	{
		minimizer.minimize();
		max_restart--;
	}
	Log.info() << "minimization complete" << endl;
	Log.info() << "final gradient: " << amber.getRMSGradient() << " kJ/mol A" << endl;
	
	S.deselect();
	amber.updateEnergy();
	Log.info() << "final energy: " << amber.getEnergy() << " kJ/mol" << endl;

	// dissolve the system again
	Log.info() << "writing PDB files" << endl;
	for (Size i = 0; i < PDB_files.size(); i++)
	{
		PDB_files[i].moveBack();
		PDB_files[i].write();
	}

	// dump the minimizer and force field options
	// for documentation purposes
	amber.options.dump(Log);
	minimizer.options.dump(Log);

	Log.info() << "done." << endl;
}

void addToSystem(System& system)
{
	// splice the system into the global system
	S.splice(system);
}

void readPDBFile(const String& filename)
{
	Log.info() << "reading PDB file " << filename << endl;
	PDB_files.push_back(SingleFile());
	PDB_files.back().readPDB(filename);
	
	// print the number of atoms read in verbose mod
	if (verbose)
	{
		Log.info() << "read " << S.countAtoms() << " atoms from " << filename << endl;
	}
	
	// normalize the names
	normalizeNames(PDB_files.back().getSystem());

	// build the bonds
	buildBonds(PDB_files.back().getSystem());

	// insert into the system
	PDB_files.back().moveTo(S);
}

void readSystemFromHINFile(const String& filename, System& system)
{
	Log.info() << "reading PDB file " << filename << endl;
	PDB_files.push_back(SingleFile());
	PDB_files.back().readHIN(filename);
	
	// print the number of atoms read in verbose mod
	if (verbose)
	{
		Log.info() << "read " << PDB_files.back().getSystem().countAtoms() << " atoms from " << filename << endl;
	}
	
	// normalize the names
	normalizeNames(PDB_files.back().getSystem());

	// build the bonds
	buildBonds(PDB_files.back().getSystem());

	// insert into the system
	PDB_files.back().moveTo(S);
}

void readHINFile(const String& filename)
{
	System system;
	readSystemFromHINFile(filename, system);
}

void readHINFileNoAssignment(const String& filename)
{
	System system;
	readSystemFromHINFile(filename, system);
	addToSystem(system);
}


void normalizeNames(System& system)
{
  if (normalize_names)
  {
    // create a fragment DB id required
    if (frag_db == 0)
    {
      frag_db = new FragmentDB;
		}

    // apply the normalize names processor
    system.apply(frag_db->normalize_names);
	}
}

void buildBonds(System& system)
{
  if (build_bonds)
  {
    // create a fragment DB id required
    if (frag_db == 0)
    {
      frag_db = new FragmentDB;
		}

    // apply the normalize names processor
    system.apply(frag_db->build_bonds);
	}
} 
