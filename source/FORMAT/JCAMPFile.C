// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: JCAMPFile.C,v 1.9 2003/06/01 09:13:41 oliver Exp $
//

#include <BALL/FORMAT/JCAMPFile.h>

namespace BALL
{

	JCAMPFile::JCAMPFile( const String& name, OpenMode open_mode) 
		throw(Exception::FileNotFound)
		: LineBasedFile(name, open_mode), 
			header_(),
			entries_()
	{
	}

	JCAMPFile::JCAMPFile(const JCAMPFile& file) 
		throw(Exception::FileNotFound)
		: LineBasedFile(file), 
			header_(file.header_),
			entries_(file.entries_)
	{
	}

	void JCAMPFile::read()
		throw(Exception::ParseError)
	{
		// Clear the old contents of the header/entry maps.
		header_.clear();
		entries_.clear();

		// First I try to read the title. Only parameters appearing *after* the
		// title are used.

		// A regular expression matching the array declaration in JCAMP
		static RegularExpression array("^##$([^=]+)= \\(0\\.\\.([1-9][0-9]*)\\)");
		static RegularExpression regular("^##$([^=]+)= (.*)$");
		static RegularExpression header("^##([^=]+)= (.*)$");
		static std::vector<Substring> groups;
		static JCAMPValue value;

		while (readLine())
		{
			if (getLine().hasPrefix("##$"))
			{

				// Check whether the definition is an array.
				if (array.find(getLine(), groups))
				{
					value.type = ARRAY;
					value.numeric_value.clear();

					static String key = groups[0];
					Size number_of_values = groups[1].toString().toInt();
					while (value.numeric_value.size() < number_of_values)
					{
						if (!readLine() || getLine().hasPrefix("#")) 
						{
							break;
						}
						static std::vector<String> fields;
						getLine().split(fields);
						for (Position i = 0; i < fields.size(); i++)
						{
							value.numeric_value.push_back(fields[i].toDouble());
						}
					}
					
					// Store the entry only if the parsing was successful.
					if (value.numeric_value.size() == number_of_values)
					{
						entries_.insert(KeyValuePair(key, value));
					}
					else
					{
						throw Exception::ParseError(__FILE__, __LINE__, getName(), getLine());
					}
					
					// next entry.
					continue;
				}
				else if (regular.find(getLine(), groups))
				{
					// read non-array entry
					if (groups[1].toString().isFloat())
					{
						value.numeric_value.resize(1);
						value.type = NUMERIC;
					}
					else
					{
						value.string_value = groups[1];
						value.numeric_value.resize(0);
						value.type = STRING;
					}
					entries_.insert(KeyValuePair(groups[0], value));

					// next entry.
					continue;
				}
			}
			else if (header.find(getLine(), groups))
			{
				if (groups[0] == "END")
				{
					// Abort for the "##END=" entry.
					break;
				}
				value.type = STRING;
				value.string_value = groups[1];
				value.numeric_value.clear();

				// Insert the header entry into the header map
				header_.insert(std::pair<String, String>(groups[0].toString(), groups[1].toString()));
				// Next entry.
				continue;
			}
			
			// We could not parse this line -- abort.
			throw Exception::ParseError(__FILE__, __LINE__, getName(), getLine());
		};
	}

	void JCAMPFile::write()
	{
		HeaderMap::ConstIterator header_it(header_.begin());
		for (; +header_it; ++header_it)
		{
			(*this) << "##" << header_it->first << "= " << header_it->second << "\n";
		}

		EntryMap::ConstIterator entry_it(entries_.begin());
		for (; +entry_it; ++entry_it)
		{
			(*this) << "##$" << entry_it->first << "= ";
			switch (entry_it->second.type)
			{
			  case STRING: 
					(*this) << entry_it->second.string_value << "\n";
					break;

			  case NUMERIC:
					(*this) << entry_it->second.numeric_value[0] << "\n";
					break;

			  case ARRAY:
					(*this) << "(0.." << entry_it->second.numeric_value.size() << ")\n";
					for (Position i = 0; i < entry_it->second.numeric_value.size(); ++i)
					{
						(*this) << entry_it->second.numeric_value[i];

						// Pack eight numbers in one line, then a newline at the end.
						if (((i + 1) % 8) == 0)
						{
							(*this) << "\n";
						}
						else
						{
							(*this) << " ";
						}
					}
					// Make sure we terminate the line with a newline.
					if ((entry_it->second.numeric_value.size() % 8) != 0)
					{
						(*this) << "\n";
					}
					break;
			}
		}
	}

	double JCAMPFile::getDoubleValue(const String& name) const throw()
	{
		if (entries_.has(name))
		{
			const JCAMPValue& val(entries_[name]);
			if (val.type == NUMERIC)
			{
				return val.numeric_value[0];
			}
			else if (val.type == STRING)
			{
				return val.string_value.toDouble();
			}
			else if ((val.type == ARRAY) && (val.numeric_value.size() > 0))
			{
				return val.numeric_value[0];
			}
		}

		return 0.0;
	}

	Index JCAMPFile::getIntValue(const String& name) const throw()
	{
		if (entries_.has(name))
		{
			const JCAMPValue& val(entries_[name]);
			if ((val.type == NUMERIC) || (val.type == STRING))
			{
				return (Index)val.string_value.toInt();
			}
			else if ((val.type == ARRAY) && (val.numeric_value.size() > 0))
			{
				return (Index)val.numeric_value[0];
			}
		}

		return 0;
	}

} // namespace BALL
