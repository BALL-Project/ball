// $Id: chain.C,v 1.11 2001/01/21 21:05:52 amoll Exp $

#include <BALL/KERNEL/chain.h>
#include <BALL/KERNEL/global.h>
#include <BALL/KERNEL/protein.h>

using namespace std;

namespace BALL 
{

	Chain::Chain()
		throw()
		:	AtomContainer()
	{
	}
		
	Chain::Chain(const Chain& chain, bool deep)
		throw()
		:	AtomContainer(chain, deep)
	{
	}
		
	Chain::Chain(const String& name)
		throw()
		:	AtomContainer(name)
	{
	}

	Chain::~Chain()
		throw()
	{
		destroy();
	}

  void Chain::persistentWrite(PersistenceManager& pm, const char* name) const
		throw()
  {
    pm.writeObjectHeader(this, name);
      AtomContainer::persistentWrite(pm);
    pm.writeObjectTrailer(name);
	}

  void Chain::persistentRead(PersistenceManager& pm)
		throw()
  {
    pm.checkObjectHeader(RTTI::getStreamName<AtomContainer>());
			AtomContainer::persistentRead(pm);
    pm.checkObjectTrailer(0);
	}
 
	void Chain::set(const Chain& chain, bool deep)
		throw()
	{
		AtomContainer::set(chain, deep);
	}
			
	const Chain& Chain::operator = (const Chain &chain)
		throw()
	{
		set(chain);
		return *this;
	}

	void Chain::get(Chain &chain, bool deep) const
		throw()
	{
		chain.set(*this, deep);
	}
			
	void Chain::swap(Chain &chain)
		throw()
	{
		AtomContainer::swap(chain);
	}

	Protein* Chain::getProtein()
		throw()
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
		throw()
	{
		return ((Chain *)this)->getProtein();
	}

	SecondaryStructure *Chain::getSecondaryStructure(Position position)
		throw()
	{
		for (SecondaryStructureIterator secondary_structure_it = beginSecondaryStructure();
				 !secondary_structure_it.isEnd(); ++secondary_structure_it)
			if (position-- == 0)
			{
				return &(*secondary_structure_it);
			}

		return 0;
	}

	const SecondaryStructure* Chain::getSecondaryStructure(Position position) const
		throw()
	{
		return ((Chain *)this)->getSecondaryStructure(position);
	}

	Residue* Chain::getResidue(Position position)
		throw()
	{
		for (ResidueIterator res_it = beginResidue(); !res_it.isEnd(); ++res_it)
		{
			if (position-- == 0)
			{
				return &(*res_it);
			}
		}
		return 0;
	}

	const Residue *Chain::getResidue(Position position) const
		throw()
	{
		return ((Chain *)this)->getResidue(position);
	}

	Residue* Chain::getNTerminal()
		throw()
	{
		return const_cast<Residue*>(::BALL::getNTerminal(*this));
	}
		
	const Residue* Chain::getNTerminal() const
		throw()
	{
		return ::BALL::getNTerminal(*this);
	}

	Residue* Chain::getCTerminal()
		throw()
	{
		return const_cast<Residue*>(::BALL::getCTerminal(*this));
	}
		
	const Residue* Chain::getCTerminal() const
		throw()
	{
		return ::BALL::getCTerminal(*this);
	}

	PDBAtom* Chain::getPDBAtom(Position position)
		throw()
	{
		for (PDBAtomIterator protein_atom_it = beginPDBAtom();
				 !protein_atom_it.isEnd(); ++protein_atom_it)
		{
			if (position-- == 0)
			{
				return &(*protein_atom_it);
			}
		}

		return 0;
	}

	const PDBAtom* Chain::getPDBAtom(Position position) const
		throw()
	{
		return ((Chain *)this)->getPDBAtom(position);
	}

	Size Chain::countSecondaryStructures() const
		throw()
	{
		Size size = 0;

		for (SecondaryStructureIterator secondary_structure_it = beginSecondaryStructure();
				 !secondary_structure_it.isEnd();++secondary_structure_it)
		{
			++size;
		}
		return size;
	}

	Size Chain::countResidues() const
		throw()
	{
		Size size = 0;

		for (ResidueIterator res_it = beginResidue();!res_it.isEnd(); ++res_it)
		{
			++size;
		}

		return size;
	}

	Size Chain::countPDBAtoms() const
		throw()
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
		throw()
	{
		AtomContainer::prepend(secondary_structure);
	}

	void Chain::append(SecondaryStructure &secondary_structure)
		throw()
	{
		AtomContainer::append(secondary_structure);
	}

	void Chain::insert(SecondaryStructure &secondary_structure)
		throw()
	{
		AtomContainer::insert(secondary_structure);
	}

	void Chain::insertBefore(SecondaryStructure &secondary_structure, Composite &before)
		throw()
	{
		AtomContainer::insertBefore(secondary_structure, before);
	}

	void Chain::insertAfter(SecondaryStructure &secondary_structure, Composite &after)
		throw()
	{
		AtomContainer::insertAfter(secondary_structure, after);
	}

	bool Chain::remove(SecondaryStructure &secondary_structure)
		throw()
	{
		return AtomContainer::remove(secondary_structure);
	}

	void Chain::prepend(Residue &residue)
		throw()
	{
		AtomContainer::prepend(residue);
	}

	void Chain::append(Residue &residue)
		throw()
	{
		AtomContainer::append(residue);
	}

	void Chain::insert(Residue &residue)
		throw()
	{
		AtomContainer::insert(residue);
	}

	void Chain::insertBefore(Residue &residue, Composite &before)
		throw()
	{
		AtomContainer::insertBefore(residue, before);
	}

	void Chain::insertAfter(Residue &residue, Composite &after)
		throw()
	{
		AtomContainer::insertAfter(residue, after);
	}

	bool Chain::remove(Residue &residue)
		throw()
	{
		return AtomContainer::remove(residue);
	}

	void Chain::spliceBefore(Chain &chain)
		throw()
	{
		AtomContainer::spliceBefore(chain);
	}

	void Chain::spliceAfter(Chain &chain)
		throw()
	{
		AtomContainer::spliceAfter(chain);
	}

	void Chain::splice(Chain &chain)
		throw()
	{
		AtomContainer::splice(chain);
	}

	void Chain::dump(ostream& s, Size depth) const
		throw()
	{
		BALL_DUMP_STREAM_PREFIX(s);
		
		AtomContainer::dump(s, depth);
		// just to avoid these damned compiler warnings
		// (dump_indent_depth_ was declared but never referenced)
		if (dump_indent_depth_ == 0) ;
	 
		BALL_DUMP_STREAM_SUFFIX(s);
	}

	void Chain::read(istream& /* s */)
		throw()
	{
		throw Exception::NotImplemented(__FILE__, __LINE__);
	}

	void Chain::write(ostream& /* s */) const
		throw()
	{
		throw Exception::NotImplemented(__FILE__, __LINE__);
	}

	bool Chain::operator == (const Chain& chain) const
		throw()
	{
		return(Object::operator == (chain));
	}

	bool Chain::operator != (const Chain& chain) const
		throw()
	{
		return ! (*this == chain);
	}

} // namespace BALL
