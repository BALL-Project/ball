// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_RENDERING_RENDERERS_RENDERER_H
#define BALL_VIEW_RENDERING_RENDERERS_RENDERER_H

#ifndef BALL_VIEW_KERNEL_REPRESENTATION_H
#	include <BALL/VIEW/KERNEL/representation.h>
#endif

#ifndef BALL_CONCEPT_OBJECT_H
#	include <BALL/CONCEPT/object.h>
#endif

#ifndef BALL_VIEW_KERNEL_STAGE_H
# include <BALL/VIEW/KERNEL/stage.h>
#endif

#ifndef BALL_MATHS_VECTOR2_H
# include <BALL/MATHS/vector2.h>
#endif

class QImage;

namespace BALL
{
	namespace VIEW
	{
		class Scene;
		class Line;
		class MultiLine;
		class Tube;
		class Mesh;
		class Label;
		class Point;
		class Sphere;
		class Disc;
		class Box;
		class SimpleBox;
		class TwoColoredLine;
		class TwoColoredTube;
		class ClippingPlane;
		class GridVisualisation;
		class QuadMesh;

		/** Renderer is just a generic base class.
		 		Derived classes are GLRenderer and POVRenderer.
				Every renderer has a pointer to a Stage object, which contains the viewpoint,
				the LightSource, etc. The renderer also knows the width and height, of the display, it shall
				render.
				\ingroup ViewRendering
		*/
		class BALL_VIEW_EXPORT Renderer
		{
			public:

			///
 			enum StereoMode
			{
				NO_STEREO = 0,

				/// Stereo mode for shutter glasses
				ACTIVE_STEREO,

				/// Stereo mode for output on two projectors
				DUAL_VIEW_STEREO,

				/// Stereo mode for output on two projectors with one display per head
				DUAL_VIEW_DIFFERENT_DISPLAY_STEREO,

				/// Stereo mode for top bottom views
				TOP_BOTTOM_STEREO,

				/// Line interlaced stereo
				INTERLACED_STEREO
			};

			BALL_CREATE(Renderer)

			/**	@name	Constructors and Destructors
			*/
			//@{

			/** Default Constructor.
			*/
			Renderer();

			/**	Copy constructor.
			*/
			Renderer(const Renderer& renderer);

			/** Destructor
			*/
			virtual ~Renderer() {}

			/** Explicit default initialization.
			*/
			virtual void clear(){}

			/** Initialization routines.
			 		This method is called by Scene::initializeGL.
			*/
			virtual bool init(Scene& scene);

			/** Initialisation with the Stage.
			 		Called by Scene::exportScene().
			 */
			virtual bool init(const Stage& stage, float height, float width);

			/// Set the light sources according to the stage
			virtual void setLights(bool reset_all = false);

			/** Update the camera position either from a given Camera, or from the default Stage.
			 */
			virtual void updateCamera(const Camera* camera = 0);

			/**
			 * Updates the material for the current representation
			 */
			virtual void updateMaterialForRepresentation(Representation const*) {};

			/// Update the background color from the stage
			virtual void updateBackgroundColor();

			/// Setup an environment map
			virtual void setupEnvironmentMap(const QImage& image);

			/** Sets this renderer as a part of a stereo setup.
			 *
			 *  eye_separation denotes the distance along the right vector used
			 *  by this "eye".
			 */
			virtual void setupStereo(float eye_separation, float focal_length);

			/** Decide between event based and (threaded) continuous loop rendering.
			*/
			virtual void useContinuousLoop(bool use_loop)
			{
				// TODO: mutex for use_continuous_loop_ just to be on the safe side
				use_continuous_loop_ = use_loop;
			};

			/** Returns the mode of the render loop.
			*/
			bool isContinuous() { return use_continuous_loop_; }

			/** Whether renderer has its own FPS counter.
			 */
			virtual bool hasFPScounter() { return false; }

			/** Internal renderer FPS if available, otherwise returns 0.
			 */
			virtual double getFPS() { return 0; }

			///
			virtual bool finish();

			/** Compute the 3D position on the view plane corresponding
			 *  to point (x,y) on the view port
			 */
			virtual Vector3 mapViewportTo3D(Position x, Position y);

			/** Compute the 2D position on the screen corresponding
			 *  to the 3D point vec
			 */
			virtual Vector2 map3DToViewport(const Vector3& vec);

			/** Pick all objects in the given screen rectangle.
			 */
			virtual void pickObjects(Position x1, Position y1, Position x2, Position y2,
			                         std::list<GeometricObject*>& objects);

			/** Return the frustum used by this renderer.
			 */
			virtual void getFrustum(float& near_f, float& far_f, float& left_f, float& right_f,
			                        float& top_f,  float& bottom_f);

			//@}
			/**	@name	Accessors
			*/
			//@{

			/** Render a Representation.
			*/
			virtual bool renderOneRepresentation(const Representation& representation);


			/** Buffer a Representation for later rendering.
			 */
			virtual void bufferRepresentation(const Representation& /*rep*/) {};

			/** Remove a representation from the buffer.
			 */
			virtual void removeRepresentation(const Representation& /*rep*/) {};

			/** Get the stage for the renderer (const)
			 */
			virtual const Stage& getStage() const
				{ return *stage_;}

			/** Set the stage for the renderer
			 */
			virtual void setStage(const Stage& stage)
				{ stage_ = &stage;}

			/// Test if a Stage was assigned to the Renderer
			bool hasStage() const;

			/// Set the size of the display
			virtual void setSize(float width, float height) {width_ = width; height_ = height;}

			///
			virtual float getWidth() const { return width_;}

			///
			virtual float getHeight() const { return height_;}

			/** Decides whether the renderer should use a faster preview mode.
			 *  Please note that not all renderers support previews and may choose
			 *  just to ignore this setting.
			 */
			virtual void setPreviewMode(bool show_preview) { show_preview_ = show_preview; }

			/** Decides whether to draw light sources explicitly.
			 *  Please note that not all renderers support this feature and may choose
			 *  just to ignore this setting.
			 */
			virtual void showLightSources(bool show_light_sources) { show_light_sources_ = show_light_sources; }

			/** Render a ruler.
			 *
			 *  If supported by the renderer implementation, this function will produce
			 *  a simple ruler that is rendered together with the other representations.
			 *  The main use of this function is in the edit mode, where it can help to
			 *  straighten-up structures and to correctly estimate angles and distances.
			 */
			virtual void renderRuler();

			///
			virtual void setStereoMode(StereoMode state);

			///
			virtual StereoMode getStereoMode() const;

			///
			virtual void setStereoFrustumConversion(int width_factor, int height_factor);

			//@}
			/**	@name Predicates
			*/
			//@{

			///
			bool operator == (const Renderer& /*renderer*/) const
				{ return true; }

			/// Wrapper for the renderering of special GeometricObjects
			virtual void render_(const GeometricObject* object);

			protected:

			///
			virtual void renderClippingPlane_(const ClippingPlane&)
				{Log.error() << "renderClippingPlane_ not implemented in derived Renderer class" << std::endl;}

			// Render a text label
			virtual void renderLabel_(const Label& /*label*/)
				{Log.error() << "renderLabel_ not implemented in derived Renderer class" << std::endl;}

			/// Render a line
			virtual void renderLine_(const Line& /*line*/)
				{Log.error() << "renderLine_ not implemented in derived Renderer class" << std::endl;}

			/// Render an illuminated line
			virtual void renderMultiLine_(const MultiLine& /*line*/)
				{Log.error() << "renderMultiLine_ not implemented in derived Renderer class" << std::endl;}

			/// Render a surface mesh
			virtual void renderMesh_(const Mesh& /*mesh*/)
				{Log.error() << "renderMesh_ not implemented in derived Renderer class" << std::endl;}

			/// Render a single point
 			virtual void renderPoint_(const Point& /*point*/)
				{Log.error() << "renderPoint_ not implemented in derived Renderer class" << std::endl;}

			/// Render a box
			virtual void renderBox_(const Box& /*box*/)
				{Log.error() << "renderBox_ not implemented in derived Renderer class" << std::endl;}

			/// Render a simple box (parallel to the axes)
			virtual void renderSimpleBox_(const SimpleBox& /*box*/)
				{Log.error() << "renderSimpleBox_ not implemented in derived Renderer class" << std::endl;}

			/// Render a sphere
			virtual void renderSphere_(const Sphere& /*sphere*/)
				{Log.error() << "renderSphere_ not implemented in derived Renderer class" << std::endl;}

			/// Render a disc
			virtual void renderDisc_(const Disc& /*disc*/)
				{Log.error() << "renderDisc_ not implemented in derived Renderer class" << std::endl;}

			/// Render a tube
			virtual void renderTube_(const Tube& /*tube*/)
				{Log.error() << "renderTube_ not implemented in derived Renderer class" << std::endl;}

			/// Render a line with two colors
			virtual void renderTwoColoredLine_(const TwoColoredLine& /*two_colored_line*/)
				{Log.error() << "renderTwoColoredLine_ not implemented in derived Renderer class" << std::endl;}

			/// Render a tube with two colors
			virtual void renderTwoColoredTube_(const TwoColoredTube& /*two_colored_tube*/)
				{Log.error() << "renderTwoColoredTube_ not implemented in derived Renderer class" << std::endl;}

			/// Render a grid
			virtual void renderGridVisualisation_(const GridVisualisation&)
				{Log.error() << "renderGridVisualisation_ not implemented in derived Renderer class" << std::endl;}
			/// Render a quad mesh
			virtual void renderQuadMesh_(const QuadMesh&)
				{Log.error() << "renderQuadMesh_ not implemented in derived Renderer class" << std::endl;}
				//@}

			//_
			Scene* scene_;

			//_
			const Stage*		stage_;

			//_The width of the render area
			float 					width_;

			//_The height of the render area
			float 					height_;

			//_
			bool						show_preview_;

			//_
			float						volume_width_;

			//_
			bool						show_light_sources_;

			//_
			Camera					camera_;

			// An offset added to camera position and look at
			Vector3					camera_offset_;

			bool						use_continuous_loop_;

			Size            stereo_frustum_conversion_width_;
			Size            stereo_frustum_conversion_height_;

			StereoMode 			stereo_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_RENDERING_RENDERER_H
