// $Id: bond.C,v 1.5 1999/12/30 18:05:32 oliver Exp $

#include <BALL/KERNEL/bond.h>

#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/baseFragment.h>

using namespace std;

namespace BALL 
{

	Bond::TooManyBonds::TooManyBonds(const char* file, int line)
		:	Exception::GeneralException(file, line)
	{
	}

	Bond::Bond()
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
		: Composite(bond, deep),
			PropertyManager(bond, deep),
			first_(bond.first_),
			second_(bond.second_),
			name_(bond.name_),
			bond_order_(bond.bond_order_),
			bond_type_(bond.bond_type_)
	{
	}
		
	Bond::Bond
		(const String &name, Atom &first,
		 Atom &second, Bond::Order order,Type type)
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
		if (bond.isBonded()) 
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
	{ 
		arrangeBonds_();
	}

	void Bond::clear()
	{
		PropertyManager::clear();
		
		arrangeBonds_();
		clear_();
	}

	void Bond::destroy()
	{
		PropertyManager::destroy();

		arrangeBonds_();
		clear_();
	}


  void Bond::persistentWrite(PersistenceManager& pm, const char* name) const
	{
		pm.writeObjectHeader(this, name);

			Composite::persistentWrite(pm);

      pm.writeStorableObject(*(PropertyManager*)this, "PropertyManager");
 
			pm.writeObjectPointer(first_, "first_");
			pm.writeObjectPointer(second_, "second_");
			pm.writePrimitive(name_, "name_");
			pm.writePrimitive(bond_order_, "bond_order_");
			pm.writePrimitive(bond_type_, "bond_type_");
		pm.writeObjectTrailer(name);
	}

  void Bond::persistentRead(PersistenceManager& pm)
	{
		pm.checkObjectHeader(RTTI::getStreamName<Composite>());
			Composite::persistentRead(pm);
		pm.checkObjectTrailer(0);

		pm.readStorableObject(*(PropertyManager*)this, "PropertyManager");

		pm.readObjectPointer(first_, "first_");
		pm.readObjectPointer(second_, "second_");
		pm.readPrimitive(name_, "name_");
		pm.readPrimitive(bond_order_, "bond_order_");
		pm.readPrimitive(bond_type_, "bond_type_");
	}
 
	void Bond::finalize()
	{
		if (*first_ > *second_)
		{
			Atom* tmp = first_;
			first_ = second_;
			second_ = tmp;
		}
	}

	void Bond::set(const Bond &bond,bool deep)
	{
		PropertyManager::set(bond, deep);

		first_ = bond.first_;
		second_ = bond.second_;
		name_ = bond.name_;
		bond_order_ = bond.bond_order_;
		bond_type_ = bond.bond_type_;
	}

	Bond& Bond::operator = (const Bond& bond)
	{
		set(bond);
		
		return *this;
	}

	void Bond::get(Bond &bond, bool deep) const
	{
		bond.set(*this, deep);
	}

	void Bond::swap(Bond &bond)
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
	{
		first_ = atom;
	}
		 
	Atom* Bond::getFirstAtom()
	{
		return first_;
	}
		 
	const Atom* Bond::getFirstAtom() const
	{
		return first_;
	}
		 
	Atom* Bond::getPartner(const Atom& atom) const
	{
		Atom* partner = 0;
		if (&atom == first_)
		{
			partner = second_;
		} 
		else if (&atom == second_)
		{
			partner = first_;
		}
		
		return partner;
	}
	
	void Bond::setSecondAtom(Atom* atom)
	{
		second_ = atom;
	}
		 
	Atom* Bond::getSecondAtom()
	{
		return second_;
	}
		 
	const Atom* Bond::getSecondAtom() const
	{
		return second_;
	}

	void Bond::setName(const String& name)
	{
		name_ = name;
	}

	String& Bond::getName()
	{
		return name_;
	}

	const String& Bond::getName() const
	{
		return name_;
	}

	void Bond::setOrder(Bond::Order bond_order)
	{
		bond_order_ = bond_order;
	}
		
	Bond::Order Bond::getOrder() const
	{
		return bond_order_;
	}
		
	void Bond::setType(Type bond_type)
	{
		bond_type_ = bond_type;
	}
		
	Bond::Type Bond::getType() const
	{
		return bond_type_;
	}
		
	Real Bond::getLength() const
	{
		if (first_ == 0 || second_ == 0)
		{
			throw Exception::NullPointer(__FILE__, __LINE__);
		}
		
		return first_->position_.getDistance(second_->position_);
	}

	Bond* Bond::getBond(Atom &first,Atom &second)
	{
		return first.getBond(second);
	}

	Atom* Bond::getBondedAtomOf(const Atom& atom)
	{
		if (first_ == &atom)
		{
			return second_;
		} else if (second_ == &atom) {
			return first_;
		} else {
			return 0;
		}
	}

	const Atom* Bond::getBondedAtomOf(const Atom& atom) const
	{
		return ((Bond *)this)->getBondedAtomOf(atom);
	}

	bool Bond::isBondOf(const Atom& atom) const
	{
		return atom.hasBond(*this);
	}

	bool Bond::isBonded() const
	{
		return (bool)(first_ != 0);
	}

	bool Bond::isInterBond() const
	{
		return (bool)(isBonded() == true
			&& (first_->Composite::getRoot() != second_->Composite::getRoot()));
	}

	bool Bond::isInterBondOf(const BaseFragment& base_fragment) const
	{
		if (isBonded() == true)
		{
  		bool first_atom_is_descendant = first_->Composite::isDescendantOf(base_fragment);
			bool second_atom_is_descendant = second_->Composite::isDescendantOf(base_fragment);

			return (bool)((first_atom_is_descendant == true && second_atom_is_descendant == false)
				|| (first_atom_is_descendant == false && second_atom_is_descendant == true));
		} else {
			return false;
		}
	}

	bool Bond::isInterBondOf(const System &system) const
	{
		if (isBonded() == true)
		{
			bool first_atom_is_descendant = first_->Composite::isDescendantOf(system);
			bool second_atom_is_descendant = second_->Composite::isDescendantOf(system);

			return (bool)((first_atom_is_descendant == true && second_atom_is_descendant == false)
				|| (first_atom_is_descendant == false && second_atom_is_descendant == true));
		}
		else
		{
			return false;
		}
	}

	bool Bond::isIntraBond() const
	{
		return (bool)(isBonded() == true
									&& (first_->Composite::getRoot() == second_->Composite::getRoot()));
	}

	bool Bond::isIntraBondOf(const BaseFragment &base_fragment) const
	{
		return (bool)(isBonded() == true
									&& first_->Composite::isDescendantOf(base_fragment) == true
									&& second_->Composite::isDescendantOf(base_fragment) == true);
	}

	bool Bond::isIntraBondOf(const System &system) const
	{
		return (bool)(isBonded() == true
									&& first_->Composite::isDescendantOf(system) == true
									&& second_->Composite::isDescendantOf(system) == true);
	}

	bool  Bond::isValid () const
	{
		return (bool)(PropertyManager::isValid() == true
									&& first_ != 0
									&& second_ != 0
									&& first_ != second_
									&& first_->hasBond(*this) == true
									&& second_->hasBond(*this) == true);
	}

	void Bond::dump(ostream& s, Size depth) const
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
	{
		first_ = BALL_BOND_DEFAULT_FIRST_ATOM;
		second_ = BALL_BOND_DEFAULT_SECOND_ATOM;
		name_ = BALL_BOND_DEFAULT_NAME;
		bond_order_ = BALL_BOND_DEFAULT_ORDER;
		bond_type_ = BALL_BOND_DEFAULT_TYPE;
	}

} // namespace BALL
