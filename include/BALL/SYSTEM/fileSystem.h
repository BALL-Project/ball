// $Id: fileSystem.h,v 1.4 2001/07/31 00:45:05 oliver Exp $

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
			This class is a wrapper around some very basic properties
			of the machine's file system properties.
			The method (\Ref{canonizePath}) is mainly used
			by \Ref{File} to obtain a unique and unambiguous representation
			of a path.
	*/
	class FileSystem
	{
		public:
			
		/**	@name	Char Constants 
		*/
		//@{

		/**	The character separating directories in a path.
				This is usually {\tt '/'}.
		*/
		static const char PATH_SEPARATOR;

		/**	The string used to indicate the current directory.
				This is usually {\tt '.'}
		*/
		static const char* const CURRENT_DIRECTORY;

		/**	The string indicating the parent directory.
				This is usually {\tt '..'}
		*/
		static const char* const PARENT_DIRECTORY;
		//@}

		/**	@name	Enums
		*/
		enum
		{
			/** The maximum length of a filename.
			*/
			MAX_FILENAME_LENGTH = 256, // NAME_MAX seems to be too restrictive for modern Unixes
			/** The maximum length for a full path.
			*/
			MAX_PATH_LENGTH = BALL_PATH_MAX
		};

		/**	@name Static methods 
		*/
		//@{

		/** Convert a given filename to a canonical name.
				This method creates a unique and unambiguous representation
				of any absolute or relative path.
				It expands the user's homedirectory ({\tt '\~'}) and
				duplicate or redundant separators, e.g.
        '//' is reduced to /'/' and '/./' is removed.
		*/
		static void canonizePath(String& path)
			throw();

		//@}

		private:

		static void expandTilde_(String& path)
			throw();
	};

} // namespace BALL

#endif // BALL_SYSTEM_FILESYSTEM_H
