// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: amiraMeshFile.C
//

#include <BALL/FORMAT/amiraMeshFile.h>
#include <cstdio>


namespace BALL
{
	AmiraMeshFile::AmiraMeshFile()
		: File(),
			binary_(false)
	{
	}
	
	AmiraMeshFile::AmiraMeshFile(const String& name, File::OpenMode open_mode)
		: File(name, open_mode),
			binary_(false)
	{
	}
	
	AmiraMeshFile::~AmiraMeshFile()
	{
		close();
		clear();
	}
	
	void AmiraMeshFile::clear()
	{
		File::clear();
		binary_= false;
	}
	
	bool AmiraMeshFile::operator == (const AmiraMeshFile& file) const
	{
		return (   (File::operator == (file))
						 &&(binary_    == file.binary_));
	}
	
	bool AmiraMeshFile::open(const String& name, File::OpenMode open_mode)
	{
		return true;
	}
	
	bool AmiraMeshFile::readHeader()
	{
		return true;
	}
	
	bool AmiraMeshFile::read(RegularData3D& density_map)
	{
		return true;
	}
	
	bool AmiraMeshFile::writeHeader()
	{
		// construct a correct header array and write it.
		// TODO: implement
		return false;
	}

	bool AmiraMeshFile::write(RegularData3D& /*map*/)
	{
		// Write the content of a RegularData3D dataset to a CCP4 file.
		// TODO: implement
		return false;
	}
	
	const char* AmiraMeshFile::findAndJump_(const char* buffer, const char* search_string)
	{
    const char* found_loc = strstr(buffer, search_string);
    if (found_loc) return found_loc + strlen(search_string);
    return buffer;
	}
} // namespace BALL
