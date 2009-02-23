// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_RENDERING_RENDERSETUP_H
#define BALL_VIEW_RENDERING_RENDERSETUP_H

#ifndef BALL_VIEW_RENDERING_GLRENDERER_H
# include <BALL/VIEW/RENDERING/glRenderer.h>
#endif

#ifndef BALL_VIEW_RENDERING_RENDERWINDOW_H
# include <BALL/VIEW/RENDERING/renderWindow.h>
#endif

#ifndef BALL_VIEW_RENDERING_GLRENDERWINDOW_H
# include <BALL/VIEW/RENDERING/glRenderWindow.h>
#endif

#ifndef BALL_VIEW_WIDGETS_SCENE_H
# include <BALL/VIEW/WIDGETS/scene.h>
#endif

#ifndef BALL_SYSTEM_MUTEX_H
# include <BALL/SYSTEM/mutex.h>
#endif

#include <QtCore/qthread.h>

namespace BALL {
	namespace VIEW {

		/** This class encapsulates a (renderer, target) pair for Scene.
		 *
		 * 	Please try to call functions inside this class instead of working with
		 * 	the renderers and targets directly to ensure thread safety.
		 */
		class BALL_EXPORT RenderSetup
			:	public QThread
		{
			public:
				RenderSetup(Renderer* r, GLRenderWindow* t, Scene* scene, const Stage* stage)
					: QThread(),
						renderer(r),
						target(t),
						rendering_paused_(false),
						receive_updates_(true),
						use_offset_(false),
						camera_(),
						camera_offset_(Vector3(0.)),
						stereo_setup_(NONE),
						use_continuous_loop_(false),
						scene_(scene),
						stage_(stage)
				{}

				RenderSetup(const RenderSetup& rs)
					: QThread(),
						renderer(rs.renderer),
						target(rs.target),
						rendering_paused_(rs.rendering_paused_),
						receive_updates_(rs.receive_updates_),
						use_offset_(rs.use_offset_),
						camera_(rs.camera_),
						camera_offset_(rs.camera_offset_),
						stereo_setup_(rs.stereo_setup_),
						use_continuous_loop_(rs.use_continuous_loop_),
						scene_(rs.scene_),
						stage_(rs.stage_)
				{}

				const RenderSetup& operator = (const RenderSetup& rs);

				// TODO: this should be boost smart pointers!
				Renderer* 				renderer;
				GLRenderWindow*		target;

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

				/** Calls light setup for the current renderer.
				 */
				void setLights(bool reset_all = false);

				/** Setup background color.
				 */
				void updateBackgroundColor();
				
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

				/** Decide between event based and (threaded) continuous loop rendering.
				 */
				void useContinuousLoop(bool use_loop);

				/** Returns the mode of the render loop.
				 */
				bool isContinuous() { return use_continuous_loop_; }

				virtual void run();

			protected:
				// does the hard work and can be called from a continuous loop as well as from event-based rendering
				void renderToBuffer_();

				bool rendering_paused_;
				bool receive_updates_;
				bool use_offset_;

				Camera  camera_;
				Vector3 camera_offset_;

				STEREO_SETUP stereo_setup_;

				// switches between rendering continously and rendering upon events
				bool use_continuous_loop_;

				Scene* scene_;
				Stage const* stage_;

				// locks the renderer during updates and rendering
				mutable Mutex render_mutex_;
		};
	}
}

#endif
