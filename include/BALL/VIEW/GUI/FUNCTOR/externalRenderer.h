// $Id: externalRenderer.h,v 1.1.2.1 2002/08/29 16:50:25 anhi Exp $

#ifndef BALL_VIEW_GUI_FUNCTOR_EXTERNALRENDERER_H
#define BALL_VIEW_GUI_FUNCTOR_EXTERNALRENDERER_H

#ifndef BALL_COMMON_H
# include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_PROCESSOR_H
# include <BALL/CONCEPT/processor.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
# include <BALL/KERNEL/atom.h>
#endif

#ifndef BALL_KERNEL_BOND_H
# include <BALL/KERNEL/bond.h>
#endif

#ifndef BALL_VIEW_GUI_WIDGETS_SCENE_H
# include <BALL/VIEW/GUI/WIDGETS/scene.h>
#endif

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
#	include <BALL/VIEW/KERNEL/geometricObject.h>
#endif

#ifndef BALL_VIEW_GUI_KERNEL_GLOBJECT_H
#	include <BALL/VIEW/GUI/KERNEL/glObject.h>
#endif

#ifndef BALL_VIEW_PRIMITIV_SPHERE_H
# include <BALL/VIEW/PRIMITIV/sphere.h>
#endif

#ifndef BALL_VIEW_PRIMITIV_LINE_H
# include <BALL/VIEW/PRIMITIV/line.h>
#endif

#ifndef BALL_VIEW_PRIMITIV_MESH_H_
# include <BALL/VIEW/PRIMITIV/mesh.h>
#endif

#ifndef BALL_VIEW_PRIMITIV_POINT_H
# include <BALL/VIEW/PRIMITIV/point.h>
#endif

#ifndef BALL_VIEW_PRIMITIV_TUBE_H
# include <BALL/VIEW/PRIMITIV/tube.h>
#endif

#ifndef BALL_MOLVIEW_PRIMITIV_TWOCOLOREDTUBE_H
# include <BALL/MOLVIEW/PRIMITIV/twoColoredTube.h>
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
