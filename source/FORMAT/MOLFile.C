// $Id: MOLFile.C,v 1.1 2001/12/17 11:27:14 oliver Exp $

#include <BALL/FORMAT/MOLFile.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/PTE.h>



namespace BALL 
{

	// the format definition of the counts line
	const String MOLFile::counts_format_ = "%3d%3d%3d%3d%3d%3d%3d%3d%3d%6s\n";

	// the format definition for the atom block
	const String MOLFile::atom_format_ = "%10.4f%10.4f%10.4f %3s%2d%3d%3d%3d%3d%3d%3d%3d%3d%3d%3d%3d%3d\n";

	// the format definition for the bond block
	const String MOLFile::bond_format_ = "%3d%3d%3d%3d%3d%3d%3d\n";

	//
  const String MOLFile::Property::ATOM_MASS_DIFFERENCE = "MOLFile::MASS_DIFFERENCE";
	//
  const String MOLFile::Property::ATOM_HYDROGEN_COUNT = "MOLFile::HYDROGEN_COUNT";
	//
  const String MOLFile::Property::ATOM_STEREO_CARE_BOX = "MOLFile::STEREO_CARE_BOX";
  //
  const String MOLFile::Property::ATOM_VALENCE = "MOLFile::ATOM_VALENCE";
  //
  const String MOLFile::Property::ATOM_H0_DESIGNATOR = "MOLFile::H0_DESIGNATOR";
  //
	const String MOLFile::Property::ATOM_REACTION_COMPONENT_TYPE = "MOLFile::REACTION_COMPONENT_TYPE";
	//
	const String MOLFile::Property::ATOM_REACTION_COMPONENT_NUMBER = "MOLFile::REACTION_COMPONENT_NUMBER";
	//
	const String MOLFile::Property::ATOM_INVERSION_RETENTION = "MOLFile::INVERSION_RETENTION";
	//
	const String MOLFile::Property::ATOM_EXACT_CHANGE = "MOLFile::ATOM_EXACT_CHANGE";

	//
	const String MOLFile::Property::BOND_STEREO = "MOLFile::STEREO";
	//
	const String MOLFile::Property::BOND_TOPOLOGY = "MOLFile::TOPOLOGY";
	//
	const String MOLFile::Property::BOND_REACTING_CENTER_STATUS = "MOLFile::REACTING_CENTER_STATUS";


	MOLFile::MOLFile()
		throw()
		:	GenericMolFile()
	{
	}

	MOLFile::MOLFile(const String& name, File::OpenMode open_mode)
		throw(Exception::FileNotFound)
		: GenericMolFile(name, open_mode)
	{
	}

	MOLFile::MOLFile(const MOLFile& file)
		throw(Exception::FileNotFound)
		: GenericMolFile(file)
	{
	}

	MOLFile::~MOLFile()
		throw()
	{
	}

	void MOLFile::write(const System& system)
	{
	}

	Molecule* MOLFile::readCTAB(vector<Atom*>& atom_map)
		throw(Exception::ParseError)
	{
		// read the counts line
		CountsStruct counts;
		readCountsLine(counts);

		// create a new molecule to hold the stuff read from the atom/bond block
		Molecule& molecule = *new Molecule;
		atom_map.resize(counts.number_of_atoms);
		try
		{
			// read the atom block
			AtomStruct atom_struct;
			for (Position i = 0; i < counts.number_of_atoms; i++)
			{
				readAtomLine(atom_struct);
				// make sure the atom number is in a valid range (as defined by the Counts line)
				if ((atom_struct.number < 1) || (atom_struct.number > counts.number_of_atoms))
				{
					throw Exception::ParseError(__FILE__, __LINE__, getLine(), String("Referencing undefined atom number: ") + String(atom_struct.number));
				}

				// create the atom
				Atom* atom = new Atom(PTE[atom_struct.symbol.trim()], 
															String("[atom ") + String(atom_struct.number) + "]", // name
															"", // emtpy type name
															Atom::UNKNOWN_TYPE,
															atom_struct.position,
															Vector3(0.0, 0.0, 0.0), // velocity undefined
															Vector3(0.0, 0.0, 0.0), // force undefined
															0.0, // charge reset below
															0.0); // radius undefined
				// insert the atom into the molecule and store its pointer in 
				// a map array
				molecule.append(*atom);
				atom_map[atom_struct.number - 1] = atom;

				// assign the charge (overridden by M CHG lines below)
				switch (atom_struct.charge)
				{
					case 0: break; // charge 0
					case 1:	atom->setCharge(3.0); break;
					case 2:	atom->setCharge(2.0); break;
					case 3:	atom->setCharge(1.0); break;
					case 4:	atom->setCharge(0.0); break; // doublet -- how to handle this?
					case 5: atom->setCharge(-1.0); break;
					case 6: atom->setCharge(-2.0); break;
					case 7: atom->setCharge(-3.0); break;
					default:
						throw Exception::ParseError(__FILE__, __LINE__, getLine(), String("Illegal charge flag: ") + String(atom_struct.charge));
				}
				
				// store the remaining information as named properties
				atom->setProperty(Property::ATOM_MASS_DIFFERENCE, atom_struct.mass_difference);
				atom->setProperty(Property::ATOM_HYDROGEN_COUNT, atom_struct.hydrogen_count);
				atom->setProperty(Property::ATOM_STEREO_CARE_BOX, atom_struct.stereo_care_box);
				atom->setProperty(Property::ATOM_VALENCE, atom_struct.valence);
				atom->setProperty(Property::ATOM_H0_DESIGNATOR, atom_struct.H0_designator);
				atom->setProperty(Property::ATOM_REACTION_COMPONENT_TYPE, atom_struct.reaction_component_type);
				atom->setProperty(Property::ATOM_REACTION_COMPONENT_NUMBER, atom_struct.reaction_component_number);
				atom->setProperty(Property::ATOM_INVERSION_RETENTION, atom_struct.inversion_retention);
				atom->setProperty(Property::ATOM_EXACT_CHANGE, atom_struct.exact_change);
			}

			// read the bond block
			BondStruct bond_struct;
			for (Position i = 0; i < counts.number_of_bonds; i++)
			{
				readBondLine(bond_struct);
				
				// ensure the atoms referenced do exist
				if ((bond_struct.first_atom < 1) || (bond_struct.first_atom > counts.number_of_atoms))
				{
					throw Exception::ParseError(__FILE__, __LINE__, getLine(), String("Referencing undefined atom number: ") + String(bond_struct.first_atom));
				}
				if ((bond_struct.second_atom < 1) || (bond_struct.second_atom > counts.number_of_atoms))
				{
					throw Exception::ParseError(__FILE__, __LINE__, getLine(), String("Referencing undefined atom number: ") + String(bond_struct.second_atom));
				}
				
				// create the bond
				Bond* bond = atom_map[bond_struct.first_atom]->createBond(*(atom_map[bond_struct.second_atom]));
				
				// translate the bond type
				switch (bond_struct.type)
				{
					case 1: // single
						bond->setType(Bond::ORDER__SINGLE); break;
					
					case 2: // double	
						bond->setType(Bond::ORDER__DOUBLE); break;

					case 3: // 
						bond->setType(Bond::ORDER__TRIPLE); break;

					case 4: // 
						bond->setType(Bond::ORDER__AROMATIC); break;

					case 5: // single or double
					case 6: // single or aromatic
					case 7: // double or aromatic
					case 8: // any
						break; 
					
					default:
						throw Exception::ParseError(__FILE__, __LINE__, getLine(), String("Illegal bond type: ") + String(bond_struct.type));
				}

				// store remaining stuff as named properties	
				bond->setProperty(Property::BOND_STEREO, bond_struct.stereo);
				bond->setProperty(Property::BOND_TOPOLOGY, bond_struct.topology);
				bond->setProperty(Property::BOND_REACTING_CENTER_STATUS, bond_struct.reacting_center_status);
			}

			// skip the atom list block
			for (Position i = 0; i < counts.number_of_atom_lists; i++)
			{
				readLine();
			}

			// skip the stext  block
			for (Position i = 0; i < counts.number_of_stext_entries; i++)
			{
				readLine();
				readLine();
			}

			// read the properties block
			while (readLine() && good() && startsWith("M "))
			{
				if (startsWith("M  END"))
				{
					break;
				}
				else
				{
					// delete the "M  " part of the line -- the next three letters are the tag.
					String tag(getLine().getSubstring(3, 3));
					tag.trim();

					if (tag == "A") // atom alias
					{
						//????
					}
					else if (tag == "V") // atom value
					{
					}
					else if (tag == "G") // group
					{
					}
					else if (tag == "CHG") // charge
					{
					}
					else if (tag == "RAD") // radical
					{
					}
					else if (tag == "ISO") // radical
					{
					}
					else if (tag == "RBC") // ring bond count
					{
					}
					else if (tag == "SUB") // substitution count
					{
					}
					else if (tag == "UNS") // unsaturated atom
					{
					}
					else if (tag == "LIN") // link atom
					{
					}
					else if (tag == "ALS") // atom list
					{
					}
					else if (tag == "APO") // attachment point
					{
					}
					else if (tag == "AAL") // atom attachment order
					{
					}
					else if (tag == "RGP") // Rgroup label location
					{
					}
					else if (tag == "LOG") // Rgroup logic, unstisfied sites, Range of occurrence
					{
					}
					else if (tag == "STY") // Sgroup type
					{
					}
					else if (tag == "SST") // Sgroup subtype
					{
					}
					else if (tag == "SLB") // Sgroup labels
					{
					}
					else if (tag == "SCN") // Sgroup connectivity
					{
					}
					else if (tag == "") // Sgroup connectivity
					{
					}
					else
					{
						Log.warn() << "MOLFile::readCTAB: ignoring property entry " << getLine() << std::endl;
					}
				}
			}
		}
		catch (Exception::GeneralException& e)
		{
			// clean up
			delete &molecule;
			throw e;
		}
		
		// return the constructed molecule
		return &molecule;
	}

	void MOLFile::read(System& system)
		throw(Exception::ParseError)
	{
		// read the header block: first line == name, third line = comment, second line ignored
		bool ok = true;
		ok &= readLine();
		String name = getLine();
		ok &= readLine();
		ok &= readLine();
		String comment = getLine();
		if (!ok)
		{
			throw Exception::ParseError(__FILE__, __LINE__, getLine(), "Unable to read header block!");
		}

		vector<Atom*> atom_map;
		Molecule* molecule = readCTAB(atom_map);
		
		// add the molecule to the system
		system.append(*molecule);
	}

	void MOLFile::readCountsLine(MOLFile::CountsStruct& counts)
	{
		// read the next line
		readLine();

		// parse the line according to the Counts format
		counts.number_of_atoms = 0;
		parseColumnFormat("%3d", 0, 3, (void*)&counts.number_of_atoms);

		counts.number_of_bonds = 0;
		parseColumnFormat("%3d", 3, 3, (void*)&counts.number_of_bonds);

		counts.number_of_atom_lists = 0;
		parseColumnFormat("%3d", 6, 3, (void*)&counts.number_of_atom_lists);

		counts.number_of_stext_entries = 0;
		parseColumnFormat("%3d", 12, 3, (void*)&counts.number_of_stext_entries);

		Size chiral = 0;
		parseColumnFormat("%3d", 15, 3, (void*)&chiral);
		counts.chiral = (chiral == 0);
		
		counts.number_of_reaction_components = 0;
		parseColumnFormat("%3d", 18, 3, (void*)&counts.number_of_reaction_components);

		counts.number_of_reactants = 0;
		parseColumnFormat("%3d", 21, 3, (void*)&counts.number_of_reactants);

		counts.number_of_products = 0;
		parseColumnFormat("%3d", 24, 3, (void*)&counts.number_of_products);

		counts.number_of_intermediates = 0;
		parseColumnFormat("%3d", 24, 3, (void*)&counts.number_of_intermediates);
	}

	void MOLFile::writeCountsLine(const MOLFile::CountsStruct& counts)
	{
		char buf[256];
		sprintf(buf, counts_format_.c_str(), 
						counts.number_of_atoms,
						counts.number_of_bonds,
						counts.number_of_atom_lists,
						999, // field is obsolete
						(int)counts.chiral,
						counts.number_of_stext_entries,
						counts.number_of_reaction_components,
						counts.number_of_reactants,
						counts.number_of_products,
						counts.number_of_intermediates,
						999, // number of additional properties -- no longer supported (default = 999)
						counts.version.c_str());
		*this << buf;
	}

	void MOLFile::readAtomLine(MOLFile::AtomStruct& atom)
	{
	}

	void MOLFile::writeAtomLine(const MOLFile::AtomStruct& atom)
	{
	}

	void MOLFile::readBondLine(MOLFile::BondStruct& bond)
	{
	}

	void MOLFile::writeBondLine(const MOLFile::BondStruct& bond)
	{
	}

} // namespace BALL
