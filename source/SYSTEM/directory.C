// $Id: directory.C,v 1.4 2000/06/13 14:12:56 amoll Exp $

//#include <assert.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
//#include <sys/types.h>
//#include <unistd.h>
//#include <iostream>
#include <BALL/SYSTEM/directory.h>

#define BALL_DIRECTORY_STAT   ::lstat

namespace BALL 
{
	Directory::Directory()
	{
		char* buffer_;
		::getcwd(buffer_, sizeof(buffer_));
		dir_ = 0;
		dirent_ = 0;
		directory_path_.set(buffer_);
		cout<<endl<<buffer_;
	}

	Directory::Directory(const String& directory_path, bool set_current = false)
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
	{
	}

	Directory::~Directory()
	{
		if (dir_ != 0) ::closedir(dir_);
	}

	void Directory::clear()
	{
		dir_ = 0;
		dirent_ = 0;
		directory_depth_ = 0;
		directory_path_ = "";
	}

	void Directory::destroy()
	{
		clear();
	}
	
	void Directory::set(const String& directory_path, bool set_current)
	{
		dir_ = 0;
		dirent_ = 0;
		directory_depth_ = 0;
		directory_path_ = directory_path;
		if (set_current) ::chdir(directory_path_.data());
	}

	void Directory::set(const Directory& directory)
	{
		dir_ = 0;
		dirent_ = 0;
		directory_depth_ = 0;
		directory_path_ = directory.directory_path_;
	}

	Directory& Directory::operator = (const Directory& directory)
	{
		set(directory);
		return *this;
	}

	void Directory::get(Directory& directory) const
	{
		directory.set(*this);
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

	const String& Directory::getPath() const
	{
		return directory_path_;
	}

	bool Directory::rename(String old_path, String new_path)
	{ 
		FileSystem::canonizePath(old_path);
		FileSystem::canonizePath(new_path);
		return (::rename(old_path.data(), new_path.data()) == 0);
	}

	bool Directory::renameTo(String new_path)
	{
		if (rename(directory_path_, new_path))
		{
			directory_path_ = new_path;
			return true;
		} else return false;
	}

	bool Directory::setCurrent(String directory_path)
	{
		FileSystem::canonizePath(directory_path);
		return (bool)(::chdir(directory_path.data()) == 0);
	}

	bool Directory::setCurrent()
	{
		return Directory::setCurrent(directory_path_);
	}

	bool Directory::create(String path, const mode_t& mode)
	{
		FileSystem::canonizePath(path);
		return (bool)(::mkdir(path.data(), mode) == 0);
	}

	bool Directory::remove(String path)
	{
		FileSystem::canonizePath(path);
		return (bool)(::rmdir(path.data()) == 0);
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
		String s;
		dirent_ = ::readdir(dir_);
		if (dirent_ == 0)
		{
			::closedir(dir_);
			dir_ = 0;
			return false;
		}
		else  entry = dirent_->d_name;
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
		dirent* dir_entry = 0;
		while((dir_entry = ::readdir(dir)) != 0)
		{
			++size;
		}
		::closedir(dir);
		return (size - 2); // ignore current (.) and parent directory entry (..)
	}

	Size Directory::countFiles() const
	{
		struct stat stats;
		Size size = 0;
		
		DIR* dir = ::opendir(directory_path_.data());
		if (dir == 0)	return 0;

		dirent* dir_entry = 0;
		while((dir_entry = ::readdir(dir)) != 0)
		{
			char* c;
			if (BALL_DIRECTORY_STAT(c, &stats) < 0) /* BALL_DIRECTORY_STAT error */
			{ 
				continue;
			}
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

		dirent *dir_entry = 0;
		while((dir_entry = ::readdir(dir)) != 0)
		{
			char* c;
			if (BALL_DIRECTORY_STAT(c, &stats) < 0) /* BALL_DIRECTORY_STAT error */
			{
				continue;
			}
			if (S_ISDIR(stats.st_mode) != 0) ++size;
		}

		::closedir(dir);
		return (size - 2); // ignore current (.) and parent directory entry (..)
	}

	bool Directory::find(const String &filename, String &filepath, bool recursive = false)
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

	bool Directory::find(const String &filename, String &filepath, bool recursive) const
	{
		return find(filename, filepath, recursive);
	}
/*
	bool Directory::apply(DirectoryApplicator &directoryApplicator)
	{
		Directory directory;
	  BALLApplicator::Result result = BALLApplicator::ABORT;
		String path;
		bool result = directory.getFirstEntry(path);
		
		for (; result == true; result = directory.getNextEntry(path))
		{
			result = directoryApplicator(path);
			
			if (result <= BALLApplicator::BREAK) break;}
		}
		
		return (bool)(result >= BALLApplicator::BREAK);
	}

	bool Directory::apply(DirectoryRecursiveApplicator &directoryRecursiveApplicator)
	{
		directory_depth_ = 0L;
		String path(directory_path_);
		return (bool)(apply_(directoryRecursiveApplicator,
						 path) >= BALLApplicator::BREAK);
	}
*/
	bool Directory::has(const String& filename, bool recursive = false) const
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
		::getcwd(buffer_, sizeof(buffer_));

		return (bool)(directory_path_ == buffer_);
	}

	bool Directory::isEmpty() const
	{
		return (bool)(countItems() == 0);
	}

	bool Directory::operator == (const Directory& directory) const
	{
		return (bool)(directory_path_ == directory.directory_path_);
	}

	bool Directory::operator != (const Directory& directory) const
	{
		return (bool)(directory_path_ != directory.directory_path_);
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
		
		if (BALL_DIRECTORY_STAT(s.c_str(), &stats) < 0)
		{ // BALL_DIRECTORY_STAT error 

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

} // namespace BALL 
