// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: directory.C,v 1.19 2002/12/16 09:08:29 oliver Exp $

#include <BALL/SYSTEM/directory.h>

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#ifdef BALL_HAS_SYS_TYPES_H
#	include <sys/types.h>
#endif
#ifdef BALL_HAS_UNISTD_H
#	include <unistd.h>
#endif

#include <iostream>
#include <errno.h>

namespace BALL 
{
	const Size Directory::MAX_PATH_LENGTH = 8192;
	
	Directory::Directory()
	{
#ifdef BALL_COMPILER_MSVC
		dir_= INVALID_HANDLE_VALUE;
		dirent_ = INVALID_HANDLE_VALUE;
		char* buffer_;

		if ((buffer_ = ::getcwd(NULL, MAX_PATH_LENGTH)) != NULL)	
		{
			directory_path_ = buffer_;
			dir_ = CreateFile(buffer_,
												FILE_LIST_DIRECTORY,                // access (read/write) mode
												FILE_SHARE_READ|FILE_SHARE_DELETE|FILE_SHARE_WRITE,  // share mode
												NULL,                               // security descriptor
												OPEN_EXISTING,                      // how to create
												FILE_FLAG_BACKUP_SEMANTICS,         // file attributes
												NULL                                // file with attributes to copy
												);
			

		}
		else 
		{
			directory_path_ = "";
		}
#else
		dir_ = 0;
		dirent_ = 0;
		char* buffer_;
		if ((buffer_ = ::getcwd(NULL, MAX_PATH_LENGTH)) != NULL)	
		{
			directory_path_ = buffer_;
		}
		else 
		{
			directory_path_ = "";
		}
#endif
	}

	Directory::Directory(const String& directory_path, bool set_current)
	{
		if (!set(directory_path, set_current)) 
		{
			directory_path_ = "";
		}
		
#ifdef BALL_COMPILER_MSVC
		dir_= INVALID_HANDLE_VALUE;
		dirent_ = INVALID_HANDLE_VALUE;
		dir_ = CreateFile(directory_path_.data(),
												FILE_LIST_DIRECTORY,                // access (read/write) mode
												FILE_SHARE_READ|FILE_SHARE_DELETE|FILE_SHARE_WRITE,  // share mode
												NULL,                               // security descriptor
												OPEN_EXISTING,                      // how to create
												FILE_FLAG_BACKUP_SEMANTICS,         // file attributes
												NULL                                // file with attributes to copy
												);
		

#endif

	}

	Directory::Directory(const Directory& directory)
	{
		set(directory);
	}

	Directory::~Directory()
	{
#ifdef BALL_COMPILER_MSVC
		CloseHandle(dir_);
		FindClose(dirent_);
#else
		if (dir_ != 0) ::closedir(dir_);
#endif
	}

	void Directory::swap(Directory& directory)
	{
#ifdef BALL_COMPILER_MSVC
		HANDLE temp_dir = dir_;
		dir_=directory.dir_;
		directory.dir_ = temp_dir;

		HANDLE temp_dirent = dirent_;
		dirent_ = directory.dirent_;
		directory.dirent_ = temp_dirent;
#else
		DIR* temp_dir = dir_;
		dir_ = directory.dir_;
		directory.dir_ = temp_dir;

		dirent* temp_dirent = dirent_;
		dirent_ = directory.dirent_;
		directory.dirent_ = temp_dirent;
#endif
		backup_path_.swap(directory.backup_path_);		
		directory_path_.swap(directory.directory_path_);
	}

	bool Directory::getFirstEntry(String& entry) 
	{
#ifdef BALL_COMPILER_MSVC
		synchronize_();
		if(dir_!=INVALID_HANDLE_VALUE) CloseHandle(dir_);
		dir_ = CreateFile(directory_path_.data(),
												FILE_LIST_DIRECTORY,                // access (read/write) mode
												FILE_SHARE_READ|FILE_SHARE_DELETE|FILE_SHARE_WRITE,  // share mode
												NULL,                               // security descriptor
												OPEN_EXISTING,                      // how to create
												FILE_FLAG_BACKUP_SEMANTICS,         // file attributes
												NULL                                // file with attributes to copy
												);
		if(dir_==INVALID_HANDLE_VALUE) return desynchronize_(false);
		
		if(dirent_!=INVALID_HANDLE_VALUE) FindClose(dirent_);
		
		String pat=directory_path_+"\\*";
		WIN32_FIND_DATA fd;
		
	
		dirent_=FindFirstFile(pat.data(),&fd);
		if(dirent_==INVALID_HANDLE_VALUE)
		{
			CloseHandle(dir_);
			dir_=INVALID_HANDLE_VALUE;
			return desynchronize_(false);
		}
		else
		{
			entry = fd.cFileName;
			
			return desynchronize_(true);
		}

#else
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
#endif
	}

	bool Directory::getNextEntry(String& entry)
	{
#ifdef BALL_COMPILER_MSVC
		synchronize_();
		
		if(dir_==INVALID_HANDLE_VALUE)
		{
			
			dir_ = CreateFile(directory_path_.data(),
												FILE_LIST_DIRECTORY,                
												FILE_SHARE_READ|FILE_SHARE_DELETE|FILE_SHARE_WRITE,  
												NULL,                               
												OPEN_EXISTING,                     
												FILE_FLAG_BACKUP_SEMANTICS,        
												NULL                                
												);
			
			if(dir_==INVALID_HANDLE_VALUE) return desynchronize_(false);
			if(dirent_!=INVALID_HANDLE_VALUE) FindClose(dirent_);

			String pat=directory_path_ + FileSystem::PATH_SEPARATOR + "*";
			WIN32_FIND_DATA fd;
			
			dirent_=FindFirstFile(pat.data(),&fd);
			
			if(dirent_==INVALID_HANDLE_VALUE)
			{
				CloseHandle(dir_);
				dir_=INVALID_HANDLE_VALUE;
				return desynchronize_(false);
			}
		}
		if(dir_==INVALID_HANDLE_VALUE) return desynchronize_(false);
		
		WIN32_FIND_DATA fd;
		
		if(dirent_ == INVALID_HANDLE_VALUE)
		{
			// someone has forgot to call FirstEntry

			String pat=directory_path_ + FileSystem::PATH_SEPARATOR + "*";
			dirent_=FindFirstFile(pat.data(),&fd);
			
			if(dirent_==INVALID_HANDLE_VALUE)
			{
				CloseHandle(dir_);
				dir_=INVALID_HANDLE_VALUE;
				return desynchronize_(false);
			}
			else
				entry = fd.cFileName;
		}
		else
		{
			if(FindNextFile(dirent_,&fd)==0) return desynchronize_(false);
			entry=fd.cFileName;
		}
		return desynchronize_(true);

#else
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
#endif
	}

	Size Directory::countItems()
	{
#ifdef BALL_COMPILER_MSVC
		synchronize_();
		Size size =0;
		if(dir_==INVALID_HANDLE_VALUE)
		{
			dir_ = CreateFile(directory_path_.data(),
												FILE_LIST_DIRECTORY,                
												FILE_SHARE_READ|FILE_SHARE_DELETE|FILE_SHARE_WRITE,  
												NULL,                               
												OPEN_EXISTING,                     
												FILE_FLAG_BACKUP_SEMANTICS,        
												NULL                                
												);
			if(dir_==INVALID_HANDLE_VALUE)
			{
				desynchronize_();
				return 0;
			}
		}
		
		if(dirent_!=INVALID_HANDLE_VALUE) FindClose(dirent_);
		String pat=directory_path_+"\\*";
		WIN32_FIND_DATA fd;
		dirent_=FindFirstFile(pat.data(),&fd);
		if(dirent_==INVALID_HANDLE_VALUE)
		{
			CloseHandle(dir_);
			dir_=INVALID_HANDLE_VALUE;
			desynchronize_();
			return 0;
		}
		++size;
		while(FindNextFile(dirent_,&fd)!=0) ++size;
		FindClose(dirent_);
		desynchronize_();
		return (size-2);

#else
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
#endif
	}	// ignore current (.) and parent directory entry (..)

	Size Directory::countFiles()
	{
#ifdef BALL_COMPILER_MSVC
		synchronize_();
		Size size =0;
		if(dir_==INVALID_HANDLE_VALUE)
		{
			dir_ = CreateFile(directory_path_.data(),
												FILE_LIST_DIRECTORY,                
												FILE_SHARE_READ|FILE_SHARE_DELETE|FILE_SHARE_WRITE,  
												NULL,                               
												OPEN_EXISTING,                     
												FILE_FLAG_BACKUP_SEMANTICS,        
												NULL                                
												);
			if(dir_==INVALID_HANDLE_VALUE)
			{
				desynchronize_();
				return 0;
			}
		}
		
		String pat=directory_path_+"\\*";
		WIN32_FIND_DATA fd;
		
		if(dirent_!=INVALID_HANDLE_VALUE) FindClose(dirent_);
		dirent_=FindFirstFile(pat.data(),&fd);
		if(dirent_==INVALID_HANDLE_VALUE)
		{
			CloseHandle(dir_);
			dir_=INVALID_HANDLE_VALUE;
			desynchronize_();
			return 0;
		}
		do
		{
			bool isfile=(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)==0;
			if(isfile) ++size;
		}
		while(FindNextFile(dirent_,&fd));
		FindClose(dirent_);
		desynchronize_();
		return size;
#else
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
#endif
	}

	Size Directory::countDirectories()
	{
#ifdef BALL_COMPILER_MSVC
		synchronize_();
		Size size =0;
		if(dir_==INVALID_HANDLE_VALUE)
		{
			dir_ = CreateFile(directory_path_.data(),
												FILE_LIST_DIRECTORY,                
												FILE_SHARE_READ|FILE_SHARE_DELETE|FILE_SHARE_WRITE,  
												NULL,                               
												OPEN_EXISTING,                     
												FILE_FLAG_BACKUP_SEMANTICS,        
												NULL                                
												);
			if(dir_==INVALID_HANDLE_VALUE)
			{
				desynchronize_();
				return 0;
			}
		}
		
		String pat=directory_path_+"\\*";
		WIN32_FIND_DATA fd;
		
		if(dirent_!=INVALID_HANDLE_VALUE) FindClose(dirent_);
		dirent_=FindFirstFile(pat.data(),&fd);
		if(dirent_==INVALID_HANDLE_VALUE)
		{
			CloseHandle(dir_);
			dir_=INVALID_HANDLE_VALUE;
			desynchronize_();
			return 0;
		}
		do
		{
			bool isfile=(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)!=0;
			if(isfile) ++size;
		}
		while(FindNextFile(dirent_,&fd));
		FindClose(dirent_);
		desynchronize_();
		return (size-2);
#else
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
#endif
	}		// ignore current (.) and parent directory entry (..)

	bool Directory::isCurrent() const
	{
		char* buffer_;
		
#ifdef BALL_COMPILER_MSVC
		if ((buffer_ = ::_getcwd(NULL, MAX_PATH_LENGTH)) == 0) return false;
#else
		if ((buffer_ = ::getcwd(NULL, MAX_PATH_LENGTH)) == 0) return false;
#endif
		return (buffer_ == directory_path_);
	}

	bool Directory::has(const String& item) //const
	{	
#ifdef BALL_COMPILER_MSVC
		synchronize_();
		String entry;
		getFirstEntry(entry);
		if(entry==item) return desynchronize_(true);
	while (getNextEntry(entry))
		{
			if (entry == item) return desynchronize_(true);
		}
		return desynchronize_(false);
#else
		synchronize_();
		String entry;
		while (getNextEntry(entry))
		{
			if (entry == item) return desynchronize_(true);
		}
		return desynchronize_(false);
#endif
	}

	bool Directory::find(const String& item, String& filepath)
	{
		
		if (has(item))
		{
			filepath = directory_path_;
			return true; // no sync needed...
		}
		
#ifdef BALL_COMPILER_MSVC
		synchronize_();

		String s;
		Directory directory;
		
		HANDLE dir = CreateFile(FileSystem::CURRENT_DIRECTORY,
												FILE_LIST_DIRECTORY,                
												FILE_SHARE_READ|FILE_SHARE_DELETE|FILE_SHARE_WRITE,  
												NULL,                               
												OPEN_EXISTING,                     
												FILE_FLAG_BACKUP_SEMANTICS,        
												NULL                                
												);
		if(dir == INVALID_HANDLE_VALUE)  return desynchronize_(false);
		HANDLE mydirent;
		WIN32_FIND_DATA fd;
		
		mydirent=FindFirstFile("*",&fd);
		do
		{
			if((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) !=0 &&
				strcmp(fd.cFileName, FileSystem::CURRENT_DIRECTORY) != 0 &&
				strcmp(fd.cFileName, FileSystem::PARENT_DIRECTORY ) != 0)
			{
				directory =Directory(fd.cFileName);
				if (directory.find(item, s))
				{
					filepath = s;
					CloseHandle(dir);
					FindClose(mydirent);
					return desynchronize_(true);
				}
			}
		}
		while(FindNextFile(mydirent,&fd));
		FindClose(mydirent);
		CloseHandle(dir);
		return desynchronize_(false);
		
			
#else
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
#endif
	}

	bool Directory::set(const String& directory_path, bool set_current)
	{
		
#ifdef BALL_COMPILER_MSVC
		CloseHandle(dir_);
		FindClose(dirent_);
		dir_ = dirent_= INVALID_HANDLE_VALUE;
		backup_path_ = "";
		char * buffer_;

		if((directory_path[0] == FileSystem::PATH_SEPARATOR)|| (directory_path[1] == ':' && directory_path[2] == FileSystem::PATH_SEPARATOR))
#else
		dir_ = 0;
		dirent_ = 0;
		backup_path_ = "";
		char* buffer_;
		if (directory_path[0] == FileSystem::PATH_SEPARATOR)		//absolute path
#endif
		{
			directory_path_ = directory_path;
			FileSystem::canonizePath(directory_path_);
			return isValid();
		}
		if ((buffer_ = ::getcwd(NULL, MAX_PATH_LENGTH)) != NULL)
		{
			directory_path_ = buffer_;
			directory_path_ += FileSystem::PATH_SEPARATOR;
			directory_path_ += directory_path;
			FileSystem::canonizePath(directory_path_);
			if (directory_path_.hasSuffix(String(FileSystem::PATH_SEPARATOR)))
			{
				directory_path_.truncate((Size)directory_path_.size() - 1);
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
#ifdef BALL_COMPILER_MSVC
		if (::_chdir("..") != 0) return desynchronize_(false);
		bool result1 = (::_rmdir(directory_path_.data()) == 0);
#else
		if (::chdir("..") != 0) return desynchronize_(false);
		bool result1 = (::rmdir(directory_path_.data()) == 0);
#endif
		bool result2 = false;
		if (backup_path_ != "")
		{
#ifdef BALL_COMPILER_MSVC
			result2 = (::_chdir(backup_path_.data()) != 0);
#else
			result2 = (::chdir(backup_path_.data()) != 0);
#endif
			backup_path_ = "";
		}
#ifdef BALL_COMPILER_MSVC
		dir_ = INVALID_HANDLE_VALUE;
		dirent_ = INVALID_HANDLE_VALUE;
#else
		dir_ = 0;
		dirent_ = 0;
#endif
		directory_path_ = "";
		
		return (result1 && result2);
	}

	bool Directory::renameTo(String new_path)
	{
		synchronize_();
		FileSystem::canonizePath(new_path);
#ifdef BALL_COMPILER_MSVC
		if (::_chdir("..") != 0)	return desynchronize_(false);
		CloseHandle(dir_);
		FindClose(dirent_);
		dir_ = dirent_ = INVALID_HANDLE_VALUE;
#else
		if (::chdir("..") != 0)	return desynchronize_(false);
#endif
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
