// $Id: persistenceManager.C,v 1.4 2000/03/12 22:19:59 oliver Exp $

#include <BALL/CONCEPT/persistenceManager.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/protein.h>
#include <BALL/KERNEL/nucleicAcid.h>
#include <BALL/KERNEL/bond.h>

// #define DEBUG_PERSISTENCE

using namespace std;

namespace BALL 
{

	PersistenceManager::PersistenceManager()
	{
		// register all kernel classes	
		registerKernelClasses_();
	}

	PersistenceManager::PersistenceManager(istream& is)
	{
		// register all kernel classes	
		registerKernelClasses_();
		setIstream(is);
	}

	PersistenceManager::PersistenceManager(ostream& os)
	{
		// register all kernel classes	
		registerKernelClasses_();
		setOstream(os);
	}

	PersistenceManager::PersistenceManager(istream& is, ostream& os)
	{
		// register all kernel classes	
		registerKernelClasses_();
		setIstream(is);
		setOstream(os);
	}

	PersistenceManager::~PersistenceManager()
	{
	}

	void PersistenceManager::registerKernelClasses_()
	{
		// register all kernel classes, their base classes, 
		// and the classes used in kernel classes
		//
		using namespace RTTI;
		#define REGISTER_CLASS(T) {CreateMethod m(getNew<T>); registerClass(getStreamName<T>(), m);}
		REGISTER_CLASS(BaseFragment)
		REGISTER_CLASS(NamedProperty)
		REGISTER_CLASS(Vector3)
		REGISTER_CLASS(Composite)
		REGISTER_CLASS(Atom)
		REGISTER_CLASS(Bond)
		REGISTER_CLASS(Fragment)
		REGISTER_CLASS(System)
		REGISTER_CLASS(Molecule)
		REGISTER_CLASS(PDBAtom)
		REGISTER_CLASS(Residue)
		REGISTER_CLASS(Chain)
		REGISTER_CLASS(Protein)
		REGISTER_CLASS(SecondaryStructure)
		REGISTER_CLASS(NucleicAcid)
		REGISTER_CLASS(Nucleotide)
		#undef REGISTER_CLASS
	}

	PersistenceManager& PersistentObject::operator >> (PersistenceManager& pm) const
	{ 
		pm.startOutput();
		persistentWrite(pm, "");
		pm.endOutput();
		return pm;
	}

	void PersistentObject::persistentWrite(PersistenceManager& /* pm */, const char* /* name */) const
	{
	}

	void PersistentObject::persistentRead(PersistenceManager& /* pm */)
	{
	}

	void PersistenceManager::registerClass(String signature, const CreateMethod	m)
	{
		create_methods_.insert(signature, m);
	}

	void* PersistenceManager::createObject(String signature) const
	{
		if (create_methods_.has(signature)) 
		{
			CreateMethod method = create_methods_[signature];

#			ifdef DEBUG_PERSISTENCE
				Log.info() << "PersistenceManager: created object of type" << signature << endl;
#			endif

			return method();

			
		} else {
			return 0;
		}
	}

	Size PersistenceManager::getNumberOfClasses() const
	{
		return create_methods_.size();
	}

	void PersistenceManager::setOstream(ostream& s) 
	{
		os = &s;

		object_out_.clear();
		object_out_needed_.clear();
	}

	void PersistenceManager::setIstream(istream& s) 
	{
		is = &s;

		pointer_list_.clear();
		pointer_map_.clear();
	}
		
	void PersistenceManager::startOutput()
	{
		object_out_.clear();
		object_out_needed_.clear();
		writeStreamHeader();
	}

	void PersistenceManager::endOutput()
	{
		writeStreamTrailer();
		addNeededObjects_();
		writeStreamTrailer();
	}

	PersistenceManager& PersistenceManager::operator << (const PersistentObject& object)
	{
		object >> *this;
		return *this;
	}

	PersistenceManager& PersistenceManager::operator >> (PersistentObject*& object_ptr)
	{
		object_ptr = readObject();
		return *this;
	}
	

	PersistentObject*	PersistenceManager::readObject()
	{
		if (is == 0)
		{
			return 0;
		}

		PersistentObject*	first_object = 0;
		PersistentObject*	obj = 0;

		pointer_map_.clear();
		pointer_list_.clear();
		object_in_.clear();
		
		String						type_name;
		LongPointerType		ptr;

		// if an error happened, just exit the loop 
		// to clean up the mess 
		bool error = false;

		// loop while the stream is not empty, 
		// we did not read the END mark and
		// an error did not occur
		while (*is && checkStreamHeader() && !error) 
		{
			// retrieve the first object signature
			getObjectHeader(type_name, ptr);
				
			if (!create_methods_.has(type_name)) 
			{
				// something bad happend - abort everything and clean up!
				Log.level(LogStream::ERROR) << "Cannot create object of unregistered class " << type_name << "!" << endl;
				error = true;
				break;
			} 
			
			// Create an instance of type_name 

			CreateMethod	m = create_methods_[type_name];
			obj = (PersistentObject*)m();

			// check whether the creation was successful
			if (obj == 0)
			{
				Log.level(LogStream::ERROR) << "Could not create object of typ " << type_name << "!" << endl;
				error = false;
				break;
			}

			// remember the this pointer of the new object 
			object_in_.push_back(obj);

			// check 
			if (ptr == 0)
			{
				Log.level(LogStream::ERROR) << "Read invalid object pointer!" << endl;
				error = true;
				break;
			}
				
			// store the old pointer and the new pointer
			addPointerPair_(ptr, (void*)obj);
				
			// make the new object read itself
			(*obj).persistentRead(*this);

			if (!checkObjectTrailer("") || !checkStreamTrailer())
			{
				error = false;
				break;
			}

			// remember the first object created to -
			// this is our return valued
			if (first_object == 0)
			{
				first_object = obj;
			}
		}

		if (error)
		{ 
			return 0;
		}

		// update the pointers: replace all old object addresses with the
		// new ones
		if (!updatePointers_())
		{
			return 0;
		}

		// finalize all objects
		ObjectList::iterator object_it = object_in_.begin();
		for (; object_it != object_in_.end(); object_it++)
		{
			const_cast<PersistentObject*>(*object_it)->finalize();
		}

		// return the first object read
		return first_object;
	}


	void PersistenceManager::addPointerPair_(LongPointerType old_ptr, void* new_ptr)
	{
#		ifdef DEBUG_PERSISTENCE
			Log.info() << "PersistenceManager: pointer pair (" << old_ptr << "/" << new_ptr << ")" << endl;
#		endif

		pointer_map_.insert(pair<LongPointerType, void*>(old_ptr, new_ptr));
	}


	void PersistenceManager::addNeededObjects_() 
	{
		while (object_out_needed_.size() > 0)
		{
			const PersistentObject*	obj = object_out_needed_.back();
			object_out_needed_.pop_back();

			if (!object_out_.has(obj))
			{
				writeStreamHeader();
				(*obj).persistentWrite(*this);
				writeStreamTrailer();
			} 
				
		}
	}

	bool PersistenceManager::updatePointers_()
	{
		// assume everything will go smoothly
		bool result = true;

		PointerList::iterator it = pointer_list_.begin();
		for (; it != pointer_list_.end(); ++it) 
		{
			if (pointer_map_.has((*it).second)) 
			{
				// OK. We know the correct value for the pointer
				(*(*it).first) = pointer_map_[(*it).second];
			} else { 
				Log.level(LogStream::ERROR) << "PersistenceManager: Could not assign object for pointer to "
					<< (*it).second << endl;
				result = false;
			}
		}
		
		return result;
	}

} // namespace BALL
