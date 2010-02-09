// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: SDFile.C,v 1.12.20.2 2007/05/30 15:17:34 anhi Exp $
//

#include <BALL/FORMAT/SDFile.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/system.h>

namespace BALL 
{

	SDFile::SDFile()
		
		:	MOLFile()
	{
	}

	SDFile::SDFile(const String& name, File::OpenMode open_mode)
		throw(Exception::FileNotFound)
		: MOLFile(name, open_mode),
			read_atoms_(true)
	{
	}

	SDFile::SDFile(const SDFile& file)
		throw(Exception::FileNotFound)
		: MOLFile(file),
			read_atoms_(true)
	{
	}

	SDFile::~SDFile()
		
	{
	}

	void SDFile::disableAtoms() 
	{
		read_atoms_ = false;
	}

	void SDFile::enableAtoms() 
	{
		read_atoms_ = true;
	}

	bool SDFile::write(const System& system)
		throw(File::CannotWrite)
	{
		MoleculeConstIterator molecule = system.beginMolecule();
		for (; +molecule; ++molecule)
		{
			if (!write(*molecule)) 
			{	
				return false;
			}
		}

		return true;
	}
	
	bool SDFile::write(const Molecule& molecule)
		throw(File::CannotWrite)
	{
		if (!MOLFile::write(molecule)) 
		{
			return false;
		}
		writePropertyBlock_(molecule);

		return true;
	}

	Molecule* SDFile::read()
		throw(Exception::ParseError)
	{
		Molecule* molecule = 0;
		// Catch any parse errors. This allows to recover 
		// and continue with the next molecule if something 
		// broken was hidden in the middle of the file.
		try
		{
			// read the molecule (MOLFile = Header + CTAB + props)
			molecule = MOLFile::read();

			// read the property block and assign these
			// properties a s named properties to the molecule
			if (molecule != 0)
			{
				if (!read_atoms_)
				{
					// destroy those atoms and bonds if they are not desired 
					molecule->clear();
				}
				readPropertyBlock_(*molecule);
			}
		}
		catch (Exception::ParseError& e)
		{
			// Read through to the end of the MOLFile (marked by $$$)
			while (readLine() && !getLine().hasPrefix("$$$$")) {};

			// Keep the memory tidy.
			if (molecule != 0)
			{
				delete molecule;
				molecule = 0;
			}

			// Rethrow the exception.
			throw e;
		}

		return molecule;
	}
	
	bool SDFile::read(System& system)
		throw(Exception::ParseError)
	{
		Molecule* molecule = 0;
		bool read_anything = false;
		while ((molecule = read()) != 0)
		{
			// add the molecule to the system
			system.append(*molecule);
			read_anything = true;
		}
		return read_anything;
	}

	void SDFile::readPropertyBlock_(Molecule& molecule)
	{
		// the end of the block is marked by "$$$$"
		while (good() && !startsWith("$$$$"))
		{
			// properties start with "> "
			if (startsWith("> "))
			{
				// we found a new property line: read it and construct 
				// a named property from it
				String property_name = String(getLine().after("<")).before(">");
				readLine();
				molecule.setProperty(property_name, getLine().trim());
			}
			
			// read the next line
			readLine();
		}

		// Happily munch empty lines at the end of a file after the properties block. 
		// This is required because otherwise, an empty line on the end of a file will
		// lead our parser to try and read another MOLFile, which won't succeed.
		Index line_number = getLineNumber();
		int no_chars = 0;
		if (good())
		{
			readLine();
			no_chars += getLine().size()+1; // +1 for new-line character
			while(good() && (getLine().trim() == ""))
			{
				readLine();
				no_chars += getLine().size()+1;
			}
		}

		// if the file is still good, we read too far.
		if (good())
		{
			for(Position i=0; i<no_chars; i++)
			{
				unget();
			}
		}
	}

	void SDFile::writePropertyBlock_(const Molecule& molecule)
	{
		// iterate over all named properties
		for (Position i = 0; i < molecule.countNamedProperties(); i++)
		{
			const NamedProperty& property(molecule.getNamedProperty(i));
			NamedProperty::Type type = property.getType();
			if (type == NamedProperty::INT || type == NamedProperty::FLOAT 
					|| type == NamedProperty::DOUBLE || type == NamedProperty::UNSIGNED_INT
					|| type == NamedProperty::BOOL || type == NamedProperty::STRING)
			{		
				getFileStream() << "> <" << property.getName() << ">" << std::endl;
				switch (type)
				{
					case  NamedProperty::INT:						getFileStream() << property.getInt();					break;
					case  NamedProperty::DOUBLE:				getFileStream() << property.getDouble();			break;
					case  NamedProperty::FLOAT:					getFileStream() << property.getFloat();				break;
					case  NamedProperty::UNSIGNED_INT:	getFileStream() << property.getUnsignedInt(); break;
					case  NamedProperty::BOOL:					getFileStream() << (property.getBool() ? "true" : "false"); break;
					case  NamedProperty::STRING:				getFileStream() << property.getString();			break;
					default:
						getFileStream() << std::endl;
				}
				// add a carriage return and a blank line (as a field separator)
				getFileStream() << std::endl << std::endl;
			}
		}

		// write end marker
		getFileStream() << "$$$$" << std::endl;
	}

	const SDFile& SDFile::operator = (const SDFile& file)
		
	{
		read_atoms_ = file.read_atoms_;
		MOLFile::operator = (file);

		return *this;
	}

	
} // namespace BALL
