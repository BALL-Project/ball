// $Id: atomTypes.C,v 1.2 1999/08/31 22:01:17 oliver Exp $
//

#include <BALL/MOLMEC/PARAMETER/atomTypes.h>
#include <BALL/MOLMEC/PARAMETER/forceFieldParameters.h>

namespace BALL 
{


	FFPSAtomTypes::FFPSAtomTypes(void)
		: FFParameterSection()
	{
	}


	FFPSAtomTypes::FFPSAtomTypes(const FFPSAtomTypes& atom_types, bool /* deep */)
		: FFParameterSection(atom_types)
	{
		names_ = atom_types.names_;
		type_map_ = atom_types.type_map_;
	}


	void FFPSAtomTypes::destroy(void)
	{
		names_.clear();
		type_map_.destroy();

		FFParameterSection::destroy();
	}

		

	FFPSAtomTypes::~FFPSAtomTypes(void)
	{
		destroy();
	}

	FFPSAtomTypes& FFPSAtomTypes::operator = (const FFPSAtomTypes& atom_types)
	{
		destroy();

		names_ = atom_types.names_;
		type_map_.set(atom_types.type_map_);

		return *this;
	}

	bool FFPSAtomTypes::extractSection
		(ForceFieldParameters& parameters, 
		 const String& section_name)
	{
		// the atom types have to be valid, as this is checked by extractSection
		// (ugly circular dependence, but works OK)
		valid_ = true;

		// extract the basis information
		if (!FFParameterSection::extractSection(parameters, section_name))
		{
			Log.level(LogStream::ERROR) << "Didn't find section for " << section_name << endl;
			return false;
		}

		type_map_.destroy();

		names_.resize(getNumberOfKeys() + 1);

		StringHashMap<Index>::iterator	it;

		names_[0] = BALL_ATOM_UNKNOWN_NAME;
		type_map_[BALL_ATOM_WILDCARD_NAME] = Atom::ANY_TYPE;
		type_map_[BALL_ATOM_UNKNOWN_NAME] = Atom::ANY_TYPE;

		Size	index = 1;
		for (it = section_entries_.begin(); !(it == section_entries_.end()); ++it)
		{
			names_[index] = it->first;
			type_map_[it->first] = (Atom::Type)(index++);
		}
		
		return true;
	}

	bool FFPSAtomTypes::hasType(const String& name) const 
	{
		return type_map_.has(name);
	}

	Atom::Type FFPSAtomTypes::getType(const String& name) const 
	{
		if (type_map_.has(name)) 
		{
			return (*type_map_.find(name)).second;
		} else {
			return Atom::UNKNOWN_TYPE;
		}
	}

	String FFPSAtomTypes::getTypeName(Atom::Type type) const 
	{
		static const String empty_string;
		if ((type < Atom::ANY_TYPE) || (type > (Index)type_map_.size()))
		{
			return empty_string;
		}
		
		if (type == Atom::ANY_TYPE)
		{
			return BALL_ATOM_WILDCARD_NAME;
		}
		else 
		{
			return names_[type];
		}
	}

	Size FFPSAtomTypes::getNumberOfTypes(void) const 
	{
		return type_map_.size();
	}

} // namespace BALL
