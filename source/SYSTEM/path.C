// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: path.C,v 1.6.20.1 2007/06/21 19:38:11 oliver Exp $
// 
// Author:
//   Oliver Kohlbacher
//

#include <BALL/COMMON/global.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/SYSTEM/file.h>
#include <BALL/SYSTEM/fileSystem.h>
#include <fstream>

#ifdef BALL_COMPILER_MSVC
#include <windows.h>
#endif

using std::ifstream;

namespace BALL 
{
	String Path::path_ = BALL_PATH;
	bool Path::path_array_valid_ = false;
	bool Path::environment_checked_ = false;
	std::vector<String> Path::path_array_;
	bool Path::initialized_ = false;

	void Path::reset()
	{
		path_ = getDefaultDataPath();
		path_array_valid_ = false;
		environment_checked_ = false;
	}

	Path::Path()
	{
		if (!initialized_) reset();
	}

	String Path::getDataPath()
	{
		buildPathArray_();
		return path_;
	}

	void Path::setDataPath(const String& path) 
	{
		path_array_valid_ = false;
		path_ = path;
		FileSystem::canonizePath(path_);
	}

	void Path::addDataPath(const String& path)
	{
		path_array_valid_ = false;
		path_ += "\n";

		String canonical_path(path);
		FileSystem::canonizePath(canonical_path);
		path_ += canonical_path;
	}

	String Path::getDefaultDataPath()
	{	
		String path = BALL_DATA_PATH;
		FileSystem::canonizePath(path);
		if (path[path.size()-1] != '/')
			path.append("/");
		return path;
	}

	void Path::buildPathArray_() 
	{	
		// everything is valid - return directly
		if (path_array_valid_)
		{
			return;
		}

		// remove old path array
		path_array_.clear();

		// check whether the environment variable BALL_DATA_PATH is set
		if (!environment_checked_)
		{
			char*	ball_data_path = getenv("BALL_DATA_PATH");
			if (ball_data_path != 0)
			{	
				String bdp(ball_data_path);
				bdp.append("\n");
				bdp.append(path_);
				setDataPath(bdp);
			}
#ifdef BALL_COMPILER_MSVC
			// we may have additional path information in the registry
			std::vector<unsigned char> regbuffer(MAX_PATH, 0);
			DWORD valuesize = regbuffer.size();

			REGSAM dSam = KEY_QUERY_VALUE;
			HKEY key;

			// first, look in the local user's part of the registry
			HKEY base = HKEY_CURRENT_USER;
			
			DWORD reg_result = RegOpenKeyEx(base, "Software\\BALL", 0, dSam, &key);
      
			if (reg_result == ERROR_SUCCESS)
				reg_result = RegQueryValueEx(key, "BALL_DATA_PATH", 0, 0, &(regbuffer[0]), &valuesize);

			// if we found BALL_DATA_PATH in the registry keys, reg_result will equal ERROR_SUCCESS
			if ((reg_result == ERROR_SUCCESS) && (valuesize > 0))
			{
				String bdp(reinterpret_cast<char*>(&(regbuffer[0])), 0, valuesize-1);
				addDataPath(bdp);
			}

			// and also look in the system wide settings
			valuesize = regbuffer.size();

			base = HKEY_LOCAL_MACHINE;
			reg_result = RegOpenKeyEx(base, "Software\\BALL", 0, dSam, &key);

			if (reg_result == ERROR_SUCCESS)
				reg_result = RegQueryValueEx(key, "BALL_DATA_PATH", 0, 0, &(regbuffer[0]), &valuesize);
			
			if ((reg_result == ERROR_SUCCESS) && (valuesize > 0))
			{
				String bdp(reinterpret_cast<char*>(&(regbuffer[0])), 0, valuesize-1);
				addDataPath(bdp);
			}
#endif
	
			// don`t try this again
			environment_checked_ = true;
		}
		
		// segment the path string and insert each path 
		// into the path array. append slashes where neccessary
		path_.split(path_array_, "\n");

		// rebuild the path from the array later on
		path_ = "";

		for (Position i=0; i<path_array_.size(); ++i)
		{
			String& current_path = path_array_[i];
			if (current_path.trim() != "")
			{
				// append a '/' if neccessary (just to be sure...)
				if (current_path[current_path.size()-1] != '/')
				{
					current_path.append("/");
				}

				path_ += current_path;

				if (i < path_array_.size() - 1)
					path_ += "\n";
			}
		}

		// remember we don't have to do this again - computation on demand!
		path_array_valid_ = true;
		initialized_ = true;
	}


	String Path::find(const String& name) 
	{
		if (name == "")
		{
			return name;
		}

		String result = findStrict(name);

		if (result == String("")) 
		{
			// if the full (path-specified) name could not be found,
			// only try the basename (remove leading directories)
			String tmp = name;
			tmp.erase(0, tmp.rfind('/') + 1);
			if (tmp != name)
			{
				result = findStrict(tmp);
			}
		}
			
		// return the path. "" means nothing found
		return result;
	}
		
	String Path::findStrict(const String& name)
	{
		// first, try the path itself
		if (File::isAccessible(name))
		{
			return name;
		}

		// rebuild the path array (only if not already done so!)
		buildPathArray_();

		// iterate over all path entries and check for 
		// a file of the desired name...
		vector<String>::iterator path_it = path_array_.begin();
		String filename;
		for (; path_it != path_array_.end(); ++path_it)
		{
			filename = *path_it + name;

			// if the file could be opened, we return its name
			if (File::isAccessible(filename))
			{				
				return filename;
			}
		}
			
		// we didn't find anything - return an empty string
		return String("");
	}

} // namespace BALL
