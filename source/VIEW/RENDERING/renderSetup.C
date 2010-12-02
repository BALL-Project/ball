#include <BALL/VIEW/RENDERING/renderSetup.h>

#include <BALL/VIEW/RENDERING/bufferedRenderer.h>
#include <BALL/VIEW/RENDERING/tilingRenderer.h>
#include <BALL/VIEW/RENDERING/glRenderWindow.h>

#include <BALL/VIEW/WIDGETS/scene.h>


#include <BALL/SYSTEM/timer.h>

#ifdef ENABLE_RAYTRACING
#include <BALL/VIEW/RENDERING/RENDERERS/rtfactRenderer.h>

#define USE_TBB
#ifdef USE_TBB
# include <tbb/task_scheduler_init.h>
#endif
#endif

namespace BALL
{
	namespace VIEW
	{
		RenderSetup::RenderSetup(Renderer* r, RenderTarget* t, Scene* scene, const Stage* stage)
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
				stage_(stage),
				render_mutex_(true),
				show_ruler_(false)
		{
			gl_target_ = dynamic_cast<GLRenderWindow*>(target);
		}

		RenderSetup::RenderSetup(const RenderSetup& rs)
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
				stage_(rs.stage_),
				render_mutex_(true),
				show_ruler_(rs.show_ruler_)
		{
			gl_target_ = dynamic_cast<GLRenderWindow*>(target);
		}

		const RenderSetup& RenderSetup::operator = (const RenderSetup& rs)
		{
			render_mutex_.lock();

			renderer = rs.renderer;
			target = rs.target;
			rendering_paused_ = rs.rendering_paused_;
			receive_updates_ = rs.receive_updates_;
			use_offset_ = rs.use_offset_;
			camera_ = rs.camera_;
			camera_offset_ = rs.camera_offset_;
			stereo_setup_ = rs.stereo_setup_;
			use_continuous_loop_ = rs.use_continuous_loop_;
			scene_ = rs.scene_;
			stage_ = rs.stage_;

			width_  = rs.width_;
			height_ = rs.height_;

			show_ruler_ = rs.show_ruler_;

			gl_target_ = dynamic_cast<GLRenderWindow*>(target);

			render_mutex_.unlock();

			return *this;
		}

		void RenderSetup::init()
		{
			render_mutex_.lock();

			do_resize_ = false;

			makeCurrent();

			// initialize the rendering target
			target->init();

			// initialize the renderer
			if(!renderer->init(*scene_))
			{
				Log.error() << "Renderer failed to initialize" << std::endl;
				throw Exception::GeneralException(__FILE__, __LINE__);
			}			
			if (RTTI::isKindOf<GLRenderer>(*renderer))
				((GLRenderer*)renderer)->enableVertexBuffers(scene_->want_to_use_vertex_buffer_);

			render_mutex_.unlock();
		}

		void RenderSetup::resize(Size width, Size height)
		{
			// prevent resizing of full screen Windows because they are
			// most probably stereo half images
			if (gl_target_ && gl_target_->isFullScreen())
				return;

			bool reset_continuous = false;

			if (use_continuous_loop_)
			{
				width_  = width;
				height_ = height;
				do_resize_ = true;
				// stop the thread
				useContinuousLoop(false);
				reset_continuous = true;
			}
			render_mutex_.lock();

			makeCurrent();

			if(!target->resize(width, height))
			{
				Log.error() << "Cannot resize window. Size " 
										<< width  << " x " 
										<< height << " is not supported" << std::endl;
			}

			if (RTTI::isKindOf<BufferedRenderer>(*renderer))
			{
				if(!(((BufferedRenderer*)renderer)->setFrameBufferFormat(target->getFormat())))
				{
					Log.error() << "Raytracing render does not support window framebuffer format. Seems to be configuration error" << std::endl;

					render_mutex_.unlock();

					throw Exception::GeneralException(__FILE__, __LINE__);
				}
			}

			renderer->setSize(width, height);

			render_mutex_.unlock();

			updateCamera();

			renderToBuffer_();

			render_mutex_.lock();

			if (gl_target_)
				gl_target_->swapBuffers();

			if (reset_continuous)
			{
//				use_continuous_loop_ = true;
//				start();
			}

			render_mutex_.unlock();
		}

		void RenderSetup::updateCamera(const Camera* camera)
		{
			Stage const& stage = renderer->getStage();
			 
			camera_ = (camera == 0) ? stage.getCamera() : *camera;

			render_mutex_.lock();

			if (stereo_setup_ != NONE)
			{
				float eye_separation = stage.getEyeDistance() * 0.5;
				eye_separation *= (stereo_setup_ == LEFT_EYE) ? -1. : 1.;
				eye_separation *= stage.swapSideBySideStereo() ? -1. : 1.;

				renderer->setupStereo(eye_separation, stage.getFocalDistance());

				camera_offset_  = Vector3(eye_separation, 0., 0.);

				use_offset_ = true;
			}

			if (use_offset_)
			{
				Vector3 const& right = camera_.getRightVector();
				Vector3 const& up    = camera_.getLookUpVector();
				Vector3 const& view  = camera_.getViewVector();

				Vector3 absolute_offset = 	right * camera_offset_.x
																	+ up    * camera_offset_.y
																	+ view  * camera_offset_.z;

				camera_.setViewPoint(camera_.getViewPoint()+absolute_offset);
				camera_.setLookAtPosition(camera_.getLookAtPosition()+absolute_offset);
			}

			renderer->updateCamera(&camera_);

			render_mutex_.unlock();
		}

		void RenderSetup::setOffset(const Vector3& offset)
		{
			camera_offset_ = offset;

			if (offset.getSquareLength() > 0)
				use_offset_ = true;
			else
				use_offset_ = false;
		}

		void RenderSetup::useContinuousLoop(bool use_loop)
		{
			// TODO: mutex for use_continuous_loop_ just to be on the safe side
			use_continuous_loop_ = use_loop;
			
			render_mutex_.lock();
			renderer->useContinuousLoop(use_loop);
			render_mutex_.unlock();
		}

		void RenderSetup::makeCurrent()
		{
			if (gl_target_ && 
				   (QGLContext::currentContext() != gl_target_->context()))
				gl_target_->makeCurrent();
			else
				target->prepareRendering();
		}

		void RenderSetup::run()
		{
#ifdef USE_TBB
			tbb::task_scheduler_init init;
#endif

			makeCurrent();

			if (gl_target_)
				gl_target_->ignoreEvents(true);

			useContinuousLoop(true);
			Timer t;

			// to be stopped from the outside, someone needs to call useContinuousLoop(false)
			while (use_continuous_loop_)
			{
				printf("###################################### FRAME #####################################\n");
				t.start();
				renderToBuffer_();
				t.stop();
				printf("###################################### DONE (%f)  #####################################\n", t.getClockTime());
				t.reset();
				msleep(16);
			}

			if (gl_target_)
				gl_target_->ignoreEvents(false);
		}

		void RenderSetup::renderToBuffer()
		{
			if (!use_continuous_loop_)
				renderToBuffer_();
		}

		void RenderSetup::renderToBuffer_()
		{
			if (rendering_paused_)
			{
				if (use_continuous_loop_)
					msleep(100);
				return;
			}

			render_mutex_.lock();

			makeCurrent();

			renderer->setPreviewMode(scene_->use_preview_ && scene_->preview_);
			renderer->showLightSources(scene_->show_light_sources_);

			updateCamera();

			if (RTTI::isKindOf<BufferedRenderer>(*renderer))
			{
				((BufferedRenderer*)renderer)->renderToBuffer(target, *stage_);
				target->refresh();
			} 
			else if (RTTI::isKindOf<GLRenderer>(*renderer))
			{
				GLRenderer* current_gl_renderer = (GLRenderer*)renderer;

				// cannot call update here, because it calls updateGL
				current_gl_renderer->renderToBuffer(target, GLRenderer::DISPLAY_LISTS_RENDERING);

				glFlush();
			}
			else if (RTTI::isKindOf<TilingRenderer>(*renderer))
			{
				static_cast<TilingRenderer*>(renderer)->renderToBuffer(target);
				glFlush();
			}

			if (show_ruler_)
				renderer->renderRuler();

			if (use_continuous_loop_ && gl_target_)
				gl_target_->swapBuffers();

			render_mutex_.unlock();
		}

		bool RenderSetup::exportPNG(const String& filename)
		{
			if (!gl_target_)
				return false;

			render_mutex_.lock();

			makeCurrent();
			QImage image(gl_target_->grabFrameBuffer(true));

			render_mutex_.unlock();

			bool ok = image.save(filename.c_str(), "PNG");

			return ok;
		}

		void RenderSetup::bufferRepresentation(const Representation& rep)
		{
			if (receive_updates_)
			{
				if (use_continuous_loop_)
					useContinuousLoop(false);

				render_mutex_.lock();

				makeCurrent();
				renderer->bufferRepresentation(rep);

				render_mutex_.unlock();
			}
		}

		void RenderSetup::removeRepresentation(const Representation& rep)
		{
			if (receive_updates_)
			{
				if (use_continuous_loop_)
					useContinuousLoop(false);

				render_mutex_.lock();

				makeCurrent();
				renderer->removeRepresentation(rep);

				render_mutex_.unlock();
			}
		}

		void RenderSetup::setLights(bool reset_all)
		{
			if (reset_all && use_continuous_loop_)
				useContinuousLoop(false);

			render_mutex_.lock();

			makeCurrent();
			renderer->setLights(reset_all);

			render_mutex_.unlock();
		}

		void RenderSetup::updateBackgroundColor()
		{
			if (use_continuous_loop_)
				useContinuousLoop(false);

			render_mutex_.lock();

			makeCurrent();
			renderer->updateBackgroundColor();

			render_mutex_.unlock();
		}

		Position RenderSetup::prepareGridTextures(const RegularData3D& grid, const ColorMap& map)
		{
			Position texname = 0;

			if (RTTI::isKindOf<GLRenderer>(*renderer))
			{
				if (use_continuous_loop_)
					useContinuousLoop(false);

				render_mutex_.lock();

				makeCurrent();
				texname = ((GLRenderer*)renderer)->createTextureFromGrid(grid, map);

				render_mutex_.unlock();
			}

			return texname;
		}

		void RenderSetup::removeGridTextures(const RegularData3D& grid)
		{
			if (RTTI::isKindOf<GLRenderer>(*renderer))
			{
				if (use_continuous_loop_)
					useContinuousLoop(false);

				MutexLocker ml(&render_mutex_);

				makeCurrent();
				((GLRenderer*)renderer)->removeTextureFor_(grid);
			}
		}

		Vector3 RenderSetup::mapViewportTo3D(Position x, Position y)
		{
			MutexLocker ml(&render_mutex_);
			render_mutex_.lock();

			makeCurrent();
			return renderer->mapViewportTo3D(x, y);
		}

		Vector2 RenderSetup::map3DToViewport(const Vector3& vec)
		{
			MutexLocker ml(&render_mutex_);

			makeCurrent();
			return renderer->map3DToViewport(vec);
		}

		void RenderSetup::pickObjects(Position x1, Position y1,
		                              Position x2, Position y2,
		                              list<GeometricObject*>& objects)
		{
			if (!RTTI::isKindOf<GLRenderer>(*renderer))
			{
				Log.error() << "RenderSetup::pickObjects not supported for this kind of renderer!" << std::endl;
				return;
			}
	
			MutexLocker ml(&render_mutex_);

			makeCurrent();

			((GLRenderer*)renderer)->pickObjects1(x1, y1, x2, y2);
			((GLRenderer*)renderer)->renderToBuffer(target, GLRenderer::DIRECT_RENDERING);
			((GLRenderer*)renderer)->pickObjects2(objects);
		}

		void RenderSetup::showRuler(bool show)
		{
			MutexLocker ml(&render_mutex_);
			show_ruler_ = show;
		}

		void RenderSetup::projectionModeChanged()
		{
			if (RTTI::isKindOf<GLRenderer>(*renderer))
			{
				static_cast<GLRenderer*>(renderer)->initPerspective();
			}
		}
	}
}
