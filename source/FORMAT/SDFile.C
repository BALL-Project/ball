// $Id: SDFile.C,v 1.1 2001/12/17 11:27:14 oliver Exp $

#include <BALL/FORMAT/SDFile.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/system.h>

namespace BALL 
{

	SDFile::SDFile()
		throw()
		:	MOLFile()
	{
	}

	SDFile::SDFile(const String& name, File::OpenMode open_mode)
		throw(Exception::FileNotFound)
		: MOLFile(name, open_mode)
	{
	}

	SDFile::SDFile(const SDFile& file)
		throw(Exception::FileNotFound)
		: MOLFile(file)
	{
	}

	SDFile::~SDFile()
		throw()
	{
	}

	void SDFile::write(const System& system)
	{
	}

	void SDFile::read(System& system)
		throw(Exception::ParseError)
	{
		// read the header block: first line == name, third line = comment, second line ignored
		bool ok = true;
		ok &= readLine();
		String name = getLine();
		ok &= readLine();
		ok &= readLine();
		String comment = getLine();
		if (!ok)
		{
			throw Exception::ParseError(__FILE__, __LINE__, getLine(), "Unable to read header block!");
		}

		vector<Atom*> atom_map;
		Molecule* molecule = readCTAB(atom_map);
		
		// add the molecule to the system
		system.append(*molecule);
	}


} // namespace BALL
