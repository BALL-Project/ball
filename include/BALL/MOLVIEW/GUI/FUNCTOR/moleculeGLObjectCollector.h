// $Id: moleculeGLObjectCollector.h,v 1.5 2001/07/16 14:49:28 amoll Exp $

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

		/** MoleculeGLObjectCollector class.
				The class MoleculeGLObjectCollector is responsible for additionally collecting 
				the \Ref{GLObject} objects that are found in \Ref{Bond} objects that are available
				in the processed \Ref{Molecule} object. This is done by overriding the \Ref{finish}
				method from the class \Ref{GLObjectCollector}. All bond primitives are collected
				after the atom primitives are collected by the base class.
				The \Ref{GLObject} objects
				are separated into different visualization groups (=lists) that are necessary
				for proper rendering.
				See \Ref{GLObjectCollector} for further information about collecting \Ref{GLObject}
				objects. \\
				{\bf Defintion:} \URL{BALL/MOLVIEW/GUI/FUNCTOR/moleculeGLObjectCollector.h}
		*/
		class MoleculeGLObjectCollector: public GLObjectCollector
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Constructs new moleculeGLObjectCollector.
					@return MoleculeGLObjectCollector new constructed moleculeGLObjectCollector
					@see    GLObjectCollector
			 */
			MoleculeGLObjectCollector()
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} moleculeGLObjectCollector.
					Calls \Ref{GLObjectCollector::destroy}
			 */
			virtual ~MoleculeGLObjectCollector()
				throw();
			//@}


			/**	@name Processor specific methods
			*/
			//@{

			/** Finish method.
					Collects all \Ref{GLObject} objects that are accessable on each \Ref{Bond}
					of the processed \Ref{Molecule} object (if \Ref{getRootComposite} is of type
					\Ref{Molecule}).
					Calls \Ref{GLObjectCollector::operator()} for each \Ref{Bond} of \Ref{Molecule}.
					@return bool {\tt true} if the finish of {\em *this} moleculeGLObjectCollector was successful, {\tt false} otherwise
			*/
			virtual bool finish()
				throw();
			//@}
		};

#			ifndef BALL_NO_INLINE_FUNCTIONS
#				include <BALL/MOLVIEW/GUI/FUNCTOR/moleculeGLObjectCollector.iC>
#			endif

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_GUI_FUNCTOR_MOLECULEGLOBJECTCOLLECTOR_H 
