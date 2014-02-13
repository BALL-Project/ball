// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
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

		// If the path starts with a '\\' or '//', assume it's in UNC and return.
		if (path.hasPrefix("\\\\") || path.hasPrefix("//"))
			return;

#ifdef BALL_COMPILER_MSVC
		// TODO: we need much, much better unc handling
		if (path.hasPrefix("//"))
			return;

		// replace "\" by "/"
		for (index = 0; index != INVALID_INDEX; index = path.substitute("\\", "/")) { }
#endif

		// replace all double occurences of a / with a single /
		String s("//");
		for (index = 0; index != INVALID_INDEX; index = path.substitute(s, "/")) { }
		
		// expand ~ to the user's home directory
		FileSystem::expandTilde_(path);

		// replace occurences of "/./" with "/"
		s = "/";
		s += FileSystem::CURRENT_DIRECTORY;
		s += "/";
		for (index = 0; index != INVALID_INDEX;
				 index = path.substitute(s, "/")) { }

		// remove a leading "./"
		s.set(FileSystem::CURRENT_DIRECTORY);
		s += "/";
		while (path.hasPrefix(s) == true)
		{
			path.erase(0, s.size());	
		}

		// remove trailing "/."
		s.set("/");
		s += FileSystem::CURRENT_DIRECTORY;
		while (path.hasSuffix(s) == true)
		{
			path.resize(path.size() - s.size());
		}

		// remove intermediate reversals of path 
		// (something like "/usr/local/../bin" 
		//  would be reduced to "/usr/bin")
		s = "[^";
		s += "/";
		s += FileSystem::CURRENT_DIRECTORY;
		s += "][^";
		s += "/";
		s += FileSystem::CURRENT_DIRECTORY;
		s += "]*";
		s += "/";
		s += REGEXP_CONFORM_PARENT_DIRECTORY;
		s += "/";

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
		
		Index index = (Index)path.find_first_not_of("/", 1);
		
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
		Index index = (Index)path.find_first_not_of("/", 1);
		
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
#		ifdef BALL_OS_WINDOWS
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

	String FileSystem::fileExtension(const String& filename)
	{
		String base_name = baseName(filename);
		Position idx = (Position)base_name.find_last_of(".");

		if (idx != String::EndPos && (idx + 1) < base_name.size())
		{
			return base_name(idx + 1);
		}

		return "";
	}

	String FileSystem::path(const String& filename)
	{
		Position idx = (Position)filename.find_last_of(PATH_SEPARATOR);
#		ifdef BALL_OS_WINDOWS
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
			return filename(0, idx + 1);
		}
		else
		{
			return "";
		}
	}

} // namespace BALL
