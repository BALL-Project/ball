// $Id: molecule.C,v 1.4 1999/12/30 18:05:33 oliver Exp $

#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/system.h>

using namespace std;

namespace BALL 
{

	Molecule::Molecule()
		:	BaseFragment()
	{
	}

	Molecule::Molecule(const Molecule& molecule,bool deep)
		: BaseFragment(molecule, deep)
	{
	}
		
	Molecule::Molecule(const String& name)
		: BaseFragment(name)
	{
	}

	Molecule::~Molecule()
	{
		destroy();
	}

	void Molecule::clear()
	{
		BaseFragment::clear();
	}
		
	void Molecule::destroy()
	{
		BaseFragment::destroy();
	}
		
	void Molecule::persistentWrite(PersistenceManager& pm, const char* name) const
	{
		pm.writeObjectHeader(this, name);
			BaseFragment::persistentWrite(pm);
		pm.writeObjectTrailer(name);
	}

	void Molecule::persistentRead(PersistenceManager& pm)
	{
		pm.checkObjectHeader(RTTI::getStreamName<BaseFragment>());
			BaseFragment::persistentRead(pm);
		pm.checkObjectTrailer(0);
	}

	void Molecule::set(const Molecule& molecule, bool deep)
	{
		BaseFragment::set(molecule, deep);
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
		BaseFragment::swap(molecule);
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
		BaseFragment::prepend(atom);
	}

	void Molecule::append(Atom& atom)
	{
		BaseFragment::append(atom);
	}

	void Molecule::insert(Atom& atom)
	{
		BaseFragment::insert(atom);
	}

	void Molecule::insertBefore(Atom& atom, Composite& before)
	{
		BaseFragment::insertBefore(atom, before);
	}

	void Molecule::insertAfter(Atom& atom,
		 Composite& after)
	{
		BaseFragment::insertAfter(atom, after);
	}

	bool Molecule::remove(Atom& atom)
	{
		return BaseFragment::remove(atom);
	}

	void Molecule::prepend(BaseFragment& base_fragment)
	{
		BaseFragment::prepend(base_fragment);
	}

	void Molecule::append(BaseFragment& base_fragment)
	{
		BaseFragment::append(base_fragment);
	}

	void Molecule::insert(BaseFragment& base_fragment)
	{
		BaseFragment::insert(base_fragment);
	}

	void Molecule::insertBefore(BaseFragment& base_fragment, Composite& before)
	{
		BaseFragment::insertBefore(base_fragment, before);
	}

	void Molecule::insertAfter(BaseFragment& base_fragment, Composite& after)
	{
		BaseFragment::insertAfter(base_fragment, after);
	}

	void Molecule::spliceBefore(BaseFragment& base_fragment)
	{
		BaseFragment::spliceBefore(base_fragment);
	}

	void Molecule::spliceAfter(BaseFragment& base_fragment)
	{
		BaseFragment::spliceAfter(base_fragment);
	}

	void Molecule::splice(BaseFragment& base_fragment)
	{
		BaseFragment::splice(base_fragment);
	}

	bool Molecule::remove(BaseFragment& base_fragment)
	{
		return BaseFragment::remove(base_fragment);
	}

	bool Molecule::isValid() const
	{ 
		return BaseFragment::isValid();
	}

	void Molecule::dump(ostream& s, Size depth) const
	{
		BALL_DUMP_STREAM_PREFIX(s);
		
		BaseFragment::dump(s, depth);
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
