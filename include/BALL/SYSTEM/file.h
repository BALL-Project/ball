// $Id: file.h,v 1.10 2000/07/12 19:42:33 oliver Exp $

#ifndef BALL_SYSTEM_FILE_H
#define BALL_SYSTEM_FILE_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#include <fstream>
#include <stdlib.h>			// 'getenv'
#include <sys/types.h>
#include <sys/stat.h>		// 'stat', 'lstat'
#include <stdio.h>			// 'rename'
#include <unistd.h>			// 'access', 'rename', 'truncate'

#ifndef BALL_DATATYPE_REGEXP_H
#	include <BALL/DATATYPE/regExp.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
#	include <BALL/DATATYPE/string.h>
#endif

#ifndef BALL_SYSTEM_FILESYSTEM_H
#	include <BALL/SYSTEM/fileSystem.h>
#endif

namespace BALL 
{
	
	/**	File Class.
	*/
	class File
		: public std::fstream
	{
		public:

		/**	@name	Type definitions
		*/
		//@{
			
		/**	File open modes.
				This type is used to describe the standard openmodes for files
				as described in Section 27.4.2.1.4 of the ANSI C++ standard.
		*/
		typedef std::ios::openmode OpenMode;			
		//@}
		
		/**	@name	Constants
		*/
		//{
		/// Open for input (default)
		static const OpenMode IN;
		/// Open for output
		static const OpenMode OUT;
			/// Append. Seek to end before each write operation
		static const OpenMode APP;
			/// Binary mode
		static const OpenMode BINARY;
			/// Seek to end directly after opening.
		static const OpenMode ATE;
			/// Truncate an existing file.
		static const OpenMode TRUNC;
		//@}

		/**	@name	Enums
		*/
		//@{

		///
		enum ProtectionMode
		{
			///
			PROTECTION_MODE__INVALID                   = -1,
			///
			PROTECTION_MODE__UNKNOWN                   = 0,
			///
			PROTECTION_MODE__MASK                      = S_IRWXU | S_IRWXG | S_IRWXO,
			///
			PROTECTION_MODE__OWNER_READ_WRITE_EXECUTE  = S_IRWXU,
			///
			PROTECTION_MODE__OWNER_READ                = S_IRUSR,
			///
			PROTECTION_MODE__OWNER_WRITE               = S_IWUSR,
			///
			PROTECTION_MODE__OWNER_EXECUTE             = S_IXUSR,
			///
			PROTECTION_MODE__GROUP_READ_WRITE_EXECUTE  = S_IRWXG,
			///
			PROTECTION_MODE__GROUP_READ                = S_IRGRP,
			///
			PROTECTION_MODE__GROUP_WRITE               = S_IWGRP,
			///
			PROTECTION_MODE__GROUP_EXECUTE             = S_IXGRP,
			///
			PROTECTION_MODE__OTHER_READ_WRITE_EXECUTE  = S_IRWXO,
			///
			PROTECTION_MODE__OTHER_READ                = S_IROTH,
			///
			PROTECTION_MODE__OTHER_WRITE               = S_IWOTH,
			///
			PROTECTION_MODE__OTHER_EXECUTE             = S_IXOTH
		};

		/**	
		*/
		enum Type
		{
			///
			TYPE__UNKNOWN            = 0,
			///
			TYPE__DIRECTORY          = 1,
			///
			TYPE__CHAR_SPECIAL_FILE  = 2,
			///
			TYPE__BLOCK_SPECIAL_FILE = 3,
			///
			TYPE__REGULAR_FILE       = 4,
			///
			TYPE__SYMBOLIC_LINK      = 5,
			///
			TYPE__SOCKET             = 6,
			///
			TYPE__FIFO_SPECIAL_FILE  = 7
		};
		//@}

		/**	@name	Constructors and Destructors
		*/
		//@{

		/** Default constructor.
				Construct new File object.
				@return    File - new constructed File object
		*/
		File();

		/** Detailed constructor.
				Construct new File object from the file {\em name}
				and open the file.
				@param  name the name of the file to be opend
				@param  OpenMode the openmode to be used
				@see    open
				@return File - new constructed File object
		*/
		File(const String& name, OpenMode open_mode = IN);

		/** Copy constructor.
				Construct new File object by copying the File {\em file}
				and open the file.
				@param  file the File object to be copied (cloned)
				@see    open
				@return File - new constructed File cloned from {\em file}
		*/
		File(const File& file);

		/** Destructor.
				Default destruction of {\em *this} File.
				The file is closed.
		*/
		virtual ~File();

		//@}

		/**	@name	Assignment */
		//@{

		/** Assign a file to this object.
				@param name the name of the file
				@return bool true if this file is valid
		*/
		bool set(const String& name);

		/** Assignment with cloning facility.
				Assign a File object to this object.
				@param file the file object to assign
				@return bool true if this file is valid
		*/
		bool set(const File& file);

		/** Assignment operator.
				Assign the File {\em file} to {\em *this} File.
		*/
		File& operator = (const File& file);

		//@}

		/**	@name	Accessors */
		//@{

		/**	Open a given file.
				The standard constructor uses this method.
				@param name the name of the file
				@param open_mode the open mode, default is IN
				@return bool true if the file could be opend
		*/
		bool open(const String& name, OpenMode open_mode = IN);

		/**	Reopen the file.
				The file is closed and reopend.
				@return bool true if the file could be reopend
		*/
		bool reopen();

		/**	Close the file.
		*/
		void close();

		/**	Return the name of the file.
				@return String the name of the file
		*/
		const String& getName() const;

		/**	Return the size of the file.
				If the file does not exist 0 is returned.
				@return Size the size of the file
		*/
		Size getSize();

		/**	Return the size of a given file.
				If the file does not exist 0 is returned.
				@return Size the size of the file
		*/
		static Size getSize(String name);

		/** Return the open mode.
				Default is IN.
				@return int the open mode
		*/
		File::OpenMode getOpenMode() const;
		
		/**	Return the filetype of a given file.
				@param name the name of the file.
				@param trace_link true to follow links
				@return Type the filetype
		*/
		static Type getType(String name, bool trace_link);

		/**	Return the filetype.
				@param trace_link true to follow links
				@return Type the filetype
		*/
		Type getType(bool trace_link) const;

		/**	Return the stream associated with this file.
				@return std::fstream the stream
		*/
		std::fstream& getFileStream();
		
		/**	Copy a given file to a given destination.
				If a file with the destination name exists allready, nothing happens.
				@param source_name the name of the source file
				@param destination_name the name of the destination file
				@param buffer_size the buffer size to use while copying
				@return true if copying was successfull
		*/
		static bool copy
			(String source_name, String destination_name, Size buffer_size = 4096);

		/**	Copy the file this File object is associated with to a given destination.
				If a file with the destination name exists allready, nothing happens.
				@param destination_name the name of the destination file
				@param buffer_size the buffer size to use while copying
				@return true if copying was successfull
		*/
		bool copyTo(const String& destination_name, Size buffer_size = 4096);

		/**	Move a given file to a given destination.
				If a file with the destination name exists allready, nothing happens.
				@param source_name the name of the source file
				@param destination_name the name of the destination file
				@param buffer_size the buffer size to use while moving
				@return true if copying was successfull
		*/
		static bool move(const String& source_name, const String& destination_name);

		/**	Move the file this File object is associated with to a given destination.
				If a file with the destination name exists allready, nothing happens.
				@param destination_name the name of the destination file
				@param buffer_size the buffer size to use while moving
				@return true if copying was successfull
		*/
		bool moveTo(const String& destination_name);

		/**	Remove the file "name".
				@param name the name of the file to be removed
				@return bool true if the file could be removed
		*/
		static bool remove(String name);

		/**	Remove the file, this File object is associated with.
				@return bool true if the file could be removed
		*/
		bool remove() const;

		/**	Rename a file.
				@param old_path the path and name of the file to be renamed
				@param new_path the new path and name of the file
				@return bool true if the file could be renamed
		*/
		static bool rename(String old_path, String new_path);

		/**	Rename the file this File object is associated with to a given name.
				If a file with the destination name exists allready, nothing happens.
				@param new_path the new path and name of the file
				@return bool true if the file could be renamed
		*/
		bool renameTo(const String& new_path);

		/**	Truncate a given file.
				@param path the path to the file
				@param size the new size of the file
				@return bool true if the file could be truncated
		*/
		static bool truncate(String path, Size size = 0);

		/**	Truncate the file this File object is associated with.
				@param size the new size of the file
				@return bool true if the file could be truncated
		*/
		bool truncate(Size size = 0) const;
			
		/**	Create a temporary filename.
				This method creates strings, starting at _AAAAAAA.TMP and tries if a 
				file with this name exists. If not the string is returned. If a file
				with this name exists, it continues to create names up to _ZZZZZZZ.TMP.
				@param temporary reference to the temporary filename
				@return bool true if a temporary filename could be found
		*/
		static bool createTemporaryFilename(String& temporary);

		//@}

		/**	@name Predicates 
		*/
		//@{

		/**	Equality comparison operator.
				Two File objects are equal if they point to the same file.
		*/
		bool operator == (const File& file) const;
		
		/**	Inequality comparison operator.
				Two File objects are equal if they point not to the same file.
		*/
		bool operator != (const File& file) const;

		//@}

		/**	Test if the file is opend.
				The standard constructor opens the file.
				@return bool true if the file is closed
		*/
		bool isOpen() const;

		/**	Test if the file is closed.
				The standard constructor opens the file.
				@return bool true if the file is closed
		*/
		bool isClosed() const;

		/**	Test if a given file can be accessed.
				@param name the name of the file to be tested
				@return bool true if the file can be accessed
		*/
		static bool isAccessible(String name);

		/**	Test if the file can be accessed.
				@return bool true if the file can be accessed
		*/
		bool isAccessible() const;

		/**	Test if the path of the file is canonize.
				The path is	compared before and after call of 
				FileSystem::canonizePath(canonized_name).
				@see FileSystem::canonizePath
				@return bool true if the path is cononized.
		*/
		bool isCanonized() const;
	
		/**	Test if a given file is readable.
				@param name the name of the file
				@return true if the file can be read
		*/
		static bool isReadable(String name);

		/**	Test if the file is readable.
				@return true if the file can be read
		*/
		bool isReadable() const;

		/**	Test if a given file is writeable.
				@param name the name of the file
				@return true if the file is writeable
		*/
		static bool isWritable(String name);

		/**	Test if the file is writeable.
				@return true if the file is writeable
		*/
		bool isWritable() const;

		/**	Test if a given file is executable.
				@param name the name of the file
				@return true if the file is executable
		*/
		static bool isExecutable(String name);

		/**	Test if the file is executable.
				@return true if the file is executable
		*/
		bool isExecutable() const;

		/**	@name	Debugging and Diagnostics */
		//@{

		/**	Test if the file ist valid.
				This function uses std::fstream::good().
				@return bool true if the file is valid
		*/
		bool isValid() const;

		//@}

		protected:

		String name_;

		private:
		OpenMode	open_mode_;
		bool			is_open_;
		bool			is_temporary_;
	};

#	ifndef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/SYSTEM/file.iC>
#	endif

} // namespace BALL

#endif // BALL_SYSTEM_FILE_H
