// $Id: secondaryStructure.C,v 1.6 2000/08/30 19:58:33 oliver Exp $

#include <BALL/KERNEL/secondaryStructure.h>

#include <BALL/KERNEL/chain.h>
#include <BALL/KERNEL/global.h>
#include <BALL/KERNEL/protein.h>

using namespace std;

namespace BALL 
{

	SecondaryStructure::SecondaryStructure()
		:	AtomContainer()
	{
	}
		
	SecondaryStructure::SecondaryStructure(const SecondaryStructure& secondary_structure, bool deep)
		:	AtomContainer(secondary_structure, deep)
	{
	}
		
	SecondaryStructure::SecondaryStructure(const String& name)
		:	AtomContainer(name)
	{
	}

	SecondaryStructure::~SecondaryStructure()
	{
		destroy();
	}

	void SecondaryStructure::clear()
	{
		AtomContainer::clear();
	}
		
	void SecondaryStructure::destroy()
	{
		AtomContainer::destroy();
	}
		
  void SecondaryStructure::persistentWrite(PersistenceManager& pm, const char* name) const
  {
    pm.writeObjectHeader(this, name);
      AtomContainer::persistentWrite(pm);
    pm.writeObjectTrailer(name);
	}

  void SecondaryStructure::persistentRead(PersistenceManager& pm)
  {
    pm.checkObjectHeader(RTTI::getStreamName<AtomContainer>());
			AtomContainer::persistentRead(pm);
    pm.checkObjectTrailer(0);
	}
 
	void SecondaryStructure::set(const SecondaryStructure& secondary_structure, bool deep)
	{
		AtomContainer::set(secondary_structure, deep);
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
		AtomContainer::swap(secondary_structure);
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

	Residue* SecondaryStructure::getResidue(Position position)
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
	{
		return const_cast<SecondaryStructure*>(this)->getResidue(position);
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

	PDBAtom* SecondaryStructure::getPDBAtom(Position position)
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
	{
		return ((SecondaryStructure *)this)->getPDBAtom(position);
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
		AtomContainer::prepend(residue);
	}

	void SecondaryStructure::append(Residue &residue)
	{
		AtomContainer::append(residue);
	}

	void SecondaryStructure::insert(Residue &residue)
	{
		AtomContainer::insert(residue);
	}

	void SecondaryStructure::insertBefore(Residue& residue, Composite& before)
	{
		AtomContainer::insertBefore(residue, before);
	}

	void SecondaryStructure::insertAfter(Residue& residue, Composite& after)
	{
		AtomContainer::insertAfter(residue, after);
	}

	bool SecondaryStructure::remove(Residue& residue)
	{
		return AtomContainer::remove(residue);
	}

	void SecondaryStructure::spliceBefore(SecondaryStructure& secondary_structure)
	{
		AtomContainer::spliceBefore(secondary_structure);
	}

	void SecondaryStructure::spliceAfter(SecondaryStructure& secondary_structure)
	{
		AtomContainer::spliceAfter(secondary_structure);
	}

	void SecondaryStructure::splice(SecondaryStructure& secondary_structure)
	{
		AtomContainer::splice(secondary_structure);
	}

	bool SecondaryStructure::isValid() const
	{ 
		return AtomContainer::isValid();
	}

	void SecondaryStructure::dump(ostream& s, Size depth) const
	{
		BALL_DUMP_STREAM_PREFIX(s);
		
		AtomContainer::dump(s, depth);
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
