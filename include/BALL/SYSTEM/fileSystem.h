// $Id: fileSystem.h,v 1.3 2001/05/10 13:47:30 oliver Exp $

#ifndef BALL_SYSTEM_FILESYSTEM_H
#define BALL_SYSTEM_FILESYSTEM_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#include <limits.h>
#include <sys/param.h>

// if PATH_MAX is undefined (which sometimes happens on systems
// where there is no unambiguous value defined due to severel differing
// filesystems), we define it for ourselves to be 1kB
#ifndef PATH_MAX
# define BALL_PATH_MAX 1023
#else
# define BALL_PATH_MAX PATH_MAX
#endif

#ifndef BALL_DATATYPE_STRING_H
#	include <BALL/DATATYPE/string.h>
#endif

namespace BALL 
{

	/**	File System Class
	*/
	class FileSystem
	{
		public:
			
		/**	@name	Char Constants 
		*/
		//@{

		/**	
		*/
		static const char PATH_SEPARATOR;

		/**	
		*/
		static const char* const CURRENT_DIRECTORY;

		/**	
		*/
		static const char* const PARENT_DIRECTORY;
		//@}

		/**	@name	Enums
		*/
		enum
		{
			/**
			*/
			MAX_FILENAME_LENGTH = 256, // NAME_MAX seems to be too restrictive for modern Unixes
			/**
			*/
			MAX_PATH_LENGTH = BALL_PATH_MAX
		};

		/**	@name Miscellaneous 
		*/
		//@{

		/**
		*/
		static String& canonizePath(String& path);

		//@}

		protected:

		private:

		static String& expandTilde_(String& path);

	};

} // namespace BALL

#endif // BALL_SYSTEM_FILESYSTEM_H
