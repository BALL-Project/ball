// $Id: secondaryStructure.C,v 1.4 1999/12/30 18:05:34 oliver Exp $

#include <BALL/KERNEL/secondaryStructure.h>

#include <BALL/KERNEL/chain.h>
#include <BALL/KERNEL/global.h>
#include <BALL/KERNEL/protein.h>

using namespace std;

namespace BALL 
{

	SecondaryStructure::SecondaryStructure()
		:	BaseFragment()
	{
	}
		
	SecondaryStructure::SecondaryStructure(const SecondaryStructure& secondary_structure, bool deep)
		:	BaseFragment(secondary_structure, deep)
	{
	}
		
	SecondaryStructure::SecondaryStructure(const String& name)
		:	BaseFragment(name)
	{
	}

	SecondaryStructure::~SecondaryStructure()
	{
		destroy();
	}

	void SecondaryStructure::clear()
	{
		BaseFragment::clear();
	}
		
	void SecondaryStructure::destroy()
	{
		BaseFragment::destroy();
	}
		
  void SecondaryStructure::persistentWrite(PersistenceManager& pm, const char* name) const
  {
    pm.writeObjectHeader(this, name);
      BaseFragment::persistentWrite(pm);
    pm.writeObjectTrailer(name);
	}

  void SecondaryStructure::persistentRead(PersistenceManager& pm)
  {
    pm.checkObjectHeader(RTTI::getStreamName<BaseFragment>());
			BaseFragment::persistentRead(pm);
    pm.checkObjectTrailer(0);
	}
 
	void SecondaryStructure::set(const SecondaryStructure& secondary_structure, bool deep)
	{
		BaseFragment::set(secondary_structure, deep);
	}
			
	SecondaryStructure& SecondaryStructure::operator =(const SecondaryStructure& secondary_structure)
	{
		set(secondary_structure);

		return *this;
	}

	void SecondaryStructure::get(SecondaryStructure& secondary_structure, bool deep) const
	{
		secondary_structure.set(*this, deep);
	}
			
	void SecondaryStructure::swap(SecondaryStructure& secondary_structure)
	{
		BaseFragment::swap(secondary_structure);
	}

	Protein* SecondaryStructure::getProtein()
	{
		for (Composite::AncestorIterator ancestor_it = beginAncestor(); !ancestor_it.isEnd(); ++ancestor_it)
		{
			if (RTTI::isKindOf<Protein>(*ancestor_it))
			{
				return (Protein *)&*ancestor_it;
			}
		}

		return 0;
	}

	const Protein *SecondaryStructure::getProtein() const
	{
		return const_cast<SecondaryStructure *>(this)->getProtein();
	}

	Chain* SecondaryStructure::getChain()
	{
		for (Composite::AncestorIterator ancestor_it = beginAncestor(); !ancestor_it.isEnd(); ++ancestor_it)
		{
			if (RTTI::isKindOf<Chain>(*ancestor_it))
			{
				return (Chain *)&*ancestor_it;
			}
		}

		return 0;
	}

	const Chain* SecondaryStructure::getChain() const
	{
		return const_cast<SecondaryStructure*>(this)->getChain();
	}

	Residue* SecondaryStructure::getResidue(Index index)
	{
		if (index < 0)
			throw Exception::IndexUnderflow(__FILE__, __LINE__, index);

		for (ResidueIterator res_it = beginResidue();
				 !res_it.isEnd();
				 ++res_it)
		{
			if (index-- == 0)
			{
				return &(*res_it);
			}
		}

		return 0;
	}

	const Residue* SecondaryStructure::getResidue(Index index) const
	{
		return const_cast<SecondaryStructure*>(this)->getResidue(index);
	}

	Residue* SecondaryStructure::getNTerminal()
	{
		return (Residue *)::BALL::getNTerminal(*this);
	}
		
	const Residue* SecondaryStructure::getNTerminal() const
	{
		return ::BALL::getNTerminal(*this);
	}

	Residue* SecondaryStructure::getCTerminal()
	{
		return (Residue *)::BALL::getCTerminal(*this);
	}
		
	const Residue* SecondaryStructure::getCTerminal() const
	{
		return ::BALL::getCTerminal(*this);
	}

	PDBAtom* SecondaryStructure::getPDBAtom(Index index)
	{
		if (index < 0)
			throw Exception::IndexUnderflow(__FILE__, __LINE__, index);

		for (PDBAtomIterator protein_atom_it = beginPDBAtom(); !protein_atom_it.isEnd(); ++protein_atom_it)
		{
			if (index-- == 0)
			{
				return &(*protein_atom_it);
			}
		}

		return 0;
	}

	const PDBAtom *SecondaryStructure::getPDBAtom(Index index) const
	{
		return ((SecondaryStructure *)this)->getPDBAtom(index);
	}

	Size SecondaryStructure::countResidues() const
	{
		Size size = 0;

		for (ResidueIterator res_it = beginResidue(); !res_it.isEnd(); ++res_it)
		{
			++size;
		}

		return size;
	}

	Size SecondaryStructure::countPDBAtoms() const
	{
		Size size = 0;

		for (PDBAtomIterator protein_atom_it = beginPDBAtom(); !protein_atom_it.isEnd(); ++protein_atom_it)
		{
			++size;
		}

		return size;
	}

	void SecondaryStructure::prepend(Residue &residue)
	{
		BaseFragment::prepend(residue);
	}

	void SecondaryStructure::append(Residue &residue)
	{
		BaseFragment::append(residue);
	}

	void SecondaryStructure::insert(Residue &residue)
	{
		BaseFragment::insert(residue);
	}

	void SecondaryStructure::insertBefore(Residue& residue, Composite& before)
	{
		BaseFragment::insertBefore(residue, before);
	}

	void SecondaryStructure::insertAfter(Residue& residue, Composite& after)
	{
		BaseFragment::insertAfter(residue, after);
	}

	bool SecondaryStructure::remove(Residue& residue)
	{
		return BaseFragment::remove(residue);
	}

	void SecondaryStructure::spliceBefore(SecondaryStructure& secondary_structure)
	{
		BaseFragment::spliceBefore(secondary_structure);
	}

	void SecondaryStructure::spliceAfter(SecondaryStructure& secondary_structure)
	{
		BaseFragment::spliceAfter(secondary_structure);
	}

	void SecondaryStructure::splice(SecondaryStructure& secondary_structure)
	{
		BaseFragment::splice(secondary_structure);
	}

	bool SecondaryStructure::isValid() const
	{ 
		return BaseFragment::isValid();
	}

	void SecondaryStructure::dump(ostream& s, Size depth) const
	{
		BALL_DUMP_STREAM_PREFIX(s);
		
		BaseFragment::dump(s, depth);
		// just to avoid these damned compiler warnings
		// (dump_indent_depth_ was declared but never referenced)
		if (dump_indent_depth_ == 0) ;
		
		BALL_DUMP_STREAM_SUFFIX(s);
	}

	void SecondaryStructure::read(istream&  /* s */)
	{
		throw Exception::NotImplemented(__FILE__, __LINE__);
	}

	void SecondaryStructure::write(ostream&  /* s */) const
	{
		throw Exception::NotImplemented(__FILE__, __LINE__);
	}


} // namespace BALL
