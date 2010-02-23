// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: atomTypes.C,v 1.13 2003/08/26 09:17:53 oliver Exp $
//

#include <BALL/MOLMEC/PARAMETER/atomTypes.h>
#include <BALL/MOLMEC/PARAMETER/forceFieldParameters.h>

using namespace std;

namespace BALL 
{

	AtomTypes::AtomTypes(ForceField* force_field) 
		: ParameterSection(),
			type_map_(),
			names_(),
			masses_(),
			comments_(),
			force_field_(force_field)
	{
	}


	AtomTypes::AtomTypes(const AtomTypes& atom_types) 
		: ParameterSection(atom_types),
			type_map_(atom_types.type_map_),
			names_(atom_types.names_),
			masses_(atom_types.masses_),
			comments_(atom_types.comments_),
			force_field_(atom_types.force_field_)
	{
	}


	AtomTypes::~AtomTypes() 
	{
		clear();

		valid_ = false;
	}

	void AtomTypes::clear() 
	{
		names_.clear();	
		masses_.clear();
		comments_.clear();
		type_map_.clear();
		force_field_ = 0;
		ParameterSection::clear();
	}


	AtomTypes& AtomTypes::operator = (const AtomTypes& atom_types) 
	{
		clear();

		ParameterSection::operator = (atom_types);
		type_map_ = atom_types.type_map_;
		names_    = atom_types.names_;
		masses_   = atom_types.masses_;	
		comments_ = atom_types.comments_;
		force_field_ = atom_types.force_field_;

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
		masses_.clear();

		// insert the wildcard name for type 0 (ANY_TYPE)
		names_.push_back(BALL_ATOM_UNKNOWN_NAME);
		masses_.push_back(BALL_ATOM_UNKNOWN_MASS);
		comments_.push_back("");
		type_map_[BALL_ATOM_WILDCARD_NAME] = (Atom::Type)0;
		type_map_[BALL_ATOM_UNKNOWN_NAME] = (Atom::Type)0;
	
		//Position type_column    = ParameterSection::getColumnIndex("type");
		Position mass_column    = ParameterSection::getColumnIndex("mass");
		Position comment_column = ParameterSection::getColumnIndex("comment");

		// iterate over all entries and insert them into
		// the hash map and the names_ array
		for (Size i = 0; i < getNumberOfKeys(); i++)
		{
			// insert the name into the hash map
			// and the names array
			type_map_[getKey(i)] = (Atom::Type)(names_.size());
			names_.push_back(getKey(i));
			masses_.push_back(ParameterSection::getValue(i, mass_column).toFloat());
			comments_.push_back(ParameterSection::getValue(i, comment_column));
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
		} 
		else 
		{
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
		return (Size)names_.size();
	}
		
	ForceField* AtomTypes::getForceField() 
	{
		return force_field_;
	}

	bool AtomTypes::exportParmFile(File& outfile) const
	{
		if (masses_.size() != names_.size())
			return false;
	
		// a string buffer for snprintf
		char buffer[1024];

		// do not write the dummy element
		for (Size i=1; i < names_.size(); i++)
		{
			if (i==1)
			{
				snprintf(buffer, 1024, "%-2s %-4.3f                !            %s", 
										names_[i].c_str(), masses_[i], comments_[i].c_str());
			}
			else
			{
				// emulate the format: H  1.008                             H bonded to nitrogen atoms
				// 										 CA 12.01                             sp2 C carbonyl group 
				snprintf(buffer, 1024, "%-2s %-4.3f                             %s", 
					 							 names_[i].c_str(), masses_[i], comments_[i].c_str());
			}
			outfile << buffer << endl;
		}		
		
		// Terminate by blank card
		outfile << endl;

		return true;
	}

	bool AtomTypes::operator == (const AtomTypes& atom_types) const 
	{
		return (ParameterSection::operator == (atom_types)
			&& (type_map_ == atom_types.type_map_)
			&& (names_    == atom_types.names_)
			&& (masses_   == atom_types.masses_)
			&& (comments_ == atom_types.comments_)
			&& (force_field_ == atom_types.force_field_));
	}

} // namespace BALL
