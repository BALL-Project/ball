// $Id: atom.C,v 1.33 2001/06/14 00:30:12 oliver Exp $

#include <BALL/KERNEL/atom.h>

#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/fragment.h>
#include <BALL/KERNEL/residue.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/PTE.h> 

using namespace std;

namespace BALL 
{

	Atom::Atom()
		throw()
		:	Composite(),
			PropertyManager(),
			element_(BALL_ATOM_DEFAULT_ELEMENT),
			charge_(BALL_ATOM_DEFAULT_CHARGE),
			name_(BALL_ATOM_DEFAULT_NAME),
			type_name_(BALL_ATOM_DEFAULT_TYPE_NAME),
			position_(BALL_ATOM_DEFAULT_POSITION),
			radius_(BALL_ATOM_DEFAULT_RADIUS),
			type_(BALL_ATOM_DEFAULT_TYPE),
			velocity_(BALL_ATOM_DEFAULT_VELOCITY),
			force_(BALL_ATOM_DEFAULT_FORCE),
			number_of_bonds_(0)
	{
	}
		
	Atom::Atom(const Atom& atom, bool deep)
		throw()
		:	Composite(atom, deep),
			PropertyManager(atom),
			element_(atom.element_),
			charge_(atom.charge_),
			name_(atom.name_),
			type_name_(atom.type_name_),
			position_(atom.position_),
			radius_(atom.radius_),
			type_(atom.type_),
			velocity_(atom.velocity_),
			force_(atom.force_),
			number_of_bonds_(0)
	{
	}

	Atom::Atom(Element& element, const String& name,
						 const String& type_name, Atom::Type type,
						 const Vector3& position, const Vector3& velocity,
						 const Vector3& force, float charge, float radius)
		throw()
		:	Composite(),
			PropertyManager(),
			element_(&element),
			charge_(charge),
			name_(name),
			type_name_(type_name),
			position_(position),
			radius_(radius),
			type_(type),
			velocity_(velocity),
			force_(force),
			number_of_bonds_(0)
	{
	}
		
	Atom::~Atom()
		throw()
	{
		destroy();
	}

	void Atom::clear()
		throw()
	{
		Composite::clear();
		PropertyManager::clear();

		clear_();
	}
		
	void Atom::destroy()
		throw()
	{
		Composite::destroy();
		PropertyManager::destroy();

		clear_();
	}

	void Atom::persistentWrite(PersistenceManager& pm, const char* name) const
		throw()
	{
		pm.writeObjectHeader(this, name);

			Composite::persistentWrite(pm);

			pm.writeStorableObject(*(PropertyManager*)this, "PropertyManager");

			pm.writePrimitive((String)element_->getSymbol(), "element_");
			pm.writePrimitive(charge_, "charge_");
			pm.writePrimitive(radius_, "radius_");
			pm.writePrimitive(name_, "name_");
			pm.writePrimitive(type_name_, "type_name_");
			pm.writePrimitive((Index)type_, "type_");

			position_.persistentWrite(pm, "position_");
			velocity_.persistentWrite(pm, "velocity_");
			force_.persistentWrite(pm, "force_");

			pm.writePrimitive(number_of_bonds_, "number_of_bonds_");
			pm.writeObjectPointerArray((Bond**)bond_, "bond_", (Size)number_of_bonds_);
		pm.writeObjectTrailer(name);
	}

	void Atom::persistentRead(PersistenceManager& pm)
		throw()
	{
		pm.checkObjectHeader(RTTI::getStreamName<Composite>());
			Composite::persistentRead(pm);
		pm.checkObjectTrailer(0);

		pm.readStorableObject(*(PropertyManager*)this, "PropertyManager");

		String s;
		pm.readPrimitive(s, "element_");	
		element_ = &PTE[s];
		pm.readPrimitive(charge_, "charge_");
		pm.readPrimitive(radius_, "radius_");
		pm.readPrimitive(name_, "name_");
		pm.readPrimitive(type_name_, "type_name_");
		Index tmp_type;
		pm.readPrimitive(tmp_type, "type_");
		type_ = (Atom::Type)tmp_type;

		pm.checkObjectHeader(position_, "position_");
			position_.persistentRead(pm);
		pm.checkObjectTrailer("position_");

		pm.checkObjectHeader(velocity_, "velocity_");
			velocity_.persistentRead(pm);
		pm.checkObjectTrailer("velocity_");

		pm.checkObjectHeader(force_, "force_");
			force_.persistentRead(pm);
		pm.checkObjectTrailer("force_");

		pm.readPrimitive(number_of_bonds_, "number_of_bonds_");
		Size n;
		pm.readObjectPointerArray(bond_, "bond_", n);
		if (n != (Size)number_of_bonds_)
		{
			Log.level(LogStream::ERROR) << "Atom::persistentRead: size of bond array: read " << n
				<< " instead of " << number_of_bonds_ << endl;
		}
	}

  void Atom::set(const Atom& atom, bool deep)
    throw()
  {
    Composite::set(atom, deep);
    PropertyManager::operator =(atom);
    
    element_ = atom.element_;
    charge_ = atom.charge_;
    name_ = atom.name_;
    type_name_ = atom.type_name_;
    position_.set(atom.position_);
    radius_ = atom.radius_;
    type_ = atom.type_;
    velocity_.set(atom.velocity_);
    force_.set(atom.force_);
    number_of_bonds_ = 0;
  }

  void Atom::get(Atom &atom, bool deep) const
    throw()
  {
    atom.set(*this, deep);
  }

	const Atom& Atom::operator =(const Atom &atom)
		throw()
	{
		Composite::operator =(atom);
		PropertyManager::operator =(atom);
		
		element_ = atom.element_;
		charge_ = atom.charge_;
		name_ = atom.name_;
		type_name_ = atom.type_name_;
		position_.set(atom.position_);
		radius_ = atom.radius_;
		type_ = atom.type_;
		velocity_.set(atom.velocity_);
		force_.set(atom.force_);
		number_of_bonds_ = 0;

		return *this;
	}
			
	bool Atom::operator == (const Atom& atom) const
		throw()
	{
		return(Object::operator ==(atom));
	}

	bool Atom::operator != (const Atom& atom) const
		throw()
	{
		return ! (*this == atom);
	}

	void Atom::swap(Atom &atom)
		throw()
	{
		Composite::swap(atom);
		PropertyManager::swap(atom);

		const Element *temp_element = element_;
		element_ = atom.element_;
		atom.element_ = temp_element;
		
		float temp = charge_;
		charge_ = atom.charge_;
		atom.charge_ = temp;

		name_.swap(atom.name_);
		type_name_.swap(atom.type_name_);

		position_.swap(atom.position_);

		temp = radius_;
		radius_ = atom.radius_;
		atom.radius_ = temp;

		Type temp_type = type_;
		type_ = atom.type_;
		atom.type_ = temp_type;

		velocity_.swap(atom.velocity_);
		force_.swap(atom.force_);

		Bond *temp_bond = 0;
		for (int i = 0; i < MAX_NUMBER_OF_BONDS;++i)
		{
			temp_bond = bond_[i];
			bond_[i] = atom.bond_[i];
			atom.bond_[i] = temp_bond;
		}

		char temp_c = number_of_bonds_;
		number_of_bonds_ = atom.number_of_bonds_;
		atom.number_of_bonds_ = temp_c;
	}
		
	Molecule* Atom::getMolecule()
		throw()
	{
		return Composite::getAncestor(RTTI::getDefault<Molecule>());
	}

	Fragment* Atom::getFragment()
		throw()
	{
		return Composite::getAncestor(RTTI::getDefault<Fragment>());
	}
					                                                                                                                              
	const Residue* Atom::getResidue() const
		throw()
	{
		return Composite::getAncestor(RTTI::getDefault<Residue>());
	}
					                                                                                                                              
	String Atom::getFullName(Atom::FullNameType type) const
		throw()
	{
		// determine the parent`s name
		String parent_name;
		const Residue* parent = getAncestor(RTTI::getDefault<Residue>());
		if (parent == 0)
		{
			// we don't have a residue, look for a fragment
			if (getFragment() != 0)
			{
				parent_name = getFragment()->getName();
				parent_name.trim();
				parent_name += ":";
			}
			// we don't have a fragment either, look for molecule
			else if (getMolecule() != 0)
			{
				parent_name = getMolecule()->getName();
				parent_name.trim();
				parent_name += ":";
			}
		} 
		else 
		{
			// retrieve the fragment name
			parent_name = parent->getFullName((Residue::FullNameType)type) + ":";
		}

		// retrieve the atom name
		String name = name_;
		name.trim();

		// add the parnet name only if non-empty
		if (parent_name != ":")
		{
			name = parent_name + name;
		}

		return name;
	}

	Size Atom::countBonds() const
		throw()
	{
		return number_of_bonds_;
	}

	Bond* Atom::getBond(Position index)
		throw(Exception::IndexOverflow)
	{
		if (index >= (Index)MAX_NUMBER_OF_BONDS)
		{
			throw Exception::IndexOverflow(__FILE__, __LINE__, (Index)index, (Size)MAX_NUMBER_OF_BONDS);
		}

		return ((Size)index < number_of_bonds_) ? bond_[index] : 0;
	}

	const Bond* Atom::getBond(Position index) const
		throw(Exception::IndexOverflow)
	{
		return (const Bond *)((Atom *)this)->getBond(index);
	}

	Bond* Atom::getBond(const Atom &atom)
		throw()
	{
		if (&atom != this)
		{
			for (int i = 0; i < number_of_bonds_; ++i)
			{
				if (bond_[i]->first_ == &atom || bond_[i]->second_ == &atom)
				{
					return bond_[i];
				}
			}
		}

		return 0;
	}

	const Bond* Atom::getBond(const Atom &atom) const
		throw()
	{
		return ((Atom *)this)->getBond(atom);
	}

	Bond* Atom::createBond(Atom &atom)
		throw()
	{
		Bond* bond = getBond(atom);
		
		if (bond == 0)
		{
			return Bond::createBond(*new Bond, *this, atom);
		} 
		else 
		{
			return bond;
		}
	}

	Bond* Atom::createBond(Bond& bond, Atom& atom)
		throw()
	{
		return Bond::createBond(bond, *this, atom);
	}

	Bond* Atom::cloneBond(Bond& bond, Atom& atom)
		throw()
	{
		Bond* bond_ptr = getBond(atom);
		
		if (bond_ptr == 0)
		{
			bond_ptr = (Bond *)bond.create();
			bond_ptr->setFirstAtom(0);
			bond_ptr->setSecondAtom(0);
			
			return Bond::createBond(*bond_ptr, *this, atom);
		} 
		else 
		{
			return bond_ptr;
		}
	}

	bool Atom::destroyBond(const Atom& atom)
		throw()
	{
		Bond* bond = getBond(atom);

		if (bond == 0)
		{
			return false;
		}
			
		if (bond->isAutoDeletable() == true)
		{
			delete bond;
		} 
		else 
		{
			bond->destroy();
		}

		return true;
	}

	void Atom::destroyBonds()
		throw()
	{
		for (int i = char(number_of_bonds_) - 1; i >= 0; --i)
		{
			if (bond_[i]->isAutoDeletable() == true)
			{
				delete bond_[i];
			} 
			else 
			{
				bond_[i]->destroy();
			}
		}
		
		number_of_bonds_ = 0;
	}

	bool Atom::hasBond(const Bond& bond) const
		throw()
	{
		for (int i = 0; i < number_of_bonds_; ++i)
		{
			if (bond_[i] == &bond)
			{
				return true;
			}
		}
		return false;
	}

	bool 	Atom::isBoundTo(const Atom& atom) const
		throw()
	{
		return (getBond(atom) != 0);
	}

	bool Atom::isBound() const
		throw()
	{
		return (number_of_bonds_ > 0);
	}

	bool Atom::isGeminal(const Atom& atom) const
		throw()
	{
		// an atom is geminal to another, if it 
		// is not directly bonded to it
		bool is_geminal = !isBoundTo(atom);

		// second, it has to be bonded to an atom
		// that is bonded to *this atom, too
		if (is_geminal)	
		{
			is_geminal = false;
			for (Size i = 0; (i < countBonds()) && !is_geminal; i++)
			{
				Atom* partner_a = getBond(i)->getPartner(*this);
				if (partner_a != 0)
				{
					if (partner_a->isBoundTo(atom))
					{
						is_geminal = true;
						break;
					}
				}
			}
		}

		return is_geminal;
	}

	bool Atom::isVicinal(const Atom& atom) const
		throw()
	{ 
		// an atom is vicinal to another, if it 
		// is not directly bonded to it
		bool is_vicinal = !isBoundTo(atom);

		// second, it has to be bonded to an atom
		// that is bonded to and atom that is bonded 
		// to *this atom
		if (is_vicinal)	
		{
			is_vicinal = false;
			for (Size i = 0; (i < countBonds()) && !is_vicinal; i++)
			{
				Atom* partner_a = getBond(i)->getPartner(*this);
				if (partner_a != 0)
				{
					for (Size j = 0; (j < partner_a->countBonds()) && !is_vicinal; j++)
					{
						Atom* partner_b = partner_a->getBond(j)->getPartner(*partner_a);
						if ((partner_b) != 0 && (partner_b->isBoundTo(atom)))
						{
							is_vicinal = true;
							break;
						}
					}
				}
			}
		}

		return is_vicinal;
	}

	bool Atom::isValid() const
		throw()
	{ 
		if (Composite::isValid() == false
				|| PropertyManager::isValid() == false
				|| element_ == 0
				|| position_.isValid() == false
				|| force_.isValid() == false
				|| velocity_.isValid() == false)
		{
			return false;
		}

		for (int i = 0; i < number_of_bonds_; ++i)
		{
			if (bond_[i]->isValid() == false)
			{
				return false;
			}
			if (bond_[i]->first_ != this && bond_[i]->second_ != this)
			{
				return false;
			}
		}

		return true;
	}

	void Atom::dump(ostream& s, Size depth) const
		throw()
	{
		BALL_DUMP_STREAM_PREFIX(s);
		
		Composite::dump(s, depth);

		BALL_DUMP_DEPTH(s, depth);
		s << "  element: " << *element_ << endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "  charge: " << charge_ << endl;
		
		BALL_DUMP_DEPTH(s, depth);
		s << "  name: " << name_ << endl;
		
		BALL_DUMP_DEPTH(s, depth);
		s << "  type name: " << type_name_ << endl;
		
		BALL_DUMP_DEPTH(s, depth);
		s << "  position: " << position_ << endl;
		
		BALL_DUMP_DEPTH(s, depth);
		s << "  radius: " << radius_ << endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "  type: " << type_ << endl;
		
		BALL_DUMP_DEPTH(s, depth);
		s << "  velocity: " << velocity_ << endl;
		
		BALL_DUMP_DEPTH(s, depth);
		s << "  force: " << force_ << endl;
		
		BALL_DUMP_DEPTH(s, depth);
		s << "  number of bonds: " << (int)number_of_bonds_ << endl;
		
		BALL_DUMP_DEPTH(s, depth);
		s << "  bonds: " << endl;

		for (int i = 0; i < number_of_bonds_; ++i)
		{
			bond_[i]->dump(s, depth + 1);
		}
		s << endl;

		BALL_DUMP_STREAM_SUFFIX(s);
	}

	void Atom::read(istream & /* s */)
		throw()
	{
		throw Exception::NotImplemented(__FILE__, __LINE__);
	}

	void Atom::write(ostream & /* s */) const
		throw()
	{
		throw Exception::NotImplemented(__FILE__, __LINE__);
	}

	bool Atom::applyBonds(UnaryProcessor<Bond>& processor)
		throw()
	{
		if (processor.start() == false)
		{
			return false;
		}
		Processor::Result result;

		for (int i = 0; i < number_of_bonds_; ++i)
		{
			result = processor(*bond_[i]);

			if (result <= Processor::BREAK)
			{
				return (result == Processor::BREAK) ? true : false;
			}
		}

		return processor.finish();
	}

	void Atom::clear_()
		throw()
	{
		element_ = BALL_ATOM_DEFAULT_ELEMENT;
		charge_ = BALL_ATOM_DEFAULT_CHARGE;
		name_ = BALL_ATOM_DEFAULT_NAME;
		type_name_ = BALL_ATOM_DEFAULT_TYPE_NAME;
		position_.set(BALL_ATOM_DEFAULT_POSITION);
		radius_ = BALL_ATOM_DEFAULT_RADIUS;
		type_ = BALL_ATOM_DEFAULT_TYPE;
		velocity_.set(BALL_ATOM_DEFAULT_VELOCITY);
		force_.set(BALL_ATOM_DEFAULT_FORCE);
				 
		destroyBonds();
	}
		
	void Atom::swapLastBond_(const Atom *atom)
		throw()
	{
		for (int i = 0; i < number_of_bonds_; ++i)
		{
			if (bond_[i]->first_ == atom
					|| bond_[i]->second_ == atom)
			{
				bond_[i] = bond_[--number_of_bonds_];
				break;
			}
		}
	}

# ifdef BALL_NO_INLINE_FUNCTIONS
#   include <BALL/KERNEL/atom.iC>
# endif

} // namespace BALL
