// $Id: secondaryStructure.C,v 1.13 2001/07/15 16:12:43 oliver Exp $

#include <BALL/KERNEL/secondaryStructure.h>

#include <BALL/KERNEL/chain.h>
#include <BALL/KERNEL/global.h>
#include <BALL/KERNEL/protein.h>

using namespace std;

namespace BALL 
{

	SecondaryStructure::SecondaryStructure()
		throw()
		:	AtomContainer()
	{
	}
		
	SecondaryStructure::SecondaryStructure(const SecondaryStructure& secondary_structure, bool deep)
		throw()
		:	AtomContainer(secondary_structure, deep)
	{
	}
		
	SecondaryStructure::SecondaryStructure(const String& name)
		throw()
		:	AtomContainer(name)
	{
	}

	SecondaryStructure::~SecondaryStructure()
		throw()
	{
		destroy();
	}

	void SecondaryStructure::clear()
		throw()
	{
		AtomContainer::clear();
	}
		
	void SecondaryStructure::destroy()
		throw()
	{
		AtomContainer::destroy();
	}
		
  void SecondaryStructure::persistentWrite(PersistenceManager& pm, const char* name) const
		throw(Exception::GeneralException)
  {
    pm.writeObjectHeader(this, name);
      AtomContainer::persistentWrite(pm);
    pm.writeObjectTrailer(name);
	}

  void SecondaryStructure::persistentRead(PersistenceManager& pm)
		throw(Exception::GeneralException)
  {
    pm.checkObjectHeader(RTTI::getStreamName<AtomContainer>());
			AtomContainer::persistentRead(pm);
    pm.checkObjectTrailer(0);
	}
 
	void SecondaryStructure::set(const SecondaryStructure& secondary_structure, bool deep)
		throw()
	{
		AtomContainer::set(secondary_structure, deep);
	}
			
	const SecondaryStructure& SecondaryStructure::operator =(const SecondaryStructure& secondary_structure)
		throw()
	{
		set(secondary_structure);
		return *this;
	}

	void SecondaryStructure::get(SecondaryStructure& secondary_structure, bool deep) const
		throw()
	{
		secondary_structure.set(*this, deep);
	}
			
	void SecondaryStructure::swap(SecondaryStructure& secondary_structure)
		throw()
	{
		AtomContainer::swap(secondary_structure);
	}

	Protein* SecondaryStructure::getProtein()
		throw()
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

	const Protein* SecondaryStructure::getProtein() const
		throw()
	{
		return const_cast<SecondaryStructure *>(this)->getProtein();
	}

	Chain* SecondaryStructure::getChain()
		throw()
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
		throw()
	{
		return const_cast<SecondaryStructure*>(this)->getChain();
	}

	Residue* SecondaryStructure::getResidue(Position position)
		throw()
	{
		for (ResidueIterator res_it = beginResidue();
				 !res_it.isEnd();
				 ++res_it)
		{
			if (position-- == 0)
			{
				return &(*res_it);
			}
		}

		return 0;
	}

	const Residue* SecondaryStructure::getResidue(Position position) const
		throw()
	{
		return const_cast<SecondaryStructure*>(this)->getResidue(position);
	}

	Residue* SecondaryStructure::getNTerminal()
		throw()
	{
		return (Residue *)::BALL::getNTerminal(*this);
	}
		
	const Residue* SecondaryStructure::getNTerminal() const
		throw()
	{
		return ::BALL::getNTerminal(*this);
	}

	Residue* SecondaryStructure::getCTerminal()
		throw()
	{
		return (Residue *)::BALL::getCTerminal(*this);
	}
		
	const Residue* SecondaryStructure::getCTerminal() const
		throw()
	{
		return ::BALL::getCTerminal(*this);
	}

	PDBAtom* SecondaryStructure::getPDBAtom(Position position)
		throw()
	{
		for (PDBAtomIterator protein_atom_it = beginPDBAtom(); !protein_atom_it.isEnd(); ++protein_atom_it)
		{
			if (position-- == 0)
			{
				return &(*protein_atom_it);
			}
		}

		return 0;
	}

	const PDBAtom *SecondaryStructure::getPDBAtom(Position position) const
		throw()
	{
		return ((SecondaryStructure *)this)->getPDBAtom(position);
	}

	Size SecondaryStructure::countResidues() const
		throw()
	{
		Size size = 0;

		for (ResidueConstIterator res_it = beginResidue(); !res_it.isEnd(); ++res_it)
		{
			++size;
		}

		return size;
	}

	Size SecondaryStructure::countPDBAtoms() const
		throw()
	{
		Size size = 0;

		for (PDBAtomConstIterator protein_atom_it = beginPDBAtom(); !protein_atom_it.isEnd(); ++protein_atom_it)
		{
			++size;
		}

		return size;
	}

	void SecondaryStructure::prepend(Residue &residue)
		throw()
	{
		AtomContainer::prepend(residue);
	}

	void SecondaryStructure::append(Residue &residue)
		throw()
	{
		AtomContainer::append(residue);
	}

	void SecondaryStructure::insert(Residue &residue)
		throw()
	{
		AtomContainer::insert(residue);
	}

	void SecondaryStructure::insertBefore(Residue& residue, Composite& before)
		throw()
	{
		AtomContainer::insertBefore(residue, before);
	}

	void SecondaryStructure::insertAfter(Residue& residue, Composite& after)
		throw()
	{
		AtomContainer::insertAfter(residue, after);
	}

	bool SecondaryStructure::remove(Residue& residue)
		throw()
	{
		return AtomContainer::remove(residue);
	}

	void SecondaryStructure::spliceBefore(SecondaryStructure& secondary_structure)
		throw()
	{
		AtomContainer::spliceBefore(secondary_structure);
	}

	void SecondaryStructure::spliceAfter(SecondaryStructure& secondary_structure)
		throw()
	{
		AtomContainer::spliceAfter(secondary_structure);
	}

	void SecondaryStructure::splice(SecondaryStructure& secondary_structure)
		throw()
	{
		AtomContainer::splice(secondary_structure);
	}

	bool SecondaryStructure::isValid() const
		throw()
	{ 
		return AtomContainer::isValid();
	}

	void SecondaryStructure::dump(ostream& s, Size depth) const
		throw()
	{
		BALL_DUMP_STREAM_PREFIX(s);
		
		AtomContainer::dump(s, depth);
		// just to avoid these damned compiler warnings
		// (dump_indent_depth_ was declared but never referenced)
		if (dump_indent_depth_ == 0) ;
		
		BALL_DUMP_STREAM_SUFFIX(s);
	}

	bool SecondaryStructure::operator == (const SecondaryStructure& secondary_structure) const
		throw()
	{
		return(Object::operator == (secondary_structure));
	}

	bool SecondaryStructure::operator != (const SecondaryStructure& secondary_structure) const
		throw()
	{
		return ! (*this == secondary_structure);
	}


} // namespace BALL
