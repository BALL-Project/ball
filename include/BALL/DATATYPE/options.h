// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: options.h,v 1.26.14.1 2007/03/25 21:23:40 oliver Exp $
//

#ifndef BALL_DATATYPE_OPTIONS_H
#define BALL_DATATYPE_OPTIONS_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
#	include <BALL/DATATYPE/string.h>
#endif

#ifndef BALL_DATATYPE_STRINGHASHMAP_H
#	include <BALL/DATATYPE/stringHashMap.h>
#endif

#include <BALL/FORMAT/paramFile.h>

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

#ifndef BALL_CONCEPT_PERSISTENTOBJECT_H
#	include <BALL/CONCEPT/persistentObject.h>
#endif

namespace BALL 
{
	/**		Options class.
				This object is intended to store options for complex
				method calls. These options are stored as key/value pairs
				of  \link String String \endlink . \par
				Using this datastructure, options for force fields ( \link ForceField ForceField \endlink ),
				finit difference Poisson Boltzmann calculations ( \link FDPB FDPB \endlink ), or the 
				results of calculations can be given, retrieved, stored into a file
				and retrieved from a file.  This simplifies the handling of such
				complex parameter sets. \par
				
    		\ingroup  Generic
	*/
	class BALL_EXPORT Options
		: public StringHashMap<String> 
	{
		
		public:

		BALL_CREATE(Options)

		/**	@name	Constants 
		*/
		//@{

		/**	Maximum length for each entry (key + value): 1024 byte.
				@memo
		*/
		static const Size MAX_ENTRY_LENGTH;

		//@}
		/**	@name Constructors and Destructors	
		*/
		//@{			

		/**	Default constructor. Creates a new and empty Options object.
		*/
		Options();

		/**	Copy constructor.	
				Creates a new option table from an existing one.
				@param	options the options to be copied
		*/
		Options(const Options& options);

		/**		Destructor.
					Destructs the option table and frees all 
					allocated memory.
					@see	clear
		*/
		virtual ~Options();

		//@}
		/**@name Assignment
		*/
		//@{
		
		/** Assignment operator
		*/
		const Options& operator = (const Options& options);

		/** Clear method
		*/
		virtual void clear();

		//@}
		/** @name	Predicates
		*/
		//@{

		/**		Returns true, if the value associated with the 
					given key is an integer
					@param	key String
					@return	true, if <b>  key </b> is an integer
		*/
		bool isInteger(const String& key) const;

		/**		Returns true, if the value associated with the 
					given key is a boolean.
					A boolean entry has either the value <tt>true</tt>
					or <tt>false</tt>. 
					@param	key String
					@return	true, if <b>  key </b> is a boolean value
		*/
		bool isBool(const String& key) const;

		/**		Returns true, if the value associated with the 
					given key is a real number
					@param	key String
					@return	true, if <b>  key </b> is a real number
		*/
		bool isReal(const String& key) const;

		/**		Returns true, if the value associated with the given key 
					can be read as a vector.
					This method simply checks whether the value is of the form
					#(<float> <float> <float>)# where #<float>#
					represents an arbitrary floating point number.
					@param					key String
					@return					true, if <b>  </b> key represents a 3D vector of reals
		*/
		bool isVector(const String& key) const;

		/**		Returns true, if a value is defined for the given key.
					@param					key String
					@return					bool, true/false
		*/
		bool isSet(const String& key) const;

		//@}
		/** @name Inspectors and mutators
		*/
		//@{


		void addParameterDescription(const String& key, String description, ParameterType type, list<String>* allowed_values = NULL);

		/* Return the description that was registered for the parameter identified by the given key */
		const ParameterDescription* getParameterDescription(const String& key) const;

		/** Create a new subcategory of options.
		@return a pointer to the newly created subcategory. Note that the Options instance that represents the subcategory belongs to the parent object, so do not delete it manually. */
		Options* createSubcategory(String name);

		/** Search and return a subcategories of options.
		@return a pointer to the newly created subcategory. Note that the Options instance that represents the subcategory belongs to the parent object, so do not delete it manually. \n If no subcategory for the given name is found, a null-pointer is returned. */
		Options* getSubcategory(String name);

		/** Get an iterator to the first subcategory */
		StringHashMap<Options*>::Iterator
		beginSubcategories();

		/** Get an iterator past the last subcategory */
		StringHashMap<Options*>::Iterator
		endSubcategories();

		/**		Sets the option table's name.
		*/
		void setName(const String& name);

		/**		Returns the option table's name.
		*/
		const String& getName() const;

		/**		Returns the value associated with the key.
					If the key does not exists an empty string is returned.
					@param	key the key
					@return	String, the value
		*/
		String get(const String& key) const;

		/**		Returns the value associated with the key as bool.
					This method tries to convert the value associated
					with <b>key</b> to a bool value. \par
					If the value is neither "true", nor "false" or the key does
					not exist false is returned!
					You might want to check the value's validity as a bool first
					by calling  isBool().
					@param	key the key
					@return	bool, boolean value
		*/
		bool	getBool(const String& key) const;

		/**		Returns the value associated with the key as a floting point number.
					If the value could not be converted to a floating point number or the key
					does not exist a value of #0.0# is returned. \par
					You might want to check the value's validity as a floating point 
					number first by calling  isReal().
					@param	key the key
					@return float, floating point value
		*/
		double getReal(const String& key) const;

		/**		Returns the value associated with the key as a Vector3 object.
					This method is useful to read threedimensional coordinates, points, etc.
					The value corresponding to <b>key</b> has to be of the form
					#(<float> <float> <float>)# (i.e. three floating point numbers separated
					by white blanks and surrounded by round brackets). \par
					If the content of the value is of a differnet format or the key does not
					exists, the content	of the returned vector is undefined.
					You might therefore check the value's validity first
					by calling  \link isVector isVector \endlink .
					@param	key the key
					@return	Vector3	vector containing the three coordinates
		*/
		Vector3	getVector(const String& key) const;

		/**		Returns the value associated with the key as an integer.
					If the value corresponding to <b>key</b> could not be converted to an
					integer number or the key does not exists, zero is returned.
					It is possible to check for the validity 
					of this conversion by calling isInteger() .
					@return 	long the integer value
					@param	key the key
		*/
		long getInteger(const String& key) const;

		/** 	Assigns value to key. 
					The string given as <b>value</b> is assigned to the <b>key</b>.
					If <b>key</b> didn't exist in the internal hash table, it 
					is inserted.
					@param key the key
		*/
		void set(const String& key, const String& value);

		/** 	Assigns the real number given by value to key.
					<b>value</b> is first converted to a string, which is 
					then stored in the option object. \par
					If <b>key</b> didn't already exist, it is created.
					@param	value the new value
					@param key the key
		*/
		void setReal(const String& key, const double value);

		/**		Assigns a Vector3 object to the value corresponding to a key.
					The given Vector3 is converted to a string by the following
					call to sprintf: \par
					#sprintf(buffer, "(%f %f %f)", value.x, value.y, value.z)# \par
					The so-built string is then stored in the hash table under key. \par
					If <b>key</b> didn't already exist, it is created.
					@param	key the key
					@param	value a vector
		*/
		void setVector(const String& key, const Vector3& value);

		/** Assigns the integer given by value to key.
				<b>value</b> is first converted to a string, which is 
				then stored in the hash table under key. \par
				If <b>key</b> didn't already exist, it is created. \par
				@param	key the key
				@param	value the integer value
		*/
		void setInteger(const String& key, const long value);

		/**	Assigns the boolean value given by value to the table entry key.
				The value associated with <b>key</b> is either set to
				the string "true" or the string "false". \par
				If <b>key</b> didn't already exist, it is created. \par
				@param	key the key
				@param	value the boolean value
		*/
		void setBool(const String& key, const bool value);

		/**	Assigns the value only, if the key is not yet defined. 
				If an entry for <b>key</b> exists, it is not modified.
				It is set to value otherwise.
				@see	set
				@param	key the key
				@param 	value	a new value
		*/
		String setDefault(const String& key, const String& value);

		/**	Assigns the value only, if the key is not yet defined. 
				If an entry for <b>key</b> exists, it is not modified.
				It is set to value otherwise.
				@see	setInteger
				@param	key the key
				@param 	value	a new value
				@return	the value of <tt>key</tt>
		*/
		long setDefaultInteger(const String& key, const long value);

		/**	Assigns the value only, if the key is not yet defined. 
				If an entry for <b>key</b> exists, it is not modified.
				It is set to value otherwise.
				@see	setReal
				@param	key the key
				@param 	value	a new value
				@return	the value of <tt>key</tt>
		*/
		double setDefaultReal(const String& key, const double value);

		/**	Assigns the value only if the key is not yet defined.
				If an entry for <b>key</b> exists, it is not modified.
				It is set to value otherwise.
				@see	setBool
				@param	key the key
				@param 	value	a new value
				@return	the value of <tt>key</tt>
		*/
		bool setDefaultBool(const String& key, const bool value);

		/**		Reads options from a file. 
					This method opens the file specified by <b>filename</b>.
					If the file could not be opened, the method returns false.
					Otherwise, the file is read line by line and each line is interpreted
					as a key (starting with the first non-blank character and	terminated
					by the next blank) separated by a blank from a value (starting
					with the first non-blank charakter after the key and terminated
					by the end of the line).
					For each line either an existing key is updated with the value read,
					or a new key-value pair is created. \par
					Lines starting with '\#', '!', or ';' are ignored and may be used
					as comments. \par
					@param	filename the name of the file to be read
					@return	bool
													- <b>true</b> if the file could be read
													- <b>false</b> otherwise
												
		*/
		bool readOptionFile(const String& filename);

		/**		Writes options to a file.
					This method opens or creates the file specified by <b>filename</b>.
					If the file could not be opened, the method returns false.
					The option table's name is writen in a line starting with '!'.
					@param	filename the name of the file to write
					@return	bool
													- <b>true</b> if the file could be writen
													- <b>false</b> otherwise
												
					@see readOptionFile
		*/		
		bool writeOptionFile(const String& filename) const;

		/**	Persistent stream writing.
				This method writes the strings using the <tt>writePrimitive</tt> method
				of the PersistenceManager.
				@param pm the persistence manager
		*/
		void write(PersistenceManager& pm) const;

		/** Persistent stream reading.
				This method reads from the
				persistent stream using the <tt>readPrimitive</tt> method
				of the PersistenceManager.
				@param pm the persistence manager
		*/
		bool read(PersistenceManager& pm);

		/// Equality operator
		bool operator == (const Options& option) const;

		/// Inequality operator
		bool operator != (const Options& option) const;


		//@}
		/**@name	Debugging 
		*/
		//@{
		
		/** Dumps the whole content of the object 
		*/
		virtual void dump (std::ostream& s = std::cout, Size depth = 0) const;

		//@}

		protected:

		/*_	The option table's name */
		String		name_;
		StringHashMap<ParameterDescription> descriptions_;
		StringHashMap<Options*> subcategories_;
	};
} // namespace BALL

#endif // BALL_DATATYPE_OPTIONS_H
