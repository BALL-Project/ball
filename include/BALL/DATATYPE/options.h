// $Id: options.h,v 1.6 2000/07/12 19:29:18 oliver Exp $

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

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

namespace BALL 
{

	/**		Options class.
				This object is intended to store options for complex
				method calls. These options are stored as key/value pairs
				of \Ref{String}s.\\
				Using this datastructure, options for force fields (\Ref{ForceField}),
				finit difference Poisson Boltzmann calculations (\Ref{FDPB}), or the results
				of calculations can be given, retrieved, stored into a file and retrieved from a file.
				This simplifies the handling of such complex parameter sets.\\

				{\bf Definition:} \URL{BALL/DATATYPE/options.h}
				\\
	*/
	class Options
		: public StringHashMap<String> 
	{
		
		public:

		/**	@name	Constants */
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
				@memo
		*/
		Options();

		/**	Copy constructor.	
				Creates a new option table from an existing one.
				@param	options the options to be copied
				@param	deep bool, no effect		
				@memo
		*/
		Options(const Options& options, bool deep = true);

		/**		Destructor.
					Destructs the option table and frees all 
					allocated memory.
					@see	clear
					@memo
		*/
		virtual ~Options();

		//@}

		
		/**@name		Predicates
		*/
		//@{
		/**		Returns true, if the value associated with the 
					given key is an integer
					@param	key String
					@return	bool, true/false
					@memo
		*/
		bool isInteger(const String& key) const;

		/**		Returns true, if the value associated with the 
					given key is a a boolean.
					A boolean entry has either the value {\tt true}
					or {\tt false}. 
					@param					key String
					@return					bool, true/false
					@memo
		*/
		bool isBool(const String& key) const;

		/**		returns true, if the value associated with the 
					given key is a real number
					@param					key String
					@return					bool, true/false
					@memo
		*/
		bool isReal(const String& key) const;


		/**		returns true, if the value associated with the given key 
					can be read as a vector.
					This method simply checks whether the value is of the form
					#(<float> <float> <float>)# where #<float>#
					represents an arbitrary floating point number.
					@param					key String
					@return					bool, true/false
					@memo
		*/
		bool isVector(const String& key) const;

		/**		returns true, if a value is defined for the given 
					key
					@param					key String
					@return					bool, true/false
					@memo
		*/
		bool isSet(const String& key) const;

		//@}

		/**@name Inspectors and mutators
		*/
		//@{

		/**		Sets the option table's name.
					@memo
		*/
		void setName(const String& name);

		/**		Returns the option table's name.
					@memo
		*/
		const String& getName() const;

		
		/**		Returns the value associated with the key.
					@param	key the key
					@return	String, the value
					@memo
		*/
		String get(const String& key) const;

		/**		Returns the value associated with the key as bool.
					This method tries to convert the value associated
					with {\bf key} to a bool value.\\
					If the value is neither "true", nor "false", false is returned!
					You might want to check the value's validity as a bool first
					by calling \Ref{isBool}.
					@param	key the key
					@return	bool, boolean value
					@memo
		*/
		bool	getBool(const String& key) const;

		/**		Returns the value associated with the key as a floting point number.
					If the value could not be converted to a floating point number, a
					value of #0.0# is returned.\\
					You might want to check the value's validity as a floating point 
					number first by calling \Ref{isReal}.
					@param	key the key
					@return float, floating point value
					@memo
		*/
		double getReal(const String& key) const;

		/**		Returns the value associated with the key as a Vector3 object.
					This method is useful to read threedimensional coordinates, points, etc.
					The value corresponding to {\bf key} has to be of the form
					#(<float> <float> <float>)# (i.e. three floating point numbers separated
					by white blanks and surrounded by round brackets).\\
					If the content of the value is of a differnet format, the content
					of the returned vector is undefined. You might therefore check 
					the value's validity first by calling \Ref{isVector}.
					@param	key the key
					@return	Vector3	vector containing the three coordinates
					@memo
		*/
		Vector3	getVector(const String& key) const;

		/**		Returns the value associated with the key as an integer.
					If the value corresponding to {\bf key} could not be converted to an
					integer number, zero is returned. It is possible to check for the validity 
					of this conversion by calling \Ref{isInteger}.
					@return 	long the integer value
					@param	key the key
					@memo
		*/
		long getInteger(const String& key) const;

		/** 	Assigns value to key. 
					The string given as {\bf value} is assigned to the {\bf key}.
					If {\bf key} didn't exist in the internal hash table, it 
					is inserted.
					@param key the key
					@memo
		*/
		void set(const String& key, const String& value);

		/** 	Assigns the real number given by value to key.
					{\bf value} is first converted to a string, which is 
					then stored in the option object.\\	
					If {\bf key} didn't already exist, it is created.
					@param	value the new value
					@param key the key
					@memo
		*/
		void setReal(const String& key, const double value);

		/**		Assigns a Vector3 object to the value corresponding to a key.
					The given Vector3 is converted to a string by the following
					call to sprintf:\\
					#sprintf(buffer, "(%f %f %f)", value.x, value.y, value.z)#\\
					The so-built string is then stored in the hash table under key.\\
					If {\bf key} didn't already exist, it is created.
					@param	key the key
					@param	value a vector
					@memo
		*/
		void setVector(const String& key, const Vector3& value);

		/** Assigns the integer given by value to key.
				{\bf value} is first converted to a string, which is 
				then stored in the hash table under key.\\
				If {\bf key} didn't already exist, it is created.\\
				@param	key the key
				@param	value the integer value
					@memo
		*/
		void setInteger(const String& key, const long value);

		/**	Assigns the boolean value given by value to the table entry key.
				The value assocaited with {\bf key} is either set to
				the string "true" or the string "false".\\
				If {\bf key} didn't already exist, it is created.\\	
				@param	key the key
				@param	value the boolean value
					@memo
		*/
		void setBool(const String& key, const bool value);



		/**	Assigns the value only, if the key is not yet defined. 
				If an entry for {\bf key} exists, it is not modified.
				It is set to value otherwise.
				@see	set
				@param	key the key
				@param 	value	a new value
				@memo
		*/
		String setDefault(const String& key, const String& value);

		/**	Assigns the value only, if the key is not yet defined. 
				If an entry for {\bf key} exists, it is not modified.
				It is set to value otherwise.
				@see	setInteger
				@param	key the key
				@param 	value	a new value
				@return	the value of {\tt key}
				@memo
		*/
		long setDefaultInteger(const String& key, const long value);

		/**	Assigns the value only, if the key is not yet defined. 
				If an entry for {\bf key} exists, it is not modified.
				It is set to value otherwise.
				@see	setReal
				@param	key the key
				@param 	value	a new value
				@return	the value of {\tt key}
				@memo
		*/
		double setDefaultReal(const String& key, const double value);


		/**	Assigns the value only if the key is not yet defined.
				If an entry for {\bf key} exists, it is not modified.
				It is set to value otherwise.
				@see	setBool
				@param	key the key
				@param 	value	a new value
				@return	the value of {\tt key}
				@memo
		*/
		bool setDefaultBool(const String& key, const bool value);


		/**		Reads options from a file. 
					This method opens the file specified by {\bf filename}.
					If the file could not be opened, the method returns with false.
					Otherwise, the file is read line by line and each line is interpreted
					as a key (starting with the first non-blank character and	terminated
					by the next blank) separated by a blank from a value (starting
					with the first non-blank charakter after the key and terminated
					by the end of the line).
					For each line either an existing key is updated with the value read,
					or a new key-value pair is created.\\
					lines starting with '\#', '!', or ';' are ignored and may be used
					as comments.\\
					@param	filename the name of the file to be read
					@return	bool \begin{itemize}
													\item {\bf true} if the file could be read
													\item {\bf false} otherwise
												\end{itemize}
					@memo
		*/
		bool readOptionFile(const String& filename);

		//@}

		/**@name	Debugging */
		//@{
		
		/** 	Dumps the whole content of the object */
		virtual void dump (std::ostream& s = std::cout, Size depth = 0) const;


		//@}

		private:
		/*_	The option table's name */
		String		name_;
	};

} // namespace BALL

#endif // BALL_DATATYPE_OPTIONS_H
