// $Id: options.C,v 1.5 1999/12/28 18:24:59 oliver Exp $ 

#include <BALL/DATATYPE/options.h>

#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <math.h>

#include <fstream>
#include <list>
#include <algorithm>

using namespace std;

namespace BALL 
{

	Options::Options()
		:	StringHashMap<String>(),
			name_("")
	{
	}


	Options::Options(const Options& options, bool deep)
		:	StringHashMap<String>(options, deep),
			name_(options.name_)
	{
	}


	Options::~Options()
	{
	}

	bool Options::isReal(const String& key) const
	{
		errno = 0;
		char*	endptr;
		String value(get(key));

		// an empty String is no real number
		if (value =="")
		{
			return false;
		}
		
		// try to convert it to a number
		strtod(value.c_str(), &endptr);

		// return and tell whether it happend to work
		return (errno == 0) && (endptr != value.c_str());
	}

	bool Options::isVector(const String& key) const 
	{
		float	dummy;
		
		if (!has(key))
			return false;

		if (sscanf(get(key).c_str(), "(%f %f %f)", &dummy, &dummy, &dummy) == 3)
			return true;
		
		return false;
	}

	bool Options::isBool(const String& key) const
	{
		String s = get(key);
		if (s == "")
			return false;

		s.toLower();
		return (s.compare("true") == 0 || s.compare("false") == 0);
	}


	bool Options::isSet(const String& key) const
	{
		return (StringHashMap<String>::find(key) != StringHashMap<String>::end());
	}

	bool Options::isInteger(const String& key) const 
	{
		double double_value;
		long long_value;

		// if it cannot be read as a floating point number
		// it cannot be an integer
		if (!isReal(key))
			return false;
					
		
		// check wheter it is an integer
		long_value = atol(get(key).c_str());
		double_value = atof(get(key).c_str());

		// check if it is an integer (cutoff is 1e-7)
		if (fabs(double_value - ((double)long_value)) <= 1e-7)
			return true;

		// it is a floating point number, but no integer
		return false;
	}

	double Options::getReal(const String& key) const 
	{
		double value;

		errno = 0;
		value = atof((*find(key)).second.c_str());
		
		if (errno == 0){
			return value;
		} else {
			return 0.0;
		}
	}

	Vector3	Options::getVector(const String& key) const 
	{
		Vector3	h(0,0,0);
		
		if (!has(key))
		{
			return h;
		}

		sscanf(get(key).c_str(), "(%f %f %f)", &(h.x), &(h.y), &(h.z));
		
		return h;
	}

	bool Options::getBool(const String& key) const 
	{
		if ((*find(key)).second == "true")
		{
			return true;
		} else {
			return false;
		}
	}

	long Options::getInteger(const String& key) const 
	{
		long value;

		errno = 0;
		ConstIterator it = find(key);
		if (it == end())
			return 0;

		value = atol((*it).second.c_str());
		
		if (errno == 0)
		{
			return value;
		} else {
			errno = 0;
			return 0;
		}
	}


	void Options::set(const String& key, const String& value)
	{
		(*this)[key] = value;
	}

	void Options::setInteger(const String& key, const long value)
	{
		static char buffer[MaxEntryLength + 1];

		sprintf(buffer, "%ld", value);
					
		set(key, &(buffer[0]));
	}

	void Options::setReal(const String& key, const double value)
	{
		char buffer[MaxEntryLength + 1];
					
		sprintf(buffer, "%f", value);
					
		set(key, buffer);
	}

	void Options::setVector(const String& key, const Vector3& value)
	{
		char buffer[MaxEntryLength + 1];

		sprintf(buffer, "(%f %f %f)", value.x, value.y, value.z);

		set(key, buffer);
	}

	void Options::setBool(const String& key, const bool value)
	{
		if (value)
		{
			set(key, "true");
		} else {
			set(key, "false");
		}
	}

	String Options::setDefault(const String& key, const String& value)
	{
		if (!has(key))
		{
			set(key, value);
			return key;
		} else {
			return get(key);
		}
	}
		
	double Options::setDefaultReal(const String& key, const double value)
	{
		if (!has(key) || !isReal(key))
		{
			setReal(key, value);
			return value;
		} else {
			return getReal(key);
		}
	}

	bool Options::setDefaultBool(const String& key, const bool value)
	{
		if (!has(key) || !isBool(key))
		{
			setBool(key, value);
			return value;
		} else {
			return getBool(key);
		}
	}


	long Options::setDefaultInteger(const String& key, const long value)
	{
		if (!has(key) || !isInteger(key))
		{
			setInteger(key, value);
			return value;
		} else {
			return getInteger(key);
		}
	}



	void Options::setName(const String& name)
	{
		name_ = name;
	}

	const String& Options::getName() const
	{
		return name_;
	}

	String Options::get(const String& key) const
	{
		ConstIterator it = find(key);

		if (it == end())
			return "";
		else
			return (*it).second;
	}

	bool Options::readOptionFile(const String& filename)
	{
		ifstream		infile;

		infile.open(filename.c_str(), ios::in);
		if (!infile)
			return false;

		char		buffer[MaxEntryLength + 1];
		String	s, key;
		while (infile.getline(buffer, MaxEntryLength))
		{
			if ((buffer[0] != '#') && (buffer[0] != '!') && (buffer[0] != ';')) 
			{
				s = buffer;
				key = s.getField(0, " ");
				s = s.after(" ");
				set(key, s);
			}
		}
					
		infile.close();

		return true;
	}
		

	void Options::dump (ostream& stream, Size /* depth */) const
	{
		std::list<String>		entry_list;
		String							entry;

		stream << "[OptionsTable: " << getName() << " (" << size() << " entries)]" << endl;

		StringHashMap<String>::ConstIterator	it(begin());
		for(; !(it == end()); ++it)
		{
			entry = (*it).first + ' ' + (*it).second;
			entry_list.push_back(entry);
		}

		entry_list.sort();

		std::list<String>::iterator	list_it = entry_list.begin();
		for (; list_it != entry_list.end(); ++list_it) 
		{
	  	stream << *list_it << endl;
		}

		stream << "-----------------------------------" << endl;

		entry_list.clear();
	}


} // namespace BALL
