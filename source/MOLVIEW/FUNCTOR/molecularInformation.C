// $Id: molecularInformation.C,v 1.2 2000/07/18 08:29:58 oliver Exp $

#include <BALL/MOLVIEW/FUNCTOR/molecularInformation.h>

using namespace std;

namespace BALL
{

	namespace MOLVIEW
	{

		MolecularInformation::MolecularInformation()
			:	Information()
		{
		}

		MolecularInformation::MolecularInformation
			(const MolecularInformation& info, bool deep)
			:	Information(info, deep),
				type_(TYPE__UNKNOWN)
		{
		}

		MolecularInformation::~MolecularInformation()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<MolecularInformation>() << endl;
			#endif 

			destroy();
		}

		void MolecularInformation::clear()
		{
			Information::clear();
			type_ = TYPE__UNKNOWN;
		}

		void MolecularInformation::destroy()
		{
			clear();
		}

		void MolecularInformation::set
			(const MolecularInformation& info,
			 bool deep)
		{
			Information::set(info, deep);
			type_ = info.type_;
		}

		MolecularInformation& 	MolecularInformation::operator =
			(const MolecularInformation& info)
		{
			set(info);

			return *this;
		}

		void MolecularInformation::get
			(MolecularInformation& info, bool deep) const
		{
			info.set(*this, deep);
		}

		void MolecularInformation::swap
			(MolecularInformation& info)
		{
			Information::swap(info);

			Type temp = type_;
			type_ = info.type_;
			info.type_ = temp;
		}

		void MolecularInformation::visit(Composite& composite)
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

		bool MolecularInformation::isValid() const
		{
			return (bool)(Information::isValid());
		}

		void MolecularInformation::dump
			(ostream& s, Size depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BALL_DUMP_DEPTH(s, depth);
			Information::dump(s, depth + 1);
					
			BALL_DUMP_STREAM_SUFFIX(s);
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
					BaseFragment* base_fragment = RTTI::castTo<BaseFragment>(composite);
					temp = base_fragment->getName();
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
