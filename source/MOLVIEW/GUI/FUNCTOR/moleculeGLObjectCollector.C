// $Id: moleculeGLObjectCollector.C,v 1.3 2001/05/13 15:02:41 hekl Exp $

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
				// BAUSTELLE: Applicator durch Processor ersetzt fuerht zu unendl. Rek.
				Composite::SubcompositeIterator	subcomp_it = bond_it->beginSubcomposite();
				for (; subcomp_it != bond_it->endSubcomposite(); ++subcomp_it)
				{
					this->operator()(*subcomp_it);
				}
				//bond_it->Composite::apply(*((UnaryProcessor<Composite>*)this));
			}

			return GLObjectCollector::finish();
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/GUI/FUNCTOR/moleculeGLObjectCollector.iC>
#		endif

	} // namespace MOLVIEW

} // namespace BALL
