#include <BALL/MOLMEC/PARAMETER/atomTypes.h>
#include <BALL/MOLMEC/PARAMETER/atomEquivalences.h>
#include <BALL/MOLMEC/COMMON/forceField.h>

#include <BALL/FORMAT/parameters.h>

#define	BALL_ATOM_WILDCARD_NAME "*" 
#define	BALL_ATOM_UNKNOWN_NAME "?"

using namespace std;

namespace BALL 
{

	AtomEquivalences::AtomEquivalences(ForceField* force_field) 
		: ParameterSection(),
			reference_type_map_(),
			equivalent_types_(),
			force_field_(force_field)
	{
	}


	AtomEquivalences::AtomEquivalences(const AtomEquivalences& atom_equivalences) 
		: ParameterSection(atom_equivalences),
			reference_type_map_(atom_equivalences.reference_type_map_),
		 	equivalent_types_(atom_equivalences.equivalent_types_),
		  force_field_(atom_equivalences.force_field_)
	{
	}


	AtomEquivalences::~AtomEquivalences() 
	{
		clear();

		valid_ = false;
	}

	bool AtomEquivalences::extractSection
		(Parameters& parameters, const String& section_name) 
	{
		valid_ = true;

		// extract the basis information
		if (!ParameterSection::extractSection(parameters, section_name))
		{
			Log.error() << "AtomEquivalences::extractSection: didn't find section for " << section_name << endl;
			return false;
		}
		
		// check whether all variables we need are defined, terminate otherwise
		if (!hasVariable("type_eqvt"))
		{
			Log.error() << "AtomEquivalences::extractSection: AtomEquivalences section (" << section_name 
				<< ") needs colum type_eqvt!" << std::endl;
			return false;
		}

		reference_type_map_.clear();                
		equivalent_types_.clear();

		// insert the wildcard name for type 0 (ANY_TYPE)
		reference_type_map_[BALL_ATOM_WILDCARD_NAME] = (Atom::Type)0;
		reference_type_map_[BALL_ATOM_UNKNOWN_NAME]  = (Atom::Type)0;
	
		Position equivalent_column = ParameterSection::getColumnIndex("type_eqvt");
	
		String	key;
		String	fields[2];
		String 	reference_name = "";
		String 	equivalent_name = "";
		Atom::Type		 reference_type;
    vector<String> dummy;

		const AtomTypes&	atom_types = force_field_->getParameters().getAtomTypes();

		// iterate over all entries and insert them into
		// the hash map and the names_ array
		for (Size i = 0; i < getNumberOfKeys(); i++)
		{
			key = getKey(i); 
			if (key.split(fields, 2) == 2)
			{
				// store the previous equivalent set
				if ((fields[1] == "1") && (reference_name != ""))
				{
					equivalent_types_[reference_name] = dummy;
					dummy.clear();
				}

				// determine all atom types
				reference_name  = fields[0];
				equivalent_name = getValue(key, "type_eqvt");//equivalent_column);	
	
				if (atom_types.hasType(reference_name))
				{ 
					reference_type  = atom_types.getType(fields[0]);

					if (fields[1] == "1")
					{ 
						reference_type_map_[reference_name] = reference_type;
						dummy.push_back(reference_name);
					}

					reference_type_map_[equivalent_name] = reference_type;
					dummy.push_back(equivalent_name);
				}
				else
				{	
					Log.error() << "AtomEquivalences::extractSection: unknown reference type" << reference_name << endl;
				}
			}
			else 
			{
				Log.error() << "AtomEquivalences::extractSection: could not interpret key " << key << std::endl;
			}
		}

		// write the last entry
		equivalent_types_[reference_name] = dummy;

		return true;
	}
	
	Atom::Type AtomEquivalences::getReferenceType(const String& name) const
	{
		Atom::Type ret_type =  Atom::UNKNOWN_TYPE;	
		
		if (force_field_)
		{
			const AtomTypes&	atom_types = force_field_->getParameters().getAtomTypes();

			Atom::Type test_name = atom_types.getType(name);
			if (reference_type_map_.has(test_name))
			{
				ret_type = atom_types.getType(reference_type_map_[test_name]);
			}
		}
		return ret_type;
			
	}

	String  AtomEquivalences::getReferenceTypeName(Atom::Type type) const
	{
		if (reference_type_map_.has(type))
		{
			return reference_type_map_[type];
		}
		else 
		{
			return BALL_ATOM_UNKNOWN_NAME;
		}
	}

	bool AtomEquivalences::isEquivalent(const String& name_a, const String& name_b) const
	{
		bool value = false;
		
		if (force_field_)
		{
			const AtomTypes&	atom_types = force_field_->getParameters().getAtomTypes();

			Atom::Type type_a = atom_types.getType(name_a);
			Atom::Type type_b = atom_types.getType(name_b);
			value = isEquivalent(type_a, type_b);
		}
		return value; 		
	}
	
	bool AtomEquivalences::isEquivalent(const Atom::Type type_a, const Atom::Type type_b) const
	{
		bool value = false;
		
		if (reference_type_map_.has(type_a))
		{
			value = (reference_type_map_[type_a] == type_b);
		}
		else if (reference_type_map_.has(type_b))
		{
			value = (reference_type_map_[type_b] == type_a);
		}
		return value; 
	}

	ForceField* AtomEquivalences::getForceField() 
	{
		return force_field_;
	}

	bool AtomEquivalences::exportParmFile(File& outfile) const
	{
		if (valid_)
		{
			String key = ""; 

			StringHashMap< vector<String> >::ConstIterator it =  equivalent_types_.begin();
			for ( ; it != equivalent_types_.end(); it++)
			{
				key = it->first;
				outfile << key << " "; 
				for (Size i=1; i < it->second.size(); i++)
				{
					outfile << "  " << it->second[i];
				}
				outfile << endl;
			}

			// Terminate by blank card
			outfile << endl;
		}
		return valid_;
	}
	
	void AtomEquivalences::clear() 
	{
		reference_type_map_.clear();                
		equivalent_types_.clear();

		ParameterSection::clear();
	}
	
	AtomEquivalences& AtomEquivalences::operator = (const AtomEquivalences& atom_equivalences) 
	{
		clear();

		ParameterSection::operator = (atom_equivalences);
		reference_type_map_ = atom_equivalences.reference_type_map_;
		equivalent_types_   = atom_equivalences.equivalent_types_;
		force_field_				= atom_equivalences.force_field_;

		return *this;
	}

	bool AtomEquivalences::operator == (const AtomEquivalences& atom_equivalences) const 
	{
		return (ParameterSection::operator == (atom_equivalences)
			&& 	(reference_type_map_ == atom_equivalences.reference_type_map_)
			&&  (equivalent_types_   == atom_equivalences.equivalent_types_)	
			&&  (force_field_				 == atom_equivalences.force_field_));
	}

} // namespace BALL
