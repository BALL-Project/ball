// $Id: moleculeGLObjectCollector.C,v 1.4.4.1 2002/10/18 14:48:27 amoll Exp $

#include <BALL/MOLVIEW/GUI/FUNCTOR/moleculeGLObjectCollector.h>

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
			#ifdef BALL_MOLVIEW_DEBUG
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
