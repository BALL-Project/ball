// $Id: molecularInformation.C,v 1.6 2001/05/13 15:02:39 hekl Exp $

#include <BALL/MOLVIEW/FUNCTOR/molecularInformation.h>

using namespace std;

namespace BALL
{

	namespace MOLVIEW
	{

		MolecularInformation::MolecularInformation()
			throw()
			:	Information()
		{
		}

		MolecularInformation::~MolecularInformation()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<MolecularInformation>() << endl;
			#endif 

			destroy();
		}

		void MolecularInformation::clear()
			throw()
		{
			Information::clear();
			type_ = TYPE__UNKNOWN;
		}

		void MolecularInformation::destroy()
			throw()
		{
		}

		void MolecularInformation::visit(Composite& composite)
			throw()
		{
			getType_(composite);

			// type is a molecular type => get Information
			if (type_ != TYPE__UNKNOWN)
			{
				getTypeName_(composite);
				getName_(composite);
			}
			else // try your luck within View-objects
			{
				Information::visit(composite);
			}
		}

	  void MolecularInformation::getType_(Composite& composite)
    {
			MolecularInformation::Type type = TYPE__UNKNOWN;
			
			if (RTTI::isKindOf<System>(composite))
			{
				type = TYPE__SYSTEM;
			}	
			else if (RTTI::isKindOf<Protein>(composite))
			{
				type = TYPE__PROTEIN;
			}
			else if (RTTI::isKindOf<Molecule>(composite))
			{
				type = TYPE__MOLECULE;
			}
			else if (RTTI::isKindOf<Chain>(composite))
			{
				type = TYPE__CHAIN;
			}	
			else if (RTTI::isKindOf<SecondaryStructure>(composite))
			{
				type = TYPE__SECONDARY_STRUCTURE;
			}	
			else if (RTTI::isKindOf<Residue>(composite))
			{
				type = TYPE__RESIDUE;
			}	
			else if (RTTI::isKindOf<Fragment>(composite))
			{
				type = TYPE__FRAGMENT;
			}	
			else if (RTTI::isKindOf<Atom>(composite))
			{
				type = TYPE__ATOM;
			}	

			type_ = type;
    }

	  void MolecularInformation::getTypeName_(Composite& /* composite */)
    {
			String temp;
			
			switch(type_)
			{
			  case TYPE__PROTEIN:
					temp = "Protein";
					break;

			  case TYPE__MOLECULE:
					temp = "Molecule";
					break;
					
			  case TYPE__SYSTEM:
					temp = "System";
					break;
					
			  case TYPE__FRAGMENT:
					temp = "Fragment";
					break;
					
			  case TYPE__CHAIN:
					temp = "Chain";
					break;
					
  			case TYPE__RESIDUE:
					temp = "Residue";
					break;
					
			  case TYPE__SECONDARY_STRUCTURE:
					temp = "SecondaryStructure";
					break;
					
  			case TYPE__ATOM:
					temp = "Atom";
					break;
					
  			default:
					temp = "unknown";
					break;
			}

			type_name_ = temp;
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
			
			// empty string
			if (temp == "")
			{
				temp = "<";
				temp += type_name_;
				temp += ">";		
			}
			
			name_ = temp;
    }

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/FUNCTOR/molecularInformation.iC>
#		endif

	} // namespace MOLVIEW

} // namespace BALL
