// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: genericMolFile.C,v 1.8 2003/07/11 15:27:44 amoll Exp $

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
		throw(File::CanNotWrite)
	{
		if (!isOpen() || getOpenMode() != File::OUT)
		{
			throw (File::CanNotWrite(__FILE__, __LINE__, name_));
		}
		return true;
	}

	bool GenericMolFile::write(const System& system)
		throw(File::CanNotWrite)
	{
		if (!isOpen() || getOpenMode() != File::OUT)
		{
			throw (File::CanNotWrite(__FILE__, __LINE__, name_));
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
		throw(File::CanNotWrite)
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
		throw(File::CanNotWrite)
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
