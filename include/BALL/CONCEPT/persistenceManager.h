// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_CONCEPT_PERSISTENCEMANAGER_H
#define BALL_CONCEPT_PERSISTENCEMANAGER_H

#ifndef BALL_COMMON_RTTI_H
#	include <BALL/COMMON/rtti.h>
#endif

#ifndef BALL_DATATYPE_HASHMAP_H
#	include <BALL/DATATYPE/hashMap.h>
#endif

#ifndef BALL_DATATYPE_STRINGHASHMAP_H
#	include <BALL/DATATYPE/stringHashMap.h>
#endif

#ifndef BALL_DATATYPE_HASHSET_H
#	include <BALL/DATATYPE/hashSet.h>
#endif

#ifndef BALL_CONCEPT_PERSISTENTOBJECT_H
#	include <BALL/CONCEPT/persistentObject.h>
#endif

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#include <fstream>
#include <iomanip>

#include <boost/shared_ptr.hpp>

#define BALL_WRITE_PRIMITIVE_MEMBER(pm,x) pm.writePrimitive(x,#x)
#define BALL_READ_PRIMITIVE_MEMBER(pm,x) pm.readPrimitive(x,#x)

namespace BALL 
{

	/**	Persistence manager class.
			This class serializes and deserializes persistent objects and
			provides support for the implementation of the object-specific 
			serialization methods  \link PersistenceManager::persistentRead persistentRead \endlink  and  \link PersistenceManager::persistentWrite persistentWrite \endlink .
			It defines three different layers:
				- <b>Layer 0</b> contains the basic I/O routines for primitive
					data types. All methods of layer 0 are virtual to exchange the 
					implementation of the format-independent storage and retrieval of
					this data (e.g. XDR or text format)
				- <b>Layer 1</b> implements the methods needed to store objects
					or more complex data structures using Layer 0. To implement
					object persistence for a user defined object, methods from layer
					0 and layer 1 are needed
				- <b>Layer 2</b> implements the persistence manager's <b>user interface</b>.
					These are the methods needed to register classes,
					set the associated streams, and to store or retrieve objects.
			When writing a pointer to a persistent object, the serialization of
			the referenced object is automatically initiated after the objects
			holding the pointer has been written. Hence, pointers between persistent 
			objects remain valid after deserializing the objects again.
			 \par
			<b>Note:</b> This class is mainly an interface definition and contains
			abstract methods. Do not try to instantiate a PersistenceManager,
			your compiler will be complaining.
			@see	PersistentObject
			@see	XDRPersistenceManager
			@see	TextPersistenceManager
		 \ingroup  Persistence
	*/		
	class BALL_EXPORT PersistenceManager
	{
		public:

		/**	@name	Type Definitions
		*/
		//@{
			
		/**	Create method type.
				This type describes a method to dynamically create a specific
				object.  It should return a <tt>void</tt> pointer for interface
				compatibility and doesn't take an argument.  It creates a new
				object and returns the object's <tt>this</tt> pointer (cast to <tt>
				void*</tt>).  The  \link PersistenceManager::getNew getNew \endlink  function (in the RTTI namespace) is an
				example for such a method.
				@see registerClass
				@see RTTI
				@see CREATE
		*/
		typedef void * (*CreateMethod) ();
		//@}

		/**	@name Constructors and Destructors 
		*/
		//@{

		/**	Default constructor
		*/
		PersistenceManager();

		/** Copy constructor
		*/
		PersistenceManager(const PersistenceManager& pm);

		/**	Detailed constructor with an input stream.
				Creates a persistence manager object and assigns an input stream.
		*/
		PersistenceManager(::std::istream& is);
			
		/**	Detailed constructor with an output stream.
				Creates a persistence manager object and assigns an output stream.
		*/
		PersistenceManager(::std::ostream& os);
			
		/**	Detailed constructor with an input stream and an output stream.
				Creates a persistence manager object and assigns an input stream
				and an output stream.
		*/
		PersistenceManager(::std::istream& is, ::std::ostream& os);
			
		/**	Destructor.
				Destruct the persistence manager and and clear up all data
				structures. The associated streams or sockets
				( \link PersistenceManager::setIStream setIStream \endlink / \link PersistenceManager::setOStream setOStream \endlink ) are not closed.
		*/
		virtual ~PersistenceManager();

		//@}

		/**	@name Layer 2 commands
		*/
		//@{

		/**	Register a create method for a class.
				Each object read by the persistence manager has to be constructed
				somehow. The persistence manager first reads a class' signature
				(i.e. a unique identifier in the context of this stream). This is
				usually the stream name of the class (see  \link PersistenceManager::getStreamName getStreamName \endlink ), but
				can be an arbitrary string (without blanks). When reading an
				object header with a given class signature, the persistence manager
				tries to find a method to create an instance of this object. For
				this purpose it contains a StringHashMap object. Each of the
				classes to be read has to be contained in this hash map together
				with a method to create an instance of this object. This is done
				by calling registerClass. The create method is usually the
				 \link PersistenceManager::getNew getNew \endlink  method for a class:
\code
	PersistenceManager pm;
	pm.registerClass(RTTI::getStreamName<Atom>(), RTTI::getNew<Atom>);
	pm.registerClass(RTTI::getStreamName<Composite>(), RTTI::getNew<Composite>);
\endcode
				Remember to include the <b>baseclasses</b> of each class, too! To
				register all kernel classes, use the
				 \link PersistenceManager::BALL_REGISTER_PERSISTENT_KERNEL_CLASSES BALL_REGISTER_PERSISTENT_KERNEL_CLASSES \endlink  macro.
				@param signature the class signatur
				@param m a dynamic class create method
		*/
		virtual void registerClass(String signature, const CreateMethod	m);

		/**	Create an object of a registered class.
				If the persistence manager has registered a create method for the
				given class signature, the corresponding create method is called
				and its result is returned. If the class is not registered, 0 is
				returned.
				@param	signature the class signature of the object to be created
				@return	void* a pointer to the new object or 0, if the signature is
								not known
				@see		registerClass
		*/
		virtual void* createObject(String signature) const;

		/**	Return the number of registerd classes.
				@return	Size the nubmer of classes contained in the persistence
								manager's internal StringHashMap
		*/
		virtual Size	getNumberOfClasses() const;
		
		/**	Set the output stream for persistent objects.
				@param	s the output stream
		*/
		virtual void setOstream(::std::ostream& s);

		/**	Set the input stream for persistent objects.
				@param	s the input stream
		*/
		virtual void setIstream(::std::istream& s);

		/**	Start the output to a persistent stream.	
				This method write a start marker to the output stream and prepares
				the stream and the persistence manager's internal data structures
				for the output of an object. The start marker is written via the
				method  \link PersistenceManager::writeStreamHeader writeStreamHeader \endlink .
				 \par
				It need not be called usually, as it is called by <tt>operator >></tt>.
		*/
		void startOutput();

		/**	Terminate the output to a persistent stream.
				This method finishes the writing of a persistent object to a
				stream.  In fact, it does most of the work. It checks for the list
				of "missing" objects, i.e. objects that have been referenced via a
				pointer or a reference by the objects written before and calls
				their persistent write methods.  It then writes an end marker to
				the file (via writeStreamTrailer) and clears the pending output
				list.
				 \par
				It need not be called usually, as it is called by <tt>operator >></tt>.
		*/
		void endOutput();

		/**	Read a persistent object from the stream.
				This method tries to read a persistent object from the stream,
				creates the object and all dependend objects, finally demangles all
				pointers and references.
				 \par
				If no object could be read or the format was not correct, a null
				pointer is returned.
				@return	0 if no object could be read, the object's <tt>this</tt>
								pointer otherwise
				\throws Exception::GeneralException
		*/	
		PersistentObject*	readObject();

		/**	Write a persistent object to the stream.
				This method writes a persistent object to a stream.
		*/
		PersistenceManager& operator << (const PersistentObject& object);

		/**	Read a persistent object from a stream.
				This method calls  \link PersistenceManager::readObject readObject \endlink .
		*/
		PersistenceManager& operator >> (PersistentObject*& object_ptr);

		//@}

		/**	@name	Layer 1 methods
		*/
		//@{

		/** Check an object header by determining its stream name by using @see
				RTTI and calling @see checkHeader.
				@param object an Object of type T
				@param name the expected name of the object
				@return true if the object header could be checked successfully
		*/
		template <typename T>
		bool checkObjectHeader(const T& /* object */, const char* name = 0);

		/** Check an object header by supplying its stream name.
				@param	type_name the stream name of the object type
				@return	true if the object header could be checked successfully
		*/
		bool checkObjectHeader(const char* type_name);

		/** Write an object Header.
				Determine the stream name of the object via @see RTTI and call @see
				writeHeader.
				@param	object a const pointer to the object
				@param	name the name of the object
		*/
		template <typename T>
		void writeObjectHeader(const T* object, const char* name = 0);

		/** Write an object trailer by calling @see writeTrailer.
				@param	name the name of the object
		*/
		void writeObjectTrailer(const char* name = 0);

		/** Check an object trailer by calling @see checkTrailer.
				@param	name the name of the object
		*/
		bool checkObjectTrailer(const char* name = 0);

		/**	Write a primitive member variable.
				This method also writes the necessary header and trailer of the
				primitive.
				@param	t the variable
				@param	name the name of the variable
		*/
		template <typename T>
		void writePrimitive(const T& t, const char* name); 

		/**	Read a primitive member variable. 
				This method also checks header and trailer of the primitive.
				@param	t a mutable reference of the primitive variable
				@param	name the expected name of the variable
				@return	true if readng was successful
		*/
		template <typename T>
		bool readPrimitive(T& t, const char* name);

		/**	Write a storable object. 
				This method also writes header and trailer of the object.
				@param	t the storable object
				@param	name the name of the object
		*/
		template <typename T>
		void writeStorableObject(const T& t, const char* name);

		/**	Read a storable object. This method also checks header and trailer
				of the object.
				@param	t a mutable reference of the object
				@param	name the expected name of the object
				@return	true if reading the object was successful.
		*/
		template <typename T>
		bool readStorableObject(T& t, const char* name);

		/**	Write a pointer to a PersistentObject. 
				This method also writes the necessary header and trailer.
				@param object a const pointer to the object we want to write
				@param name the name of the object pointer (the name of the member variable written)
		*/
		template <typename T>
		void writeObjectPointer(const T* object, const char* name);
 
		/**	Read a pointer to a PersistentObject.
				This method also checks header and trailer.
				@param	object a mutable pointer reference we want to read.
				@param	name the name of the object pointer (usually the name of the member variable)
				@return	true if reading was successful
		*/
		template <typename T>
		bool readObjectPointer(T*& object, const char* name);

		/** Read a smart pointer to a PersistentObject.
				This method also checks header and trailer, and registers a 
				shared pointer for later updating when an object with corresponding 
				this pointer has been read.
				@param s_ptr a mutable shared_ptr reference for later update
				@param ptr the pointer stub for the this pointer
		 */
		template <typename T>
		bool readObjectSmartPointer(boost::shared_ptr<T>& s_ptr, const char* name);

		/** Write a reference to a PersistentObject. 
				This method also writes the necessary header and trailer.
				@param	object a const reference to the object
				@param	name the name of the object (usually the name of the member variable)
		*/
		template <typename T>
		void writeObjectReference(const T& object, const char* name);

		/**	Read a reference to a PersistentObject.
				This method also checks header and trailer of the object reference.
				@param	object a mutable reference
				@param	name the name of the object (usually the name of the member variable)
				@return	true if reading was successful
		*/
		template <typename T>
		bool readObjectReference(T& object, const char* name);

		/**	Write an array of persistent objects.
				This method writes <tt>size</tt> persistent objects to the persistent
				stream. It also writes the necessary header and trailer.
				@param	array the array of persistent objects
				@param	name the name (usually the name of the member variable)
				@param	size the number of elements in the array
		*/
		template <typename T>
		void writeObjectArray(const T* array, const char* name, Size size);

		/**	Read an array of persistent objects.
				This method reads <tt>size</tt> persistent objects from the persistent
				stream. It also checks header and trailer of the array.
				@param	array the array of persistent objects
				@param	name the name (usually the name of the member variable)
				@param	size the number of elements in the array
		*/
		template <typename T>
		bool readObjectArray(const T* array, const char* name, Size& size);

		/** Write an array of pointers to persistent objects.
				This method writes <tt>size</tt> persistent objects to the persistent
				stream. It also writes the necessary header and trailer.
				@param  arr the array of persistent object pointers
				@param  name the name (usually the name of the member variable)
				@param  size the number of elements in the array
		*/
		template <typename T>
		void writeObjectPointerArray(T** arr, const char* name, const Size size);
	
		/**	Read an array of persistent object pointers.
				This method reads <tt>size</tt> persistent object pointers from the
				persistent stream. It also checks header and trailer of the array.
				@param	array the array of persistent object pointers
				@param	name the name (usually the name of the member variable)
				@param	size the number of elements in the array
		*/
		template <typename T>
		bool readObjectPointerArray(T** array, const char* name, Size& size);
	 
		//@}

		/**	@name	Layer 0 methods
		*/
		//@{
			
		/**	Write the header for an object.
				This method writes the header information containing the class
				signature, the name and its <tt>this pointer</tt>.  The name
				information is required to differentiate between base classes of an
				object and member objects or the object itself. If writeHeader is
				called for a base class, name should be set to 0. <tt>type_name</tt>
				should refer to the stream name of an object (see
				 \link PersistenceManager::getStreamName getStreamName \endlink ).  \par
				When defining an object (i.e. when writing the first header to a
				persistent stream), <tt>name</tt> should be set to <tt>""</tt>. For base
				classes, name has to be set to 0. The exact behaviour of this
				method is implementation dependend - it is abstract for
				PersistenceManager.
		*/
		virtual void writeHeader(const char* type_name, const char* name,
				LongSize ptr) = 0;

		/**	Check an object header.
				@param	type_name the stream name of the class to be read
				@param	name the expected name of the object 
				@param	ptr a reference to a <tt>PointerSizeUInt</tt> to store the <tt>this</tt> 
								pointer of the object read from the stream
				@return	bool true, if the header was correct, <b>false</b>
								otherwise
				@return	ptr the pointer is set to the value read from the file
		*/
		virtual bool checkHeader(const char* type_name, const char* name,
				LongSize& ptr) = 0;

		/** Write the trailer for an object. 
				@param name the name of the object
		*/
		virtual void writeTrailer(const char* name = 0) = 0;

		/** Check an object trailer.
				This method checks the trailer of an object.
				@param	name the name of the object
				@return	true if the trailer was correct
		*/
		virtual bool checkTrailer(const char* name = 0) = 0;


		/**	Write a start marker to the output stream.
		*/
		virtual void writeStreamHeader() = 0;


		/**	Write an end marker to the output stream.
		*/
		virtual void writeStreamTrailer() = 0;


		/**	Check for the start marker in the input stream.
				@return true if the marker could be checked.
		*/
		virtual bool checkStreamHeader() = 0;


		/**	Check for the end marker in the output stream.
				@return true if the marker could be checked.
		*/
		virtual bool checkStreamTrailer() = 0;


		/**	Get an (unknown) object header.
				The name (if set) is ignored. The type name is returned in <tt>
				type_name</tt> and the address of the object is read but not inserted
				into the table.
		*/
		virtual bool getObjectHeader(String& type_name, LongSize& ptr) = 0;


		/**	Write a variable/member name.
				@param name the name we want to write
		*/
		virtual void writeName(const char* name) = 0;


		/** Check for variable/member name.
				@param	name the name we want to check
				@return	true if the name matches
		*/
		virtual bool checkName(const char* name) = 0;


		/**	Write storable object header.
				@param type_name the stream name of the storable object
				@param name the name of the object
		*/
		virtual void writeStorableHeader(const char* type_name,
				const char* name) = 0;
			
		/**	Check for storable object header.
				@param type_name the stream name of the storable object
				@param name the name of the object
		*/
		virtual bool checkStorableHeader(const char* type_name,
				const char* name) = 0;

		/**	Write the trailer for a storable object.
		*/
		virtual void writeStorableTrailer() = 0;


		/**	Check for the trailer of a storable object.
				@return true if the trailer was correct
		*/
		virtual bool checkStorableTrailer() = 0;


		/**	Write type header and name for a primitive type.
				@param	type_name the stream name of the primitive
				@param	name the name of the primitive
		*/
		virtual void writePrimitiveHeader(const char* type_name,
				const char* name) = 0;

		/**	Check for a type header and name for a primitive type.
				@param	type_name the stream name of the primitive
				@param	name the name of the primitive
				@return	true if type and name of the primitive match
		*/
		virtual bool checkPrimitiveHeader(const char* type_name,
				const char* name) = 0;

		/**	Write the trailer for a primitive type.
		*/
		virtual void writePrimitiveTrailer() = 0;


		/**	Check for the trailer of a primitive type.
				@return true if the trailer was correct
		*/
		virtual bool checkPrimitiveTrailer() = 0;


		/**	Write header for a pointer to a PersistentObject.
				@param	type_name the stream name of the object type
				@param	name the name of the object
		*/
		virtual void writeObjectPointerHeader(const char* type_name,
				const char* name) = 0;


		/**	Check for header for a pointer to a PersistentObject.
				@param	type_name the stream name of the object type
				@param	name the name of the object
				@return	true if the header was correct
		*/
		virtual bool checkObjectPointerHeader(const char* type_name,
				const char* name) = 0;


		/**	Write header for a reference to a PersistentObject.
				@param	type_name the stream name of the object type
				@param	name the name of the object
		*/
		virtual void writeObjectReferenceHeader(const char* type_name,
				const char* name) = 0;


		/**	Check for header for a reference to a PersistentObject.
				@param	type_name the stream name of the object type
				@param	name the name of the object
				@return	true if the header was correct
		*/
		virtual bool checkObjectReferenceHeader(const char* type_name,
				const char* name) = 0;


		/**	Write header for an array of pointers to PersistentObjects.
				@param	type_name the stream name of the object type
				@param	name the name of the object array (?)
				@param	size the size of the array
		*/
		virtual void writeObjectPointerArrayHeader(const char* type_name,
				const char* name, Size size) = 0;


		/**	Check for header for an array of pointers to PersistentObjects.
				@param	type_name the stream name of the object type
				@param	name the name of the object array (?)
				@param	size the size of the array
				@return	true if the header was correct
		*/
		virtual bool checkObjectPointerArrayHeader(const char* type_name,
				const char* name, Size& size) = 0;


		/**	Write trailer for an array of pointers to PersistentObjects.
		*/
		virtual void writeObjectPointerArrayTrailer() = 0;


		/**	Check for trailer for an array of pointers to PersistentObjects.
				@return	true if the trailer was correct
		*/
		virtual bool checkObjectPointerArrayTrailer() = 0;


		/**	Prepare the output stream for output.
		*/
		virtual void initializeOutputStream();


		/**	Prepare the output stream for closing.
		*/
		virtual void finalizeOutputStream();


		/**	Prepare the input stream for reading.
		*/
		virtual void initializeInputStream();


		/**	Prepare the input stream for closing.
		*/
		virtual void finalizeInputStream();

		//@}

		/**	@name	Put methods for primitive data types.
				Persistence in BALL supports the following predefined data types:
				\begin{tabular}{lcc}
					Name & signed/unsigned & Size (in bit) \par
					\hline
					char & signed & 8 \par
					bool & - & 1 \par
					Byte & unsigned & 8 \par
					Index & signed & 32 \par
					Size/Position & unsigned & 32 \par
					LongSize & unsigned & 64 \par
					float & signed & 32 \par
					double & signed & 64 \par
					long double & signed & 128 \par
					String & - & -
				\end{tabular}
		*/
		//@{
		
		/**	Write a signed char to the output stream.
		*/
		virtual void put(const char c) = 0;

		/**	Write a single byte to the output stream.
		*/
		virtual void put(const Byte c) = 0;

		/**	Write an Index to the output stream.
		*/
		virtual void put(const Index i) = 0;

		/**	Write a Position or a Size to the output stream.
		*/
		virtual void put(const Size p) = 0;

		/**	Write a boolean value to the output stream.
		*/
		virtual void put(const bool b) = 0;

		/**	Write a single precision floating point number to the output stream.
		*/
		virtual void put(const Real f) = 0;

		/**	Write a double precision floating point number to the output stream.
		*/
		virtual void put(const DoubleReal d) = 0;

		/**	Write a string to the output.
		*/
		virtual void put(const string& s) = 0;

		/**	Write a pointer to the output.
		*/
		virtual void put(const LongSize p) = 0;

		//@}

		/**	@name	Get methods for primitive data types.
		*/
		//@{

		/**	Read a signed char from the input stream.
		*/
		virtual void get(char& c) = 0;

		/**	Read a single Byte from the input stream.
		*/
		virtual void get(Byte& b) = 0;

		/**	Read an Index from the input stream.
		*/
		virtual void get(Index& s) = 0;

		/**	Read a Size or a Position from the input stream.
		*/
		virtual void get(Size& s) = 0;

		/**	Read a boolean value from the input stream.
		*/
		virtual void get(bool& b) = 0;

		/**	Read a single precision floating point number from the input stream.
		*/
		virtual void get(Real& f) = 0;

		/**	Read a double precision floating point number from the input stream.
		*/
		virtual void get(DoubleReal& d) = 0;

		/**	Read a string from the output stream.
		*/
		virtual void get(string& s) = 0;

		/**	Read a 64-bit pointer from the input stream.
		*/
		virtual void get(LongSize& p) = 0;

		//@}

		protected:

		/*_	Register all BALL kernel classes.
				This method is automatically called in the constructor.
		*/
		void registerKernelClasses_();

		/*_
		*/
		void addPointerPair_(LongSize old_ptr, void* new_ptr);
				
		/*_
		 * \throws Exception::GeneralException
		*/
		void addNeededObjects_();

		/*_
		*/
		bool updatePointers_();

		/*_
		*/
		typedef	HashSet<const PersistentObject*>			ObjectSet;
		
		/*_
		*/
		typedef	std::list<const PersistentObject*>		ObjectList;
		
		/*_
		*/
		typedef	HashMap<LongSize, void*>				PointerMap;
		
		/*_
		*/
		typedef	std::list<std::pair<void**, LongSize> >		PointerList;

		/*_
		*/
		typedef	std::list<std::pair<boost::shared_ptr<PersistentObject>*, LongSize> >	SmartPointerList;

		/*_
		*/
		StringHashMap<CreateMethod>		create_methods_;

		/*_ a hash set containing the pointers of the 
				objects that were already written
		*/
		ObjectSet		object_out_;

		/*_ a list of object pointers that were referenced
				by objects already written, but have not yet
				been written themselves
		*/
		ObjectList	object_out_needed_;

		/*_ a map relating the pointers read from the stream (LongSize)
				with the pointers of the persistent objects that were created
				dynamically
		*/
		PointerMap	pointer_map_;
		
		//_
		PointerList	pointer_list_;

		//_
		SmartPointerList smart_pointer_list_;

		//_
		ObjectList	object_in_;

		//_
		::std::ostream*	ostr_;
		//_
		::std::istream*	istr_;
	};


	// implementation of templated methods
	
	template <typename T>
	bool PersistenceManager::checkObjectHeader(const T& /* object */,
			const char* name)
	{
		LongSize ptr;
		return checkHeader(RTTI::getStreamName<T>(), name, ptr);
	}


	template <typename T>
	void PersistenceManager::writeObjectHeader(const T* object,
			const char* name)
	{
		object_out_.insert(object);
		writeHeader(RTTI::getStreamName<T>(), name, (LongSize)reinterpret_cast<PointerSizeUInt>(object));
	}


	template <typename T>
	void PersistenceManager::writePrimitive(const T& t, const char* name)
	{
		writePrimitiveHeader(RTTI::getStreamName<T>(), name);
		put(t);
		writePrimitiveTrailer();
	}


	template <typename T>
	bool PersistenceManager::readPrimitive(T& t, const char* name)
	{
		if (!checkPrimitiveHeader(RTTI::getStreamName<T>(), name))
		{
			return false;
		}

		get(t);
		return checkPrimitiveTrailer();
	}


	template <typename T>
	void PersistenceManager::writeStorableObject(const T& t, const char* name)
	{
		writeStorableHeader(RTTI::getStreamName<T>(), name);
		t.write(*this);
		writeStorableTrailer();
	}


	template <typename T>
	bool PersistenceManager::readStorableObject(T& t, const char* name)
	{
		return (checkStorableHeader(RTTI::getStreamName<T>(), name) 
						&& t.read(*this) && checkStorableTrailer());
	}


	template <typename T>
	void PersistenceManager::writeObjectPointer(const T* object, const char* name)
	{
		if (object != 0 && !object_out_.has(object))
		{
			object_out_needed_.push_back(object);
		}

		writeObjectPointerHeader(RTTI::getStreamName<T>(), name);
		put(static_cast<LongSize>(reinterpret_cast<PointerSizeUInt>(object)));
		writePrimitiveTrailer();
	}


	template <typename T>
	bool PersistenceManager::readObjectPointer(T*& object, const char* name)
	{
		if (!checkObjectPointerHeader(RTTI::getStreamName<T>(), name))
		{
			return false;
		}

		LongSize ptr;
		get(ptr);

		if (ptr != 0)
		{
			pointer_list_.push_back(std::make_pair((void**)&object, ptr));
		}

		object = reinterpret_cast<T*>(static_cast<PointerSizeUInt>(ptr));

		return checkPrimitiveTrailer();
	} 

	template <typename T>
	bool PersistenceManager::readObjectSmartPointer(boost::shared_ptr<T>& s_ptr, const char* name)
	{
		if (!checkObjectPointerHeader(RTTI::getStreamName<T>(), name))
		{
			return false;
		}

		LongSize ptr;
		get(ptr);

		if (ptr != 0)
		{
			smart_pointer_list_.push_back(std::make_pair((boost::shared_ptr<PersistentObject>*)&s_ptr, (LongSize)((PersistentObject*)ptr)));
		}

		return checkPrimitiveTrailer();
	} 

	template <typename T>
	void PersistenceManager::writeObjectReference(const T& object,
			const char* name)
	{
		if (&object != 0 && !object_out_.has(&object))
		{
			object_out_needed_.push_back(&object);
		}

		writeObjectReferenceHeader(RTTI::getStreamName<T>(), name);
		put((LongSize)(void*)&object);
		writePrimitiveTrailer();
	} 


	template <typename T>
	bool PersistenceManager::readObjectReference(T& object, const char* name)
	{
		if (!checkObjectReferenceHeader(RTTI::getStreamName<T>(), name))
		{
			return false;
		}

		LongSize ptr;
		get(ptr);

		// store a zero in the corresponding pointer
		// since we cannot convert 64 bit pointers to
		// 32 bit pointers - this is required, if an object
		// written on a 64 bit architecture is read on a 32 bit
		// machine
		object = 0;

		if (ptr != 0)
		{
			pointer_list_.push_back(std::make_pair((void**)&object, ptr));
		}

		return checkPrimitiveTrailer();
	}

	template <typename T>
	void PersistenceManager::writeObjectArray(const T* array, const char* name,
			Size size)
	{
		writeObjectPointerArrayHeader(RTTI::getStreamName<T>(), name, size);

		for (Position i = 0; i < size; i++)
		{
			(*this) << array[i];
		}

		writeObjectPointerArrayTrailer();
	}

	template <typename T>
	bool PersistenceManager::readObjectArray
		(const T* array, const char* name, Size& size)
	{
		if (!checkObjectPointerArrayHeader(RTTI::getStreamName<T>(), name, size))
		{
			return false;
		}

		T* ptr = const_cast<T*>(array);
		for (Position i = 0; i < size; i++) 
		{
			(*this) >> ptr[i];
		}

		bool result = checkObjectPointerArrayTrailer();
		return result;
	} 

	template <typename T>
	void PersistenceManager::writeObjectPointerArray
		(T** arr, const char* name, const Size size)
	{
		writeObjectPointerArrayHeader(RTTI::getStreamName<T>(), name, size);

		PersistentObject* ptr;
		for (Position i = 0; i < size; i++)
		{
			ptr = (PersistentObject*)arr[i];
			put(static_cast<LongSize>(reinterpret_cast<PointerSizeUInt>(ptr)));
			if (ptr != 0 && !object_out_.has(ptr))
			{
				object_out_needed_.push_back(ptr);
			}
		}
		
		writeObjectPointerArrayTrailer();
	}


	template <typename T>
	bool PersistenceManager::readObjectPointerArray(T** array, const char* name,
			Size& size)
	{
		if (!checkObjectPointerArrayHeader(RTTI::getStreamName<T>(), name, size))
		{
			return false;
		}

		LongSize ptr;
		for (Position i = 0; i < size; i++) 
		{
			get(ptr);

			if (ptr != 0)
			{
				pointer_list_.push_back(std::make_pair((void**)&(array[i]), ptr));
			}

			array[i] = reinterpret_cast<T*>(static_cast<PointerSizeUInt>(ptr));
		}

		return checkObjectPointerArrayTrailer();
	}

#ifndef BALL_NO_INLINE_FUNCTIONS
#	include <BALL/CONCEPT/persistenceManager.iC>
#endif

} // namespace BALL

#endif // BALL_CONCEPT_PERSISTENCEMANAGER_H
