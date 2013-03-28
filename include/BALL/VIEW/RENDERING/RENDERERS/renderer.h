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

#ifndef BALL_VIEW_RENDERING_RENDERTARGET_H
# include <BALL/VIEW/RENDERING/renderTarget.h>
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
		class FrameBuffer;
		class RenderTarget;
		class RenderSetup;
		class Resolution;
		class PixelFormat;

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

				/**	@name	Constructors and Destructors
				 */	
				//@{

				/** Default Constructor.
				 */
				Renderer(const QString& name = "GenericRenderer");

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

				/** Render the current frame to the target's buffer using the \link Stage \endlink setup.
				 *  Throws FrameBufferFormatException if the RenderTarget's format is not
				 *  the one that you set before with setFrameBufferFormat().
				 *  @throw Exception::InvalidFormat if the FrameBufferFormat is not supported by the renderer
				 *  @throw Exception::NoBufferAvailable if the frame buffer was not correctly initialized
				 */
				void renderToBuffer(RenderTarget* renderTarget);

				/** Tries to choose a format for buffered rendering.
				 *  @return true if the format could be set,
				 *  @return false if it is not supported.
				 */
				bool setFrameBufferFormat(const FrameBufferFormat &format);

				/**
				 * Updates the material for the current representation
				 */
				virtual void updateMaterialForRepresentation(const Representation*) {}

				/** Decide between event based and (threaded) continuous loop rendering.
				 */
				virtual void useContinuousLoop(bool use_loop) 
				{ 
					// TODO: mutex for use_continuous_loop_ just to be on the safe side
					use_continuous_loop_ = use_loop;
				};

				/** Checks if a particular PixelFormat is supported by the renderer. */
				virtual bool supports(const PixelFormat &format) const = 0;

				/**
				 * Creates a render setup that links this renderer to a render setup
				 */
				virtual boost::shared_ptr<RenderSetup> createRenderSetup(RenderTarget* target, Scene* scene);

				/** Returns a supported resolution within the given limits.
				 *  The x resolution will be between min.x and max.x and the y resolution
				 *  will be between min.y and max.y.
				 *  The returned resolution must be supported by the renderer with the
				 *  requested PixelFormat.
				 *  @throw Exception::FormatUnsupported if the FrameBufferFormat is not supported by the renderer
				 */
				virtual Resolution getSupportedResolution(
						const Resolution &min, const Resolution &max,
						const PixelFormat &format) const;

				/** Set an offset into the data buffer 
				 */
				virtual void setOffset(Size offset) {offset_ = offset;}

				/** Set a stride for the data buffer
				 */
				virtual void setStride(Size stride) {stride_ = stride;}


				virtual std::vector<float> intersectRaysWithGeometry(const std::vector<Vector3>& origins,
						const std::vector<Vector3>& directions);

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
				virtual const QString& getName() const { return name_; }

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

			protected:
				/** Checks if a particular FrameBufferFormat is supported by the renderer.
				 *  This checks the combination of pixel format and resolution.
				 *  If this returns true, the renderer is able to write to a buffer in
				 *  this format.
				 */
				virtual bool supports(const FrameBufferFormat &format) const;

				//@}

				/** This is called once the FrameBufferFormat has been set so the renderer
				 *  can perform any initialization steps that are needed.
				 */
				virtual void formatUpdated() = 0;

				/** Render to the frame buffer.
				 *  @pre The frame buffer's format is the one stored in bufferFormat and
				 *       prepareBufferedRendering() has been called.
				 */
				virtual void renderToBufferImpl(boost::shared_ptr<FrameBuffer> buffer) = 0;            

				/** Returns the current FrameBufferFormat. */
				const FrameBufferFormat& getFrameBufferFormat() const;

				Size offset_;
				Size stride_;

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
				QString name_;

			private:
				FrameBufferFormat bufferFormat;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_RENDERING_RENDERER_H
