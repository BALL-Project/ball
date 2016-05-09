#include <BALL/FORMAT/antechamberFile.h>
#include <BALL/DATATYPE/hashMap.h>
#include <BALL/KERNEL/atomContainer.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/residue.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/MATHS/common.h>

#include <vector>

namespace BALL
{
	AntechamberFile::AntechamberFile()
		: GenericMolFile()
	{
	}

	AntechamberFile::AntechamberFile(const String& name, File::OpenMode open_mode)
		: GenericMolFile()
	{
		GenericMolFile::open(name, open_mode);
	}

	AntechamberFile::~AntechamberFile()
	{
	}

	bool AntechamberFile::write(const AtomContainer& ac)
	{
		if (!isOpen() || getOpenMode() != std::ios::out)
		{
			throw (File::CannotWrite(__FILE__, __LINE__, name_));
		}

		// first, we write the charge information
		float total_charge = 0.f;
		for (AtomConstIterator at_it = ac.beginAtom(); +at_it; ++at_it)
		{
			total_charge += at_it->getCharge();
		}

		int rounded_charge = (int) Maths::round(total_charge);

		String charge_line(255, "CHARGE      %.2f ( %d )", total_charge, rounded_charge);
		(File&)(*this) << charge_line << std::endl;

		// next, we need the element counts
		std::map<String, Size> element_counts;
		for (AtomConstIterator at_it = ac.beginAtom(); +at_it; ++at_it)
		{
			String const& element = at_it->getElement().getSymbol();
			if (element_counts.find(element) != element_counts.end())
				element_counts[element]++;
			else
				element_counts[element] = 1;
		}

		// and write it to the file.
		String formula_line("Formula: ");

		std::map<String, Size>::iterator element_iterator = element_counts.begin();
		for (; element_iterator != element_counts.end(); ++element_iterator)
		{
			formula_line += element_iterator->first + String(element_iterator->second) + " ";
		}
		(File&)(*this) << formula_line << std::endl;

		// write all atoms
		String atom_format("ATOM%7d  %-4s%-4s%5d%12.3f%8.3f%8.3f%10.6lf%10s");

		Position current_atom = 1;		

		HashMap<Atom const*, Position> atom_indices;
		HashMap<Atom const*, String>   atom_names;
		HashMap<String, Position>      element_count;

		AtomConstIterator at_it = ac.beginAtom();
		for (; +at_it; ++at_it, ++current_atom)
		{
			// we will need the indices for the bonds later on
			atom_indices[&*at_it] = current_atom;

			String parent_name = ((AtomContainer const*)(at_it->getParent()))->getName();
			parent_name.truncate(4);  
			if (parent_name == "")
				parent_name = "UNK";

			Position residue_id = 1;
			Residue const* residue = at_it->getResidue();
			if (residue)
			{
				residue_id = residue->getID().toInt();
			}

			String atom_type = at_it->getTypeName();
			if (at_it->hasProperty("atomtype"))
				atom_type = at_it->getProperty("atomtype").getString();
			if (atom_type.size()>10)
				atom_type.truncate(10);

			String atom_name = at_it->getElement().getSymbol();
			if (element_count.has(atom_name))
			{
				++element_count[atom_name];
			}
			else
			{
				element_count[atom_name] = 1;
			}
			atom_name += String(element_count[atom_name]);
			//atom_name.substitute(" ", "_");
			
			//String atom_name = at_it->getName();
			atom_name.truncate(4);

			atom_names[&*at_it] = atom_name;

			String atom_line(255, atom_format.c_str(), current_atom, atom_name.c_str(), parent_name.c_str(), 
														residue_id, at_it->getPosition().x, at_it->getPosition().y, at_it->getPosition().z,
														at_it->getCharge(), atom_type.c_str());

			(File&)(*this) << atom_line << std::endl;
		}

		// and finally, write out all bonds
		String bond_format("BOND%5d%5d%5d%5d  %5s%5s");

		Position current_bond = 1;

		Atom::BondConstIterator bit;
		BALL_FOREACH_BOND(ac, at_it, bit)
		{
			String bond_line(255, bond_format.c_str(), current_bond, atom_indices[bit->getFirstAtom()],
			                      atom_indices[bit->getSecondAtom()], bit->getOrder(),
														atom_names[bit->getFirstAtom()].c_str(), atom_names[bit->getSecondAtom()].c_str());

			(File&)(*this) << bond_line << std::endl;

			++current_bond;
		}
		
		// yep, that's it...

		return true;
	}

	bool AntechamberFile::write(const System& system)
	{
		return write((const AtomContainer&)system);
	}

	bool AntechamberFile::write(const Molecule& mol)
	{
		return write((const AtomContainer&)mol);
	}

	bool AntechamberFile::read(System& system)
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

	Molecule* AntechamberFile::read()
	{
		// ...create a molecule to hold the atoms, and start reading...
		Molecule* mol = new Molecule;

		// the first two lines contain charge and chemical formula, and we don't care
		// about them. but for later reference, we store them as properties.
		String line;

		line.getline(*this);
		mol->setProperty("AC_CHARGE_LINE", line);

		line.getline(*this);
		mol->setProperty("AC_FORMULA_LINE", line);

		// antechamber does not guarantee the order of the lines to follow, so we have
		// to be a little creative
		HashMap<Position, Atom*> atom_indices;

		// TODO: if there is a TER, create chains
		while (!eof())
		{
			line.getline(*this);
			
			std::vector<String> split;
			ac_split_(line, split);

			if (split.empty())
				continue;

			if (split[0] == "ATOM")
			{
				// TODO: currently, we ignore the residue name and the residue number!
				if (split.size() < 10)
				{
					Log.error() << "AntechamberFile::read: illegal atom line in ac file " << getName() << std::endl;
					continue;
				}
				// create a new atom, fill it with information, and put it where it belongs
				Atom* new_atom = new Atom;
				atom_indices[split[1].toInt()] = new_atom;

				new_atom->setName(split[2]);

				// split the name into the element component and the index
				String element_symbol;

				bool first_char_found = false;
				for (Position i=0; i < split[2].size(); ++i)
				{
					// only use characters to determine the element type, and after the first
					// character was found, only use lowercase chars (CA => C, Ca => Ca)
					if (isalpha(split[2][i]) && ((!first_char_found) || islower(split[2][i])))
					{
						first_char_found = true;
						element_symbol += split[2][i];
					}
				}

				new_atom->setElement(PTE[element_symbol]);
				new_atom->setPosition(Vector3(split[5].toFloat(), split[6].toFloat(), split[7].toFloat()));
				new_atom->setCharge(split[8].toFloat());

				new_atom->setProperty("atomtype", split[9].trim());

				mol->insert(*new_atom);
			}
			else if (split[0] == "BOND")
			{
				// TODO: currently, we ignore the bonds atom types
				if (split.size() < 7)
				{
					Log.error() << "AntechamberFile::read: illegal bond line in ac file " << getName() << std::endl;
					continue;
				}
				
				// create a new bond
				Atom* first_atom  = atom_indices[split[2].toInt()];
				Atom* second_atom =  atom_indices[split[3].toInt()];
				if (first_atom && second_atom)
				{
					Bond* b = first_atom->createBond(*second_atom);

					// we need to convert the bond order from AM1-BCC to our own definitions
					Position new_order = split[4].toInt();
					
					// NOTE: like antechamber, we treat delocalized bonds as single bonds
					if (new_order == 7) // aromatic single in AM1
						new_order = 1;
					else if (new_order == 8) // aromatic double in AM1
						new_order = 2;
					else if (new_order == 9) // delocalized in AM1
						new_order = 1;

					b->setOrder(new_order);
				}
				else
				{
					Log.error() << "AntechamberFile::read: illegal bond line in ac file " << getName() << std::endl;
					continue;
				}
			}	
		}
		return mol;
	}	


	void AntechamberFile::ac_split_(const String& line, std::vector<String>& split)
	{ 
		Size line_length = line.size();
		if (line_length < 4)
			return;

		if (line.getSubstring(0,4) == "ATOM")
		{
			if (line_length < 74)
			{	
				Log.error() << "AntechamberFile::read: illegal atom line in ac file " << getName() << std::endl;
				return;
			}
			else
			{	
				// ATOM     40  H8  UNK     1      20.773  19.186  35.058  0.000000        ha
				// String atom_format("ATOM%7d  %-4s%-4s%5d%12.3f%8.3f%8.3f%10.6lf%10s");
				
				// first the key
				split.push_back(line.getSubstring(0,4));

				// the atom index
				split.push_back(line.getSubstring(4,9).toString().trim());

				// the atom name
				split.push_back(line.getSubstring(13,4).toString().trim());

				// the chain name	
				split.push_back(line.getSubstring(17,4).toString().trim());

				// the residue id
				split.push_back(line.getSubstring(21,5).toString().trim());

				// the positions (x,y,z)
				split.push_back(line.getSubstring(26,12).toString().trim());
				split.push_back(line.getSubstring(38,8).toString().trim());
				split.push_back(line.getSubstring(46,8).toString().trim());

				// the atom type
				split.push_back(line.getSubstring(54,10).toString().trim());

				// the charge
				split.push_back(line.getSubstring(64,10).toString().trim());
			}
		}
		else if (line.getSubstring(0,4) == "BOND")
		{
			// BOND   22   11   38    1     C6   H6
			// String bond_format("BOND%5d%5d%5d%5d  %5s%5s");
			line.split(split);
		}
		else if (line.getSubstring(0,4) == "CHAR")
		{
			//CHARGE      0.00 ( 0 )
			line.split(split);
		}
		else if (line.getSubstring(0,4) == "Form")
		{
			//Formula: H23 C23 N7 O2 
			line.split(split);
		}		
	}
}
