// $Id: file.C,v 1.4 1999/12/17 11:18:04 oliver Exp $

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

	File::ActionManager File::action_manager_;
	unsigned char File::protocol_ability_ = BALL_BIT(File::PROTOCOL__FILE)
																									 | BALL_BIT(File::PROTOCOL__EXEC)
																									 | BALL_BIT(File::PROTOCOL__ACTION);



	File::File()
		:	fstream(),
			name_(),
			original_name_(),
			open_mode_(ios::in),
			is_open_(false),
			is_temporary_(false)
	{
	}

	File::File(const String& name, File::OpenMode open_mode)
		:	fstream(),
			name_(),
			original_name_(),
			open_mode_(ios::in),
			is_open_(false),
			is_temporary_(false)
	{
		open(name, open_mode);
	}

	File::~File()
	{
		close();
	}

	bool File::open(const String& name, File::OpenMode open_mode)
	{
		close();
		
		original_name_ = name_ = name;
		
		if ((open_mode == ios::in) && (name_.hasPrefix("exec:") == true))
		{
			if (BALL_BIT_IS_CLEARED(protocol_ability_, File::PROTOCOL__EXEC))
			{
				return false;
			}
			
			name_.erase(0, 5);
			
			String exec_string("exec ");
			exec_string += name_;
			createTemporaryFilename(name_);
			exec_string += '>';
			exec_string += name_;
			system(exec_string.c_str());

			is_temporary_ = true;
		} else {
			if (name_.hasPrefix("file:") == true)
			{
				if (BALL_BIT_IS_CLEARED(protocol_ability_, File::PROTOCOL__FILE))
				{
					return false;
				}

				name_.erase(0, 5);
			}

			FileSystem::canonizePath(name_);

			if (open_mode == ios::in)
			{
				String* exec_string_ptr = action_manager_.findExecString(name_);

				if ((File::isAccessible(name_) == true) && (exec_string_ptr != 0))
				{
					if (BALL_BIT_IS_CLEARED(protocol_ability_, File::PROTOCOL__ACTION))
					{
						return false;
					}

					String exec_string("exec ");
					exec_string += *exec_string_ptr;
					exec_string.substitute("%s", name_);
					createTemporaryFilename(name_);
					exec_string += '>';
					exec_string += name_;
					system(exec_string.c_str());
		
					is_temporary_ = true;
				} else {
					is_temporary_ = false;
				}
			} else {
				is_temporary_ = false;
			}
		}

		fstream::open(name_.c_str(), open_mode);

		open_mode_ = open_mode;
		is_open_ = is_open();

		return good();
	}

	bool File::reopen()
	{
		close();

		return open(original_name_, open_mode_);
	}

	bool File::copy(String source_name, String destination_name, Size buffer_size)
	{
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

		if (destination)
		{
			source.close();
			destination.close();

			return true;
		} else {
			source.close();
			destination.close();

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

	Size File::getSize() const
	{
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
		
		register char a, b, c, d, e, f, g;
		
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
