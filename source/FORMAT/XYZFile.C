// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: XYZFile.C,v 1.7.10.3 2006/10/15 15:31:43 amoll Exp $
//

#include <BALL/FORMAT/XYZFile.h>
#include <BALL/DATATYPE/string.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/PTE.h>

using namespace std;

namespace BALL 
{

	XYZFile::XYZFile()
	{
	}

	XYZFile::XYZFile(const String& name, File::OpenMode open_mode)
		throw(Exception::FileNotFound)
		: File(name, open_mode)
	{
	}

	XYZFile::XYZFile(const XYZFile& file)
		throw(Exception::FileNotFound)
		: File(file)
	{
	}

	XYZFile::~XYZFile()
		throw()
	{
	}
	
	bool XYZFile::write(const System& system)
		throw(File::CannotWrite)
	{
		if (!isOpen() || getOpenMode() != std::ios::out)
		{
			throw (File::CannotWrite(__FILE__, __LINE__, name_));
		}

		// write the first and the second line:
		//  - the number of atoms
		//  - a comment line
		(File&)(*this) << (Size)system.countAtoms() << endl
									 << system.getName() << endl;
		
		// write all atoms
		AtomConstIterator it = system.beginAtom();
		for (; +it; ++it)
		{
			// write the atom symbol and the coordinates
			(File&)(*this) << it->getElement().getSymbol() << " "
										 << it->getPosition().x << " "
										 << it->getPosition().y << " "
									   << it->getPosition().z << endl;
		}

		return true;
	}

	bool XYZFile::read(System& system)
	{
		// remove old rubbish from the system
		system.destroy();

		// remember the line number for error messages
		Size number_of_lines = 0;
		String line;

		// prepare a buffer 
		const Size BUF_SIZE = 1024; 
		char buffer[BUF_SIZE];
		Size number_of_atoms = 0;

		comment_.clear();

		// read the number of atoms (first line)
		if (getline(buffer, BUF_SIZE))
		{
			line.set(buffer);
			line.trim();
			number_of_lines++;
			if (line.countFields() >= 1)
			{
				// retrieve the number of atoms
				number_of_atoms = line.getField(0).toUnsignedInt();
			}
			else
			{
				// if the first line cannot be read correctly, abort immediately
				Log.error() << "XYZFile::read: illegal header line in XYZ file " << getName() << endl;
				return false;
			}
		}
		else 
		{
			// we could not read the first line. Abort
			// if the first line cannot be read correctly, abort immediately
			Log.error() << "XYZFile::read: illegal header line in XYZ file " << getName() << endl;
			return false;
		}

		// second line: comment -> name of the system
		getline(buffer, BUF_SIZE);
		system.setName(buffer);
		comment_ = buffer;

		Position start = 0;

		HashMap<Position, Atom*> pos_to_atom;

		bool ok = true;

		bool modern_type = 0;
		// ...create a molecule to hold the atoms, and start reading...
		Molecule* mol = new Molecule;

		try
		{
			while (getline(buffer, BUF_SIZE) && (number_of_lines < (number_of_atoms + 2)))
			{
				// read an atom
				number_of_lines++;
				line.set(buffer);

				vector<String> fields;
				Size nr_fields = line.split(fields);
				if (nr_fields < 4)
				{
					Log.error() << "XYZFile: Not enought fields per line!" << std::endl;
					ok = false;
					break;
				}

				if (number_of_lines < 3)
				{
					if (nr_fields > 4) 
					{
						start = 1;
						modern_type = true;
					}
				}
				
				// create the atom, insert it into the molecule
				Atom* atom = new Atom;
				mol->insert(*atom);

				// determine the element
				String elementname = line.getField(start);
				if (modern_type)
				{
					atom->setName(elementname);
					elementname=elementname[0];
				}

				Element& element = PTE[elementname];
				if (element == Element::UNKNOWN)
				{
					Log.error() << "XYZFile::read: unknown element " << elementname 
											<< " in line " << number_of_lines << " of " << getName() 
											<< endl;
				}

				// assign the element and the atom position
				atom->setElement(element);
				atom->setPosition(Vector3(fields[start + 1].toFloat(), 
																	fields[start + 2].toFloat(), 
																	fields[start + 3].toFloat()));

				if (modern_type)
				{
					Position nr = fields[0].toUnsignedInt();
					pos_to_atom[nr] = atom;

					for (Position p = 5; p < nr_fields; p ++)
					{
						Position partner = fields[p].toUnsignedInt();
						if (partner < nr)
						{
							if (!pos_to_atom.has(partner))
							{
								ok = false;
								break;
							}

							atom->createBond(*pos_to_atom[partner]);
						}
					}
				}
			}
		}
		catch(...)
		{
			Log.error() << "XYZFile: Aborting, could not parse line!" << std::endl;
			ok = false;
		}

		if (!ok)
		{
			delete mol;
			return false;
		}

		system.insert(*mol);
		return true;
	}
				
} // namespace BALL
