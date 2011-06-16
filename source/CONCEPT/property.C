// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: property.C,v 1.31 2003/08/26 09:17:45 oliver Exp $
//

#include <BALL/CONCEPT/property.h>
#include <BALL/CONCEPT/persistenceManager.h>
#include <BALL/CONCEPT/textPersistenceManager.h>

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
 
	void NamedProperty::persistentWrite(PersistenceManager& pm, const char* name) const
	{
		pm.writeObjectHeader(this, name);
			pm.writePrimitive((int)type_, "type_");
			pm.writePrimitive(String(name_), "name_");
			
			switch (type_)
			{
				case	INT:					pm.writePrimitive(boost::any_cast<int>(data_), "data_.i");		break;
				case	FLOAT:				pm.writePrimitive(boost::any_cast<float>(data_), "data_.f");		break;
				case	DOUBLE:				pm.writePrimitive(boost::any_cast<double>(data_), "data_.d");		break;
				case	UNSIGNED_INT:	pm.writePrimitive(boost::any_cast<unsigned int>(data_), "data_.ui"); break;
				case	BOOL:					pm.writePrimitive(boost::any_cast<bool>(data_), "data_.b");		break;
				case	OBJECT:				pm.writeObjectPointer(boost::any_cast<PersistentObject*>(data_), "data_.object"); break;
				case	SMART_OBJECT:	pm.writeObjectPointer(boost::any_cast<boost::shared_ptr<PersistentObject> >(data_).get(), "data_.smart_object"); break;
				case	NONE:					break;
				case	STRING:				pm.writePrimitive(String(*boost::any_cast<string*>(data_)), "data_.s");	break;
				default:
					Log.error() << "cannot write unknown property type: " << (int)type_ << endl;
			}
		pm.writeObjectTrailer(name);
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
	
	void NamedProperty::persistentRead(PersistenceManager& pm)
	{
		// Clear potentially allocated strings.
		if (type_ == STRING)
		{
			delete boost::any_cast<string*>(data_);
		}

		int type;
		pm.readPrimitive(type, "type_");
		type_ = (Type)type;
		String s;
		pm.readPrimitive(s, "name_");
		name_ = s;

		switch (type_)
		{
			case	INT:					int i;
													pm.readPrimitive(i, "data_.i");		
													data_ = i;
													break;
			case	FLOAT:				float f;
													pm.readPrimitive(f, "data_.f");		
													data_ = f;
													break;
			case	DOUBLE:				double d;
			                    pm.readPrimitive(d, "data_.d");
													data_ = d;
													break;
			case	UNSIGNED_INT:	unsigned int ui;
			                    pm.readPrimitive(ui, "data_.ui");  
													data_ = ui;
													break;
			case	BOOL:					bool b;
			                    pm.readPrimitive(b, "data_.b");		
													data_ = b;
													break;
			case	NONE:	break;

			case	STRING:				
				// we have to create a new string
				pm.readPrimitive(s, "data_.s");	
				data_ = new string(s);
				break;

			case	OBJECT:				
				// the persistence manager will take care of
				// reading the object and will set this pointer afterwards 
				data_ = (PersistentObject*)0;
				pm.readObjectPointer(*(boost::any_cast<PersistentObject*>(&data_)), "data_.object"); 
				break;

			case	SMART_OBJECT:				
				// the persistence manager will take care of
				// reading the object and will set this pointer afterwards
				data_ = boost::shared_ptr<PersistentObject>();
				pm.readObjectSmartPointer(*(boost::any_cast<boost::shared_ptr<PersistentObject> >(&data_)), "data_.smart_object");
				break;

			default:
				Log.error() << "Unknown type while reading NamedProperty: " << (int)type_ << endl;
		}
	}

  void PropertyManager::write(PersistenceManager& pm) const
  {
		pm.writeStorableObject(bitvector_, "bitvector_");
		Size size = (Size)named_properties_.size();
		pm.writePrimitive(size, "size");
		for (Size i = 0; i < size; i++)
		{
			named_properties_[i].persistentWrite(pm, "");
		}
	}

  bool PropertyManager::read(PersistenceManager& pm)
  {
		if (!pm.readStorableObject(bitvector_, "bitvector_"))
		{
			return false;
		}
		
		Size size = 0;
		pm.readPrimitive(size, "size");
		named_properties_.resize(size);
		for (Size i = 0; i < size; i++)
		{
			NamedProperty& property = named_properties_[i];
			pm.checkObjectHeader(property, "");
				property.persistentRead(pm);
			pm.checkObjectTrailer("");
		}

		return true;
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
				case NamedProperty::SMART_OBJECT:
					s << "SMART_OBJECT: " << named_properties_[i].getSmartObject();
			}

			s << ")" << endl;
		}

		BALL_DUMP_STREAM_SUFFIX(s);
	}

#	ifdef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/CONCEPT/property.iC>
#	endif

} // namespace BALL
