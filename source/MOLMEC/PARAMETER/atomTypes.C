// $Id: atomTypes.C,v 1.6 2000/02/14 22:42:45 oliver Exp $
//

#include <BALL/MOLMEC/PARAMETER/atomTypes.h>
#include <BALL/MOLMEC/PARAMETER/forceFieldParameters.h>

using namespace std;

namespace BALL 
{


	AtomTypes::AtomTypes()
		: ParameterSection()
	{
	}


	AtomTypes::AtomTypes(const AtomTypes& atom_types, bool /* deep */)
		: ParameterSection(atom_types)
	{
		names_ = atom_types.names_;
		type_map_ = atom_types.type_map_;
	}


	void AtomTypes::destroy()
	{
		names_.clear();
		type_map_.destroy();

		ParameterSection::destroy();
	}

		

	AtomTypes::~AtomTypes()
	{
		destroy();
	}

	AtomTypes& AtomTypes::operator = (const AtomTypes& atom_types)
	{
		destroy();

		names_ = atom_types.names_;
		type_map_.set(atom_types.type_map_);

		return *this;
	}

	bool AtomTypes::extractSection
		(Parameters& parameters, const String& section_name)
	{
		valid_ = true;
		// extract the basis information
		if (!ParameterSection::extractSection(parameters, section_name))
		{
			Log.error() << "AtomTypes::extractSection: didn't find section for " << section_name << endl;
			return false;
		}

		// clear type map and name array
		type_map_.clear();
		names_.clear();

		// insert the wildcard name for type 0 (ANY_TYPE)
		names_.push_back(BALL_ATOM_UNKNOWN_NAME);
		type_map_[BALL_ATOM_WILDCARD_NAME] = (Atom::Type)0;
		type_map_[BALL_ATOM_UNKNOWN_NAME] = (Atom::Type)0;

		// iterate over all entries and insert them into
		// the hash map and the names_ array
		for (Size i = 0; i < getNumberOfKeys(); i++)
		{
			// insert the name into the hash map
			// and the names array
			type_map_[getKey(i)] = (Atom::Type)(names_.size());
			names_.push_back(getKey(i));
		}
		
		return true;
	}

	bool AtomTypes::hasType(const String& name) const 
	{
		return type_map_.has(name);
	}

	Atom::Type AtomTypes::getType(const String& name) const 
	{
		// try to find the name in the hash map
		StringHashMap<Atom::Type>::ConstIterator it = type_map_.find(name);
		if (it != type_map_.end())
		{
			return it->second;
		} else {
			return Atom::UNKNOWN_TYPE;
		}
	}

	String AtomTypes::getTypeName(Atom::Type type) const 
	{
		static const String empty_string;
		if ((type < 0) || (type >= (Index)names_.size()))
		{
			return empty_string;
		}
		
		return names_[type];
	}

	Size AtomTypes::getNumberOfTypes() const 
	{
		return names_.size();
	}

} // namespace BALL
