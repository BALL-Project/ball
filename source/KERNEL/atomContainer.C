// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/KERNEL/atomContainer.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/global.h>

using namespace::std;
namespace BALL
{

	AtomContainer::AtomContainer()
		:	Composite(),
			PropertyManager(),
			name_(BALL_ATOMCONTAINER_DEFAULT_NAME)
	{
	}

	AtomContainer::AtomContainer(const AtomContainer& atom_container, bool deep)
		:	Composite(),
			PropertyManager(),
			name_()
	{
		set(atom_container, deep);
	}

	AtomContainer::AtomContainer(const String& name)
		:	Composite(),
			PropertyManager(),
			name_(name)
	{
	}

	AtomContainer::~AtomContainer()
	{
		destroy();
	}

	void AtomContainer::clear()
	{
		Composite::clear();
		PropertyManager::clear();

		name_ = BALL_ATOMCONTAINER_DEFAULT_NAME;
	}

	void AtomContainer::destroy()
	{
		Composite::destroy();
		PropertyManager::destroy();

		name_ = BALL_ATOMCONTAINER_DEFAULT_NAME;
	}

	void AtomContainer::persistentWrite(PersistenceManager& pm, const char* name) const
	{
		pm.writeObjectHeader(this, name);
			Composite::persistentWrite(pm);

			pm.writeStorableObject(dynamic_cast<const PropertyManager&>(*this), "PropertyManager");

			pm.writePrimitive(name_, "name_");
		pm.writeObjectTrailer(name);
	}

	void AtomContainer::persistentRead(PersistenceManager& pm)
	{
		pm.checkObjectHeader(RTTI::getStreamName<Composite>());
			Composite::persistentRead(pm);
		pm.checkObjectTrailer(0);

		pm.readStorableObject(dynamic_cast<PropertyManager&>(*this), "PropertyManager");

		pm.readPrimitive(name_, "name_");
	}

	void AtomContainer::set(const AtomContainer& atom_container, bool deep)
	{
    bool clone_them = clone_bonds;
    clone_bonds = false;

		Composite::set(atom_container, deep);
		PropertyManager::set(atom_container);
		name_ = atom_container.name_;

		// clone the bonds only of we are the outmost set method
		// involved!
    if (clone_them && deep)
    {
			BALL::cloneBonds(atom_container, *this);
		}

    clone_bonds = clone_them;
	}

	AtomContainer& AtomContainer::operator = (const AtomContainer& atom_container)
	{
		set(atom_container);
		return *this;
	}

	void AtomContainer::get(AtomContainer& atom_container, bool deep) const
	{
		atom_container.set(*this, deep);
	}

	void AtomContainer::swap(AtomContainer& atom_container)
	{
		Composite::swap(atom_container);
		PropertyManager::swap(atom_container);
		name_.swap(atom_container.name_);
	}

	void AtomContainer::setName(const String& name)
	{
		name_ = name;
	}

	const String& AtomContainer::getName() const
	{
		return name_;
	}

	AtomContainer* AtomContainer::getSuperAtomContainer()
	{
		for (Composite::AncestorIterator ancestor_it = beginAncestor();
				 !ancestor_it.isEnd(); ++ancestor_it)
		{
			if (RTTI::isKindOf<AtomContainer>(*ancestor_it) == true)
			{
				return (AtomContainer *)&*ancestor_it;
			}
		}

		return 0;
	}

	const AtomContainer* AtomContainer::getSuperAtomContainer() const
	{
		return ((AtomContainer *)this)->getSuperAtomContainer();
	}

	AtomContainer* AtomContainer::getAtomContainer(Position position)
	{
		for (AtomContainerIterator atom_container_it = ++beginAtomContainer();
				 !atom_container_it.isEnd(); ++atom_container_it)
		{
			if (position-- == 0)
			{
				return &(*atom_container_it);
			}
		}

		return 0;
	}

	const AtomContainer*  AtomContainer::getAtomContainer(Position position) const
	{
		return ((AtomContainer *)this)->getAtomContainer(position);
	}

	Atom* AtomContainer::getAtom(Position position)
	{
		for (AtomIterator atom_it = beginAtom(); !atom_it.isEnd(); ++atom_it)
		{
			if (position-- == 0)
			{
				return &(*atom_it);
			}
		}

		return 0;
	}

	const Atom* AtomContainer::getAtom(Position position) const
	{
		return ((AtomContainer *)this)->getAtom(position);
	}

	Atom* AtomContainer::getAtom(const String& name)
	{
		for (AtomIterator atom_it = beginAtom(); !atom_it.isEnd(); ++atom_it)
		{
			if ((*atom_it).getName() == name)
			{
				return &(*atom_it);
			}
		}

		return 0;
	}

	const Atom* AtomContainer::getAtom(const String& name) const
	{
		return ((AtomContainer *)this)->getAtom(name);
	}

	Size AtomContainer::countAtomContainers() const
	{
		Size size = 0;

		for (AtomContainerConstIterator atom_container_it = ++beginAtomContainer();
				 !atom_container_it.isEnd(); ++atom_container_it)
		{
			++size;
		}

		return size;
	}

	Size AtomContainer::countAtoms() const
	{
		Size size = 0;

		for (AtomConstIterator atom_it(beginAtom()); !atom_it.isEnd(); ++atom_it)
		{
			++size;
		}

		return size;
	}

	Size  AtomContainer::countBonds() const
	{
		Size								size = 0;
		AtomConstIterator				atom_it;
		Atom::BondConstIterator	bond_it;

		BALL_FOREACH_BOND(*this, atom_it, bond_it)
		{
			++size;
		}

		return size;
	}

	Size AtomContainer::countInterBonds() const
	{
		Size size = 0;
		AtomConstIterator atom_it;
		Atom::BondConstIterator bond_it;

		BALL_FOREACH_INTERBOND(*this, atom_it, bond_it)
		{
			++size;
		}

		return size;
	}

	Size AtomContainer::countIntraBonds() const
	{
		Size size = 0;
		AtomConstIterator atom_it;
		Atom::BondConstIterator bond_it;

		BALL_FOREACH_INTRABOND(*this, atom_it, bond_it)
		{
			++size;
		}

		return size;
	}

	void AtomContainer::prepend(Atom& atom)
	{
		Composite::prependChild(atom);
	}

	void AtomContainer::append(Atom &atom)
	{
		Composite::appendChild(atom);
	}

	void AtomContainer::insert(Atom &atom)
	{
		append(atom);
	}

	void AtomContainer::insertBefore(Atom &atom, Composite& before)
	{
		before.Composite::insertBefore(atom);
	}

	void AtomContainer::insertAfter(Atom& atom, Composite &after)
	{
		after.Composite::insertAfter(atom);
	}

	bool AtomContainer::remove(Atom& atom)
	{
		return Composite::removeChild(atom);
	}

	Size AtomContainer::removeHavingProperty(BALL::Property p)
	{
		// Collect all unselected composites in a list.
		std::list<Composite*> atoms;
		for (AtomIterator ai = beginAtom(); +ai; ++ai)
		{
			if (ai->hasProperty(p))
			{
				atoms.push_back(&*ai);
			}
		}

		deleteChildrenList_(atoms);

		// Return the number of composites deleted.
		return atoms.size();
	}

	Size AtomContainer::removeNotHavingProperty(BALL::Property p)
	{
		// Collect all unselected composites in a list.
		std::list<Composite*> atoms;
		for (AtomIterator ai = beginAtom(); +ai; ++ai)
		{
			if (!ai->hasProperty(p))
			{
				atoms.push_back(&*ai);
			}
		}

		deleteChildrenList_(atoms);

		// Return the number of composites deleted.
		return atoms.size();
	}

	Size AtomContainer::removeHavingProperty(const string& name)
	{
		// Collect all unselected composites in a list.
		std::list<Composite*> atoms;
		for (AtomIterator ai = beginAtom(); +ai; ++ai)
		{
			if (ai->hasProperty(name))
			{
				atoms.push_back(&*ai);
			}
		}

		deleteChildrenList_(atoms);

		// Return the number of composites deleted.
		return atoms.size();
	}

	Size AtomContainer::removeNotHavingProperty(const string& name)
	{
		// Collect all unselected composites in a list.
		std::list<Composite*> atoms;
		for (AtomIterator ai = beginAtom(); +ai; ++ai)
		{
			if (!ai->hasProperty(name))
			{
				atoms.push_back(&*ai);
			}
		}

		deleteChildrenList_(atoms);

		// Return the number of composites deleted.
		return atoms.size();
	}

	void AtomContainer::prepend(AtomContainer& atom_container)
	{
		Composite::prependChild(atom_container);
	}

	void AtomContainer::append(AtomContainer& atom_container)
	{
		Composite::appendChild(atom_container);
	}

	void AtomContainer::insert(AtomContainer& atom_container)
	{
		append(atom_container);
	}

	void AtomContainer::insertBefore(AtomContainer& atom_container, Composite& before)
	{
		before.Composite::insertBefore(atom_container);
	}

	void AtomContainer::insertAfter(AtomContainer& atom_container, Composite& after)
	{
		after.Composite::insertAfter(atom_container);
	}

	void AtomContainer::spliceBefore(AtomContainer& atom_container)
	{
		Composite::spliceBefore(atom_container);
	}

	void AtomContainer::spliceAfter(AtomContainer& atom_container)
	{
		Composite::spliceAfter(atom_container);
	}

	void AtomContainer::splice(AtomContainer& atom_container)
	{
		Composite::splice(atom_container);
	}

	bool AtomContainer::remove(AtomContainer& atom_container)
	{
		return Composite::removeChild(atom_container);
	}

	void AtomContainer::destroyBonds()
	{
		for (AtomIterator atom_it = beginAtom(); !atom_it.isEnd(); ++atom_it)
		{
			(*atom_it).destroyBonds();
		}
	}

	bool AtomContainer::isSubAtomContainerOf(const AtomContainer& atom_container) const
	{
		return atom_container.isAncestorOf(*this);
	}

	bool AtomContainer::isSuperAtomContainerOf(const AtomContainer& atom_container) const
	{
		return isAncestorOf(atom_container);
	}

	bool AtomContainer::isValid() const
	{
		return (Composite::isValid() && PropertyManager::isValid() && name_.isValid());
	}

	void AtomContainer::dump(ostream& s, Size depth) const
	{
		BALL_DUMP_STREAM_PREFIX(s);

		Composite::dump(s, depth);
		PropertyManager::dump(s, depth);

		BALL_DUMP_DEPTH(s, depth);
		s << "  name: " << name_ << endl;

		BALL_DUMP_STREAM_SUFFIX(s);
	}

	bool AtomContainer::applyInterBond(UnaryProcessor<Bond>& processor)
	{
		if (processor.start() == false)
		{
			return false;
		}

		Processor::Result		result;
		AtomIterator				atom_it;
		Atom::BondIterator	bond_it;

		BALL_FOREACH_INTERBOND(*this, atom_it, bond_it)
		{
			result = processor(*bond_it);

			if (result <= Processor::BREAK)
			{
				return (result == Processor::BREAK) ? true : false;
			}
		}

		return processor.finish();
	}

	bool AtomContainer::applyIntraBond(UnaryProcessor<Bond> &processor)
	{
    if (processor.start() == false)
		{
      return false;
		}

		Processor::Result  result;
		AtomIterator			 atom_it;
		Atom::BondIterator bond_it;

		BALL_FOREACH_INTRABOND(*this, atom_it, bond_it)
		{
			result = processor(*bond_it);

			if (result <= Processor::BREAK)
			{
				return (result == Processor::BREAK) ? true : false;
			}
		}

		return processor.finish();
	}

	bool AtomContainer::operator == (const AtomContainer& atom_container) const
	{
		return(Object::operator == (atom_container));
	}

	bool AtomContainer::operator != (const AtomContainer& atom_container) const
	{
		return ! (*this == atom_container);
	}


} // namespace BALL 
