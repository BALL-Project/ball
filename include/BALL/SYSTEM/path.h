// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: path.h,v 1.11 2004/02/23 17:26:08 anhi Exp $
//

#ifndef BALL_COMMON_PATH_H
#define BALL_COMMON_PATH_H

#ifndef BALL_CONFIG_CONFIG_H
#	include <BALL/CONFIG/config.h>
#endif

#include <string>
#include <vector>
using std::string;
using std::vector;


namespace BALL 
{
	/**	Data path management class.
			This class is intended to provide a unique interface to 
			directories where the data files needed by BALL reside.	 \par
	\ingroup System		
	*/
	class Path 
	{
		public:
		/**	@name Constructors and Destructors
		*/	
		//@{
		
		/**	Default constructor
		*/
		Path();
		//@}

		/** Returns the path to the current BALL data directory.
				This directory is set to a default value at compile time
				of the library (see configure to change this setting).
				It may be overridden at runtime by setting the environment
				variable "BALL\_DATA" to the desired value. \par
				The environment variable always overrides the compiled
				settings. \par
				The default path may be accessed using  \link Path::getDefaultDataPath getDefaultDataPath \endlink .
				The path may also be modified at runtime by calling  \link Path::setDataPath setDataPath \endlink .
				The path contains a colon separated list of paths that are searched 
				in the order of occurence. \par
		*/		
		string getDataPath();

		/**	Modify the data path.
				This method accepts a colon separated list of paths to
				specify data paths.
		*/
		void setDataPath(const string& path);

		/**	Add a single path to the list of paths.
				@param	path the path to be added to the path list
		*/
		void addDataPath(const string& path);

		/**	Returns the default data path compiled into the library.
				This method ignores possible contents of the environment
				variable "BALL\_DATA".
		*/
		string getDefaultDataPath();

		/**	Returns the full path to a file residing in one of the data directories.
				If a file that matches the name is not found, an empty string is returned.
				Directories are searched in the order of occurence in the data path.
				If <b>name</b> contains relative path information but no file matching 
				this path could be found, another search is performed as a second step
				taking in account only the basename of the file. \par
				E.g.: \par
				Specifying <b>data/test.dat</b> will search for data/test.dat in each
				data directory first. If this search doesn't yield a match, find will search 
				for a file named <b>test.dat</b> in each of the directories. \par
				If this behaviour is not desired, try  \link Path::findStrict findStrict \endlink  instead. \par
		*/
		string find(const string& name);

		/**	Returns the full path to a file residing in one of the data directories.
				@see find
		*/
		string findStrict(const string& name);

		
		protected:
				
		void buildPathArray_();
		string path_;
		bool path_array_valid_;
		bool environment_checked_;
		std::vector<std::string>	path_array_;
	};
  
} // namespace BALL


#endif // BALL_COMMON_PATH_H
