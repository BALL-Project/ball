// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/FORMAT/KCFFile.h>

#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/forEach.h>

// enable/disable some debug output
#define DEBUG
#undef DEBUG

namespace BALL 
{

  const char* KCFFile::ENTRY_TAG = "ENTRY ";
  const char* KCFFile::NODE_TAG  = "NODE  ";
  const char* KCFFile::EDGE_TAG  = "EDGE  ";
  const char* KCFFile::DELIMITER_TAG = "///";
	const char* KCFFile::CONTINUED_LINE = " ";
	
	KCFFile::KCFFile() 
		:	GenericMolFile()
	{
	}

	KCFFile::KCFFile(const String& name, File::OpenMode open_mode)
		: GenericMolFile()
	{
		GenericMolFile::open(name, open_mode);
	}

	KCFFile::~KCFFile() 
	{
	}

	bool KCFFile::write(const Molecule& molecule)
	{
		if (!isOpen() || getOpenMode() != std::ios::out)
		{
			throw File::CannotWrite(__FILE__, __LINE__, name_);
		}

		// An alias for simplicity's sake...
		std::ostream& os(getFileStream());
		
		// Write ENTRY block
		// number of blanks????  properties are not read, written??? Which ones are there?
		os << ENTRY_TAG << "      " << molecule.getName() << std::endl;
		
		static vector<char> buffer_data(BALL_MAX_LINE_LENGTH);
		char* buffer = &(buffer_data[0]);

		// Write NODE block
		// How to create the KEGG atom types? How many blanks?
		// This is not specified in the KCF format description, so we use what we can
    // deduce from example files.
		// First line gets the NODE tag
		os << NODE_TAG << "      " << molecule.countAtoms() << "\n"; 
		Size count = 1;
		AtomConstIterator ai(molecule.beginAtom());
		std::map<const Atom*, Position> atom_to_index;
		for (; +ai; ++ai, ++count)
		{
			// Write the atom line.
			// Blanks????
			String type = ai->getTypeName();
			String comment;
			
			// Make sure the type is in the set of KEGG types????
			// Blanks?
			sprintf(buffer, "             %d %s %s %6.4f %6.4f %s\n", 
							count, type.c_str(), ai->getElement().getSymbol().c_str(), 
							ai->getPosition().x, ai->getPosition().y, comment.c_str());
			os << buffer;
			
			// Remember the index of the current atom to map atom
			// pointers back to indices for the EDGE section.
			atom_to_index[&*ai] = count;
		}
		
		// Write EDGE block. Walk over all bonds to do so.
		// Blanks????
		os << "EDGE    " << molecule.countBonds() << "\n";
		count = 1;
		for (ai = molecule.beginAtom(); +ai; ++ai)
		{
			for (Atom::BondConstIterator bi(ai->beginBond()); +bi; ++bi)
			{
				Position index1 = atom_to_index[bi->getFirstAtom()];
				Position index2 = atom_to_index[bi->getSecondAtom()];
				String comment;
		
				// Write every bond just once				
				if (bi->getFirstAtom() == &*ai)
				{
					sprintf(buffer, "          %4d %4d %4d %1d%s\n", 
									count, index1, index2, bi->getOrder(), comment.c_str());
					os << buffer;
					++count;
				}
			}
		}
		
		// Write the DELIMITER block
		os << DELIMITER_TAG << std::endl;
		
		return true;
	}

	bool KCFFile::write(const System& system)
	{
		// Walk over all molecules, write them one by one and abort 
		// if one of them could not be written.
		MoleculeConstIterator mol_it(system.beginMolecule());
		for (; +mol_it; ++mol_it)
		{
			if (!write(*mol_it)) 
			{
				return false;
			}
		}
		return true;
	}


	bool KCFFile::read(System& system)
	{
		Molecule* molecule = 0;
		bool read_anything = false;

		// Read all molecules.
		try
		{
			while ((molecule = read()) != 0)
			{
				// add the molecule to the system
				system.append(*molecule);
				molecule = 0;
				read_anything = true;
			}
		}
		catch (Exception::ParseError& e)
		{
			#ifdef DEBUG
				Log.info() << "KCFFile::read(System&): caught exception while parsing line " << getLineNumber() << ": " << e << std::endl;
			#endif

			// clean up and rethrow
			delete molecule;
			throw e;
		}

		// Make sure to return false if nothing could be read.
		return read_anything;
	}

	Molecule* KCFFile::read()
	{
		// Read the first line, this ought to be an ENTRY tag.
		bool ok = readLine();
		if (!ok || !good())
		{
			// end of file encountered or not open -- nothing there to read...
			return 0;
		}

		Molecule* mol = new Molecule;
		IndexAtomMap index_to_atom;
		ok &= readENTRY_(*mol);
		ok &= readNODE_(*mol, index_to_atom);
		ok &= readEDGE_(index_to_atom);
		ok &= readDELIMITER_();

		if (!ok)	
		{
			throw Exception::ParseError(__FILE__, __LINE__, "unknown", getName());
		}

		return mol;
	}
	
	bool KCFFile::readENTRY_(Molecule& mol)
	{
		if (!getLine().hasPrefix(ENTRY_TAG))
		{
			throw Exception::ParseError(__FILE__, __LINE__,
																	String("'") + getLine() + "' (line " + String(getLineNumber()) + " of "  + getName() + "'",
																	String("Expected ENTRY tag: "));
		}
		
		// The name is just the first field in the entry line.
		/// Neglects all comments! ????
		mol.setName(getLine().getField(1));
		
		// Read the next line.
		return readLine();
	}

	bool KCFFile::readNODE_(Molecule& molecule, KCFFile::IndexAtomMap& index_to_atom)
	{
		if (!getLine().hasPrefix(NODE_TAG))
		{
			throw Exception::ParseError(__FILE__, __LINE__,
																	String("'") + getLine() + "' (line " + String(getLineNumber()) + " of "  + getName() + "'",
																	String("Expected NODE tag: "));
		}

		Size number_of_nodes = getLine().getField(1).toInt();
		bool ok = true;
		for (Position i = 0; ok && (i < number_of_nodes); i++)
		{
			ok &= readLine();
			ok &= getLine().hasPrefix(CONTINUED_LINE);
			if (ok)
			{
				Size index = getLine().getField(0).toInt();
				String type = getLine().getField(1);
				String element = getLine().getField(2);
				float x = getLine().getField(3).toFloat();
				float y = getLine().getField(4).toFloat();
				
				// Create a new atom and insert it into the molecule.
				Atom* atom = new Atom;
				molecule.insert(*atom);
				atom->setTypeName(type);
				atom->setElement(PTE[element]);
				atom->setPosition(Vector3(x, y, 0.0));
				
				// Store the pointer/index relation in a hash map
				// to resolve the bonds afterwards.
				index_to_atom[index] = atom;
			}
		}
		
		return (ok && readLine());
	}

	bool KCFFile::readEDGE_(KCFFile::IndexAtomMap& index_to_atom)
	{
		if (!getLine().hasPrefix(EDGE_TAG))
		{
			throw Exception::ParseError(__FILE__, __LINE__,
																	String("'") + getLine() + "' (line " + String(getLineNumber()) + " of "  + getName() + "'",
																	String("Expected EDGE tag: "));
		}

		Size number_of_edges = getLine().getField(1).toInt();
		bool ok = true;
		for (Position i = 0; ok && (i < number_of_edges); i++)
		{
			// Get the next line
			readLine();
			
			// Make sure the line starts with a blank
			ok &= getLine().hasPrefix(CONTINUED_LINE);
			
			// ??? Comments still mising
			Position first = getLine().getField(1).toInt();
			Position second = getLine().getField(2).toInt();
			Position order = getLine().getField(3).toInt();
			
			// Make sure the indices refered to do exist
			ok &= index_to_atom.has(first) && index_to_atom.has(second);
			if (ok)
			{
				Bond* bond = index_to_atom[first]->createBond(*index_to_atom[second]);
				bond->setOrder(order);
			}
		}
		
		return (ok && readLine());
	}

	bool KCFFile::readDELIMITER_()
	{
		return (getLine() == DELIMITER_TAG);
  }
	

	const KCFFile& KCFFile::operator = (const KCFFile& file)
	{
		GenericMolFile::operator = (file);
		return *this;
	}

} // namespace BALL
