// $Id: templates.C,v 1.9 2000/07/06 14:44:25 oliver Exp $
//

#include <BALL/MOLMEC/PARAMETER/templates.h>
#include <BALL/KERNEL/residue.h>

using namespace std;

namespace BALL 
{

	Templates::Templates()
		: ParameterSection()
	{
	}


	Templates::Templates(const Templates& templates, bool /* deep */)
		: ParameterSection(templates)
	{
		charges_ = templates.charges_;
		type_names_ = templates.type_names_;
	}


	void Templates::destroy()
	{
		charges_.destroy();
		type_names_.destroy();

		ParameterSection::destroy();
	}

		

	Templates::~Templates()
	{
		destroy();
	}

	Templates& Templates::operator = (const Templates& templates)
	{
		charges_.set(templates.charges_);
		type_names_.set(templates.type_names_);

		return *this;
	}

	bool Templates::extractSection
		(Parameters& parameters, const String& section_name)
	{
		// clean up first
		type_names_.destroy();
		charges_.destroy();

		// extract the basis information
		if (!ParameterSection::extractSection(parameters, section_name))
		{
			Log.level(LogStream::ERROR) << "Didn't find section for " << section_name << endl;
			return false;
		}

		// check for the correct entries:
		if (!hasVariable("q") || !hasVariable("type"))
		{
			Log.level(LogStream::ERROR) << "residue template section [" << section_name << "] requires variables q and type." << endl;
			return false;
		}

		// check for the correct unit
		if (!options.has("unit_q") || (options["unit_q"] != "e0"))
		{
			Log.level(LogStream::ERROR) << "unknown unit in section [" << section_name 
				<< "]. Please specify charges in multiples of the elementary charge (e0)" << endl;
			return false;
		}

		// iterate over all entries and store charges and types in the
		// corresponding hash maps
		StringHashMap<Index>::Iterator	it;
		for (it = section_entries_.begin(); it != section_entries_.end(); ++it)
		{
			type_names_[it->first] = getValue(it->first, "type");
			charges_[it->first] = getValue(it->first, "q").toFloat();
		}
		
		return true;
	}

	bool Templates::has(const String& name) const 
	{
		return type_names_.has(name);
	}

	float Templates::getCharge(const String& name) const 
	{
		if (charges_.has(name)) 
		{
			return (*charges_.find(name)).second;
		} else {
			return 0.0;
		}
	}

	String Templates::getTypeName(const String& name) const 
	{
		if (type_names_.has(name)) 
		{
			return (*type_names_.find(name)).second;
		} else {
			return "";
		}
	}
	
	void Templates::assign(System& system, bool overwrite_existing_type_names, bool overwrite_non_zero_charges) const
	{
		// remember the current parent to avoid
		// the recalculation of the parent`s name
		String		parent_name(":");

		// iterate over all atoms
		AtomIterator it = system.beginAtom();
		for (; +it; ++it)
		{
			String name(it->getFullName());
			if (overwrite_non_zero_charges || (it->getCharge() == 0.0))
			{
				if (charges_.has(name))
				{
					it->setCharge(charges_[name]);
				} 
				else 
				{
					Log.warn() << "Templates::assign: cannot assign charge for atom " << name << endl;
				}
			}
			if (overwrite_existing_type_names || (it->getTypeName() == BALL_ATOM_DEFAULT_TYPE_NAME))
			{
				if (type_names_.has(name))
				{
					it->setTypeName(type_names_[name]);
				} else {
					Log.warn() << "Templates::assign: cannot assign type name for atom " << name << endl;
				}
			}
		}	
	}



	void Templates::assignTypeNames(System& system, bool overwrite_existing_type_names) const
	{
		// iterate over all atoms
		AtomIterator it = system.beginAtom();
		for (; +it; ++it)
		{
			if ((overwrite_existing_type_names || (it->getTypeName() == BALL_ATOM_DEFAULT_TYPE_NAME)))
			{
				// retrieve the atom's full name
				String name(it->getFullName());
				if (charges_.has(name))
				{	
					// assign the charge
					it->setCharge(charges_[name]);
				} else {
					// try the residue name without variant extension
					name = it->getFullName(Atom::NO_VARIANT_EXTENSIONS);
					if (charges_.has(name))
					{	
						// assign the charge
						it->setCharge(charges_[name]);
					} else {
						// try a final wildcard match
						name = "*:" + it->getName();
						name.trim();
						if (charges_.has(name))
						{
							// assign the charge
							it->setCharge(charges_[name]);
						} else {
							Log.warn() << "Templates::assignTypeNames: cannot assign type name for atom " << it->getFullName() << endl;
						}
					}
				}
			}
		}	
	}


	void Templates::assignCharges(System& system, bool overwrite_non_zero_charges) const
	{
		// iterate over all atoms
		AtomIterator it = system.beginAtom();
		for (; +it; ++it)
		{
			if ((overwrite_non_zero_charges || (it->getTypeName() == BALL_ATOM_DEFAULT_TYPE_NAME)))
			{
				// retrieve the atom's full name
				String name(it->getFullName());
				if (charges_.has(name))
				{	
					// assign the type name
					it->setCharge(charges_[name]);
				} else {
					// try the residue name without variant extension
					name = it->getFullName(Atom::NO_VARIANT_EXTENSIONS);
					if (charges_.has(name))
					{	
						// assign the type name
						it->setCharge(charges_[name]);
					} else {
						// try a final wildcard match
						name = "*:" + it->getName();
						name.trim();
						if (charges_.has(name))
						{
							// assign it
							it->setCharge(charges_[name]);
						} else {
							Log.warn() << "Templates::assignCharges: cannot assign charge for atom " << it->getFullName() << endl;
						}
					}
				}
			}
		}	
	}

} // namespace BALL
