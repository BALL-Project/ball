// $Id: ooiEnergy.C,v 1.2 2000/02/12 19:31:16 oliver Exp $

#include <BALL/SOLVATION/ooiEnergy.h>


#include <BALL/common.h>
#include <BALL/DATATYPE/hashGrid.h>
#include <BALL/DATATYPE/string.h>
#include <BALL/DATATYPE/stringHashMap.h>
#include <BALL/STRUCTURE/geometricProperties.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/STRUCTURE/numericalSAS.h>
#include <BALL/MOLMEC/PARAMETER/forceFieldParameters.h>
#include <BALL/MOLMEC/PARAMETER/FFParameterSection.h>

#define OOI_PARAMETER_FILENAME "solvation/Ooi.ini"

// #define BALL_DEBUG_OOI

using namespace std;

namespace BALL 
{

	namespace OoiEnergy
	{
		bool is_initialized = false;

		// a hash map to convert names to Ooi types
		StringHashMap<Atom::Type> type_map;

		// free energy is calculated as
		// dG = \sum_i g_i A_i
		// where A_i is the atomic solvent accesible surface
		// area. The atomic radii are taken from the vector radius below.
		vector<float> radius;
		vector<float> g;

		// read the parameter files for calcualteOoiEnergy 
		// and set up the basic data structures
		void init()
		{
			// extract the parameters from the file
			Path path;
			String filename = path.find(OOI_PARAMETER_FILENAME);
			if (filename == "")
			{
				filename = OOI_PARAMETER_FILENAME;
			}
			ForceFieldParameters parameters(filename);

			FFParameterSection parameter_section;
			if (!parameter_section.extractSection(parameters, "OoiParameters"))
			{
				Log.error() << "calculateOoiEnergy: cannot find section [OoiParameters] in file "
					<< parameters.getFilename() << "." << endl;
				return;
			}
			if (!parameter_section.hasVariable("g") || !parameter_section.hasVariable("radius"))
			{
				Log.error() << "OoiEnergy: section [OoiTypes] of file " 
					<< parameters.getFilename() << " requires at least the columns 'g' and 'radius'." << endl;
				return;
			}							

			FFParameterSection type_section;
			if (!type_section.extractSection(parameters, "OoiTypes"))
			{
				Log.error() << "calculateOoiEnergy: cannot find section [OoiTypes] in file "
					<< parameters.getFilename() << "." << endl;
				return;
			}
			if (!type_section.hasVariable("type"))
			{
				Log.error() << "OoiEnergy: section [OoiTypes] of file " 
					<< parameters.getFilename() << " does not contain a variable column 'type'." << endl;
				return;
			}			

			// extract the parameters for each type
			// 
			Position radius_column = parameter_section.getColumnIndex("radius");
			Position g_column = parameter_section.getColumnIndex("g");
			Index max_index = -1;
			Size i;
			for (i = 1; i <= parameter_section.getNumberOfKeys(); i++)
			{
				Index index = parameter_section.getKey(i).toInt();
				if (index < 0)
				{
					Log.error() << "calculateOoiEnergy: illegal atom type index: " << index << endl;
				} else {
					if (index > max_index)
					{
						max_index = index;
					}
				}
			}

			if (max_index < 0)
			{
				Log.error() << "calculateOoiEnergy: could not find any atom type in file "
									  << parameters.getFilename() << endl;
				return;
			}
			
			
			// resize the vectors to hold all indices
			radius.resize((Size)max_index + 1);
			g.resize((Size)max_index + 1);

			// and read all values from the parameter section
			for (i = 1; i <= parameter_section.getNumberOfKeys(); i++)
			{
				Position index = parameter_section.getKey(i).toInt();

				// we ignore illegal (negative) indices
				if (index >= 0)
				{
					radius[index] = parameter_section.getValue(i, radius_column).toFloat();
					g[index] = parameter_section.getValue(i, g_column).toFloat();
					Log.info() << "read parameter for type " << index << " " << g[index]
											<< " " << radius[index] << endl;
				}
			}

			// extract all known types by iterating over all keys
		  // and construct the hash map type_map
			Position type_column = type_section.getColumnIndex("type");
			for (i = 1; i <= type_section.getNumberOfKeys(); i++)
			{
				// retrieve the type and check for validity
				Atom::Type type = type_section.getValue(i, type_column).toInt();
				if (type >= (Atom::Type)radius.size())
				{
					Log.error() << "calculateOoiEnergy: illegal atom type: " << type << " while reading parameter file." << endl;
				} else {
					type_map.insert(type_section.getKey(i), (Atom::Type)type_section.getValue(i, type_column).toInt());
				}
			}
			
			// we're done with the initialization
			is_initialized = true;
		}
	}

	double calculateOoiEnergy(Composite& composite) 
	{
		using namespace OoiEnergy;

		// read and interpret the parameters 
		// this is only done the first time calculateOoiEnergy is called
		if (!is_initialized)
		{
			init();
			if (!is_initialized)
			{
				return 0;
			}
		}
		
		// assign radii and atom types for all atoms
		Composite::SubcompositeIterator composite_it;
		for (composite_it = composite.beginSubcomposite(); +composite_it; ++composite_it) 
		{
			if (RTTI::isKindOf<Atom>(*composite_it))
			{
				Atom& atom = dynamic_cast<Atom&>(*composite_it);

				// construct correct name, <RESNAME>:<ATOMNAME>
				String atom_name = atom.getFullName();
				
				// get the atom type from hash table
				// first, try a direct match
				Atom::Type atom_type = -1;
				if (type_map.has(atom_name))
				{
					atom_type = type_map[atom_name];
				} else {
					atom_name = atom.getFullName(Atom::NO_VARIANT_EXTENSIONS);
					if (type_map.has(atom_name))
					{
						atom_type = type_map[atom_name];
					} else {
						// try wildcard match
						atom_name = "*:" + atom.getName();
						if (type_map.has(atom_name))
						{
							atom_type = type_map[atom_name];
						}
					}
				}

				// if the atom type could not be determined, complain
				if (atom_type < 0)
				{
					Log.warn() << "calculateOOIEnergy: did not find an OOI type for " << atom_name << endl;
					// ignore this atom....
					atom.setType(-1);
					atom.setRadius(0.0);
				} else {
					// assign type and radius
					atom.setType(atom_type);
					atom.setRadius(radius[atom_type]);
				}
			}		
		}

		// calculate the atomic SAS areas
		// atom_SAS_areas hashes the atom pointer to the
		// surface area (in Angstrom^2)
		HashMap<Atom*,float> atom_SAS_areas;
		calculateNumericalSASAtomAreas(atom_SAS_areas, composite, 1.4, 1888);

		// iterate over all atoms and add up the energies
		float energy = 0.0;
		for (composite_it = composite.beginSubcomposite(); +composite_it; ++composite_it) 
		{
			if (RTTI::isKindOf<Atom>(*composite_it))
			{
				Atom& atom = dynamic_cast<Atom&>(*composite_it);
			
				if (atom_SAS_areas.has(&atom))
				{
					Atom::Type atom_type = atom.getType();
					// if the atom type could not be determined, complain
					if (atom_type >= 0)
					{
						// add the energy contribution of the atom
						float tmp = atom_SAS_areas[&atom] * g[atom_type];
						Log.info() << atom.getFullName() << " " << atom_SAS_areas[&atom] << " " << tmp << " kJ/mol" << endl;
						energy += tmp;
					}
				}		
			}
		}

		// we're done.
		return energy;
	}

} // namespace BALL
