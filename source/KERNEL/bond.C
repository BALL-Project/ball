// $Id: bond.C,v 1.25 2001/07/15 16:12:42 oliver Exp $

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

	Bond::NotBound::NotBound(const char* file, int line)
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
			PropertyManager(bond),
			first_(bond.first_),
			second_(bond.second_),
			name_(bond.name_),
			bond_order_(bond.bond_order_),
			bond_type_(bond.bond_type_)
	{
	}
		
	Bond::Bond(const String& name, Atom& first, Atom &second, Bond::Order order,Type type)
		throw(Bond::TooManyBonds)
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

		// first check the cases where no new bond will be created.
		if (&first == &second)
		{
			return 0;
		}
		
		Bond *bond_ptr = first.getBond(second);

		if (bond_ptr != 0)
		{
			return bond_ptr;
		}
		
		// throw an exception if there is no possibility to create another bond
		// for any of the two atoms
		if (((Size)first.number_of_bonds_ >= (Size)Atom::MAX_NUMBER_OF_BONDS)
				|| ((Size)second.number_of_bonds_ >= (Size)Atom::MAX_NUMBER_OF_BONDS))
		{
			throw TooManyBonds(__FILE__, __LINE__);
		}

		// if the bond is already bound, delete it and create 
		// it anew
		if (bond.isBound()) 
		{
			bond.clear();
		}

		first.bond_[first.number_of_bonds_]
			= second.bond_[second.number_of_bonds_]
			= &bond;
		
		++(first.number_of_bonds_);
		++(second.number_of_bonds_);

		// keep the order 
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

 void Bond::persistentWrite(PersistenceManager& pm, const char* name) const
		throw(Exception::GeneralException)
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
		throw(Exception::GeneralException)
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
 
	const Bond& Bond::operator = (const Bond& bond)
		throw()
	{
		PropertyManager::operator = (bond);

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

# ifdef BALL_NO_INLINE_FUNCTIONS
#   include <BALL/KERNEL/bond.iC>
# endif

} // namespace BALL
