// $Id: moleculeGLObjectCollector.h,v 1.2 2000/12/12 16:16:57 oliver Exp $

#ifndef BALL_MOLVIEW_GUI_FUNCTOR_MOLECULEGLOBJECTCOLLECTOR_H
#define BALL_MOLVIEW_GUI_FUNCTOR_MOLECULEGLOBJECTCOLLECTOR_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
#	include <BALL/KERNEL/atom.h>
#endif

#ifndef BALL_KERNEL_BOND_H
#	include <BALL/KERNEL/bond.h>
#endif

#ifndef BALL_KERNEL_FOREACH_H
#	include <BALL/KERNEL/forEach.h>
#endif

#ifndef BALL_KERNEL_MOLECULE_H
#	include <BALL/KERNEL/molecule.h>
#endif

#ifndef BALL_VIEW_GUI_FUNCTOR_GLOBJECTCOLLECTOR_H
#	include <BALL/VIEW/GUI/FUNCTOR/glObjectCollector.h>
#endif
  
namespace BALL
{

	namespace MOLVIEW
	{

		using VIEW::GLObjectCollector;

		/**
		*/
		class MoleculeGLObjectCollector
			: public GLObjectCollector
		{
			public:

			/**	@name	Constructors and Destructors
			*/
			//@{

			MoleculeGLObjectCollector();

			MoleculeGLObjectCollector
				(const MoleculeGLObjectCollector& molecule_GL_object_collector, bool deep = true);

			virtual ~MoleculeGLObjectCollector()
				throw();
			//@}


			/**	@name Accessors
			*/
			//@{

			virtual bool finish();
			//@}
		};

#			ifndef BALL_NO_INLINE_FUNCTIONS
#				include <BALL/MOLVIEW/GUI/FUNCTOR/moleculeGLObjectCollector.iC>
#			endif

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_GUI_FUNCTOR_MOLECULEGLOBJECTCOLLECTOR_H 
