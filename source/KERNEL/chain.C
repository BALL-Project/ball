// $Id: chain.C,v 1.6 2000/04/27 15:09:34 amoll Exp $

#include <BALL/KERNEL/chain.h>
#include <BALL/KERNEL/global.h>
#include <BALL/KERNEL/protein.h>

using namespace std;

namespace BALL 
{

	Chain::Chain()
		:	BaseFragment()
	{
	}
		
	Chain::Chain(const Chain& chain, bool deep)
		:	BaseFragment(chain, deep)
	{
	}
		
	Chain::Chain(const String& name)
		:	BaseFragment(name)
	{
	}

	Chain::~Chain()
	{
		destroy();
	}

  void Chain::persistentWrite(PersistenceManager& pm, const char* name) const
  {
    pm.writeObjectHeader(this, name);
      BaseFragment::persistentWrite(pm);
    pm.writeObjectTrailer(name);
	}

  void Chain::persistentRead(PersistenceManager& pm)
  {
    pm.checkObjectHeader(RTTI::getStreamName<BaseFragment>());
			BaseFragment::persistentRead(pm);
    pm.checkObjectTrailer(0);
	}
 
	void Chain::set(const Chain& chain, bool deep)
	{
		BaseFragment::set(chain, deep);
	}
			
	Chain& Chain::operator = (const Chain &chain)
	{
		set(chain);

		return *this;
	}

	void Chain::get(Chain &chain, bool deep) const
	{
		chain.set(*this, deep);
	}
			
	void Chain::swap(Chain &chain)
	{
		BaseFragment::swap(chain);
	}

	Protein *Chain::getProtein()
	{
		for (Composite::AncestorIterator ancestor_it = beginAncestor();
				 !ancestor_it.isEnd(); ++ancestor_it)
		{
			if (RTTI::isKindOf<Protein>(*ancestor_it) == true)
			{
				return (Protein *)&*ancestor_it;
			}
		}

		return 0;
	}

	const Protein *Chain::getProtein() const
	{
		return ((Chain *)this)->getProtein();
	}

	SecondaryStructure *Chain::getSecondaryStructure(Position position)
	{
		for (SecondaryStructureIterator secondary_structure_it = beginSecondaryStructure();
				 !secondary_structure_it.isEnd(); ++secondary_structure_it)
			if (position-- == 0)
				return &(*secondary_structure_it);

		return 0;
	}

	const SecondaryStructure *Chain::getSecondaryStructure(Position position) const
	{
		return ((Chain *)this)->getSecondaryStructure(position);
	}

	Residue *Chain::getResidue(Position position)
	{
		for (ResidueIterator res_it = beginResidue(); !res_it.isEnd(); ++res_it)
			if (position-- == 0)
				return &(*res_it);

		return 0;
	}

	const Residue *Chain::getResidue(Position position) const
	{
		return ((Chain *)this)->getResidue(position);
	}

	Residue *Chain::getNTerminal()
	{
		return const_cast<Residue*>(::BALL::getNTerminal(*this));
	}
		
	const Residue *Chain::getNTerminal() const
	{
		return ::BALL::getNTerminal(*this);
	}

	Residue *Chain::getCTerminal()
	{
		return const_cast<Residue*>(::BALL::getCTerminal(*this));
	}
		
	const Residue *Chain::getCTerminal() const
	{
		return ::BALL::getCTerminal(*this);
	}

	PDBAtom *Chain::getPDBAtom(Position position)
	{
		for (PDBAtomIterator protein_atom_it = beginPDBAtom();
				 !protein_atom_it.isEnd(); ++protein_atom_it)
		{
			if (position-- == 0)
				return &(*protein_atom_it);
		}

		return 0;
	}

	const PDBAtom *Chain::getPDBAtom(Position position) const
	{
		return ((Chain *)this)->getPDBAtom(position);
	}

	Size Chain::countSecondaryStructures() const
	{
		Size size = 0;

		for (SecondaryStructureIterator secondary_structure_it = beginSecondaryStructure();
				 !secondary_structure_it.isEnd();++secondary_structure_it)
			++size;

		return size;
	}

	Size Chain::countResidues() const
	{
		Size size = 0;

		for (ResidueIterator res_it = beginResidue();!res_it.isEnd(); ++res_it)
			++size;

		return size;
	}

	Size Chain::countPDBAtoms() const
	{
		Size size = 0;

		for (PDBAtomIterator protein_atom_it = beginPDBAtom();
				 !protein_atom_it.isEnd(); ++protein_atom_it)
		{
			++size;
		}

		return size;
	}

	void Chain::prepend(SecondaryStructure &secondary_structure)
	{
		BaseFragment::prepend(secondary_structure);
	}

	void Chain::append(SecondaryStructure &secondary_structure)
	{
		BaseFragment::append(secondary_structure);
	}

	void Chain::insert(SecondaryStructure &secondary_structure)
	{
		BaseFragment::insert(secondary_structure);
	}

	void Chain::insertBefore
		(SecondaryStructure &secondary_structure,
		 Composite &before)
	{
		BaseFragment::insertBefore(secondary_structure, before);
	}

	void Chain::insertAfter(SecondaryStructure &secondary_structure, Composite &after)
	{
		BaseFragment::insertAfter(secondary_structure, after);
	}

	bool Chain::remove(SecondaryStructure &secondary_structure)
	{
		return BaseFragment::remove(secondary_structure);
	}

	void Chain::prepend(Residue &residue)
	{
		BaseFragment::prepend(residue);
	}

	void Chain::append(Residue &residue)
	{
		BaseFragment::append(residue);
	}

	void Chain::insert(Residue &residue)
	{
		BaseFragment::insert(residue);
	}

	void Chain::insertBefore(Residue &residue, Composite &before)
	{
		BaseFragment::insertBefore(residue, before);
	}

	void Chain::insertAfter(Residue &residue, Composite &after)
	{
		BaseFragment::insertAfter(residue, after);
	}

	bool Chain::remove(Residue &residue)
	{
		return BaseFragment::remove(residue);
	}

	void Chain::spliceBefore(Chain &chain)
	{
		BaseFragment::spliceBefore(chain);
	}

	void Chain::spliceAfter(Chain &chain)
	{
		BaseFragment::spliceAfter(chain);
	}

	void Chain::splice(Chain &chain)
	{
		BaseFragment::splice(chain);
	}

	void Chain::dump(ostream& s, Size depth) const
	{
		BALL_DUMP_STREAM_PREFIX(s);
		
		BaseFragment::dump(s, depth);
		// just to avoid these damned compiler warnings
		// (dump_indent_depth_ was declared but never referenced)
		if (dump_indent_depth_ == 0) ;
	 
		BALL_DUMP_STREAM_SUFFIX(s);
	}

	void Chain::read(istream& /* s */)
	{
		throw Exception::NotImplemented(__FILE__, __LINE__);
	}

	void Chain::write(ostream& /* s */) const
	{
		throw Exception::NotImplemented(__FILE__, __LINE__);
	}


} // namespace BALL
