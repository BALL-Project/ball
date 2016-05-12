// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/SYSTEM/file.h>
#include <BALL/SYSTEM/simpleDownloader.h>

#include <BALL/DATATYPE/regularExpression.h>

#include <BALL/DATATYPE/regularExpression.h>

#include <cstdlib>     // 'getenv'
#include <sys/types.h>
#include <sys/stat.h>   // 'stat', 'lstat'
#include <cstdio>      // 'rename'

// maximum number of substitutions performed in TransformationManager::transform()
// (in order to avoid infinite recursion)
#define MAX_SUBSTITUTIONS 10

namespace BALL 
{
	using namespace Exception;
 
	const File::OpenMode File::MODE_IN = std::ios::in;
	const File::OpenMode File::MODE_OUT = std::ios::out;
	const File::OpenMode File::MODE_APP = std::ios::app;
	const File::OpenMode File::MODE_BINARY = std::ios::binary;
	const File::OpenMode File::MODE_ATE = std::ios::ate;
	const File::OpenMode File::MODE_TRUNC = std::ios::trunc;
	HashSet<String> File::created_temp_filenames_ = HashSet<String>();
 
	File::CannotWrite::CannotWrite(const char* file, int line, const String& filename)
		:	Exception::GeneralException(file, line, "File::CannotWrite", ""),
			filename_(filename)
	{
		message_ = "the file " + filename + " could not be written(i.g. not open or wrong open mode)";
		globalHandler.setMessage(message_);
	}

	File::CannotWrite::~CannotWrite()
		throw()
	{
	}

	String File::CannotWrite::getFilename() const
	{
		return filename_;
	}

	TransformationManager::TransformationManager()
	{
	}
	
	TransformationManager::~TransformationManager()
	{
	}

	void TransformationManager::registerTransformation(const String& pattern, const String& command)
	{
		if (!pattern.empty())
		{
			transformation_methods_.insert(std::pair<String, String>(pattern, command));	
		}
	}
	
	void TransformationManager::unregisterTransformation(const String& pattern)
	{
		if (transformation_methods_.find(pattern) != transformation_methods_.end())
		{
			transformation_methods_.erase(pattern);
		}
	}

	String TransformationManager::findTransformation(const String& name) const
	{		
		std::map<String, String>::const_iterator it(transformation_methods_.begin());

		for (; it != transformation_methods_.end(); ++it)
		{
			if (RegularExpression(it->first).match(name))
			{
				return it->second;
			}
		}

		return "";
	}

	String TransformationManager::transform(const String& name)
	{
		String result = findTransformation(name);
		if (!result.empty())
		{
			// substitute "%s" with the full file name
			Size count = 0;
			while ((result.substitute("%s", name) != String::EndPos) && (++count <= MAX_SUBSTITUTIONS)) { }

			// substitute "%f" with the full name without the last file suffix (after and including the last ".")
			// and "%f[suffix]" with the full filename without [suffix]
			if (result.hasSubstring("%f["))
			{
				String full_name = name;
				RegularExpression suffix_regexp("%f\\[[^]]*\\]");
				Substring suffix_substring;
				count = 0;
				while (suffix_regexp.find(result, suffix_substring) && (++count <= MAX_SUBSTITUTIONS))
				{
					String suffix = suffix_substring;
					suffix = suffix(3, (Size)suffix.size() - 4);
					if (!suffix.empty() && full_name.hasSuffix(suffix))
					{
						full_name = full_name(0, (Size)(full_name.size() - suffix.size()));
					}
					suffix_substring = full_name;
				}
			}
			if (result.hasSubstring("%f"))
			{
				String full_name = name;
				if (full_name.has('.'))
				{
					String suffix = name(name.find_last_of('.'));
					if (!suffix.empty() && full_name.hasSuffix(suffix))
					{
						full_name = full_name(0, (Size)(full_name.size() - suffix.size()));
					}
				}
				count = 0;
				while ((result.substitute("%f", full_name) != String::EndPos) && (++count <= MAX_SUBSTITUTIONS)) { }
			}
			// substitute "%f" with the full name without the last file suffix (after and including the last ".")
			// and "%f[suffix]" with the full filename without [suffix]
			if (result.hasSubstring("%b["))
			{
				RegularExpression suffix_regexp("%b\\[[^]]*\\]");
				Substring suffix_substring;
				count = 0;
				while (suffix_regexp.find(result, suffix_substring) && (++count <= MAX_SUBSTITUTIONS))
				{
					String suffix = suffix_substring;
					String base_name = FileSystem::baseName(name);
					suffix = suffix(3, suffix.size() - 4);
					if (!suffix.empty() && base_name.hasSuffix(suffix))
					{
						base_name = base_name(0, base_name.size() - suffix.size());
					}
					suffix_substring = base_name;
				}
			}
			if (result.hasSubstring("%b"))
			{
				String base_name = FileSystem::baseName(name);
				if (base_name.has('.'))
				{
					String suffix = name(name.find_last_of("."));
					if (!suffix.empty() && base_name.hasSuffix(suffix))
					{
						base_name = base_name(0, base_name.size() - suffix.size());
					}
				}
				count = 0;
				while ((result.substitute("%b", base_name) != String::EndPos) && (++count <= MAX_SUBSTITUTIONS)) { }
			}

			// "%p" with the path to the file
			count = 0;
			while ((result.substitute("%p", FileSystem::path(name)) != String::EndPos) && (++count <= MAX_SUBSTITUTIONS)) { }

			// "%t" with a temporary file name
			if (result.hasSubstring("%t"))
			{
				String tmp_file;
				File::createTemporaryFilename(tmp_file);
				count = 0;
				while ((result.substitute("%t", tmp_file) != String::EndPos) && (++count <= MAX_SUBSTITUTIONS)) { }
			}
		}

		return result;
	}
	
	TransformationManager File::transformation_manager_;

	const String File::TRANSFORMATION_EXEC_PREFIX = "exec:";
	const String File::TRANSFORMATION_FILE_PREFIX = "file:";
	const String File::TRANSFORMATION_HTTP_PREFIX = "http://";
	const String File::TRANSFORMATION_FTP_PREFIX  = "ftp://";

	Size File::transformation_methods_ = BALL_BIT(File::TRANSFORMATION__EXEC) 
																				| BALL_BIT(File::TRANSFORMATION__FILTER) 
																				| BALL_BIT(File::TRANSFORMATION__URL);

	File::File()
		:	std::fstream(),
			name_(),
			open_mode_(std::ios::in),
			is_open_(false),
			is_temporary_(false)
	{
	}

	File::File(const String& name, File::OpenMode open_mode)
		:	std::fstream(),
			name_(),
			open_mode_(open_mode),
			is_open_(false),
			is_temporary_(false)
	{
		if (name == "")
		{
			throw Exception::FileNotFound(__FILE__, __LINE__, "<empty name>");
		}
		open(name, open_mode);

		if (!isOpen()) throw(Exception::FileNotFound(__FILE__, __LINE__, name));
	}

	File::~File()
	{
		close();
	}

	bool File::open(const String& name, File::OpenMode open_mode)
	{
		close();	
		original_name_ = name_ = name;
		is_temporary_ = false;

		// we are reading files
		if (open_mode & MODE_IN)
		{
			// check
			String transformation_command = transformation_manager_.findTransformation(name_);
			if (transformation_command != "")
			{
				if (BALL_BIT_IS_CLEARED(transformation_methods_, File::TRANSFORMATION__FILTER))
				{		
					setstate(std::ios_base::failbit);
					throw Exception::FileNotFound(__FILE__, __LINE__, name_ + " (using " + transformation_command + ")");
				}
				name_ = transformation_manager_.transform(name);
			}

			// check for the FTP and HTTP transformation prefix
			if (name_.hasPrefix(TRANSFORMATION_FTP_PREFIX) ||
			    name_.hasPrefix(TRANSFORMATION_HTTP_PREFIX))
			{
				SimpleDownloader dl(name_);

				String tmp_file;
				createTemporaryFilename(tmp_file);

				int err;
				if((err = dl.downloadToFile(tmp_file)) != 0) {
					setstate(std::ios_base::failbit);
					throw Exception::FileNotFound(__FILE__, __LINE__,
									name_ + String(" from network."));
				}

				name_ = tmp_file;
				is_temporary_ = true;
			}

			// check for the EXEC transformation prefix
			if (name_.hasPrefix(TRANSFORMATION_EXEC_PREFIX))
			{
				// is the EXEC transformation enabled?
				if (BALL_BIT_IS_CLEARED(transformation_methods_, File::TRANSFORMATION__EXEC))
				{
					setstate(std::ios_base::failbit);
					throw Exception::FileNotFound(__FILE__, __LINE__, name);
				}
				
				// remove the prefix from the filename
				name_.erase(0, File::TRANSFORMATION_EXEC_PREFIX.size());
					
				// create a temporary file and redirect the command's output to that file
				String tmp_file;
				createTemporaryFilename(tmp_file);
				String command = String("exec ") + name_ + " >" + tmp_file;
				
				// execute the command
				int system_result = system(command.c_str());

				if (system_result < 0)
				{
					Log.info() << "Warning: system returned " << system_result << std::endl;
				}
		
				// remember the current file is a temporary file!
				name_ = tmp_file;
				is_temporary_ = true;
			}
			else 
			{
				// any file may be prefixed with "file:" -- for convenience and to make
				// it look like an URL...
				if (name_.hasPrefix(File::TRANSFORMATION_FILE_PREFIX))
				{
					name_.erase(0, TRANSFORMATION_FILE_PREFIX.size());
				}

				// check whether the file itself is accessible
				FileSystem::canonizePath(name_);
				if (!File::isAccessible(name_))
				{
					setstate(std::ios_base::failbit);
					throw Exception::FileNotFound(__FILE__, __LINE__, name_);
				}
			}	
		}

		std::fstream::open(name_.c_str(), open_mode);
		open_mode_ = open_mode;
		is_open_ = is_open();

		return good();
	}

	bool File::reopen()
	{
		close();
		return open(name_, open_mode_);
	}

	bool File::reopen(File::OpenMode open_mode)
	{
		close();
		return open(name_, open_mode);
	}

	bool File::copy(String source_name, String destination_name, Size buffer_size)
	{
		if (source_name == "" || destination_name == "" || source_name == destination_name)
		{
			return false;
		}

		FileSystem::canonizePath(source_name);
		FileSystem::canonizePath(destination_name);

		if (source_name == destination_name)
		{
			return false;
		}

		if (!isAccessible(source_name))
		{
			throw (Exception::FileNotFound(__FILE__, __LINE__, source_name));
		}

		std::ifstream source(source_name.c_str(), std::ios::in);
		std::ofstream destination(destination_name.c_str(), std::ios::out | std::ios::trunc);

		if (!source || !destination)
		{
			return false;
		}

		char* buffer = new char[buffer_size];

		while(source)
		{
			source.read(buffer, (int)buffer_size);

			if (!destination.write(buffer, source.gcount()))
			{
				break;
			}
		}
		
		delete [] buffer;
		source.close();
		destination.close();

		return destination.good();
	}

	void File::close()
	{
		if (is_open_)
		{
			std::fstream::clear();
			std::fstream::close();

			if (is_temporary_)
			{
				remove(name_);
				is_temporary_ = false;
			}

			is_open_ = false;
		}
	}

	Size File::getSize()
	{
		if (!is_open_)
		{
			// dont open the file with File::OUT here, or it might get overwritten
			if (!open(name_, MODE_IN))
			{
				throw Exception::FileNotFound(__FILE__, __LINE__, name_);
			}		
			((std::fstream*)this)->seekg(0, std::ios::end);
			Size size = (Size)((std::fstream*)this)->tellg();
			close();
			return size;
		}

		// store old position in file
		std::streampos old_position = ((std::fstream*)this)->tellg();
		// get filesize
		((std::fstream*)this)->seekg(0, std::ios::end);
		Size size = (Size)((Size)((std::fstream*)this)->tellg() - old_position);

		// reset in file to old position
		((std::fstream*)this)->seekg(old_position);
		
		return size;
	}

	File::Type File::getType(String name, bool trace_link)
	{
		// Canonize the path
		FileSystem::canonizePath(name);
		
		if (!File::isAccessible(name))
		{
			throw Exception::FileNotFound(__FILE__, __LINE__, name);
		}
		#ifdef BALL_COMPILER_MSVC
			struct _stat stats;
			if (_stat(name.c_str(), &stats) < 0)
			{
				return File::TYPE__UNKNOWN;
			}
		#else
			struct stat stats;
			if (trace_link
					?  ::stat(name.c_str(), &stats) < 0
					: ::lstat(name.c_str(), &stats) < 0)
			{ 
				/* unknown file type */
				return File::TYPE__UNKNOWN;
			}
		#endif

		#ifdef BALL_COMPILER_MSVC
			if ((stats.st_mode & _S_IFDIR) == _S_IFDIR)
			{
				return File::TYPE__DIRECTORY;
			}
			if ((stats.st_mode & _S_IFREG)== _S_IFREG)
			{
				return File::TYPE__REGULAR_FILE;
			}
		#else
			if (S_ISREG(stats.st_mode))
			{ 
				/* regular file */
				return File::TYPE__REGULAR_FILE;
			}

			if (S_ISDIR(stats.st_mode))
			{ /* directory */
				return File::TYPE__DIRECTORY;
			}  

			if (S_ISCHR(stats.st_mode))
			{ /* char oriented device */
				return File::TYPE__CHAR_SPECIAL_FILE;
			}

			if (S_ISBLK(stats.st_mode))
			{ /* block oriented device */
				return File::TYPE__BLOCK_SPECIAL_FILE;
			}

			if (S_ISFIFO(stats.st_mode))
			{ /* fifo device */
				return File::TYPE__FIFO_SPECIAL_FILE;
			}
			
			#ifdef S_ISLNK
				if (S_ISLNK(stats.st_mode))
				{ /* symbolic link */
					return File::TYPE__SYMBOLIC_LINK;
				}
			#endif
			#ifdef S_ISSOCK
				if (S_ISSOCK(stats.st_mode)) 
				{ /* socket */
					return File::TYPE__SOCKET;
				}
			#endif
		#endif

		/* unknown file type */
		return File::TYPE__UNKNOWN;
	}

	bool File::createTemporaryFilename(String& temporary, const String& suffix)
	{
		temporary = "_1234567" + suffix;
		
		char a, b, c, d, e, f, g;
		
		for (a = 'A'; a <= 'Z'; a++)
			for (b = 'A'; b <= 'Z'; b++)
				for (c = 'A'; c <= 'Z'; c++)
					for (d = 'A'; d <= 'Z'; d++)
						for (e = 'A'; e <= 'Z'; e++)
							for (f = 'A'; f <= 'Z'; f++)
								for (g = 'A'; g <= 'Z'; g++)
								{
									temporary[1] = a;
									temporary[2] = b;
									temporary[3] = c;
									temporary[4] = d;
									temporary[5] = e;
									temporary[6] = f;
									temporary[7] = g;
			
									if (created_temp_filenames_.find(temporary) == created_temp_filenames_.end() && ::access(temporary.c_str(), F_OK) < 0)
									{
										created_temp_filenames_.insert(temporary);
										return true;
									}					
								}

		return false;
	}

	TransformationManager& File::getTransformationManager()
	{
		return transformation_manager_;
	}
    
	const TransformationManager& File::getTransformationManager() const
	{
		return transformation_manager_;
	}
    
	void File::enableTransformation(File::Transformation transformation)
	{
		transformation_methods_ |= transformation;
	}

	void File::disableTransformation(Transformation transformation)
	{
		transformation_methods_ &= ~transformation;
	}

	bool File::isTransformationEnabled(Transformation transformation)
	{
		return ((transformation_methods_ & transformation) != 0);
	}

	void File::registerTransformation(const String& pattern, const String& exec)
	{
		transformation_manager_.registerTransformation(pattern, exec);
	}

	void File::unregisterTransformation(const String& pattern)
	{
		transformation_manager_.unregisterTransformation(pattern);
	}

#	ifdef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/SYSTEM/file.iC>
#	endif

} // namespace BALL
