// $Id: file.h,v 1.16 2000/12/14 16:54:14 anker Exp $

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
			\\
			{\bf Definition:} \URL{BALL/SYSTEM/file.h}
			\\
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
		//@{

		/// Open for input (default)
		static const OpenMode IN = std::ios::in;
		/// Open for output
		static const OpenMode OUT = std::ios::out;
		/// Append. Seek to end before each write operation
		static const OpenMode APP = std::ios::app;
		/// Binary mode
		static const OpenMode BINARY = std::ios::binary;
		/// Seek to end directly after opening.
		static const OpenMode ATE = std::ios::ate;
		/// Truncate an existing file.
		static const OpenMode TRUNC = std::ios::trunc;
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
		File()
			throw();

		/** Detailed constructor.
				Construct new File object from the file {\em name}
				and open the file.
				@param  name the name of the file to be opend
				@param  OpenMode the openmode to be used
				@see    open
				@return File - new constructed File object
		*/
		File(const String& name, OpenMode open_mode = IN)
			throw (Exception::FileNotFound);

		/** Copy constructor.
				Construct new File object by copying the filename from File {\em file}.
				The file is not opend.
				@param  file the File object to be copied (cloned)
				@see    open
				@return File - new constructed File object cloned from {\em file}
		*/
		File(const File& file)
			throw (Exception::FileNotFound);

		/** Destructor.
				Default destruction of {\em *this} File.
				The file is closed.
		*/
		virtual ~File();

		//@}

		/**	@name	Assignment */
		//@{

		/** Assignment operator.
				Assign the filename from File {\em file} to {\em *this} File.
				The file is not opend.
		*/
		const File& operator = (const File& file)
			throw();

		//@}

		/**	@name	Accessors */
		//@{

		/** Clears the File object.
		*/
		void clear()
			throw();

		/**	Open a given file.
				The standard constructor uses this method.
				@param name the name of the file
				@param open_mode the open mode, default is IN
				@return bool true if the file could be opend
		*/
		bool open(const String& name, OpenMode open_mode = IN)
			throw (Exception::FileNotFound);

		/**	Reopen the file.
				The file is closed and reopend.
				@return bool true if the file could be reopend
		*/
		bool reopen()
			throw (Exception::FileNotFound);

		/**	Close the file.
		*/
		void close()
			throw();

		/**	Return the name of the file.
				@return String the name of the file
		*/
		const String& getName()
			const	throw();

		/**	Return the size of the file.
				If the file does not exist 0 is returned.
				@return Size the size of the file
		*/
		Size getSize()
			throw();

		/**	Return the size of a given file.
				@exception FileNotFound if file does not exists.
				@return Size the size of the file
		*/
		static Size getSize(String name)
			throw (Exception::FileNotFound);

		/** Return the open mode.
				Default is IN.
				@return int the open mode
		*/
		File::OpenMode getOpenMode()
			const	throw();
		
		/**	Return the filetype of a given file.
				@param name the name of the file.
				@param trace_link true to follow links
				@return Type the filetype
		*/
		static Type getType(String name, bool trace_link)
			throw(Exception::FileNotFound);

		/**	Return the filetype.
				@param trace_link true to follow links
				@return Type the filetype
		*/
		Type getType(bool trace_link)
			const	throw(Exception::FileNotFound);
		
		/**	Copy a given file to a given destination.
				If a file with the destination name exists allready, nothing happens.
				@param source_name the name of the source file
				@param destination_name the name of the destination file
				@param buffer_size the buffer size to use while copying
				@return true if copying was successfull
		*/
		static bool copy
			(String source_name, String destination_name, Size buffer_size = 4096)
			throw(Exception::FileNotFound);

		/**	Copy the file this File object is associated with to a given destination.
				If a file with the destination name exists allready, nothing happens.
				@param destination_name the name of the destination file
				@param buffer_size the buffer size to use while copying
				@return true if copying was successfull
		*/
		bool copyTo(const String& destination_name, Size buffer_size = 4096)
			throw(Exception::FileNotFound);

		/**	Move a given file to a given destination.
				If a file with the destination name exists allready, nothing happens.
				@param source_name the name of the source file
				@param destination_name the name of the destination file
				@param buffer_size the buffer size to use while moving
				@return true if copying was successfull
		*/
		static bool move(const String& source_name, const String& destination_name)
			throw(Exception::FileNotFound);

		/**	Move the file this File object is associated with to a given destination.
				If a file with the destination name exists allready, nothing happens.
				@param destination_name the name of the destination file
				@param buffer_size the buffer size to use while moving
				@return true if copying was successfull
		*/
		bool moveTo(const String& destination_name)
			throw(Exception::FileNotFound);

		/**	Remove the file "name".
				@param name the name of the file to be removed
				@return bool true if the file could be removed
		*/
		static bool remove(String name)
			throw();

		/**	Remove the file, this File object is associated with.
				@return bool true if the file could be removed
		*/
		bool remove()
			throw();

		/**	Rename a file.
				@param old_path the path and name of the file to be renamed
				@param new_path the new path and name of the file
				@return bool true if the file could be renamed
		*/
		static bool rename(String old_path, String new_path)
			throw (Exception::FileNotFound);

		/**	Rename the file this File object is associated with to a given name.
				If a file with the destination name exists allready, nothing happens.
				@param new_path the new path and name of the file
				@return bool true if the file could be renamed
		*/
		bool renameTo(const String& new_path)
			throw (Exception::FileNotFound);

		/**	Truncate a given file.
				@param path the path to the file
				@param size the new size of the file
				@return bool true if the file could be truncated
		*/
		static bool truncate(String path, Size size = 0)
			throw (Exception::FileNotFound);

		/**	Truncate the file this File object is associated with.
				@param size the new size of the file
				@return bool true if the file could be truncated
		*/
		bool truncate(Size size = 0)
			const throw (Exception::FileNotFound);
			
		/**	Create a temporary filename.
				This method creates strings, starting at _AAAAAAA.TMP and tries if a 
				file with this name exists. If not the string is returned. If a file
				with this name exists, it continues to create names up to _ZZZZZZZ.TMP.
				@param temporary reference to the temporary filename
				@return bool true if a temporary filename could be found
		*/
		static bool createTemporaryFilename(String& temporary)
			throw();

    /** Return the stream associated with this file.
				Implemented just for convenience.
        @return std::fstream the stream
    */
    std::fstream& getFileStream();

		//@}

		/**	@name Predicates 
		*/
		//@{

		/**	Equality comparison operator.
				Two File objects are equal if they point to the same canonzied filename.
		*/
		bool operator == (const File& file)
			const	throw();
		
		/**	Inequality comparison operator.
				Two File objects are equal if they point not to the same canonized filename.
		*/
		bool operator != (const File& file)
			const	throw();

		//@}

		/**	Test if the file is opend.
				The standard constructor opens the file.
				@return bool true if the file is closed
		*/
		bool isOpen()
			const	throw();

		/**	Test if the file is closed.
				The standard constructor opens the file.
				@return bool true if the file is closed
		*/
		bool isClosed()
			const	throw();

		/**	Test if a given file can be accessed.
				@param name the name of the file to be tested
				@return bool true if the file can be accessed
				@exception FileNotFound is thrown if name is an empty string
		*/
		static bool isAccessible(String name)
			throw (Exception::FileNotFound);

		/**	Test if the file can be accessed.
				@return bool true if the file can be accessed
				@exception FileNotFound is thrown if name_ is an empty string
		*/
		bool isAccessible()
			const throw (Exception::FileNotFound);

		/**	Test if the path of the file is canonized.
				The path is	compared before and after call of 
				FileSystem::canonizePath(canonized_name).
				@see FileSystem::canonizePath
				@return bool true if the path is cononized.
		*/
		bool isCanonized()
			const throw (Exception::FileNotFound);
	
		/**	Test if a given file is readable.
				@param name the name of the file
				@return true if the file can be read
		*/
		static bool isReadable(String name)
			throw (Exception::FileNotFound);

		/**	Test if the file is readable.
				@return true if the file can be read
		*/
		bool isReadable()
			const throw (Exception::FileNotFound);

		/**	Test if a given file is writeable.
				@param name the name of the file
				@return true if the file is writeable
		*/
		static bool isWritable(String name)
			throw (Exception::FileNotFound);

		/**	Test if the file is writeable.
				@return true if the file is writeable
		*/
		bool isWritable()
			const throw (Exception::FileNotFound);

		/**	Test if a given file is executable.
				@param name the name of the file
				@return true if the file is executable
		*/
		static bool isExecutable(String name)
			throw (Exception::FileNotFound);

		/**	Test if the file is executable.
				@return true if the file is executable
		*/
		bool isExecutable()
			const throw (Exception::FileNotFound);
 
		void setName(const String& name)
			throw (Exception::FileNotFound);

		/**	@name	Debugging and Diagnostics */
		//@{

		/**	Test if the file ist valid.
				This function uses std::fstream::good().
				@return bool true if the file is valid
		*/
		bool isValid()
			const	throw();

		//@}

		private:

		String		name_;
		OpenMode	open_mode_;
		bool			is_open_;
		bool			is_temporary_;
	};

#	ifndef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/SYSTEM/file.iC>
#	endif

} // namespace BALL

#endif // BALL_SYSTEM_FILE_H
