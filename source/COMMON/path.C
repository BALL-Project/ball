// $Id: path.C,v 1.3 1999/09/17 11:17:05 oliver Exp $

#include <BALL/COMMON/global.h>
#include <BALL/COMMON/path.h>
#include <fstream.h>
#include <stdlib.h>

namespace BALL 
{
	
	string					Path::path_ = BALL_PATH"/data/";
	bool						Path::path_array_valid_ = false;
	bool						Path::environment_checked_ = false;
	vector<string>	Path::path_array_;		
	
	
	string Path::getDataPath()
	{
		return path_;
	}

	void Path::setDataPath(const string& path) 
	{
		path_array_valid_ = false;
		path_ = path;
	}

	string Path::getDefaultDataPath()
	{	
		return BALL_PATH"/data/";
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
				path_ = string(ball_data_path) + ":" + path_;
			}
			
			// don`t try this again
			environment_checked_ = true;
		}

		// segment the path string and insert each path 
		// into the path array. append slashes where neccessary
		string tmp = path_ + ":";
		Size position;
		while ((position = tmp.find(":")) != string::npos) 
		{
			// extract the next path...
			string path = tmp.substr(0, position);

			// append a '/' if neccessary (just to be sure...)
			if (path[path.size() - 1] != '/')
			{
				path.append("/");
			}

			// store the path...
			path_array_.push_back(path);
			tmp.erase(0, position + 1);
		}

		// remember we don't have to do this again - computation on demand!
		path_array_valid_ = true;
	}


	string Path::find(const string& name) 
	{
		string	result = findStrict(name);

		if (result == string("")) 
		{
			// if the full (path-specified) name could not be found,
			// only try the basename (remove leading directories)
			string tmp = name;
			tmp.erase(0, tmp.rfind("/") + 1);
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
		// rebuild the path array (only if not already done so!)
		buildPathArray_();

		vector<string>::iterator	path_it = path_array_.begin();
		string filename;

		// first, try the path itself
		ifstream file(name.c_str());
		if (file)
		{
			file.close();
			return name;
		}
		file.close();

		// iterate over all path entries and check for 
		// a file of the desired name...
		for (; path_it != path_array_.end(); ++path_it)
		{
			filename = *path_it + name;
			file.open(filename.c_str());
			
			// if the file could be opened, we return its name
			if (file)	
			{
				file.close();
				return filename;
			}
			file.close();
		}
			
		// we didn't find anything - return an empty string
		return string("");
	}

} // namespace BALL
