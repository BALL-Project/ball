// $Id: file.C,v 1.23 2001/12/30 00:09:43 oliver Exp $

#include <BALL/SYSTEM/file.h>
#include <BALL/SYSTEM/TCPTransfer.h>
#include <math.h>

using std::ios;
using std::fstream;
using std::ifstream;
using std::ofstream;
using std::streampos;
using std::endl;

// maximum number of substitutions performed in TransformationManager::transform()
// (in order to avoid infinite recursion)
#define MAX_SUBSTITUTIONS 10

namespace BALL 
{

	TransformationManager::TransformationManager()
	{
	}
	
	TransformationManager::~TransformationManager()
	{
	}

	void TransformationManager::registerTransformation(const String& pattern, const String& command)
	{
		transformation_methods_.insert(std::pair<String, String>(pattern, command));
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
			while ((result.substitute("%s", name) != String::EndPos) && (++count <= MAX_SUBSTITUTIONS));

			// substitute "%f" with the full name without the last file suffix (after and including the last ".")
			// and "%f[suffix]" with the full filename without [suffix]
			if (result.hasSubstring("%f["))
			{
				std::cout << "has substring %f[" << std::endl;
				String full_name = name;
				RegularExpression suffix_regexp("%f\\[[^\\]]\\]");
				Substring suffix_substring;
				count = 0;
				std::cout << "suffix_regexp.match: " << suffix_regexp.match(result) << std::endl;
				while (suffix_regexp.find(result, suffix_substring) && (++count <= MAX_SUBSTITUTIONS))
				{
					String suffix = suffix_substring;
					std::cout << "suffix = " << suffix << std::endl;
					suffix = suffix(3, suffix.size() - 4);
					std::cout << "substituting with user-defined prefix:" << name << "/" << suffix << std::endl;
					if (!suffix.empty())
					{
						full_name.substitute(suffix, "");
					}
					suffix_substring = full_name;
				}
			}
			if (result.hasSubstring("%f"))
			{
				String full_name = name;
				String suffix = name(name.find_last_of("."));
				if (!suffix.empty())
				{
					full_name.substitute(suffix +"$", "");
				}
				count = 0;
				while ((result.substitute("%f", full_name) != String::EndPos) && (++count <= MAX_SUBSTITUTIONS));
			}
			// substitute "%f" with the full name without the last file suffix (after and including the last ".")
			// and "%f[suffix]" with the full filename without [suffix]
			if (result.hasSubstring("%b"))
			{
				if (result.hasSubstring("%b["))
				{
				}
				String base_name = FileSystem::baseName(name);
			}

			// "%p" with the path to the file
			count = 0;
			while ((result.substitute("%p", FileSystem::path(name)) != String::EndPos) && (++count <= MAX_SUBSTITUTIONS));

			// "%t" with a temporary file name
			if (result.hasSubstring("%t"))
			{
				String tmp_file;
				File::createTemporaryFilename(tmp_file);
				count = 0;
				while ((result.substitute("%t", tmp_file) != String::EndPos) && (++count <= MAX_SUBSTITUTIONS));
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


  const File::OpenMode File::IN;
  const File::OpenMode File::OUT;
  const File::OpenMode File::APP;
  const File::OpenMode File::BINARY;
  const File::OpenMode File::ATE;
  const File::OpenMode File::TRUNC;
 
	File::File()
		throw()
		:	fstream(),
			name_(),
			open_mode_(ios::in),
			is_open_(false),
			is_temporary_(false)
	{
	}

	File::File(const String& name, File::OpenMode open_mode)
		throw (Exception::FileNotFound)
		:	fstream(),
			name_(),
			open_mode_(ios::in),
			is_open_(false),
			is_temporary_(false)
	{
		if (name == "")
		{
			throw (Exception::FileNotFound(__FILE__, __LINE__, name_));
		}
		open(name, open_mode);
	}

	File::File(const File& file)
		throw (Exception::FileNotFound)
		: fstream(),
			name_(file.name_),
			open_mode_(file.open_mode_),
			is_open_(file.is_open_),
			is_temporary_(file.is_temporary_)
	{
		if (name_ == "")
		{
			throw (Exception::FileNotFound(__FILE__, __LINE__, name_));
		}

		open(name_, open_mode_);
	}

	File::~File()
		throw()
	{
		close();
	}

	bool File::open(const String& name, File::OpenMode open_mode)
		throw (Exception::FileNotFound)
	{
		close();	
		original_name_ = name_ = name;
		is_temporary_ = false;

		// we are reading files
		if (open_mode == IN)
		{
			// check
			String transformation_command = transformation_manager_.findTransformation(name_);
			if (transformation_command != "")
			{
				if (BALL_BIT_IS_CLEARED(transformation_methods_, File::TRANSFORMATION__FILTER))
				{		
					throw Exception::FileNotFound(__FILE__, __LINE__, name_ + " (using " + transformation_command + ")");
				}
				name_ = transformation_manager_.transform(name_);
			}

			// check for the FTP and HTTP transformation prefix
			if (name_.hasPrefix(TRANSFORMATION_FTP_PREFIX) ||
				 	name_.hasPrefix(TRANSFORMATION_HTTP_PREFIX))
			{
				// create a temporary file and redirect the file-transfer to that file
				String tmp_file;
				createTemporaryFilename(tmp_file);
				ofstream out(tmp_file.c_str(), ios::out);
				TCPTransfer tcp_t(out, name, false);
				name_ = tmp_file;
				is_temporary_ = true;
			}
				
			// check for the EXEC transformation prefix
			if (name_.hasPrefix(TRANSFORMATION_EXEC_PREFIX))
			{
				// is the EXEC transformation enabled?
				if (BALL_BIT_IS_CLEARED(transformation_methods_, File::TRANSFORMATION__EXEC))
				{
					throw Exception::FileNotFound(__FILE__, __LINE__, name);
				}
				
				// remove the prefix from the filename
				name_.erase(0, File::TRANSFORMATION_EXEC_PREFIX.size());
					
				// create a temporary file and redirect the command's output to that file
				String tmp_file;
				createTemporaryFilename(tmp_file);
				String command = String("exec ") + name_ + " >" + tmp_file;
				
				// execute the command
				system(command.c_str());
		
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
					throw Exception::FileNotFound(__FILE__, __LINE__, name_);
				}
			}	
		}

		fstream::open(name_.c_str(), open_mode);
		open_mode_ = open_mode;
		is_open_ = is_open();

		return good();
	}

	bool File::reopen()
		throw (Exception::FileNotFound)
	{
		close();
		return open(name_, open_mode_);
	}

	bool File::reopen(File::OpenMode open_mode)
		throw (Exception::FileNotFound)
	{
		close();
		return open(name_, open_mode);
	}

	bool File::copy(String source_name, String destination_name, Size buffer_size)
		throw (Exception::FileNotFound)
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

		char* buffer = new char[buffer_size];
		ifstream source(source_name.c_str(), ios::in);
		ofstream destination(destination_name.c_str(), ios::out | ios::trunc);

		if (!source || !destination)
		{
			return false;
		}
		
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

		return (destination);
	}

	void File::close()
		throw()
	{
		if (is_open_ == true)
		{
			fstream::clear();
			fstream::close();

			if (is_temporary_ == true)
			{
				remove(name_);
				is_temporary_ = false;
			}

			is_open_ = false;
		}
	}

	Size File::getSize()
		throw()
	{
		if (!is_open_)
		{
			if (open(name_, open_mode_) == false)
			{
				return 0;
			}		
		}
		streampos old_position = ((fstream*)this)->tellg();
		((fstream*)this)->seekg(0, ios::end);
		Size size = (Size)((Size)((fstream*)this)->tellg() - old_position);
		((fstream*)this)->seekg(old_position);
		
		return size;
	}

	File::Type File::getType(String name, bool trace_link)
		throw (Exception::FileNotFound)
	{
		struct stat stats;
		FileSystem::canonizePath(name);
		
		if ((trace_link == true) 
				?  ::stat(name.c_str(), &stats) < 0
				: ::lstat(name.c_str(), &stats) < 0)
		{ /* unknown file type */
			return File::TYPE__UNKNOWN;
		}

		if (S_ISREG(stats.st_mode))
		{ /* regular file */
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
		/* unknown file type */
		return File::TYPE__UNKNOWN;
	}

	bool File::createTemporaryFilename(String& temporary)
		throw()
	{
		temporary = "_1234567.TMP";
		
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
			
									if (::access(temporary.c_str(),F_OK) < 0)
									{ 
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
