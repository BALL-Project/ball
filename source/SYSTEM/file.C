// $Id: file.C,v 1.10 2000/07/12 20:45:08 oliver Exp $

#include <BALL/SYSTEM/file.h>


#include <math.h>

using std::ios;
using std::fstream;
using std::ifstream;
using std::ofstream;
using std::streampos;
using std::endl;

namespace BALL 
{

  const File::OpenMode File::IN;
  const File::OpenMode File::OUT;
  const File::OpenMode File::APP;
  const File::OpenMode File::BINARY;
  const File::OpenMode File::ATE;
  const File::OpenMode File::TRUNC;
 
	File::File()
		:	fstream(),
			name_(),
			open_mode_(ios::in),
			is_open_(false),
			is_temporary_(false)
	{
	}

	File::File(const String& name, File::OpenMode open_mode)
		:	fstream(),
			name_(),
			open_mode_(ios::in),
			is_open_(false),
			is_temporary_(false)
	{
		open(name, open_mode);
	}

	File::File(const File& file)
	{
		name_ = file.name_;
		open_mode_ = file.open_mode_;
		is_open_ = file.is_open_;
		is_temporary_ = file.is_temporary_;

		open(name_, open_mode_);
	}

	File::~File()
	{
		close();
	}

	bool File::open(const String& name, File::OpenMode open_mode)
	{
		close();
		
		name_ = name;
		
		FileSystem::canonizePath(name_);

		fstream::open(name_.c_str(), open_mode);

		open_mode_ = open_mode;
		is_open_ = is_open();

		return good();
	}

	bool File::reopen()
	{
		close();

		return open(name_, open_mode_);
	}

	bool File::copy(String source_name, String destination_name, Size buffer_size)
	{

		if (source_name == "" || destination_name == "" || source_name == destination_name)
		{
			return false;
		}

		FileSystem::canonizePath(source_name);
		FileSystem::canonizePath(destination_name);

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
		};
		
		delete [] buffer;
		source.close();
		destination.close();

		if (destination)
		{
			return true;
		}
		else 
		{
			return false;
		}
	}

	void File::close()
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
		Size size = (Size)(((fstream*)this)->tellg() - old_position);
		((fstream*)this)->seekg(old_position);
		
		return size;
	}

	File::Type File::getType(String name, bool trace_link)
	{
		struct stat stats;
		
		FileSystem::canonizePath(name);
		
		if ((trace_link == true) 
				?  ::stat(name.c_str(), &stats) < 0
				: ::lstat(name.c_str(), &stats) < 0)
		{ /* unknown file type */
			return File::TYPE__UNKNOWN;
		}
		else if (S_ISREG(stats.st_mode))
		{ /* regular file */
			return File::TYPE__REGULAR_FILE;
		}
		else if (S_ISDIR(stats.st_mode))
		{ /* directory */
			return File::TYPE__DIRECTORY;
		}  
		else if (S_ISCHR(stats.st_mode))
		{ /* char oriented device */
			return File::TYPE__CHAR_SPECIAL_FILE;
		}
		else if (S_ISBLK(stats.st_mode))
		{ /* block oriented device */
			return File::TYPE__BLOCK_SPECIAL_FILE;
		}
		else if (S_ISFIFO(stats.st_mode))
		{ /* fifo device */
			return File::TYPE__FIFO_SPECIAL_FILE;
		}
#ifdef S_ISLNK
		else if (S_ISLNK(stats.st_mode))
		{ /* symbolic link */
			return File::TYPE__SYMBOLIC_LINK;
		}
#endif
#ifdef S_ISSOCK
		else if (S_ISSOCK(stats.st_mode)) 
		{ /* socket */
			return File::TYPE__SOCKET;
		}
#endif
		else
		{ /* unknown file type */
			return File::TYPE__UNKNOWN;
		}
	}

	bool File::createTemporaryFilename(String& temporary)
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

#	ifdef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/SYSTEM/file.iC>
#	endif

} // namespace BALL
