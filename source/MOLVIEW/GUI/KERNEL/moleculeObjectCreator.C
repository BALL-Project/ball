// $Id: moleculeObjectCreator.C,v 1.3 2001/12/26 03:35:53 oliver Exp $

#include <BALL/MOLVIEW/GUI/KERNEL/moleculeObjectCreator.h>

using namespace std;

namespace BALL
{

	namespace MOLVIEW
	{

		MoleculeObjectCreator::MoleculeObjectCreator()
			throw()
			:	ObjectCreator()
		{
		}

		MoleculeObjectCreator::~MoleculeObjectCreator()
			throw()
		{
			#ifdef BALL_MOLVIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<MoleculeObjectCreator>() << endl;
			#endif 

			destroy();
		}

		void MoleculeObjectCreator::clear()
			throw()
		{
			ObjectCreator::clear();
		}

		void MoleculeObjectCreator::destroy()
			throw()
		{
			ObjectCreator::destroy();
		}

	  void MoleculeObjectCreator::initPersistenceManager(TextPersistenceManager &pm)
			throw()
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
			throw()
    {
			cout << "read object @ " << (void*)&po << endl;
		
			if (RTTI::isKindOf<Atom>(po))
			{
				Atom*	atom = RTTI::castTo<Atom>(po);
				cout << " read atom " << atom->getName() << endl;
				
				return (Composite *)atom;
			} 
			else if (RTTI::isKindOf<Protein>(po)) 
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
			else if (RTTI::isKindOf<System>(po)) 
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

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/GUI/KERNEL/moleculeObjectCreator.iC>
#		endif

	} // namespace MOLVIEW

} // namespace BALL
