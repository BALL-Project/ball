// $Id: path.C,v 1.1 1999/08/26 08:02:33 oliver Exp $

#include <BALL/COMMON/global.h>
#include <BALL/COMMON/path.h>
#include <fstream.h>


namespace BALL {

	
	string					Path::path_ = BALL_PATH"/data/";
	bool						Path::path_array_valid_ = false;
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

	void Path::buildPathArray_(void) 
	{	
		// everything is valid - return directly
		if (path_array_valid_)
			return;

		// remove old path array
		path_array_.clear();

		// segment the path string and insert each path 
		// into the path array. append slashes where neccessary
		string tmp = path_ + ":";
		Size position;

		while ((position = tmp.find(":")) != string::npos) {
			// extract the next path...
			string path = tmp.substr(0, position - 1);

			// append a '/' if neccessary (just to be sure...)
			if (path[path.size() - 1] != '/')
				path.append("/");

			// store the path...
			path_array_.push_back(path);
			tmp.erase(0, position + 1);
		}

		// remember we don't have to do this again - computation on demand!
		path_array_valid_ = true;
	}


	string Path::find(const string& name) 
	{
		string	result;

		if ((result = findStrict(name)) == string("")) 
		{
			// if the full (path-specified) name could not be found,
			// only try the basename (remove leading directories)
			string tmp = name;
			tmp.erase(0, tmp.rfind("/") + 1);
			if (tmp != name)
				result = findStrict(tmp);
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

		// iterate over all path entries and check for 
		// a file of the desired name...
		for (; path_it != path_array_.end(); ++path_it)
		{
			filename = *path_it + name;
			ifstream file(filename.c_str());
			
			// if the file could be opened, we return its name
			if (file)
				return filename;
		}
			
		// we didn't find anything - return an empty string
		return string("");
	}

} // namespace BALL
