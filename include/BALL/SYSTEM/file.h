// $Id: file.h,v 1.1 1999/08/26 07:53:20 oliver Exp $

#ifndef BALL_SYSTEM_FILE_H
#define BALL_SYSTEM_FILE_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#include <fstream.h>
#include <stdlib.h>   // 'getenv'
#include <sys/types.h>
#include <sys/stat.h> // 'stat', 'lstat'
#include <stdio.h>    // 'rename'
#include <unistd.h>   // 'access', 'rename', 'truncate'

#ifndef BALL_DATATYPE_REGEXP_H
#	include <BALL/DATATYPE/regExp.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
#	include <BALL/DATATYPE/string.h>
#endif

#ifndef BALL_SYSTEM_FILESYSTEM_H
#	include <BALL/SYSTEM/fileSystem.h>
#endif

namespace BALL {


	class File: public fstream
	{
		public:

		/**	@name	Enums
		*/
		//@{
		
		/**
		*/
		enum OpenMode
		{
			/**
			*/
			OPEN_MODE__INVALID = -1
		};
			
		enum ProtectionMode
		{
			PROTECTION_MODE__INVALID                   = -1,
			PROTECTION_MODE__UNKNOWN                   = 0,
			PROTECTION_MODE__MASK                      = S_IRWXU | S_IRWXG | S_IRWXO,

			PROTECTION_MODE__OWNER_READ_WRITE_EXECUTE  = S_IRWXU,
			PROTECTION_MODE__OWNER_READ                = S_IRUSR,
			PROTECTION_MODE__OWNER_WRITE               = S_IWUSR,
			PROTECTION_MODE__OWNER_EXECUTE             = S_IXUSR,

			PROTECTION_MODE__GROUP_READ_WRITE_EXECUTE  = S_IRWXG,
			PROTECTION_MODE__GROUP_READ                = S_IRGRP,
			PROTECTION_MODE__GROUP_WRITE               = S_IWGRP,
			PROTECTION_MODE__GROUP_EXECUTE             = S_IXGRP,

			PROTECTION_MODE__OTHER_READ_WRITE_EXECUTE  = S_IRWXO,
			PROTECTION_MODE__OTHER_READ                = S_IROTH,
			PROTECTION_MODE__OTHER_WRITE               = S_IWOTH,
			PROTECTION_MODE__OTHER_EXECUTE             = S_IXOTH
		};

		enum Protocol
		{
			PROTOCOL__FILE   = 1,
			PROTOCOL__EXEC   = 2,
			PROTOCOL__ACTION = 3
		};
		
		enum Type
		{
			TYPE__UNKNOWN            = 0,
			TYPE__DIRECTORY          = 1,
			TYPE__CHAR_SPECIAL_FILE  = 2,
			TYPE__BLOCK_SPECIAL_FILE = 3,
			TYPE__REGULAR_FILE       = 4,
			TYPE__SYMBOLIC_LINK      = 5,
			TYPE__SOCKET             = 6,
			TYPE__FIFO_SPECIAL_FILE  = 7
		};
		//@}

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**
		*/
		File(void);

		/**
		*/
		File(const String& name, int open_mode = ios::in);

		/**
		*/
		virtual ~File(void);

		//@}
			
			
		static void enableProtocol(Protocol protocol);

		static void disableProtocol(Protocol protocol);

		bool open(const String& name, int open_mode = ios::in);

		bool reopen(void);

		void close(void);

		const String& getName(void) const;

		const String& getOriginalName(void) const;

		Size getSize(void) const;

		int getOpenMode(void) const;
		
		static Type getType(String name, bool trace_link);

		Type getType(bool trace_link) const;

		fstream& getFileStream(void);
		
		static bool copy
			(String source_name, String destination_name,
			 Size buffer_size = 4096);

		bool copyTo(const String& destination_name, Size buffer_size = 4096);

		static bool move(const String& source_name, const String& destination_name);

		bool moveTo(const String& destination_name);

		static bool remove(String name);

		bool remove(void) const;

		static bool rename(String old_path, String new_path);

		bool renameTo(const String& new_path) const;

		static bool truncate(String path, Size size = 0);

		bool truncate(Size size = 0) const;

		static void registerAction(const String& pattern, const String& exec);
			
		static void unregisterAction(const String& pattern);
			
		static bool createTemporaryFilename(String& temporary);

		/**	@name Predicates 
		*/
		//@{

		/**	Equality comparison operator.
		*/
		bool operator == (const File &file) const;
		
		/**	Inequality comparison operator.
		*/
		bool operator != (const File &file) const;
		//@}

		static bool isProtocolEnabled(Protocol protocol);

		bool isOpen(void) const;

		bool isClosed(void) const;

		static bool isAccessible(String name);

		bool isAccessible(void) const;

		bool isCanonized(void) const;
	
		static bool isReadable(String name);

		bool isReadable(void) const;

		static bool isWritable(String name);

		bool isWritable(void) const;

		static bool isExecutable(String name);

		bool isExecutable(void) const;

		virtual bool hasFormat(void);

		bool hasFormat(void) const;

		/**
		*/
		virtual bool hasFormat(const String& s) const;

		/**	@name	Debugging and Diagnostics */
		//@{

		/**
		*/
		bool isValid(void) const;
	
		/**
		*/
		static void dumpRegisteredActions(ostream &s)
		{
			action_manager_.dump(s);
		}
		//@}
		
		protected:

		private:

		File(const File& file);

		File& operator = (const File &file);

		class Action_
		{
			public:

			Action_(void)
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

			~Action_(void)
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

			ActionManager(void)
				:	first_action_(0)
			{
			}

			~ActionManager(void)
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

			void unregisterAction(const String &pattern)
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

			void unregisterActions(void)
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

			String* findExecString(const String &filename)
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

			void dump(ostream &s) const
			{
				s << "Registered actions: " << endl;
		
				for (register Action_ *action = first_action_;
						 action != 0; action = action->next_action_)
				{
					s << "  regular expression: \"" << action->regular_expression_ << "\"" << endl
						<< "  exec: \"" << action->exec_string_ << "\"" << endl;
				}
			}	

			protected:

			private:
			
			Action_* first_action_;
		};

		friend class ActionManager;
	
		String name_;
		String original_name_;
		int open_mode_;
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
