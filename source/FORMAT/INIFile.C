// $Id: INIFile.C,v 1.14 2001/03/14 14:19:31 amoll Exp $

#include <BALL/FORMAT/INIFile.h>
#include <fstream>

using namespace std;

namespace BALL 
{

	const String INIFile::UNDEFINED = "[UNDEFINED!]";
	const String INIFile::PREFIX = "#PREFIX!";	

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

    // create the prefix; this has to be done to prevent problems 
		// with the positions in the vectors; the prefix is section 0
		Index section_nr = 0;
 		section_names_.push_back(PREFIX);
		section_index_[PREFIX] = 0;
		section_start_.push_back(0);

		// read all lines from the file
		char buffer[MAX_LINE_LENGTH];
		while (infile.getline(&(buffer[0]), MAX_LINE_LENGTH))
		{
			// store the line
			lines_.push_back(buffer);
      line_section_index_.push_back(section_nr);
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
				// this line is part of a new section
				line_section_index_[line_section_index_.size() - 1]++;
				// section will start in the next line
				section_start_.push_back(lines_.size());

				// if last section has size 0 mark it
				if (section_start_[section_nr -1] == lines_.size() - 1)
				{
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
			if (line.find_first_of("=") > 0)
			{
				String key(line.before("="));
				key.trim();
				String match_name = section_names_[section_nr] + "]" + key;
				section_key_map_[match_name] = lines_.size() - 1;
			}
		}
		
		// terminate the last section
		// if it had no lines mark it
		if (section_start_[section_nr] >= lines_.size() - 1)
		{
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

		// vector of the positions of lines in all sections
		// positions[section][linenr]
		vector<vector <Position> > positions;
		positions.resize(section_names_.size());

		// iterate over all lines and fill the vectors with the positions
		for (Position line = 0; line < lines_.size(); line++)
		{
			// deleted lines
			if (line_section_index_[line] == -1)
			{	
				continue;
			}
			
			positions[(Position)line_section_index_[line]].push_back(line);
		}
		
		// iterate over all sections
		for (Position section = 0; section < section_names_.size(); section++)
		{
			// iterate over all lines of the section and write them
			for (Position sec_line = 0; sec_line < positions[section].size(); sec_line++)
			{
				out << lines_[positions[section][sec_line]] << endl;
			}
		}			
		
		out.close();
		valid_ = true;
		return true;
	}

	const String* INIFile::getLine(Size line_number) const
	{
		if (line_number < lines_.size() &&
				line_section_index_[line_number] != -1)
		{
			return &(lines_[line_number]);
		}

		return 0;
	}

	bool INIFile::setLine(Size line_number, const String& line)
	{
		// section lines cannot be changed with this method
		if (line_number >= lines_.size() ||
				lines_[line_number][0] == '[')
		{
			return false;
		}
		
		// oh, this is a key :(
		if (lines_[line_number].find_first_of("=") > 0)
		{
		  Index section_nr = line_section_index_[line_number];
			String old_key(lines_[line_number].before("="));
			old_key.trim();

			String new_key(lines_[line_number].before("="));
			new_key.trim();

			// its a new key: delete the old one.
			if (old_key != new_key)
			{		
				deleteLine(line_number);
				line_section_index_[line_number = section_nr];
				String match_name(section_names_[section_nr] + "]" + old_key);
			  section_key_map_.remove(match_name);
			 }
		}
		lines_[line_number] = line;
		
		return true;
	}


	bool INIFile::deleteLine(Size line_number)
	{
		Index section_nr = line_section_index_[line_number];

		// test if line exists and if we try to remove a section line
		if (line_number >= lines_.size() || section_nr == -1 || 
				lines_[line_number][0] == '[')
		{
			return false;
		}

		// falls key, entfernen		
		if (lines_[line_number].find_first_of("=") > 0)
		{
			String key(lines_[line_number].before("="));
			key.trim();

			String match_name = section_names_[section_nr] + "]" + key;
			section_key_map_.remove(match_name);
		}

		line_section_index_[line_number] = INVALID_INDEX;

		return true;
	}

	
	bool INIFile::insertLine(const String& section_name, const String& line)
	{
		if (!hasSection(section_name) || line[0] == '[')
		{
			return false;
		}

		Position section_nr(section_index_[section_name]);
		
		// if no key, we are finished
    if (line.find_first_of("=") > 0)
    {
			String key(line.before("="));
			key.trim();

			String match_name(section_names_[section_nr] + "]" + key);
			if (section_key_map_.has(match_name))
			{
				return false;
			}
			
			// somehow dirty trick, but line will be added next
			section_key_map_[match_name] = lines_.size();			
		}

		lines_.push_back(line);
		line_section_index_.push_back(section_nr);

		return true;
	}
	
	
	Size INIFile::getOriginalNumberOfLines() const
	{
		return original_number_of_lines_;
	}

	Size INIFile::getNumberOfLines() const
	{
		Size number_of_lines(0);
	
		for (Position pos = 0; pos < lines_.size(); pos++)
		{
			if (line_section_index_[pos] != -1)
			{
				number_of_lines++;
			}
		}
		
		return number_of_lines;
	}	
	
	Size INIFile::getNumberOfSections() const 
	{
		// PREFIX is not counted
		return section_names_.size() -1;
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
			// the section had no lines in the read file
		  if (section_end_[section_index_[section_name]] == INVALID_SIZE)
		  {
		  	return INVALID_SIZE;
		  }
		  	
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
		
		Index section_nr(section_index_[section_name]);
    Size number_of_lines(0);
		
		if (section_end_[section_nr] != INVALID_SIZE)
		{
 			for (Position pos = section_start_[section_nr];
 					 pos <= section_end_[section_nr]; pos ++)
 			{
 				// dont count deleted lines
  			if (line_section_index_[pos] != INVALID_INDEX)
 				{
 					number_of_lines++;
 			 	}
 			}
		}
		
		// start counting all added lines
		for (Position pos = original_number_of_lines_;
				 pos < lines_.size(); pos++)
		{
			if (line_section_index_[pos] == section_nr)
			{
				number_of_lines++;
			}
		}
		
		return number_of_lines;
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
		// does section exists?
		if (!section_index_.has(section))
		{
			return false;
		}
		
		if (key.isEmpty() || new_value.isEmpty())
		{
			return false;
		}
		
		// create a hash key from section and key. Use "]" as a delimiter
		// (it cannot be part of the section name)
		String match_name(section + "]" + key);		

		// does key exists in section?
		if (!section_key_map_.has(match_name))
		{
			return false;
		}
		
		String new_line(key + "=" + new_value);
		lines_[section_key_map_[match_name]] = new_line;
		
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
			return UNDEFINED;
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
