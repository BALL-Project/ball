// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: moleculeGLObjectCollector.h,v 1.11 2003/03/26 13:56:52 anhi Exp $

#ifndef BALL_MOLVIEW_GUI_FUNCTOR_MOLECULEGLOBJECTCOLLECTOR_H
#define BALL_MOLVIEW_GUI_FUNCTOR_MOLECULEGLOBJECTCOLLECTOR_H

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
				the  \link GLObject GLObject \endlink  objects that are found in  \link Bond Bond \endlink  objects that are available
				in the processed  \link Molecule Molecule \endlink  object. This is done by overriding the  \link finish finish \endlink 
				method from the class  \link GLObjectCollector GLObjectCollector \endlink . All bond primitives are collected
				after the atom primitives are collected by the base class.
				The  \link GLObject GLObject \endlink  objects
				are separated into different visualization groups (=lists) that are necessary
				for proper rendering.
				See  \link GLObjectCollector GLObjectCollector \endlink  for further information about collecting  \link GLObject GLObject \endlink  objects. \par
				<b>Defintion:</b> BALL/MOLVIEW/GUI/FUNCTOR/moleculeGLObjectCollector.h
    		
				\ingroup  MolviewGuiFunctor
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
					Calls  \link GLObjectCollector::destroy GLObjectCollector::destroy \endlink 
			 */
			virtual ~MoleculeGLObjectCollector()
				throw();

			//@}
			/**	@name Processor specific methods
			*/
			//@{

			/** Finish method.
					Collects all  \link GLObject GLObject \endlink  objects that are accessable on each  \link Bond Bond \endlink 
					of the processed  \link Molecule Molecule \endlink  object (if  \link getRootComposite getRootComposite \endlink  is of type
					 \link Molecule Molecule \endlink ).
					Calls  \link GLObjectCollector::operator() GLObjectCollector::operator() \endlink  for each  \link Bond Bond \endlink  of  \link Molecule Molecule \endlink .
					@return bool <tt>true</tt> if the finish of {\em *this} moleculeGLObjectCollector was successful, 
					             <tt>false</tt> otherwise
			*/
			virtual bool finish()
				throw();
			//@}
		};
	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_GUI_FUNCTOR_MOLECULEGLOBJECTCOLLECTOR_H 
