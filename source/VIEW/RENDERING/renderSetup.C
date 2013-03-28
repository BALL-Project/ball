#include <BALL/VIEW/RENDERING/renderSetup.h>

#include <BALL/VIEW/RENDERING/RENDERERS/bufferedRenderer.h>
#include <BALL/VIEW/RENDERING/RENDERERS/tilingRenderer.h>
#include <BALL/VIEW/RENDERING/RENDERERS/POVRenderer.h>
#include <BALL/VIEW/RENDERING/RENDERERS/VRMLRenderer.h>
#include <BALL/VIEW/RENDERING/RENDERERS/STLRenderer.h>
#include <BALL/VIEW/RENDERING/glRenderWindow.h>

#include <BALL/VIEW/WIDGETS/scene.h>


#include <BALL/SYSTEM/timer.h> 
#ifdef BALL_HAS_RTFACT
#include <BALL/VIEW/RENDERING/RENDERERS/rtfactRenderer.h>

#define USE_TBB
#ifdef USE_TBB
# include <tbb/task_scheduler_init.h>
#endif
#endif

#include <QtGui/QApplication>

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
				about_to_quit_(false),
				camera_(),
				camera_offset_(Vector3(0.)),
				stereo_setup_(NONE),
				use_continuous_loop_(false),
				scene_(scene),
				stage_(stage),
				render_mutex_(true),
				show_ruler_(false),
				ttl_(-1),
				export_after_ttl_(false),
				export_after_ttl_filename_(),
				buffer_is_ready_(true)
		{
			initType_();

			gl_target_   = dynamic_cast<GLRenderWindow*>(target);
			gl_renderer_ = dynamic_cast<GLRenderer*>(renderer);
		}

		RenderSetup::RenderSetup(const RenderSetup& rs)
			: QThread(),
				renderer(rs.renderer),
				target(rs.target),
				rendering_paused_(rs.rendering_paused_),
				receive_updates_(rs.receive_updates_),
				use_offset_(rs.use_offset_),
				about_to_quit_(false),
				camera_(rs.camera_),
				camera_offset_(rs.camera_offset_),
				stereo_setup_(rs.stereo_setup_),
				use_continuous_loop_(rs.use_continuous_loop_),
				scene_(rs.scene_),
				stage_(rs.stage_),
				render_mutex_(true),
				show_ruler_(rs.show_ruler_),
				ttl_(rs.ttl_),
				export_after_ttl_(rs.export_after_ttl_),
				export_after_ttl_filename_(rs.export_after_ttl_filename_),
				buffer_is_ready_(rs.buffer_is_ready_),
				renderer_type_(rs.renderer_type_)
		{
			gl_target_   = dynamic_cast<GLRenderWindow*>(target);
			gl_renderer_ = dynamic_cast<GLRenderer*>(renderer);
		}

		RenderSetup::~RenderSetup()
		{
		}

		const RenderSetup& RenderSetup::operator = (const RenderSetup& rs)
		{
			if (&rs == this) return *this;

			render_mutex_.lock();

			renderer = rs.renderer;
			target = rs.target;
			rendering_paused_ = rs.rendering_paused_;
			receive_updates_ = rs.receive_updates_;
			use_offset_ = rs.use_offset_;
			about_to_quit_ = rs.about_to_quit_;
			camera_ = rs.camera_;
			camera_offset_ = rs.camera_offset_;
			stereo_setup_ = rs.stereo_setup_;
			use_continuous_loop_ = rs.use_continuous_loop_;
			scene_ = rs.scene_;
			stage_ = rs.stage_;
			ttl_ = rs.ttl_;
			export_after_ttl_ = rs.export_after_ttl_;
			export_after_ttl_filename_ = rs.export_after_ttl_filename_;

			show_ruler_ = rs.show_ruler_;

			gl_target_   = dynamic_cast<GLRenderWindow*>(target);
			gl_renderer_ = dynamic_cast<GLRenderer*>(renderer);

			buffer_is_ready_ = rs.buffer_is_ready_;
			renderer_type_   = rs.renderer_type_;

			render_mutex_.unlock();

			return *this;
		}

		void RenderSetup::init()
		{
			render_mutex_.lock();

			initType_();

			// Yes, this seems inconsistent... but we need to start with a ready (albeit empty)
			// buffer to boot the system
			buffer_is_ready_ = true;

			makeCurrent();

			gl_target_   = dynamic_cast<GLRenderWindow*>(target);
			gl_renderer_ = dynamic_cast<GLRenderer*>(renderer);

			// initialize the rendering target
			target->init();

			// initialize the renderer
			if(!renderer->init(*scene_))
			{
				Log.error() << "Renderer failed to initialize" << std::endl;
				throw Exception::GeneralException(__FILE__, __LINE__);
			}			

			if (gl_renderer_)
				gl_renderer_->enableVertexBuffers(scene_->useVertexBuffers());

			render_mutex_.unlock();
		}

		void RenderSetup::resize(Size width, Size height)
		{
			// prevent resizing of full screen Windows because they are
			// most probably stereo half images
			// TODO: Move this to the doNotResize call (reimplement in GLRenderWindow)
			if (gl_target_ && gl_target_->isFullScreen())
				return;

			if(target->doNotResize()) {
				return;
			}

			render_mutex_.lock();

			makeCurrent();

			if(!target->resize(width, height))
			{
				Log.error() << "Cannot resize window. Size " 
										<< width  << " x " 
										<< height << " is not supported" << std::endl;
			}

			renderer->setSize(width, height);
			if (RTTI::isKindOf<BufferedRenderer>(*renderer))
			{
				if(!(((BufferedRenderer*)renderer)->setFrameBufferFormat(target->getFormat())))
				{
					Log.error() << "Raytracing render does not support window framebuffer format. Seems to be configuration error" << std::endl;

					render_mutex_.unlock();

					throw Exception::GeneralException(__FILE__, __LINE__);
				}
			}

			render_mutex_.unlock();

			updateCamera();

			//renderToBuffer_();

			render_mutex_.lock();

			if (gl_target_)
				gl_target_->swapBuffers();

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

#ifndef BALL_HAS_RTFACT
				renderer->setupStereo(eye_separation, stage.getFocalDistance());
#else
				//target->setupStereo(eye_separation, stage.getFocalDistance());
				renderer->setupStereo(eye_separation, stage.getFocalDistance());
#endif

				//delta *= stage.swapSideBySideStereo() ? -1. : 1.;

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
			render_mutex_.lock();
#ifdef USE_TBB
			boost::shared_ptr<tbb::task_scheduler_init> scheduler;

			if (!gl_renderer_)
			{
				const char* num_threads_s = getenv("BALL_NUM_RENDER_THREADS");

				Index num_threads = tbb::task_scheduler_init::automatic;
				if (num_threads_s != 0)
					num_threads = atoi(num_threads_s);

				printf("Using %d render threads\n", num_threads);
				scheduler = boost::shared_ptr<tbb::task_scheduler_init>(new tbb::task_scheduler_init(num_threads));
			}
#endif
			if (gl_target_)
				gl_target_->ignoreEvents(true);
			Timer t;

			Size current_frame = 0;
			double fps = 0.;
			// to be stopped from the outside, someone needs to call useContinuousLoop(false)
			render_mutex_.unlock();

			while (!about_to_quit_)
			{
				t.start();
				// NOTE: GLRenderers currently *have* to render in the GUI thread!
				if (!gl_renderer_)
					renderToBuffer_();
				t.stop();

				fps += t.getClockTime();

				//For GLRenderers this message is not useful and plain annoying
/*
				if (!gl_renderer_)
				{
					if (current_frame % 10 == 0)
					{
						fps = 10./fps;
						printf("########## Average rendering FPS = %.2lf  #########\n", fps);

						fps = 0.;
					}
				}
*/
				loop_mutex.lock();
				QApplication::instance()->postEvent(scene_, new RenderToBufferFinishedEvent(shared_from_this()));
				wait_for_render.wait(&loop_mutex);
				loop_mutex.unlock();
				t.reset();
				current_frame++;
			}
		}

		bool RenderSetup::isReadyToSwap()
		{
			bool result = buffer_is_ready_;

			for (std::deque<boost::shared_ptr<RenderSetup> >::iterator render_it  = keep_in_sync_.begin();
					result && (render_it != keep_in_sync_.end()); ++render_it)
			{
				result &= (*render_it)->bufferIsReady();
			}
		
			return result;	
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

			// we are working on a new buffer... so let people know...
			buffer_is_ready_ = false;

			// reduce the ttl
			if (ttl_ > 0) --ttl_;

			renderer->setPreviewMode(scene_->usePreview());
			renderer->showLightSources(scene_->showLightSourcesEnabled());

			updateCamera();

			if (RTTI::isKindOf<BufferedRenderer>(*renderer))
			{
				((BufferedRenderer*)renderer)->renderToBuffer(target, *stage_);
			} 
			else if (RTTI::isKindOf<GLRenderer>(*renderer))
			{
				GLRenderer* current_gl_renderer = static_cast<GLRenderer*>(renderer);
				// TODO: what do we do here? should we push the gl calls somewhere else, i.e. in the GUI thread?
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

			render_mutex_.unlock();

			if (ttl_ == 0 && export_after_ttl_)
				exportPNG(export_after_ttl_filename_);
		}

		bool RenderSetup::exportPNG(const String& filename)
		{
			// is the target a glRenderWindow?
			if (gl_target_)
			{
				// Yes => grab its frame buffer
				render_mutex_.lock();

				makeCurrent();
				QImage image(gl_target_->grabFrameBuffer(true));

				render_mutex_.unlock();

				bool ok = image.save(filename.c_str(), "PNG");

				return ok;
			}
			else
			{
				// let's see...

				// Nope. Let's try to export the buffer directly
				render_mutex_.lock();

				bool ok = false;

				FrameBufferPtr buffer = target->getBuffer();
				if (buffer->getFormat().getPixelFormat() == PixelFormat::RGBF_96)
				{
					// Unfortunately, we will have to convert the data pixel by pixel, since QImage does
					// not seem to support anything like RGBF_96

					float *data = (float*)(buffer->getData());

					Size width  = (Size)renderer->getWidth();
					Size height = (Size)renderer->getHeight();

					QImage image(width, height, QImage::Format_RGB888);

					Size line_width = width*3;

					QRgb value;
					for (Position current_line = 0; current_line < height; ++current_line)
					{
						for (Position current_col = 0; current_col < width; ++current_col)
						{
							Position current_offset = 3*(width*current_line + current_col);

							int red   = std::min((int)(data[current_offset  ]*255.), 255);
							int green = std::min((int)(data[current_offset+1]*255.), 255);
							int blue  = std::min((int)(data[current_offset+2]*255.), 255);

							value = qRgb(red, green, blue);

							image.setPixel(current_col, current_line, value);
						}
					}

					render_mutex_.unlock();

					ok = image.save(filename.c_str(), "PNG");
				} 
				else 
				{
					Log.error() << "RenderSetup::exportPNG(): Frame Buffer format not supported!" << std::endl;
				}

				target->releaseBuffer(buffer);

				return ok;
			}
		}

		void RenderSetup::bufferRepresentation(const Representation& rep)
		{
			if (receive_updates_)
			{
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
				render_mutex_.lock();

				makeCurrent();

				renderer->removeRepresentation(rep);

				render_mutex_.unlock();
			}
		}

		void RenderSetup::setLights(bool reset_all)
		{
			render_mutex_.lock();

			makeCurrent();
				
			renderer->setLights(reset_all);

			render_mutex_.unlock();
		}
		void RenderSetup::updateMaterialForRepresentation(const Representation* rep)
		{
			render_mutex_.lock();

			//makeCurrent();

			renderer->updateMaterialForRepresentation(rep);

			render_mutex_.unlock();
		}
		
		void RenderSetup::updateBackgroundColor()
		{
			render_mutex_.lock();

			makeCurrent();

			renderer->updateBackgroundColor();

			render_mutex_.unlock();
		}

		void RenderSetup::setupEnvironmentMap(const QImage& image)
		{
			render_mutex_.lock();

			makeCurrent();

			renderer->setupEnvironmentMap(image);

			render_mutex_.unlock();
		}

		Position RenderSetup::prepareGridTextures(const RegularData3D& grid, const ColorMap& map)
		{
			Position texname = 0;

			if (RTTI::isKindOf<GLRenderer>(*renderer))
			{
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
				MutexLocker ml(&render_mutex_);

				makeCurrent();

				((GLRenderer*)renderer)->removeTextureFor_(grid);
			}
		}

		Vector3 RenderSetup::mapViewportTo3D(Position x, Position y)
		{
			MutexLocker ml(&render_mutex_);
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
			MutexLocker ml(&render_mutex_);
			makeCurrent();

			renderer->pickObjects(x1, y1, x2, y2, objects);
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

		void RenderSetup::initType_()
		{
			// set the type variable
			if (RTTI::isKindOf<GLRenderer>(*renderer))
				renderer_type_ = OPENGL_RENDERER;
			else if (RTTI::isKindOf<POVRenderer>(*renderer))
				renderer_type_ = POV_RENDERER;
			else if (RTTI::isKindOf<VRMLRenderer>(*renderer))
				renderer_type_ = VRML_RENDERER;
			else if (RTTI::isKindOf<STLRenderer>(*renderer))
				renderer_type_ = STL_RENDERER;
			else if (RTTI::isKindOf<TilingRenderer>(*renderer))
				renderer_type_ = TILING_RENDERER;
#ifdef BALL_HAS_RTFACT
			else if (RTTI::isKindOf<RTfactRenderer>(*renderer))
				renderer_type_ = RTFACT_RENDERER;
#endif
			else
				renderer_type_ = UNKNOWN_RENDERER;
		}
	}
}
