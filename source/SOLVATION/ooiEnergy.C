// $Id: ooiEnergy.C,v 1.9 2000/07/23 22:16:05 oliver Exp $

#include <BALL/SOLVATION/ooiEnergy.h>


#include <BALL/common.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/COMMON/exception.h>
#include <BALL/DATATYPE/hashGrid.h>
#include <BALL/DATATYPE/string.h>
#include <BALL/DATATYPE/stringHashMap.h>
#include <BALL/STRUCTURE/geometricProperties.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/STRUCTURE/numericalSAS.h>
#include <BALL/FORMAT/parameters.h>
#include <BALL/FORMAT/parameterSection.h>
#include <BALL/MOLMEC/COMMON/typeRuleProcessor.h>

#define OOI_PARAMETER_FILENAME "solvation/Ooi.ini"

//#define BALL_DEBUG_OOI

using namespace std;

namespace BALL 
{

	namespace OoiEnergy
	{
		bool is_initialized = false;

		// a hash map to convert names to Ooi types
		StringHashMap<Atom::Type> type_map;

		// a type rule assignment processor
		TypeRuleProcessor type_rule;

		// free energy is calculated as
		// dG = \sum_i g_i A_i
		// where A_i is the atomic solvent accesible surface
		// area. The atomic radii are taken from the vector radius below.
		vector<float> radius;
		vector<float> g;

		// read the parameter files for calculateOoiEnergy 
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
			Parameters parameters(filename);

			ParameterSection parameter_section;
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

			ParameterSection type_section;
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
				
				String index_str(parameter_section.getKey(i));
				Index index;
				try
				{
					index = index_str.trim().toInt();
					if (index < 0)
					{
						Log.error() << "calculateOoiEnergy: illegal atom type index: " << index << endl;
					} 
					else 
					{
						if (index > max_index)
						{
							max_index = index;
						}
					}
				} 
				catch (Exception::InvalidFormat e)
				{
					Log.error() << "calculateOoiEnergy: cannot convert to a number: " << index_str << endl;
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
				String index_str(parameter_section.getKey(i));
				Index index;
				try
				{
					index = index_str.trim().toInt();

					// we ignore illegal (negative) indices
					if (index >= 0)
					{
						radius[index] = parameter_section.getValue(i, radius_column).toFloat();
						g[index] = parameter_section.getValue(i, g_column).toFloat();
					}
				}
				catch (Exception::InvalidFormat)
				{
					Log.error() << "calculateOoiEnergy: cannot convert to a number: " << index_str << endl;
				}
			}

			// extract all known types by iterating over all keys
		  // and construct the hash map type_map
			Position type_column = type_section.getColumnIndex("type");
			for (i = 1; i <= type_section.getNumberOfKeys(); i++)
			{
				// retrieve the type and check for validity
				String index_str(type_section.getValue(i, type_column));
				Atom::Type type = index_str.trim().toInt();
				if (type >= (Atom::Type)radius.size())
				{
					Log.error() << "calculateOoiEnergy: illegal atom type: " << type << " while reading parameter file." << endl;
				} 
				else 
				{
					index_str = type_section.getValue(i, type_column);
					type_map.insert(type_section.getKey(i), (Atom::Type)index_str.trim().toInt());
				}
			}
			
			// set up the type rule processor
			// from the rules in the INI file
			type_rule.initialize(parameters.getParameterFile(), "TypeRules");

			// we're done with the initialization
			is_initialized = true;
		}
	}

	double calculateOoiEnergy(BaseFragment& fragment) 
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
		AtomIterator atom_it = fragment.beginAtom();
		for (; +atom_it; ++atom_it) 
		{
			// construct correct name, <RESNAME>:<ATOMNAME>
			String atom_name = atom_it->getFullName();
			
			// get the atom type from hash table
			// first, try a direct match
			Atom::Type atom_type = -1;
			if (type_map.has(atom_name))
			{
				atom_type = type_map[atom_name];
			} 
			else 
			{
				atom_name = atom_it->getFullName(Atom::NO_VARIANT_EXTENSIONS);
				if (type_map.has(atom_name))
				{
					atom_type = type_map[atom_name];
				} 
				else 
				{
					// try wildcard match
					atom_name = "*:" + atom_it->getName();
					if (type_map.has(atom_name))
					{
						atom_type = type_map[atom_name];
					}
				}
			}

			// if the atom type could not be determined, complain
			if (atom_type < 0)
			{
				// try to apply the type rule processor
				atom_it->setType(-1);
				type_rule(*atom_it);
				atom_type = atom_it->getType();
			}

			if (atom_type < 0)
			{
				Log.warn() << "calculateOOIEnergy: did not find a suitable type for " << atom_it->getFullName() << endl;

				// ignore this atom....
				atom_it->setType(-1);
				atom_it->setRadius(0.0);
			} 
			else 
			{
				// assign type and radius
				atom_it->setType(atom_type);
				atom_it->setRadius(radius[atom_type]);
			}
		}

		// calculate the atomic SAS areas
		// atom_SAS_areas hashes the atom pointer to the
		// surface area (in Angstrom^2)
		HashMap<Atom*,float> atom_SAS_areas;
		calculateSASAtomAreas(fragment, atom_SAS_areas, 1.4, 1888);

		// iterate over all atoms and add up the energies
		float energy = 0.0;
		for (atom_it = fragment.beginAtom(); +atom_it; ++atom_it) 
		{
			
			if (atom_SAS_areas.has(&*atom_it))
			{
				Atom::Type atom_type = atom_it->getType();
				if (atom_type >= 0)
				{
					// add the energy contribution of the atom
					float tmp = atom_SAS_areas[&*atom_it] * g[atom_type];
					energy += tmp;

					#ifdef BALL_DEBUG_OOI
						Log.info() << atom_it->getFullName() << " A = " << atom_SAS_areas[&*atom_it] 
    									 << " E = " << tmp << " kJ/mol" << endl;
					#endif
				}
			}		
		}

		// we're done.
		return energy;
	}

} // namespace BALL
