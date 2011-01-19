// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//


#include <BALL/FORMAT/JCAMPFile.h>
#include <BALL/DATATYPE/regularExpression.h>
#include <BALL/MATHS/common.h>

namespace BALL
{

	bool JCAMPFile::JCAMPValue::operator == (const JCAMPValue& value) const
	{
		return string_value  	== value.string_value &&
					 numeric_value  == value.numeric_value && 
					 type 				  == value.type;
	}

	bool JCAMPFile::JCAMPValue::operator != (const JCAMPValue& value) const
	{
		return !(*this == value);
	}

	JCAMPFile::JCAMPFile(const String& name, OpenMode open_mode) 
		: LineBasedFile(), 
			header_(),
			entries_()
	{
		LineBasedFile::open(name, open_mode);
		enableTrimWhitespaces(true);
	}

	void JCAMPFile::read()
	{
		// Clear the old contents of the header/entry maps.
		header_.clear();
		entries_.clear();

		// First I try to read the title. 
		// Only parameters appearing *after* the title are used.

		// Expressions for the valid declarations in a JCAMPFile 
		const RegularExpression array("^##\\$([^=]+)= \\(0\\.\\.([1-9][0-9]*)\\)");
		const RegularExpression regular("^##\\$([^=]+)= (.*)");
		const RegularExpression header("^##([^=]+)= (.*)");

		std::vector<Substring> groups;

		while (readLine())
		{
			JCAMPValue value;
			try
			{
				//overread a comment 
				if (getLine().hasPrefix("$$"))
				{
					continue;
				}
				else if (getLine().hasPrefix("##$"))
				{

					// Check whether the definition is an array.
					if (array.find(getLine(), groups))
					{
						value.type = ARRAY;

						static String key = groups[1];
						Size number_of_values = groups[2].toString().toInt() + 1;

						// first try to parse the remainder of this line
						std::vector<String> remainder;
						getLine().split(remainder, ")"); // take the part right of the array definition
						if (remainder.size() > 2)
						{
							throw Exception::ParseError(__FILE__, __LINE__, getName(), 
																	String("Error parsing array entry on line ") 
																	+ String(getLineNumber()));
						}

						if (remainder.size() == 2)
						{
							std::vector<String> fields;
							remainder[1].split(fields);
							for (Position i=0; i<fields.size(); i++)
							{
								if (!fields[i].isFloat())
								{
									Log.error() << "Warning: " << fields[i] << " is not a float!" << std::endl;
								}
								else
								{
									value.numeric_value.push_back(fields[i].toDouble());
								}
							}
						}

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
								if (!fields[i].isFloat())
								{
									Log.error() << "Warning: " << fields[i] << " is not a float!" << std::endl;
								}
								else
								{
									value.numeric_value.push_back(fields[i].toDouble());
								}
							}
						}
						
						// Store the entry only if the parsing was successful.
						if (value.numeric_value.size() == number_of_values)
						{
							entries_.insert(KeyValuePair(key, value));
						}
						else
						{
							throw Exception::ParseError(__FILE__, __LINE__, getName(), 
																	String("Error parsing array entry on line ") 
																	+ String(getLineNumber()) + String(" (number of values read:") 
																	+ String(value.numeric_value.size())
																	+ " - number of values expected: " + String(number_of_values) + ")");
						}
						
						// next entry.
						continue;
					}
					// =================== read non-array entry ===============================
					else if (regular.find(getLine(), groups))
					{
						// =================== read numeric entry ===============================
						if (groups[2].toString().isFloat())
						{
							value.numeric_value.push_back(groups[2].toString().toFloat());
							value.type = NUMERIC;
						}
						// =================== read string entry ================================
						else
						{
							value.string_value = groups[2];
							value.type = STRING;
						}
						entries_.insert(KeyValuePair(groups[1], value));

						// next entry.
						continue;
					}
				}
				else if (header.find(getLine(), groups))
				{
					if (groups[1] == "END")
					{
						// Abort for the "##END=" entry.
						break;
					}
					value.type = STRING;
					value.string_value = groups[2];

					// Insert the header entry into the header map
					header_.insert(std::pair<String, String>(groups[1].toString(), groups[2].toString()));
					// Next entry.
					continue;
				}
				else if (getLine().hasPrefix("##END"))
				{
					break;
				}
			}
			catch(Exception::InvalidFormat&)
			{
				Log.error() << "Could not convert string to number" << std::endl;
				// throwing exception ParseError anyhow in next line
			}

			// We could not parse this line -- abort.
			throw Exception::ParseError(__FILE__, __LINE__, getName(), getLine());
		};
	}

	bool JCAMPFile::write()
	{
		if (!isOpen() || getOpenMode() != File::MODE_OUT)
		{
			throw(File::CannotWrite(__FILE__, __LINE__, name_));
		}

		HeaderMap::ConstIterator header_it(header_.begin());
		for (; header_it != header_.end(); ++header_it)
		{
			(*this) << "##" << header_it->first << "= " << header_it->second << "\n";
		}

		EntryMap::ConstIterator entry_it(entries_.begin());
		for (; entry_it != entries_.end(); ++entry_it)
		{
			(*this) << "##$" << entry_it->first << "= ";
			switch (entry_it->second.type)
			{
			  case STRING: 
					(*this) << entry_it->second.string_value.c_str() << "\n";
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

		return true;
	}

	double JCAMPFile::getDoubleValue(const String& name) const 
	{
		if (!entries_.has(name)) return 0.0;

		const JCAMPValue& val(entries_[name]);
		if (val.type == NUMERIC)
		{
			return val.numeric_value[0];
		}
		else if (val.type == STRING)
		{
			return val.string_value.trim().toDouble();
		}
		else if ((val.type == ARRAY) && (val.numeric_value.size() > 0))
		{
			return val.numeric_value[0];
		}

		return 0.0;
	}

	Index JCAMPFile::getIntValue(const String& name) const 
	{
		if (!entries_.has(name)) return 0;

		const JCAMPValue& val(entries_[name]);
		if (val.type == NUMERIC) 
		{
			int i = (int)Maths::round(val.numeric_value[0]);
			return i; //round(val.numeric_value[0]).toInt();
		}
		else if(val.type == STRING)
		{
			return (Index)val.string_value.trim().toInt();
		}
		else if ((val.type == ARRAY) && (val.numeric_value.size() > 0))
		{
			return (Index)val.numeric_value[0];
		}

		return 0;
	}

	const JCAMPFile& JCAMPFile::operator = (const JCAMPFile& file)
	{
		header_  = file.header_;
		entries_ = file.entries_;
		LineBasedFile::operator = (file);

		return *this;
	}

	bool JCAMPFile::operator == (const JCAMPFile& f)  const 
	{
		return header_ == f.header_ &&
					 entries_ == f.entries_;

	}

	bool JCAMPFile::operator != (const JCAMPFile& f)  const 
	{
		return ! (*this == f);
	}

} // namespace BALL
