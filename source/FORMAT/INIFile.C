// $Id: INIFile.C,v 1.3 1999/12/04 18:34:28 oliver Exp $

#include <BALL/FORMAT/INIFile.h>

#include <fstream>

namespace BALL 
{

	// Default constructor
	INIFile::INIFile()
	{	
	}

	INIFile::INIFile(const String& filename)
	{
		valid_ = true;
		setFilename(filename);
	}

	void INIFile::destroy()
	{
		lines_.clear();
		
		section_start_.clear();
		section_end_.clear();
		section_key_map_.destroy();
		section_index_.destroy();

		filename_ = "";

		valid_ = false;
	}

	INIFile::~INIFile()
	{
		destroy();
	}


	bool INIFile::isValid() const 
	{
		return valid_;
	}

	void INIFile::setFilename(const String& filename)
	{
		filename_ = filename;
	}


	const String& INIFile::getFilename() const
	{
		return filename_;
	}

	bool INIFile::read()
	{
		// destroy all datastructures - we make a new start
		lines_.clear();
		
		section_start_.clear();
		section_end_.clear();
		section_key_map_.destroy();
		section_index_.destroy();


		// we invalidate this instance. If we could read everything
		// correctly, it will be tagged valid at the end of ::read
		valid_ = false;

		// try to open the file
		ifstream infile(filename_.c_str());

		// if we couldn't open the file: abort
		if (!infile)
			return false;

		// the section zero is named ""
		String section_name("");

		// read all lines from the file
		char buffer[MAX_LINE_LENGTH];
		while (infile.getline(&(buffer[0]), MAX_LINE_LENGTH))
		{
			// store the line
			lines_.push_back(buffer);

			// rmove leading blanks
			String line(buffer);
			line.trimLeft();

			// check for comment lines
			if ((buffer[0] != '!') && (buffer[0] != ';') && (buffer[0] != '#'))
			{

				// check for start of section
				if (buffer[0] == '[')
				{	
					// remove the leading '['
					line.erase(0, 1);

					// check for a closing "]"
					Size index;
					if (line.size() > 0){
						index = line.find_first_of("]");
					} else {
						index = string::npos;
					}

					if (index != string::npos)
					{
						// OK, this is really a section, strip the bracket to get the name
						line = line.before("]");
						
						// insert this section
						if (section_start_.size() != 0)
						{
							// if this is the first section, there is not end to remember
							section_end_.push_back(lines_.size() - 2);
						}
						section_index_[line] = section_start_.size();
						section_start_.push_back(lines_.size());
						
						// remember the current section_name
						section_name = line;
					}
				} else {
					// check for lines matching ".*=". These are key/value pairs
					if ((line.size() > 0) && (line.find_first_of("=") > 0))
					{
						String key(line.before("="));
						String match_name = section_name + "]" + key;
						section_key_map_[match_name] = lines_.size() - 1;
					}
				}
			} 
		}
		
		// terminate the last section
		section_end_.push_back(lines_.size() - 1);
		
		// close the file
		infile.close();

		// done.
		valid_ = true;
		return true;
	}


	// BAUSTELLE
	bool INIFile::write()
	{
		return true;
	}

	String* INIFile::getLine(Size line_number)
	{
		if (line_number < lines_.size())
			return &(lines_[line_number]);

		return 0;
	}

	bool INIFile::setLine(Size line_number, const String& line)
	{
		if (line_number >= lines_.size())
			return false;
		
		lines_[line_number] = line;
		
		return true;
	}

	Size INIFile::getNumberOfLines() const 
	{
		return lines_.size();
	}

	bool INIFile::hasSection(const String& section_name) const
	{
		return section_index_.has(section_name);
	}

	Size INIFile::getSectionFirstLine(const String& section_name) const
	{
		if (section_index_.has(section_name)) 
		{
			Size index = (*section_index_.find(section_name)).second;
			return section_start_[index];
		}

		return INVALID_SIZE;
	}

	Size INIFile::getSectionLastLine(const String& section_name) const
	{
		if (section_index_.has(section_name)) 
		{
			StringHashMap<Size>&	map = const_cast<StringHashMap<Size>&>(section_index_);
			Size index = map[section_name];
			vector<Size>&	end = const_cast<vector<Size>&>(section_end_);
			return end[index];
		}

		return INVALID_SIZE;
	}

	Size INIFile::getSectionLength(const String& section_name) const
	{
		if (section_index_.has(section_name))
		{
			StringHashMap<Size>&	map = const_cast<StringHashMap<Size>&>(section_index_);
			Size index = map[section_name];
			vector<Size>&	start = const_cast<vector<Size>&>(section_start_);
			return start[index];
		}
		
		return INVALID_SIZE;
	}

	bool INIFile::hasEntry(const String& section, const String& key) const
	{
		String match_name;

		// create a hash key from section and key. Use "]" as a delimiter
		// (it cannot be part of the section name)
		match_name = section + "]" + key;

		// check the hash map for the key
		return section_key_map_.has(match_name);
	}

	bool INIFile::setValue(const String& section, const String& key, const String& new_value)
	{
		String match_name;

		// create a hash key from section and key. Use "]" as a delimiter
		// (it cannot be part of the section name)
		match_name = section + "]" + key;

		// check whether we know the key - return false in case of failure
		if (!section_key_map_.has(match_name))
			return false;

		// create the substring describing the value part of the line
		Substring value = lines_[section_key_map_[match_name]].after('=', 0);

		// assign its new value
		value = new_value;
		
		return true;
	}

	String INIFile::getValue(const String& section, const String& key) const
	{
		String match_name;

		// create a hash key from section and key. Use "]" as a delimiter
		// (it cannot be part of the section name)
		match_name = section + "]" + key;

		// check whether we know the key - return false in case of failure
		if (!section_key_map_.has(match_name))
			return "";

		// get the part of the line behind the "="
		StringHashMap<Size>&	map = const_cast<StringHashMap<Size>&>(section_key_map_);
		Size index = map[match_name];
		vector<String>&	lines = const_cast<vector<String>&>(lines_);
		Substring value = lines[index].after('=', 0);
		
		// assign it to match_name -- recycling!
		match_name = value;

		// return the value
		return match_name;
	}

} // namespace BALL
