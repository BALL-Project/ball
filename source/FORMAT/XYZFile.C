// $Id: XYZFile.C,v 1.2 2000/06/20 21:31:42 oliver Exp $

#include <BALL/FORMAT/XYZFile.h>
#include <BALL/DATATYPE/string.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/PTE.h>

using namespace std;

namespace BALL 
{

	XYZFile::XYZFile()
	{
	}

	XYZFile::XYZFile(const String& name, File::OpenMode open_mode)
		: File(name, open_mode)
	{
	}

	XYZFile::XYZFile(const XYZFile& file)
		: File(file)
	{
	}

	XYZFile::~XYZFile()
	{
	}
	
	void XYZFile::write(const System& system)
	{
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
	}

	void XYZFile::read(System& system)
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

		// read the number of atoms (first line)
		if (getline(buffer, BUF_SIZE))
		{
			line.set(buffer);
			number_of_lines++;
			if (line.countFields() == 1)
			{
				// retrieve the number of atoms
				number_of_atoms = line.getField(0).toUnsignedInt();
			}
			else
			{
				// if the first line cannot be read correctly, abort immediately
				Log.error() << "XYZFile::read: illegal header line in XYZ file " << getName() << endl;
				return;
			}
		}
		else 
		{
			// we could not read the first line. Abort
			// if the first line cannot be read correctly, abort immediately
			Log.error() << "XYZFile::read: illegal header line in XYZ file " << getName() << endl;
			return;
		}

		// second line: comment -> name of the system
		getline(buffer, BUF_SIZE);
		system.setName(buffer);

		// ...create a molecule to hold the atoms, and start reading...
		Molecule* mol = new Molecule;
    while (getline(buffer, BUF_SIZE) && (number_of_lines < (number_of_atoms + 2)))
    {
			// read an atom
      number_of_lines++;
      line.set(buffer);
			
			// create the atom, insert it into the molecule
			Atom* atom = new Atom;
			mol->insert(*atom);

			// determine the element
			String elementname = line.getField(0);
			Element& element = PTE[line.getField(0)];
			if (element == Element::UNKNOWN)
			{
				Log.error() << "XYZFile::read: unknown element " << elementname 
										<< " in line " << number_of_lines << " of " << getName() 
										<< endl;
			}

			// assign the element and the atom position
			atom->setElement(element);
			atom->setPosition(Vector3(line.getField(1).toFloat(), 
																line.getField(2).toFloat(), 
																line.getField(3).toFloat()));
		}

		system.insert(*mol);
	}
				
} // namespace BALL
