// $Id: atom.C,v 1.7 1999/12/30 18:05:31 oliver Exp $

#include <BALL/KERNEL/atom.h>

#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/fragment.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/PSE.h> 

using namespace std;

namespace BALL 
{

	Atom::Atom()
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
		:	Composite(atom, deep),
			PropertyManager(atom, deep),
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
	{
		destroy();
	}

	void Atom::clear()
	{
		Composite::clear();
		PropertyManager::clear();

		clear_();
	}
		
	void Atom::destroy()
	{
		Composite::destroy();
		PropertyManager::destroy();

		clear_();
	}

	void Atom::persistentWrite(PersistenceManager& pm, const char* name) const
	{
		pm.writeObjectHeader(this, name);

			Composite::persistentWrite(pm);

			pm.writeStorableObject(*(PropertyManager*)this, "PropertyManager");

			pm.writePrimitive((String)element_->getSymbol(), "element_");
			pm.writePrimitive(charge_, "charge_");
			pm.writePrimitive(radius_, "radius_");
			pm.writePrimitive(name_, "name_");
			pm.writePrimitive(type_name_, "type_name_");
			pm.writePrimitive(type_, "type_");

			position_.persistentWrite(pm, "position_");
			velocity_.persistentWrite(pm, "velocity_");
			force_.persistentWrite(pm, "force_");

			pm.writePrimitive(number_of_bonds_, "number_of_bonds_");
			pm.writeObjectPointerArray((Bond**)bond_, "bond_", (Size)number_of_bonds_);
		pm.writeObjectTrailer(name);
	}

	void Atom::persistentRead(PersistenceManager& pm)
	{
		pm.checkObjectHeader(RTTI::getStreamName<Composite>());
			Composite::persistentRead(pm);
		pm.checkObjectTrailer(0);

		pm.readStorableObject(*(PropertyManager*)this, "PropertyManager");

		String s;
		pm.readPrimitive(s, "element_");	
		element_ = &PSE[s];
		pm.readPrimitive(charge_, "charge_");
		pm.readPrimitive(radius_, "radius_");
		pm.readPrimitive(name_, "name_");
		pm.readPrimitive(type_name_, "type_name_");
		pm.readPrimitive(type_, "type_");

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
	{
		Composite::set(atom, deep);
		PropertyManager::set(atom, deep);
		
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
			
	Atom& Atom::operator =(const Atom &atom)
	{
		set(atom);

		return *this;
	}

	void Atom::get(Atom &atom, bool deep) const
	{
		atom.set(*this, deep);
	}
			
	void Atom::swap(Atom &atom)
	{
		Composite::swap(atom);
		PropertyManager::swap(atom);

		Element *temp_element = element_;
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
		for (register int i = 0; i < MAX_NUMBER_OF_BONDS;++i)
		{
			temp_bond = bond_[i];
			bond_[i] = atom.bond_[i];
			atom.bond_[i] = temp_bond;
		}

		char temp_c = number_of_bonds_;
		number_of_bonds_ = atom.number_of_bonds_;
		atom.number_of_bonds_ = temp_c;
	}
		
	void Atom::setElement(Element &element)
	{
		element_ = &element;
	}

	Element & Atom::getElement()
	{
		return *element_;
	}

	const Element &Atom::getElement() const
	{
		return *element_;
	}

	void Atom::setCharge(float charge)
	{
		charge_ = charge;
	}

	float Atom::getCharge() const
	{
		return charge_;
	}

	Molecule *Atom::getMolecule()
	{
		for (Composite::AncestorIterator ancestor_it = beginAncestor();
				 !ancestor_it.isEnd();
				 ++ancestor_it)
		{
			if (RTTI::isKindOf<Molecule>(*ancestor_it))
			{
				return (Molecule *)&*ancestor_it;
			}
		}

		return 0;
	}

	const Molecule *
	Atom::getMolecule
		() const
	{
		return ((Atom *)this)->getMolecule();
	}

	Fragment *
	Atom::getFragment
		()
	{
		for (Composite::AncestorIterator ancestor_it = beginAncestor();
				 !ancestor_it.isEnd(); ++ancestor_it)
		{
			if (RTTI::isKindOf<Fragment>(*ancestor_it))
			{
				return (Fragment *)&*ancestor_it;
			}
		}

		return 0;
	}

	const Fragment *Atom::getFragment() const
	{
		return ((Atom *)this)->getFragment();
	}

	void Atom::setName(const String &name)
	{
		name_ = name;
	}

	String & Atom::getName()
	{
		return name_;
	}

	const String &Atom::getName() const
	{
		return name_;
	}

	void Atom::setPosition(const Vector3 &position)
	{
		position_.set(position);
	}

	Vector3 &Atom::getPosition()
	{
		return position_;
	}

	const Vector3 &Atom::getPosition() const
	{
		return position_;
	}

	void Atom::setRadius(float radius)
	{
		radius_ = radius;
	}

	float Atom::getRadius() const
	{
		return radius_;
	}

	void Atom::setType(Type atom_type)
	{
		type_ = atom_type;
	}
		
	Atom::Type Atom::getType() const
	{
		return type_;
	}
		
	void Atom::setTypeName(const String& type_name)
	{
		type_name_ = type_name;
	}
		
	String Atom::getTypeName() const
	{
		return type_name_;
	}
		
	void Atom::setVelocity(const Vector3 &velocity)
	{
		velocity_.set(velocity);
	}

	Vector3& Atom::getVelocity()
	{
		return velocity_;
	}

	const Vector3& Atom::getVelocity() const
	{
		return velocity_;
	}

	void Atom::setForce(const Vector3& force)
	{
		force_.set(force);
	}

	Vector3& Atom::getForce()
	{
		return force_;
	}

	const Vector3& Atom::getForce() const
	{
		return force_;
	}

	Size Atom::countBonds() const
	{
		return number_of_bonds_;
	}

	Bond* Atom::getBond(Position index)
	{
		if (index >= (Index)MAX_NUMBER_OF_BONDS)
		{
			throw Exception::IndexUnderflow(__FILE__, __LINE__, (Index)index, (Size)MAX_NUMBER_OF_BONDS);
		}

		return ((Size)index < number_of_bonds_) ? bond_[index] : 0;
	}

	const Bond* Atom::getBond(Position index) const
	{
		return (const Bond *)((Atom *)this)->getBond(index);
	}

	Bond* Atom::getBond(const Atom &atom)
	{
		if (&atom != this)
		{
			for (register int i = 0; i < number_of_bonds_; ++i)
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
	{
		return ((Atom *)this)->getBond(atom);
	}

	Bond* Atom::createBond(Atom &atom)
	{
		Bond* bond = getBond(atom);
		
		if (bond == 0)
		{
			return Bond::createBond(*new Bond, *this, atom);
		} else {
			return bond;
		}
	}

	Bond* Atom::createBond(Bond& bond, Atom& atom)
	{
		return Bond::createBond(bond, *this, atom);
	}

	Bond* Atom::cloneBond(Bond& bond, Atom& atom)
	{
		Bond* bond_ptr = getBond(atom);
		
		if (bond_ptr == 0)
		{
			bond_ptr = (Bond *)bond.create();
			bond_ptr->setFirstAtom(0);
			bond_ptr->setSecondAtom(0);
			
			return Bond::createBond(*bond_ptr, *this, atom);
		} else {
			return bond_ptr;
		}
	}

	bool Atom::destroyBond(const Atom& atom)
	{
		Bond* bond = getBond(atom);

		if (bond == 0)
		{
			return false;
		}
			
		if (bond->isAutoDeletable() == true)
		{
			delete bond;
		} else {
			bond->destroy();
		}

		return true;
	}

	void Atom::destroyBonds()
	{
		for (register int i = char(number_of_bonds_) - 1; i >= 0; --i)
		{
			if (bond_[i]->isAutoDeletable() == true)
			{
				delete bond_[i];
			} else {
				bond_[i]->destroy();
			}
		}
		
		number_of_bonds_ = 0;
	}

	bool Atom::hasBond(const Bond& bond) const
	{
		for (register int i = 0; i < number_of_bonds_; ++i)
			if (bond_[i] == &bond)
				return true;

		return false;
	}

	bool 	Atom::isBondedTo(const Atom& atom) const
	{
		return (bool)(getBond(atom) != 0);
	}

	bool Atom::isBonded() const
	{
		return (bool)(number_of_bonds_ > 0);
	}

	bool Atom::isGeminal(const Atom& atom) const
	{
		// an atom is geminal to another, if it 
		// is not directly bonded to it
		bool is_geminal = !isBondedTo(atom);

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
					if (partner_a->isBondedTo(atom))
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
	{ 
		// an atom is vicinal to another, if it 
		// is not directly bonded to it
		bool is_vicinal = !isBondedTo(atom);

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
						if ((partner_b) != 0 && (partner_b->isBondedTo(atom)))
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

		for (register int i = 0; i < number_of_bonds_; ++i)
		{
			if (bond_[i]->isValid() == false)
				return false;

			if (bond_[i]->first_ != this && bond_[i]->second_ != this)
				return false;
		}

		return true;
	}

	void Atom::dump(ostream& s, Size depth) const
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

		for (register int i = 0; i < number_of_bonds_; ++i)
			bond_[i]->dump(s, depth + 1);

		s << endl;

		BALL_DUMP_STREAM_SUFFIX(s);
	}

	void Atom::read(istream & /* s */)
	{
		throw Exception::NotImplemented(__FILE__, __LINE__);
	}

	void Atom::write(ostream & /* s */) const
	{
		throw Exception::NotImplemented(__FILE__, __LINE__);
	}

	bool Atom::apply(UnaryProcessor<Bond>& processor)
	{
		if (processor.start() == false)
			return false;

		Processor::Result result;

		for (register int i = 0; i < number_of_bonds_; ++i)
		{
			result = processor(*bond_[i]);

			if (result <= Processor::BREAK)
				return (result == Processor::BREAK) ? true : false;
		}

		return processor.finish();
	}

	bool Atom::apply(UnaryProcessor<Object>& processor)
	{
		if (processor.start() == false)
			return false;

		Processor::Result result;

		for (register int i = 0; i < number_of_bonds_; ++i)
		{
			result = processor(*bond_[i]);

			if (result <= Processor::BREAK)
				return (result == Processor::BREAK) ? true : false;
		}

		return processor.finish();
	}

	void Atom::clear_()
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
	{
		for (register int i = 0; i < number_of_bonds_; ++i)
		{
			if (bond_[i]->first_ == atom
					|| bond_[i]->second_ == atom)
			{
				bond_[i] = bond_[--number_of_bonds_];
				break;
			}
		}
	}

} // namespace BALL
