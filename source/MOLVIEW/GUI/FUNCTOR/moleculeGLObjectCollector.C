// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: moleculeGLObjectCollector.C,v 1.4.2.1 2003/01/07 13:21:31 anker Exp $

#include <BALL/MOLVIEW/GUI/FUNCTOR/moleculeGLObjectCollector.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/molecule.h>

namespace BALL
{
	namespace MOLVIEW
	{

		MoleculeGLObjectCollector::MoleculeGLObjectCollector()
			throw()
			:	GLObjectCollector()
		{
		}

		MoleculeGLObjectCollector::~MoleculeGLObjectCollector()
      throw()
 		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<MoleculeGLObjectCollector>() << endl;
			#endif 

			destroy();
		}


		bool MoleculeGLObjectCollector::finish()
			throw()
		{
			Molecule* molecule = (Molecule*)getRootComposite();
			AtomIterator atom_it;
			AtomBondIterator bond_it;

			if (molecule == 0)
			{
				return GLObjectCollector::finish();
			}

			BALL_FOREACH_INTRABOND(*molecule, atom_it, bond_it)
			{
				// ?????: Applicator durch Processor ersetzt fuerht zu unendl. Rek.
				Composite::SubcompositeIterator	subcomp_it = bond_it->beginSubcomposite();
				for (; subcomp_it != bond_it->endSubcomposite(); ++subcomp_it)
				{
					this->operator()(*subcomp_it);
				}
				//bond_it->Composite::apply(*((UnaryProcessor<Composite>*)this));
			}

			return GLObjectCollector::finish();
		}

	} // namespace MOLVIEW

} // namespace BALL
