// $Id: fileSystem.h,v 1.2 2000/01/06 09:46:05 oliver Exp $

#ifndef BALL_SYSTEM_FILESYSTEM_H
#define BALL_SYSTEM_FILESYSTEM_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#include <limits.h>
#include <sys/param.h>

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
			MAX_PATH_LENGTH = PATH_MAX
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
