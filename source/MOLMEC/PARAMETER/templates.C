// $Id: templates.C,v 1.3 1999/12/04 18:34:32 oliver Exp $
//

#include <BALL/MOLMEC/PARAMETER/templates.h>
#include <BALL/KERNEL/residue.h>

namespace BALL 
{

	FFPSTemplates::FFPSTemplates()
		: FFParameterSection()
	{
	}


	FFPSTemplates::FFPSTemplates(const FFPSTemplates& templates, bool /* deep */)
		: FFParameterSection(templates)
	{
		charges_ = templates.charges_;
		type_names_ = templates.type_names_;
	}


	void FFPSTemplates::destroy()
	{
		charges_.destroy();
		type_names_.destroy();

		FFParameterSection::destroy();
	}

		

	FFPSTemplates::~FFPSTemplates()
	{
		destroy();
	}

	FFPSTemplates& FFPSTemplates::operator = (const FFPSTemplates& templates)
	{
		charges_.set(templates.charges_);
		type_names_.set(templates.type_names_);

		return *this;
	}

	bool FFPSTemplates::extractSection
		(ForceFieldParameters& parameters, 
		 const String& section_name)
	{
		// clean up first
		type_names_.destroy();
		charges_.destroy();

		// extract the basis information
		if (!FFParameterSection::extractSection(parameters, section_name))
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
		StringHashMap<Index>::iterator	it;
		for (it = section_entries_.begin(); it != section_entries_.end(); ++it)
		{
			type_names_[it->first] = getValue(it->first, "type");
			charges_[it->first] = getValue(it->first, "q").toFloat();
		}
		
		return true;
	}

	bool FFPSTemplates::has(const String& name) const 
	{
		return type_names_.has(name);
	}

	float FFPSTemplates::getCharge(const String& name) const 
	{
		if (charges_.has(name)) 
		{
			return (*charges_.find(name)).second;
		} else {
			return 0.0;
		}
	}

	String FFPSTemplates::getTypeName(const String& name) const 
	{
		if (type_names_.has(name)) 
		{
			return (*type_names_.find(name)).second;
		} else {
			return "";
		}
	}
	
	void FFPSTemplates::assign(System& system, bool overwrite_existing_type_names, bool overwrite_non_zero_charges) const
	{
		// remember the current parent to avoid
		// the recalculation of the parent`s name
		Fragment* parent = 0;
		String		parent_name(":");

		// iterate over all atoms
		AtomIterator it = system.beginAtom();
		for (; +it; ++it)
		{
			// determine the parent`s name
			if (it->getFragment() != parent)
			{
				parent = it->getFragment();
				if (parent == 0)
				{
					parent_name = ":";
				} else {
					parent_name = parent->getName().trim();
					Residue* res = dynamic_cast<Residue*>(parent);
					// if the parent fragment is a residue, determine its properties
					if (res != 0)
					{
						String suffix = "-";
						if (res->isNTerminal())
						{
							suffix = "-N";
						}
						if (res->isCTerminal())
						{
							suffix = "-C";
						}
						if (res->hasProperty(Residue::PROPERTY__HAS_SSBOND))
						{
							suffix += "S";
						}
						
						if (suffix != "-")
						{
							parent_name += suffix;
						}
					}
					
					parent_name += ":";
				}
			}

			String name(parent_name + it->getName().trim());
			if (overwrite_non_zero_charges || (it->getCharge() == 0.0))
			{
				if (charges_.has(name))
				{
					it->setCharge(charges_[name]);
				} else {
					Log.warn() << "FFPSTemplates::assign: cannot assign charge for atom " << name << endl;
				}
			}
			if (overwrite_existing_type_names || (it->getTypeName() == BALL_ATOM_DEFAULT_TYPE_NAME))
			{
				if (type_names_.has(name))
				{
					it->setTypeName(type_names_[name]);
				} else {
					Log.warn() << "FFPSTemplates::assign: cannot assign type name for atom " << name << endl;
				}
			}
		}	
	}



	void FFPSTemplates::assignTypeNames(System& system, bool overwrite_existing_type_names) const
	{
		// remember the current parent to avoid
		// the recalculation of the parent`s name
		Fragment* parent = 0;
		String		parent_name(":");

		// iterate over all atoms
		AtomIterator it = system.beginAtom();
		for (; +it; ++it)
		{
			// determine the parent`s name
			if (it->getFragment() != parent)
			{
				parent = it->getFragment();
				if (parent == 0)
				{
					parent_name = ":";
				} else {
					parent_name = parent->getName().trim();
					Residue* res = dynamic_cast<Residue*>(parent);
					// if the parent fragment is a residue, determine its properties
					if (res != 0)
					{
						String suffix = "-";
						if (res->isNTerminal())
						{
							suffix = "-N";
						}
						if (res->isCTerminal())
						{
							suffix = "-C";
						}
						if (res->hasProperty(Residue::PROPERTY__HAS_SSBOND))
						{
							suffix += "S";
						}
						
						if (suffix != "-")
						{
							parent_name += suffix;
						}
					}
					
					parent_name += ":";
				}
			}

			String name(parent_name + it->getName().trim());
			if ((overwrite_existing_type_names || (it->getTypeName() == BALL_ATOM_DEFAULT_TYPE_NAME)))
			{
				if (type_names_.has(name))
				{
					it->setTypeName(type_names_[name]);
				} else {
					Log.warn() << "FFPSTemplates::assignTypeNames: cannot assign type name for atom " << name << endl;
				}
			}
		}	
	}


	void FFPSTemplates::assignCharges(System& system, bool overwrite_non_zero_charges) const
	{
		// remember the current parent to avoid
		// the recalculation of the parent`s name
		Fragment* parent = 0;
		String		parent_name(":");

		// iterate over all atoms
		AtomIterator it = system.beginAtom();
		for (; +it; ++it)
		{
			// determine the parent`s name
			if (it->getFragment() != parent)
			{
				parent = it->getFragment();
				if (parent == 0)
				{
					parent_name = ":";
				} else {
					parent_name = parent->getName().trim();
					Residue* res = dynamic_cast<Residue*>(parent);
					// if the parent fragment is a residue, determine its properties
					if (res != 0)
					{
						String suffix = "-";
						if (res->isNTerminal())
						{
							suffix = "-N";
						}
						if (res->isCTerminal())
						{
							suffix = "-C";
						}
						if (res->hasProperty(Residue::PROPERTY__HAS_SSBOND))
						{
							suffix += "S";
						}
						
						if (suffix != "-")
						{
							parent_name += suffix;
						}
					}
					
					parent_name += ":";
				}
			}

			String name(parent_name + it->getName().trim());
			if ((overwrite_non_zero_charges || (it->getCharge() == 0.0)))
			{
				if (charges_.has(name))
				{
					it->setCharge(charges_[name]);
				} else {
					Log.warn() << "FFPSTemplates::assignCharges: cannot assign charge for atom " << name << endl;
				}
			}
		}	
	}

} // namespace BALL
