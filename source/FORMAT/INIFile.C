// $Id: INIFile.C,v 1.7 2000/01/18 12:53:50 oliver Exp $

#include <BALL/FORMAT/INIFile.h>

#include <fstream>

using namespace std;

namespace BALL 
{

	// Default constructor
	INIFile::INIFile()
		: valid_(true),
			filename_(""),
			original_number_of_lines_(0)
	{	
	}

	INIFile::INIFile(const String& filename)
		: valid_(true),
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
		if (!infile) return false;

		// the section zero is named ""
		String section_name("");
		Size section_index = 0;

		// read all lines from the file
		char buffer[MAX_LINE_LENGTH];
		while (infile.getline(&(buffer[0]), MAX_LINE_LENGTH))
		{
			// store the line
			lines_.push_back(buffer);

			// remove leading blanks
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
						
						
						// remember the current section_name
						section_name = line;
						section_index++;

						// insert this section
						if (section_start_.size() != 0)
						{
							// if this is the first section, there is no end to remember
							section_end_.push_back(lines_.size() - 2);
						} else {
							// if the first line of the first section is the first line
							// of the file, then there is no empty section 0
							if (lines_.size() == 1)
							{
								section_names_.push_back(section_name);
							} else {
								// insert a dummy name for the first section
								section_names_.push_back("");
								section_names_.push_back(section_name);
								section_index++;
							}
						}
						section_index_[line] = section_start_.size();
						section_start_.push_back(lines_.size());
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
			// store the section index for each line
			line_section_index_.push_back((Index)section_index);
		}
		
		// terminate the last section
		section_end_.push_back(lines_.size() - 1);
		
		// close the file
		infile.close();

		// remember the number of lines in the file
		// lines that are added afterwards appear at the end of the
		// lines_ vector
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
		Size i;
		for (i = 0; i < original_number_of_lines_; ++i)
		{
			// write all lines of a section 
			if (line_section_index_[i] == current_section_index)
			{	
				out << lines_[i] << endl;
			} 
			// skip the line if it was marked for removal 
			// (negative sign of the index)
			else if (line_section_index_[i] == -current_section_index)
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

		// now append all new sections and lines that were not
		// read from a file
		while (current_section_index < (Index)section_names_.size())
		{
			for (i = original_number_of_lines_; i < lines_.size(); i++)
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
			
			// done.
			return true;
		}

		Size index = section_key_map_[match_name];
		if (lines_[index][-1] != '=')
		{
			// create the substring describing the value part of the line
			Substring value = lines_[index].after('=', 0);

			// assign its new value
			value = new_value;
		} else {
			lines_[index] += new_value;
		}
		
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
