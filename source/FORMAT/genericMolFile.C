// $Id: genericMolFile.C,v 1.1 2001/12/17 11:32:47 oliver Exp $

#include <BALL/FORMAT/genericMolFile.h>

namespace BALL 
{
	GenericMolFile::GenericMolFile()
		throw()
		:	LineBasedFile()
	{
	}

	GenericMolFile::GenericMolFile(const String& filename, File::OpenMode open_mode)
		throw(Exception::FileNotFound)
		:	LineBasedFile(filename, open_mode)
	{
	}

	GenericMolFile::GenericMolFile(const GenericMolFile& file)
		:	LineBasedFile(file)
	{
	}

	GenericMolFile::~GenericMolFile()
		throw()
	{
	}

	void GenericMolFile::read(System& system)
		throw(Exception::ParseError)
	{
	}

	void GenericMolFile::write(const System& system)
	{
	}

	GenericMolFile& GenericMolFile::operator >> (System& system)
		throw(Exception::ParseError)
	{
		read(system);
		return *this;
	}
 
	GenericMolFile& GenericMolFile::operator << (const System& system)
	{
		write(system);
	}

} // namespace BALL
