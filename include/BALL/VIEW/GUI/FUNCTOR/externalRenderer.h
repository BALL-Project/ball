// $Id: externalRenderer.h,v 1.1.2.2 2002/10/26 00:15:44 amoll Exp $

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
				{\bf Definition:} \URL{BALL/VIEW/GUI/FUNCTOR/externalRenderer.h}\\
			*/
		class ExternalRenderer : public UnaryProcessor<Composite>
		{
			public:

			/** @name Constructors and Destructors.
			 */
			//@[

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
			 		Traverses the \Ref{Composite} tree with the start \Ref{Composite} {\em composite}
					and searches for \Ref{GeometricObject} objects that are also of kind \Ref{GLObject}.
					@param composite the \Ref{Composite} to be searched for \Ref{GLObject} objects
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

			//@}
			
#endif // BALL_VIEW_GUI_FUNCTOR_EXTERNALRENDERER_H
