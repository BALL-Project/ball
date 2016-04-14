// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/molecularInformation.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/bond.h>

using namespace std;

namespace BALL
{
	MolecularInformation::MolecularInformation()
		
		:	Visitor<Composite>()
	{
	}

	MolecularInformation::~MolecularInformation()
		
	{
		#ifdef BALL_DEBUG
			Log.info() << "Destructing object " << (void *)this 
								 << " of class " << RTTI::getName<MolecularInformation>() << std::endl;
		#endif 
	}

	void MolecularInformation::clear()
		
	{
		name_ = "unkown";
		type_name_ = "unkown type";
		type_ = TYPE__UNKNOWN;
	}

	void MolecularInformation::visit(Composite& composite)
	{
		getType_(composite);
		getTypeName_();
		getName_(composite);
	}

	void MolecularInformation::getType_(Composite& composite)
	{
		type_ = TYPE__UNKNOWN;

        if (RTTI::isKindOf<Bond>(&composite))
		{
			type_ = TYPE__BOND;
		}			
        else if (RTTI::isKindOf<System>(&composite))
		{
			type_ = TYPE__SYSTEM;
		}	
        else if (RTTI::isKindOf<Protein>(&composite))
		{
			type_ = TYPE__PROTEIN;
		}
        else if (RTTI::isKindOf<Molecule>(&composite))
		{
			type_ = TYPE__MOLECULE;
		}
        else if (RTTI::isKindOf<Chain>(&composite))
		{
			type_ = TYPE__CHAIN;
		}	
        else if (RTTI::isKindOf<SecondaryStructure>(&composite))
		{
			type_ = TYPE__SECONDARY_STRUCTURE;
		}	
        else if (RTTI::isKindOf<Residue>(&composite))
		{
			type_ = TYPE__RESIDUE;
		}	
        else if (RTTI::isKindOf<Fragment>(&composite))
		{
			type_ = TYPE__FRAGMENT;
		}	
        else if (RTTI::isKindOf<Atom>(&composite))
		{
			type_ = TYPE__ATOM;
		}	
	}

	void MolecularInformation::getTypeName_()
	{
		type_name_ = "unkown type";
		switch(type_)
		{
			case TYPE__PROTEIN:
				type_name_ = "Protein";
				break;

			case TYPE__MOLECULE:
				type_name_ = "Molecule";
				break;
				
			case TYPE__SYSTEM:
				type_name_ = "System";
				break;
				
			case TYPE__FRAGMENT:
				type_name_ = "Fragment";
				break;
				
			case TYPE__CHAIN:
				type_name_ = "Chain";
				break;
				
			case TYPE__RESIDUE:
				type_name_ = "Residue";
				break;
				
			case TYPE__SECONDARY_STRUCTURE:
				type_name_ = "SecondaryStructure";
				break;
				
			case TYPE__ATOM:
				type_name_ = "Atom";
				break;

			case TYPE__BOND:
				type_name_ = "Bond";
				break;
				
			default:
				type_name_ = "unknown";
				break;
		}
	}

	String MolecularInformation::getBondAtomName_(Atom* atom)
	{
		if (atom == 0) return "?";

		MolecularInformation info;
		info.visit(*atom);

		String temp(info.getName());
		if (atom->getParent() != 0 &&
                RTTI::isKindOf<Residue>(atom->getParent()))
		{
			info.visit(*atom->getParent());
			temp = info.getName() + " : " + temp;
		}

		return temp;
	}

	void MolecularInformation::getName_(Composite& composite)
	{
		String temp = "UNKNOWN";
		
		switch(type_)
		{
			case TYPE__BOND:
			{
				temp.clear();

				Bond* bond = RTTI::castTo<Bond>(composite);
				Atom* a1 = (Atom*) bond->getFirstAtom();
				Atom* a2 = (Atom*) bond->getSecondAtom();

				temp = getBondAtomName_(a1) + " -> " + getBondAtomName_(a2);
			}
			break;

			case TYPE__SYSTEM:
			{
				System* system = RTTI::castTo<System>(composite);
				temp = system->getName(); 			
			}
			break;
			
			case TYPE__RESIDUE:
			{
				Residue* residue = RTTI::castTo<Residue>(composite);
				temp = residue->getName();
				temp += " ";
				temp += residue->getID();
			}
			break;
			
			case TYPE__MOLECULE:
			case TYPE__PROTEIN:
			case TYPE__CHAIN:
			case TYPE__FRAGMENT:
			case TYPE__SECONDARY_STRUCTURE:
			{
				AtomContainer* atom_container = RTTI::castTo<AtomContainer>(composite);
				temp = atom_container->getName();
			}
			break;

			case TYPE__ATOM:
			{
				Atom* atom = RTTI::castTo<Atom>(composite);
				temp = atom->getName();
			}
			break;
			
			default:
				break;
		}
		
		if (type_ == TYPE__SECONDARY_STRUCTURE)
		{
			const SecondaryStructure* ss = dynamic_cast<SecondaryStructure*>(&composite);
			if (ss != 0)
			{
				switch (ss->getType())
				{
					case SecondaryStructure::HELIX:
						temp += " Helix"; 
						break;
					case SecondaryStructure::TURN:
						temp += " Turn";
						break;
					case SecondaryStructure::STRAND:
						temp += " Strand";
						break;
					case SecondaryStructure::COIL:
						temp += " Coil";
						break;
					default:
						temp += " Unknown";
				}
			}
			else
			{
				temp += " Unknown";
			}
		}
		else
		{
			if (temp == "")
			{
				temp = "<";
				temp += type_name_;
				temp += ">";		
			}
		}
		
		name_ = temp;
	}

} // namespace BALL
