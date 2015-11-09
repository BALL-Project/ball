// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: moleculeObjectCreator.C,v 1.1.4.1 2007/03/25 22:00:06 oliver Exp $

#include <BALL/CONCEPT/moleculeObjectCreator.h>

#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/protein.h>
#include <BALL/KERNEL/system.h>

using namespace std;

namespace BALL
{

	MoleculeObjectCreator::MoleculeObjectCreator()
		
		:	ObjectCreator()
	{
	}

	MoleculeObjectCreator::~MoleculeObjectCreator()
		
	{
		#ifdef BALL_DEBUG
			cout << "Destructing object " << (void *)this 
				<< " of class " << RTTI::getName<MoleculeObjectCreator>() << endl;
		#endif 
	}

	void MoleculeObjectCreator::clear()
		
	{
		ObjectCreator::clear();
	}

	void MoleculeObjectCreator::initPersistenceManager(TextPersistenceManager &pm)
		
	{
		using namespace RTTI;

		#define REGISTER(A)\
					pm.registerClass(getStreamName<A>(), getNew<A>);

		REGISTER(Atom)
		REGISTER(Bond)
		REGISTER(AtomContainer)
		REGISTER(Vector3)
		REGISTER(PDBAtom)
		REGISTER(Residue)
		REGISTER(Chain)
		REGISTER(SecondaryStructure)
		REGISTER(Protein)
		REGISTER(Molecule)
		REGISTER(System)
		REGISTER(Composite)
		REGISTER(Fragment)
		REGISTER(Nucleotide)
		REGISTER(NucleicAcid)

		#undef REGISTER
	}

	Composite* MoleculeObjectCreator::convertObject(PersistentObject &po)
		
	{
		cout << "read object @ " << (void*)&po << endl;
	
        if (RTTI::isKindOf<Atom>(&po))
		{
			Atom*	atom = RTTI::castTo<Atom>(po);
			cout << " read atom " << atom->getName() << endl;
			
			return (Composite *)atom;
		} 
        else if (RTTI::isKindOf<Protein>(&po))
		{
			Protein*	p = RTTI::castTo<Protein>(po);
			cout << " read protein with " 
					 << p->count(KernelPredicate<Atom>()) << " atoms." << endl;
			
			AtomIterator        atom_it;
			Atom::BondIterator  bond_it;
			Size count = 0;
			BALL_FOREACH_BOND(*p, atom_it, bond_it)
			{
				count++;
			}
			cout << "read " << count << " bonds" << endl;
			
			System *s = new System;
			s->insert(*p);
			
			return (Composite *)s;
		} 
        else if (RTTI::isKindOf<System>(&po))
		{
			System *s = RTTI::castTo<System>(po);
			cout << " read system with " 
					 << s->count(KernelPredicate<Atom>()) << " atoms." << endl;
			AtomIterator        atom_it;
			Atom::BondIterator  bond_it;
			Size count = 0;
			BALL_FOREACH_BOND(*s, atom_it, bond_it)
			{
				count++;
			}
			cout << "read " << count << " bonds" << endl;
		
			return (Composite *)s;
		} 
		else 
		{
			cout << "read something strange!" << endl;
		}

		return 0;
	}

} // namespace BALL
