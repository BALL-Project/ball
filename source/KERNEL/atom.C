// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: atom.C,v 1.38 2003/01/15 10:51:08 anhi Exp $

#include <BALL/KERNEL/atom.h>

#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/fragment.h>
#include <BALL/KERNEL/residue.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/PTE.h> 

#include <algorithm>
#include <functional>

using namespace std;

namespace BALL 
{

	Atom::AttributeVector			Atom::static_attributes_;
	Atom::AtomIndexList				Atom::free_list_;
	
	void Atom::StaticAtomAttributes::clear()
	{
		charge = BALL_ATOM_DEFAULT_CHARGE;
		type = BALL_ATOM_DEFAULT_TYPE;
		position.set(BALL_ATOM_DEFAULT_POSITION);
		velocity.set(BALL_ATOM_DEFAULT_VELOCITY);
		force.set(BALL_ATOM_DEFAULT_FORCE);
	}

	void Atom::StaticAtomAttributes::swap(Atom::StaticAtomAttributes& attr)
	{
		std::swap(charge, attr.charge);
		std::swap(type, attr.type);
		std::swap(ptr, attr.ptr);
		position.swap(attr.position);
		velocity.swap(attr.velocity);
		force.swap(attr.force);		
	}

	void Atom::StaticAtomAttributes::set(Atom::StaticAtomAttributes& attr)
	{
		charge = attr.charge;
		type = attr.type;
		//ptr = attr.ptr;
		position = attr.position;
		velocity = attr.velocity;
		force = attr.force;		
	}

	
	const Atom::StaticAtomAttributes& 
		Atom::StaticAtomAttributes::operator = (const Atom::StaticAtomAttributes& attr)
	{
		charge = attr.charge;
		type = attr.type;
		//ptr = attr.ptr;
		position = attr.position;
		velocity = attr.velocity;
		force = attr.force;		

		return *this;
	}	

	Atom::Atom()
		throw()
		:	Composite(),
			PropertyManager(),
			index_(Atom::nextIndex_()),
			element_(BALL_ATOM_DEFAULT_ELEMENT),
			name_(BALL_ATOM_DEFAULT_NAME),
			type_name_(BALL_ATOM_DEFAULT_TYPE_NAME),
			radius_(BALL_ATOM_DEFAULT_RADIUS),
			number_of_bonds_(0)
	{
		// set the static attributes to their default values
		static_attributes_[index_].clear();
		static_attributes_[index_].ptr = this;
	}
		
	Atom::Atom(const Atom& atom, bool deep)
		throw()
		:	Composite(atom, deep),
			PropertyManager(atom),
			index_(Atom::nextIndex_()),
			element_(atom.element_),
			name_(atom.name_),
			type_name_(atom.type_name_),
			radius_(atom.radius_),
			number_of_bonds_(0)
	{
		// copy the static attributes
		static_attributes_[index_] = atom.static_attributes_[atom.index_];
		static_attributes_[index_].ptr = this;
	}

	Atom::Atom
			(Element& element, const String& name,
			 const String& type_name, Atom::Type type,
			 const Vector3& position, const Vector3& velocity,
			 const Vector3& force, float charge, float radius)
		throw()
		:	Composite(),
			PropertyManager(),
			index_(Atom::nextIndex_()),
			element_(&element),
			name_(name),
			type_name_(type_name),
			radius_(radius),
			number_of_bonds_(0)
	{
		static_attributes_[index_].charge = charge;
		static_attributes_[index_].type = type;
		static_attributes_[index_].force = force;
		static_attributes_[index_].velocity = velocity;
		static_attributes_[index_].position = position;
		static_attributes_[index_].ptr = this;
	}
		
	Atom::~Atom()
		throw()
	{
		destroy();
		freeIndex_(index_);
	}

	Position Atom::nextIndex_()
	{
		if (free_list_.empty() || static_attributes_.empty())
		{
			// double the size of the array
			size_t i = static_attributes_.size();
			static_attributes_.resize(std::max((size_t)200, 2 * i));

			// add the free indices to the free list
			for (; i < static_attributes_.size(); free_list_.push_back((Position)i++));
		}
		
		// return the next free index
		Position index = free_list_.front();
		free_list_.pop_front();
		return index;
	}

	void Atom::freeIndex_(Position index)
	{
		free_list_.push_front(index);
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
		throw(Exception::GeneralException)
	{
		pm.writeObjectHeader(this, name);

			Composite::persistentWrite(pm);

			pm.writeStorableObject(*(PropertyManager*)this, "PropertyManager");

			pm.writePrimitive((String)element_->getSymbol(), "element_");
			pm.writePrimitive(static_attributes_[index_].charge, "charge_");
			pm.writePrimitive(radius_, "radius_");
			pm.writePrimitive(name_, "name_");
			pm.writePrimitive(type_name_, "type_name_");
			pm.writePrimitive((Index)static_attributes_[index_].type, "type_");

			pm.writeStorableObject(static_attributes_[index_].position, "position_");
			pm.writeStorableObject(static_attributes_[index_].velocity, "velocity_");
			pm.writeStorableObject(static_attributes_[index_].force, "force_");

			pm.writePrimitive(number_of_bonds_, "number_of_bonds_");
			pm.writeObjectPointerArray((Bond**)bond_, "bond_", (Size)number_of_bonds_);
		pm.writeObjectTrailer(name);
	}

	void Atom::persistentRead(PersistenceManager& pm)
		throw(Exception::GeneralException)
	{
		pm.checkObjectHeader(RTTI::getStreamName<Composite>());
			Composite::persistentRead(pm);
		pm.checkObjectTrailer(0);

		pm.readStorableObject(*(PropertyManager*)this, "PropertyManager");

		String s;
		pm.readPrimitive(s, "element_");	
		element_ = &PTE[s];
		pm.readPrimitive(static_attributes_[index_].charge, "charge_");
		pm.readPrimitive(radius_, "radius_");
		pm.readPrimitive(name_, "name_");
		pm.readPrimitive(type_name_, "type_name_");
		Index tmp_type;
		pm.readPrimitive(tmp_type, "type_");
		static_attributes_[index_].type = (Atom::Type)tmp_type;

		pm.readStorableObject(static_attributes_[index_].position, "position_");
		pm.readStorableObject(static_attributes_[index_].velocity, "velocity_");
		pm.readStorableObject(static_attributes_[index_].force, "force_");

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
    PropertyManager::operator = (atom);
    
    element_ = atom.element_;
    name_ = atom.name_;
    type_name_ = atom.type_name_;
    radius_ = atom.radius_;
    number_of_bonds_ = 0;

		// copy static attributes
    static_attributes_[index_] = static_attributes_[atom.index_];
  }

  void Atom::get(Atom& atom, bool deep) const
    throw()
  {
    atom.set(*this, deep);
  }

	const Atom& Atom::operator = (const Atom &atom)
		throw()
	{
		Composite::operator =(atom);
		PropertyManager::operator = (atom);
		
		element_ = atom.element_;
		name_ = atom.name_;
		type_name_ = atom.type_name_;
		radius_ = atom.radius_;
		number_of_bonds_ = 0;

		// copy static attributes
		static_attributes_[index_] = static_attributes_[atom.index_];

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
		
		name_.swap(atom.name_);
		type_name_.swap(atom.type_name_);

		float temp = radius_;
		radius_ = atom.radius_;
		atom.radius_ = temp;

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

		// swap static attributes by swapping the indices
		Position tmp_index = index_;
		index_ = atom.index_;
		atom.index_ = tmp_index;
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
				|| element_ == 0)
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
		s << "  charge: " << static_attributes_[index_].charge << endl;
		
		BALL_DUMP_DEPTH(s, depth);
		s << "  name: " << name_ << endl;
		
		BALL_DUMP_DEPTH(s, depth);
		s << "  type name: " << type_name_ << endl;
		
		BALL_DUMP_DEPTH(s, depth);
		s << "  position: " << static_attributes_[index_].position << endl;
		
		BALL_DUMP_DEPTH(s, depth);
		s << "  radius: " << radius_ << endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "  type: " << static_attributes_[index_].type << endl;
		
		BALL_DUMP_DEPTH(s, depth);
		s << "  velocity: " << static_attributes_[index_].velocity << endl;
		
		BALL_DUMP_DEPTH(s, depth);
		s << "  force: " << static_attributes_[index_].force << endl;
		
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
		name_ = BALL_ATOM_DEFAULT_NAME;
		type_name_ = BALL_ATOM_DEFAULT_TYPE_NAME;
		radius_ = BALL_ATOM_DEFAULT_RADIUS;
				 
		static_attributes_[index_].clear();

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

	Position Atom::compact(const Atom::AtomIndexList& indices)
		throw(Exception::OutOfRange)
	{
		// create a sorted copy of the indices
		AtomIndexList sorted_indices(indices);
		sorted_indices.sort();	

		// Remove all indices in the range of [i...i+indices.size()],
		// where i is the smalles index in the list. Those entries
		// are already in the correct range and won't have to be
		// reassigned.
		Position first_pos = sorted_indices.front();

	
		// sort the free list to achieve higher locality 
		free_list_.sort();
				
		Atom::AtomIndexList::const_iterator idx_it = indices.begin();
		Atom::AtomIndexList::iterator free_it = free_list_.begin();
		for (Position i = first_pos; (idx_it != indices.end()) && (i < static_attributes_.size()); ++i, ++idx_it)
		{
			// The atom is already where it's supposed to be. Fine.
			if (*idx_it == i)
			{
				continue;
			}

			if (*idx_it >= static_attributes_.size())
			{
				throw Exception::OutOfRange(__FILE__, __LINE__);
			}

			// Make sure that we are at the right position in the free list.
			while (free_it != free_list_.end() && (*free_it < i)) 
			{
				++free_it;
			}

			// 
			if ((free_it != free_list_.end()) && (i == *free_it))
			{
				// Copy the contents of the next atom to the current
				// position (which is free) and adjust the atom's index.
				static_attributes_[i].set(static_attributes_[*idx_it]);
				static_attributes_[i].ptr->index_ = i;
				
				// Mark the old position as free.
				*free_it = *idx_it;
				
				// Resort the free_list and re-initialize the invalidated iterator.
				free_list_.sort();
				free_it = free_list_.begin();
			}
			else
			{
				// Default case: we just swap the atoms and their index_ attributes.
				static_attributes_[i].swap(static_attributes_[*idx_it]);
				std::swap(static_attributes_[i].ptr->index_, static_attributes_[*idx_it].ptr->index_);
			}
		}	

		return first_pos;
	}


# ifdef BALL_NO_INLINE_FUNCTIONS
#   include <BALL/KERNEL/atom.iC>
# endif

} // namespace BALL
