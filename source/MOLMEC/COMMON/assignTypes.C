// $Id: assignTypes.C,v 1.2 1999/09/03 14:10:52 oliver Exp $

#include <BALL/MOLMEC/COMMON/assignTypes.h>
#include <iostream.h>

namespace BALL 
{

	AssignTypeNameProcessor::AssignTypeNameProcessor
		(const String& filename, bool overwrite)
		:	overwrite_(overwrite)
	{
		ifstream	type_file(filename.c_str());

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
			Fragment* frag = atom.getFragment();
			if (frag != 0)
			{
				name = frag->getName().trim();
			}

			name += ":";
			name += atom.getName().trim();
			
			if (!type_map_.has(name))
			{
				if (atom.getTypeName() == BALL_ATOM_DEFAULT_TYPE_NAME)
				{
					Log.level(LogStream::WARNING) << "Could not assign atom type name for " << name << endl;
				}
			} else {
				atom.setTypeName(type_map_[name]);
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
