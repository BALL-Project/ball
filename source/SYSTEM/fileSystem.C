// $Id: fileSystem.C,v 1.8 2001/07/31 00:49:39 oliver Exp $

#include <BALL/SYSTEM/fileSystem.h>

#include <BALL/DATATYPE/regExp.h>
#include <pwd.h> // 'getpwnam'

namespace BALL 
{

	const char FileSystem::PATH_SEPARATOR = '/';
	const char* const FileSystem::CURRENT_DIRECTORY = ".";
	const char* const FileSystem::PARENT_DIRECTORY = "..";
	// must be adapted in case of porting to other platforms than UNIX
	static const char* const REGEXP_CONFORM_PARENT_DIRECTORY = "\\.\\.";
	static const char* const REGEXP_CONFORM_CURRENT_DIRECTORY = "\\.";

	void FileSystem::canonizePath(String& path)
		throw()
	{
		if (path == "")
		{
			return;
		}

		Index index = 0;

		// replace all double occurences of a PATH_SEPARATOR with a single PATH_SEPARATOR
		String s(FileSystem::PATH_SEPARATOR);
		s += FileSystem::PATH_SEPARATOR;
		for (; index != INVALID_INDEX; index = path.substitute(s, FileSystem::PATH_SEPARATOR));
		
		// expand ~ to the user's home directory
		FileSystem::expandTilde_(path);

		// replace occurences of "/./" with "/"
		s = FileSystem::PATH_SEPARATOR;
		s += FileSystem::CURRENT_DIRECTORY;
		s += FileSystem::PATH_SEPARATOR;
		for (index = 0; index != INVALID_INDEX;
				 index = path.substitute(s, FileSystem::PATH_SEPARATOR));

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
		RegularExpression reg_exp(s);
		Substring sub;
		while (reg_exp.find(path, sub) == true)
		{
			sub.destroy();
		}
	}

	void FileSystem::expandTilde_(String& path)
		throw()
	{
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
			const char* user_name = ::getenv("USER");

			if (user_name != 0)
			{
				Size size = 0;
				size = (Size)strlen(user_name);
				path.insert(1, user_name);
				index += (Index)(size - 1);
			}
		}
		else if (index == INVALID_INDEX)
		{
			index = (Index)path.size() - 1;
		}

		char c = path[index];
		path[index] = 0;
		struct passwd *passwd = ::getpwnam(&path[1]);
		if(passwd == 0)
		{ 
			return;                                 
		}
		path[index] = c;
		
		String buffer(passwd->pw_dir);
		buffer.append(path.c_str() + index);
		buffer.swap(path);
		
		return;
	}

} // namespace BALL
