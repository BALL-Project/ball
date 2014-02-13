// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: fileSystem.h,v 1.14 2005/07/29 12:38:14 amoll Exp $
//

#ifndef BALL_SYSTEM_FILESYSTEM_H
#define BALL_SYSTEM_FILESYSTEM_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifdef BALL_HAS_SYS_PARAM_H
#	include <sys/param.h>
#endif

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
			The method ( \link FileSystem::canonizePath canonizePath \endlink ) is mainly used
			by  \link File File \endlink  to obtain a unique and unambiguous representation
			of a path.
\ingroup System
	*/
	class BALL_EXPORT FileSystem
	{
		public:
			
		/**	@name	Char Constants 
		*/
		//@{

		/**	The character separating directories in a path.
				This is usually <tt>'/'</tt>.
		*/
		static const char PATH_SEPARATOR;

		/**	The string used to indicate the current directory.
				This is usually <tt>'.'</tt>
		*/
		static const char* const CURRENT_DIRECTORY;

		/**	The string indicating the parent directory.
				This is usually <tt>'..'</tt>
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
				It expands the user's homedirectory (<tt>'~'</tt>) and
				duplicate or redundant separators, e.g.
        '//' is reduced to /'/' and '/./' is removed.
				Also, all occurrences of \link FileSystem::PATH_SEPARATOR PATH_SEPARATOR \endlink 
				are canonized to '/'.
				NOTE: if the path starts with '\\', we do not canonize further and assume it to
				      be a UNC path.
		*/
		static void canonizePath(String& path);

		/** Return the base name of a file.
				This strips the path from the filename, i.e. everything
				before and including the last occurence of  \link FileSystem::PATH_SEPARATOR PATH_SEPARATOR \endlink .
		*/
		static String baseName(const String& filename);

		/** Return the extension of a file of present.
				If file has no extension an empty String is returned.
		*/
		static String fileExtension(const String& filename);

		/** Return the path to a file.
				This method returns the path to a file, i.e. everything up to
				and including the last occurence of  \link FileSystem::PATH_SEPARATOR PATH_SEPARATOR \endlink .
		*/
		static String path(const String& filename);
		//@}

		private:

		static void expandTilde_(String& path);
	};
  
} // namespace BALL

#endif // BALL_SYSTEM_FILESYSTEM_H
