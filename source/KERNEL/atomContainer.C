// $Id: atomContainer.C,v 1.4 2001/01/14 21:57:15 amoll Exp $

#include <BALL/KERNEL/atomContainer.h>

#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/global.h>
#include <BALL/KERNEL/molecule.h>

using namespace std;

namespace BALL 
{

	AtomContainer::AtomContainer()
		throw()
		:	Composite(),
			PropertyManager(),
			name_(BALL_ATOMCONTAINER_DEFAULT_NAME)
	{
	}
		
	AtomContainer::AtomContainer(const AtomContainer& atom_container, bool deep)
		throw()
		:	Composite(),
			PropertyManager(),
			name_()
	{
		set(atom_container, deep);
	}
		
	AtomContainer::AtomContainer(const String& name)
		throw()
		:	Composite(),
			PropertyManager(),
			name_(name)
	{
	}

	AtomContainer::~AtomContainer()
		throw()
	{
		destroy();
	}

	void AtomContainer::clear()
		throw()
	{
		Composite::clear();
		PropertyManager::clear();

		name_ = BALL_ATOMCONTAINER_DEFAULT_NAME;
	}
		
	void AtomContainer::destroy()
		throw()
	{
		Composite::destroy();
		PropertyManager::destroy();

		name_ = BALL_ATOMCONTAINER_DEFAULT_NAME;
	}
		
	void AtomContainer::persistentWrite(PersistenceManager& pm, const char* name) const
		throw()
	{
		pm.writeObjectHeader(this, name);
			Composite::persistentWrite(pm);

			pm.writeStorableObject(*(PropertyManager*)this, "PropertyManager");

			pm.writePrimitive(name_, "name_");
		pm.writeObjectTrailer(name);
	}

	void AtomContainer::persistentRead(PersistenceManager& pm)
		throw()
	{
		pm.checkObjectHeader(RTTI::getStreamName<Composite>());
			Composite::persistentRead(pm);
		pm.checkObjectTrailer(0);

		pm.readStorableObject(*(PropertyManager*)this, "PropertyManager");
				
		pm.readPrimitive(name_, "name_");
	}

	void AtomContainer::set(const AtomContainer& atom_container, bool deep)
		throw()
	{
    bool clone_them = clone_bonds;
    clone_bonds = false;

		Composite::set(atom_container, deep);
		PropertyManager::set(atom_container, deep);
		name_ = atom_container.name_;

		// clone the bonds only of we are the outmost set method
		// involved!
    if (clone_them && deep)
    {
      BALL::cloneBonds(atom_container, *this);
		}

    clone_bonds = clone_them;
	}
			
	const AtomContainer& AtomContainer::operator = (const AtomContainer& atom_container)
		throw()
	{
		set(atom_container);
		return *this;
	}

	void AtomContainer::get(AtomContainer& atom_container, bool deep) const
		throw()
	{
		atom_container.set(*this, deep);
	}
			
	void AtomContainer::swap(AtomContainer& atom_container)
		throw()
	{
		Composite::swap(atom_container);
		PropertyManager::swap(atom_container);
		name_.swap(atom_container.name_);
	}
		
	void AtomContainer::setName(const String& name)
		throw()
	{
		name_ = name;
	}

	String& AtomContainer::getName()
		throw()
  {
	  return name_;
	}

	const String& AtomContainer::getName() const
		throw()
	{
		return name_;
	}

	Molecule* AtomContainer::getMolecule()
		throw()
	{
		for (Composite::AncestorIterator ancestor_it = beginAncestor();
				 !ancestor_it.isEnd(); ++ancestor_it)
		{
			if (RTTI::isKindOf<Molecule>(*ancestor_it) == true)
			{
				return (Molecule *)&*ancestor_it;
			}
		}

		return 0;
	}

	const Molecule* AtomContainer::getMolecule() const
		throw()
	{
		return ((AtomContainer *)this)->getMolecule();
	}

	AtomContainer* AtomContainer::getSuperAtomContainer()
		throw()
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
		throw()
	{
		return ((AtomContainer *)this)->getSuperAtomContainer();
	}

	AtomContainer* AtomContainer::getAtomContainer(Position position)
		throw()
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
		throw()
	{
		return ((AtomContainer *)this)->getAtomContainer(position);
	}

	Atom* AtomContainer::getAtom(Position position)
		throw()
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
		throw()
	{
		return ((AtomContainer *)this)->getAtom(position);
	}

	Atom* AtomContainer::getAtom(const String& name)
		throw()
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
		throw()
	{
		return ((AtomContainer *)this)->getAtom(name);
	}

	Size AtomContainer::countAtomContainers() const
		throw()
	{
		Size size = 0;

		for (AtomContainerIterator atom_container_it = ++beginAtomContainer();
				 !atom_container_it.isEnd();++atom_container_it)
		{
			++size;
		}

		return size;
	}

	Size AtomContainer::countAtoms() const
		throw()
	{
		Size size = 0;

		for (AtomIterator atom_it = beginAtom(); !atom_it.isEnd(); ++atom_it)
		{
			++size;
		}

		return size;
	}

	Size  AtomContainer::countBonds() const
		throw()
	{
		Size								size = 0;
		AtomIterator				atom_it;
		Atom::BondIterator	bond_it;

		BALL_FOREACH_BOND(*this, atom_it, bond_it)
		{
			++size;
		}

		return size;
	}

	Size AtomContainer::countInterBonds() const
		throw()
	{
		Size size = 0;
		AtomIterator atom_it;
		Atom::BondIterator bond_it;

		BALL_FOREACH_INTERBOND(*this, atom_it, bond_it)
		{
			++size;
		}

		return size;
	}

	Size AtomContainer::countIntraBonds() const
		throw()
	{
		Size size = 0;
		AtomIterator atom_it;
		Atom::BondIterator bond_it;

		BALL_FOREACH_INTRABOND(*this, atom_it, bond_it)
		{
			++size;
		}

		return size;
	}

	void AtomContainer::prepend(Atom& atom)
		throw()
	{
		Composite::prependChild(atom);
	}

	void AtomContainer::append(Atom &atom)
		throw()
	{
		Composite::appendChild(atom);
	}

	void AtomContainer::insert(Atom &atom)
		throw()
	{
		append(atom);
	}

	void AtomContainer::insertBefore(Atom &atom, Composite& before)
		throw()
	{
		before.Composite::insertBefore(atom);
	}

	void AtomContainer::insertAfter(Atom& atom, Composite &after)
		throw()
	{
		after.Composite::insertAfter(atom);
	}

	bool AtomContainer::remove(Atom& atom)
		throw()
	{
		return Composite::removeChild(atom);
	}

	void AtomContainer::prepend(AtomContainer& atom_container)
		throw()
	{
		Composite::prependChild(atom_container);
	}

	void AtomContainer::append(AtomContainer& atom_container)
		throw()
	{
		Composite::appendChild(atom_container);
	}

	void AtomContainer::insert(AtomContainer& atom_container)
		throw()
	{
		append(atom_container);
	}

	void AtomContainer::insertBefore(AtomContainer& atom_container, Composite& before)
		throw()
	{
		before.Composite::insertBefore(atom_container);
	}

	void AtomContainer::insertAfter(AtomContainer& atom_container, Composite& after)
		throw()
	{
		after.Composite::insertAfter(atom_container);
	}

	void AtomContainer::spliceBefore(AtomContainer& atom_container)
		throw()
	{
		Composite::spliceBefore(atom_container);
	}

	void AtomContainer::spliceAfter(AtomContainer& atom_container)
		throw()
	{
		Composite::spliceAfter(atom_container);
	}

	void AtomContainer::splice(AtomContainer& atom_container)
		throw()
	{
		Composite::splice(atom_container);
	}

	bool AtomContainer::remove(AtomContainer& atom_container)
		throw()
	{
		return Composite::removeChild(atom_container);
	}

	void AtomContainer::destroyBonds()
		throw()
	{
		for (AtomIterator atom_it = beginAtom(); !atom_it.isEnd(); ++atom_it)
		{
			(*atom_it).destroyBonds();
		}
	}

	bool AtomContainer::isSubAtomContainerOf(const AtomContainer& atom_container) const
		throw()
	{
		return atom_container.isAncestorOf(*this);
	}

	bool AtomContainer::isSuperAtomContainerOf(const AtomContainer& atom_container) const
		throw()
	{
		return isAncestorOf(atom_container);
	}

	bool AtomContainer::isValid() const
		throw()
	{ 
		return (Composite::isValid() && PropertyManager::isValid() && name_.isValid());
	}

	void AtomContainer::dump(ostream& s, Size depth) const
		throw()
	{
		BALL_DUMP_STREAM_PREFIX(s);
		
		Composite::dump(s, depth);

		BALL_DUMP_DEPTH(s, depth);
		s << "  name: " << name_ << endl;
		
		BALL_DUMP_STREAM_SUFFIX(s);
	}

	void AtomContainer::read(istream&  /* s */)
		throw()
	{
		throw Exception::NotImplemented(__FILE__, __LINE__);
	}

	void AtomContainer::write(ostream&  /* s */) const
		throw()
	{
		throw Exception::NotImplemented(__FILE__, __LINE__);
	}


	bool AtomContainer::applyInterBond(UnaryProcessor<Bond>& processor)
		throw()
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
		throw()
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
		throw()
	{
		return(name_	== name_													&&
			PropertyManager::operator == (atom_container) &&
						Composite::operator	== (atom_container) );
	}

	bool AtomContainer::operator != (const AtomContainer& atom_container) const
		throw()
	{
		return ! (*this == atom_container);
	}


} // namespace BALL 
