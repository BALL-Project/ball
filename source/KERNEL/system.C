// $Id: system.C,v 1.11 2000/05/16 15:17:35 amoll Exp $

#include <BALL/KERNEL/system.h>

#include <BALL/KERNEL/global.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/fragment.h>
#include <BALL/KERNEL/protein.h>

using namespace std;

namespace BALL 
{

	System::System()
		:	BaseFragment()
	{
	}
		
	System::System(const System& system,bool deep)
	{
		set(system, deep);
	}
		
	System::System(const String& name)
		:	BaseFragment(name)
	{
	}

  void System::persistentWrite(PersistenceManager& pm, const char* name) const
  {
    pm.writeObjectHeader(this, name);
      BaseFragment::persistentWrite(pm);
    pm.writeObjectTrailer(name);
	}

  void System::persistentRead(PersistenceManager& pm)
  {
    pm.checkObjectHeader(RTTI::getStreamName<BaseFragment>());
			Composite::persistentRead(pm);
    pm.checkObjectTrailer(0);
	}

	System::~System()
	{
		destroy();
	}
		
	void System::set(const System& system, bool deep)
	{
		BaseFragment::set(system, deep);
	}
			
	System& System::operator = (const System& system)
	{
		BaseFragment::set(system);

		return *this;
	}

	void System::get(System& system, bool deep) const
	{
		system.set(*this, deep);
	}

	Molecule *System::getMolecule(Position position)
	{
		for (MoleculeIterator res_it = beginMolecule(); !res_it.isEnd(); ++res_it)
			if (position-- == 0)
				return &(*res_it);

		return 0;
	}

	const Molecule *System::getMolecule(Position position) const
	{
		return ((System *)this)->getMolecule(position);
	}

			
	Size System::countMolecules() const
	{
		Size size = 0;

		for (MoleculeIterator mol_it = beginMolecule(); !mol_it.isEnd(); ++mol_it)
		{
			++size;
		}

		return size;
	}

	Size  System::countFragments() const
	{
		Size size = 0;

		for (FragmentIterator frag_it = beginFragment(); !frag_it.isEnd(); ++frag_it)
		{
			++size;
		}

		return size;
	}

	Size  System::countResidues() const
	{
		Size size = 0;

		for (ResidueIterator frag_it = beginResidue(); !frag_it.isEnd(); ++frag_it)
		{
			++size;
		}

		return size;
	}

	Size  System::countAtoms() const
	{
		Size size = 0;

		for (AtomIterator frag_it = beginAtom(); !frag_it.isEnd(); ++frag_it)
		{
			++size;
		}

		return size;
	}

	Size  System::countProteins() const
	{
		Size size = 0;

		for (ProteinIterator frag_it = beginProtein(); !frag_it.isEnd(); ++frag_it)
		{
			++size;
		}

		return size;
	}

	Size  System::countChains() const
	{
		Size size = 0;

		for (ChainIterator frag_it = beginChain(); !frag_it.isEnd(); ++frag_it)
		{
			++size;
		}

		return size;
	}

	Size  System::countSecondaryStructures() const
	{
		Size size = 0;

		for (SecondaryStructureIterator frag_it = beginSecondaryStructure(); !frag_it.isEnd(); ++frag_it)
		{
			++size;
		}

		return size;
	}

	Size  System::countNucleotides() const
	{
		Size size = 0;

		for (NucleotideIterator frag_it = beginNucleotide(); !frag_it.isEnd(); ++frag_it)
		{
			++size;
		}

		return size;
	}

	Size  System::countNucleicAcids() const
	{
		Size size = 0;

		for (NucleicAcidIterator frag_it = beginNucleicAcid(); !frag_it.isEnd(); ++frag_it)
		{
			++size;
		}

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
/*
	void System::destroyBonds()
	{
		 for (AtomIterator atom_it = beginAtom(); !atom_it.isEnd(); ++atom_it)
		 {
						 atom_it->destroyBonds();
		 }
	}
*/
	void System::read(istream& /* s */)
	{
		throw Exception::NotImplemented(__FILE__, __LINE__);
	}

	void System::write(ostream& /* s */) const
	{
		throw Exception::NotImplemented(__FILE__, __LINE__);
	}
} // namespace BALL
