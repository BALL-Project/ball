// $Id: system.C,v 1.1 1999/08/26 08:02:34 oliver Exp $

#include <BALL/KERNEL/system.h>

#include <BALL/KERNEL/global.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/fragment.h>
#include <BALL/KERNEL/protein.h>

namespace BALL 
{

	System::System(void)
		:	Composite(),
			PropertyManager(),
			Selectable(),
			name_(BALL_SYSTEM_DEFAULT_NAME)
	{
	}
		
	System::System(const System& system,bool deep)
	{
		set(system, deep);
	}
		
	System::System(const String& name)
		:	Composite(),
			PropertyManager(),
			Selectable(),
			name_(name)
	{
	}

  void System::persistentWrite(PersistenceManager& pm, const char* name) const
  {
    pm.writeObjectHeader(this, name);
      Composite::persistentWrite(pm);

      pm.writeStorableObject(*(PropertyManager*)this, "PropertyManager");
      pm.writeStorableObject(*(Selectable*)this, "Selectable");
 
      pm.writePrimitive(name_, "name_");
    pm.writeObjectTrailer(name);
	}

  void System::persistentRead(PersistenceManager& pm)
  {
    pm.checkObjectHeader(RTTI<Composite>::getStreamName());
			Composite::persistentRead(pm);
    pm.checkObjectTrailer(0);

		pm.readStorableObject(*(PropertyManager*)this, "PropertyManager");
		pm.readStorableObject(*(Selectable*)this, "Selectable");

    pm.readPrimitive(name_, "name_");
	}
 

	System::~System(void)
	{
		destroy();
	}

	void System::clear(void)
	{
		Composite::clear();
		PropertyManager::clear();
		Selectable::clear();

		clear_();
	}
		
	void System::destroy(void)
	{
		Composite::destroy();
		PropertyManager::destroy();
		Selectable::destroy();

		clear_();
	}
		
	void System::set(const System& system, bool deep)
	{
		bool clone_them = clone_bonds;
		clone_bonds = false;

		Composite::set(system, deep);
		PropertyManager::set(system, deep);
		Selectable::set(system, deep);
		name_ = system.name_;

		if (clone_them && deep)
		{
			BALL::cloneBonds(system, *this);
		}
		
		clone_bonds = clone_them;
	}
			
	System& System::operator = (const System& system)
	{
		set(system);

		return *this;
	}

	void System::get(System& system, bool deep) const
	{
		system.set(*this, deep);
	}
			
	void System::swap(System& system)
	{
		Composite::swap(system);
		PropertyManager::swap(system);
		Selectable::swap(system);

		name_.swap(system.name_);
	}

	void System::setName(const String &name)
	{
		name_ = name;
	}

	String& System::getName(void)
	{
		return name_;
	}

	const String& System::getName(void) const
	{
		return name_;
	}

	Size System::countMolecules(void) const
	{
		Size size = 0;

		for (MoleculeIterator mol_it = beginMolecule(); !mol_it.isEnd(); ++mol_it)
			++size;

		return size;
	}

	Size  System::countFragments(void) const
	{
		Size size = 0;

		for (FragmentIterator frag_it = beginFragment(); !frag_it.isEnd(); ++frag_it)
			++size;

		return size;
	}

	Size System::countAtoms(void) const
	{
		Size size = 0;

		for (AtomIterator atom_it = beginAtom(); !atom_it.isEnd(); ++atom_it)
			++size;

		return size;
	}

	void System::prepend(Molecule& molecule)
	{
		Composite::prependChild(molecule);
	}

	void System::append(Molecule& molecule)
	{
		Composite::appendChild(molecule);
	}

	void System::insert(Molecule& molecule)
	{
		append(molecule);
	}

	void System::insertBefore(Molecule& molecule, Composite& before)
	{
		before.Composite::insertBefore(molecule);
	}

	void System::insertAfter(Molecule& molecule, Composite& after)
	{
		after.Composite::insertAfter(molecule);
	}

	bool System::remove(Molecule& molecule)
	{
		return Composite::removeChild(molecule);
	}

	void System::spliceBefore(System& system)
	{
		Composite::spliceBefore(system);
	}

	void System::spliceAfter(System& system)
	{
		Composite::spliceAfter(system);
	}

	void System::splice(System& system)
	{
		Composite::splice(system);
	}

	void System::destroyBonds(void)
	{
		for (AtomIterator atom_it = beginAtom(); !atom_it.isEnd(); ++atom_it)
		{
			(*atom_it).destroyBonds();
		}
	}

	bool System::isValid(void) const
	{ 
		if (Composite::isValid() == false
				|| PropertyManager::isValid() == false
				|| name_.isValid() == false)
		{
			return false;
		}

		return true;
	}

	void System::dump(ostream& s, unsigned long depth) const
	{
		BALL_DUMP_STREAM_PREFIX(s);
		
		Composite::dump(s, depth);

		BALL_DUMP_DEPTH(s, depth);
		s << "  name: " << name_ << endl;
		
		BALL_DUMP_STREAM_SUFFIX(s);
	}

	void System::read(istream& /* s */)
	{
		throw Exception::NotImplemented(__FILE__, __LINE__);
	}

	void System::write(ostream& /* s */) const
	{
		throw Exception::NotImplemented(__FILE__, __LINE__);
	}

	void System::clear_(void)	
	{
		name_ = BALL_SYSTEM_DEFAULT_NAME;
	}


} // namespace BALL
