// $Id: assignTypes.C,v 1.8 1999/12/30 18:05:35 oliver Exp $

#include <BALL/MOLMEC/COMMON/assignTypes.h>

#include <iostream>

using namespace std;

namespace BALL 
{

	AssignTypeNameProcessor::AssignTypeNameProcessor
		(const String& filename, bool overwrite)
		:	overwrite_(overwrite)
	{
		Path path;
		String filepath = path.find(filename);
		if (filepath == "")
		{
			throw Exception::FileNotFound(__FILE__, __LINE__, filename);
		}
		ifstream	type_file(filepath.c_str());

		String line;
		String type;
		String name;

		while (type_file)
		{
			line.getline(type_file);
			
			name = line.getField(0);
			type = line.getField(1);
			
			type_map_[name] = type;
		}

		type_file.close();
	}

	Processor::Result	AssignTypeNameProcessor::operator () (Atom& atom)
	{
		if (overwrite_ || atom.getTypeName() == BALL_ATOM_DEFAULT_TYPE_NAME)
		{
			
			String name;
			String base_name;
			Fragment* frag = atom.getFragment();
			if (frag != 0)
			{
				name = frag->getName().trim();
				base_name = name;
				if (RTTI::isKindOf<Residue>(*frag))
				{
					String suffix = "-";
					Residue*	res = RTTI::castTo<Residue>(*frag);
					if (res->isNTerminal())
					{
						suffix += "N";
					} else if (res->isCTerminal())
					{
						suffix += "C";
					}
					if (res->hasProperty(Residue::PROPERTY__HAS_SSBOND))
					{
						suffix += "S";
					}
					
					if (suffix != "-")
					{
						name += suffix;
					}
				}
			}

			name += ":";
			base_name += ":";
			name += atom.getName().trim();
			base_name += atom.getName().trim();
			
			if (type_map_.has(name))
			{
				atom.setTypeName(type_map_[name]);
			} else if (type_map_.has(base_name)) {
				atom.setTypeName(type_map_[base_name]);				
			} else {
				if (atom.getTypeName() == BALL_ATOM_DEFAULT_TYPE_NAME)
				{
					Log.warn() << "Could not assign atom type name for " << name << " (could not match "
										 << name << " or " << base_name << ")" << endl;
				}
			}
		}

		return Processor::CONTINUE;
	}

	AssignTypeProcessor::AssignTypeProcessor
		(const FFPSAtomTypes& atom_types)
	{
		atom_types_ = atom_types;
	}

	Processor::Result	AssignTypeProcessor::operator () (Atom& atom)
	{
		Atom::Type type = atom_types_.getType(atom.getTypeName());
		if (type == Atom::UNKNOWN_TYPE)
		{
			Log.level(LogStream::WARNING) << "Could not assign atom type for " << atom.getTypeName() << endl;
		} else {
			atom.setType(type);
		}

		return Processor::CONTINUE;
	}

} // namespace BALL
