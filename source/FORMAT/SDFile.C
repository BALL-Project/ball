// $Id: SDFile.C,v 1.2 2001/12/18 01:14:46 oliver Exp $

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
		MoleculeConstIterator molecule = system.beginMolecule();
		for (; +molecule; ++molecule)
		{
			write(*molecule);
		}
	}
	
	void SDFile::write(const Molecule& molecule)
	{
		MOLFile::write(molecule);
		writePropertyBlock_(molecule);
	}

	void SDFile::read(Molecule& molecule)
		throw(Exception::ParseError)
	{
		// read the molecule (MOLFile = Header + CTAB + props)
		MOLFile::read(molecule);

		// read the property block and assign these
		// properties a s named properties to the molecule
		readPropertyBlock_(molecule);
	}
	
	void SDFile::read(System& system)
		throw(Exception::ParseError)
	{
		Molecule* molecule = 0;
		try 
		{
			while (good())
			{
				molecule = new Molecule;
				read(*molecule);
			
				// add the molecule to the system
				system.append(*molecule);
			}
		}
		catch (Exception::ParseError& e)
		{
			delete molecule;
			throw e;
		}
	}

	void SDFile::readPropertyBlock_(Molecule& molecule)
	{
		// the end of the block is marked by "$$$$"
		while (readLine() && good() && !startsWith("$$$$"))
		{
			if (startsWith("> "))
			{
				// we found a new property line: read it and construct 
				// a named property from it
				String property_name = String(getLine().after("<")).before(">");
				readLine();
				molecule.setProperty(property_name, getLine().trim());
			}
		}
	}

	void SDFile::writePropertyBlock_(const Molecule& molecule)
	{
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
				getFileStream() << std::endl;
			}
		}

		// write end marker
		getFileStream() << "$$$$" << std::endl;
	}
	
} // namespace BALL
