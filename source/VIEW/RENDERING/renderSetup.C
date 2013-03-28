#include <BALL/VIEW/RENDERING/renderSetup.h>

#include <BALL/VIEW/RENDERING/RENDERERS/renderer.h>

#include <BALL/VIEW/WIDGETS/scene.h>

#include <QtCore/QCoreApplication>

namespace BALL
{
	namespace VIEW
	{
		RenderSetup::RenderSetup(Renderer* r, RenderTarget* t, Scene* scene)
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
				render_mutex_(true),
				show_ruler_(false),
				ttl_(-1),
				export_after_ttl_(false),
				export_after_ttl_filename_(),
				buffer_is_ready_(true)
		{
		}

		RenderSetup::~RenderSetup()
		{
		}

		void RenderSetup::init()
		{
			render_mutex_.lock();

			// Yes, this seems inconsistent... but we need to start with a ready (albeit empty)
			// buffer to boot the system
			buffer_is_ready_ = true;

			makeCurrent();

			// initialize the rendering target
			target->init();

			// initialize the renderer
			if(!renderer->init(*scene_))
			{
				Log.error() << "Renderer failed to initialize" << std::endl;
				throw Exception::GeneralException(__FILE__, __LINE__);
			}			

			render_mutex_.unlock();
		}

		void RenderSetup::resize(Size width, Size height)
		{
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

			if(!renderer->setFrameBufferFormat(target->getFormat()))
			{
				Log.error() << "Raytracing render does not support window framebuffer format. Seems to be configuration error" << std::endl;

				render_mutex_.unlock();

				throw Exception::GeneralException(__FILE__, __LINE__);
			}

			render_mutex_.unlock();

			updateCamera();

			//renderToBuffer_();

			render_mutex_.lock();

			target->swapBuffers();

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
			target->prepareRendering();
		}

		void RenderSetup::run()
		{
			lock();

			startRunning_();

			// to be stopped from the outside, someone needs to call useContinuousLoop(false)
			unlock();

			while (!about_to_quit_)
			{
				doRender_();

				loop_mutex.lock();
				QCoreApplication::instance()->postEvent(scene_, new RenderToBufferFinishedEvent(shared_from_this()));
				wait_for_render.wait(&loop_mutex);
				loop_mutex.unlock();
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

		void RenderSetup::swapBuffers()
		{
			target->swapBuffers();
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

			renderer->renderToBuffer(target);

			if (show_ruler_)
				renderer->renderRuler();

			render_mutex_.unlock();

			if (ttl_ == 0 && export_after_ttl_)
				exportPNG(export_after_ttl_filename_);
		}

		bool RenderSetup::exportPNG(const String& filename)
		{

			// is the target a glRenderWindow?
//			if (gl_target_)
//			{
//				// Yes => grab its frame buffer
//				render_mutex_.lock();
//
//				makeCurrent();
//				QImage image(gl_target_->grabFrameBuffer(true));
//
//				render_mutex_.unlock();
//
//				bool ok = image.save(filename.c_str(), "PNG");
//
//				return ok;
//			}
//			else
//			{
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
//			}
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

			makeCurrent();

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

		const QString& RenderSetup::getRendererType() const
		{
			return renderer->getName();
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
			/*
            if (RTTI::isKindOf<GLRenderer>(renderer))
			{
				static_cast<GLRenderer*>(renderer)->initPerspective();
			}
			*/
		}
	}
}
