// $Id: bond.C,v 1.19 2001/06/08 16:57:13 anker Exp $

#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/atomContainer.h>

using namespace std;

namespace BALL 
{

	Bond::TooManyBonds::TooManyBonds(const char* file, int line)
		throw()
		:	Exception::GeneralException(file, line)
	{
	}

	Bond::NotBounded::NotBounded(const char* file, int line)
		throw()
		:	Exception::GeneralException(file, line)
	{
	}

	Bond::Bond()
		throw()
		: Composite(),
			PropertyManager(),
			first_(BALL_BOND_DEFAULT_FIRST_ATOM),
			second_(BALL_BOND_DEFAULT_SECOND_ATOM),
			name_(BALL_BOND_DEFAULT_NAME),
			bond_order_(BALL_BOND_DEFAULT_ORDER),
			bond_type_(BALL_BOND_DEFAULT_TYPE)
	{
	}

	Bond::Bond(const Bond& bond, bool deep)
		throw()
		: Composite(bond, deep),
			PropertyManager(bond, deep),
			first_(bond.first_),
			second_(bond.second_),
			name_(bond.name_),
			bond_order_(bond.bond_order_),
			bond_type_(bond.bond_type_)
	{
	}
		
	Bond::Bond(const String &name, Atom &first, Atom &second, Bond::Order order,Type type)
		throw()
		: Composite(),
			PropertyManager(),
			first_(BALL_BOND_DEFAULT_FIRST_ATOM),
			second_(BALL_BOND_DEFAULT_SECOND_ATOM),
			name_(name),
			bond_order_(order),
			bond_type_(type)
	{
		createBond(*this, first, second);
	}

	Bond* Bond::createBond(Bond& bond, Atom& first, Atom& second)
		throw(Bond::TooManyBonds)
	{
		if ((Size)first.number_of_bonds_ > (Size)Atom::MAX_NUMBER_OF_BONDS)
		{
			throw TooManyBonds(__FILE__, __LINE__);
		}
		
		if ((Size)second.number_of_bonds_ > (Size)Atom::MAX_NUMBER_OF_BONDS)
		{
			throw TooManyBonds(__FILE__, __LINE__);
		}

		// if the bond is already bonded, delete it and create 
		// it anew
		if (bond.isBound()) 
		{
			bond.clear();
		}

		if (&first == &second)
		{
			return 0;
		}
		
		Bond *bond_ptr = first.getBond(second);

		if (bond_ptr != 0)
		{
			return bond_ptr;
		}
		
		first.bond_[first.number_of_bonds_]
			= second.bond_[second.number_of_bonds_]
			= &bond;
		
		++(first.number_of_bonds_);
		++(second.number_of_bonds_);

		if (first < second)
		{
			bond.first_ = &first;
			bond.second_ = &second;
		}
		else
		{
			bond.first_ = &second;
			bond.second_ = &first;
		}

		return &bond;
	}

	Bond::~Bond()
		throw()
	{ 
		arrangeBonds_();
	}

	void Bond::clear()
		throw()
	{
		PropertyManager::clear();
		
		arrangeBonds_();
		clear_();
	}

	void Bond::destroy()
		throw()
	{
		PropertyManager::destroy();
		arrangeBonds_();
		clear_();
	}

  void Bond::persistentWrite(PersistenceManager& pm, const char* name) const
		throw()
	{
		pm.writeObjectHeader(this, name);

			Composite::persistentWrite(pm);

      pm.writeStorableObject(*(PropertyManager*)this, "PropertyManager");
 
			pm.writeObjectPointer(first_, "first_");
			pm.writeObjectPointer(second_, "second_");
			pm.writePrimitive(name_, "name_");
			pm.writePrimitive((Index)bond_order_, "bond_order_");
			pm.writePrimitive((Index)bond_type_, "bond_type_");
		pm.writeObjectTrailer(name);
	}

  void Bond::persistentRead(PersistenceManager& pm)
		throw()
	{
		pm.checkObjectHeader(RTTI::getStreamName<Composite>());
			Composite::persistentRead(pm);
		pm.checkObjectTrailer(0);

		pm.readStorableObject(*(PropertyManager*)this, "PropertyManager");

		pm.readObjectPointer(first_, "first_");
		pm.readObjectPointer(second_, "second_");
		pm.readPrimitive(name_, "name_");
		Index tmp;
		pm.readPrimitive(tmp, "bond_order_");
		bond_order_ = (BondType)tmp;
		pm.readPrimitive(tmp, "bond_type_");
		bond_type_ = (Bond::Order)tmp;
	}
 
	void Bond::finalize()
		throw()
	{
		if (*first_ > *second_)
		{
			Atom* tmp = first_;
			first_ = second_;
			second_ = tmp;
		}
	}

	const Bond& Bond::operator = (const Bond& bond)
		throw()
	{
		PropertyManager::operator =(bond);

		first_			=	bond.first_;
		second_			=	bond.second_;
		name_				= bond.name_;
		bond_order_ = bond.bond_order_;
		bond_type_  = bond.bond_type_;

		return *this;
	}

	void Bond::swap(Bond &bond)
		throw()
	{
		PropertyManager::swap(bond);

		Atom* temp_atom = first_;
		first_ = bond.first_;
		bond.first_ = temp_atom;

		temp_atom = second_;
		second_ = bond.second_;
		bond.second_ = temp_atom;

		name_.swap(bond.name_);

		Order temp_order = bond_order_;
		bond_order_ = bond.bond_order_;
		bond.bond_order_ = temp_order;

		Type temp_type = bond_type_;
		bond_type_ = bond.bond_type_;
		bond.bond_type_ = temp_type;
	}

	void Bond::setFirstAtom(Atom* atom)
		throw()
	{
		first_ = atom;
	}
		 
	Atom* Bond::getFirstAtom()
		throw()
	{
		return first_;
	}
		 
	const Atom* Bond::getFirstAtom() const
		throw()
	{
		return first_;
	}
		 
	Atom* Bond::getPartner(const Atom& atom) const
		throw()
	{
		Atom* partner = 0;
		if (&atom == first_)
		{
			partner = second_;
		} 
		else 
		{
			if (&atom == second_)
			{
				partner = first_;
			}
		}		
		return partner;
	}
	
	void Bond::setSecondAtom(Atom* atom)
		throw()
	{
		second_ = atom;
	}
		 
	Atom* Bond::getSecondAtom()
		throw()
	{
		return second_;
	}
		 
	const Atom* Bond::getSecondAtom() const
		throw()
	{
		return second_;
	}

	void Bond::setName(const String& name)
		throw()
	{
		name_ = name;
	}

	const String& Bond::getName() const
		throw()
	{
		return name_;
	}

	void Bond::setOrder(Bond::Order bond_order)
		throw()
	{
		bond_order_ = bond_order;
	}
		
	Bond::Order Bond::getOrder() const
		throw()
	{
		return bond_order_;
	}
		
	void Bond::setType(Type bond_type)
		throw()
	{
		bond_type_ = bond_type;
	}
		
	Bond::Type Bond::getType() const
		throw()
	{
		return bond_type_;
	}
		
	Real Bond::getLength() const
		throw(Bond::NotBounded)
	{
		if (first_ == 0 || second_ == 0)
		{
			throw NotBounded(__FILE__, __LINE__);
		}
		
		return first_->position_.getDistance(second_->position_);
	}

	Bond* Bond::getBond(Atom &first,Atom &second)
		throw()
	{
		return first.getBond(second);
	}

	Atom* Bond::getBoundAtom(const Atom& atom)
		throw()
	{
		if (first_ == &atom)
		{
			return second_;
		} 
		else 
		{
			if (second_ == &atom) 
			{
				return first_;
			} 
		}

		return 0;
	}

	const Atom* Bond::getBoundAtom(const Atom& atom) const
		throw()
	{
		return ((Bond *)this)->getBoundAtom(atom);
	}

	bool Bond::isBondOf(const Atom& atom) const
		throw()
	{
		return atom.hasBond(*this);
	}

	bool Bond::isBound() const
		throw()
	{
		return (first_ != 0);
	}

	bool Bond::isInterBond() const
		throw()
	{
		// the two atoms have to be inside the same (non-NULL) composite
		return (isBound() && (first_->Composite::getRoot() != second_->Composite::getRoot())
						&& (&first_->Composite::getRoot() != first_));
	}

	bool Bond::isInterBondOf(const AtomContainer& atom_container) const
		throw()
	{
		if (isBound())
		{
  		bool first_atom_is_descendant  = first_->Composite::isDescendantOf(atom_container);
			bool second_atom_is_descendant = second_->Composite::isDescendantOf(atom_container);

			return (( first_atom_is_descendant && !second_atom_is_descendant) ||
							(!first_atom_is_descendant &&  second_atom_is_descendant));
		} 

		return false;
	}

	bool Bond::isInterBondOf(const System &system) const
		throw()
	{
		if (isBound() == true)
		{
			bool first_atom_is_descendant  =  first_->Composite::isDescendantOf(system);
			bool second_atom_is_descendant = second_->Composite::isDescendantOf(system);

			return (( first_atom_is_descendant && !second_atom_is_descendant) ||
							(!first_atom_is_descendant &&  second_atom_is_descendant));
		}

		return false;
	}

	bool Bond::isIntraBond() const
		throw()
	{	
		// the two atoms have to be inside two different (non-NULL) composite
		return (isBound() && (first_->Composite::getRoot() == second_->Composite::getRoot()) 
						&& (&first_->Composite::getRoot() != first_) && (&second_->Composite::getRoot() != second_));
	}

	bool Bond::isIntraBondOf(const AtomContainer &atom_container) const
		throw()
	{
		return (isBound() &&  first_->Composite::isDescendantOf(atom_container)
											&& second_->Composite::isDescendantOf(atom_container));
	}

	bool Bond::isIntraBondOf(const System &system) const
		throw()
	{
		return (isBound() &&  first_->Composite::isDescendantOf(system)
											&& second_->Composite::isDescendantOf(system));
	}

	bool Bond::isValid () const
		throw()
	{
		return (PropertyManager::isValid()
									&& first_  != 0
									&& second_ != 0
									&& first_  != second_
									&&  first_->hasBond(*this)
									&& second_->hasBond(*this));
	}

	void Bond::dump(ostream& s, Size depth) const
		throw()
	{
		BALL_DUMP_STREAM_PREFIX(s);
		
		Composite::dump(s, depth);

		BALL_DUMP_DEPTH(s, depth);
		s << "  name: " << name_ << endl;
		
		BALL_DUMP_DEPTH(s, depth);
		s << "  order: " << bond_order_ << endl;
		
		BALL_DUMP_DEPTH(s, depth);
		s << "  type: " << bond_type_ << endl;
		
		BALL_DUMP_DEPTH(s, depth);
		s << "  first atom: " << first_ << endl;
		
		BALL_DUMP_DEPTH(s, depth);
		s << "  second atom: " << second_ << endl;

		BALL_DUMP_STREAM_SUFFIX(s);
	}

	void Bond::arrangeBonds_()
		throw()
	{
		if (first_ != 0 && second_ != 0)
		{
			if (first_->number_of_bonds_ > 0)
			{
				first_->swapLastBond_(second_);
			}
			
			if (second_->number_of_bonds_ > 0)
			{
				second_->swapLastBond_(first_);
			}
		}
	}

	void Bond::clear_()
		throw()
	{
		first_ = BALL_BOND_DEFAULT_FIRST_ATOM;
		second_ = BALL_BOND_DEFAULT_SECOND_ATOM;
		name_ = BALL_BOND_DEFAULT_NAME;
		bond_order_ = BALL_BOND_DEFAULT_ORDER;
		bond_type_ = BALL_BOND_DEFAULT_TYPE;
	}

	bool Bond::operator == (const Bond& bond) const
		throw()
	{
		return(Object::operator ==(bond));
	}

	bool Bond::operator != (const Bond& bond) const
		throw()
	{
		return ! (*this == bond);
	}


} // namespace BALL
