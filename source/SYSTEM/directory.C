// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: directory.C,v 1.29.18.1 2007/03/25 22:00:35 oliver Exp $
//

#include <BALL/SYSTEM/directory.h>

#include <cstdio>
#include <cstring>
#include <sys/stat.h>

#ifdef BALL_HAS_SYS_TYPES_H
#	include <sys/types.h>
#endif

#ifdef BALL_HAS_UNISTD_H
#	include <unistd.h>
#endif

#ifdef BALL_OS_WINDOWS
# include <Tchar.h>
#endif

#include <iostream>
#include <cerrno>

#ifdef BALL_COMPILER_MSVC
#	define chdir _chdir
# define rmdir _rmdir
#endif

namespace BALL 
{
	const Size Directory::MAX_PATH_LENGTH = 8192;
	
	Directory::Directory()
		: dir_(INVALID_HANDLE_VALUE),
			dirent_(INVALID_HANDLE_VALUE)
	{
		char* buffer;

		#ifdef BALL_OS_WINDOWS
			if ((buffer = ::_getcwd(NULL, MAX_PATH_LENGTH)) != NULL)	
			{
				directory_path_ = buffer;
				FileSystem::canonizePath(directory_path_);
				dir_ = CreateFile(_T(buffer),
													FILE_LIST_DIRECTORY,                // access (read/write) mode
													FILE_SHARE_READ|FILE_SHARE_DELETE|FILE_SHARE_WRITE,  // share mode
													NULL,                               // security descriptor
													OPEN_EXISTING,                      // how to create
													FILE_FLAG_BACKUP_SEMANTICS,         // file attributes
													NULL                                // file with attributes to copy
													);
				free(buffer);
			}
			else 
			{
				directory_path_ = "";
			}
		#else
			if ((buffer = ::getcwd(NULL, MAX_PATH_LENGTH)) != NULL)	
			{
				directory_path_ = buffer;
				FileSystem::canonizePath(directory_path_);
				free(buffer);
			}
			else 
			{
				directory_path_ = "";
			}
		#endif
	}

	Directory::Directory(const String& directory_path, bool set_current)
		: dir_(INVALID_HANDLE_VALUE),
			dirent_(INVALID_HANDLE_VALUE)
	{
		if (!set(directory_path, set_current)) 
		{
			directory_path_ = "";
		}
		
		#ifdef BALL_OS_WINDOWS
			dir_ = CreateFile(_T(directory_path_.c_str()),
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
		: dir_(INVALID_HANDLE_VALUE),
			dirent_(INVALID_HANDLE_VALUE)
	{
		set(directory);
	}

	Directory::~Directory()
	{
		clear();
	}

	bool Directory::getFirstEntry(String& entry) 
	{
		#ifdef BALL_OS_WINDOWS
			synchronize_();
			if (dir_ != INVALID_HANDLE_VALUE) 
			{	
				CloseHandle(dir_);
			}
			dir_ = CreateFile(_T(directory_path_.c_str()),
												FILE_LIST_DIRECTORY,                // access (read/write) mode
												FILE_SHARE_READ|FILE_SHARE_DELETE|FILE_SHARE_WRITE,  // share mode
												NULL,                               // security descriptor
												OPEN_EXISTING,                      // how to create
												FILE_FLAG_BACKUP_SEMANTICS,         // file attributes
												NULL                                // file with attributes to copy
												);
			if (dir_ == INVALID_HANDLE_VALUE) 
			{
				return desynchronize_(false);
			}
		
			if (dirent_ != INVALID_HANDLE_VALUE) 
			{
				FindClose(dirent_);
			}
		
			String pat = directory_path_+"/*";
			WIN32_FIND_DATA fd;
		
	
			dirent_ = FindFirstFile(_T(pat.c_str()),&fd);
			if (dirent_ == INVALID_HANDLE_VALUE)
			{
				CloseHandle(dir_);
				dir_ = INVALID_HANDLE_VALUE;
				return desynchronize_(false);
			}
			else
			{
				entry = _T(fd.cFileName);
			
				return desynchronize_(true);
			}

	#else
		synchronize_();
		if (dir_ != 0) 
		{
			::closedir(dir_);
		}
		dir_ = ::opendir(directory_path_.c_str());
		if (dir_ == 0) 
		{
			return desynchronize_(false);	
		}
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
		
		if (dir_ == INVALID_HANDLE_VALUE)
		{
			
			dir_ = CreateFile(directory_path_.c_str(),
												FILE_LIST_DIRECTORY,                
												FILE_SHARE_READ|FILE_SHARE_DELETE|FILE_SHARE_WRITE,  
												NULL,                               
												OPEN_EXISTING,                     
												FILE_FLAG_BACKUP_SEMANTICS,        
												NULL                                
												);
			
			if (dir_ == INVALID_HANDLE_VALUE) return desynchronize_(false);
			if (dirent_ != INVALID_HANDLE_VALUE) FindClose(dirent_);

			String pat=directory_path_ + "/*";
			WIN32_FIND_DATA fd;
			
			dirent_=FindFirstFile(pat.c_str(),&fd);
			
			if (dirent_==INVALID_HANDLE_VALUE)
			{
				CloseHandle(dir_);
				dir_ = INVALID_HANDLE_VALUE;
				return desynchronize_(false);
			}
		}
		if (dir_ == INVALID_HANDLE_VALUE) return desynchronize_(false);
		
		WIN32_FIND_DATA fd;
		
		if (dirent_ == INVALID_HANDLE_VALUE)
		{
			// someone has forgot to call FirstEntry

			String pat=directory_path_ + "/*";
			dirent_=FindFirstFile(pat.c_str(),&fd);
			
			if (dirent_==INVALID_HANDLE_VALUE)
			{
				CloseHandle(dir_);
				dir_ = INVALID_HANDLE_VALUE;
				return desynchronize_(false);
			}
			else
				entry = fd.cFileName;
		}
		else
		{
			if (FindNextFile(dirent_,&fd)==0) return desynchronize_(false);
			entry=fd.cFileName;
		}
		return desynchronize_(true);

#else
		synchronize_();
		if (dir_ == 0) 
		{
			dir_ = ::opendir(directory_path_.c_str());
		}
		if (dir_ == 0) 
		{
			return desynchronize_(false);	
		}

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

		if (dir_ == INVALID_HANDLE_VALUE)
		{
			dir_ = CreateFile(directory_path_.c_str(),
												FILE_LIST_DIRECTORY,                
												FILE_SHARE_READ|FILE_SHARE_DELETE|FILE_SHARE_WRITE,  
												NULL,                               
												OPEN_EXISTING,                     
												FILE_FLAG_BACKUP_SEMANTICS,        
												NULL                                
												);
			if (dir_ == INVALID_HANDLE_VALUE)
			{
				desynchronize_();
				return 0;
			}
		}
		
		if (dirent_ != INVALID_HANDLE_VALUE) 
		{
			FindClose(dirent_);
		}
		String pat = directory_path_ + "/*";
		WIN32_FIND_DATA fd;
		dirent_ = FindFirstFile(pat.c_str(),&fd);
		if (dirent_ == INVALID_HANDLE_VALUE)
		{
			if (dir_ != INVALID_HANDLE_VALUE)
			{
				CloseHandle(dir_);
				dir_ = INVALID_HANDLE_VALUE;
			}
			desynchronize_();
			return 0;
		}
		Size size = 1;
		while(FindNextFile(dirent_,&fd) != 0) ++size;
		FindClose(dirent_);
		desynchronize_();
		return (size - 2);

#else
		synchronize_();
		Size size = 0;
		DIR* dir = ::opendir(directory_path_.c_str());
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
		if (dir_ == INVALID_HANDLE_VALUE)
		{
			dir_ = CreateFile(directory_path_.c_str(),
												FILE_LIST_DIRECTORY,                
												FILE_SHARE_READ|FILE_SHARE_DELETE|FILE_SHARE_WRITE,  
												NULL,                               
												OPEN_EXISTING,                     
												FILE_FLAG_BACKUP_SEMANTICS,        
												NULL                                
												);
			if (dir_ == INVALID_HANDLE_VALUE)
			{
				desynchronize_();
				return 0;
			}
		}
		
		String pat=directory_path_+"/*";
		WIN32_FIND_DATA fd;
		
		if (dirent_!=INVALID_HANDLE_VALUE) FindClose(dirent_);
		dirent_=FindFirstFile(pat.c_str(),&fd);
		if (dirent_==INVALID_HANDLE_VALUE)
		{
			CloseHandle(dir_);
			dir_ = INVALID_HANDLE_VALUE;
			desynchronize_();
			return 0;
		}
		do
		{
			bool isfile=(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)==0;
			if (isfile) ++size;
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
		DIR* dir = ::opendir(directory_path_.c_str());
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
		Size size = 0;
		if (dir_ == INVALID_HANDLE_VALUE)
		{
			dir_ = CreateFile(directory_path_.c_str(),
												FILE_LIST_DIRECTORY,                
												FILE_SHARE_READ|FILE_SHARE_DELETE|FILE_SHARE_WRITE,  
												NULL,                               
												OPEN_EXISTING,                     
												FILE_FLAG_BACKUP_SEMANTICS,        
												NULL                                
												);
			if (dir_ == INVALID_HANDLE_VALUE)
			{
				desynchronize_();
				return 0;
			}
		}
		
		String pat=directory_path_+"/*";
		WIN32_FIND_DATA fd;
		
		if (dirent_!=INVALID_HANDLE_VALUE) FindClose(dirent_);
		dirent_=FindFirstFile(pat.c_str(),&fd);
		if (dirent_==INVALID_HANDLE_VALUE)
		{
			CloseHandle(dir_);
			dir_ = INVALID_HANDLE_VALUE;
			desynchronize_();
			return 0;
		}
		do
		{
			bool isfile=(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)!=0;
			if (isfile) ++size;
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
		DIR* dir = ::opendir(directory_path_.c_str());
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
		char* buffer;
		
#ifdef BALL_COMPILER_MSVC
		if ((buffer = ::_getcwd(NULL, MAX_PATH_LENGTH)) == 0) return false;
#else
		if ((buffer = ::getcwd(NULL, MAX_PATH_LENGTH)) == 0) return false;
#endif
		String s(buffer);
		FileSystem::canonizePath(s);
		bool result = (s == directory_path_);
		free(buffer);
		return result;
	}

	bool Directory::has(const String& item) //const
	{	
		synchronize_();

		String entry;

		getFirstEntry(entry);
		if (entry==item) return desynchronize_(true);

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
		if (dir == INVALID_HANDLE_VALUE)  return desynchronize_(false);
		HANDLE mydirent;
		WIN32_FIND_DATA fd;
		
		mydirent=FindFirstFile("*",&fd);
		do
		{
			if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) !=0 &&
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
		if (dir == 0)	
		{
			return desynchronize_(false);
		}

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
		backup_path_ = "";
		char* buffer;

		clear();
#ifdef BALL_COMPILER_MSVC
		if (   (directory_path.size() > 0) 
			&& ((directory_path[0] == FileSystem::PATH_SEPARATOR) || 
			    (directory_path[0] == '/') ||
				(directory_path[1] == ':' && 
				 (  (directory_path[2] == FileSystem::PATH_SEPARATOR)
				  ||(directory_path[2] == '/')))))
#else
		dir_ = 0;
		dirent_ = 0;
		if (  (directory_path.size() > 0)
			&&(directory_path[0] == FileSystem::PATH_SEPARATOR))		//absolute path
#endif
		{
			directory_path_ = directory_path;
			FileSystem::canonizePath(directory_path_);
			return isValid();
		}
#ifdef BALL_OS_WINDOWS
		if ((buffer = ::_getcwd(NULL, MAX_PATH_LENGTH)) != NULL)
#else
		if ((buffer = ::getcwd(NULL, MAX_PATH_LENGTH)) != NULL)
#endif
		{
			directory_path_ = buffer;
			free(buffer);
			directory_path_ += "/";
			directory_path_ += directory_path;
			FileSystem::canonizePath(directory_path_);
			if (directory_path_.hasSuffix("/"))
			{
				directory_path_.truncate((Size)directory_path_.size() - 1);
			}
		}
		else 
		{	
			directory_path_ = "";
			return false;
		}
		if (!isValid())	
		{
			return false;
		}
		if (set_current) 
		{
			return (::chdir(directory_path_.c_str()) == 0);
		}

		return true;
	}

	bool Directory::remove()
	{
		synchronize_();	
		if (::chdir("..") != 0)
		{
			return desynchronize_(false);
		}
		bool result1 = (::rmdir(directory_path_.c_str()) == 0);
		bool result2 = true;
		if (backup_path_ != "")
		{
			result2 = (::chdir(backup_path_.c_str()) == 0);
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
		if (::chdir("..") != 0)	return desynchronize_(false);
#ifdef BALL_COMPILER_MSVC
		CloseHandle(dir_);
		FindClose(dirent_);
		dir_ = dirent_ = INVALID_HANDLE_VALUE;
#endif
		if (::rename(directory_path_.c_str(), new_path.c_str()) == 0)
		{
			directory_path_ = new_path;
			FileSystem::canonizePath(directory_path_);
			return desynchronize_(true);
		}
		return desynchronize_(false);
	}

	bool Directory::isValid() const
	{
		#ifdef BALL_COMPILER_MSVC
			HANDLE dir = CreateFile(const_cast<char*>(directory_path_.c_str()),
							FILE_LIST_DIRECTORY,                
							FILE_SHARE_READ|FILE_SHARE_DELETE,  
							NULL,                               
							OPEN_EXISTING,                     
							FILE_FLAG_BACKUP_SEMANTICS,        
							NULL                                
							);
			if (dir == INVALID_HANDLE_VALUE)
			{
				return false;
			}
			else 
			{
				CloseHandle(dir);
				return true;
			}
		#else
			if (directory_path_ == "")
			{
				return false;
			}
			DIR* dir = ::opendir(directory_path_.c_str());
			if (dir == 0)
			{
				return false;
			}
			else
			{
				::closedir(dir);
				return true;
			}
		#endif
	}

	String Directory::getUserHomeDir()
	{
		String homedir;

		// default for UNIX/LINUX
		char* home = getenv("HOME");
		if (home != 0) 
		{
			homedir = String(home);
		}
		else
		{
			// windows
			char* home_drive = getenv("HOMEDRIVE");
			char* home_path = getenv("HOMEPATH");
			if (home_drive != 0 && home_path != 0)
			{
				homedir = String(home_drive) + "\\" + home_path;
			}

			FileSystem::canonizePath(homedir);
		}

		if (homedir == "")
		{
			return "";
		}

		// backup current position
		Directory current;
		
		// changedir to the homedir
		Directory dir(homedir);
		if (!dir.setCurrent()) return "";

		current.setCurrent();

		return homedir;
	}


	bool Directory::changeToUserHomeDir()
	{
		String home = getUserHomeDir();
		if (home == "") return false;
		Directory d(home);
 		return d.setCurrent();
	}

#ifdef BALL_NO_INLINE_FUNCTIONS
# include <BALL/SYSTEM/directory.iC>
#endif

} // namespace BALL 
