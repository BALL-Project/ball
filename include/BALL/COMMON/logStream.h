// $Id: logStream.h,v 1.12 2000/10/18 12:39:32 oliver Exp $

#ifndef BALL_COMMON_LOGSTREAM_H
#define BALL_COMMON_LOGSTREAM_H

#ifndef BALL_CONFIG_CONFIG_H
#	include <BALL/CONFIG/config.h>
#endif

#ifndef BALL_COMMON_GLOBAL_H
#	include <BALL/COMMON/global.h>
#endif

#ifndef BALL_COMMON_LIMITS_H
#	include <BALL/COMMON/limits.h>
#endif

#ifndef BALL_COMMON_DEBUG_H
#	include <BALL/COMMON/debug.h>
#endif

#include <sys/time.h>

#include <iostream>
#include <list>
#include <vector>
#include <string>


using std::list;
using std::vector;
using std::string;

namespace BALL 
{

	/**	@name Log streams
			Logging, filtering, and storing messages.
			Many programs emit warning messages, error messages, or simply
			informations and remarks to their users. The \Ref{LogStream}
			class provides a convenient and straight-forward interface 
			to classify these messages according to their importance 
			(via the loglevel), filter and store them in files or
			write them to streams.\\
			As the LogStream class is derived from ostream, it behaves 
			as any ostream object. Additionally you may associate
			streams with each LogStream object that catch only 
			messages of certain loglevels. So the user might decide to
			redirect all error messages to cerr, all warning messages
			to cout and all informations to a file.\\
			Along with each message its time of creation and its loglevel
			is stored. So the user might also decide to store all 
			errors he got in the last two hours or alike.\\
			The \Ref{LogStream} class heavily relies on the \Ref{LogStreamBuf}
			class, which does the actual buffering and storing, but is only
			of interest if you want to implement a derived class, as the 
			actual user interface is implemented in the LogStream class.
	*/
	//@{

	// forward declarations
	class LogStream;

	template <typename T>
	class NotificationTarget;

	
	class LogStreamNotifier
	{
		public:
		
		LogStreamNotifier();
			
		LogStreamNotifier(const NotificationTarget<LogStreamNotifier>& target);
		
		~LogStreamNotifier();

		void notify() const;
	};


	/** Stream buffer used by LogStream.
			This class implements the low level behaviour of
			\Ref{LogStream}. It takes care of the buffers and stores
			the lines written into the \Ref{LogStream} object.
			It also contains a list of streams that are associated with
			the LogStream object. This list contains pointers to the
			streams and their minimum and maximum log level.
			Each line entered in the \Ref{LogStream} is marked with its
			time (in fact, the time \Ref{sync} was called) and its
			loglevel. The loglevel is determined by either the current
			loglevel (as set by \Ref{LogStream::setLevel} or a temporary
			level (as set by \Ref{LogStream::level} for a single line only).
			For each line stored, the list of associated streams is checked
			whether the loglevel falls into the range declared by the 
			stream's minimum and maximum level. If this condition is met,
			the logline (with its prefix, see \Ref{LogStream::setPrefix})
			is also copied to the associated stream and this stream is 
			flushed, too.
	*/
	class LogStreamBuf
		: public std::streambuf
	{

		friend class LogStream;

		public:

			/**	@name Constructors and Destructors
			*/
			//@{
			
			/** Default constructor.
					Create a new LogStreamBuf object.
			*/
			LogStreamBuf();

			/** Destructor.
					Destruct the buffer and free all stored messages strings.
			*/
			virtual ~LogStreamBuf();
			
			//@}
			

			/**	@name	Debugging and Diagnostics
			*/
			//@{
			
			/** Dump method.
					Dumps the contents of the whole message buffer 
					including time and log level.
			*/
			virtual void dump(std::ostream& s);

			//@}

			/**	@name	Stream methods 
			*/
			//@{

			/**	Sync method.
					This method is called as soon as the ostream is flushed
					(especially this method is called by flush or endl).
					It transfers the contents of the streambufs putbuffer 
					into a logline if a newline or linefeed character 
					is found in the buffer ("$\backslash$n" or "$\backslash$r" resp.).
					The line is then removed from the putbuffer.
					Incomplete lines (not terminated by $\backslash$n/$\backslash$r are
					stored in incomplete\_line\_.
			*/
			virtual int sync();

			/**	Overflow method.
					This method calls sync and {\tt streambuf::overflow(c)} to 
					prevent a buffer overflow.
			*/
			virtual int overflow(int c = -1);
			//@}

			struct Stream 
			{
				std::ostream*				stream;
				string							prefix;
				int									min_level;
				int									max_level;
				LogStreamNotifier*	target;
			
				Stream()
					:	stream(0),
						min_level(Limits<int>::min()),
						max_level(Limits<int>::max()),
						target(0)
				{
				}
			};

			typedef struct Stream StreamStruct;
		

		protected:

			struct LoglineStruct 
			{	
				int     level;
				string  text;
				Time  time;

				LoglineStruct()
					: level(0),
						text(""),
						time(0)
				{}
			};

			typedef struct LoglineStruct Logline;


			// interpret the prefix format string and return the expanded prefix
			string expandPrefix_(const string& prefix, const int& level, const Time& time) const;
	
			char* 									pbuf_;

			vector<Logline> 				loglines_;
		
			int											level_;

			int											tmp_level_;
			
			list<StreamStruct>			stream_list_;

			string									incomplete_line_;
	};


	/**	Log Stream Class.
			\\
			{\bf Definition:} \URL{BALL/COMMON/logStream.h}
			\\
	*/
	class LogStream
		: public std::ostream
	{
		public:

		/**	@name	Type definitions
		*/
		//@{
			
		/**
		*/
		typedef NotificationTarget<LogStreamNotifier>	Target;
		//@}

		/**	@name Enums
		*/
		//@{
			
		/** Log levels.
				Constants for the different predefined log levels.
				Use \Ref{ERROR} to indicate a severe error, \Ref{WARNING} to 
				indicate a problem that could be fixed or is of minor importance, 
				and \Ref{INFORMATION} for messages that do not indicate any problem 
				(e.g. progress messages).
		*/
		enum 
		{
			/** Loglevels >= ERROR should be used to indicate errors
			*/
			ERROR = 2000,
			/** Loglevels >= WARNING should be used to indicate warnings
			*/
			WARNING = 1000,
			/** Loglevels >= INFORMATION indicate information messages
			*/
			INFORMATION = 0
		};

		//@}
	
		/**	@name	Constructors and Destructors
		*/
		//@{

		/** Default constructor.
				Creates a new LogStream object that is not associated with any stream.
				If the argument {\tt associate\_stdio} is set to {\bf true},
				{\tt cout} is associated with all messages of levels \Ref{INFORMATION} 
				and \Ref{WARNING}, and {\tt cerr} is associated with all messages
				of level \Ref{ERROR}.
				@param	associate_stdio bool, default is false
		*/
		LogStream(const bool& associate_stdio = false);

		/** Constructor.
				Create a new LogStream object with a given LogStreamBuf
		*/
		LogStream(LogStreamBuf* buf);

		/** Destructor.
				Clears all message buffers.
		*/
		virtual ~LogStream();
	
		//@}		

		/**	@name	Stream Methods
		*/
		//@{

		/**	{\tt rdbuf} method of ostream.
				This method is needed to access the LogStreamBuf object.
		*/
		LogStreamBuf* rdbuf();

		/** Arrow operator.
		*/
		LogStreamBuf* operator -> ();
		//@}

		/**	@name Loglevel management 
		*/
		//@{

		/**	Assign a new log level.
				This method assigns a new loglevel which will be used
				for all messages sent to the LogStream after that call
				(except for messages which use the temporary loglevel
				set by \Ref{level}).
		*/
		void setLevel(const int& level);

		/**	Return the current log level.
				The LogStreamBuf object has an internal current log level ({\tt level\_}).
				It is set to 0 by the LogStreamBuf default constructor.
				This method returns {\tt rdbuf()->level\_} if rdbuf() does not
				return a null pointer, 0 otherwise.
				@return		int the current log level
		*/
		int getLevel();

		/**	Set a temporary log level.
				Using {\bf level}, a temporary loglevel may be defined.
				It is valid unly until the next {\bf flush} or {\bf endl} is issued.\\
				Use this command to log a single line with a certain log level.\\
				{\bf Example:}
\begin{verbatim}
	log << "log message 1" << endl;
	log.level(4) << "log message 2" << endl;
	log << "log message 3" << endl;
\end{verbatim}
				In this example, only the second message will be logged using level 4.
				
				@return	LogStream the log stream
				@param	level the temporary log level
		*/
		LogStream& level(const int& n);

		/**	Log an information message.
				This is method equivalent to \Ref{level}(LogStream::INFORMATION + n). 
				@param	n the channel 
		*/
		LogStream& info(const int& n = 0);

		/**	Log an error message.
				This is method equivalent to \Ref{level}(LogStream::ERROR + n). 
				@param	n the channel 
		*/
		LogStream& error(const int& n = 0);

		/**	Log an information message.
				This is method equivalent to \Ref{level}(LogStream::WARNING + n). 
				@param	n the channel 
		*/
		LogStream& warn(const int& n = 0);

		//@}

		/**	@name Associating Streams 
		*/
		//@{

		/**	Associate a new stream with this logstream.
				This method inserts a new stream into the list of 
				associated streams and sets the corresponding minimum
				and maximum log levels.
				Any message that is subsequently logged, will be copied
				to this stream if its log level is between {\tt min\_level}
				and {\tt max\_level}. If {\tt min\_level} and {\tt max\_level}
				are omitted, all messages are copied to this stream.
				If {\tt min\_level}	and {\tt max\_level} are equal, this function can be used
				to listen to a specified channel.
				@param	s a reference to the stream to be associated
				@param	min_level the minimum level of messages copied to this stream
				@param	max_level the maximum level of messages copied to this stream
		*/
		void insert(std::ostream& s, const int& min_level = Limits<int>::min(), const int& max_level = Limits<int>::max());

		/**	Remove an association with a stream.
				Remove a stream from the stream list and avoid the copying of new messages to
				this stream.\\
				If the stream was not in the list of associated streams nothing will
				happen.
				@param	s the stream to be removed
		*/
		void remove(std::ostream& s);

		/**	Add a notification target for the stream.	
		*/
		void insertNotification(const std::ostream& s, const Target& target);

		/**	Remove a notification target for the stream.
		*/
		void removeNotification(const std::ostream& s);

		/**	Set the minimum log level of an associated stream.
				This method changes the minimum log level of an already
				associated stream. However, if the stream is not
				associated, nothing will happen.
				@param	s the associated stream
				@param	min_level the new minimum level
		*/
		void setMinLevel(const std::ostream& s, const int& min_level);
		
		/**	Set the maximum log level of an associated stream.
				This method changes the maximum log level of an already
				associated stream. However, if the stream is not
				associated, nothing will happen.
				@param	s the associated stream
				@param	min_level the new minimum level
		*/
		void setMaxLevel(const std::ostream& s, const int& max_level);

		/**	Set prefix for output to this stream.
				Each line written to the stream will be prefixed by
				this string. The string may also contain trivial 
				format specifiers to include loglevel and time/date 
				of the logged message.\\
				The following format tags are recognized:
				\begin{itemize}
					\item {\bf %l}	loglevel
					\item {\bf %y}	message type ("Error", "Warning", "Information", "-")
					\item {\bf %T}  time (HH:MM:SS)
					\item {\bf %t}  time in short format (HH:MM)
					\item {\bf %D}	date (DD.MM.YYYY)
					\item {\bf %d}  date in short format (DD.MM.)
					\item {\bf %S}  time and date (DD.MM.YYYY, HH:MM:SS)
					\item {\bf %s}  time and date in short format (DD.MM., HH:MM)
					\item {\bf %%}	percent sign (escape sequence)
				\end{itemize}
		*/
		void setPrefix(const std::ostream& s, const string& prefix);
		//@}		
		
		/**	@name	Message Buffer Management */
		//@{
			
		/** Clear the message buffer.
				This method removes all stored messages from the 
				messages buffer.
		*/
		void clear();
	
		/**	Return the number of lines.
				This method retruns the number of lines in the buffer 
				for a given range of levels.\\
				If the range is omitted, the total number of messages is
				returned.
				@return Size the number of lines matching the log level range
				@param	min_level the minimum log level of the counted messages
				@param	max_level the maximum log level of the counted messages
		*/
		Size	getNumberOfLines(const int& min_level = Limits<int>::min(), const int& max_level = Limits<int>::max()) const;

		/**	Return the text of a specific line.
				This method returns the content of a specific message without
				time and level.
				@return string the mesasge text
				@param	Index the index of the line
		*/
		string getLineText(const Index& index) const;

		/**	Return the log time of a specific line
				@param index the index of the messages
				@return Time the time of the message
		*/
		Time getLineTime(const Index& index) const;	
	
		/**	Return the log level of a specific line.
				If the given line does not exists, {\em -1} is returned.
				@param index the index of the messages
				@return int - the level
		*/
		int getLineLevel(const Index& index) const;
		
		/** Retrieve a list of indices of lines that match certain criteria.
				If a criterion is left empty, it is not used.
				@param min_level the minimum level of messages
				@param max_level the maximum level of messages
				@param earliest (long) the time of messages to start filtering
				@param latest (long) the time of messages to stop filtering
				@param s a string to look for
		*/
		list<int>	filterLines(const int& min_level = Limits<int>::min(), const int& max_level = Limits<int>::max(),
														 const Time& earliest = 0, const Time& latest = Limits<Time>::max(), const string& s = "") const;
		//@}

		private:

		// flag needed by the destructor to decide whether the streambuf
		// has to be deleted. If the default ctor is used to create
		// the LogStreamBuf, delete_buffer_ is set to true and the ctor
		// also deletes the buffer.
		bool	delete_buffer_;
	};


	/** Global static instance of a logstream.
			This instance of LogStream is by default bound to {\bf cout} {\bf cerr} by calling
			the default constructor.
	*/
	extern LogStream	Log;

	//@}
	
#	ifndef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/COMMON/logStream.iC>
#	endif

} // namespace BALL

#endif // BALL_COMMON_LOGSTREAM_H
