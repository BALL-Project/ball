// $Id: embeddable.C,v 1.10 2001/12/26 03:35:52 oliver Exp $

#include <BALL/CONCEPT/embeddable.h>

using namespace std;

#define DEBUG

namespace BALL
{
	Embeddable::Embeddable(const String& identifier)
		throw()
		:	identifier_(identifier)
	{
		#ifdef DEBUG
			Log.info() << "constructing Embeddable(const String& identified = '" << identifier << "') [this = " << (void*)this << "]" << std::endl;
		#endif
	}
	
	Embeddable::Embeddable(const Embeddable& embeddable)
		throw()
		:	identifier_(embeddable.identifier_)
	{
		#ifdef DEBUG
			Log.info() << "copy constructing Embeddable(embeddable = " << (void*)&embeddable <<" [this = " << (void*)this << "]" << std::endl;
		#endif
	}
	
	Embeddable::~Embeddable()
		throw()
	{
		#ifdef DEBUG
			Log.info() << "destructing Embeddable [this = " << (void*)this << "]" << std::endl;
		#endif
		// make sure destructed instances are securely unregistered
		unregisterInstance_(this);
		identifier_ = "<destructed>";
	}

	const String& Embeddable::getIdentifier() const
		throw()
	{
		return identifier_;
	}

	void Embeddable::setIdentifier(const String& identifier)
		throw()
	{
		identifier_ = identifier;
	}

	void Embeddable::registerThis() 
		throw()
	{
		if (typeid(*this) != typeid(BALL::Embeddable))
		{
			Log.error() << "Warning: derived class " << typeid(*this).name() << " was derived from BALL::Embeddable, but the macro "
									<< "BALL_EMBEDDABLE(...) was not specified in the class declaration!" << std::endl;
		}
		registerInstance_(typeid(BALL::Embeddable), this);
	}

	void Embeddable::unregisterThis() 
		throw()
	{
		Embeddable::unregisterInstance_(this);
	}

	// protected:

	void Embeddable::registerInstance_
		(const type_info& type, const Embeddable* instance) 
		throw()
	{
		// retrieve the class name
		const char* class_id_string = type.name();

		if (!instance_to_type_map_.has(const_cast<Embeddable*>(instance)))
		{
			// this is fact a new registration
			if (!instance_lists_.has(class_id_string))
			{
				// create a new list for instances of this type
				instance_lists_.insert(class_id_string, EmbeddableList());
			}
			// store the pointer to the instance in the instance list...
			instance_lists_[class_id_string].push_back(const_cast<Embeddable*>(instance));
			
			// ...and in the hash map for fast retrieval of the class id string
			instance_to_type_map_.insert(pair<Embeddable*, string>(const_cast<Embeddable*>(instance), class_id_string));
			#ifdef DEBUG
				Log.info() << "Embeddable::registerInstance_: registering " << class_id_string 
									 << " @ " << (void*)instance << std::endl;
			#endif
		}
		else 
		{
			Log.warn() << "Embeddable::registerInstance_: "
				<< "trying to register an already registered instance at " 
				<< instance << " (type = " << class_id_string << ")" << endl;
		}
	}

	void Embeddable::unregisterInstance_(const Embeddable* const_instance) 
		throw()
	{
		// for convenience, we use const pointers in the map, but const ptrs
		// for registerInstance/unregisterInstance, since it is *logically* const.
		Embeddable* instance = const_cast<Embeddable*>(const_instance);

		// check whether this instance was registered...
		if (instance_to_type_map_.has(instance))
		{
			// retrieve the class id string for this specific instance
			string class_id_string = instance_to_type_map_[instance];
			if (instance_lists_.has(class_id_string))
			{
				// remove the instance from the instance list
				instance_lists_[class_id_string].remove(instance);

				// remove it from the instance hash map
				instance_to_type_map_.erase(instance);
				#ifdef DEBUG
					Log.info() << "Embeddable::unregisterInstance_: unregistering " << (void*)instance << std::endl;
				#endif
			}
			else 
			{
				// something terrible has happened: the instance was in the
				// hash map but not in the list!
				Log.error() << "Embeddable::unregisterInstance_: Internal error: "
										<< "instance_lists_ and instance_to_type_map_ are inconsistent!" << endl;
			}
		}
		else 
		{
			// complain if trying to unregister an unregistered instance
			Log.warn() << "Embeddable::unregisterInstance_: trying to unregister unregistered instance at " 
								 << instance << endl;
		}	
	}

	Size Embeddable::countInstances_(const type_info& type) 
		throw()
	{
		Size number = 0;
		// check whether we got a list with that name
		if (instance_lists_.has(type.name()))
		{
			// retrieve the size of the coresponding instance list
			number = (Size)instance_lists_[type.name()].size();
		}

		return number;
	}
	
	Embeddable* Embeddable::getInstance_(const type_info& type, Position index)
		throw()
	{
		// the return value;
		Embeddable* instance = 0;

		// check whether we got a list with that name
		if (instance_lists_.has(type.name()))
		{
			// walk along the list, decrement the counter until
			// the desired list element is reached
			Index counter = (Index)index + 1;
			EmbeddableList::Iterator it = instance_lists_[type.name()].begin();	
			for (; it != instance_lists_[type.name()].end() && counter != 0; ++it, --counter)
			{
				instance = *it;
			}
			if (counter != 0)
			{
				// we didn't find the correct index - return NULL
				instance = 0;
			}
		}

		return instance;
	}

	Embeddable* Embeddable::getInstance_(const type_info& type, const String& identifier)
		throw()
	{
		// the return value;
		Embeddable* instance = 0;

		// check whether we got a list with that name
		if (instance_lists_.has(type.name()))
		{
			// walk along the list, return the first instance
			// whose identifier matches
			EmbeddableList::Iterator it = instance_lists_[type.name()].begin();	
			for (; it != instance_lists_[type.name()].end(); ++it)
			{
				if ((*it)->getIdentifier() == identifier)
				{
					// assign the pointer and abort loop
					instance = *it;
					break;
				}
			}
		}

		return instance;
	}

	StringHashMap<Embeddable::EmbeddableList>				
		Embeddable::instance_lists_;

	HashMap<Embeddable*, string>	
		Embeddable::instance_to_type_map_;	
}
