// $Id: genericMolFile.C,v 1.4 2001/12/26 03:35:52 oliver Exp $

#include <BALL/FORMAT/genericMolFile.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/molecule.h>

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

	bool GenericMolFile::read(System& system)
		throw(Exception::ParseError)
	{
		if (!isOpen())
		{
			return false;
		}

		initRead();

		bool read_anything = false;
		Molecule* molecule = 0;
		while ((molecule = read()) != 0)
		{
			system.append(*molecule);
			read_anything = true;
		}

		return read_anything;
	}

	Molecule* GenericMolFile::read()
		throw(Exception::ParseError)
	{
		return 0;
	}

	void GenericMolFile::write(const Molecule& /* molecule */)
	{
	}

	void GenericMolFile::write(const System& system)
	{
		if (!isOpen())
		{
			return;
		}

		initWrite();
		MoleculeConstIterator molecule = system.beginMolecule();
		for (; +molecule; ++molecule)
		{
			write(*molecule);
		}
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
		return *this;
	}

	GenericMolFile& GenericMolFile::operator >> (Molecule& molecule)
		throw(Exception::ParseError)
	{
		molecule.clear();
		Molecule* new_mol = read();
		if (new_mol != 0)
		{
			molecule = *new_mol;
		}
		return *this;
	}
 
	GenericMolFile& GenericMolFile::operator << (const Molecule& molecule)
	{
		write(molecule);
		return *this;
	}

	void GenericMolFile::initRead()
	{
	}

	void GenericMolFile::initWrite()
	{
	}

} // namespace BALL
