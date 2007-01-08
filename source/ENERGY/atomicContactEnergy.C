// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: atomicContactEnergy.C,v 1.6 2003/08/26 09:17:45 oliver Exp $
//

#include <BALL/ENERGY/atomicContactEnergy.h>

#include <BALL/common.h>
#include <BALL/SYSTEM/file.h>
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
	typedef StringHashMap<Atom::Type>								ACETypeTable;
	typedef std::vector<std::vector<double> >				ACEParameter;
	typedef std::pair<ACETypeTable, ACEParameter>		ACEData;

	 const ACEData& ACEBuildTable
		(const String& type_filename, const String& parameter_filename)
	{
		// A static string hash map holds the types.
		static ACEData data;
		static String last_type_filename;
		static String last_parameter_filename;

		// Check whether the last files read are identical with the
		// filenames specified. We are fine if they are identical
		// and reread everything otherwise.
		if (last_type_filename == type_filename && last_parameter_filename == parameter_filename)
		{
			return data;
		}

		// Determine the path to the type file.
		Path path;
		String filename = path.find(type_filename);
		if (filename == "")
		{
			throw Exception::FileNotFound(__FILE__, __LINE__, type_filename);
		}

		// Read the file.
		data.first.clear();
		File datafile(filename);
		String key;
		String atom_name;
		Index atom_type;
		while (datafile.good())
		{
			datafile >> key >> atom_name >> atom_type;
			key.append(":");
			key.append(atom_name);
			data.first[key] = atom_type;
		}			
		datafile.close();

		filename = path.find(parameter_filename);
		if (filename == "")
		{
			throw Exception::FileNotFound(__FILE__, __LINE__, filename);
		}

		// Read the parameter table.
		datafile.open(filename);
		Size number_of_entries;
		datafile >> number_of_entries;
		
		// Read the matrix of ACE parameters (symmetric, number_of_entries x number_of_entries)
		data.second.resize(number_of_entries);
		for (Position i = 0; i < number_of_entries; i++)
		{
			data.second[i].resize(number_of_entries);

			for (Position j = 0; j < number_of_entries; j++)
			{
				datafile >> data.second[i][j];
			}
		}

		// Consistency check in the matrix
		for (Position i = 0; i < number_of_entries; i++)
		{
			for (Position j = 0; j < number_of_entries; j++)
			{
				if (data.second[i][j] != data.second[j][i])
				{
					throw Exception::ParseError(__FILE__, __LINE__, 
																			String("ACE data from ") + filename,
																			String("inconsistent table entry at position ") + String(i) + "/" + String(j));
				}
			}
		}
		
				
		// Remember we read the files
		last_type_filename = type_filename;
		last_parameter_filename = parameter_filename;

		return data;
	}

	typedef struct 
	{
		Index				type;
		Vector3			v;
		Position		index;
		String			name;
		const Atom*	atom;
	} 
	ACEFastAtomType;


	double calculateACE
		(const AtomContainer& atom_container, 
		 const string& type_filename, const string& parameter_filename) 
	{
		// Retrieve the ACE types and parameters.
		// We do this just once, unless different filenames are 
		// being specified.
		const ACEData& data = ACEBuildTable(type_filename, parameter_filename);
		const ACETypeTable& table = data.first;
		const ACEParameter& params = data.second;

		// Compute the bounding box of the atom container.
		BoundingBoxProcessor bounding_box;
		const_cast<AtomContainer&>(atom_container).apply(bounding_box);
		Vector3 size = bounding_box.getUpper() - bounding_box.getLower();

		// Create a three-dimensional hash grid. The border has to
		// extend by at least one box size (the max distance used by ACE) over the bounding box.
		HashGrid3<ACEFastAtomType*> grid(bounding_box.getLower() - Vector3(7.0), size + Vector3(14.0), 6.0);

		// Create a vector of ACEFastAtom entries. We will stick pointers 
		// to those into the hash grid. It will never shrink again, but then 
		// we won't have to reallocate all the time. This seems like a good 
		// trade-off for now.
		static std::vector<ACEFastAtomType> atoms;

		// Remove old stuff
		atoms.clear();

		String atom_name;
		Index atom_type;
		AtomConstIterator atom_it(atom_container.beginAtom());
		for (; +atom_it; ++atom_it) 
		{
			// construct correct name, <RESNAME>:<ATOMNAME>
			atom_name = atom_it->getFullName(Atom::NO_VARIANT_EXTENSIONS);		

			// get the atom type from hash table
			//    atom type = -1 for hydrogen (not to be considered)
			//    atom type = -2 for atoms that are not parametrized
			//    atom type = -3 for unknown atom names
			//
			if (table.has(atom_name))
			{
				atom_type = table[atom_name];
			} 
			else 
			{
				atom_type = -3;
			}

			// insert all neccessary atoms into the grid
			if (atom_type >= 0)
			{
				static ACEFastAtomType atom;
				atom.v = atom_it->getPosition();
				atom.type = atom_type;
				atom.name = atom_name;
				atom.index = atoms.size();
				atom.atom = &*atom_it;

				atoms.push_back(atom);
			} 
			else 
			{
				// some types are known but not parametrized
				if (atom_type == -2)
				{
					Log.warn() << "calculateACE: ACE not parametrized for " << atom_name << endl;
				} 
				else if (atom_type == -3) 
				{
					// hydrogens are just to be ignored, unknown types are a 
					// real reason for concern
					Log.warn() << "calculateACE: did not find an ACE type for " << atom_name << endl;
				}
			}		
		}

		std::vector<ACEFastAtomType>::iterator it(atoms.begin());
		for (; it != atoms.end(); ++it)
		{
			grid.insert(it->v, &*it);
		}

		double contact_energy = 0;
		double sq_distance;
		Vector3 v;

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
		HashGrid3<ACEFastAtomType*>::BoxIterator box_it(grid.beginBox());
		for (; +box_it; ++box_it)
		{
			HashGridBox3<ACEFastAtomType*>::DataIterator			data_it;
			HashGridBox3<ACEFastAtomType*>::BoxIterator				box_it2;
			HashGridBox3<ACEFastAtomType*>::DataIterator			data_it2;

			// iterate over all items in the box
			for (data_it = box_it->beginData(); +data_it; ++data_it) 
			{
				v = (*data_it)->v;

				// iterate over all neighbouring boxes (includig the box itself!)
				for (box_it2 = box_it->beginBox(); +box_it2; ++box_it2)
				{
					// iterate over all items in the box
					for (data_it2 = box_it2->beginData(); +data_it2; ++data_it2) 
					{
						// consider only pairs of different atoms and consider each pair only once 
						if ((*data_it2)->index > (*data_it)->index)
						{					
							// check whether the distance between the atoms is at most 6.0 Angstrom
							sq_distance = v.getSquareDistance((*data_it2)->v);
							if (sq_distance <= 36.0)
							{
								if (!(*data_it)->atom->isBoundTo(*(*data_it2)->atom))
								{
									contact_energy += params[(*data_it)->type][(*data_it2)->type];
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
