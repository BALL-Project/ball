// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: fileSystem.C,v 1.20 2004/11/07 14:44:15 oliver Exp $
//

#include <BALL/SYSTEM/fileSystem.h>

#include <BALL/DATATYPE/regularExpression.h>
#ifdef BALL_HAS_PWD_H
#include <pwd.h> // 'getpwnam'
#endif

namespace BALL 
{
#ifdef BALL_COMPILER_MSVC
	const char FileSystem::PATH_SEPARATOR='\\';
#else
	const char FileSystem::PATH_SEPARATOR = '/';
#endif
	const char* const FileSystem::CURRENT_DIRECTORY = ".";
	const char* const FileSystem::PARENT_DIRECTORY = "..";
	// must be adapted in case of porting to other platforms than UNIX

	static const char* const REGEXP_CONFORM_PARENT_DIRECTORY = "\\.\\.";

	void FileSystem::canonizePath(String& path)
	{
		if (path == "")
		{
			return;
		}

		Index index = 0;

		// replace all double occurences of a PATH_SEPARATOR with a single PATH_SEPARATOR
		String s(FileSystem::PATH_SEPARATOR);
		s += FileSystem::PATH_SEPARATOR;
		for (; index != INVALID_INDEX; index = path.substitute(s, FileSystem::PATH_SEPARATOR)) { }
		
		// expand ~ to the user's home directory
		FileSystem::expandTilde_(path);

		// replace occurences of "/./" with "/"
		s = FileSystem::PATH_SEPARATOR;
		s += FileSystem::CURRENT_DIRECTORY;
		s += FileSystem::PATH_SEPARATOR;
		for (index = 0; index != INVALID_INDEX;
				 index = path.substitute(s, FileSystem::PATH_SEPARATOR)) { }

		// remove a leading "./"
		s.set(FileSystem::CURRENT_DIRECTORY);
		s += FileSystem::PATH_SEPARATOR;
		while (path.hasPrefix(s) == true)
		{
			path.erase(0, s.size());	
		}

		// remove trailing "/."
		s.set(FileSystem::PATH_SEPARATOR);
		s += FileSystem::CURRENT_DIRECTORY;
		while (path.hasSuffix(s) == true)
		{
			path.resize(path.size() - s.size());
		}

		// remove intermediate reversals of path 
		// (something like "/usr/local/../bin" 
		//  would be reduced to "/usr/bin")
#ifdef BALL_COMPILER_MSVC
		s = "[^";
		s += FileSystem::PATH_SEPARATOR;
		s += FileSystem::PATH_SEPARATOR;
		s += FileSystem::CURRENT_DIRECTORY;
		s += "][^";
		s += FileSystem::PATH_SEPARATOR;
		s += FileSystem::PATH_SEPARATOR;
		s += FileSystem::CURRENT_DIRECTORY;
		s += "]*";
		s += FileSystem::PATH_SEPARATOR;
		s += FileSystem::PATH_SEPARATOR;
		s += REGEXP_CONFORM_PARENT_DIRECTORY;
		s += FileSystem::PATH_SEPARATOR;
		s += FileSystem::PATH_SEPARATOR;
#else
		s = "[^";
		s += FileSystem::PATH_SEPARATOR;
		s += FileSystem::CURRENT_DIRECTORY;
		s += "][^";
		s += FileSystem::PATH_SEPARATOR;
		s += FileSystem::CURRENT_DIRECTORY;
		s += "]*";
		s += FileSystem::PATH_SEPARATOR;
		s += REGEXP_CONFORM_PARENT_DIRECTORY;
		s += FileSystem::PATH_SEPARATOR;
#endif
		RegularExpression reg_exp(s);
		Substring sub;
		while (reg_exp.find(path, sub) == true)
		{
			sub.destroy();
		}
	}

	void FileSystem::expandTilde_(String& path)
	{


		#ifndef BALL_COMPILER_MSVC
		if (path.isEmpty() == true)
		{
			return;
		}
		
		if (path[0] != '~')
		{ 
			return;                
		}
		
		Index index = (Index)path.find_first_not_of(FileSystem::PATH_SEPARATOR, 1);
		
		if (index == 2)
		{
			// check for the user's name
			const char* user_name = ::getenv("USER");

			if (user_name != 0)
			{
				Size size = 0;
				size = (Size)strlen(user_name);
				path.insert(1, user_name);
				index += (Index)(size - 1);
			}
			else
			{
				// check for the user's home directory
				const char* user_dir = ::getenv("HOME");

				if (user_dir == 0)
				{
					Log.warn() << "FileSystem::expandTilde: unable to expand '~' to"
					              "the user's home directory -- please set $USER or"
												"$HOME in your environment!" << std::endl;
				}
				else
				{
					// replace the '~' by the user's home dir (from $HOME)
					path.replace(0, 1, user_dir);
				}
			}
		}
		else if (index == INVALID_INDEX)
		{
			index = (Index)path.size() - 1;
		}

		char c = path[index];
		path[index] = 0;
		struct passwd* passwd_ptr = ::getpwnam(&path[1]);
		if (passwd_ptr == 0)
		{ 
			return;                                 
		}
		path[index] = c;
		
		String buffer(passwd_ptr->pw_dir);
		buffer.append(path.c_str() + index);
		buffer.swap(path);
		endpwent();

#else // Windows implementation
		Index index = (Index)path.find_first_not_of(FileSystem::PATH_SEPARATOR, 1);
		
		if (index == 2)
		{
			const char* user_dir = ::getenv("USERPROFILE");

			if (user_dir == 0)
			{
				Log.warn() << "FileSystem::expandTilde: unable to expand '~' to"
					              "the user's home directory -- please set $USER or"
												"$HOME in your environment!" << std::endl;
			}
			else
			{
				// replace the '~' by the user's home dir (from $HOME)
				path.replace(0, 1, user_dir);
			}
		}

#endif

		return;
	}

	String FileSystem::baseName(const String& filename)
	{
		Position idx = (Position)filename.find_last_of(PATH_SEPARATOR);
#		ifdef BALL_PLATFORM_WINDOWS
		// this is an ugly hack to force similar behaviour of the
		// TransformationManager for Windows and Unix
		// this means that "/" and "\" are treated the same
		Position idx2 = (Position)filename.find_last_of('/');
		idx  = (idx  == String::EndPos) ? idx2 : idx;
		idx2 = (idx2 == String::EndPos) ?  idx : idx2;
		idx  = std::max(idx, idx2);
#		endif
		if (idx != String::EndPos)
		{
			if ((idx + 1)< filename.size())
			{
				return filename(idx + 1);
			}
			else
			{
				return "";
			}
		}
		else
		{
			return filename;
		}
	}

	String FileSystem::path(const String& filename)
	{
		Position idx = (Position)filename.find_last_of(PATH_SEPARATOR);
		if (idx != String::EndPos)
		{
			return filename(0, idx + 1);
		}
		else
		{
			return "";
		}
	}

} // namespace BALL
