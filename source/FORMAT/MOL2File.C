// $Id: MOL2File.C,v 1.12 2001/08/16 00:32:29 oliver Exp $

#include <BALL/FORMAT/MOL2File.h>
#include <BALL/DATATYPE/string.h>
#include <BALL/KERNEL/residue.h>
#include <BALL/KERNEL/protein.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/PDBAtom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/forEach.h>

#include <stack>

using namespace std;

namespace BALL 
{
	MOL2File::MOL2File()
	{
	}

	MOL2File::MOL2File(const String& name, File::OpenMode open_mode)
		throw(Exception::FileNotFound)
		: File(name, open_mode)
	{
	}

	MOL2File::MOL2File(const MOL2File& file)
		throw(Exception::FileNotFound)
		: File(file)
	{
	}

	MOL2File::~MOL2File()
		throw()
	{
	}
	
	// the Tripos record type identifier: RTI
	const String MOL2File::TRIPOS = "@<TRIPOS>";
	const Size MOL2File::MAX_LENGTH_ = 4096;

	void MOL2File::write(const System& system)
	{
		// create a shorthand for the file of the MOL2File object
		File& f = static_cast<File&>(*this);

		// write the molecule header
		f << TRIPOS << "MOLECULE" << endl;

		// if the system name is empty...
		String name = system.getName();
		if (name == "")
		{
			// .. replace it by four stars
			name = "****";
		}
		f << name << endl;

		// determine the number of substructures (= fragments) 
		// and hash the fragment pointers to a substructure ID (Position)
		// the vector substr_names holds the assembled names of the 
		// substructures
		HashMap<const AtomContainer*, Position> substructure_map;
		vector<String>	substructure_name;
		vector<const AtomContainer*> substructure_pointers;
		AtomContainerConstIterator frag_it = system.beginAtomContainer();

		// increment the iterator once to skip the system (which is no substructure)
		++frag_it;
		for (; +frag_it; ++frag_it)
		{
			name = frag_it->getName();
			// if the fragment is a residue...
			const Residue* residue = dynamic_cast<const Residue*>(&*frag_it);
			if (residue != 0)
			{	
				// ...append its ID (PDB ID) to the name. E.g.: ALA154
				name = name + residue->getID();
			}

			// empty names are replaced by "****"
			name.trim();
			if (name == "")
			{
				name = "****";
			}

			// store the name and the pointer 
			substructure_map.insert(pair<const AtomContainer*, Position>(&*frag_it, (Size)substructure_name.size()));
			substructure_pointers.push_back(&*frag_it);
			substructure_name.push_back(name);
		}

		// count the bonds in the system
		Atom::BondConstIterator bond_it;
		AtomConstIterator atom_it;
		Size number_of_bonds = 0;
		BALL_FOREACH_BOND(system, atom_it, bond_it)
		{
			number_of_bonds++;
		}

		// write the number of atoms, bonds, and substructures
		f << system.countAtoms() << " " << number_of_bonds << " " << substructure_name.size() << endl;
		
		String mol_type = "SMALL";
		MoleculeConstIterator mol_it = system.beginMolecule();	
		for (; +mol_it; ++mol_it)
		{
			// if we find a protein, set the molecule type to PROTEIN
			if (RTTI::isKindOf<Protein>(*mol_it))
			{
				mol_type = "PROTEIN";
			}
			// if there is a nucleic acid in the system,
			// set the type to NUCLEIC_ACID. PROTEIN has priority!
			if (RTTI::isKindOf<NucleicAcid>(*mol_it) && (mol_type == "SMALL"))
			{
				mol_type = "PROTEIN";
			}
		}
	
		// write it the molecule type, the charge type, flags, and a comment line
		f << mol_type << endl
			<< "USER_CHARGES" << endl
			<< endl
			<< endl;
		// done with the molecule header.


		// now, write the atoms: Format: atom_id atom_name x y z atom_type subst_id subst_name charge
		// the atom pointers are stored in a hash map (required for bond list construction)
		f << TRIPOS << "ATOM" << endl;

		HashMap<const Atom*, Position> atom_map;
		Size number_of_atoms = 0;
		atom_it = system.beginAtom();
		for (; +atom_it; ++atom_it)
		{
			number_of_atoms++;
			// remember this atom in the hash map
			atom_map.insert(pair<const Atom*, Position>(&*atom_it, number_of_atoms));
			
			f << number_of_atoms << " ";
			name = atom_it->getName();
			name.trim();
			if (name == "")
			{
				// replace empty names by "****"
				name = "****";
			}
			f << name << " "
				<< atom_it->getPosition().x << " " 
				<< atom_it->getPosition().y << " " 
				<< atom_it->getPosition().z << " ";
			name = getSybylType_(*atom_it);
			name.trim();
			if (name == "")
			{
				// replace empty type names by "****"
				name = "****";
			}
			f	<< name << " ";
			
			// if the atom has a substructure, retrieve its name and ID
			const AtomContainer* frag = dynamic_cast<const AtomContainer*>(atom_it->getParent());
			if ((frag != 0) && substructure_map.has(frag))
			{
				f << substructure_map[frag] << " " << substructure_name[substructure_map[frag]] << " ";
			}
			else 
			{
				// write empty substructure ID and name
				f << "1 **** ";
			}

			// write the charge	
			f << atom_it->getCharge() << endl;
		}
		// done with the atom section.

		// write the bonds
		if (number_of_bonds > 0)
		{
			f << TRIPOS << "BOND" << endl;
			number_of_bonds = 0;
			BALL_FOREACH_BOND(system, atom_it, bond_it)
			{
				number_of_bonds++;
				// check whether both atoms were written
				if (atom_map.has(bond_it->getFirstAtom()) && atom_map.has(bond_it->getSecondAtom()))
				{
					
					f << number_of_bonds << " " << atom_map[bond_it->getFirstAtom()] 
						<< " " << atom_map[bond_it->getSecondAtom()] << " ";

					// determine the bond type
					switch (bond_it->getOrder())
					{
						case Bond::ORDER__AROMATIC:		f << "ar" << endl; break;
						case Bond::ORDER__DOUBLE:	  	f << "2"  << endl; break;
						case Bond::ORDER__TRIPLE:	  	f << "3"  << endl; break;
						case Bond::ORDER__QUADRUPLE:	f << "4"  << endl; break;

						case Bond::ORDER__UNKNOWN:
						case Bond::ORDER__SINGLE:		  f << "1"  << endl;
					}
				}
				else 
				{
					// emit a warning
					Log.warn() << "MOL2File::write: could not write bond from " 
										 << bond_it->getFirstAtom()->getFullName() 
										 << " to " << bond_it->getSecondAtom()->getFullName() 
										 << " - not in system!" << endl;	
				}
			}
		}

		// write the substructure section
		// iterate over all substructures and write the substructure parts
		// 
		for (Position i = 0; i < substructure_pointers.size(); i++)
		{
			f << TRIPOS << "SUBSTRUCTURE" << endl
				<< i + 1 << " "
				<< substructure_name[i] << " ";
			Position root_atom = atom_map[&*(substructure_pointers[i]->beginAtom())];
			f << root_atom;
			if (RTTI::isKindOf<Residue>(*substructure_pointers[i]))	
			{
				f << " RESIDUE";
			}
			f << endl;
		}

		// done with writing.
	}


	void MOL2File::read(System& system)
	{
		// remove old rubbish from the system
		system.destroy();

		// reset the contents of the members
		clear_();

		// remember the line number for error messages
		number_of_lines_ = 0;
		
    while (nextLine_())
    {
			line_.toUpper();
			
			while (line_.hasPrefix(TRIPOS))
			{
				// we found a "Record Type Identifier" (RTI)
				String RTI = line_.after(TRIPOS);
				RTI.trim();
				
				
				// interpret the RTI (at least the known ones)
				if (RTI == "ATOM")
				{
					readAtomSection_();
				} 
				else if (RTI == "BOND") 
				{
					readBondSection_();
				}	
				else if (RTI == "MOLECULE") 
				{
					readMoleculeSection_();
				}	
				else if (RTI == "SET") 
				{
					readSetSection_();
				}	
				else if (RTI == "SUBSTRUCTURE") 
				{
					 readSubstructureSection_();
				} 
				else 
				{	
					// we found an unknown MOL2 section: print a warning message and ignore it!
					Log.warn() << "MOL2File::read: section ignored in line " 
										 << number_of_lines_ << ": " << RTI << endl;
					nextLine_();
				}
				
				line_.toUpper();
			}
		}

		// interpret the section we already read from the file
		buildAll_(system);
	}
				
	void MOL2File::readAtomSection_()
	{
		Size number_of_fields = 1;
		while (nextLine_() && (number_of_fields > 0) && !line_.hasPrefix(TRIPOS))
		{
			Size number_of_fields = line_.countFields();
			if (number_of_fields > 0)
			{
				if (number_of_fields < 6)
				{
					Log.error() << "MOL2File::readAtomSection_: too few fields for an atom entry in line " 
											<< number_of_lines_ << endl;
				} 
				else 
				{	
					// split the line into fields
					String	fields[10];
					line_.split(fields, 10);

					// create an atom and assign the fields of the line
					AtomStruct	atom;
					atom.name = fields[1];
					atom.position.x = fields[2].toFloat();
					atom.position.y = fields[3].toFloat();
					atom.position.z = fields[4].toFloat();
					atom.type = fields[5];
					atom.substructure = fields[6].toUnsignedInt();
					atom.substructure_name = fields[7];
					atom.charge = fields[8].toFloat();

					// remember this atom
					atoms_.push_back(atom);
				}
			}
		}	
	}
			
	void MOL2File::readBondSection_()
	{
		Size number_of_fields = 1;
		while (nextLine_() && (number_of_fields > 0) && !line_.hasPrefix(TRIPOS))
		{
			line_.trim();
			Size number_of_fields = line_.countFields();
			if (number_of_fields > 0)
			{
				if (number_of_fields < 4)
				{
					Log.error() << "MOL2File::readBondSection_: too few fields for a bond entry in line " 
											<< number_of_lines_ << endl;
				} 
				else 
				{
					// split the line into fields
					String	fields[4];
					line_.split(fields, 4);

					// create an atom and assign the fields of the line
					BondStruct	bond;
					bond.atom1 = fields[1].toUnsignedInt();
					bond.atom2 = fields[2].toUnsignedInt();
					bond.type = fields[3];
					
					// remember this bond
					bonds_.push_back(bond);
				}
			}
		}	
	}
			
	void MOL2File::readSetSection_()
	{
		Size number_of_fields = 1;
		while (nextLine_() && (number_of_fields > 0) && !line_.hasPrefix(TRIPOS))
		{
			line_.trim();
			Size number_of_fields = line_.countFields();
			if (number_of_fields > 0)
			{
				if (number_of_fields < 3)
				{
					Log.error() << "MOL2File::readSetSection_: too few fields for a bond entry in line " 
											<< number_of_lines_ << endl;
				} 
				else 
				{
					// split the line into fields
					String	fields[6];
					line_.split(fields, 6);

					// create an atom and assign the fields of the line
					SetStruct	set;
					set.name = fields[1].toInt();
					set.type = fields[3].toInt();
					set.subtype = fields[4];
					set.comment = fields[6];
					if (fields[2] == "static")
					{
						nextLine_();
						line_.trim();
						Size number_of_fields = line_.countFields();

						for (Size i = 1; (i <= (Size)line_.getField(0).toInt()) && (i < number_of_fields); i++)
						{
							set.members.push_back(line_.getField(i).toInt());
						}

						// remember this set
						sets_.push_back(set);

					} 
					else 
					{	
						// we cannot read dynamic sets. What is the syntax of these rules?
						Log.warn() << "MOL2File::readSetSection: unsupported set type: " 
											 << fields[2] << ". Ignored." << endl;
					}					
				}
			}
		}	
	}
			
	void MOL2File::readMoleculeSection_()
	{
		Size number_of_fields = 1;
		Size line_number = 0;
		while (nextLine_() && (number_of_fields > 0) && !line_.hasPrefix(TRIPOS) && (line_number <= 5))
		{
			// read four lines
			line_number++;
			number_of_fields = line_.countFields();

			switch (line_number)
			{
				case 1:
					// read the first line: the molecule name (->BALL system name)
					molecule_.name = line_.trim();
					break;

				case 2:
					// read the number of atoms, bonds, and substructures
					molecule_.number_of_atoms = line_.getField(0).toUnsignedInt();
					if (number_of_fields > 1)
					{
						molecule_.number_of_bonds = line_.getField(1).toUnsignedInt();
					}
					if (number_of_fields > 2)
					{
						molecule_.number_of_substructures = line_.getField(2).toUnsignedInt();
					}
					if (number_of_fields > 3)
					{
						molecule_.number_of_features = line_.getField(3).toUnsignedInt();
					}
					if (number_of_fields > 4)
					{
						molecule_.number_of_sets = line_.getField(4).toUnsignedInt();
					}
					break;

				case 3:
				case 4:
				case 5:
					// ignore lines 3 - 5
					;
			}
		}	
	}

	void MOL2File::readSubstructureSection_()
	{
		while (nextLine_() && (line_.countFields() > 0) && !line_.hasPrefix(TRIPOS))
		{
			SubstructureStruct sub;

			Size number_of_fields = line_.countFields();

			sub.name = line_.getField(1);
			sub.root_atom = line_.getField(2).toUnsignedInt();
			if (number_of_fields > 3)
			{
				sub.substructure_type = line_.getField(3);
			}
			if (number_of_fields > 4)
			{
				sub.dictionary_type = line_.getField(4).toUnsignedInt();
			}
			if (number_of_fields > 5)
			{
				sub.chain = line_.getField(5);
			}
			if (number_of_fields > 6)
			{
				sub.sub_type = line_.getField(6);
			}
			if (number_of_fields > 7)
			{
				sub.inter_bonds = line_.getField(7).toUnsignedInt();
			}
			for (Position i = 8; i < number_of_fields; i++)
			{
				sub.comment += line_.getField(i) + " ";
			}
			sub.comment.trimRight();
					
			substructures_.push_back(sub);
		}	
	}

	void MOL2File::clear_()
	{
		// clear the structure for the molecule section
		molecule_.name = "";
		molecule_.number_of_atoms = 0;
		molecule_.number_of_bonds = 0;
		molecule_.number_of_substructures = 0;
		molecule_.number_of_features = 0;
		molecule_.number_of_sets = 0;

		// clear the vectors for the other sections
		atoms_.clear();
		bonds_.clear();
		substructures_.clear();
		sets_.clear();
	}
		

	void MOL2File::buildAll_(System& system) 
	{
		// name the system
		system.setName(molecule_.name);

		// consistency check
		if (atoms_.size() != molecule_.number_of_atoms)
		{
			Log.error() << "MOL2File::read: number of atoms in the MOLECULE section (" 
									<< molecule_.number_of_atoms << ")" << endl
									<< " is not consistent with the contents of the ATOM section (" 
									<< atoms_.size() << " atoms)!" << endl;
			return;
		}
		if (bonds_.size() != molecule_.number_of_bonds)
		{
			Log.error() << "MOL2File::read: number of bonds in the MOLECULE section (" 
									<< molecule_.number_of_bonds << ")" << endl
									<< " is not consistent with the contents of the BOND section (" 
									<< bonds_.size() << " bonds)!" << endl;
			return;
		}
		if (substructures_.size() != molecule_.number_of_substructures)
		{
			Log.error() << "MOL2File::read: number of substructures in the MOLECULE section (" 
									<< molecule_.number_of_substructures << ")" << endl
									<< " is not consistent with the contents of the SUBSTRUCTURE section (" 
									<< substructures_.size() << " substructures)!" << endl;
			return;
		}

		// construct the substructures (if any)
		vector<AtomContainer*>	sub_ptr(substructures_.size());
		Position i;
		for (i = 0; i < substructures_.size(); i++)
		{
			AtomContainer* frag = 0;
			if (substructures_[i].substructure_type == "RESIDUE")
			{
				Residue* residue= new Residue;
				frag = static_cast<AtomContainer*>(residue);

				// Sybyl stores the residue (PDB) ID in the 
				// residue name (e.g. ALA175)
				RegularExpression re("[0-9][0-9A-Z]*$");
				Substring ID;
				if (re.find(substructures_[i].name, ID))
				{
					// assign the ID to the residue
					residue->setID(ID.toString());

					// and remove it from the fragment name
					ID = "";
				}
			}
			else
			{
				// create a fragment
				frag = static_cast<AtomContainer*>(new Fragment);
			}

			// set the fragment name
			if (frag != 0)
			{
				frag->setName(substructures_[i].name);
			}

			// store the pointer to this substructure
			sub_ptr[i] = frag;
 		}
		

		// construct the atoms
		vector<Atom*> atom_ptr(atoms_.size());
		for (i = 0; i < atoms_.size(); i++)
		{
			// create a new atom and assign its attributes
			Atom* atom = new Atom;
			atom->setName(atoms_[i].name);
			atom->setPosition(atoms_[i].position);
			atom->setTypeName(atoms_[i].type);
			atom->setCharge(atoms_[i].charge);

			// Try to assign the element. MOL2 format
      // usually has type names like O.2 or C.ar, so 
			// we use the portion before the dot or the complete
			// name if there's no dot in the name. PTE will translate
			// it to the correct element or Element::UNKNOWN otherwise.
			if (atoms_[i].type.has('.'))
			{
				atom->setElement(PTE[atoms_[i].type.before(".")]);
			}
			else
			{
				atom->setElement(PTE[atoms_[i].type]);
			}
			
			// store the atom pointer for bond construction
			atom_ptr[i] = atom;
		}

		// construct the bonds
		for (i = 0; i < bonds_.size(); i++)
		{
			if ((bonds_[i].atom1 > atom_ptr.size()) || (bonds_[i].atom2 > atom_ptr.size()))
			{
				Log.error() << "MOL2File::read: cannot build bond between atoms " 
					<< bonds_[i].atom1 << " and " << bonds_[i].atom2 << endl;
			}
			else 
			{
				Bond* bond = atom_ptr[bonds_[i].atom1 - 1]->createBond(*atom_ptr[bonds_[i].atom2 - 1]);
				if (bonds_[i].type == "ar")
				{
					bond->setOrder(Bond::ORDER__AROMATIC);
				}
				else if (bonds_[i].type == "1")
				{
					bond->setOrder(Bond::ORDER__SINGLE);
				}
				else if (bonds_[i].type == "2")
				{
					bond->setOrder(Bond::ORDER__DOUBLE);
				}
				else if (bonds_[i].type == "1")
				{
					bond->setOrder(Bond::ORDER__TRIPLE);
				}
				else
				{
					Log.error() << "MOL2File::read: unkown bond type " << bonds_[i].type << endl;
				}
			}
		}

		// insert all atoms into their proper substructures
		for (i = 0; i < sub_ptr.size(); i++)
		{
			Size last = (Size)atoms_.size();
			if (i < sub_ptr.size() - 1)
			{
				last = substructures_[i + 1].root_atom - 1;
			}

			for (Size j = substructures_[i].root_atom; j <= last; j++)
			{
				if (j < 1)
				{
					Log.error() << "MOL2File::read: cannot access atom with index below 1 (root atom of substructure " 
											<< i + 1 << ")" << endl;
				}
				else
				{
					sub_ptr[i]->insert(*atom_ptr[j - 1]);
				}
			}
			
		}

		// insert all substructures into the system
		Molecule* molecule = new Molecule;
		system.insert(*molecule);
		for (i = 0; i < sub_ptr.size(); i++)
		{
			molecule->insert(*sub_ptr[i]);
		}
		
		// if there are no substructures, insert the atoms
    // into the molecule
		if (substructures_.size() == 0)
		{
			for (i = 0; i < atom_ptr.size(); i++)
			{	
				molecule->insert(*atom_ptr[i]);
			}
		}
	}

	bool MOL2File::nextLine_() 
	{
		if (getline(buffer_, MAX_LENGTH_))
		{
			line_.set(buffer_);
			number_of_lines_++;
			
			return true;
		}
		
		return false;
	}
			
	String MOL2File::getSybylType_(const Atom& atom) const
	{
		// the basename of Sybyl name is always the element
		// and a trailing dot
		String name = atom.getElement().getSymbol();
		
		// if there's more than one bond, add the number of
		// bonds and a dot as separator
		if (atom.countBonds() > 1)
		{
			name = name + ".";
			name = name + atom.countBonds();
		}
		
		return name;
	}

} // namespace BALL
