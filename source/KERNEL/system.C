// $Id: system.C,v 1.21 2001/06/26 19:35:41 amoll Exp $

#include <BALL/KERNEL/system.h>

#include <BALL/KERNEL/global.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/fragment.h>
#include <BALL/KERNEL/protein.h>

using namespace std;

namespace BALL 
{

	System::System()
		throw()
		:	AtomContainer()
	{
	}
		
	System::System(const System& system, bool deep)
		throw()
		: AtomContainer()
	{
		set(system, deep);
	}
		
	System::System(const String& name)
		throw()
		:	AtomContainer(name)
	{
	}

  void System::persistentWrite(PersistenceManager& pm, const char* name) const
		throw()
  {
    pm.writeObjectHeader(this, name);
      AtomContainer::persistentWrite(pm);
    pm.writeObjectTrailer(name);
	}

  void System::persistentRead(PersistenceManager& pm)
		throw()
  {
    pm.checkObjectHeader(RTTI::getStreamName<AtomContainer>());
			AtomContainer::persistentRead(pm);
    pm.checkObjectTrailer(0);
	}

	System::~System()
		throw()
	{
		destroy();
	}
		
	void System::set(const System& system, bool deep)
		throw()
	{
		AtomContainer::set(system, deep);
	}
			
	const System& System::operator = (const System& system)
		throw()
	{
		AtomContainer::set(system);
		return *this;
	}

	void System::get(System& system, bool deep) const
		throw()
	{
		system.set(*this, deep);
	}

	Molecule* System::getMolecule(Position position)
		throw()
	{
		for (MoleculeIterator res_it = beginMolecule(); !res_it.isEnd(); ++res_it)
		{
			if (position-- == 0)
			{
				return &(*res_it);
			}
		}

		return 0;
	}

	const Molecule* System::getMolecule(Position position) const
		throw()
	{
		return ((System *)this)->getMolecule(position);
	}
			
	Size System::countMolecules() const
		throw()
	{
		Size size = 0;

		for (MoleculeConstIterator mol_it = beginMolecule(); !mol_it.isEnd(); ++mol_it)
		{
			++size;
		}

		return size;
	}

	Size  System::countFragments() const
		throw()
	{
		Size size = 0;

		for (FragmentConstIterator frag_it = beginFragment(); !frag_it.isEnd(); ++frag_it)
		{
			++size;
		}

		return size;
	}

	Size  System::countResidues() const
		throw()
	{
		Size size = 0;

		for (ResidueConstIterator frag_it = beginResidue(); !frag_it.isEnd(); ++frag_it)
		{
			++size;
		}

		return size;
	}

	Size  System::countAtoms() const
		throw()
	{
		Size size = 0;

		for (AtomConstIterator frag_it = beginAtom(); !frag_it.isEnd(); ++frag_it)
		{
			++size;
		}

		return size;
	}

	Size  System::countProteins() const
		throw()
	{
		Size size = 0;

		for (ProteinConstIterator frag_it = beginProtein(); !frag_it.isEnd(); ++frag_it)
		{
			++size;
		}

		return size;
	}

	Size  System::countChains() const
		throw()
	{
		Size size = 0;

		for (ChainConstIterator frag_it = beginChain(); !frag_it.isEnd(); ++frag_it)
		{
			++size;
		}

		return size;
	}

	Size  System::countSecondaryStructures() const
		throw()
	{
		Size size = 0;

		for (SecondaryStructureConstIterator frag_it = beginSecondaryStructure(); !frag_it.isEnd(); ++frag_it)
		{
			++size;
		}

		return size;
	}

	Size  System::countNucleotides() const
		throw()
	{
		Size size = 0;

		for (NucleotideConstIterator frag_it = beginNucleotide(); !frag_it.isEnd(); ++frag_it)
		{
			++size;
		}

		return size;
	}

	Size  System::countNucleicAcids() const
		throw()
	{
		Size size = 0;

		for (NucleicAcidConstIterator frag_it = beginNucleicAcid(); !frag_it.isEnd(); ++frag_it)
		{
			++size;
		}

		return size;
	}

	void System::prepend(Molecule& molecule)
		throw()
	{
		Composite::prependChild(molecule);
	}

	void System::append(Molecule& molecule)
		throw()
	{
		Composite::appendChild(molecule);
	}

	void System::insert(Molecule& molecule)
		throw()
	{
		append(molecule);
	}

	void System::insertBefore(Molecule& molecule, Composite& before)
		throw()
	{
		before.Composite::insertBefore(molecule);
	}

	void System::insertAfter(Molecule& molecule, Composite& after)
		throw()
	{
		after.Composite::insertAfter(molecule);
	}

	bool System::remove(Molecule& molecule)
		throw()
	{
		return Composite::removeChild(molecule);
	}

	void System::spliceBefore(System& system)
		throw()
	{
		Composite::spliceBefore(system);
	}

	void System::spliceAfter(System& system)
		throw()
	{
		Composite::spliceAfter(system);
	}

	void System::splice(System& system)
		throw()
	{
		Composite::splice(system);
	}

	bool System::operator == (const System& system) const
		throw()
	{
		return(Object::operator == (system));
	}

	bool System::operator != (const System& system) const
		throw()
	{
		return !(*this == system);
	}

} // namespace BALL
