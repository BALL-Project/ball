// $Id: molecule.C,v 1.11 2001/06/26 19:35:40 amoll Exp $

#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/system.h>

using namespace std;

namespace BALL 
{

	Molecule::Molecule()
		throw()
		:	AtomContainer()
	{
	}

	Molecule::Molecule(const Molecule& molecule,bool deep)
		throw()
		: AtomContainer(molecule, deep)
	{
	}
		
	Molecule::Molecule(const String& name)
		throw()
		: AtomContainer(name)
	{
	}

	Molecule::~Molecule()
		throw()
	{
		destroy();
	}
		
	void Molecule::persistentWrite(PersistenceManager& pm, const char* name) const
		throw()
	{
		pm.writeObjectHeader(this, name);
			AtomContainer::persistentWrite(pm);
		pm.writeObjectTrailer(name);
	}

	void Molecule::persistentRead(PersistenceManager& pm)
		throw()
	{
		pm.checkObjectHeader(RTTI::getStreamName<AtomContainer>());
			AtomContainer::persistentRead(pm);
		pm.checkObjectTrailer(0);
	}

	void Molecule::set(const Molecule& molecule, bool deep)
		throw()
	{
		AtomContainer::set(molecule, deep);
	}
			
	const Molecule& Molecule::operator =(const Molecule& molecule)
		throw()
	{
		set(molecule);
		return *this;
	}

	void Molecule::get(Molecule& molecule, bool deep) const
		throw()
	{
		molecule.set(*this, deep);
	}
			
	void Molecule::swap(Molecule& molecule)
		throw()
	{
		AtomContainer::swap(molecule);
	}
		
	System* Molecule::getSystem()
		throw()
	{
		for (Composite::AncestorIterator ancestor_it = beginAncestor();
				 !ancestor_it.isEnd(); ++ancestor_it)
		{
			if (RTTI::isKindOf<System>(*ancestor_it) == true)
			{
				return (System *)&*ancestor_it;
			}
		}

		return 0;
	}

	const System* Molecule::getSystem() const
		throw()
	{
		return ((Molecule *)this)->getSystem();
	}

	void Molecule::prepend(Atom& atom)
		throw()
	{
		AtomContainer::prepend(atom);
	}

	void Molecule::append(Atom& atom)
		throw()
	{
		AtomContainer::append(atom);
	}

	void Molecule::insert(Atom& atom)
		throw()
	{
		AtomContainer::insert(atom);
	}

	void Molecule::insertBefore(Atom& atom, Composite& before)
		throw()
	{
		AtomContainer::insertBefore(atom, before);
	}

	void Molecule::insertAfter(Atom& atom, Composite& after)
		throw()
	{
		AtomContainer::insertAfter(atom, after);
	}

	bool Molecule::remove(Atom& atom)
		throw()
	{
		return AtomContainer::remove(atom);
	}

	void Molecule::prepend(AtomContainer& atom_container)
		throw()
	{
		AtomContainer::prepend(atom_container);
	}

	void Molecule::append(AtomContainer& atom_container)
		throw()
	{
		AtomContainer::append(atom_container);
	}

	void Molecule::insert(AtomContainer& atom_container)
		throw()
	{
		AtomContainer::insert(atom_container);
	}

	void Molecule::insertBefore(AtomContainer& atom_container, Composite& before)
		throw()
	{
		AtomContainer::insertBefore(atom_container, before);
	}

	void Molecule::insertAfter(AtomContainer& atom_container, Composite& after)
		throw()
	{
		AtomContainer::insertAfter(atom_container, after);
	}

	void Molecule::spliceBefore(AtomContainer& atom_container)
		throw()
	{
		AtomContainer::spliceBefore(atom_container);
	}

	void Molecule::spliceAfter(AtomContainer& atom_container)
		throw()
	{
		AtomContainer::spliceAfter(atom_container);
	}

	void Molecule::splice(AtomContainer& atom_container)
		throw()
	{
		AtomContainer::splice(atom_container);
	}

	bool Molecule::remove(AtomContainer& atom_container)
		throw()
	{
		return AtomContainer::remove(atom_container);
	}

	void Molecule::dump(ostream& s, Size depth) const
		throw()
	{
		BALL_DUMP_STREAM_PREFIX(s);
		
		AtomContainer::dump(s, depth);
		// just to avoid these damned compiler warnings
		// (dump_indent_depth_ was declared but never referenced)
		if (dump_indent_depth_ == 0) ;
	 
		BALL_DUMP_STREAM_SUFFIX(s);
	}

	bool Molecule::operator == (const Molecule& molecule) const
		throw()
	{
		return(Object::operator == (molecule));
	}

	bool Molecule::operator != (const Molecule& molecule) const
		throw()
	{
		return ! (*this == molecule);
	}


}  //  namespace BALL
