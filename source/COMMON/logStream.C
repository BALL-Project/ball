// $Id: logStream.C,v 1.4 1999/09/17 16:39:09 oliver Exp $

#include <BALL/COMMON/logStream.h>

#include <sys/time.h>
#include <stdio.h>

namespace BALL 
{

	LogStreamBuf::LogStreamBuf() 
		: streambuf(),
			level_(0),
			tmp_level_(0),
			incomplete_line_("")
	{
		pbuf_ = new char [1024];
		streambuf::setp(pbuf_, pbuf_ + 1023);
	}
		
	LogStreamBuf::~LogStreamBuf() 
	{
		sync();

		delete [] pbuf_;
	}

	void LogStreamBuf::dump(ostream& stream) 
	{
		char buf[1024];
		Size line;
		for (line = loglines_.size(); line > 0; --line) 
		{
			strftime(&(buf[0]), 1023, "%d.%m.%Y %T ", localtime(&(loglines_[line - 1].time)));
			stream << buf << "[" << loglines_[line - 1].level
						 << "]:" << loglines_[line - 1].text << endl;
		}
	}
 
	int LogStreamBuf::sync() 
	{
		static char buf[1024];

		// sync our streambuffer...

		if (pptr() != pbase()) 
		{
				
			char*	line_start = pbase();
			char*	line_end = pbase();
			
			while (line_end <= pptr())
			{
				// search for the first end of line
				for (; line_end < pptr() && *line_end != '\n'; line_end++);

				if (line_end >= pptr()) 
				{
					// copy the incomplete line to the incomplete_line_ buffer
					strncpy(&(buf[0]), line_start, line_end - line_start + 1);
					buf[line_end - line_start] = '\0';
					incomplete_line_ += &(buf[0]);

					// mark everything as read
					line_end = pptr() + 1;
				} else {
					memcpy(&(buf[0]), line_start, line_end - line_start + 1);
					buf[line_end - line_start] = '\0';
						
					// assemble the string to be written
					// (consider leftovers of the last buffer from incomplete_line_)
					string outstring = incomplete_line_;
					incomplete_line_ = "";
					outstring += &(buf[0]);

					// if there are any streams in our list, we
					// copy the line into that streams, too and flush them
					using ::std::list;
					list<StreamStruct>::iterator list_it = stream_list_.begin();
					for (; list_it != stream_list_.end(); ++list_it)
					{
						// if the stream is open for that level, write to it...
						if ((list_it->min_level <= tmp_level_) && (list_it->max_level >= tmp_level_))
						{
							*(list_it->stream) << expandPrefix_(list_it->prefix, tmp_level_, time(0))
																 << outstring << endl;
						}
					}
			
					// update the line pointers (increment both)
					line_start = ++line_end;
					

					// remove cr/lf from the end of the line
					
					while (outstring[outstring.size() - 1] == 10 || outstring[outstring.size() - 1] == 13)
					{
						outstring.erase(outstring.end());
					}
		
					// store the line 
					Logline	logline;

					logline.text = outstring;
					logline.level = tmp_level_;
					logline.time = time(0);
			
					// store the new line
					loglines_.push_back(logline);

					// reset tmp_level_ to the previous level
					// (needed for LogStream::level() only)
					tmp_level_ = level_;
				}
			}

			// remove all processed lines from the buffer
			pbump((int)(pbase() - pptr()));
		}

	
		return 0;
	}

	string LogStreamBuf::expandPrefix_(const string& prefix, int level, time_t time) const
	{
		Size		index = 0;
		Size		copied_index = 0;
		string	result("");

		while ((index = prefix.find("%", index)) != string::npos) 
		{
			// append any constant parts of the string to the result
			if (copied_index < index) 
			{
				result.append(prefix.substr(copied_index, index - copied_index));
				copied_index = index;
			}
			
			if (index < prefix.size()) 
			{
				char	buffer[64];
				char*	buf = &(buffer[0]);

				switch (prefix[index + 1]) 
				{
	
					case '%': // append a '%' (escape sequence)
						result.append("%");
						break;

					case 'l': // append the loglevel
						sprintf(buf, "%d", level);
						result.append(buf);
						break;

					case 'y':	// append the message type (error/warning/information)
						if (level >= LogStream::ERROR) 
						{
							result.append("ERROR");
						} else if (level >= LogStream::WARNING) {
							result.append("WARNING");
						} else if (level >= LogStream::INFORMATION) {
							result.append("INFORMATION");
						} else {
							result.append("LOG");						}
						break;

					case 'T':	// time: HH:MM:SS
						strftime(buf, 1023, "%R", localtime(&time));
						result.append(buf);
						break;

					case 't': // time: HH:MM	
						strftime(buf, 1023, "%T", localtime(&time));
						result.append(buf);
						break;

					case 'D':	// date: DD.MM.YYYY
						strftime(buf, 1023, "%d.%m.%Y", localtime(&time));
						result.append(buf);
						break;

					case 'd':	// date: DD.MM.
						strftime(buf, 1023, "%d.%m.", localtime(&time));
						result.append(buf);
						break;

					case 'S':	// time+date: DD.MM.YYYY, HH:MM:SS
						strftime(buf, 1023, "%d.%m.%Y, %R", localtime(&time));
						result.append(buf);
						break;

					case 's':	// time+date: DD.MM., HH:MM
						strftime(buf, 1023, "%d.%m., %T", localtime(&time));
						result.append(buf);
						break;

					default:
						break;
				}
				index += 2;
				copied_index += 2;
			}
		}

		if (copied_index < prefix.size()) {
			result.append(prefix.substr(copied_index, prefix.size() - copied_index));
		}

		return result;
	}

	// keep the given buffer	
	LogStream::LogStream(LogStreamBuf* buf)		
		: ostream(buf),
			delete_buffer_(false)
	{
	}

	// create a new buffer
	LogStream::LogStream(bool associate_stdio)
		: ostream(new LogStreamBuf),
			delete_buffer_(true)
	{
		if (associate_stdio == true) 
		{
			// associate cout to informations and warnings,
			// cerr to errors by default
			insert(cout, INFORMATION, ERROR - 1);
			insert(cerr, ERROR);
		}
	}

	// remove the streambuffer
	LogStream::~LogStream(void)
	{
		if (delete_buffer_)
			delete rdbuf();
	}
		
	void LogStream::insert(ostream& stream, int min_level, int max_level) 
	{
		// return if no LogStreamBuf is defined!
		if (rdbuf() == 0)
		{
			return;
		}
			
		// first, check whether the stream is already associated (avoid
		// multiple insertions)
																																	
		using std::list;
		list<LogStreamBuf::StreamStruct>::iterator list_it = rdbuf()->stream_list_.begin();
		for (; list_it != rdbuf()->stream_list_.end(); ++list_it) 
		{
			if ((*list_it).stream == &stream) 
			{
				return;
			}
		}

		// we didn`t find it - create a new entry in the list
		LogStreamBuf::StreamStruct s_struct;
		s_struct.min_level = min_level;
		s_struct.max_level = max_level;
		s_struct.stream = &stream;
		rdbuf()->stream_list_.push_back(s_struct);
	}

	void LogStream::remove(ostream& stream) 
	{
		// return if no LogStreamBuf is defined!
		if (rdbuf() == 0)
		{
			return;
		}
			
		// find the stream in the LogStreamBuf's list
		
    using std::list;																																
		list<LogStreamBuf::StreamStruct>::iterator	list_it = rdbuf()->stream_list_.begin();
		for (; list_it != rdbuf()->stream_list_.end(); ++list_it)
		{
			if ((*list_it).stream == &stream) 
			{
				// remove the stream - iterator becomes invalid, so exit
				rdbuf()->stream_list_.erase(list_it);
				break;
			}
		}
		
		// if the stream is not found nothing happens!		
	}

	
	void LogStream::setMinLevel(const ostream& stream, int level) 
	{
		// return if no LogStreamBuf is defined!
		if (rdbuf() == 0)
		{
			return;
		}
			
		// find the stream in the LogStreamBuf's list
		
    using std::list;																																
		list<LogStreamBuf::StreamStruct>::iterator	list_it = rdbuf()->stream_list_.begin();
		for (; list_it != rdbuf()->stream_list_.end(); ++list_it)
		{
			if ((*list_it).stream == &stream) 
			{
				// change the streams min_level and exit the loop
				(*list_it).min_level = level;
				break;
			}	
		}
	}

	void LogStream::setMaxLevel(const ostream& stream, int level) 
	{
		// return if no LogStreamBuf is defined!
		if (rdbuf() == 0)
			return;
			
		// find the stream in the LogStreamBuf's list:
		// iterate over the list until you find the stream`s pointer

    using std::list;																																
		list<LogStreamBuf::StreamStruct>::iterator	list_it = rdbuf()->stream_list_.begin();

		for (; list_it != rdbuf()->stream_list_.end(); ++list_it)
		{
			if ((*list_it).stream == &stream) 
			{
				// change the streams max_level and exit the loop
				(*list_it).max_level = level;
				break;
			}
		}
	}
	
	void LogStream::setPrefix(const ostream& s, const string& prefix) 
	{
		// return if no LogStreamBuf is defined!
		if (rdbuf() == 0)
		{
			return;
		}
			
		// find the stream in the LogStreamBuf's list:
		// iterate over the list until you find the stream`s pointer

    using std::list;																																
		list<LogStreamBuf::StreamStruct>::iterator	list_it = rdbuf()->stream_list_.begin();

		for (; list_it != rdbuf()->stream_list_.end(); ++list_it)
		{
			if ((*list_it).stream == &s) 
			{
				// change the streams max_level and exit the loop
				(*list_it).prefix = prefix;
				return;
			} 
		}
	}
	
	Size LogStream::getNumberOfLines(int min_level, int max_level) const  {
		
		// cast this to const, to access non const method rdbuf() which
		// is usually const, but declared non const

		LogStream*	non_const_this = const_cast<LogStream*>(this);


		// iterate over all loglines and count the lines of interest

		vector<LogStreamBuf::Logline>::iterator	it = non_const_this->rdbuf()->loglines_.begin();
		Size	count = 0;

		for (; it != non_const_this->rdbuf()->loglines_.end(); ++it) 
		{
			if ((*it).level >= min_level && (*it).level <= max_level)
			{
				count++;
			}
		}

		return count;
	}

	// global default logstream
	LogStream	Log(true);

#	ifdef	BALL_NO_INLINE_FUNCTIONS
#		include <BALL/COMMON/logStream.iC>
#	endif

} // namespace BALL
