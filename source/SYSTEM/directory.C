// $Id: directory.C,v 1.13 2000/07/03 12:24:52 oliver Exp $

#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <errno.h>
#include <BALL/SYSTEM/directory.h>

#define BALL_MAX_PATH_LENGTH 8192

namespace BALL 
{
	Directory::Directory()
	{
		dir_ = 0;
		dirent_ = 0;
		char* buffer_;
		if ((buffer_ = ::getcwd(NULL, BALL_MAX_PATH_LENGTH)) != NULL)	directory_path_ = buffer_;
		else directory_path_ = "";
	}

	Directory::Directory(const String& directory_path, bool set_current)
	{
		if (!set(directory_path, set_current)) directory_path_ = "";
	}

	Directory::Directory(const Directory& directory)
	{
		set(directory);
	}

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

		backup_path_.swap(directory.backup_path_);		
		directory_path_.swap(directory.directory_path_);
	}

	bool Directory::getFirstEntry(String& entry) 
	{
		synchronize_();
		if (dir_ != 0) ::closedir(dir_);
		dir_ = ::opendir(directory_path_.data());
		if (dir_ == 0) return desynchronize_(false);
		dirent_ = ::readdir(dir_);
		if (dirent_ == 0)
		{
			::closedir(dir_);
			dir_ = 0;
			return desynchronize_(false);
		} 
		else 
		{
			entry = dirent_->d_name;
			return desynchronize_(true);
		}
	}

	bool Directory::getNextEntry(String& entry)
	{
		synchronize_();
		if (dir_ == 0) dir_ = ::opendir(directory_path_.data());
		if (dir_ == 0) return desynchronize_(false);

		dirent_ = ::readdir(dir_);
		if (dirent_ == 0)
		{
			::closedir(dir_);
			dir_ = 0;
			return desynchronize_(false);
		}
		entry = dirent_->d_name;
		return desynchronize_(true);
	}

	Size Directory::countItems()
	{
		synchronize_();
		Size size = 0;
		DIR* dir = ::opendir(directory_path_.data());
		if (dir == 0)	
		{	
			desynchronize_();
			return 0;
		}
		while(::readdir(dir) != 0) ++size;
		::closedir(dir);
		desynchronize_();
		return (size - 2); 
	}	// ignore current (.) and parent directory entry (..)

	Size Directory::countFiles()
	{
		synchronize_();
		struct stat stats;
		Size size = 0;
		dirent* myDirent;
		DIR* dir = ::opendir(directory_path_.data());
		if (dir == 0)	
		{	
			desynchronize_();
			return 0;
		}
		while((myDirent = ::readdir(dir)) != 0)
		{
			if (lstat(myDirent->d_name, &stats) < 0)	continue;
			if (S_ISDIR(stats.st_mode) == 0) ++size;
		}
		::closedir(dir);
		desynchronize_();
		return size;
	}

	Size Directory::countDirectories()
	{
		synchronize_();
		struct stat stats;
		Size size = 0;
		dirent* myDirent;
		DIR *dir = ::opendir(directory_path_.data());
		if (dir == 0)	
		{	
			desynchronize_();
			return 0;
		}
		while((myDirent = ::readdir(dir)) != 0)
		{
			if (lstat(myDirent->d_name, &stats) < 0)	continue;
			if (S_ISDIR(stats.st_mode) != 0) ++size;
		}
		::closedir(dir);
		desynchronize_();
		return (size - 2);
	}		// ignore current (.) and parent directory entry (..)

	bool Directory::isCurrent() const
	{
		char* buffer_;
		if ((buffer_ = ::getcwd(NULL, BALL_MAX_PATH_LENGTH)) == 0) return false;
		return (buffer_ == directory_path_);
	}

	bool Directory::has(const String& item) //const
	{	
		synchronize_();
		String entry;
		while (getNextEntry(entry))
		{
			if (entry == item) return desynchronize_(true);
		}
		return desynchronize_(false);
	}

	bool Directory::find(const String& item, String& filepath)
	{
		if (has(item))
		{
			filepath = directory_path_;
			return true; // no sync needed...
		}
		synchronize_();
		struct stat stats;
		dirent* myDirent;
		Directory directory;
		String s;
		DIR* dir = ::opendir(FileSystem::CURRENT_DIRECTORY);
		if (dir == 0)	return desynchronize_(false);

		while((myDirent = ::readdir(dir)) != 0)
		{
			if (lstat(myDirent->d_name, &stats) < 0) continue;
			if (S_ISDIR(stats.st_mode) != 0 && 
					strcmp(myDirent->d_name, FileSystem::CURRENT_DIRECTORY) != 0 &&
			    strcmp(myDirent->d_name, FileSystem::PARENT_DIRECTORY ) != 0) 
			{
				directory =Directory(myDirent->d_name);
				if (directory.find(item, s))
				{
					filepath = s;
					::closedir(dir);
					return desynchronize_(true);
				}
			}
		}
		::closedir(dir);
		return desynchronize_(false);
	}

	bool Directory::set(const String& directory_path, bool set_current)
	{
		dir_ = 0;
		dirent_ = 0;
		backup_path_ = "";
		char* buffer_;
		if (directory_path[0] == '/')		//absolute path
		{
			directory_path_ = directory_path;
			FileSystem::canonizePath(directory_path_);
			return isValid();
		}
		if ((buffer_ = ::getcwd(NULL, BALL_MAX_PATH_LENGTH)) != NULL)
		{
			directory_path_ = buffer_;
			directory_path_ += FileSystem::PATH_SEPARATOR;
			directory_path_ += directory_path;
			FileSystem::canonizePath(directory_path_);
			if (directory_path_.hasSuffix(String(FileSystem::PATH_SEPARATOR)))
			{
				directory_path_.truncate(directory_path_.size() - 1);
			}
		}
		else 
		{	
			directory_path_ = "";
			return false;
		}
		if (!isValid())	return false;
		if (set_current) return (::chdir(directory_path_.data()) == 0);
		return true;
	}

	bool Directory::remove()
	{
		synchronize_();	
		if (::chdir("..") != 0) return desynchronize_(false);
		bool result1 = (::rmdir(directory_path_.data()) == 0);
		bool result2;
		if (backup_path_ != "")
		{
			result2 = ::chdir(backup_path_.data());
			backup_path_ = "";
		}
		dir_ = 0;
		dirent_ = 0;
		directory_path_ = "";
		return (result1 && result2);
	}

	bool Directory::renameTo(String new_path)
	{
		synchronize_();
		FileSystem::canonizePath(new_path);
		if (::chdir("..") != 0)	return desynchronize_(false);
		if (::rename(directory_path_.data(), new_path.data()) == 0)
		{
			directory_path_ = new_path;
			return desynchronize_(true);
		}
		return desynchronize_(false);
	}

#       ifdef BALL_NO_INLINE_FUNCTIONS
#               include <BALL/SYSTEM/directory.iC>
#       endif

} // namespace BALL 
