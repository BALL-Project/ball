// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/fragment.h>
#include <BALL/KERNEL/chain.h>
#include <BALL/KERNEL/secondaryStructure.h>
#include <BALL/KERNEL/residue.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/molecularInteractions.h>

#include <algorithm>

using namespace::std;

namespace BALL
{
	Atom::Atom()
		: Composite(),
		  PropertyManager(),
		  interactions(0),
		  store_interactions_disabled_(0),
		  name_(BALL_ATOM_DEFAULT_NAME),
		  type_name_(BALL_ATOM_DEFAULT_TYPE_NAME),
		  element_(BALL_ATOM_DEFAULT_ELEMENT),
		  radius_(BALL_ATOM_DEFAULT_RADIUS),
		  type_(BALL_ATOM_DEFAULT_TYPE),
		  number_of_bonds_(0),
		  formal_charge_(BALL_ATOM_DEFAULT_FORMAL_CHARGE),
		  position_(BALL_ATOM_DEFAULT_POSITION),
		  charge_(BALL_ATOM_DEFAULT_CHARGE),
		  velocity_(BALL_ATOM_DEFAULT_VELOCITY),
		  force_(BALL_ATOM_DEFAULT_FORCE)
	{
	}

	Atom::Atom(const Atom& atom, bool deep)
		: Composite(atom, deep),
		  PropertyManager(atom),
		  interactions(0),
		  store_interactions_disabled_(0),
		  name_(atom.name_),
		  type_name_(atom.type_name_),
		  element_(atom.element_),
		  radius_(atom.radius_),
		  type_(atom.type_),
			number_of_bonds_(0),
		  formal_charge_(atom.formal_charge_),
		  position_(atom.position_),
		  charge_(atom.charge_),
		  velocity_(atom.velocity_),
		  force_(atom.force_)
	{
	}

	Atom::Atom
			(Element& element, const String& name,
			 const String& type_name, Atom::Type type,
			 const Vector3& position, const Vector3& velocity,
			 const Vector3& force, float charge, float radius, Index formal_charge)
		: Composite(),
		  PropertyManager(),
		  interactions(0),
		  store_interactions_disabled_(0),
		  name_(name),
		  type_name_(type_name),
		  element_(&element),
		  radius_(radius),
		  type_(type),
		  number_of_bonds_(0),
		  formal_charge_(formal_charge),
		  position_(position),
		  charge_(charge),
		  velocity_(velocity),
		  force_(force)
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
		delete interactions;
		interactions = 0;

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

			pm.writeStorableObject(dynamic_cast<const PropertyManager&>(*this),
														 "PropertyManager");

			pm.writePrimitive((String)element_->getSymbol(), "element_");
			pm.writePrimitive(formal_charge_, "formal_charge_");
			pm.writePrimitive(charge_, "charge_");
			pm.writePrimitive(radius_, "radius_");
			pm.writePrimitive(name_, "name_");
			pm.writePrimitive(type_name_, "type_name_");
			pm.writePrimitive((Index)type_, "type_");

			pm.writeStorableObject(position_, "position_");
			pm.writeStorableObject(velocity_, "velocity_");
			pm.writeStorableObject(force_, "force_");

			pm.writePrimitive(number_of_bonds_, "number_of_bonds_");
			pm.writeObjectPointerArray((Bond**)bond_, "bond_", (Size)number_of_bonds_);
		pm.writeObjectTrailer(name);
	}

	void Atom::persistentRead(PersistenceManager& pm)
	{
		pm.checkObjectHeader(RTTI::getStreamName<Composite>());
			Composite::persistentRead(pm);
		pm.checkObjectTrailer(0);

		pm.readStorableObject(dynamic_cast<PropertyManager&>(*this),
													"PropertyManager");

		String s;
		pm.readPrimitive(s, "element_");
		element_ = &PTE[s];
		pm.readPrimitive(formal_charge_, "formal_charge_");
		pm.readPrimitive(charge_, "charge_");
		pm.readPrimitive(radius_, "radius_");
		pm.readPrimitive(name_, "name_");
		pm.readPrimitive(type_name_, "type_name_");
		Index tmp_type;
		pm.readPrimitive(tmp_type, "type_");
		type_ = (Atom::Type)tmp_type;

		pm.readStorableObject(position_, "position_");
		pm.readStorableObject(velocity_, "velocity_");
		pm.readStorableObject(force_, "force_");

		pm.readPrimitive(number_of_bonds_, "number_of_bonds_");
		Size n;
		pm.readObjectPointerArray(bond_, "bond_", n);
		if (n != (Size)number_of_bonds_)
		{
			Log.error() << "Atom::persistentRead: size of bond array: read " << n
				<< " instead of " << number_of_bonds_ << endl;
		}
	}

  void Atom::set(const Atom& atom, bool deep)
  {
    Composite::set(atom, deep);
    PropertyManager::operator = (atom);

    element_ = atom.element_;
    name_ = atom.name_;
    type_name_ = atom.type_name_;
    radius_ = atom.radius_;
		type_ = atom.type_;
    number_of_bonds_ = 0;
		formal_charge_ = atom.formal_charge_;
		position_ = atom.position_;
		charge_ = atom.charge_;
		velocity_ = atom.velocity_;
		force_ = atom.force_;
  }

	Atom& Atom::operator = (const Atom& atom)
	{
		Composite::operator =(atom);
		PropertyManager::operator = (atom);

		name_ = atom.name_;
		type_name_ = atom.type_name_;
		element_ = atom.element_;
		radius_ = atom.radius_;
		number_of_bonds_ = 0;
		type_ = atom.type_;
		formal_charge_ = atom.formal_charge_;
		position_ = atom.position_;
		charge_ = atom.charge_;
		velocity_ = atom.velocity_;
		force_ = atom.force_;

		return *this;
	}

	void Atom::swap(Atom &atom)
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

		std::swap(type_, atom.type_);
		std::swap(number_of_bonds_, atom.number_of_bonds_);
		std::swap(formal_charge_, atom.formal_charge_);
		std::swap(position_, atom.position_);
		std::swap(charge_, atom.charge_);
		std::swap(velocity_, atom.velocity_);
		std::swap(force_, atom.force_);
	}

	Molecule* Atom::getMolecule()
	{
		return Composite::getAncestor(RTTI::getDefault<Molecule>());
	}

	Fragment* Atom::getFragment()
	{
		return Composite::getAncestor(RTTI::getDefault<Fragment>());
	}

	Residue* Atom::getResidue()
	{
		return Composite::getAncestor(RTTI::getDefault<Residue>());
	}

	Chain* Atom::getChain()
	{
		return Composite::getAncestor(RTTI::getDefault<Chain>());
	}

	SecondaryStructure* Atom::getSecondaryStructure()
	{
		return Composite::getAncestor(RTTI::getDefault<SecondaryStructure>());
	}

	String Atom::getFullName(Atom::FullNameType type) const
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

		// add the parent name only if non-empty
		if (parent_name != ":")
		{
			name = parent_name + name;
		}

		return name;
	}

	Bond* Atom::getBond(Position index)
	{
		if (index >= (Index)MAX_NUMBER_OF_BONDS)
		{
			throw Exception::IndexOverflow(__FILE__, __LINE__, (Index)index, (Size)MAX_NUMBER_OF_BONDS);
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
	{
		return ((Atom*)this)->getBond(atom);
	}

	Bond* Atom::createBond(Atom& atom)
	{
		// Check whether this bond exists already
		Bond* bond = getBond(atom);

		// If the atom does not yet exist and the bond is not
		// to this atom, we have to construct a new one.
		if ((bond == 0) && (&atom != this))
		{
			// No, we have to create a new one.
			bond = Bond::createBond(*new Bond, *this, atom);
		}
		return bond;
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
			bond_ptr = (Bond*)bond.create();
			bond_ptr->setFirstAtom(0);
			bond_ptr->setSecondAtom(0);

			try
			{
				bond_ptr = Bond::createBond(*bond_ptr, *this, atom);
			}
			catch (Exception::TooManyBonds&)
			{
				// Clear up the mess we made.
				delete bond_ptr;
				bond_ptr = 0;
			}
		}
		return bond_ptr;
	}

	bool Atom::destroyBond(const Atom& atom)
	{
		Bond* bond = getBond(atom);

		if (bond == 0)
		{
			return false;
		}

		if (bond->isAutoDeletable())
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
	{
		for (int i = char(number_of_bonds_) - 1; i >= 0; --i)
		{
			if (bond_[i]->isAutoDeletable())
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

	Atom* Atom::getPartnerAtom(Position i)
	{
		return getBond(i)->getBoundAtom(*this);
	}

	const Atom* Atom::getPartnerAtom(Position i) const
	{
		return getBond(i)->getBoundAtom(*this);
	}

	bool Atom::hasBond(const Bond& bond) const
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

	bool Atom::isBoundTo(const Atom& atom) const
	{
		const Bond* bond = getBond(atom);
		if (bond != 0)
		{
			// Ignore H-bonds
			return (bond->getType() != Bond::TYPE__HYDROGEN);
		}
		return false;
	}

	bool Atom::isGeminal(const Atom& atom) const
	{
		if (atom == *this) return false;

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
	{
		if (atom == *this)
		{
			return false;
		}

		// an atom is vicinal to another, if it 
		// is not directly bonded to it
		bool is_vicinal = !isBoundTo(atom);

		// second, it has to be bonded to an atom
		// that is bonded to an atom that is bonded 
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
	{
		if (!Composite::isValid()
				|| !PropertyManager::isValid()
				|| element_ == 0)
		{
			return false;
		}

		for (int i = 0; i < number_of_bonds_; ++i)
		{
			if (!bond_[i]->isValid())
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
	{
		BALL_DUMP_STREAM_PREFIX(s);

		Composite::dump(s, depth);

		BALL_DUMP_DEPTH(s, depth);
		s << "  element: " << *element_ << endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "  formal charge: " << formal_charge_ << endl;

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

	bool Atom::applyBonds(UnaryProcessor<Bond>& processor)

	{
		if (!processor.start())
		{
			return false;
		}
		Processor::Result result;

		for (int i = 0; i < number_of_bonds_; ++i)
		{
			result = processor(*bond_[i]);

			if (result <= Processor::BREAK)
			{
				return result == Processor::BREAK;
			}
		}

		return processor.finish();
	}

	void Atom::clear_()
	{
		name_ = BALL_ATOM_DEFAULT_NAME;
		type_name_ = BALL_ATOM_DEFAULT_TYPE_NAME;
		element_ = BALL_ATOM_DEFAULT_ELEMENT;
		radius_ = BALL_ATOM_DEFAULT_RADIUS;
		type_ = BALL_ATOM_DEFAULT_TYPE;
		formal_charge_ = BALL_ATOM_DEFAULT_FORMAL_CHARGE;
		position_.set(BALL_ATOM_DEFAULT_POSITION);
		charge_ = BALL_ATOM_DEFAULT_CHARGE;
		velocity_.set(BALL_ATOM_DEFAULT_VELOCITY);
		force_.set(BALL_ATOM_DEFAULT_FORCE);

		delete interactions;
		interactions = 0;

		destroyBonds();
	}

	void Atom::swapLastBond_(const Atom *atom)
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

	void Atom::addInteraction(const Atom* atom, String interaction_type, double energy)
	{
		if(store_interactions_disabled_) return;
		if(interactions==NULL) interactions = new MolecularInteractions;
		interactions->addInteraction(atom, interaction_type, energy);
	}

	void Atom::addInteraction(String interaction_type, double energy)
	{
		if(store_interactions_disabled_) return;
		if(interactions==NULL) interactions = new MolecularInteractions;
		interactions->addInteraction(interaction_type, energy);
	}

	void Atom::disableStoreInteractions()
	{
		store_interactions_disabled_ = 1;
	}

	void Atom::enableStoreInteractions()
	{
		store_interactions_disabled_ = 0;
	}

# ifdef BALL_NO_INLINE_FUNCTIONS
#   include <BALL/KERNEL/atom.iC>
# endif

} // namespace BALL
