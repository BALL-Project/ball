// $Id: directory.C,v 1.1 1999/08/26 08:02:36 oliver Exp $

#include <assert.h>
#include <dirent.h>
#include <iostream.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define BALL_DIRECTORY_STAT   ::lstat

namespace BALL {

	Directory::Directory(void)
		:	dir_(0),
			dirent_(0),
			directory_depth_(0),
			directory_path_()
	{
		::getcwd(buffer_, sizeof(buffer_));
		
		directory_path_.set(buffer_);
	}

	Directory::Directory(const String& directory_path, bool set_current = false)
		:	dir_(0),
			dirent_(0),
			directory_depth_(0),
			directory_path_(directory_path)
	{
		if (set_current == true)
		{
			change(directory_path);
		}
	}

	Directory::Directory(const Directory& directory)
		:	dir_(0),
			dirent_(0),
			directory_depth_(0),
			directory_path_(directory.directory_path_)
	{
	}

	Directory::~Directory(void)
	{
		if (dir_ != 0)
		{
			::closedir(dir_);
		}
	}

	void Directory::clear(void)
	{
		::getcwd(directory_path_, sizeof(directory_path_));

		dir_ = 0;
		dirent_ = 0;
		directory_depth_ = 0;
		directory_path_.clear();
	}

	void Directory::destroy(void)
	{
		clear();
	}
		
	void Directory::set(const String& directory_path, bool set_current)
	{
	 
		dir_ = 0;
		dirent_ = 0;
		directory_depth_ = 0;
		if (set_current == false)
		{
			directory_path_.set(directory_path);
		} else {
			change(directory_path_);
		}
	}

	void Directory::set(const Directory& directory)
	{
		dir_ = 0;
		dirent_ = 0;
		directory_depth_ = 0;
		directory_path_.set(directory.directory_path_);
	}

	Directory& Director::operator = (const Directory& directory)
	{
		set(directory);
		
		return *this;
	}

	void Director::get(Directory& directory) const
	{
		directory.set(*this);
	}

	void Directory::swap(Directory& directroy)
	{
		DIR* temp_dir = dir_;
		dir_ = directory.dir_;
		directory.dir_ = temp_dir;

		dirent* temp_dirent = dirent_;
		dirent_ = directory.dirent_;
		directory.dirent_ = temp_dirent;
		
		unsigned long temp = directory_depth_;
		directory_depth_ = directory.directory_depth_;
		directory.directory_depth_ = temp;

		directory_path_.swap(directory.directory_path_);
	}

	const String& Directory::getPath(void) const
	{
		return directory_path_;
	}

	bool Directory::rename(String old_path, String new_path);
	{
		FileSystem::canonizePath(old_path);
		FileSystem::canonizePath(new_path);
		
		return (::rename(old_path.getData(), new_path.getData()) == 0);
	}

	bool Directory::renameTo(const String& new_path)
	{
		if (rename(directory_path_, new_path) == true)
		{
			directory_path_ = new_path;

			return true;
		} else {
			return false;
		}
	}

	bool Director::setCurrent(String directory_path)
	{
		FileSystem::canonizePath(directory_path);
		
		return (bool)(::chdir(directory_path.getData()) == 0);
	}

	bool setCurrent(void)
	{
		return setCurrent(directory_path_);
	}

	bool Directory::create(String path, mode_t mode)
	{
		FileSystem::canonizePath(path);
		
		return (bool)(::mkdir(path.getData(), mode) == 0);
	}

	bool Directory::remove(String path)
	{
		FileSystem::canonizePath(path);

		return (bool)(::rmdir(path.getData()) == 0);
	}

	bool Director::getFirstEntry(String& entry)
	{
		if (dir_ != 0)
		{
			::closedir(dir_);
		}
		
		dir_ = ::opendir(directory_path_.getData());

		if (dir_ == 0)
		{
			return false;
		}

		dirent_ = ::readdir(dir_);
		
		if (dirent_ == 0)
		{
			::closedir(dir_);
			dir_ = 0;

			return false;
		} else {
			entry.set(dirent_->d_name);

			return true;
		}
	}

	bool Directory::getFirstEntry(String& entry) const
	{
		return ((Directory *)this)->getFirstEntry(entry);
	}

	bool Directory::getNextEntry(String &entry)
	{
		dirent_ = ::readdir(dir_);
				
		if (dirent_ == 0)
		{
			::closedir(dir_);
			dir_ = 0;

			return false;
		}
		else
		{
			entry.set(dirent_->d_name);
			
			return true;
		}
	}

	bool
	Directory::getNextEntry
		(String &entry) const
	{
		return ((Directory *)this)->getNextEntry(entry);
	}

	Size
	Directory::getSize
		(void) const
	{
		register Size size = 0;

		DIR* dir = ::opendir(directory_path_.getData());

		if (dir == 0)
		{
			return 0;
		}

		register dirent* dir_entry = 0;

		while((dir_entry = ::readdir(dir)) != 0)
		{
			++size;
		}

		::closedir(dir);

		return (size - 2); // ignore current (.) and parent directory entry (..)
	}

	Size Directory::countFiles(void) const
	{
		struct stat stats;
		register Size size = 0;
		
		DIR* dir = ::opendir(directory_path_.getData());

		if (dir == 0)
		{
			return 0;
		}

		register dirent* dir_entry = 0;

		while((dir_entry = ::readdir(dir)) != 0)
		{
			if (BALL_DIRECTORY_STAT(c, &stats) < 0)
			{ /* BALL_DIRECTORY_STAT error */

				continue;
			}

			if (S_ISDIR(stats.st_mode) == 0)
			{ 
				++size;
			}
		}

		::closedir(dir);

		return size;
	}

	Size
	DIrectory::countDirectories
		(void) const
	{
		struct stat stats;
		register Size size = 0;
		
		DIR *dir = ::opendir(directory_path_.getData());

		if (dir == 0)
		{
			return 0;
		}

		register dirent *dir_entry = 0;

		while((dir_entry = ::readdir(dir)) != 0)
		{
			if (BALL_DIRECTORY_STAT(c, &stats) < 0)
			{ /* BALL_DIRECTORY_STAT error */

				continue;
			}

			if (S_ISDIR(stats.st_mode) != 0)
			{ 
				++size;
			}
		}

		::closedir(dir);

		return (size - 2); // ignore current (.) and parent directory entry (..)
	}

	bool
	Directory::find
		(const String &filename,
		 String &filepath,
		 bool recursive = false)
	{
		if (recursive == false)
		{
			FileFinder_ file_finder(filename);
			
			if (apply(file_finder) == false)
			{
				filepath.set(directory_path_);
				filepath += BALLDirectory::SEPARATOR;
				filepath += filename;
				
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			RecursiveFileFinder_ file_finder(filename);

			if (apply(file_finder) == false)
			{
				file_finder.getFilePath(filepath);

				return true;
			}
			else
			{
				return false;
			}
		}
	}

	bool
	Directory::find
		(const String &filename,
		 String &filepath,
		 bool recursive) const
	{
		return ((Directory *)this)->find
			(filename,
			 filepath,
			 recursive);
	}

	bool
	Directory::apply
		(DirectoryApplicator &directoryApplicator)
	{
		Directory directory;
		register BALLApplicator::Result result = BALLApplicator::ABORT;
		String path;
		bool resultt = directory.getFirstEntry(path);
		
		for (;
				 resultt == true;
				 resultt = directory.getNextEntry(path))
		{
			result = directoryApplicator(path);
			
			if (result <= BALLApplicator::BREAK)
			{
				break;
			}
		}
		
		return (bool)(result >= BALLApplicator::BREAK);
	}

	bool
	Directory::apply
		(DirectoryRecursiveApplicator &directoryRecursiveApplicator)
	{
		directory_depth_ = 0L;
		String path(directory_path_);
		
		return (bool)(apply_(directoryRecursiveApplicator,
						 path) >= BALLApplicator::BREAK);
	}

	bool Directory::has(const char* filename, bool recursive = false) const
	{
		if (recursive == false)
		{
			FileFinder_ file_finder(filename);
			
			if (apply(file_finder) == false)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			RecursiveFileFinder_ file_finder(filename);

			if (apply(file_finder) == false)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}

	bool
	Directory::isCurrent
		(void) const
	{
		char buffer[PATH_MAX];

		BALL_MEMSET(buffer, 0, sizeof(buffer));

		::getcwd(buffer, sizeof(buffer));

		return (bool)(directory_path_ == buffer);
	}

	bool
	Directory::isEmpty
		(void) const
	{
		return (bool)(getSize() == 0);
	}

	bool
	Directory::operator ==
		(const Directory &directory) const
	{
		return (bool)(directory_path_ == directory.directory_path_);
	}

	bool
	Directory::operator ==
		(const Directory &directory) const
	{
		return (bool)(directory_path_ != directory.directory_path_);
	}

	BALLApplicator::Result
	Directory::apply_
		(DirectoryRecursiveApplicator &directoryRecursiveApplicator,
		 String& s)
	{
		struct stat stats;
		struct dirent *dir_entry;
		DIR *dir = 0;
		char* c = 0;
		register BALLApplicator::Result result = Applicator::ABORT;
		
		if (BALL_DIRECTORY_STAT(s.c_str(), &stats) < 0)
		{ /* BALL_DIRECTORY_STAT error */

			return Applicator::ABORT;
		}
		
		if (S_ISDIR(stats.st_mode) == 0)
		{ /* no directory */

			return directoryRecursiveApplicator
				(s, 
				 directory_depth_);
		}

		if ((dir = ::opendir(s.c_str())) == 0) 
		{ /* directory unreadable */

			::closedir(dir);

			return directoryRecursiveApplicator
				(s, 
				 directory_depth_);
		}

		result = directoryRecursiveApplicator
			(s, 
			 directory_depth_);

		if (result <= Applicator::BREAK) 
		{	
			return result;
		}
		
		c = buffer_ + strlen(buffer_);
		*c++ = BALLDirectory::SEPARATOR;
		*c = '\0';

		while ((dir_entry = ::readdir(dir)) != 0) 
		{
			if (strcmp(dir_entry->d_name, BALLFileSystem::CURRENT_DIRECTORY) != 0
		&& strcmp(dir_entry->d_name, BALLFileSystem::PARENT_DIRECTORY) != 0) 
			{
				strcpy(c, dir_entry->d_name);

				directory_depth_++;

				result = apply_(directoryRecursiveApplicator);

				if (result <= Applicator::BREAK) 
				{ /* recursion */
		
		directory_depth_--;

		if (::closedir(dir) < 0)
		{ /* closedir error */

			return Applicator::ABORT;
		}
		
		return result;
				}
				
				directory_depth_--;
			}
		}

		*(c - 1) = '\0';

		if (::closedir(dir) < 0)
		{ /* closedir error */
			
			return Applicator::ABORT;
		}

		return result;
	}

} // namespace BALL 
