// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: molecularInformation.C,v 1.3 2004/02/23 16:27:26 oliver Exp $
//

#include <BALL/VIEW/KERNEL/molecularInformation.h>
#include <BALL/KERNEL/system.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

		MolecularInformation::MolecularInformation()
			throw()
			:	Visitor<Composite>()
		{
		}

		MolecularInformation::~MolecularInformation()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << (void *)this 
				 					 << " of class " << RTTI::getName<MolecularInformation>() << std::endl;
			#endif 
		}

		void MolecularInformation::clear()
			throw()
		{
			name_ = "unkown";
			type_name_ = "unkown type";
			type_ = TYPE__UNKNOWN;
		}

		void MolecularInformation::visit(Composite& composite)
		{
			getType_(composite);
			getTypeName_(composite);
			getName_(composite);
		}

	  void MolecularInformation::getType_(Composite& composite)
    {
			type_ = TYPE__UNKNOWN;
			
			if (RTTI::isKindOf<System>(composite))
			{
				type_ = TYPE__SYSTEM;
			}	
			else if (RTTI::isKindOf<Protein>(composite))
			{
				type_ = TYPE__PROTEIN;
			}
			else if (RTTI::isKindOf<Molecule>(composite))
			{
				type_ = TYPE__MOLECULE;
			}
			else if (RTTI::isKindOf<Chain>(composite))
			{
				type_ = TYPE__CHAIN;
			}	
			else if (RTTI::isKindOf<SecondaryStructure>(composite))
			{
				type_ = TYPE__SECONDARY_STRUCTURE;
			}	
			else if (RTTI::isKindOf<Residue>(composite))
			{
				type_ = TYPE__RESIDUE;
			}	
			else if (RTTI::isKindOf<Fragment>(composite))
			{
				type_ = TYPE__FRAGMENT;
			}	
			else if (RTTI::isKindOf<Atom>(composite))
			{
				type_ = TYPE__ATOM;
			}	
    }

	  void MolecularInformation::getTypeName_(Composite& /* composite */)
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
					
  			default:
					type_name_ = "unknown";
					break;
			}
    }

	  void MolecularInformation::getName_(Composite& composite)
    {
			String temp = "UNKNOWN";
			
			switch(type_)
			{
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
				const SecondaryStructure& ss = *(SecondaryStructure*) &composite;
				if (ss.hasProperty(SecondaryStructure::PROPERTY__HELIX))
				{
					temp += " Helix";
				}
				else if (ss.hasProperty(SecondaryStructure::PROPERTY__TURN))
				{
					temp += " Turn";
				}
				else if (ss.hasProperty(SecondaryStructure::PROPERTY__STRAND))
				{
					temp += " Strand";
				}
				else if (ss.hasProperty(SecondaryStructure::PROPERTY__COIL))
				{
					temp += " Random Coil";
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

	} // namespace VIEW
} // namespace BALL
