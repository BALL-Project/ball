// $Id: file.h,v 1.7 2000/06/09 12:30:17 amoll Exp $

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
		typedef std::ios::openmode OpenMode;
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
		enum Protocol
		{
			///
			PROTOCOL__FILE   = 1,
			///
			PROTOCOL__EXEC   = 2,
			///
			PROTOCOL__ACTION = 3
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
				@param  OpenMode the std::ios::openmode to be used
				@see    open
				@return File - new constructed File object
		*/
		File(const String& name, OpenMode open_mode = std::ios::in);

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
			
			
		/**	
		*/
		static void enableProtocol(Protocol protocol);

		/**	
		*/
		static void disableProtocol(Protocol protocol);

		/**	Open a given file.
				The standard constructor uses this method.
				@param name the name of the file
				@param open_mode the open mode, default is std::ios::in
				@return bool true if the file could be opend
		*/
		bool open(const String& name, OpenMode open_mode = std::ios::in);

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

		/**
		*/
		const String& getOriginalName() const;

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

		/**	
		*/
		int getOpenMode() const;
		
		/**	
		*/
		static Type getType(String name, bool trace_link);

		/**	
		*/
		Type getType(bool trace_link) const;

		/**	
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

		/**	
		*/
		static void registerAction(const String& pattern, const String& exec);
			
		/**	
		*/
		static void unregisterAction(const String& pattern);
			
		/**	
		*/
		static bool createTemporaryFilename(String& temporary);

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

		/**	
		*/
		static bool isProtocolEnabled(Protocol protocol);

		/**	Test if the file is opend.
				The standard constructor opens the file.
		*/
		bool isOpen() const;

		/**	Test if the file is closed.
				The standard constructor opens the file.
		*/
		bool isClosed() const;

		/**	
		*/
		static bool isAccessible(String name);

		/**	
		*/
		bool isAccessible() const;

		/**	
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

		/**	
		*/
		static bool isExecutable(String name);

		/**	
		*/
		bool isExecutable() const;

		/**	
		*/
		virtual bool hasFormat();

		/**	
		*/
		bool hasFormat() const;

		/**
		*/
		virtual bool hasFormat(const String& s) const;

		/**	@name	Debugging and Diagnostics */
		//@{

		/**	Test if the file ist valid.
				This function uses std::fstream::good().
				@return bool true if the file is valid
		*/
		bool isValid() const;

		/**
		*/
		static void dumpRegisteredActions(std::ostream& s)
		{
			action_manager_.dump(s);
		}
		//@}
		
		private:

		File& operator = (const File& file);

		public:

		class Action_
		{
			friend class ActionManager;
			public:

			Action_()
				:	regular_expression_(),
					exec_string_(),
					previous_action_(0),
					next_action_(0)
			{
			}

			Action_
				(const String& pattern,
				 const String& exec_string,
				 Action_* previous_action,
				 Action_* next_action)
				:	regular_expression_(pattern, true),
					exec_string_(exec_string),
					previous_action_(previous_action),
					next_action_(next_action)
			{
				if (previous_action != 0)
				{
					previous_action->next_action_ = this;
				}
		
				if (next_action != 0)
				{
					next_action->previous_action_ = this;
				}
			}

			~Action_()
			{
				if (previous_action_ != 0)
				{
					previous_action_->next_action_ = next_action_;
				}

				if (next_action_ != 0)
				{
					next_action_->previous_action_ = previous_action_;
				}
			}
			
			RegularExpression regular_expression_;
			String		exec_string_;
			Action_*	previous_action_;
			Action_*	next_action_;
		};

		friend class Action_;

		class ActionManager
		{
			public:
			friend class Action_;

			ActionManager()
				:	first_action_(0)
			{
			}

			~ActionManager()
			{
				unregisterActions();
			}
			
			// INVARIANT: always insert neew action at the head
			void registerAction(const String& pattern, const String& exec_string)
			{
				Action_* found_action = findAction(pattern);
		
				if (found_action == 0)
				{
					first_action_ = new Action_(pattern, exec_string, 0, first_action_);
				} else {
					found_action->regular_expression_.set(pattern, true);
			
					found_action->exec_string_ = exec_string;
				}
			}

			void unregisterAction(const String& pattern)
			{
				for (register Action_* action = first_action_;
						 action != 0; action = action->next_action_)
				{
					if (pattern == action->regular_expression_.getPattern())
					{
						if (action == first_action_)
						{
							first_action_ = first_action_->next_action_;
						}
						delete action;
						break;
					}
				}
			}

			void unregisterActions()
			{
				if (first_action_ == 0)
				{
					return;
				}
			
				for (register Action_ *next_action = 0;
						 first_action_ != 0; first_action_ = next_action)
				{
					next_action = first_action_->next_action_;
					delete first_action_;
				}
			}

			Action_* findAction(const String& pattern)
			{
				for (register Action_ *action = first_action_;
						 action != 0; action = action->next_action_)
				{
					if (pattern == action->regular_expression_.getPattern())
					{
						return action;
					}
				}

				return 0;
			}

			String* findExecString(const String& filename)
			{
				for (register Action_ *action = first_action_;
						 action != 0; action = action->next_action_)
				{
					if (action->regular_expression_.match(filename.c_str()) == true)
					{
						return &(action->exec_string_);
					}
				}

				return 0;
			}

			void dump(std::ostream& s) const
			{
				s << "Registered actions: " << std::endl;
		
				for (register Action_ *action = first_action_;
						 action != 0; action = action->next_action_)
				{
					s << "  regular expression: \"" << action->regular_expression_ << "\"" << std::endl
						<< "  exec: \"" << action->exec_string_ << "\"" << std::endl;
				}
			}	

			private:
			
			Action_* first_action_;
		};

		friend class ActionManager;
	
		String name_;
		String original_name_;
		OpenMode open_mode_;
		bool is_open_;
		bool is_temporary_;

		static ActionManager action_manager_;
		static unsigned char protocol_ability_;
	};

#	ifndef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/SYSTEM/file.iC>
#	endif

} // namespace BALL

#endif // BALL_SYSTEM_FILE_H
