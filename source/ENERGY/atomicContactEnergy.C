// $Id: atomicContactEnergy.C,v 1.1 2000/09/05 14:29:15 oliver Exp $

#include <BALL/ENERGY/atomicContactEnergy.h>

#include <strstream>
#include <stdio.h>

#include <BALL/common.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/DATATYPE/hashGrid.h>
#include <BALL/DATATYPE/string.h>
#include <BALL/DATATYPE/stringHashMap.h>
#include <BALL/MATHS/vector3.h>
#include <BALL/STRUCTURE/geometricProperties.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/atomContainer.h>

#define ACE_FILENAME "energy/AtomicContactEnergy.dat"
#define ACE_TYPES_FILENAME "energy/ACE_types.dat"

// #define BALL_DEBUG_ACE

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
		String filename = path.find(ACE_TYPES_FILENAME);
		if (filename == "")
		{
			throw Exception::FileNotFound(__FILE__, __LINE__, ACE_TYPES_FILENAME);
		}

		datafile = fopen(filename.c_str(), "r");
		if (datafile == 0)
		{
			throw Exception::FileNotFound(__FILE__, __LINE__, ACE_TYPES_FILENAME);
		}
		

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



	typedef struct 
	{
		int					type;
		Vector3			v;
		unsigned		index;
		String			name;
		Atom*				atom;
	} ACEFastAtomType_;


	double calculateACE(AtomContainer& atoms) 
	{
		 
		Vector3										bounding_box_lower;	
		Vector3										bounding_box_upper;
		Vector3										size;

		StringHashMap<Atom::Type>	table;
		float											ACE[18][18];
		FILE*											ACE_file;
		int												i;
		int												j;

		table = buildTable_();

		Path path;
		String filename = path.find(ACE_FILENAME);
		if (filename == "")
		{
			throw Exception::FileNotFound(__FILE__, __LINE__, ACE_FILENAME);
		}
		ACE_file = fopen(filename.c_str(), "r");
					
		for (i = 0; i < 18; i++)
		{
			for (j = 0; j < 18; j++)
			{
				fscanf(ACE_file, "%f", &(ACE[i][j]));
			}
		}
		fclose(ACE_file);
		
		// perform consistency check: matrix has to be symmetric
		for (i = 0; i < 18; i++)
		{
			for (j = i; j < 18; j++)
			{
				if (ACE[i][j] != ACE[j][i])
				{
					Log.error() << "calculateACE: inconsistent values in contact matrix: ACE[" 
											<< i << "][" << j << "] != ACE[" << j << "][" << i << "]!" << endl;
				}
			}
		}
			
		BoundingBoxProcessor	bounding_box;

		atoms.apply(bounding_box);
		bounding_box_lower = bounding_box.getLower();
		bounding_box_upper = bounding_box.getUpper();
		
				
		size = bounding_box_upper - bounding_box_lower;

		HashGrid3<ACEFastAtomType_*>*	grid;
		grid = new HashGrid3<ACEFastAtomType_*>(bounding_box_lower - Vector3(7.0), size + Vector3(14.0), 6.0);

		ACEFastAtomType_*		atom;
		String							atom_name;
		int									atom_type;
		unsigned						index = 0;


		AtomIterator	atom_iterator;
		for (atom_iterator = atoms.beginAtom(); +atom_iterator; ++atom_iterator) 
		{
			// construct correct name, <RESNAME>:<ATOMNAME>
			atom_name = atom_iterator->getFragment()->getName().trim();		
			atom_name.append(":");
			atom_name.append((*atom_iterator).getName().trim());

			// get the atom type from hash table
			//    atom type = -1 for hydrogen (not to be considered)
			//    atom type = -2 for atoms that are not parametrized
			//    atom type = -3 for unknown atom names
			//
			if (table.has(atom_name))
			{
				atom_type = table[atom_name];
			} else {
				atom_type = -3;
			}

			// insert all neccessary atoms into the grid
			if (atom_type >= 0)
			{
				atom = new ACEFastAtomType_;
				atom->index = index++;
				atom->v = atom_iterator->getPosition();
				atom->type = atom_type;
				atom->name = atom_name;
				atom->atom = &(*atom_iterator);
				grid->insert(atom->v, atom);
				// Log.error() << atom_name << ": " << atom_type << endl;
			} else {
				// some types are known but not parametrized
				if (atom_type == -2)
				{
					Log.warn() << "calculateACE: ACE not parametrized for " << atom_name << endl;
				} 
				// hydrogens are just to be ignored, unknown types are a 
				// real reason for concern
				else if (atom_type == -3) 
				{
					Log.warn() << "calculateACE: did not find an ACE type for " << atom_name << endl;
				}
			}		
		}

		float			contact_energy = 0;
		float			sq_distance;
		Vector3		v;

		HashGrid3<ACEFastAtomType_*>::BoxIterator					box_iterator;
		HashGridBox3<ACEFastAtomType_*>::DataIterator			data_iterator;

		HashGridBox3<ACEFastAtomType_*>::BoxIterator			box_iterator2;
		HashGridBox3<ACEFastAtomType_*>::DataIterator			data_iterator2;

#ifdef BALL_DEBUG_ACE
		Size count = 0;
		AtomIterator debug_atom_it1 = atoms.beginAtom();
		AtomIterator debug_atom_it2;
		for (; +debug_atom_it1; ++debug_atom_it1)
		{
			for (debug_atom_it2 = debug_atom_it1; +debug_atom_it2; ++debug_atom_it2)
			{
				if ((debug_atom_it1 != debug_atom_it2)
						&& (debug_atom_it1->getPosition().getDistance(debug_atom_it2->getPosition()) <= 6.0))
				{
					count++;
				}
			}
		}
		Log.error() << "count(BF) = " << count << endl;
		count = 0;
#endif
		

		// iterate over all (non empty) grid boxes
		for (box_iterator = grid->beginBox(); +box_iterator; ++box_iterator)
		{
			// iterate over all items in the box
			for (data_iterator = (*box_iterator).beginData(); +data_iterator; ++data_iterator) 
			{
				v = (*data_iterator)->v;

				// iterate over all neighbouring boxes (includig the box itself!)
				for (box_iterator2 = (*box_iterator).beginBox(); +box_iterator2; ++box_iterator2)
				{
					// iterate over all items in the box
					for (data_iterator2 = (*box_iterator2).beginData(); +data_iterator2; ++data_iterator2) 
					{
						// consider only pairs of different atoms and consider each pair only once 
						if ((*data_iterator2)->index > (*data_iterator)->index)
						{					
							// check whether the distance between the atoms is at most 6.0 Angstrom
							sq_distance = v.getSquareDistance((*data_iterator2)->v);
							if (sq_distance <= 36.0)
							{
								if (!(*data_iterator)->atom->isBoundTo(*(*data_iterator2)->atom))
								{
									contact_energy += ACE[(*data_iterator)->type][(*data_iterator2)->type];
#ifdef BALL_DEBUG_ACE
									count++;
#endif
								}
							}
						}
					}
				}
			}
		}

#ifdef BALL_DEBUG_ACE
		Log.error() << "count(BF) = " << count << endl;
#endif

		// scale by a factor of 1 / 21.0 to get energy in kcal/mol
		// and another factor of 4.184 to get kJ/mol
		// return the energy in units of kJ/mol
		return contact_energy / (21.0 / 4.184);
	}

} // namespace BALL
