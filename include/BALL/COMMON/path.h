// $Id: path.h,v 1.4 2000/05/06 14:10:48 oliver Exp $

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
			directories where the data files needed by BALL reside.
			\\
			{\bf Definition:}\URL{BALL/COMMON/path.h}
			\\
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
				variable "BALL\_DATA" to the desired value.\\
				The environment variable always overrides the compiled
				settings.\\
				The default path may be accessed using \Ref{getDefaultDataPath}.
				The path may also be modified at runtime bu calling \Ref{setDataPath}.
				The path contains a colon separated list of paths that are searched 
				in the order of occurence.\\
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
				If {\bf name} contains relative path information but no file matching 
				this path could be found, another search is performed as a second step
				taking in account only the basename of the file.\\
				E.g.:\\
				Specifying {\bf data/test.dat} will search for data/test.dat in each
				data directory first. If this search doesn't yield a match, find will search 
				for a file named {\bf test.dat} in each of the directories.\\
				If this behaviour is not desired, try \Ref{findStrict} instead.\\
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
