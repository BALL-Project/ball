// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: genericMolFile.C,v 1.12 2004/03/07 22:36:14 oliver Exp $
//

#include <BALL/FORMAT/genericMolFile.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/molecule.h>

namespace BALL 
{
	GenericMolFile::GenericMolFile()
		
		:	LineBasedFile()
	{
	}

	GenericMolFile::GenericMolFile(const String& filename, File::OpenMode open_mode)
		throw(Exception::FileNotFound)
		:	LineBasedFile(filename, open_mode)
	{
	}

	GenericMolFile::GenericMolFile(const GenericMolFile& file)
		throw(Exception::FileNotFound)
		:	LineBasedFile()
	{
    open(file.name_, file.open_mode_);
	}
 
	GenericMolFile::~GenericMolFile()
		
	{
	}

	const GenericMolFile& GenericMolFile::operator = (const GenericMolFile& rhs)
		throw(Exception::FileNotFound)
	{
		LineBasedFile::operator = (rhs);
		return *this;
	}

	bool GenericMolFile::read(System& system)
		throw(Exception::ParseError)
	{
		if (!isOpen())
		{
			return false;
		}

		initRead_();

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

	bool GenericMolFile::write(const Molecule& /* molecule */)
		throw(File::CannotWrite)
	{
		if (!isOpen() || getOpenMode() != std::ios::out)
		{
			throw (File::CannotWrite(__FILE__, __LINE__, name_));
		}
		return true;
	}

	bool GenericMolFile::write(const System& system)
		throw(File::CannotWrite)
	{
		if (!isOpen() || getOpenMode() != std::ios::out)
		{
			throw (File::CannotWrite(__FILE__, __LINE__, name_));
		}

		initWrite_();
		MoleculeConstIterator molecule = system.beginMolecule();
		for (; +molecule; ++molecule)
		{
			if (!write(*molecule)) return false;
		}
		
		return true;
	}

	GenericMolFile& GenericMolFile::operator >> (System& system)
		throw(Exception::ParseError)
	{
		read(system);
		return *this;
	}
 
	GenericMolFile& GenericMolFile::operator << (const System& system)
		throw(File::CannotWrite)
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
			molecule = *new_mol; // copy the Molecule
		}
		
		delete new_mol;
		return *this;
	}
 
	GenericMolFile& GenericMolFile::operator << (const Molecule& molecule)
		throw(File::CannotWrite)
	{
		write(molecule);
		return *this;
	}

	void GenericMolFile::initRead_()
	{
	}

	void GenericMolFile::initWrite_()
	{
	}

} // namespace BALL
