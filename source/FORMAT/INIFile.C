// $Id: INIFile.C,v 1.12 2001/03/09 20:53:03 amoll Exp $

#include <BALL/FORMAT/INIFile.h>
#include <fstream>

using namespace std;

namespace BALL 
{

	// Default constructor
	INIFile::INIFile()
		: valid_(false),
			filename_(""),
			original_number_of_lines_(0)
	{	
	}

	INIFile::INIFile(const String& filename)
		: valid_(false),
			filename_(filename),
			original_number_of_lines_(0)
	{
	}

	void INIFile::destroy()
	{
		clear();
		filename_ = "";
		valid_ = false;
	}

	void INIFile::clear()
	{
		lines_.clear();
		line_section_index_.clear();
		original_number_of_lines_ = 0;
		
		section_names_.clear();
		section_start_.clear();
		section_end_.clear();
		section_key_map_.destroy();
		section_index_.destroy();
		valid_ = false;
	}

	INIFile::~INIFile()
	{
		clear();
	}


	bool INIFile::isValid() const 
	{
		return valid_;
	}

	void INIFile::setFilename(const String& filename)
	{
		filename_ = filename;
		valid_ = false;
	}

	const String& INIFile::getFilename() const
	{
		return filename_;
	}

	bool INIFile::read()
	{
		// destroy all datastructures - we make a new start
		// we only keep the filename...
		clear();

		// try to open the file
		ifstream infile(filename_.c_str());

		// if we couldn't open the file: abort
		if (!infile)
		{
			return false;
		}

		// the section zero is named ""
		Index section_nr = -1;

		// read all lines from the file
		char buffer[MAX_LINE_LENGTH];
		while (infile.getline(&(buffer[0]), MAX_LINE_LENGTH))
		{
			// store the line
			lines_.push_back(buffer);

			// remove leading blanks
			String line(buffer);
			line.trimLeft();

			// check for comment lines or empty line
			if ((line.size() == 0) || (line[0] == '!') || 
					(line[0] == ';')   || (line[0] == '#'))
			{
				continue;
			}

			// check for start of section
			if (line[0] == '[')
			{	
				// remove the leading '['
				line.erase(0, 1);

				// check for a closing "]"
				if (!line.has(']'))
				{
					continue;
				}

				// OK, this is really a section
				// strip the bracket to get the name
				line = line.before("]");

				section_nr++;
				
				// remember the current section_name
				section_names_.push_back(line);
				section_index_[line] = section_nr;
				
				// insert this section
				line_section_index_.push_back(section_nr);
				section_start_.push_back(lines_.size());

				// if this is the first section, nothing left to do...
				if (section_nr == 0)
				{
					continue;
				}

				// this is not the first section; finish the last

				// if last section has size 0 mark it
				if (section_start_[section_nr -1] == lines_.size() - 1)
				{
					section_start_[section_nr -1] = INVALID_SIZE;
					section_end_.push_back(INVALID_SIZE);
				}
				else
				{
					// there is an end to remember
					section_end_.push_back(lines_.size() - 2);
				}

				continue;
			}

			// this is neither a comment line nor a section start
			// check for lines matching ".*=". These are key/value pairs
			if ((line.size() > 0) && (line.find_first_of("=") > 0))
			{
				String key(line.before("="));
				key.trim();
				String match_name = section_names_[section_nr] + "]" + key;
				section_key_map_[match_name] = lines_.size() - 1;
				line_section_index_.push_back(section_nr);
			}
		}
		
		// terminate the last section
		if (section_nr > -1 && section_start_[section_nr] >= lines_.size() - 1)
		{
			section_start_[section_nr] = INVALID_SIZE;
			section_end_.push_back(INVALID_SIZE);
		}
		else
		{
			// there is an end to remember
			section_end_.push_back(lines_.size() - 1);
		}

		// close the file
		infile.close();

		// remember the number of lines in the file
		// lines that are added afterwards appear at the end of the lines_ vector
		original_number_of_lines_ = lines_.size();

		// done.
		valid_ = true;
		return true;
	}


	bool INIFile::write()
	{
		// try to open the file
		ofstream out(filename_.c_str(), ios::out);
		
		if (out.bad())
		{
			return false;
		}

		// write all the original lines of the file
		Index current_section_index = 0;

		for (Size i = 0; i < original_number_of_lines_; ++i)
		{
			// write all lines of a section 
			if (line_section_index_[i] == current_section_index)
			{	
				out << lines_[i] << endl;
			} 
			// skip the line if it was marked for removal 
			// (negative sign of the index)
			else 
			{
				if (line_section_index_[i] == -current_section_index)
				{
					continue;
				}
				else 
				{
					// we entered a new section: we have to check for added lines
					// which are inserted at the end of lines_ (index above
					// original_number_of_lines_)
					for (Size j = original_number_of_lines_; j < lines_.size(); j++)
					{
						// if any of the added lines belongs to our current
						// section, write it to the file
						if (line_section_index_[j] == current_section_index)
						{
							out << lines_[j] << endl;
						}
					}
					
					// now write the first line of the new section 
					// if it was not marked for removal
					if (line_section_index_[i] >= 0)
					{
						out << lines_[i] << endl;
					}
					
					// and increment the section index
					current_section_index++;
				}
			}
		}

		// now append all new sections and lines that were not read from a file
		while (current_section_index < (Index)section_names_.size())
		{
			for (Size i = original_number_of_lines_; i < lines_.size(); i++)
			{
				if (line_section_index_[i] == current_section_index)
				{
					out << lines_[i] << endl;
				}
			}
				
			// next section
			current_section_index++;
		}

		out.close();
		valid_ = true;
		return true;
	}

	String* INIFile::getLine(Size line_number)
	{
		if (line_number < lines_.size())
		{
			return &(lines_[line_number]);
		}

		return 0;
	}

	// BAUSTELLE changes in section name or key pair should be noticed
	bool INIFile::setLine(Size line_number, const String& line)
	{
		if (line_number >= lines_.size())
		{
			return false;
		}
		
		lines_[line_number] = line;
		
		return true;
	}

	Size INIFile::getNumberOfLines() const 
	{
		return lines_.size();
	}

	Size INIFile::getNumberOfSections() const 
	{
		return section_names_.size();
	}

	bool INIFile::hasSection(const String& section_name) const
	{
		return section_index_.has(section_name);
	}

	String* INIFile::getSectionName(Position pos)
	{
		if (pos >= section_names_.size())
		{
			return 0;
		}
		return &section_names_[pos];
	}


	Size INIFile::getSectionFirstLine(const String& section_name) const
	{
		if (section_index_.has(section_name)) 
		{
			return section_start_[section_index_[section_name]];
		}

		return INVALID_SIZE;
	}

	Size INIFile::getSectionLastLine(const String& section_name) const
	{
		if (section_index_.has(section_name)) 
		{
			return section_end_[section_index_[section_name]];
		}

		return INVALID_SIZE;
	}

	Size INIFile::getSectionLength(const String& section_name) const
	{
		if (!section_index_.has(section_name))
		{
			return INVALID_SIZE;
		}
		
		if (section_start_[section_index_[section_name]] != INVALID_SIZE)
		{
			return section_end_[section_index_[section_name]] -
						 section_start_[section_index_[section_name]] + 1;
		}
		
		return 0;
	}

	bool INIFile::hasEntry(const String& section, const String& key) const
	{
		// create a hash key from section and key. Use "]" as a delimiter
		// (it cannot be part of the section name)
		String match_name(section + "]" + key);

		// check the hash map for the key
		return section_key_map_.has(match_name);
	}

	bool INIFile::setValue(const String& section, const String& key, const String& new_value)
	{
		// create a hash key from section and key. Use "]" as a delimiter
		// (it cannot be part of the section name)
		String match_name(section + "]" + key);

		// BAUSTELLE
		// check whether we know the key - create it if it does not exist
		if (!section_key_map_.has(match_name))
		{
			// verify whether the section exists
			Index section_index = -1;
			for (Index i = 0; i < (Index)section_names_.size(); ++i)
			{
				if (section_names_[(Size)i] == section)
				{	
					section_index = i;
					break;
				}
			}
			if (section_index < 0)
			{
				// create a new section header
				section_index = (Index)section_names_.size();
				section_names_.push_back(section);
				lines_.push_back("[" + section + "]");
				line_section_index_.push_back((Index)section_index);
			}
				
			// create the line
			lines_.push_back(key + "=" + new_value);
			line_section_index_.push_back((Index)section_index);

			// insert the line into the hash map
			section_key_map_.insert
				(StringHashMap<Size>::ValueType(match_name, lines_.size() - 1));
			
			return true;
		}

		Size index = section_key_map_[match_name];
		// nasty workaround: KCC produces a warning 
		Index minus_one = -1;
		if (lines_[index][minus_one] != '=')
		{
			// create the substring describing the value part of the line
			Substring value = lines_[index].after('=', 0);

			// assign its new value
			value = new_value;
		} 
		else 
		{
			lines_[index] += new_value;
		}
		
		return true;
	}

	String INIFile::getValue(const String& section, const String& key) const
	{
		// create a hash key from section and key. Use "]" as a delimiter
		// (it cannot be part of the section name)
		String match_name(section + "]" + key);

		// check whether we know the key - return false in case of failure
		if (!section_key_map_.has(match_name))
		{
			return "";
		}

		// get the part of the line behind the "="
		match_name = lines_[section_key_map_[match_name]].after('=', 0);
		match_name.trim();
		
		return match_name;
	}


	bool INIFile::operator == (const INIFile& inifile) const
	{
		return ((valid_ == inifile.valid_)
		&& (filename_ == inifile.filename_)
		&& (section_names_ == inifile.section_names_)
		&& (lines_ == inifile.lines_)
		&& (line_section_index_ == inifile.line_section_index_)
		&& (section_start_== inifile.section_start_)
		&& (section_end_ == inifile.section_end_)
		&& (section_index_ == inifile.section_index_)
		&& (section_key_map_ == inifile.section_key_map_)
		&& (original_number_of_lines_ == inifile.original_number_of_lines_));
	}

} // namespace BALL
