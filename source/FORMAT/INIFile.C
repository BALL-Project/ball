// $Id: INIFile.C,v 1.15 2001/04/08 23:30:03 amoll Exp $

#include <BALL/FORMAT/INIFile.h>
#include <fstream>

#include <iostream>

using namespace std;

namespace BALL 
{

	const String INIFile::UNDEFINED = "[UNDEFINED!]";
	const String INIFile::HEADER = "#HEADER!";	

	// Default constructor
	INIFile::INIFile()
		: valid_(false),
			filename_("")
	{	
	}

	INIFile::INIFile(const String& filename)
		: valid_(false),
			filename_(filename)
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
		sections_.destroy();
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

    // create the header; this has to be done to prevent problems 
		// with the positions in the list; the HEADER is section 0
		Section header;
		header.name_ = HEADER;
 		sections_.push_back(header);
		section_index_[HEADER] = sections_.begin();

		List<Section>::Iterator	section_it(sections_.begin());
		List<String >::Iterator	   line_it(section_it->lines_.begin());

		// read all lines from the file
		char buffer[MAX_LINE_LENGTH];
		while (infile.getline(&(buffer[0]), MAX_LINE_LENGTH))
		{
			// remove leading blanks
			String line(buffer);
			line.trimLeft();

			// check for comment lines or empty line
			if ((line.size() == 0) || (line[0] == '!') || 
					(line[0] == ';')   || (line[0] == '#'))
			{
				section_it->lines_.push_back(buffer);
				line_it++;
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
					// error in line, skip it
					continue;
				}

				// OK, this is really a section

				// strip the bracket to get the name
				line = line.before("]");

				Section section;
				section.name_ = line;
				sections_.push_back(section);
				section_it++;

				// store the line
				section_it->lines_.push_back(buffer);
				line_it = section_it->lines_.begin();
				
				// remember the current section_name
				section_index_[line] = section_it;
				
				continue;
			}
			
			// this is neither a comment line nor a section start
			// line still has to be added
			section_it->lines_.push_back(buffer);
			line_it++;

			// check for lines matching ".*=". These are key/value pairs
			if (line.find_first_of("=") > 0)
			{
				String key(line.before("="));
				key.trim();
				section_it->key_map_[key] = line_it;			
			}
		}
		
		// close the file
		infile.close();

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

		LineIterator	line_it(getLine(0));
		// iterate over all lines and write them
		for (; +line_it; ++line_it)
		{
			out << *line_it << endl;
		}
		
		out.close();
		valid_ = true;
		return true;
	}

	INIFile::LineIterator INIFile::getLine(Size line_number)
	{
		if (line_number >= getNumberOfLines())
		{
			return LineIterator();
		}

		int line_nr(line_number);
		int nr(-1);
		LineIterator line_it;		

		Section_iterator section_it(sections_.begin());
		for (; (section_it != sections_.end() && nr != line_nr); ++section_it)
		{
			if ((int)section_it->lines_.size() + nr < line_nr)
			{
				nr += section_it->lines_.size();
				continue;
			}

			line_it = getSectionFirstLine(section_it->getName());
			nr++;

			for (; nr < line_nr; ++line_it)
			{
				nr++;
			}
		}
		return (line_it);
	}

	bool INIFile::setLine(LineIterator line_it, const String& line)
	{
		// section lines cannot be changed with this method
		if ( line_it.getSection() == sections_.end() ||
				 ! +line_it ||
				 (*line_it)[0] == '[')
		{
			return false;
		}
		
		String new_key(line.before("="));
		new_key.trim();

		if ((*line_it).find_first_of("=") > 0)
		{
			// oh, this line had a key :(
			String old_key((*line_it).before("="));
			old_key.trim();

			// its a new key: delete the old one.
			if (old_key != new_key)
			{		
			  line_it.getSection()->key_map_.remove(old_key);
			}
		}

		*line_it = line;

		if (line.find_first_of("=") > 0)
		{
			// oh, the new line has a key :(
			line_it.getSection()->key_map_[new_key] = line_it.getPosition();		
		}

		return true;
	}

	bool INIFile::deleteLine(LineIterator line_it)
	{
		// test if line exists and if we try to remove a section line
		if (!+line_it || (*line_it)[0] == '[')
		{
			return false;
		}

		// falls key, entfernen		
		if ((*line_it).find_first_of("=") > 0)
		{
			String key((*line_it).before("="));
			key.trim();

			line_it.getSection()->key_map_.remove(key);
		}

		line_it.getSection()->lines_.erase(line_it.position_);

		return true;
	}

	
	bool INIFile::appendLine(const String& section_name, const String& line)
	{
		if (!hasSection(section_name) || line[0] == '[')
		{
			return false;
		}
		
		Section& section(*getSection(section_name));

		// if no key, we are finished
    if (line.find_first_of("=") > 0)
    {
			String key(line.before("="));
			key.trim();

			if (section.key_map_.has(key))
			{
				return false;
			}
			
			section.lines_.push_back(line);
			List<String >::Iterator	line_it(section.lines_.end());
			line_it--;

			section.key_map_[key] = line_it;

			return true;
		}

		section.lines_.push_back(line);

		return true;
	}
	
	Size INIFile::getNumberOfLines() const
	{
		Size number_of_lines(0);
	
		List<Section>::ConstIterator it = sections_.begin();
		for (; it != sections_.end(); ++it)
		{
			number_of_lines += it->lines_.size();
		}
		
		return number_of_lines;
	}	
	
	Size INIFile::getNumberOfSections() const 
	{
		// HEADER is not counted
		return sections_.size() -1;
	}

	bool INIFile::hasSection(const String& section_name) const
	{
		return section_index_.has(section_name);
	}

	INIFile::Section_iterator INIFile::getSection(const String& section_name)
	{
		if (!section_index_.has(section_name)) 
		{
			return 0;
		}
		return section_index_[section_name];
	}


	INIFile::Section_iterator INIFile::getSection(Position pos)
	{
		if (pos >= sections_.size())
		{
			return 0;
		}

		Section_iterator it = sections_.begin();
		for (Position i = 0; i < pos && it != sections_.end(); i++)
		{
			++it;
		}
		return it;
	}

	INIFile::LineIterator INIFile::getSectionFirstLine(const String& section_name)
	{
		if (!section_index_.has(section_name)) 
		{
			return LineIterator();
		}

		return LineIterator(sections_, getSection(section_name), 
												 getSection(section_name)->lines_.begin());
	}

	INIFile::LineIterator INIFile::getSectionLastLine(const String& section_name)
	{
		if (!section_index_.has(section_name)) 
		{
			return LineIterator();
		}

		List<String >::Iterator	line_it(getSection(section_name)->lines_.end());
		line_it--;
		return LineIterator(sections_, getSection(section_name), line_it);
	}

	Size INIFile::getSectionLength(const String& section_name) const
	{
		if (!section_index_.has(section_name))
		{
			return INVALID_SIZE;
		}

		return section_index_[section_name]->lines_.size();
	}

	bool INIFile::hasEntry(const String& section_name, const String& key) const
	{
		if (!section_index_.has(section_name))
		{
			return false;
		}

		// check the hash map for the key
		return section_index_[section_name]->key_map_.has(key);
	}

	bool INIFile::setValue(const String& section_name, const String& key, const String& new_value)
	{
		// does section exists?
		if (!section_index_.has(section_name)									||
				key.isEmpty()																			||
				!section_index_[section_name]->key_map_.has(key))
		{
			return false;
		}
		
		String new_line(key + "=" + new_value);
		(*section_index_[section_name]->key_map_[key]) = new_line;
		
		return true;
	}

	String INIFile::getValue(const String& section_name, const String& key) const
	{
		if (!section_index_.has(section_name)									||
				key.isEmpty()																			||
				!section_index_[section_name]->key_map_.has(key))
		{
			return UNDEFINED;
		}

		// get the part of the line behind the "="
		String match_name((*section_index_[section_name]->key_map_[key]).after('=', 0));
		match_name.trim();
		
		return match_name;
	}

	bool INIFile::apply(UnaryProcessor<LineIterator>& processor)
	{		
		if (!processor.start())
		{
			return false;
		}

		LineIterator line_it(getLine(0));
		for (; +line_it; ++line_it)
		{
			cout << endl << *line_it;
			Processor::Result result = processor(line_it);
		
			if (result <= Processor::BREAK)
			{
				return (result && processor.finish());
			}
		}

		return (processor.finish());
	}

	bool INIFile::operator == (const INIFile& inifile) const
	{
		return (valid_ == inifile.valid_
				&& filename_ == inifile.filename_
				&& sections_ == inifile.sections_
				&& section_index_ == inifile.section_index_);
	}

} // namespace BALL
