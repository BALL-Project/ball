// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: embeddable.C,v 1.18 2005/12/23 17:02:33 amoll Exp $
//

#include <BALL/CONCEPT/embeddable.h>

#undef BALL_DEBUG

namespace BALL
{

	Embeddable::Embeddable(const String& identifier)
		
		:	identifier_(identifier)
	{
		#ifdef BALL_DEBUG
			Log.info() << "constructing Embeddable(const String& identified = '" 
								 << identifier << "') [this = " << (void*)this << "]" << std::endl;
		#endif
	}

	Embeddable::Embeddable(const Embeddable& embeddable)
		
		:	identifier_(embeddable.identifier_)
	{
		#ifdef BALL_DEBUG
			Log.info() << "copy constructing Embeddable(embeddable = " << (void*)&embeddable 
								 <<" [this = " << (void*)this << "]" << std::endl;
		#endif
	}

	Embeddable::~Embeddable()
		
	{
		#ifdef BALL_DEBUG
			Log.info() << "destructing Embeddable [this = " << (void*)this << "]" << std::endl;
		#endif
		// make sure destructed instances are securely unregistered
		unregisterInstance_(this);
		identifier_ = "<destructed>";
	}

	const String& Embeddable::getIdentifier() const
		
	{
		return identifier_;
	}

	void Embeddable::setIdentifier(const String& identifier)
		
	{
		identifier_ = identifier;
	}

	void Embeddable::registerThis() 
		
	{
		if (typeid(*this) != typeid(BALL::Embeddable))
		{
			Log.error() << "Warning: derived class " << typeid(*this).name() << " was derived from BALL::Embeddable, but the macro "
									<< "BALL_EMBEDDABLE(...) was not specified in the class declaration!" << std::endl;
		}
		registerInstance_(typeid(BALL::Embeddable), this);
	}

	void Embeddable::unregisterThis() 
		
	{
		Embeddable::unregisterInstance_(this);
	}


	void Embeddable::registerInstance_(const std::type_info& type, const Embeddable* instance) 
		
	{
		// retrieve the class name
		const char* class_id_string = type.name();
		Embeddable* e_ptr = const_cast<Embeddable*>(instance);

		if (instance_to_type_map_.has(e_ptr))
		{
			EmbeddableVector& v = instance_vectors_[class_id_string];

			// Check whether we have that one already...
			if (std::find(v.begin(), v.end(), instance) == v.end())
			{
				// store the pointer to the instance in the instance vector...
				v.push_back(const_cast<Embeddable*>(instance));
			}

			return;
		}

		// this is in fact a new registration
		if (!instance_vectors_.has(class_id_string))
		{
			// create a new vector for instances of this type
			instance_vectors_.insert(class_id_string, EmbeddableVector());
		}
		// store the pointer to the instance in the instance vector...
		instance_vectors_[class_id_string].push_back(e_ptr);
		
		// ...and in the hash map for fast retrieval of the class id string
		instance_to_type_map_.insert(std::pair<Embeddable*, string>(e_ptr, class_id_string));
		#ifdef BALL_DEBUG
			Log.info() << "Embeddable::registerInstance_: registering " << class_id_string 
								 << " @ " << (void*)instance << std::endl;
		#endif
	}

	void Embeddable::unregisterInstance_(const Embeddable* const_instance) 
		
	{
		// for convenience, we use const pointers in the map, but const ptr
		// for registerInstance/unregisterInstance, since it is *logically* const.
		Embeddable* instance = const_cast<Embeddable*>(const_instance);

		// check whether this instance was registered...
		if (!instance_to_type_map_.has(instance))
		{
			return;
		}

		// retrieve the class id string for this specific instance
		string class_id_string = instance_to_type_map_[instance];
		if (!instance_vectors_.has(class_id_string))
		{
			// something terrible has happened: the instance was in the
			// hash map but not in the vector!
			Log.error() << "Embeddable::unregisterInstance_: Internal error: "
									<< "instance_vectors_ and instance_to_type_map_ are inconsistent!" << std::endl;
			return;
		}

		// remove the instance from all instance vectors
		HashMap<String, std::vector<Embeddable*> >::Iterator it = instance_vectors_.begin();
		for (; it != instance_vectors_.end(); ++it)
		{
			EmbeddableVector& v = it->second;
			EmbeddableVector::iterator last = std::remove(v.begin(), v.end(), instance);
			if (last != v.end())
			{
				v.erase(last);
			}
		}

		// remove it from the instance hash map
		instance_to_type_map_.erase(instance);
		#ifdef BALL_DEBUG
			Log.info() << "Embeddable::unregisterInstance_: unregistering " << (void*)instance << std::endl;
		#endif
	}

	Size Embeddable::countInstances_(const std::type_info& type) 
		
	{
		// check whether we got a vector with that name
		if (instance_vectors_.has(type.name()))
		{
			// retrieve the size of the coresponding instance vector
			return (Size)instance_vectors_[type.name()].size();
		}

		return 0;
	}

	Embeddable* Embeddable::getInstance_(const std::type_info& type, Position index)
		
	{
		// check whether we got a vector with that name
		if (instance_vectors_.has(type.name()))
		{
			EmbeddableVector& v = instance_vectors_[type.name()];
			if (v.size() > index)
			{
				return v[index];
			}
		}

		return 0;
	}

	Embeddable* Embeddable::getInstance_(const std::type_info& type, const String& identifier)
		
	{
		// check whether we got a vector with that name
		if (instance_vectors_.has(type.name()))
		{
			// walk along the vector, return the first instance
			// whose identifier matches
			EmbeddableVector::iterator it = instance_vectors_[type.name()].begin();	
			for (; it != instance_vectors_[type.name()].end(); ++it)
			{
				if ((*it)->getIdentifier() == identifier)
				{
					return *it;
				}
			}
		}

		return 0;
	}

	StringHashMap<Embeddable::EmbeddableVector>				
		Embeddable::instance_vectors_;

	HashMap<Embeddable*, string>	
		Embeddable::instance_to_type_map_;	


	void Embeddable::dump(std::ostream& s, Size depth) const
		
	{
		BALL_DUMP_STREAM_PREFIX(s);
		
		BALL_DUMP_DEPTH(s, depth);
		BALL_DUMP_HEADER(s, this, this);

		BALL_DUMP_DEPTH(s, depth);
		s << "number of instances: " << countInstances_(typeid(this)) << std::endl;
		BALL_DUMP_STREAM_SUFFIX(s);
	}

} // namespace
