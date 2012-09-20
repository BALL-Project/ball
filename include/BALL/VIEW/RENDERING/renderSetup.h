// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_RENDERING_RENDERSETUP_H
#define BALL_VIEW_RENDERING_RENDERSETUP_H

#ifndef BALL_VIEW_RENDERING_GLRENDERER_H
# include <BALL/VIEW/RENDERING/RENDERERS/glRenderer.h>
#endif

#ifndef BALL_VIEW_RENDERING_RENDERTARGET_H
# include <BALL/VIEW/RENDERING/renderTarget.h>
#endif

#ifndef BALL_VIEW_RENDERING_GLRENDERWINDOW_H
# include <BALL/VIEW/RENDERING/glRenderWindow.h>
#endif

#ifndef BALL_SYSTEM_MUTEX_H
# include <BALL/SYSTEM/mutex.h>
#endif

#include <QtCore/QThread>
#include <QtCore/QWaitCondition>

#include <boost/enable_shared_from_this.hpp>

#include <deque>

class QImage;

namespace BALL {
	namespace VIEW {

		class ColorMap;
		class Scene;

		/** This class encapsulates a (renderer, target) pair for Scene.
		 *
		 * 	Please try to call functions inside this class instead of working with
		 * 	the renderers and targets directly to ensure thread safety.
		 */
		class BALL_VIEW_EXPORT RenderSetup
			:	public QThread, public boost::enable_shared_from_this<RenderSetup>
		{
			public:
				RenderSetup(Renderer* r, RenderTarget* t, Scene* scene, const Stage* stage);

				RenderSetup(const RenderSetup& rs);

				virtual ~RenderSetup();

				const RenderSetup& operator = (const RenderSetup& rs);

				// TODO: this should be boost smart pointers!
				Renderer* 			renderer;
				RenderTarget*		target;

				/** The type of the encapsulated renderer
				 */
				enum RendererType
				{
					/// A fallback
					UNKNOWN_RENDERER = 0,

					/// Standard OpenGL renderer
					OPENGL_RENDERER,

					/// Renders into a POVRay file
					POV_RENDERER,

					/// Renders into a VRML file
					VRML_RENDERER,

					/// Renders into an STL file
					STL_RENDERER,

					/// Used for offscreen rendering
					TILING_RENDERER,

					/// RTfact renderer (depends on the RTfact library)
					RTFACT_RENDERER
				};

				enum STEREO_SETUP {
					NONE,
					LEFT_EYE,
					RIGHT_EYE
				};

				/** Initialize renderer and target.
				 */
				void init();

				/** Resize renderer and target.
				 */
				void resize(Size width, Size height);

				/** Pause rendering.
				 *  This function allows to skip rendering while still
				 *  buffering representations. Thus, a paused renderer
				 *  will still receive all updates to representations,
				 *  it will just not display anything.
				 */
				void pauseRendering() { rendering_paused_ = true; }

				/** This function allows to activate a paused renderer
				 *  again. Please note that the corresponding window will
				 *  not be automatically updated. This has to be performed
				 *  by the caller.
				 */
				void startRendering() { rendering_paused_ = false; }

				/** This function returns the rendering state of our renderer.
				 */
				bool isPaused() const { return rendering_paused_; }

				/** Returns the type of the encapsulated renderer.
				 */
				RendererType getRendererType() { return renderer_type_; }

				/** Prevent updating of rendering buffers.
				 *
				 *  This function allows to prevent the renderer from buffering or 
				 * 	deleting representations.
				 *
				 * 	Please note that this is potentially dangerous, since the renderer
				 * 	will not be made aware of deleted representations. Callers should
				 * 	make sure that the renderer state is effectively reset upon reactivating
				 * 	this switch.
				 *
				 * 	A common use case for this function is to support two render targets 
				 * 	sharing one render context, where we want to prevent the second target
				 * 	from rebuffering all representations already seen by the first.
				 * 	
				 */
				void setReceiveBufferUpdates(bool do_receive) { receive_updates_ = do_receive; }

				/** Returns the buffering state of the renderer.
				 */
				bool receivesBufferUpdates() const { return receive_updates_; };

				/** Tells the connected renderer to update its camera.
				 *
				 * 	This function applies transformations like adding an offset to camera
				 * 	position and view point before handing over the camera to the renderer.
				 */
				void updateCamera(const Camera* camera = 0);

				/** Sets an offset to apply to camera position and look at point.
				 *
				 *  Please note that the offset is interpreted as relative to the camera with
				 *  components (right, up, view) and will be updated at each position change.
				 */
				void setOffset(const Vector3& offset);

				/** This function turns the connected renderer into part of a stereo setup.
				 *
				 * 	Depending on the value set here, renderers will be passed additional information
				 * 	upon update camera to set eye separation and focal distance or to prepare correct
				 * 	frustra.
				 */
				void setStereoMode(STEREO_SETUP stereo) { stereo_setup_ = stereo; };

				/** Makes the target the current context and forwards the given representation to the renderer for buffering.
				 *  
				 *  If the renderer is not receiving buffer updates, this is a noop.
				 */
				void bufferRepresentation(const Representation& rep);

				/** Makes the target the current context and forwards the given representation to the renderer for removal.
				 *  
				 *  If the renderer is not receiving buffer updates, this is a noop.
				 */
				void removeRepresentation(const Representation& rep);

				/** Renders the current state into the given buffer.
				 *
				 *  Please note that this function only does anything when rendering is event-based, and if the renderer is not paused.
				 *  If continuous rendering is set to true, the updates will be performed in a continuous
				 *  loop instead and this function will immediately return.
				 */
				void renderToBuffer();

				/** Render the current state into a PNG file.
				 */
				bool exportPNG(const String& filename);

				/** Calls light setup for the current renderer.
				 */
				void setLights(bool reset_all = false);

				/** Setup background color.
				 */
				void updateBackgroundColor();
				
				/** Setup an environment map if supported by the Renderer.
				 */
				void setupEnvironmentMap(const QImage& image);

				/** Send a grid texture to the renderer.
				 *  
				 *  If the current renderer can not handle this kind of object,
				 *  this is a noop.
				 */
				Position prepareGridTextures(const RegularData3D& grid, const ColorMap& map);

				/** Remove a grid texture from the renderer.
				 *  
				 *  If the current renderer can not handle this kind of object,
				 *  this is a noop.
				 */
				void removeGridTextures(const RegularData3D& grid);

				/** Compute the 3D position on the view plane corresponding
				 *  to point (x,y) on the view port
				 */
				Vector3 mapViewportTo3D(Position x, Position y);

				/** Compute the 2D position on the screen corresponding
				 *  to the 3D point vec
				 */
				Vector2 map3DToViewport(const Vector3& vec);

				/** Pick all objects in the given screen rectangle.
				 */
				void pickObjects(Position x1, Position y1, Position x2, Position y2, std::list<GeometricObject*>& objects);

				/** Show a simple ruler.
				 *
				 *  If supported by the renderer implementation, this function can be used 
				 *  to produce a simple ruler that is rendered together with the other 
				 *  representations. 
				 *  The main use of this function is in the edit mode, where it can help to 
				 *  straighten-up structures and to correctly estimate angles and distances.
				 */
				void showRuler(bool show);
				
				void updateMaterialForRepresentation(const Representation* rep);

				/** Notifies the renderer that the projection mode has changed.
				 */
				void projectionModeChanged();

				/** Decide between event based and (threaded) continuous loop rendering.
				 */
				void useContinuousLoop(bool use_loop);

				/** Signal the renderer to quit as soon as possible.
				 *
				 *  Note: this function works asynchronously, so a rs.wait(); maybe required.
				 */
				void stop() { MutexLocker ml(&render_mutex_); about_to_quit_ = true; }

				/** Indicates whether the renderer has already been stopped.
				 */
				bool isAboutToQuit() { return about_to_quit_; }

				/** Returns the mode of the render loop.
				 */
				bool isContinuous() { return use_continuous_loop_; }

				/** Sets the number of render operations this renderer should perform.
				 *
				 *  Every render operation will decrease the time to live of this RenderSetup.
				 *  If ttl == 0, the owner of this RenderSetup (e.g., the Scene), will usually
				 *  destroy this instance.
				 *
				 *  To represent an infinite live time, set ttl = -1 (this is also the default).
				 */
				void setTimeToLive(int ttl) { ttl_ = ttl; }

				/** Returns the time to live for this renderer.
				 */
				int getTimeToLive() const { return ttl_; }

				/** Export rendered image to PNG after time to live has expired.
				 */
				void exportPNGAfterTTL(String filename) { export_after_ttl_ = true; export_after_ttl_filename_ = filename; }

				/** Ensure correct current rendering context.
				 		
					  If the target type uses a GL context, this is made current if
						necessary. If the target does not need GL, this function is a
						noop.
				*/
				virtual void makeCurrent();

				virtual void run();

				void lock() { render_mutex_.lock(); }
				void unlock() { render_mutex_.unlock(); }

				mutable QWaitCondition wait_for_render;
				// locks the renderer before the next render call
				mutable Mutex loop_mutex;

				/// Returns true if the buffer has been filled and just waits for painting
				bool bufferIsReady() { return buffer_is_ready_; }

				/// Returns true iff bufferIsReady() holds for this renderer and all dependent ones
				bool isReadyToSwap();

				///
				void setBufferReady(bool is_ready) { buffer_is_ready_ = is_ready; }

				///
				std::deque<boost::shared_ptr<RenderSetup> >& getDependentRenderers() { return keep_in_sync_; }

				///
				void makeDependentOn(boost::shared_ptr<RenderSetup>& partner) { keep_in_sync_.push_back(partner); }

			protected:
				// does the hard work and can be called from a continuous loop as well as from event-based rendering
				void renderToBuffer_();

				void initType_();

				bool rendering_paused_;
				bool receive_updates_;
				bool use_offset_;

				bool about_to_quit_;

				Camera  camera_;
				Vector3 camera_offset_;

				STEREO_SETUP stereo_setup_;

				// switches between rendering continously and rendering upon events
				bool use_continuous_loop_;

				Scene* scene_;
				Stage const* stage_;

				// locks the renderer during updates and rendering
				mutable Mutex render_mutex_;
				
				bool show_ruler_;

				// This pointer is used to avoid uneccessary RTTI calls and casting. If the target is not a
				// GLRenderWindow or one of its derived classes, this pointer will simply be NULL
				GLRenderWindow* gl_target_;
				
				// This pointer is used to avoid uneccessary RTTI calls and casting. If the renderer is not a
				// GLRenderer or one of its derived classes, this pointer will simply be NULL
				GLRenderer* gl_renderer_;

				// The number of render operations to perform before destruction
				int ttl_;

				bool export_after_ttl_;
				String export_after_ttl_filename_;

				// a list of render setups that should be synchronized with this one so that buffer
				// swaps only happen simulatneously
				std::deque<boost::shared_ptr<RenderSetup> > keep_in_sync_;

				bool buffer_is_ready_;

				// the type of the encapsulated renderer
				RendererType renderer_type_;
		};

		/** This class is used for communication of render events over thread boundaries.
		 */
		class BALL_VIEW_EXPORT RenderToBufferFinishedEvent
			: public QEvent
		{
			public:
				RenderToBufferFinishedEvent(boost::shared_ptr<RenderSetup> renderer)
					: QEvent(static_cast<QEvent::Type>(RENDER_TO_BUFFER_FINISHED_EVENT)),
						renderer_(renderer)
				{};

				boost::shared_ptr<RenderSetup> getRenderer() { return renderer_; }

			protected:
				boost::shared_ptr<RenderSetup> renderer_;
		};

	}
}

#endif
