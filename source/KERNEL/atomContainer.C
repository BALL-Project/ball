// $Id: atomContainer.C,v 1.1 2000/08/30 19:58:31 oliver Exp $

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

		clear_();
	}
		
	void AtomContainer::destroy()
	{
		Composite::destroy();
		PropertyManager::destroy();

		clear_();
	}
		
	void AtomContainer::persistentWrite(PersistenceManager& pm, const char* name) const
	{
		pm.writeObjectHeader(this, name);
			Composite::persistentWrite(pm);

			pm.writeStorableObject(*(PropertyManager*)this, "PropertyManager");

			pm.writePrimitive(name_, "name_");
		pm.writeObjectTrailer(name);
	}

	void AtomContainer::persistentRead(PersistenceManager& pm)
	{
		pm.checkObjectHeader(RTTI::getStreamName<Composite>());
			Composite::persistentRead(pm);
		pm.checkObjectTrailer(0);

		pm.readStorableObject(*(PropertyManager*)this, "PropertyManager");
				
		pm.readPrimitive(name_, "name_");
	}

	void AtomContainer::set(const AtomContainer& atom_container, bool deep)
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

	String& AtomContainer::getName()
  {
	  return name_;
	}

	const String& AtomContainer::getName() const
	{
		return name_;
	}

	Molecule* AtomContainer::getMolecule()
	{
		for (Composite::AncestorIterator ancestor_it = beginAncestor();
				 !ancestor_it.isEnd(); ++ancestor_it)
		{
			if (RTTI::isKindOf<Molecule>(*ancestor_it) == true)
				return (Molecule *)&*ancestor_it;
		}

		return 0;
	}

	const Molecule* AtomContainer::getMolecule() const
	{
		return ((AtomContainer *)this)->getMolecule();
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
		register Size size = 0;

		for (AtomContainerIterator atom_container_it = ++beginAtomContainer();
				 !atom_container_it.isEnd();++atom_container_it)
		{
			++size;
		}

		return size;
	}

	Size AtomContainer::countAtoms() const
	{
		register Size size = 0;

		for (AtomIterator atom_it = beginAtom(); !atom_it.isEnd(); ++atom_it)
		{
			++size;
		}

		return size;
	}

	Size  AtomContainer::countBonds() const
	{
		register Size			size = 0;
		AtomIterator			atom_it;
		Atom::BondIterator	bond_it;

		BALL_FOREACH_BOND(*this, atom_it, bond_it)
		{
			++size;
		}

		return size;
	}

	Size AtomContainer::countInterBonds() const
	{
		register Size size = 0;
		AtomIterator atom_it;
		Atom::BondIterator bond_it;

		BALL_FOREACH_INTERBOND(*this, atom_it, bond_it)
		{
			++size;
		}

		return size;
	}

	Size AtomContainer::countIntraBonds() const
	{
		register Size size = 0;
		AtomIterator atom_it;
		Atom::BondIterator bond_it;

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
		if (Composite::isValid() == false
				|| PropertyManager::isValid() == false
				|| name_.isValid() == false)
		{
			return false;
		}

		return true;
	}

	void AtomContainer::dump(ostream& s, Size depth) const
	{
		BALL_DUMP_STREAM_PREFIX(s);
		
		Composite::dump(s, depth);

		BALL_DUMP_DEPTH(s, depth);
		s << "  name: " << name_ << endl;
		
		BALL_DUMP_STREAM_SUFFIX(s);
	}

	void AtomContainer::read(istream&  /* s */)
	{
		throw Exception::NotImplemented(__FILE__, __LINE__);
	}

	void AtomContainer::write(ostream&  /* s */) const
	{
		throw Exception::NotImplemented(__FILE__, __LINE__);
	}


	bool AtomContainer::applyInterBond(UnaryProcessor<Bond>& processor)
	{
		if (processor.start() == false)
			return false;

		Processor::Result result;

		AtomIterator atom_it;
		Atom::BondIterator bond_it;

		BALL_FOREACH_INTERBOND(*this, atom_it, bond_it)
		{
			result = processor(*bond_it);

			if (result <= Processor::BREAK)
				return (result == Processor::BREAK) ? true : false;
		}

		return processor.finish();
	}

	bool AtomContainer::applyIntraBond(UnaryProcessor<Bond> &processor)
	{
    if (processor.start() == false)
      return false;
 
		Processor::Result result;

		AtomIterator atom_it;
		Atom::BondIterator bond_it;

		BALL_FOREACH_INTRABOND(*this, atom_it, bond_it)
		{
			result = processor(*bond_it);

			if (result <= Processor::BREAK)
				return (result == Processor::BREAK) ? true : false;
		}
 
		return processor.finish();
	}

	void AtomContainer::clear_()
	{
		name_ = BALL_ATOMCONTAINER_DEFAULT_NAME;
	}

} // namespace BALL 
