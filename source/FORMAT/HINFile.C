// $Id: HINFile.C,v 1.32 2001/05/07 18:05:06 anker Exp $

#include <BALL/FORMAT/HINFile.h>
#include <BALL/CONCEPT/composite.h>
#include <BALL/KERNEL/residue.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/protein.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/PDBAtom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/PTE.h>

#include <stack>

using namespace std; // needed for endl

namespace BALL 
{

  struct HINFileBondStruct
  {
    Size        atom1;
    Size        atom2;
    Bond::Order order;
	};
 
	HINFile::HINFile()
	{
	}

	HINFile::HINFile(const String& name, File::OpenMode open_mode)
		: File(name, open_mode)
	{
	}

	HINFile::HINFile(const HINFile& file)
		: File(file)
	{
	}

	HINFile::~HINFile()
	{
	}

	void HINFile::writeAtom_(const Atom& atom, Size number, Size atom_offset)
	{
		*(File*)this << "atom " << number + 1 - atom_offset << " ";
		String name = atom.getName();
		if (name != "") 
		{
			*(File*)this << name.trim() << " ";
		} 
		else 
		{
			*(File*)this << "- ";
		}

		*(File*)this << atom.getElement().getSymbol() << " ";
		if (atom.getTypeName() == "?")
		{
			*(File*)this << "**";
		} 
		else 
		{
			*(File*)this << atom.getTypeName();	
		}
		*(File*)this << " - ";
		*(File*)this << atom.getCharge() << " ";
		*(File*)this << atom.getPosition().x << " ";
		*(File*)this << atom.getPosition().y << " ";
		*(File*)this << atom.getPosition().z << " ";

		Size number_of_bonds = 0;
		String bond_string(" ");
		const Atom*	bond_partner;

		// count the valid bonds (bonds to atoms inside the system)
		for (Position i = 0; i < atom.countBonds(); ++i) 
		{
			const Bond*	bond = atom.getBond(i);
			bond_partner = bond->getPartner(atom);

			Size index = bond_partner->getProperty("__HINFILE_INDEX").getUnsignedInt();
			if (index != 0)
			{
				number_of_bonds++;

				bond_string += String(index);

				switch (bond->getOrder()) 
				{
					case Bond::ORDER__DOUBLE:		bond_string += " d "; break;
					case Bond::ORDER__TRIPLE:		bond_string += " t "; break;
					case Bond::ORDER__AROMATIC:	bond_string += " a "; break;
					// default is single bond!
					default:										bond_string += " s ";
				}
			}
		}
		
		// write the bonds
		*(File*)this << number_of_bonds << bond_string << endl;

		// HyperChem uses A/ps, as does BALL. So, no conversion is needed.
		*(File*)this << "vel " << number + 1 - atom_offset << " " 
								 << atom.getVelocity().x << " " 
								 << atom.getVelocity().y << " "
								 << atom.getVelocity().z << endl;
	}
	
	void HINFile::write(const System& system)
	{
		// the atom_vector contains the atoms in the order of
		// the atom iterator
		vector<Atom*>		atom_vector;

		// create a vector containing pointers to the atoms
		AtomIterator	atom_it;		
		for (atom_it = system.beginAtom(); +atom_it; ++atom_it)
		{
			atom_vector.push_back(&(*atom_it));
			atom_it->setProperty("__HINFILE_INDEX", (unsigned int)atom_vector.size());
		}

		// the index_vector contains the index of the connected component
		// (HyperChem molecule) it is in and initialize it to zero
		vector<Index>		index_vector(atom_vector.size(), -1);

		typedef list<Size> Component;
		typedef	vector<Component>	ComponentVector;

		// now calculate all connected components in the graph
		// formed by atoms and bonds of the system
		// each of these connected components represents a molecule
		// for the new HyperChem file
			
		// index of the current connected component
		Size current_index = 0;

		// the index of the atom to start a new component
		Size start_index = 0;

		// search components until all atoms have been considered
		while (start_index < atom_vector.size())
		{
			while ((index_vector[start_index] >= 0) && 
						 (start_index < atom_vector.size()))
			{
				start_index++;
			}
					
			if (start_index < atom_vector.size())
			{
				// create a stack containing all atoms to be axamined for this component
				stack<Size>	atom_stack;
				
				// our start atom is the first to be considered and is marked, too
				atom_stack.push(start_index);
				index_vector[start_index] = (Index)current_index;

				// never examine this atom again as start atom
				start_index++;

				// calculate the connected component
				while (atom_stack.size() > 0)
				{
					// check all bonds of this atom
					// and remove it from the stack
					Atom& current_atom = *atom_vector[atom_stack.top()];
					atom_stack.pop();

					Atom::BondIterator	bond_it = current_atom.beginBond();
					for (; +bond_it; ++bond_it) 
					{
						// add the atom if it is not marked yet
						// ignore all bonds to atoms outside the system 
						// (these atoms have not been marked, so getProperty will return 0)
						Size atom_index = bond_it->getPartner(current_atom)
																->getProperty("__HINFILE_INDEX").getUnsignedInt();
						if ((atom_index != 0) && (index_vector[atom_index - 1] == -1))
						{
							// remember this atom in the stack
							// and mark it in the index_vector
							atom_stack.push(atom_index - 1);
							index_vector[atom_index - 1] = (Index)current_index;
						}
					}
				}
				
				// done with this component, increase the component counter
				current_index++;
			}
		}

		// create an empty vector for the connected components
		// 
		ComponentVector components(current_index);

		// now extract lists of all connected component 
		// in the order of the atom_vector to keep the
		// order of residues
		for (Size i = 0; i < atom_vector.size(); i++)
		{
			// remember the index of the atom
			components[index_vector[i]].push_back(i);
			
			// and set the atom's HINFILE_INDEX properly
			// (i.e. to the index in the right connected component
			atom_vector[i]->setProperty("__HINFILE_INDEX", 
																	(unsigned int)components[index_vector[i]].size());
		}

		// write some default header
		*(File*)this << "; HyperChem file created by BALL" << endl;
		*(File*)this << ";" << endl;
		*(File*)this << "forcefield AMBER" << endl;

		// BAUSTELLE: insert system temperature here
		*(File*)this << "sys 0" << endl;
		
		Size atom_count = 0;
		Size atom_offset = 0;

		// for each connected component: write a molecule
		for (Size j = 0; j < current_index; j++)
		{
			// renumber all atoms relative to the first atom of
			// each connected component to obtain a numbering starting
			// with 1 for each molecule
			atom_offset = atom_count;

			// try to find a name for the molecule
			Molecule* mol = atom_vector[components[j].front()]->getMolecule();
			String name = "-";
			if (mol != 0)
			{
				name = mol->getName();
				name.trim();
			}

			// write the molecule identifier	
			*(File*)this << "mol " << j + 1 << " " << name << endl;
			
			// now iterate over all atoms and write them
			Residue* current_residue = 0;

			// the residues start at zero in each molecule as do the atoms
			Size res_count = 0;			
			Component::const_iterator comp_it = components[j].begin();
			for (; comp_it != components[j].end(); comp_it++)
			{
				// counter for the residues
				Atom* this_atom = atom_vector[*comp_it];
					
				Residue* this_residue = this_atom->Composite::getAncestor(RTTI::getDefault<Residue>());
				if (this_residue != current_residue)
				{
					if (current_residue != 0)
					{
						// write and "endres" tag
						*(File*)this << "endres " << res_count << endl;
					}

					current_residue = this_residue;

					if (this_residue != 0)
					{
						res_count++;

						// write a new residue tag
						name = this_residue->getName();
						name.trim();
						if (name == "")
						{
							name = "-";
						}
						
						*(File*)this << "res " << res_count << " ";
						*(File*)this << name << " ";
						
						name = this_residue->getID();
						name.trim();
						if (name == "")
						{
							name = "-";
						}

						*(File*)this << name << " - ";
						
						// write the chain name
						Chain*	chain = this_residue->getChain();
						if ((chain != 0)) 
						{
							name = chain->getName();
							name.trim();
							
							if (name == "")
							{
								name = "-";
							}
						} 
						else 
						{
							name = "-";
						}

						*(File*)this << "-" << endl;
					}
				}

				// write the atom
				writeAtom_(*this_atom, atom_count++, atom_offset);
			}

			// if the last atom was inside a residue, write the 
			// endres tag
			if (current_residue != 0)
			{
				// write and "endres" tag
				*(File*)this << "endres " << res_count << endl;
			}

			// write endmol keyword
			*(File*)this << "endmol " << j + 1 << endl;
		}

		// clear the atom properties
		for (atom_it = system.beginAtom(); +atom_it; ++atom_it)
		{
			atom_it->clearProperty("__HINFILE_INDEX");
		}
	}

	HINFile& HINFile::operator >> (System& system)
	{
		read(system);
		return *this;
	}
 
	HINFile& HINFile::operator << (const System& system)
	{
		write(system);
		return *this;
	}
 

	void HINFile::read(System& system)
	{
		if (!isValid())
		{
			Log.error() << "HINFile is not valid, but was tried to be opened: " 
									<< getName() << endl;
			return;
		}
		
		// this is just for the paranoid...
		reopen();

		// destroy the old contents of the system
		system.destroy();
		
		// we define some states for our simple parser machine
		// legal transitions are:
		// START->IN_MOLECULE, IN_MOLECULE->IN_RESIDUE
		// IN_RESIDUE->IN_MOLECULE, IN_MOLECULE->START
		enum State 
		{
			START = 0,
			IN_MOLECULE,
			IN_RESIDUE
		};
		
		// we remember the first error that occured - we only want to 
		// log the filename once!
		bool				error = false;

		// count the lines to tell the line number in case of error
		Size				number_of_lines = 0;

		// reset some private members
		box_.a.set(0.0);
		box_.b.set(0.0);
		temperature_ = 0;

		// define a macro to print an error message for the file (only once!)
#		define ERROR_HEADER\
			if (!error) \
			{\
				Log.error() << "HINFile::read: Invalid HyperChem file: " << getName() << endl;\
				error = true;\
			}\
			Log.error() << "Line " << number_of_lines << ": "
		

		const	Size	MAX_LENGTH = 512;	
		char				buffer[MAX_LENGTH + 1];	
		String			line;
		String			tag;

		// the current state - we don't want to insert an atom if
		// we don't have created the molecule/residue to insert it into!
		State	state = START;

		// 
		Residue*	residue = 0;
		Molecule*	molecule = 0;	
		Fragment*	fragment = 0;

		// initial size: 100 atoms, all set to NULL pointer
		vector<Atom*>	atom_vector;

		vector<struct HINFileBondStruct>	bond_vector(100);

		Size	number_of_bonds = 0;

		// All <mol>s that contain <res>idues are inserted
		// as single chains into this protein
		Protein*	protein = 0;
		Chain*		chain = 0;

		while (getline(buffer, MAX_LENGTH)) 
		{
			number_of_lines++;
			line = buffer;
			line.trimLeft();

			// ignore comment lines
			if (line[0] == ';' || line == "") 
			{
				continue;
			}

			// determine the hyperchem tag: always the first word in a line
			tag = line.getField(0);

			if (tag == "atom")
			{
				if (state == IN_RESIDUE || state == IN_MOLECULE) 
				{
					Size number_of_fields = line.countFields();
					if (number_of_fields < 11) 
					{
						ERROR_HEADER << "illegal <atom> line: at least 10 arguments are required for"
												 << "<atom>!" << endl;
						continue;
					}

					Atom*	atom;
					if (state == IN_RESIDUE) 
					{
						PDBAtom*	prot_atom = new PDBAtom;
						atom = RTTI::castTo<Atom>(*prot_atom);
						residue->insert(*prot_atom);

						// check the atom flags, whther this is a PDB HETATM: 
						// any HETATM sets its residue to NON_STANDARD 
						// (compare FORMAT/PDBFile:readRecordHETATM)
						if (line.getField(5).has('h')) 
						{
							residue->clearProperty(Residue::PROPERTY__AMINO_ACID);
							residue->setProperty(Residue::PROPERTY__NON_STANDARD);
						} 
						else 
						{
							residue->setProperty(Residue::PROPERTY__AMINO_ACID);
							residue->clearProperty(Residue::PROPERTY__NON_STANDARD);
						}
					} 
					else 
					{
						atom = new Atom;
						if (molecule == 0) 
						{
							fragment->insert(*atom);
						} 
						else 
						{
							molecule->insert(*atom);
						}
					}

					atom->setName(line.getField(2));

					if (PTE[line.getField(3)] == Element::UNKNOWN)
					{
						ERROR_HEADER << "unknown element: " 
												 << line.getField(3) << endl;
						continue;
					}
					atom->setElement(PTE[line.getField(3)]);
					// set the atom radius
					atom->setRadius(PTE[line.getField(3)].getVanDerWaalsRadius());

					if (line.getField(4) == "**")
					{
						atom->setTypeName("?");
					} 
					else	
					{
						atom->setTypeName(line.getField(4));
					}

					try 
					{
						atom->setCharge(line.getField(6).toFloat());
					}
					catch (Exception::InvalidFormat)
					{
						ERROR_HEADER << "illegal charge " 
												 << line.getField(6) << endl;
						continue;
					}

					try 
					{
						atom->setPosition(Vector3(line.getField(7).toFloat(), 
																			line.getField(8).toFloat(), 
																			line.getField(9).toFloat()));
					}
					catch (Exception::InvalidFormat)
					{
						ERROR_HEADER << "illegal position (" 
												 << line.getField(7) << " / "
												 << line.getField(8) << " / " 
												 << line.getField(9) << ")"<< endl;
						continue;
					}

					Size number_of_atom_bonds;
					try 
					{
						number_of_atom_bonds = ((Size)line.getField(10).toInt());
					}
					catch (Exception::InvalidFormat)
					{
						ERROR_HEADER << "illegal number of bonds: " << line.getField(10) << endl;
						continue;
					}

					Position atom_number;
					try 
					{
						atom_number = (Position)line.getField(1).toInt();
					}
					catch (Exception::InvalidFormat)
					{
						ERROR_HEADER << "illegal atom number: " << line.getField(1) << endl;
						continue;
					}

					// store the atom pointer in the atom_vector - we need it later to create the bonds!
					// if the atom_vector is to small, grow it!
					if (atom_number >= atom_vector.size())
					{
						atom_vector.resize(atom_number * 2);
					}
					atom_vector[atom_number] = atom;

					// now iterate over all bonds and insert them into the bond_vector 
					// this table will be processed afterwards to create the bonds, as most of
					// the bound atoms are not yet created by now
					if (number_of_atom_bonds > 0)
					{
						// check whether the total number of fields is consistent
						// with the number of bonds
						if (number_of_fields != (11 + 2 * number_of_atom_bonds))
						{
							// write an error message!
							if (number_of_fields < (11 + 2 * number_of_atom_bonds))
							{
								ERROR_HEADER << "too few fields in atom line for " 
														 << number_of_atom_bonds << " bonds" << endl;
							}
							else 
							{
								ERROR_HEADER << "too many fields in atom line for " 
														 << number_of_atom_bonds << " bonds" << endl;
							}
						}
						else 
						{
							for (Position i = 0 ; i < number_of_atom_bonds; i++) 
							{ 
								if (number_of_bonds >= bond_vector.capacity())
								{
									bond_vector.resize(number_of_bonds * 2);
								}

								bond_vector[number_of_bonds].atom1 = atom_number;
								try 
								{
									bond_vector[number_of_bonds].atom2 = (Index)line.getField(11 + 2 * (Index)i).toInt();
								}
								catch (Exception::InvalidFormat)
								{
									ERROR_HEADER << "illegal atom number for bond " 
															 << i << ": " << line.getField(11 + 2 * (Index)i) << endl;
									break;
								}
								Bond::Order order = Bond::ORDER__UNKNOWN;
								String type_field = line.getField(12 + 2 * (Index)i);

								if (type_field.size() == 1)
								{
									switch (type_field[0]) 
									{
										case 's':	order = Bond::ORDER__SINGLE;		break;
										case 'd':	order = Bond::ORDER__DOUBLE;		break;
										case 't': order = Bond::ORDER__TRIPLE;		break;
										case 'a': order = Bond::ORDER__AROMATIC;	break;
									}
								}
											
								bond_vector[number_of_bonds++].order = order;
							}
						}
					}
				}	
				else 
				{
					ERROR_HEADER << "<atom> tag may appear only inside a <mol> or <res>!" << endl;
				}
				continue;
			}


			if (tag == "vel")
			{
				// read the velocity of an atom
				// since HyperChem uses the same units for velocities 
				// (resp. A/ps) we don't need a conversion
				Vector3 velocity;

				try
				{
					velocity.x = line.getField(2).toFloat();
					velocity.y = line.getField(3).toFloat();
					velocity.z = line.getField(4).toFloat();
				}
				catch (Exception::InvalidFormat)
				{
					ERROR_HEADER << "illegal velocity (" 
											 << line.getField(2) << " / "
											 << line.getField(3) << " / " 
											 << line.getField(4) << ")"<< endl;
					continue;
				}

					
				// check whether the atom exists
				Position atom_number;
				try
				{
					atom_number = (Position)line.getField(1).toInt();
				}
				catch (Exception::InvalidFormat)
				{
					ERROR_HEADER << "illegal atom number " << line.getField(1) << endl;
					continue;
				}

				if (atom_number >= atom_vector.size())
				{
					ERROR_HEADER << "cannot assign velocity for atom " << atom_number 
											 << ": atom not defined!" << endl;
					continue;
				} 

				if (atom_vector[atom_number] != 0)
				{
					atom_vector[atom_number]->setVelocity(velocity);
				} 
				else 
				{
					ERROR_HEADER << "cannot assign velocity for atom " 
											 << atom_number << ": atom not defined!" << endl;
				}
					
				continue;
			}


			if (tag == "res")
			{
				// remember where we are.
				if (state != IN_MOLECULE) 
				{
					ERROR_HEADER << "<res> tag must be inside a <mol>/<endmol>" << endl;
				}

				state = IN_RESIDUE;
				
				// create a protein if it doesn't exist already
				if (protein == 0)
				{
					system.insert(*(protein = new Protein));
				}

				// check whether we already have a chain to insert into
				if (chain == 0)
				{
					protein->insert(*(chain = new Chain));
				}

				// create a new residue and insert it into the chain
				residue = new Residue;
				chain->insert(*residue);

				// set the residue's name
				if (line.getField(2) != "-")
				{
					residue->setName(line.getField(2));
				}

				// set the residue's PDB ID
				if (line.getField(3) != "-")
				{
					residue->setID(line.getField(3));
				}

				// set the chain name 
				if (line.getField(5) != "-")
				{
					chain->setName(line.getField(5));
				}
				
				// create a fragment to insert the "loose" atoms into
				if (fragment == 0)
				{
					chain->AtomContainer::insert(*(fragment = new Fragment));
				}

				// now check for a molecule, that might already exist
				// and move its atoms into a fragment (to be inserted into the chain)
				if (molecule == 0)
				{
					continue;
				}

				AtomIterator atom_it = molecule->beginAtom();
				vector<Atom*>	tmp_atoms(20);
				for (; +atom_it; ++atom_it)
				{
					tmp_atoms.push_back(&*atom_it);
				}
				for (Size i = 0; i < tmp_atoms.size(); ++i)
				{
					fragment->insert(*tmp_atoms[i]);
				}
				
				// delete the (now empty!) molecule and clear the pointer
				system.remove(*molecule);
				delete molecule;
				molecule = 0;
				continue;
			}


			if (tag == "endres")
			{
				if (state != IN_RESIDUE) 
				{
					ERROR_HEADER << "<endres> without <res>!" << endl;
				}
				
				state = IN_MOLECULE;

				// reset the residue pointer to zero
				residue = 0;
				
				continue;
			}


			if (tag == "mol")
			{
				if (state != START) 
				{
					ERROR_HEADER << "<mol> inside <mol> or <res> definition!" << endl;
				}

				state = IN_MOLECULE;

				// create a new molecule and insert it into the system.
				// We do not yet know, whether this contains residues.
				// If it does, we have to convert it to a protein afterwards.
				system.insert(*(molecule = new Molecule));

				if (line.countFields() > 2)
				{
					String name = line.getField(2);
					if ((name != "") && (name != "-"))
					{
						molecule->setName(name);
					}	
				}
				
				continue;
			}

			if (tag == "endmol")
			{
				if (state != IN_MOLECULE)
				{
					ERROR_HEADER << "missing <mol> or <endres> tag!" << endl;
				}

				state = START;
				
				if (fragment != 0)
				{
					if (fragment->countAtoms() == 0)
					{
						chain->AtomContainer::remove(*fragment);
					}
				}

				fragment = 0;
				chain = 0;
				molecule = 0;

				// now build all bonds
				for (Size i = 0; i < number_of_bonds; i++)
				{
					// check whether both atoms exist
					if (bond_vector[i].atom1 >= atom_vector.size() || 
							bond_vector[i].atom2 >= atom_vector.size())
					{
						// complain if one of the atoms does not exist
						Log.error() << "HINFile: cannot create bond from atom " << bond_vector[i].atom1 
												<< " to atom " << bond_vector[i].atom2 << " of molecule " 
												<< line.getField(1) << " - non-existing atom!" << endl;
					} 
					else  
					{
						// everything all right, create the bond
						Bond* b =  atom_vector[bond_vector[i].atom1]->createBond(*atom_vector[bond_vector[i].atom2]);
					
						b->setOrder(bond_vector[i].order);
					}
				}

				number_of_bonds = 0;

				continue;
			}


			if (tag == "sys")
			{
				// set the system's temperature
				try
				{
					temperature_ = line.getField(2).toFloat();
				}
				catch (Exception::InvalidFormat)
				{
					ERROR_HEADER << "illegal temperature " << line.getField(2) << endl;
					continue;
				}

				continue;
			}


			if (tag == "box")
			{
				// retrieve the periodic boundary
				// we assume that the box is centered about the origin
				// of the coordinate system
				// The manual says the parameters are the dimensions of the box,
				// so we have to divide by two.
				try
				{
					box_.a.x = - line.getField(1).toFloat() / 2.0;
					box_.a.y = - line.getField(2).toFloat() / 2.0;
					box_.a.z = - line.getField(3).toFloat() / 2.0;
				}
				catch (Exception::InvalidFormat)
				{
					ERROR_HEADER << "illegal box position (" 
											 << line.getField(1)  << " / "
											 << line.getField(2)  << " / "
											 << line.getField(3)  << " )" << endl;
					continue;
				}		

				box_.b.x = - box_.a.x;
				box_.b.y = - box_.a.y;
				box_.b.z = - box_.a.z;
				continue;
			}


			// ignore the irrelevant fields
			if (tag == "forcefield" ||
					tag == "user1color" ||
					tag == "user2color" ||
					tag == "user3color" ||
					tag == "user4color" ||
					tag == "view"				||
					tag == "seed"				||
					tag == "mass"				||
					tag == "basisset"		||
					tag == "selection"	||
					tag == "endselection"		 ||
					tag == "selectrestraint" ||
					tag == "selectatom"			 ||
					tag == "formalcharge")
			{
				continue;
			}	

			// if the tag was not recognized: complain about it
			Log.warn() << "HINFile: unknown tag " << tag << " ignored." << endl;
		}

		// if desired, try to remove the lone pairs from old AMBER HC-Files 
		if (true) // BAUSTELLE
		{
			// a list to hold the lone pairs (for deletion)
			list<Atom*> del_list;
			
			// iterate over all atoms
			AtomIterator it = system.beginAtom();
			for (; +it; ++it)
			{
				if (it->getElement().getSymbol() == "Lp")
				{
					// store lone pair in the del_list
					del_list.push_back(&*it);
					
					// sum the lone pair charge into the
					// heavy atom it is bound to
					if (it->countBonds() > 0)
					{
						float charge = it->getCharge() / (float)it->countBonds();
						for (Atom::BondIterator bond_it = it->beginBond(); +bond_it; ++bond_it)
						{
							Atom* partner = bond_it->getPartner(*it);
							if (partner != 0)
							{
								partner->setCharge(partner->getCharge() + charge);
							}
						}
					}

					// remove all bonds to the lone pair
					it->destroyBonds();
				}
			}
			
			// remove the lone pairs
			list<Atom*>::iterator list_it = del_list.begin();
			for (; list_it != del_list.end(); ++list_it)
			{
				if ((*list_it)->isAutoDeletable())
				{
					// delete dynamically created objects
					delete *list_it;
				} 
				else 
				{
					// destroy static atoms
					(*list_it)->destroy();
				}
			}
		}
	}

	bool HINFile::hasPeriodicBoundary() const
	{
		return (box_.a != box_.b);
	}

	Box3 HINFile::getPeriodicBoundary() const
	{
		return box_;
	}

	float HINFile::getTemperature() const
	{
		return temperature_;
	}

} // namespace BALL
