// $Id: parameterSection.C,v 1.11 2000/10/17 17:17:24 anker Exp $
//

#include <BALL/FORMAT/parameterSection.h>
#include <BALL/FORMAT/parameters.h>

using namespace std;

namespace BALL 
{

	ParameterSection::ParameterSection() throw()
		:	section_name_(""),
			format_line_(""),
			section_entries_(),
			variable_names_(),
			entries_(0),
			keys_(0),
			number_of_variables_(0),
			version_(0),
			valid_(false)
	{
	}


	ParameterSection::ParameterSection(const ParameterSection& parameter_section) 
		throw()
		:	section_name_(parameter_section.section_name_),
			format_line_(parameter_section.format_line_),
			section_entries_(parameter_section.section_entries_),
			variable_names_(parameter_section.variable_names_),
			entries_(parameter_section.entries_),
			keys_(parameter_section.keys_),
			number_of_variables_(parameter_section.number_of_variables_),
			version_(parameter_section.version_),
			valid_(parameter_section.valid_)
	{
	}


	ParameterSection::~ParameterSection() throw()
	{
		clear();
	}


	void ParameterSection::clear() throw()
	{
		// clear the section name
		section_name_ = "";
		format_line_ = "";

		// destroy all hash maps
		section_entries_.clear();
		variable_names_.clear();

		// clear all allocated entries
		entries_.clear();

		// clear all keys
		keys_.clear();

		// delete the version array
		version_.clear();

		// clear the number of entries/variables
		number_of_variables_ = 0;

		// tag this instance as invalid
		valid_ = false;
	}


	const String& ParameterSection::getSectionName() const
		throw()
	{
		return section_name_;
	}


	bool ParameterSection::extractSection(Parameters& parameters, 
			const String& section_name) throw()
 	{
		if (!parameters.isValid())
		{
			return false;
		}
	
		// this instance should be valid only if extractSection didn`t fail
		valid_ = false;

		// store the section name
		section_name_ = section_name;
		
		// check for the existence of the required section
		if (!parameters.getParameterFile().hasSection(section_name))
		{
			return false;
		}

		// count non-comment lines only
		int	number_of_lines = 0;
		String line;
		Position i;

		INIFile&	ini_file = parameters.getParameterFile();

		// loop over all lines and store the format line
		// skip all comment and options lines
		// and count the number of non-comment lines
		Size first_line = ini_file.getSectionFirstLine(section_name);
		Size last_line = ini_file.getSectionLastLine(section_name);

		for (i = first_line; i <= last_line; i++)
		{
			// get the line and remove leading white spaces
			line = *ini_file.getLine(i);
			line.trimLeft();

			// comment lines start with "!" or ";" or "#"
			// option lines start with "@"
			if (line.size() > 0)
			{
				if ((line[0] != ';') && (line[0] != '!') && (line[0] != '#') && (line[0] != '@'))
				{
					// extract format line: the first non-comment/non-option line
					if (number_of_lines == 0)
					{
						format_line_ = line;
					}

					// count non-comment lines
					number_of_lines++;
				}
			}
		}


		// clear the old contents of variable_names_
		variable_names_.clear();

		//
		// interpret the format line
		//
	
		// f contains the fields resulting from a splitQuoted of the format line
		vector<String> f;
		Size number_of_fields = format_line_.split(f, String::CHARACTER_CLASS__WHITESPACE);

		if (number_of_fields == 0)
		{
			Log.error() << "ParameterSection::extractSection: error reading section " << section_name 
				<< " of file " << ini_file.getFilename() << ": wrong number of fields in the format line: " << number_of_fields << endl;
			Log.error() << "FORMAT: " << format_line_ << endl;

			return false;
		}

		// keys is an array containing the fields that will we assembled to the line key
		Index	keys[ParameterSection::MAX_FIELDS];
		Size	number_of_keys = 0;

		// variables is an array containing the fields that represent variables
		Index	variables[ParameterSection::MAX_FIELDS];
		Size number_of_variables = 0;


		// check every field definition 

		// true, if each line contains version information
		// indicated by a variable definition named "ver"
		bool check_version;
		check_version = false;

		for (i = 0; i < (Position)number_of_fields; i++)
		{
			if (f[i].hasPrefix("key:"))
			{
				keys[number_of_keys++] = (Index)i;
			} 
			else if (f[i].hasPrefix("value:")) 
			{
				// check whether a  variable name was given
				String variable_name = f[i].after(":", 0);
				if (variable_name == "")
				{
					Log.error() << "ParameterSection::extractSection: error while reading section "
							<< section_name << ": empty variable name: " << f[i] << endl;	
				}
				else if (variable_names_.has(variable_name))
				{	
					Log.error() << "ParameterSection::extractSection: error while reading section "
							<< section_name << ": duplicate variable name: " << f[i] << endl;	
				}
				else
				{
					// correct definition: store it.
					variable_names_[variable_name] = (Index)number_of_variables;
					variables[number_of_variables++] = (Index)i;
				}
			} 
		}

		check_version = variable_names_.has("ver");

		// store for faster access
		number_of_variables_ = variable_names_.size();
		
		if (number_of_keys == 0)
		{
			return false;
		}

		// allocate space for all entries
		entries_.clear();
		entries_.resize(number_of_lines * number_of_variables);
		
		// clear all former contest of the keys_ array
		keys_.clear();
		section_entries_.clear();
		
		// now extract all non-comment lines
		bool ignore_entry;
		number_of_lines = -1; // skip format line
		for (i = ini_file.getSectionFirstLine(section_name); i <= ini_file.getSectionLastLine(section_name); i++) 
		{
			line = *ini_file.getLine(i);
			line.trimLeft();
			if (line.size() > 0) 
			{
				if ((line[0] != ';') && (line[0] != '!') && (line[0] != '#') && (line[0] != '@')) 
				{
					number_of_lines++;
					// skip format line 
					if (number_of_lines > 0) 
					{
						// now split the line...
						number_of_fields = line.splitQuoted(f);
						
						// assemble the keys
						Size	j;	// loop variable
						String	key = "";
						for (j = 0; j < number_of_keys; j++) 
						{
							key.append(f[keys[j]]);	
							if (j != number_of_keys - 1)
							{
								key.append(" ");
							}
						}
							
						// if another entry with higher version number has already been read, 
						// ignore_entry is set to true.
						ignore_entry = false;

						if (check_version) 
						{
							// check whether we have already read this key
							if (section_entries_.has(key)) 
							{
								// yes, we do - compare version numbers
								float old_version = entries_[section_entries_[key] 
																						* number_of_variables_ 
																						+ variable_names_["ver"]].toFloat();
								float new_version = f[variables[variable_names_["ver"]]].toFloat();
								if (old_version > new_version)	
								{
									ignore_entry = true;
								} 
								else 
								{
									if (old_version == new_version)
									{
										Log.warn() << "ParameterSection: repeated entry with same version number in line " << number_of_lines 
															 << " of section [" << section_name << "] " << endl;
										Log.warn() << "  in file " << ini_file.getFilename() << ":" << endl;
										Log.warn() << " > " << line << endl;
										ignore_entry = true;
									}
								}
							}
						}
						
						if (!ignore_entry)
						{
							// if this key is new, remember it!
							if (!section_entries_.has(key))
							{
								// add the key to the array of keys
								keys_.push_back(key);

								// insert the key into the hash map
								section_entries_[key] = number_of_lines;
							}

							// copy all variable fields to the corresponding array
							for (j = 0; j < (Position)number_of_variables; j++)
							{
								if ((Position)variables[j] < f.size())
								{
									entries_[number_of_lines * number_of_variables_ + j] = f[variables[j]];
								}
							}
						} 
					}
				} 
				else if (line[0] == '@') 
				{
					// we found an option. 
					// options are of the form "@option=value"
					
					// remove the leading '@'
					line.erase(0, 1);
					
					// insert the option
					String option_key		= line.before("=");
					String option_value = line.after("=");
					options.set(option_key, option_value);
				}
			}
		}

		// mark as valid
		valid_ = true;
		
		return true;
	}

	bool ParameterSection::has(const String& key, const String& variable) const 
		throw()
	{
		return section_entries_.has(key) && variable_names_.has(variable);
	}

	bool ParameterSection::has(const String& key) const throw()
	{
		return section_entries_.has(key);
	}

	const String& ParameterSection::getValue(const String& key,	 
			const String& variable) const throw()
	{
		// define a dummy value returned, if a undefined key/variable
		// pair is requested
		static const String undefined("(undefined)");
		
		// check whether the entry exists
		if (!section_entries_.has(key) || !variable_names_.has(variable))
		{
			return undefined;
		}

		// return the value
		return entries_[section_entries_[key] * number_of_variables_ + variable_names_[variable]];
	}
		
	const String& ParameterSection::getValue(Size key_index, 
			Size variable_index) const throw()
	{
		// define a dummy value returned, if a undefined key/variable
		// pair is requested
		static const String undefined("(undefined)");
		
		// check whether the entry exists
		if ((key_index < keys_.size()) 
				&& (variable_index < number_of_variables_))
		{
			// return the section entry corresponding to the key
			return entries_[(section_entries_[keys_[key_index]]) * number_of_variables_ + variable_index];
		} 
		else 
		{
			return undefined;
		}
	}
		
	const String& ParameterSection::getKey(Position key_index) const throw()
	{
		// define a dummy value returned, if an undefined key/variable
		// pair is requested
		static const String undefined("(undefined)");
		
		// check whether the entry exists
		if ((key_index >= section_entries_.size()))
		{
			return undefined;
		}

		// return the value
		return keys_[key_index];
	}

	Size ParameterSection::getNumberOfKeys() const throw()
	{
		return keys_.size();
	}

	Size ParameterSection::getNumberOfVariables() const throw()
	{
		return number_of_variables_;
	}

	bool ParameterSection::hasVariable(const String& variable) const throw()
	{
		return variable_names_.has(variable);
	}

	Position ParameterSection::getColumnIndex(const String& variable) const
		throw()
	{
		if (variable_names_.has(variable))
		{
			return variable_names_[variable];
		} else {
			return 0;
		}
	}


	const ParameterSection& ParameterSection::operator = 
		(const ParameterSection& section) throw()
	{
		options = section.options;
		section_name_ = section.section_name_;
		format_line_ = section.format_line_;
		section_entries_ = section.section_entries_;
		variable_names_ = section.variable_names_;
		entries_ = section.entries_;
		keys_ = section.keys_;
		number_of_variables_ = section.number_of_variables_;
		version_ = section.version_;
		valid_ = section.valid_;

		return *this;
	}


	bool ParameterSection::isValid() const throw()
	{
		return valid_;
	}


	bool ParameterSection::operator == (const ParameterSection& parameter_section)
		const throw()
	{
		return ( (options == parameter_section.options)
				&& (section_name_ == parameter_section.section_name_)
				&& (format_line_ == parameter_section.format_line_)
				&& (section_entries_ == parameter_section.section_entries_)
				&& (variable_names_ == parameter_section.variable_names_)
				&& (entries_ == parameter_section.entries_)
				&& (keys_ == parameter_section.keys_)
				&& (number_of_variables_ == parameter_section.number_of_variables_)
				&& (version_ == parameter_section.version_)
				&& (valid_ == parameter_section.valid_) );
	}

} // namespace BALL
