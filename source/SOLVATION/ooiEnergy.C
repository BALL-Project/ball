// $Id: ooiEnergy.C,v 1.1 2000/02/12 11:17:06 oliver Exp $

#include <BALL/SOLVATION/ooiEnergy.h>


#include <strstream>
#include <stdio.h>

#include <BALL/common.h>
#include <BALL/DATATYPE/hashGrid.h>
#include <BALL/DATATYPE/string.h>
#include <BALL/DATATYPE/stringHashMap.h>
#include <BALL/DATATYPE/hashMap.h>
#include <BALL/MATHS/vector3.h>
#include <BALL/STRUCTURE/geometricProperties.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/STRUCTURE/numericalSAS.h>

#define OOI_PARAMETER_FILENAME "solvation/Ooi.ini"

// #define BALL_DEBUG_OOI

using namespace std;

namespace BALL 
{

	StringHashMap<Atom::Type>& buildTable_()
	{
		FILE*				datafile;
		char				residue[10];
		char				atom_name[10];
		Atom::Type*	atom_type;
		String*			key;

		// determine the path to the data file
		Path path;
		String filename = path.find(OOI_TYPES_FILENAME);
		if (filename == "")
		{
			throw Exception::FileNotFound(__FILE__, __LINE__, OOI_TYPES_FILENAME);
		}

		datafile = fopen(filename.c_str(), "r");


		// create a new StringHashMap
		StringHashMap<Atom::Type>* table = new StringHashMap<Atom::Type>;

		while (!feof(datafile))
		{
			atom_type = new Atom::Type;
			fscanf(datafile, "%s %s %hd", &(residue[0]), &(atom_name[0]), atom_type);
			key = new String(residue);
			key->append(":");
			key->append(atom_name);
			(*table)[*key] = (Atom::Type)*atom_type;
		}
					
		fclose(datafile);

		return *table;
	}


	/**	Calculate the solvation energy after
			Ooi et al. (Proc. Natl. Acad. Sci. USA)
			The energy is returned in units of kJ/mol
			Unparametrized Atoms are simply ignored, this leads to
			unreasonable values.\\

			{\bf Definition:} \URL{BALL/SOLVATION/ooiEnergy.h>
			\\
			{\bf Files:} solvation/Ooi.ini\\
			Types are taken from data/solvation/OOITypes.dat\\
			Energies are taken from data/solvation/OOIEnergies.dat\\
			values for contact energies are in data/energy/AtomicContactEnergy.dat
	*/
	double calculateOOIEnergy(System& system) 
	{
		 
		StringHashMap<Atom::Type>	table;
		float                           energy = 0;
		float                           OOI[8];
		FILE*                           OOI_file;
		int                             i;
		Atom::Type                      atom_type;
		String                          atom_name;

		table = buildTable_();

		Path path;
		String filename = path.find(OOI_ENERGIES_FILENAME);
		if (filename == "")
		{
			throw Exception::FileNotFound(__FILE__, __LINE__, OOI_ENERGIES_FILENAME);
		}
		OOI_file = fopen(filename.c_str(), "r");
					
		for (i = 0; i < 8; i++)
		{
				fscanf(OOI_file, "%f", &(OOI[i]));
		}
		fclose(OOI_file);
		

		HashMap<Atom*,float> aareas;

		calculateNumericalSASAtomAreas(aareas, system, 1.5, 400);


		float atom_area;
		AtomIterator	atom_iterator;
		for (atom_iterator = system.beginAtom(); +atom_iterator; ++atom_iterator) 
		{

			if (aareas.has(&(*atom_iterator)))
			{
				atom_area = aareas[&(*atom_iterator)];

				// construct correct name, <RESNAME>:<ATOMNAME>
				atom_name = atom_iterator->getFragment()->getName().trim();		
				atom_name.append(":");
				atom_name.append((*atom_iterator).getName().trim());

				// get the atom type from hash table
				if (table.has(atom_name))
				{
					atom_type = table[atom_name];
					// add the energy contribution of the atom
					energy += atom_area * OOI[atom_type];
				} else {
					Log.warn() << "calculateOOIEnergy: did not find an OOI type for " << atom_name << endl;
				}
			}		
		}

		return energy;
	}

} // namespace BALL
