// $Id: molecule.C,v 1.6 2000/08/30 19:58:33 oliver Exp $

#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/system.h>

using namespace std;

namespace BALL 
{

	Molecule::Molecule()
		:	AtomContainer()
	{
	}

	Molecule::Molecule(const Molecule& molecule,bool deep)
		: AtomContainer(molecule, deep)
	{
	}
		
	Molecule::Molecule(const String& name)
		: AtomContainer(name)
	{
	}

	Molecule::~Molecule()
	{
		destroy();
	}
		
	void Molecule::persistentWrite(PersistenceManager& pm, const char* name) const
	{
		pm.writeObjectHeader(this, name);
			AtomContainer::persistentWrite(pm);
		pm.writeObjectTrailer(name);
	}

	void Molecule::persistentRead(PersistenceManager& pm)
	{
		pm.checkObjectHeader(RTTI::getStreamName<AtomContainer>());
			AtomContainer::persistentRead(pm);
		pm.checkObjectTrailer(0);
	}

	void Molecule::set(const Molecule& molecule, bool deep)
	{
		AtomContainer::set(molecule, deep);
	}
			
	Molecule& Molecule::operator =(const Molecule& molecule)
	{
		set(molecule);

		return *this;
	}

	void Molecule::get(Molecule& molecule, bool deep) const
	{
		molecule.set(*this, deep);
	}
			
	void Molecule::swap(Molecule& molecule)
	{
		AtomContainer::swap(molecule);
	}
		
	System* Molecule::getSystem()
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
	{
		return ((Molecule *)this)->getSystem();
	}

	void Molecule::prepend(Atom& atom)
	{
		AtomContainer::prepend(atom);
	}

	void Molecule::append(Atom& atom)
	{
		AtomContainer::append(atom);
	}

	void Molecule::insert(Atom& atom)
	{
		AtomContainer::insert(atom);
	}

	void Molecule::insertBefore(Atom& atom, Composite& before)
	{
		AtomContainer::insertBefore(atom, before);
	}

	void Molecule::insertAfter(Atom& atom,
		 Composite& after)
	{
		AtomContainer::insertAfter(atom, after);
	}

	bool Molecule::remove(Atom& atom)
	{
		return AtomContainer::remove(atom);
	}

	void Molecule::prepend(AtomContainer& atom_container)
	{
		AtomContainer::prepend(atom_container);
	}

	void Molecule::append(AtomContainer& atom_container)
	{
		AtomContainer::append(atom_container);
	}

	void Molecule::insert(AtomContainer& atom_container)
	{
		AtomContainer::insert(atom_container);
	}

	void Molecule::insertBefore(AtomContainer& atom_container, Composite& before)
	{
		AtomContainer::insertBefore(atom_container, before);
	}

	void Molecule::insertAfter(AtomContainer& atom_container, Composite& after)
	{
		AtomContainer::insertAfter(atom_container, after);
	}

	void Molecule::spliceBefore(AtomContainer& atom_container)
	{
		AtomContainer::spliceBefore(atom_container);
	}

	void Molecule::spliceAfter(AtomContainer& atom_container)
	{
		AtomContainer::spliceAfter(atom_container);
	}

	void Molecule::splice(AtomContainer& atom_container)
	{
		AtomContainer::splice(atom_container);
	}

	bool Molecule::remove(AtomContainer& atom_container)
	{
		return AtomContainer::remove(atom_container);
	}

	void Molecule::dump(ostream& s, Size depth) const
	{
		BALL_DUMP_STREAM_PREFIX(s);
		
		AtomContainer::dump(s, depth);
		// just to avoid these damned compiler warnings
		// (dump_indent_depth_ was declared but never referenced)
		if (dump_indent_depth_ == 0) ;
	 
		BALL_DUMP_STREAM_SUFFIX(s);
	}

	void Molecule::read(istream&  /* s */)
	{
		throw Exception::NotImplemented(__FILE__, __LINE__);
	}

	void Molecule::write(ostream& /* s */) const
	{
		throw Exception::NotImplemented(__FILE__, __LINE__);
	}


}  //  namespace BALL
