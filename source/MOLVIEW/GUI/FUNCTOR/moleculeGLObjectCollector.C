// $Id: moleculeGLObjectCollector.C,v 1.1 2000/09/23 15:39:14 hekl Exp $

#include <BALL/MOLVIEW/GUI/FUNCTOR/moleculeGLObjectCollector.h>

namespace BALL
{

	namespace MOLVIEW
	{

		MoleculeGLObjectCollector::MoleculeGLObjectCollector()
			:	GLObjectCollector()
		{
		}

		MoleculeGLObjectCollector::MoleculeGLObjectCollector
			(const MoleculeGLObjectCollector& molecule_GL_object_collector, bool deep)
			:	GLObjectCollector(molecule_GL_object_collector, deep)
		{
		}

		MoleculeGLObjectCollector::~MoleculeGLObjectCollector()
		{
			#ifdef BALL_MOLVIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<MoleculeGLObjectCollector>() << endl;
			#endif 

			destroy();
		}


		bool MoleculeGLObjectCollector::finish()
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
