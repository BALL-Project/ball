// $Id: persistenceManager.h,v 1.18 2000/09/18 14:34:37 oliver Exp $

#ifndef BALL_CONCEPT_PERSISTENCE_H
#define BALL_CONCEPT_PERSISTENCE_H

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

#include <fstream>
#include <iomanip>

using std::pair;

namespace BALL 
{

	/**	Persistence manager class.
			This class serializes and deserializes persistent objects and
			provides support for the implementation of the object-specific 
			serialization methods (\Ref{persistentRead} and \Ref{persistentWrite}).
			It defines three different layers:
			\begin{itemize}
				\item {\bf Layer 0} contains the basic I/O routines for primitive
					data types. All methods of layer 0 are virtual to exchange the 
					implementation of the format-independent storage and retrieval of
					this data (e.g. XDR or text format)
				\item {\bf Layer 1} implements the methods needed to store objects or
					more complex data structures using Layer 0. To implement object persistence
					for a user defined object, methods from layer 0 and layer 1 are needed
				\item {\bf Layer 2} implements the persistence manager's {\em user interface}.
					These are the methods needed to register classes, set the associated streams,
					and to store or retrieve objects.
			\end{itemize}
			@see	PersistentObject
			@see	XDRPersistenceManager
			@see	TextPersistenceManager
	*/		
	class PersistenceManager
	{
		public:

		/**	@name	Type Definitions
		*/
		//@{
			
		/**	Create method type.
				This type describes a method to dynamically create a specific object.
				It should return a {\tt void} pointer for interface compatibility and doesn't take an argument.
				It creates a new object and returns the object's {\tt this} pointer (cast to {\tt void*}).
				The \Ref{getNew} function (in the RTTI namespace) is an example for such a method.
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

		/**	Detailed constructor with an input stream.
				Creates a persistence manager object and assigns an input stream.
		*/
		PersistenceManager(std::istream& is);
			
		/**	Detailed constructor with an output stream.
				Creates a persistence manager object and assigns an output stream.
		*/
		PersistenceManager(std::ostream& os);
			
		/**	Detailed constructor with an input stream and an output stream.
				Creates a persistence manager object and assigns an input stream and an output stream.
		*/
		PersistenceManager(std::istream& is, std::ostream& os);
			
		/**	Destructor.
				Destruct the persistence manager and and clear up all data structures.
				The associated streams or sockets (\Ref{setIStream}/\Ref{setOStream}) are
				not closed.
		*/
		virtual ~PersistenceManager();
		//@}


		/**	@name Layer 2 commands
		*/
		//@{

		/**	Register a create method for a class.
				Each object read by the persistence manager has to be constructed somehow.
				The persistence manager first reads a class' signature (i.e. a unique identifier
        in the context of this stream). This is usually the stream name of the class 
        (see \Ref{getStreamName}), but can be an arbitrary string (without blanks).
        When reading an object header with a given class signature, the persistence manager
        tries to find a method to create an instance of this object. For this purpose it
        contains a StringHashMap object. Each of the classes to be read has to be contained 
        in this hash map together with a method to create an instance of this object.
        This is done by calling registerClass. The create method is usually the \Ref{getNew}
        method for a class:
\begin{verbatim}
	PersistenceManager pm;
	pm.registerClass(RTTI::getStreamName<Atom>(), RTTI::getNew<Atom>);
	pm.registerClass(RTTI::getStreamName<Composite>(), RTTI::getNew<Composite>);
\end{verbatim}
				Remember to include the {\em baseclasses} of each class, too! To register all 
				kernel classes, use the \Ref{BALL_REGISTER_PERSISTENT_KERNEL_CLASSES} macro.
				@param signature the class signatur
				@param m a dynamic class create method
		*/
		virtual void registerClass(String signature, const CreateMethod	m);

		/**	Create an object of a registered class.
				If the persistence manager has registered a create method for the given
			  class signature, the corresponding create method is called and its result is
				returned. If the class is not registered, 0 is returned.
				@param signature the class signature of the object to be created
				@return void* a pointer to the new object or 0, if the signature is not known
				@see registerClass
		*/
		virtual void* createObject(String signature) const;

		/**	Return the number of registerd classes.
				@return Size the nubmer of classes contained in the persistence manager's internal StringHashMap
		*/
		virtual Size	getNumberOfClasses() const;
		
		/**	Set the output stream for persistent objects.
				@param	s the output stream
		*/
		virtual void setOstream(std::ostream& s);

		/**	Set the input stream for persistent objects.
				@param	s the input stream
		*/
		virtual void setIstream(std::istream& s);

		/**	Start the output to a persistent stream.	
				This method write a start marker to the output stream and
				prepares the stream and the persistence manager's internal data structures
				for the output of an object. The start marker is written via the method 
				\Ref{writeStreamHeader}.\\
				It need not be called usually, as it is called by {\tt operator >>}.
		*/
		void startOutput();

		/**	Terminate the output to a persistent stream.
				This method finishes the writing of a persistent object to a stream.
				In fact, it does most of th work. It checks for the list of "missing"
				objects, i.e. objects that have been referenced via a pointer or a reference by 
				the objects written before and calls their persistent write methods.
				It then writes and end marker to the file (via writeStreamTrailer)
				and clears the pending output list.\\
				It need not be called usually, as it is called by {\tt operator >>}.
		*/
		void endOutput();

		/**	Read a persistent object from the stream.
				This method tries to read a persistent object from the stream, creates
				the object and all dependend objects, finally demangles all pointers and
				references.\\
				If no object could be read or the format was not correct, a null pointer 
				is returned.
				@return 0 if no object could be read, the object's {\tt this} pointer otherwise
		*/	
		PersistentObject*	readObject();

		/**	Write a persistent object to the stream
				This method writes a persistent object to a stream.
		*/
		PersistenceManager& operator << (const PersistentObject& object);

		/**	Read a persistent object from a stream.
				This method calls \Ref{readObject}.
		*/
		PersistenceManager& operator >> (PersistentObject*& object_ptr);
		//@}
			

		/**	@name	Layer 1 methods
		*/
		//@{


		/**
		*/
		template <typename T>
		bool checkObjectHeader(const T& /* object */, const char* name = 0)
		{
			LongPointerType ptr;
			return checkHeader(RTTI::getStreamName<T>(), name, ptr);
		}

		/**
		*/
		bool checkObjectHeader(const char* type_name)
		{
			LongPointerType ptr;
			return checkHeader(type_name, 0, ptr);
		}

		/**
		*/
		template <typename T>
		void writeObjectHeader(const T* object, const char* name = 0)
		{
			object_out_.insert(object);

			writeHeader(RTTI::getStreamName<T>(), name, (LongPointerType)(void*)object);
		}

		/**
		*/
		void writeObjectTrailer(const char* name = 0)
		{
			writeTrailer(name);
		}

		/**
		*/
		bool checkObjectTrailer(const char* name = 0)
		{
			return checkTrailer(name);
		}

		/**	Write a primitive member variable.
				@param	t the variable
				@param	name the variable name
		*/
		template <typename T>
		void writePrimitive(const T& t, const char* name)
		{
			writePrimitiveHeader(RTTI::getStreamName<T>(), name);
			put(t);
			writePrimitiveTrailer();
		}
 
		/**	Read a primitive member variable.
		*/
		template <typename T>
		bool readPrimitive(T& t, const char* name)
		{
			if (!checkPrimitiveHeader(RTTI::getStreamName<T>(), name))
			{
				return false;
			}

			get(t);

			return checkPrimitiveTrailer();
		}

		/**	Write a storable object.
		*/
		template <typename T>
		void writeStorableObject(const T& t, const char* name)
		{
			writeStorableHeader(RTTI::getStreamName<T>(), name);
			t.write(*this);
			writeStorableTrailer();
		}
			
		/**	Read a storable object.
		*/
		template <typename T>
		bool readStorableObject(T& t, const char* name)
		{
			return (checkStorableHeader(RTTI::getStreamName<T>(), name) 
							&& t.read(*this) && checkStorableTrailer());
		}

		/**	Write a pointer to a PersistentObject.
		*/
		template <class T>
		void writeObjectPointer(const T* object, const char* name)
	 	{
			if (object != 0 && !object_out_.has(object))
			{
				object_out_needed_.push_back(object);
			}

			writeObjectPointerHeader(RTTI::getStreamName<T>(), name);
			put((LongPointerType)(void*)object);
			writePrimitiveTrailer();
		}
 
		/**	Read a pointer to a PersistentObject
		*/
		template <class T>
		bool readObjectPointer(T*& object, const char* name)
		{
			if (!checkObjectPointerHeader(RTTI::getStreamName<T>(), name))
			{
				return false;
			}

			LongPointerType ptr;
			get(ptr);

			if (ptr != 0)
			{
				pointer_list_.push_back(pair<void**, LongPointerType>((void**)&object, ptr));
			}

			object = (T*)ptr;

			return checkPrimitiveTrailer();
		} 

		/**
		*/
		template <class T>
		void writeObjectReference(const T& object, const char* name)
		{
			if (&object != 0 && !object_out_.has(&object))
			{
				object_out_needed_.push_back(&object);
			}

			writeObjectReferenceHeader(RTTI::getStreamName<T>(), name);
			put((LongPointerType)(void*)&object);
			writePrimitiveTrailer();
		} 

		/**	Read a reference to a PersistentObject
		*/
		template <class T>
		bool readObjectReference(T& object, const char* name)
		{
			if (!checkObjectReferenceHeader(RTTI::getStreamName<T>(), name))
			{
				return false;
			}

			LongPointerType ptr;
			get(ptr);

			// store a zero in the corresponding pointer
			// since we cannot convert 64 bit pointers to
			// 32 bit pointers - this is required, if an object
			// written on a 64 bit architecture is read on a 32 bit
			// machine
			object = 0;

			if (ptr != 0);
			{
				pointer_list_.push_back(pair<void**, LongPointerType>((void**)&object, ptr));
			}

			return checkPrimitiveTrailer();
		}

		/**	Write an array of persistent objects.
				This method writes {\tt size} persistent objects to the persistent stream.
				@param	array the array of persistent objects
				@param	name the name
				@param	size the number of elements in the array
		*/
		template <class T>
		void writeObjectArray(const T* array, const char* name, Size size)
		{
			writeObjectArrayHeader(RTTI::getStreamName<T>(), name, size);

			for (Position i = 0; i < size; i++)
				(*this).writeObject(array[i], "-");

			writeObjectArrayTrailer();
		}

		/**	Read an array of persistent objects.
				This method reads {\tt size} persistent objects from the persistent stream.
				@param	array the array of persistent objects
				@param	name the name
				@param	size the number of elements in the array
		*/
		template <class T>
		bool readObjectArray(const T* array, const char* name, Size& size)
		{
			if (!checkObjectArrayHeader(RTTI::getStreamName<T>(), name, size))
			{
				return false;
			}

			T* ptr = const_cast<T*>(array);
			for (Position i = 0; i < size; i++) 
			{
				(*this).readObject(ptr[i], "");
			}

			return checkObjectArrayTrailer();
		} 

		/**
		*/
		template <class T>
		void writeObjectPointerArray(T** arr, const char* name, const Size size)
		{
			writeObjectPointerArrayHeader(RTTI::getStreamName<T>(), name, size);

			PersistentObject* ptr;
			for (Position i = 0; i < size; i++)
			{
				ptr = (PersistentObject*)arr[i];
				put((LongPointerType)(void*)ptr);
				if (ptr != 0 && !object_out_.has(ptr))
				{
					object_out_needed_.push_back(ptr);
				}
			}
			
			writeObjectPointerArrayTrailer();
		}
	
		/**
		*/
		template <class T>
		bool readObjectPointerArray(T** array, const char* name, Size& size)
		{
			if (!checkObjectPointerArrayHeader(RTTI::getStreamName<T>(), name, size))
			{
				return false;
			}

			LongPointerType ptr;
			for (Position i = 0; i < size; i++) 
			{
				get(ptr);

				if (ptr != 0)
				{
					pointer_list_.push_back(pair<void**, LongPointerType>((void**)&(array[i]), ptr));
				}

				array[i] = (T*)ptr;
			}

			return checkObjectPointerArrayTrailer();
		}
	 
		//@}

		/**	@name	Layer 0 methods
		*/
		//@{
			
		/**	Write the header for an object.
				This method writes the header information containing the class signature,
				the name and its {\tt this pointer}.
				The name information is required to differentiate between base classes of an object
				and member objects or the object itself. If writeHeader is called for a base class,
				name should be set to 0. {\tt type\_name} should refer to the stream name of 
				an object (see \Ref{getStreamName}).\\
				When defining an object (i.e. when writing the first header to a persistent stream),
				{\tt name} should be set to {\tt ""}. For base classes, name has to be set to 0.
				The exact behaviour of this method is implementation dependend - it is abstract for
				PersistenceManager.
		*/
		virtual void writeHeader(const char* type_name, const char* name, LongPointerType ptr) = 0;

		/**	Check for an object header.
				@param type_name the stream name of the class to be read
				@param name the expected name of the object 
				@param ptr a reference to a {\tt LongPointerType} to store the {\tt this} pointer of the 
								object read from the stream
				@return bool {\bf true}, if the header was correct, {\bf false} otherwise
				@return ptr the pointer is set to the value read from the file
		*/
		virtual bool checkHeader(const char* type_name, const char* name, LongPointerType& ptr) = 0;

		/**
		*/
		virtual void writeTrailer(const char* name = 0) = 0;

		/**
		*/
		virtual bool checkTrailer(const char* name = 0) = 0;



		/**	Write a start marker to the output stream.
		*/
		virtual void writeStreamHeader() = 0;

		/**	Write an end marker to the output stream.
		*/
		virtual void writeStreamTrailer() = 0;

		/**	Check for the start marker in the input stream.
		*/
		virtual bool checkStreamHeader() = 0;

		/**	Check for the end marker in the output stream.
		*/
		virtual bool checkStreamTrailer() = 0;

		/**	Get an (unknown) object header.
				The name (if set) is ignored. The type name is returned in {\tt type\_name}
				and the address of the object is read but not inserted into the table.
		*/
		virtual bool getObjectHeader(String& type_name, LongPointerType& ptr) = 0;



		/**	Write a variable/member name.
		*/
		virtual void writeName(const char* name) = 0;

		/** Check for variable/member name.
		*/
		virtual bool checkName(const char* name) = 0;

		/**	Write storable object header.
		*/
		virtual void writeStorableHeader(const char* type_name, const char* name) = 0;
			
		/**	Check for storable object header.
		*/
		virtual bool checkStorableHeader(const char* type_name, const char* name) = 0;

		/**	Write the trailer for a storable object.
		*/
		virtual void writeStorableTrailer() = 0;

		/**	Check for the trailer of a storable object.
		*/
		virtual bool checkStorableTrailer() = 0;



		/**	Write type header and name for a primitive type.
		*/
		virtual void writePrimitiveHeader(const char* type_name, const char* name) = 0;

		/**	Check for a type header and name for a primitive type.
		*/
		virtual bool checkPrimitiveHeader(const char* type_name, const char* name) = 0;

		/**	Write the trailer for a primitive type.
		*/
		virtual void writePrimitiveTrailer() = 0;

		/**	Check for the trailer of a primitive type.
		*/
		virtual bool checkPrimitiveTrailer() = 0;



		/**	Write header for a pointer to a PersistentObject.
		*/
		virtual void writeObjectPointerHeader(const char* type_name, const char* name) = 0;

		/**	Check for header for a pointer to a PersistentObject.
		*/
		virtual bool checkObjectPointerHeader(const char* type_name, const char* name) = 0;

		/**	Write header for a reference to a PersistentObject.
		*/
		virtual void writeObjectReferenceHeader(const char* type_name, const char* name) = 0;

		/**	Check for header for a reference to a PersistentObject.
		*/
		virtual bool checkObjectReferenceHeader(const char* type_name, const char* name) = 0;

		/**	Write header for an array of pointers to PersistentObjects.
		*/
		virtual void writeObjectPointerArrayHeader(const char* type_name, const char* name, Size size) = 0;

		/**	Check for header for an array of pointers to PersistentObjects.
		*/
		virtual bool checkObjectPointerArrayHeader(const char* type_name, const char* name, Size& size) = 0;

		/**	Write trailer for an array of pointers to PersistentObjects.
		*/
		virtual void writeObjectPointerArrayTrailer() = 0;

		/**	Check for trailer for an array of pointers to PersistentObjects.
		*/
		virtual bool checkObjectPointerArrayTrailer() = 0;
		//@}

		/**	@name	Put methods for primitive data types.
		*/
		//@{
		
		/**	Write a signed char to the output stream.
		*/
		virtual void put(const char c) = 0;

		/**	Write an unsigned char to the output stream.
		*/
		virtual void put(const unsigned char c) = 0;

		/**	Write a signed short to the output stream.
		*/
		virtual void put(const short s) = 0;

		/**	Write an unsigned short to the output stream.
		*/
		virtual void put(const unsigned short s) = 0;

		/**	Write a signed integer to the output stream.
		*/
		virtual void put(const int s) = 0;

		/**	Write an unsigned integer to the output stream.
		*/
		virtual void put(const unsigned int s) = 0;

		/**	Write a signed long to the output stream.
		*/
		virtual void put(const long s) = 0;

		/**	Write an unsigned long to the output stream.
		*/
		virtual void put(const unsigned long s) = 0;

#ifndef BALL_64BIT_ARCHITECTURE
		/**	Write a signed long long to the output stream.
				Available on 32bit machines only!
		*/
		virtual void put(const long long s) = 0;

		/**	Write an unsigned long long to the output stream.
				Available on 32bit machines only!
		*/
		virtual void put(const unsigned long long s) = 0;
#endif

		/**	Write a boolean value to the output stream.
		*/
		virtual void put(const bool b) = 0;

		/**	Write a single precision floating point number to the output stream.
		*/
		virtual void put(const float f) = 0;

		/**	Write a double precision floating point number to the output stream.
		*/
		virtual void put(const double d) = 0;

		/**	Write a string to the output.
		*/
		virtual void put(const string& s) = 0;

		/**	Write a pointer to the output.
		*/
		virtual void put(const void* p) = 0;

		//@}

		/**	@name	Get methods for primitive data types.
		*/
		//@{

		/**	Read a signed char from the input stream.
		*/
		virtual void get(char& c) = 0;

		/**	Read an unsigned char from the input stream.
		*/
		virtual void get(unsigned char& c) = 0;

		/**	Read a signed short from the input stream.
		*/
		virtual void get(short& s) = 0;

		/**	Read an unsigned short from the input stream.
		*/
		virtual void get(unsigned short& s) = 0;

		/**	Read a signed integer from the input stream.
		*/
		virtual void get(int& s) = 0;

		/**	Read an unsigned integer from the input stream.
		*/
		virtual void get(unsigned int& s) = 0;

		/**	Read a signed long from the input stream.
		*/
		virtual void get(long& s) = 0;

		/**	Read an unsigned long from the input stream.
		*/
		virtual void get(unsigned long& s) = 0;

#ifndef BALL_64BIT_ARCHITECTURE
		/**	Read a signed long from the input stream.
				Available on 32bit machines only;
		*/
		virtual void get(long long& s) = 0;

		/**	Read an unsigned long from the input stream.
				Available on 32bit machines only;
		*/
		virtual void get(unsigned long long& s) = 0;
#endif

		/**	Read a boolean value from the input stream.
		*/
		virtual void get(bool& b) = 0;

		/**	Read a single precision floating point number from the input stream.
		*/
		virtual void get(float& f) = 0;

		/**	Read a double precision floating point number from the input stream.
		*/
		virtual void get(double& d) = 0;

		/**	Read a string from the output stream.
		*/
		virtual void get(string& s) = 0;

		/**	Read a pointer from the input stream.
		*/
		virtual void get(void*& p) = 0;
		//@}


		protected:

		/**	Register all BALL kernel classes.
				This method is automatically called in the constructor.
		*/
		void registerKernelClasses_();

		void addPointerPair_(LongPointerType old_ptr, void* new_ptr);
				
		void addNeededObjects_();

		bool updatePointers_();

		typedef	HashSet<const PersistentObject*>			ObjectSet;
		typedef	list<const PersistentObject*>					ObjectList;
		typedef	HashMap<LongPointerType, void*>				PointerMap;
		typedef	list<pair<void**, LongPointerType> >	PointerList;

		StringHashMap<CreateMethod>		create_methods_;

		// a hash set containing the pointers of the 
		// objects that were already written
		ObjectSet		object_out_;

		// a list of object pointers that were referenced
		// by objects already written, but have not yet
		// been written themselves
		ObjectList	object_out_needed_;

		// a map relating the pointers read from the stream (LongPointerType)
		// with the pointers of the persistent objects that were created dynamically
		PointerMap	pointer_map_;
		
		//
		PointerList	pointer_list_;

		//
		ObjectList	object_in_;

		std::ostream*	ostr_;
		std::istream*	istr_;
	};

} // namespace BALL

#endif // BALL_CONCEPT_PERSISTENCE_H
