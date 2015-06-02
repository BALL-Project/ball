// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
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
		: MOLFile(name, open_mode),
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
	{
		if (!MOLFile::write(molecule)) 
		{
			return false;
		}
		writePropertyBlock_(molecule);

		return true;
	}

	Molecule* SDFile::read()
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

	Size SDFile::countMolecules()
	{
		Size n_molecules = 0;
		Molecule* mol = NULL;

		while ( (mol = read()) )
		{
			++n_molecules;

			delete mol;
			mol = NULL;
		}

		reopen();

		return n_molecules;
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
		if (good())
		{
			streampos current_pos = tellg();
			String current_line = getLine();
			Position current_line_number = line_number_;

			readLine();
			while(good() && (getLine().trim() == ""))
			{
				readLine();
			}

			// If the file is still good we have read too far.
			// Thus, we reset the LineBasedFile to the stored position
			if (good())
			{
				seekg(current_pos);
				line_ = current_line;
				line_number_ = current_line_number;
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
