// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/FORMAT/XYZFile.h>
#include <BALL/DATATYPE/string.h>
#include <BALL/KERNEL/atomContainer.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/COMMON/exception.h>

using namespace std;

namespace BALL 
{

	XYZFile::XYZFile()
		: GenericMolFile()
	{
	}

	XYZFile::XYZFile(const String& name, File::OpenMode open_mode)
		: GenericMolFile()
	{
		GenericMolFile::open(name, open_mode);
	}

	XYZFile::~XYZFile()
	{
	}
	
	bool XYZFile::write(const AtomContainer& ac)
	{
		if (!isOpen() || getOpenMode() != std::ios::out)
		{
			throw (File::CannotWrite(__FILE__, __LINE__, name_));
		}

		// write the first and the second line:
		//  - the number of atoms
		//  - a comment line
		(File&)(*this) << (Size)ac.countAtoms() << endl
									 << ac.getName() << endl;
		
		// write all atoms
		AtomConstIterator it = ac.beginAtom();
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
	bool XYZFile::write(const System& system)
	{
		return write((const AtomContainer&)system);
	}

	bool XYZFile::write(const Molecule& mol)
	{
		return write((const AtomContainer&)mol);
	}
	
	bool XYZFile::read(System& system)
	{
		// remove old rubbish from the system
		system.destroy();

		Molecule *molecule = read();

		if (!molecule)
			return false;

		system.insert(*molecule);
		system.setName(molecule->getName());

		return true;
	}

	Molecule* XYZFile::read()
	{
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
				if (line.countFields() > 1)
				{
					comment_ = line.getField(1);
				}
			}
			else
			{
				// if the first line cannot be read correctly, abort immediately
				Log.error() << "XYZFile::read: illegal header line in XYZ file " << getName() << endl;
				return NULL;
			}
		}
		else 
		{
			// we could not read the first line. Abort
			// if the first line cannot be read correctly, abort immediately
			Log.error() << "XYZFile::read: illegal header line in XYZ file " << getName() << endl;
			return NULL;
		}

		// ...create a molecule to hold the atoms, and start reading...
		Molecule* mol = new Molecule;

		// second line: comment -> name of the system
		if (comment_ == "")
		{
			getline(buffer, BUF_SIZE);
			mol->setName(buffer);
			comment_ = buffer;
		}

		Position start = 0;

		HashMap<Position, Atom*> pos_to_atom;

		bool ok = true;

		bool modern_type = 0;

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
								Log.error() << "Could not create bond!" << std::endl;
								ok = false;
								break;
							}

							atom->createBond(*pos_to_atom[partner]);
						}
					}
				}
			}
		}
		catch(Exception::GeneralException& e)
		{
			ok = false;
			Log.error() <<  e << std::endl;
		}
		catch(...)
		{
			ok = false;
		}

		if (number_of_lines != number_of_atoms + 1)
		{
			ok = false;
			Log.error() << "Could not read all atoms" << std::endl;
		}

		if (!ok)
		{
			Log.error() << "XYZFile: Aborting, could not parse line!" << std::endl;
			delete mol;
			return NULL;
		}

		return mol;
	}
} // namespace BALL
