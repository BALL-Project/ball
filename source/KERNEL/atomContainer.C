// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/KERNEL/atomContainer.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/global.h>
#include <BALL/KERNEL/system.h>          // K0.4.3: auto-adopt into root System
#include <BALL/CONCEPT/composite.h>      // ancestor lookup
#include <BALL/COMMON/rtti.h>            // RTTI::getDefault<System>

using namespace::std;
namespace BALL
{

	// K0.4.3 helper: walk up the Composite tree to find the root System.
	// Returns nullptr if this AtomContainer isn't rooted under a System.
	// File-local helper used by all the auto-adopting insert/prepend/append
	// overloads below.
	static System* findRootSystem_(Composite* node)
	{
		// Self-or-ancestor lookup. getAncestor with the System type
		// returns the nearest System enclosing this node, or null if none.
		if (node == nullptr) return nullptr;
		// If this node itself is a System, that's the root.
		if (System* self = dynamic_cast<System*>(node)) return self;
		return node->getAncestor(RTTI::getDefault<System>());
	}

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
            if (RTTI::isKindOf<AtomContainer>(&*ancestor_it))
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

	// 2026-05-18 (Codex R12 fix K8): pre-check canAdopt() BEFORE
	// Composite-tree insertion. Pre-fix, `Composite::prependChild` ran
	// first, then `sys->adopt(atom)` could soft-reject (bonded atom
	// with partner in another store), leaving the atom in this
	// container's tree but still bound to its old store. Now if adopt
	// would fail, the whole prepend+adopt sequence is skipped — caller
	// gets the same Log.warn but tree state stays consistent.
	void AtomContainer::prepend(Atom& atom)
	{
		System* sys = findRootSystem_(this);
		if (sys != nullptr && !sys->canAdopt(atom))
		{
			(void) sys->adopt(atom);   // emits the Log.warn; no-op otherwise
			return;
		}
		Composite::prependChild(atom);
		// K0.4.3 auto-adopt: if rooted under a System, migrate the atom
		// from its current store (typically orphan) into the System's
		// store. Single-atom insert path; bond migration is trivial
		// (atom has no bonds to other atoms in src yet, or partner is
		// already in dst).
		if (sys != nullptr) sys->adopt(atom);
	}

	void AtomContainer::append(Atom &atom)
	{
		System* sys = findRootSystem_(this);
		if (sys != nullptr && !sys->canAdopt(atom))
		{
			(void) sys->adopt(atom);
			return;
		}
		Composite::appendChild(atom);
		if (sys != nullptr) sys->adopt(atom);
	}

	void AtomContainer::insert(Atom &atom)
	{
		append(atom);
	}

	void AtomContainer::insertBefore(Atom &atom, Composite& before)
	{
		System* sys = findRootSystem_(this);
		if (sys != nullptr && !sys->canAdopt(atom))
		{
			(void) sys->adopt(atom);
			return;
		}
		before.Composite::insertBefore(atom);
		if (sys != nullptr) sys->adopt(atom);
	}

	void AtomContainer::insertAfter(Atom& atom, Composite &after)
	{
		// R12 fix K8: pre-check canAdopt before tree insertion.
		System* sys = findRootSystem_(this);
		if (sys != nullptr && !sys->canAdopt(atom))
		{
			(void) sys->adopt(atom);
			return;
		}
		after.Composite::insertAfter(atom);
		if (sys != nullptr) sys->adopt(atom);
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
		// K0.4.3 batch auto-adopt: subtree's atoms migrate together,
		// preserving intra-subtree bonds (closes the K0.4.2 sequential-
		// adopt orphan-bond limitation).
		if (System* sys = findRootSystem_(this)) sys->adoptSubtree(atom_container);
	}

	void AtomContainer::append(AtomContainer& atom_container)
	{
		Composite::appendChild(atom_container);
		if (System* sys = findRootSystem_(this)) sys->adoptSubtree(atom_container);
	}

	void AtomContainer::insert(AtomContainer& atom_container)
	{
		append(atom_container);
	}

	void AtomContainer::insertBefore(AtomContainer& atom_container, Composite& before)
	{
		before.Composite::insertBefore(atom_container);
		if (System* sys = findRootSystem_(this)) sys->adoptSubtree(atom_container);
	}

	void AtomContainer::insertAfter(AtomContainer& atom_container, Composite& after)
	{
		after.Composite::insertAfter(atom_container);
		if (System* sys = findRootSystem_(this)) sys->adoptSubtree(atom_container);
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
		if (!processor.start())
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
				return result == Processor::BREAK;
			}
		}

		return processor.finish();
	}

	bool AtomContainer::applyIntraBond(UnaryProcessor<Bond> &processor)
	{
    if (!processor.start())
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
				return result == Processor::BREAK;
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
