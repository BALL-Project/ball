// $Id: parameterSection.h,v 1.13 2001/05/14 19:20:44 amoll Exp $
// Format: general  parameter section class

#ifndef BALL_FORMAT_PARAMETERSECTION_H
#define BALL_FORMAT_PARAMETERSECTION_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_DATATYPE_OPTIONS_H
# include <BALL/DATATYPE/options.h>
#endif

namespace BALL 
{

	class Parameters;

	/**	General Parameter Section Class.
			It is similar to \Ref{INIFile}, but a little bit more sophisticated.
			Just as in a INIFile there can be comment lines starting with either 
			"{\bf ;}", "{\bf !}" or "{\bf \#}" and it is divided in sections.
			A section-line looks like "{\bf [ElectricFieldEffect]}".
			The first non-comment line in a section is the format-line.
			It defines the sort of values stored in the section and their order: \\
			"{\bf key:residue key:atom value:charge }" \\
			Option lines start with a "{\bf @ }": \\
			"{\bf @exclude_residue_field=true }" \\
			The values are stored in fields seperated by whitespaces in the order given
			by their format-line: \\
			"{\bf     Ca       Ca        9.6 }" \\		
			Each value line may contain version information, indicated by a variable 
			definition named "ver"; thus only the latest version will be version will be accessed.
			{\bf Definition:} \URL{BALL/FORMAT/ParameterSection.h} \\
	*/
	class ParameterSection 
	{
		public:

		BALL_CREATE(ParameterSection)


		/**	Result Type for unknown items.
		*/
		static const String UNDEFINED;


		/**	@name Enums
		*/
		//@{

		///
		enum 
		{
			MAX_FIELDS = 20
		};

		//@}
		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
		*/
		ParameterSection() throw();

		/** Copy constructor 
		*/
		ParameterSection(const ParameterSection& parameter_section) throw();

		/**	Destructor.
		*/
		virtual ~ParameterSection() throw();

		//@}	
		/**	@name	Extracting Data from the Section
		*/
		//@{

		/**	Reads a parameter section from an INI file.
				This method reads the section given in section_name from ini_file,
				interprets (if given) a format line, reads the data from this section according to 
				the format, and builds some datastructures for fast and easy acces to the data.
				@param	ini_file the inifile to be read from
				@param	section_name the name of the section to be read (without the squared brackets)
				@return bool - {\bf true} if the section could be read, {\bf false} otherwise
		*/
		bool extractSection(Parameters& parameters, const String& section_name)
			throw();

		/**	Return the name of the section read.
				The section name is empty before \Ref{extractSection} was called.
				@return the	name of the section extracted
		*/
		const String& getSectionName() const
			throw();

		/** Returns the value associated with the key and returns the value of the
				given variable.
				If the requested variable is not defined in the format line, an empty string is returned.
		*/
		const String& getValue(const String& key, const String& variable) const
			throw();
		
		/**	Query for a pair of key and variable.
				False is returned if 
				\begin{itemize}
					\item the key could not be found
					\item the the key was found but the variable name was not specified
								in the format line
				\end{itemize}
		*/
		bool has(const String& key, const String& variable) const throw();
		
		/**	Query for a key.
				False is returned if the key could not be found.
				@param key the key to serch in the hash table
		*/
		bool has(const String& key) const throw();
		
		/**	Query whether a specified variable was defined in the format line.
		*/
		bool hasVariable(const String& variable) const throw();

		/**	Return the column index of a variable.
				If section doesn't have the variable, INVALID_POSITION
				is returned.
		*/
		Position getColumnIndex(const String& variable) const throw();

		/**	Returns the number of defined variables.
		*/
		Size getNumberOfVariables() const throw();

		/**	Returns the number of different keys defined.
		*/
		Size getNumberOfKeys() const throw();

		/**	Fast access to the value array.
				If the value is undefined, \Ref{UNDEFINED} is returned.
		*/
		const String& getValue(Position key_index, Position variable_index)
			const throw();

		/**	Fast access to the key array.
				The first key has the index 0.
				If the key is undefined, \Ref{UNDEFINED} is returned.
		*/
		const String& getKey(Position key_index) const throw();

		//@}
		/** @name Assignment 
		*/
		//@{

		/**	Clear method.  
		*/
		virtual void clear() 
			throw();

		/** Assignment operator 
		*/
		const ParameterSection& operator = (const ParameterSection& section)
			throw();

		//@}
		/**	@name	Predicates
		*/
		//@{
			
		/**	Validity predicate
		*/
		virtual bool isValid() const throw();

		/** Equality operator 
		*/
		bool operator == (const ParameterSection& parameter_section) const
			throw();

		//@}
		/**	@name	Public Members
		*/
		//@{

		/**	The options read in from options lines contained in this section.
				Remember: options lines start with "@" as the first character
				and must be of the form "@name=value".
		*/
		Options	options;

		//@}

		protected:

		/*_	The name of the section.
		*/
		String	section_name_;

		/*_	The format line.
		*/
		String	format_line_;

		/*_	String hash map containing an index for each key.
				This index is the index for the entries_ array.
		*/
		StringHashMap<Index>	section_entries_;

		/*_	String has map relating a variable name to the index in entries_.
		*/
		StringHashMap<Index>	variable_names_;
		
		/*_	One-dimensional array of the values read from the section.
				The index of a specific value is calculated as
				section_entries_[key] * number_of_variables_ * variable_names_[name]
		*/
		std::vector<String>					entries_;

		/*_	One-dimensional array of the keys read from the section.
		*/
		std::vector<String>	keys_;

		/*_	The number of variables specified in the format line.
				Variables also include "ver:" entries.
		*/
		Size		number_of_variables_;

		/*_	The version numbers of each key.
		*/
		std::vector<float>	version_;

		/*_	The valid flag.
		*/
		bool		valid_;

	};

} // namespace BALL

#endif // BALL_FORMAT_PARAMETERSECTION_H
