// $Id: directory.C,v 1.7 2000/06/17 10:54:32 oliver Exp $

#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <BALL/SYSTEM/directory.h>

namespace BALL 
{
	Directory::Directory()
	{
		dir_ = 0;
		dirent_ = 0;
		char* buffer_;
		if ((buffer_ = ::getcwd(NULL, 64)) != NULL)
			directory_path_ = buffer_;
		else directory_path_ = "";
	}

	Directory::Directory(const String& directory_path, bool set_current)
		:	dir_(0),
			dirent_(0),
			directory_depth_(0),
			directory_path_(directory_path)
	{
		if (set_current) ::chdir(directory_path.data());
	}

	Directory::Directory(const Directory& directory)
		:	dir_(0),
			dirent_(0),
			directory_depth_(0),
			directory_path_(directory.directory_path_)
	{}

	Directory::~Directory()
	{
		if (dir_ != 0) ::closedir(dir_);
	}

	void Directory::swap(Directory& directory)
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


	bool Directory::getFirstEntry(String& entry) 
	{
		if (dir_ != 0) ::closedir(dir_);
		dir_ = ::opendir(directory_path_.data());
		if (dir_ == 0) return false;
		dirent_ = ::readdir(dir_);
		if (dirent_ == 0)
		{
			::closedir(dir_);
			dir_ = 0;
			return false;
		} 
		else 
		{
			entry = dirent_->d_name;
			return true;
		}
	}

	bool Directory::getFirstEntry(String& entry) const
	{
		return getFirstEntry(entry);
	}

	bool Directory::getNextEntry(String& entry)
	{
		if (dir_ == 0) dir_ = ::opendir(directory_path_.data());

		if (dir_ == 0) return false;
		dirent_ = ::readdir(dir_);
		if (dirent_ == 0)
		{
			if (dir_ != 0)
			{
				::closedir(dir_);
				dir_ = 0;
			}
			return false;
		}
		entry = dirent_->d_name;
		return true;
	}

	bool Directory::getNextEntry(String& entry) const
	{
		return getNextEntry(entry);
	}

	Size Directory::countItems() const
	{
		Size size = 0;
		DIR* dir = ::opendir(directory_path_.data());
		if (dir == 0)	return 0;
		while(::readdir(dir) != 0) ++size;
		::closedir(dir);
		return (size - 2); // ignore current (.) and parent directory entry (..)
	}

	Size Directory::countFiles() const
	{
		struct stat stats;
		Size size = 0;
		DIR* dir = ::opendir(directory_path_.data());
		if (dir == 0)	return 0;

		while(readdir(dir) != 0)
		{
			char* c;
			if (lstat(c, &stats) < 0)	continue;
			if (S_ISDIR(stats.st_mode) == 0) ++size;
		}

		::closedir(dir);
		return size;
	}

	Size Directory::countDirectories() const
	{
		struct stat stats;
		Size size = 0;
		DIR *dir = ::opendir(directory_path_.data());
		if (dir == 0)	return 0;

		while(::readdir(dir) != 0)
		{
			char* c;
			if (lstat(c, &stats) < 0)	continue;
			if (S_ISDIR(stats.st_mode) != 0) ++size;
		}

		::closedir(dir);
		return (size - 2); // ignore current (.) and parent directory entry (..)
	}

	bool Directory::find(const String &filename, String &filepath, bool recursive)
	{/*
		if (!recursive)
		{
			FileFinder_ file_finder(filename);
			if (!apply(file_finder))
			{
				filepath = directory_path_;
				filepath += FileSystem::PATH_SEPARATOR;
				filepath += filename;
				return true;
			}
			else return false;
		}
		else
		{
			RecursiveFileFinder_ file_finder(filename);
			if (!apply(file_finder))
			{
				file_finder.getFilePath(filepath);*/
				return true;
	/*		}
			else return false;
		}*/
	}

	bool Directory::has(const String& filename, bool recursive) const
	{
		if (!recursive)
		{
			FileFinder_ file_finder(filename);
		}
		else
		{
			//RecursiveFileFinder_ file_finder(filename);
		}
		//return (!apply(file_finder));
		return true;
	}

	bool Directory::isCurrent() const
	{
		char* buffer_;
		if ((buffer_ = ::getcwd(NULL, 64)) == NULL)
		{
			return false;
		}
		return (bool)(directory_path_ == buffer_);
	}

	BALL::Processor::Result Directory::FileFinder_::operator() (const String& path)
	{
		const char* filename = strrchr(path.data(), FileSystem::PATH_SEPARATOR);

		if ((filename != 0 && strcmp(filename + 1, filename_.data()) == 0)
				|| path == filename_)
		{
			return Processor::ABORT;
		} else return Processor::CONTINUE;
	}

/*
	BALLApplicator::Result Directory::apply_
		(DirectoryRecursiveApplicator &directoryRecursiveApplicator,
		 String& s)
	{
		struct stat stats;
		struct dirent *dir_entry;
		DIR *dir = 0;
		char* c = 0;
		 BALLApplicator::Result result = Applicator::ABORT;
		
		if (lstat(s.c_str(), &stats) < 0)
		{
			return Applicator::ABORT;
		}
		
		if (S_ISDIR(stats.st_mode) == 0) // no directory
		{ 
			return directoryRecursiveApplicator(s, directory_depth_);
		}

		if ((dir = ::opendir(s.c_str())) == 0) 
		{ // directory unreadable 

			::closedir(dir);

			return directoryRecursiveApplicator(s, directory_depth_);
		}

		result = directoryRecursiveApplicator(s, directory_depth_);

		if (result <= Applicator::BREAK) return result;
		
		c = buffer_ + strlen(buffer_);
		*c++ = FileSystem::PATH_SEPARATOR;
		*c = '\0';

		while ((dir_entry = ::readdir(dir)) != 0) 
		{
			if (strcmp(dir_entry->d_name, BALLFileSystem::CURRENT_DIRECTORY) != 0
			 && strcmp(dir_entry->d_name, BALLFileSystem::PARENT_DIRECTORY ) != 0) 
			{
				strcpy(c, dir_entry->d_name);

				directory_depth_++;

				result = apply_(directoryRecursiveApplicator);

				if (result <= Applicator::BREAK) // recursion 
				{
					directory_depth_--;

					if (::closedir(dir) < 0)// closedir error
					{ 
						return Applicator::ABORT;
					}
					return result;
				}
				directory_depth_--;
			}
		}

		*(c - 1) = '\0';

		if (::closedir(dir) < 0)// closedir error
		{ 
			return Applicator::ABORT;
		}
		return result;
	}*/

#       ifdef BALL_NO_INLINE_FUNCTIONS
#               include <BALL/SYSTEM/directory.iC>
#       endif

} // namespace BALL 
