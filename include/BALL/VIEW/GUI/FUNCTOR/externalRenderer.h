// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: externalRenderer.h,v 1.5 2003/02/21 16:07:22 anhi Exp $

#ifndef BALL_VIEW_GUI_FUNCTOR_EXTERNALRENDERER_H
#define BALL_VIEW_GUI_FUNCTOR_EXTERNALRENDERER_H

#ifndef BALL_CONCEPT_PROCESSOR_H
# include <BALL/CONCEPT/processor.h>
#endif

#ifndef BALL_VIEW_GUI_WIDGETS_SCENE_H
# include <BALL/VIEW/GUI/WIDGETS/scene.h>
#endif

namespace BALL
{
	namespace VIEW
	{

		/** ExternalRenderer class.
		    This class provides an interface for exporting the glObjects contained
				in a Scene to the file format of an external renderer.
				<b>Definition:</b> BALL/VIEW/GUI/FUNCTOR/externalRenderer.h
			*/
		class ExternalRenderer : public UnaryProcessor<Composite>
		{
			public:

			/** @name Constructors and Destructors.
			 */
			//@{

			/** Default Constructor.
			 		Construct a new ExternalRenderer.
			*/
			ExternalRenderer()
				throw();

			/** Destructor.
			 */
			virtual ~ExternalRenderer()
				throw();

			/** Clear method.
			 */
			virtual void clear()
				throw();

			/** Destroy method.
			 */
			virtual void destroy()
				throw();
			//@}
			
			/** @name Processor specific methods
			 */
			//@{
			/** This method is called by the Scene that will call us later.
			 */
			virtual void setScene(const Scene& scene)
				throw();
			
			/** Start method.
			 */
			virtual bool start()
				throw();

			/** Finish method.
			 	*/
			virtual bool finish()
				throw();

			/** Operator ().
			 		Traverses the  \link Composite Composite \endlink  tree with the start  \link Composite Composite \endlink  {\em composite}
					and searches for  \link GeometricObject GeometricObject \endlink  objects that are also of kind  \link GLObject GLObject \endlink .
					@param composite the  \link Composite Composite \endlink  to be searched for  \link GLObject GLObject \endlink  objects
					@return Processor::Result the result of {\em *this} ExternalRenderer
				*/
			virtual Processor::Result operator() (Composite& composite)
				throw();
			//@}

		  protected:
			  const Scene *scene_;
		};

	} // namespace VIEW

}  // namespace BALL

			
#endif // BALL_VIEW_GUI_FUNCTOR_EXTERNALRENDERER_H
