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

#include <BALL/DATATYPE/raytraceableGrid.h>

#ifndef APIENTRY
#define APIENTRY
#endif

class QFont;

namespace BALL
{
	namespace VIEW
	{
		class Scene;
		// TODO: do we need this?
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

			// Lukas: can we support stereo display for raytracing as well???
			/*
			///
 			enum StereoMode
			{
				NO_STEREO = 0,

				/// Stereo mode for shutter glasses
				ACTIVE_STEREO,

				/// Stereo mode for output on two projectors
				DUAL_VIEW_STEREO
			};
			*/

			// Lukas: which render modes can be supported by raytracing?
			///
			enum RenderMode
			{
				///
				RENDER_MODE_UNDEFINED = 0,
				
				///
				RENDER_MODE_SOLID,

				///
				RENDER_MODE_TRANSPARENT,

				///
				RENDER_MODE_ALWAYS_FRONT
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

			// Lukas: do we need this for raytracing?
			/*
			/// Initialise the renderer, e.g. the display lists.
			virtual bool init(const Stage& stage, float height, float width)
				throw();

			/// Set the light sources according to the stage
			virtual void setLights(bool reset_all = false)
				throw();
			 */

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

			// Lukas: does this make sense for raytracing?
			/*
			// Initialise transparent rendering
			void initTransparent() 
				throw();

			// Initialise solid rendering
			void initSolid()
				throw();
			
			// Initialise always front rendering
			void initAlwaysFront()
				throw();

			/// Enable or disable antialiasing
			void setAntialiasing(bool state);
			 */
			
			/// Remove all references and auxilliary data for the given Representation
			void removeRepresentation(const Representation& rep)
				throw();

			/// Initialize the renderer for a new representation
			void addRepresentation(const Representation& rep)
				throw();
		
			/* Lukas: can we support those?
			///
			void setStereoMode(StereoMode state)
				throw();

			///
			StereoMode getStereoMode() const
				throw();
			*/

			///
			RenderMode getRenderMode() const
				throw();

			///
			void setRenderMode(RenderMode mode) { render_mode_ = mode;}
			
			///
			virtual bool render(const Representation& representation)
				throw();

			///
			String getRenderer();

			/*
			///
			DrawingMode getDrawingMode() const;
			*/

			///
			void initPerspective();

			void raytraceRepresentation_(const Representation& representation);

			//_
			void setColorRGBA_(const ColorRGBA& color)
				throw();

			//_
			void generateIlluminationTexture_(float ka, float kd, float kr, float shininess);

			inline Position getTextureIndex_(Position x, Position y, Position z, Size width, Size height);
			Position createTextureFromGrid(const RegularData3D& grid, const ColorMap& map);
			void removeTextureFor_(const RegularData3D& grid);
			
			/* Lukas: can we supprot clipping planes??  
			void setupGridClipPlanes_(const GridVisualisation& slice);
			*/
			Scene* 								scene_;

			///
			DrawingMode 					drawing_mode_;

			///
			Index 								drawing_precision_;

			//_
			float 								x_scale_;

			//_
			float 								y_scale_;

			// naming of geometric objects
			typedef HashMap<const GeometricObject*, Name> NameHashMap;
			typedef HashMap<Name, const GeometricObject*> GeometricObjectHashMap;
			typedef HashMap<const Representation*, GLDisplayList*> DisplayListHashMap;
			typedef HashMap<const Representation*, vector<MeshBuffer*> > MeshBufferHashMap;

			GeometricObjectHashMap	name_to_object_;
			NameHashMap							object_to_name_;
			DisplayListHashMap 			display_lists_;
			MeshBufferHashMap 			rep_to_buffers_;
			Name 										all_names_;
			Vector3 								normal_vector_;

			StereoMode 							stereo_;
			RenderMode 							render_mode_;

			bool 										use_vertex_buffer_;
			bool 										picking_mode_;
			ModelType 							model_type_;
			Position 								display_lists_index_;
			bool 										single_pick_;
			bool 										drawed_other_object_;
			bool 										drawed_mesh_;
			HashMap<const RegularData3D*, Position> grid_to_texture_;
		};

#	ifndef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/VIEW/RENDERING/glRenderer.iC>
#	endif

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_RENDERING_GLRENDERER_H
