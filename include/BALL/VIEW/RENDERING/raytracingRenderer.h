// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_RENDERING_RAYTRACINGRENDERER_H
#define BALL_VIEW_RENDERING_RAYTRACINGRENDERER_H

#ifndef BALL_VIEW_RENDERING_RENDERER_H
#	include <BALL/VIEW/RENDERING/renderer.h>
#endif

// TODO: do we need these includes?
/*#ifndef BALL_MATHS_QUATERNION_H
# include <BALL/MATHS/quaternion.h>
#endif 

#ifndef BALL_VIEW_DATATYPE_COLORRGBA_H
# include <BALL/VIEW/DATATYPE/colorRGBA.h>
#endif

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
#	include <BALL/VIEW/KERNEL/geometricObject.h>
#endif
*/

#ifndef BALL_VIEW_KERNEL_STAGE_H
# include <BALL/VIEW/KERNEL/stage.h>
#endif

#include <BALL/VIEW/DATATYPE/raytraceableGrid.h>

//#ifndef APIENTRY
//#define APIENTRY
//#endif

class QFont;

namespace BALL
{
	namespace VIEW
	{
		class Scene;
		class ColorMap;

		/** RaytracingRenderer
		 		Renderer which provides raytracing instead of regular rendering.
				\ingroup ViewRendering
		*/
		class BALL_VIEW_EXPORT RaytracingRenderer
			: public Renderer
		{
			friend class Scene;
			public:

			///
 			enum StereoMode
			{
				NO_STEREO = 0,

				/// Stereo mode for shutter glasses
				ACTIVE_STEREO,

				/// Stereo mode for output on two projectors
				DUAL_VIEW_STEREO
			};

			// Lukas: which render modes can be supported by raytracing?
			///
			enum RenderMode
			{
				///
				RENDER_MODE_UNDEFINED = 0,
				
				///
				RENDER_MODE_SOLID,

				///
				RENDER_MODE_TRANSPARENT
			};

			/// Default Constructor.
			RaytracingRenderer()
				throw();

			/// Destructor
			virtual ~RaytracingRenderer()
				throw();

			/// Explicit default initialization.
			virtual void clear()
				throw();

			///
			void dump(std::ostream& s, Size depth) const
				throw();

			/** Initialise the renderer, by calling the init method below
			 		This method is called by Scene::initializeRaytracing.
			*/
			virtual bool init(Scene& scene)
				throw();

			/// Initialise the renderer, e.g. the display lists.
			virtual bool init(const Stage& stage, float height, float width)
				throw();

			/// Set the light sources according to the stage
			virtual void setLights(bool reset_all = false)
				throw();

			// Lukas: does picking make sense for raytracing?
			/* Pick geometric objects
			 		\param x1, y1, x2, y2 the rectangle of the selection
			*/
			/*
			void pickObjects1(Position x1, Position y1, Position x2, Position y2)
				throw();
			 */

			/** Pick geometric objects method2.
			 		Call this method after pickObjects1 and rendering the representations.
					\param objects returns the picked objects
			*/
			/*
			void pickObjects2(List<GeometricObject*>& objects)
				throw();

			///
			void enterPickingMode();

			///
			void exitPickingMode();
			 */

			///
			void setSize(float width, float height)
				throw();

			///
			float getXScale() const
				throw();

			///
			float getYScale() const
				throw();

			/** Update the camera position either from a given Camera, or from the default Stage.
			*/
			void updateCamera(const Camera* camera = 0)
				throw();

			/// Update the background color from the stage
			void updateBackgroundColor()
				throw();

			// Initialise transparent rendering
			void initTransparent() 
				throw();

			// Initialise solid rendering
			void initSolid()
				throw();
			
			/// Enable or disable antialiasing
			void setAntialiasing(bool state);
			
			/// Remove all references and auxilliary data for the given Representation
			void removeRepresentation(const Representation& rep)
				throw();

			/// Initialize the renderer for a new representation
			void addRepresentation(const Representation& rep)
				throw();
		
			///
			void setStereoMode(StereoMode state)
				throw();

			///
			StereoMode getStereoMode() const
				throw();

			///
			RenderMode getRenderMode() const
				throw();

			///
			void setRenderMode(RenderMode mode) { render_mode_ = mode;}
			
			///
			virtual void deactivateAllRepresentations()
				throw();

			///
			virtual bool activateRepresentation(const Representation& representation)
				throw();

			///
			String getRenderer()	
				throw();

			///
			DrawingMode getDrawingMode() const {return drawing_mode_;};
			
			///
			void initPerspective();

			/** The main raytracing function.
			 *  This function is called by the scene as soon as all active
			 *  representations have been registered using activateRepresentation()
			 */
			void raytraceAllRepresentations()
				throw();

			void setupGridClipPlanes_(const GridVisualisation& slice);
			Scene* scene_;

			///
			DrawingMode drawing_mode_;

			//_
			float x_scale_;

			//_
			float y_scale_;
	
			Vector3 normal_vector_;
	
			StereoMode stereo_;
			RenderMode render_mode_;

			bool picking_mode_;
		};


	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_RENDERING_RAYTRACINGRENDERER_H
