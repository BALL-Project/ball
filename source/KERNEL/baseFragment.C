// $Id: baseFragment.C,v 1.6 2000/04/17 13:46:04 amoll Exp $

#include <BALL/KERNEL/baseFragment.h>

#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/global.h>
#include <BALL/KERNEL/molecule.h>

using namespace std;

namespace BALL 
{

	BaseFragment::BaseFragment()
		:	Composite(),
			PropertyManager(),
			name_(BALL_BASEFRAGMENT_DEFAULT_NAME)
	{
	}
		
	BaseFragment::BaseFragment(const BaseFragment& base_fragment, bool deep)
		:	Composite(),
			PropertyManager(),
			name_()
	{
		set(base_fragment, deep);
	}
		
	BaseFragment::BaseFragment(const String& name)
		:	Composite(),
			PropertyManager(),
			name_(name)
	{
	}

	BaseFragment::~BaseFragment()
	{
		destroy();
	}

	void BaseFragment::clear()
	{
		Composite::clear();
		PropertyManager::clear();

		clear_();
	}
		
	void BaseFragment::destroy()
	{
		Composite::destroy();
		PropertyManager::destroy();

		clear_();
	}
		
	void BaseFragment::persistentWrite(PersistenceManager& pm, const char* name) const
	{
		pm.writeObjectHeader(this, name);
			Composite::persistentWrite(pm);

			pm.writeStorableObject(*(PropertyManager*)this, "PropertyManager");

			pm.writePrimitive(name_, "name_");
		pm.writeObjectTrailer(name);
	}

	void BaseFragment::persistentRead(PersistenceManager& pm)
	{
		pm.checkObjectHeader(RTTI::getStreamName<Composite>());
			Composite::persistentRead(pm);
		pm.checkObjectTrailer(0);

		pm.readStorableObject(*(PropertyManager*)this, "PropertyManager");
				
		pm.readPrimitive(name_, "name_");
	}

	void BaseFragment::set(const BaseFragment& base_fragment, bool deep)
	{
    bool clone_them = clone_bonds;
    clone_bonds = false;

		Composite::set(base_fragment, deep);
		PropertyManager::set(base_fragment, deep);
		name_ = base_fragment.name_;

		// clone the bonds only of we are the outmost set method
		// involved!
    if (clone_them && deep)
    {
      BALL::cloneBonds(base_fragment, *this);
		}

    clone_bonds = clone_them;
	}
			
	BaseFragment& BaseFragment::operator = (const BaseFragment& base_fragment)
	{
		set(base_fragment);

		return *this;
	}

	void BaseFragment::get(BaseFragment& base_fragment, bool deep) const
	{
		base_fragment.set(*this, deep);
	}
			
	void BaseFragment::swap(BaseFragment& base_fragment)
	{
		Composite::swap(base_fragment);
		PropertyManager::swap(base_fragment);

		name_.swap(base_fragment.name_);
	}
		
	void BaseFragment::setName(const String& name)
	{
		name_ = name;
	}

	const String& BaseFragment::getName() const
	{
		return name_;
	}

	Molecule* BaseFragment::getMolecule()
	{
		for (Composite::AncestorIterator ancestor_it = beginAncestor();
				 !ancestor_it.isEnd(); ++ancestor_it)
		{
			if (RTTI::isKindOf<Molecule>(*ancestor_it) == true)
				return (Molecule *)&*ancestor_it;
		}

		return 0;
	}

	const Molecule* BaseFragment::getMolecule() const
	{
		return ((BaseFragment *)this)->getMolecule();
	}

	BaseFragment* BaseFragment::getSuperBaseFragment()
	{
		for (Composite::AncestorIterator ancestor_it = beginAncestor();
				 !ancestor_it.isEnd(); ++ancestor_it)
		{
			if (RTTI::isKindOf<BaseFragment>(*ancestor_it) == true)
			{
				return (BaseFragment *)&*ancestor_it;
			}
		}

		return 0;
	}

	const BaseFragment* BaseFragment::getSuperBaseFragment() const
	{
		return ((BaseFragment *)this)->getSuperBaseFragment();
	}

	BaseFragment* BaseFragment::getBaseFragment(Index index)
	{
		if (index < 0 )
			throw Exception::IndexUnderflow(__FILE__, __LINE__, index);

		for (BaseFragmentIterator base_fragment_it = ++beginBaseFragment();
				 !base_fragment_it.isEnd(); ++base_fragment_it)
		{
			if (index-- == 0)
			{
				return &(*base_fragment_it);
			}
		}

		return 0;
	}

	const BaseFragment*  BaseFragment::getBaseFragment(Index index) const
	{
		return ((BaseFragment *)this)->getBaseFragment(index);
	}

	Atom* BaseFragment::getAtom(Index index)
	{
		if (index < 0 )
			throw Exception::IndexUnderflow(__FILE__, __LINE__, index);

		for (AtomIterator atom_it = beginAtom(); !atom_it.isEnd(); ++atom_it)
		{
			if (index-- == 0)
			{
				return &(*atom_it);
			}
		}

		return 0;
	}

	const Atom* BaseFragment::getAtom(Index index) const
	{
		return ((BaseFragment *)this)->getAtom(index);
	}

	Atom* BaseFragment::getAtom(const String& name)
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

	const Atom* BaseFragment::getAtom(const String& name) const
	{
		return ((BaseFragment *)this)->getAtom(name);
	}

	Size BaseFragment::countBaseFragments() const
	{
		register Size size = 0;

		for (BaseFragmentIterator base_fragment_it = ++beginBaseFragment();
				 !base_fragment_it.isEnd();++base_fragment_it)
		{
			++size;
		}

		return size;
	}

	Size BaseFragment::countAtoms() const
	{
		register Size size = 0;

		for (AtomIterator atom_it = beginAtom(); !atom_it.isEnd(); ++atom_it)
		{
			++size;
		}

		return size;
	}

	Size  BaseFragment::countBonds() const
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

	Size BaseFragment::countInterBonds() const
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

	Size BaseFragment::countIntraBonds() const
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

	void BaseFragment::prepend(Atom& atom)
	{
		Composite::prependChild(atom);
	}

	void BaseFragment::append(Atom &atom)
	{
		Composite::appendChild(atom);
	}

	void BaseFragment::insert(Atom &atom)
	{
		append(atom);
	}

	void BaseFragment::insertBefore(Atom &atom, Composite& before)
	{
		before.Composite::insertBefore(atom);
	}

	void BaseFragment::insertAfter(Atom& atom, Composite &after)
	{
		after.Composite::insertAfter(atom);
	}

	bool BaseFragment::remove(Atom& atom)
	{
		return Composite::removeChild(atom);
	}

	void BaseFragment::prepend(BaseFragment& base_fragment)
	{
		Composite::prependChild(base_fragment);
	}

	void BaseFragment::append(BaseFragment& base_fragment)
	{
		Composite::appendChild(base_fragment);
	}

	void BaseFragment::insert(BaseFragment& base_fragment)
	{
		append(base_fragment);
	}

	void BaseFragment::insertBefore(BaseFragment& base_fragment, Composite& before)
	{
		before.Composite::insertBefore(base_fragment);
	}

	void BaseFragment::insertAfter(BaseFragment& base_fragment, Composite& after)
	{
		after.Composite::insertAfter(base_fragment);
	}

	void BaseFragment::spliceBefore(BaseFragment& base_fragment)
	{
		Composite::spliceBefore(base_fragment);
	}

	void BaseFragment::spliceAfter(BaseFragment& base_fragment)
	{
		Composite::spliceAfter(base_fragment);
	}

	void BaseFragment::splice(BaseFragment& base_fragment)
	{
		Composite::splice(base_fragment);
	}

	bool BaseFragment::remove(BaseFragment& base_fragment)
	{
		return Composite::removeChild(base_fragment);
	}

	void BaseFragment::destroyBonds()
	{
		for (AtomIterator atom_it = beginAtom(); !atom_it.isEnd(); ++atom_it)
		{
			(*atom_it).destroyBonds();
		}
	}

	bool BaseFragment::isSubBaseFragmentOf(const BaseFragment& base_fragment) const
	{
		return base_fragment.isAncestorOf(*this);
	}

	bool BaseFragment::isSuperBaseFragmentOf(const BaseFragment& base_fragment) const
	{
		return isAncestorOf(base_fragment);
	}

	bool BaseFragment::isValid() const
	{ 
		if (Composite::isValid() == false
				|| PropertyManager::isValid() == false
				|| name_.isValid() == false)
		{
			return false;
		}

		return true;
	}

	void BaseFragment::dump(ostream& s, Size depth) const
	{
		BALL_DUMP_STREAM_PREFIX(s);
		
		Composite::dump(s, depth);

		BALL_DUMP_DEPTH(s, depth);
		s << "  name: " << name_ << endl;
		
		BALL_DUMP_STREAM_SUFFIX(s);
	}

	void BaseFragment::read(istream&  /* s */)
	{
		throw Exception::NotImplemented(__FILE__, __LINE__);
	}

	void BaseFragment::write(ostream&  /* s */) const
	{
		throw Exception::NotImplemented(__FILE__, __LINE__);
	}


	bool BaseFragment::applyInterBond(UnaryProcessor<Bond>& processor)
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

	bool BaseFragment::applyIntraBond(UnaryProcessor<Bond> &processor)
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

	void BaseFragment::clear_()
	{
		name_ = BALL_BASEFRAGMENT_DEFAULT_NAME;
	}

} // namespace BALL 
