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
	string Path::path_ = BALL_PATH;
	bool Path::path_array_valid_ = false;
	bool Path::environment_checked_ = false;
	std::vector<string> Path::path_array_;
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

	string Path::getDataPath()
	{
		buildPathArray_();
		return path_;
	}

	void Path::setDataPath(const string& path) 
	{
		path_array_valid_ = false;
		path_ = path;
	}

	void Path::addDataPath(const string& path)
	{
		path_array_valid_ = false;
		path_ += "\n";
		path_ += path;
	}

	string Path::getDefaultDataPath()
	{	
		string path = BALL_PATH;
		path = path + FileSystem::PATH_SEPARATOR + "data" + FileSystem::PATH_SEPARATOR;
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
				string bdp(ball_data_path);
				bdp.append("\n");
				bdp.append(path_);
				setDataPath(bdp);
			}
#ifdef BALL_COMPILER_MSVC
			// we may have additional path information in the registry
			std::vector<unsigned char> regbuffer(MAX_PATH, 0);
			DWORD valuesize;
			DWORD regresult;

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
				std::string bdp(reinterpret_cast<char*>(&(regbuffer[0])), valuesize-1);
				addDataPath(bdp);
			}

			// and also look in the system wide settings
			base = HKEY_LOCAL_MACHINE;
			reg_result = RegOpenKeyEx(base, "Software\\BALL", 0, dSam, &key);

			if (reg_result == ERROR_SUCCESS)
				reg_result = RegQueryValueEx(key, "BALL_DATA_PATH", 0, 0, &(regbuffer[0]), &valuesize);
			
			if ((reg_result == ERROR_SUCCESS) && (valuesize > 0))
			{
				std::string bdp(reinterpret_cast<char*>(&(regbuffer[0])), valuesize-1);
				addDataPath(bdp);
			}
#endif
	
			// don`t try this again
			environment_checked_ = true;
		}

		
		// segment the path string and insert each path 
		// into the path array. append slashes where neccessary
		string tmp = path_ + "\n";
		string::size_type position = tmp.find("\n");
		while (position != string::npos) 
		{
			// extract the next path...
			string path = tmp.substr(0, position);

			// append a '/' if neccessary (just to be sure...)
			if (path[path.size() - 1] != FileSystem::PATH_SEPARATOR)
			{
				path.append(String(FileSystem::PATH_SEPARATOR));
			}

			// store the path...
			path_array_.push_back(path);
			tmp.erase(0, position + 1);

			// find the next occurence	
			position = tmp.find("\n");
		}

		// remember we don't have to do this again - computation on demand!
		path_array_valid_ = true;
	}


	string Path::find(const string& name) 
	{
		if (name == "")
		{
			return name;
		}

		string result = findStrict(name);

		if (result == string("")) 
		{
			// if the full (path-specified) name could not be found,
			// only try the basename (remove leading directories)
			string tmp = name;
			tmp.erase(0, tmp.rfind(FileSystem::PATH_SEPARATOR) + 1);
			if (tmp != name)
			{
				result = findStrict(tmp);
			}
		}
			
		// return the path. "" means nothing found
		return result;
	}
		
	string Path::findStrict(const string& name)
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
		vector<string>::iterator path_it = path_array_.begin();
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
		return string("");
	}

} // namespace BALL
