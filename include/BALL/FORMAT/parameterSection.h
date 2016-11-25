// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: parameterSection.h,v 1.25 2005/12/23 17:01:46 amoll Exp $
//

// Format: general  parameter section class

#ifndef BALL_FORMAT_PARAMETERSECTION_H
#define BALL_FORMAT_PARAMETERSECTION_H

#ifndef BALL_DATATYPE_OPTIONS_H
# include <BALL/DATATYPE/options.h>
#endif

namespace BALL 
{
	class Parameters;

	/**	General Parameter Section Class.
			It is similar to  \link INIFile INIFile \endlink , but a little bit more sophisticated.
			Just as in a INIFile there can be comment lines starting with either 
			"<b>;</b>", "<b>!</b>" or "<b>\#</b>" and it is divided in sections.
			A section-line looks like "<b>[ElectricFieldEffect]</b>".
			The first non-comment line in a section is the format-line.
			It defines the sort of values stored in the section and their order:  \par
			"<b>key:residue key:atom value:charge </b>"  \par
			Option lines start with a "<b>@ </b>":  \par
			"<b>@exclude_residue_field=true </b>"  \par
			The values are stored in fields seperated by whitespaces in the order given
			by their format-line:  \par
			"<b>    Ca       Ca        9.6 </b>"  \par
		
			Each value line may contain version information, indicated by a variable 
			definition named "ver"; thus only the latest version will be version will be accessed.
			
    	\ingroup  General
	*/
	class BALL_EXPORT ParameterSection 
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
		ParameterSection() ;

		/** Copy constructor 
		*/
		ParameterSection(const ParameterSection& parameter_section) ;

		/**	Destructor.
		*/
		virtual ~ParameterSection() ;

		//@}	
		/**	@name	Extracting Data from the Section
		*/
		//@{

		/**	Reads a parameter section from an INI file.
				This method reads the section given in section_name from ini_file,
				interprets (if given) a format line, reads the data from this section according to 
				the format, and builds some datastructures for fast and easy acces to the data.
				@param	parameters the parameters defining the inifile to be read from
				@param	section_name the name of the section to be read (without the squared brackets)
				@return bool - <b>true</b> if the section could be read, <b>false</b> otherwise
		*/
		bool extractSection(Parameters& parameters, const String& section_name)
			;

		/**	Return the name of the section read.
				The section name is empty before  \link extractSection extractSection \endlink  was called.
				@return the	name of the section extracted
		*/
		const String& getSectionName() const
			;

		/** Returns the value associated with the key and returns the value of the
				given variable.
				If the requested variable is not defined in the format line, an empty string is returned.
		*/
		const String& getValue(const String& key, const String& variable) const
			;
		
		/**	Query for a pair of key and variable.
				False is returned if 

					- the key could not be found
					- the the key was found but the variable name was not specified
								in the format line
				
		*/
		bool has(const String& key, const String& variable) const ;
		
		/**	Query for a key.
				False is returned if the key could not be found.
				@param key the key to serch in the hash table
		*/
		bool has(const String& key) const ;
		
		/**	Query whether a specified variable was defined in the format line.
		*/
		bool hasVariable(const String& variable) const ;

		/**	Return the column index of a variable.
				If section doesn't have the variable, INVALID_Position
				is returned.
		*/
		Position getColumnIndex(const String& variable) const ;

		/**	Returns the number of defined variables.
		*/
		Size getNumberOfVariables() const ;

		/**	Returns the number of different keys defined.
		*/
		Size getNumberOfKeys() const ;

		/**	Fast access to the value array.
				If the value is undefined,  \link UNDEFINED UNDEFINED \endlink  is returned.
		*/
		const String& getValue(Position key_index, Position variable_index)
			const ;

		/**	Fast access to the key array.
				The first key has the index 0.
				If the key is undefined,  \link UNDEFINED UNDEFINED \endlink  is returned.
		*/
		const String& getKey(Position key_index) const ;

		//@}
		/** @name Assignment 
		*/
		//@{

		/**	Clear method.  
		*/
		virtual void clear() 
			;

		/** Assignment operator 
		*/
		const ParameterSection& operator = (const ParameterSection& section)
			;

		//@}
		/**	@name	Predicates
		*/
		//@{
			
		/**	Validity predicate
		*/
		virtual bool isValid() const ;

		/** Equality operator 
		*/
		bool operator == (const ParameterSection& parameter_section) const
			;

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
