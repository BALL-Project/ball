// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: property.C,v 1.31 2003/08/26 09:17:45 oliver Exp $
//

#include <BALL/CONCEPT/property.h>

using namespace std;

namespace BALL 
{

	NamedProperty::NamedProperty()
		: PersistentObject(),
			type_(NONE),
			name_("")
	{
	}

	NamedProperty::NamedProperty(const NamedProperty& property, const std::string& name)
		: PersistentObject(property),
			type_(property.type_),
			name_(name)
	{
		if (type_ != STRING)
		{
			data_ = property.data_;
		}
		else
		{
			data_ = new string(*(boost::any_cast<std::string*>(property.data_)));
		}
	}

	NamedProperty::NamedProperty(const NamedProperty& property) 
		: PersistentObject(property),
			type_(property.type_),
			name_(property.name_)
	{	
		if (type_ != STRING)
		{
			data_ = property.data_;
		} 
		else 
		{
			data_ = new string(*(boost::any_cast<std::string*>(property.data_)));
		}
	}
 


	void NamedProperty::operator = (const NamedProperty& np)
	{
		if(&np == this) return; // thou shalt not copy yourself!
		
		clear();
		name_ = np.name_;
		type_ = np.type_;
		if(type_ == STRING)
		{
			data_ = new string(np.getString());
		}
		else data_ = np.data_;
	}
	


  
	void PropertyManager::set(const PropertyManager& property_manager)
	{
		bitvector_ = property_manager.bitvector_;
		named_properties_ = property_manager.named_properties_;
	}

	void PropertyManager::setProperty(const NamedProperty& property)
	{
		// search whether the property already exists
		vector<NamedProperty>::iterator it = named_properties_.begin();
		for (; it != named_properties_.end(); ++it)
		{
			if (it->getName() == property.getName())
			{
				// yes, it exists. Erase the old content
				named_properties_.erase(it);
				break;
			}
		}

		// add the new content
		named_properties_.push_back(property);
	}

	void PropertyManager::setProperty(const string& name)
	{
		// search whether a property with the same name already exists
		vector<NamedProperty>::iterator it = named_properties_.begin();
		for (; it != named_properties_.end(); ++it)
		{
			if (it->getName() == name.c_str())
			{
				// yes, it exists. Erase the old content
				named_properties_.erase(it);
				break;
			}
		}

		// add the new content
		named_properties_.push_back(NamedProperty(name));
	}

	const NamedProperty& PropertyManager::getProperty(const string& name) const
	{
		for (Size i = 0; i < named_properties_.size(); ++i)
		{
			if (named_properties_[i].getName() == name.c_str())
			{
				return named_properties_[i];
			}
		}

		return RTTI::getDefault<NamedProperty>();
	}
	
	void PropertyManager::clearProperty(const string& name)
	{
    vector<NamedProperty>::iterator it = named_properties_.begin();
		for (; it != named_properties_.end(); ++it)
 		{
			if (it->getName() == name.c_str())
			{
				named_properties_.erase(it);
				break;
			}
		}
	}

	bool PropertyManager::hasProperty(const string& name) const
	{
		for (std::vector<NamedProperty>::const_iterator it = named_properties_.begin(); it != named_properties_.end(); ++it)
		{
			if (it->getName() == name)
			{
				return true;
			}
		}

		return false;
	}

	NamedPropertyIterator PropertyManager::beginNamedProperty()
	{
		return named_properties_.begin();
	}

	NamedPropertyIterator PropertyManager::endNamedProperty()
	{
		return named_properties_.end();
	}

	void PropertyManager::dump(ostream& s, Size depth) const
	{
		BALL_DUMP_STREAM_PREFIX(s);

		BALL_DUMP_DEPTH(s, depth);
		BALL_DUMP_CLASS_HEADER(s, PropertyManager, this);

		BALL_DUMP_DEPTH(s, depth);
		s << bitvector_ << endl;
		
		for (Size i = 0; i < named_properties_.size(); ++i)
		{
			BALL_DUMP_DEPTH(s, depth);
			s << "(" << named_properties_[i].getName();
			switch (named_properties_[i].getType())
			{
				case NamedProperty::NONE:
					s << "NONE: ";
					break;
				case NamedProperty::BOOL:
					s << "BOOL: " << named_properties_[i].getBool();
					break;
				case NamedProperty::INT:
					s << "INT: " << named_properties_[i].getInt();
					break;
				case NamedProperty::UNSIGNED_INT:
					s << "UNSIGNED_INT: " << named_properties_[i].getUnsignedInt();
					break;
				case NamedProperty::FLOAT:
					s << "FLOAT: " << named_properties_[i].getFloat();
					break;
				case NamedProperty::DOUBLE:
					s << "DOUBLE: " << named_properties_[i].getDouble();
					break;
				case NamedProperty::STRING:
					s << "STRING: " << (char)34 << named_properties_[i].getString().c_str() << (char)34;
					break;
				case NamedProperty::OBJECT:
					s << "OBJECT: " << named_properties_[i].getObject();
					break;
				case NamedProperty::SMART_OBJECT:
					s << "SMART_OBJECT: " << named_properties_[i].getSmartObject();
					break;
			}

			s << ")" << endl;
		}

		BALL_DUMP_STREAM_SUFFIX(s);
	}

#	ifdef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/CONCEPT/property.iC>
#	endif

} // namespace BALL
