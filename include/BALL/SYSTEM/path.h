// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: path.h,v 1.13.20.1 2007/06/21 19:38:10 oliver Exp $
//
// Author:
//   Oliver Kohlbacher
//

#ifndef BALL_COMMON_PATH_H
#define BALL_COMMON_PATH_H

#ifndef BALL_CONFIG_CONFIG_H
#	include <BALL/CONFIG/config.h>
#endif

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
# include <BALL/DATATYPE/string.h>
#endif

#include <vector>

using std::vector;

namespace BALL 
{
	/**	Data path management class.
			This class is intended to provide a unique interface to 
			directories where the data files needed by BALL reside.	 \par
			Path is a singleton, so there is only one unique application-wide
			instance of and changes to this will be seen by all classes using
			path.
	\ingroup System		
	*/
	class BALL_EXPORT Path 
	{
		public:
		/**	@name Constructors and Destructors
		*/	
		//@{
		
		/**	Default constructor
		*/
		Path();
		//@}

		/** Return a list of paths to the BALL data directories.
				This directory is set to a default value at compile time
				of the library.
				It may be overridden at runtime by setting the environment
				variable "BALL_DATA" to the desired value. This value
        is then prepended to the list of directories. 
        Directories are separated by linefeeds and returned 
        as a single string. To locate files in these directories,
        \link Path::find find \endlink will search the directories
        in the specified order.\par

				The default path may be accessed using  \link Path::getDefaultDataPath getDefaultDataPath \endlink .
				The path may also be modified at runtime by calling  \link Path::setDataPath setDataPath \endlink .\par

				The path contains a newline ("\n") separated list of paths that are searched 
				in the order of occurence. \par
		*/		
		String getDataPath();

		/**	Modify the data path.
				This method accepts a newline separated list of paths to
				specify data paths.
		*/
		void setDataPath(const String& path);

		/**	Add a single path to the list of paths.
				@param	path the path to be added to the path list
		*/
		void addDataPath(const String& path);

		/**	Returns the default data path compiled into the library.
				This method ignores possible contents of the environment
				variable "BALL\_DATA".
		*/
		String getDefaultDataPath();

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
		String find(const String& name);

		/**	Returns the full path to a file residing in one of the data directories.
				@see find
		*/
		String findStrict(const String& name);

		/// Reset the path variable to its default state (as it was a the start of the application).
		void reset();

		
		protected:
				
		void buildPathArray_();

		static String path_;
		static bool path_array_valid_;
		static bool environment_checked_;
		static std::vector<String>	path_array_;
		static bool initialized_;
	};
  
} // namespace BALL


#endif // BALL_COMMON_PATH_H
