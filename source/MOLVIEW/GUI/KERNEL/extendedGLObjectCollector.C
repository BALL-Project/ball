// $Id: extendedGLObjectCollector.C,v 1.2 2001/12/30 13:28:52 sturm Exp $

#include <BALL/MOLVIEW/GUI/KERNEL/extendedGLObjectCollector.h>

namespace BALL
{

	namespace MOLVIEW
	{

		ExtendedGLObjectCollector::ExtendedGLObjectCollector()
			:	GLObjectCollector()
		{
		}

		ExtendedGLObjectCollector::ExtendedGLObjectCollector
			(const ExtendedGLObjectCollector& extended_GL_object_collector, bool deep)
			:	GLObjectCollector(extended_GL_object_collector, deep)
		{
		}

		ExtendedGLObjectCollector::~ExtendedGLObjectCollector()
		{
			#ifdef BALL_MOLVIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<ExtendedGLObjectCollector>() << endl;
			#endif 

			destroy();
		}


		bool ExtendedGLObjectCollector::finish()
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

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/GUI/KERNEL/extendedGLObjectCollector.iC>
#		endif

	} // namespace MOLVIEW

} // namespace BALL
