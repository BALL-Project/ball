#include <BALL/VIEW/RENDERING/renderSetup.h>

namespace BALL
{
	namespace VIEW
	{
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

			render_mutex_.unlock();

			return *this;
		}

		void RenderSetup::init()
		{
			render_mutex_.lock();

			target->makeCurrent();

			// initialize the rendering target
			target->init();

			// initialize the renderer
			if(!renderer->init(*scene_))
			{
				Log.error() << "Renderer failed to initialize" << endl;
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
			if (target->isFullScreen())
				return;

			render_mutex_.lock();

			target->makeCurrent();

			if(!target->resize(width, height))
			{
				Log.error() << "Cannot resize window. Size " 
										<< width  << " x " 
										<< height << " is not supported" << endl;
			}

			if (RTTI::isKindOf<BufferedRenderer>(*renderer))
			{
				if(!(((BufferedRenderer*)renderer)->setFrameBufferFormat(scene_->getFormat())))
				{
					Log.error() << "Raytracing render does not support window framebuffer format. Seems to be configuration error" << endl;

					render_mutex_.unlock();

					throw Exception::GeneralException(__FILE__, __LINE__);
				}
			}

			if (RTTI::isKindOf<GLRenderer>(*renderer))
				((GLRenderer*)renderer)->setSize(width, height);

			// do *not* move this unlock call below updateCamera, or we will deadlock!
			render_mutex_.unlock();

			updateCamera();
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

				camera_offset_  = Vector3(eye_separation, 5., 0.);

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

		void RenderSetup::run()
		{
			useContinuousLoop(true);

			// to be stopped from the outside, someone needs to call useContinuousLoop(false)
			while (use_continuous_loop_)
				renderToBuffer_();
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

			target->makeCurrent();

			renderer->setPreviewMode(scene_->use_preview_ && scene_->preview_);
			renderer->showLightSources(scene_->show_light_sources_);

			// We need to find out whether a recursive lock would be faster...
			render_mutex_.unlock();
			updateCamera();
			render_mutex_.lock();

			if (RTTI::isKindOf<BufferedRenderer>(*renderer))
			{
				((BufferedRenderer*)renderer)->renderToBuffer(target, *stage_);
				target->refresh();

				// TODO: render coordinate systems!
			}

			if (RTTI::isKindOf<GLRenderer>(*renderer))
			{
				GLRenderer* current_gl_renderer = (GLRenderer*)renderer;

				// cannot call update here, because it calls updateGL
				current_gl_renderer->renderToBuffer(target, GLRenderer::DISPLAY_LISTS_RENDERING);

				glFlush();

				// todo: does this work correctly???
				//scene_->renderGrid_();

				// make sure that the scene does not receive this event again
				if (target != scene_)
					target->updateGL();
			}

			render_mutex_.unlock();
		}

		void RenderSetup::bufferRepresentation(const Representation& rep)
		{
			if (receive_updates_)
			{
				render_mutex_.lock();

				target->makeCurrent();
				renderer->bufferRepresentation(rep);

				render_mutex_.unlock();
			}
		}

		void RenderSetup::removeRepresentation(const Representation& rep)
		{
			if (receive_updates_)
			{
				render_mutex_.lock();

				target->makeCurrent();
				renderer->removeRepresentation(rep);

				render_mutex_.unlock();
			}
		}

		void RenderSetup::setLights(bool reset_all)
		{
			render_mutex_.lock();

			target->makeCurrent();
			renderer->setLights(reset_all);

			render_mutex_.unlock();
		}

		void RenderSetup::updateBackgroundColor()
		{
			render_mutex_.lock();

			target->makeCurrent();
			renderer->updateBackgroundColor();

			render_mutex_.unlock();
		}

		Position RenderSetup::prepareGridTextures(const RegularData3D& grid, const ColorMap& map)
		{
			Position texname;

			if (RTTI::isKindOf<GLRenderer>(*renderer))
			{
				render_mutex_.lock();

				target->makeCurrent();
				texname = ((GLRenderer*)renderer)->createTextureFromGrid(grid, map);

				render_mutex_.unlock();
			}

			return texname;
		}

		void RenderSetup::removeGridTextures(const RegularData3D& grid)
		{
			if (RTTI::isKindOf<GLRenderer>(*renderer))
			{
				render_mutex_.lock();

				target->makeCurrent();
				((GLRenderer*)renderer)->removeTextureFor_(grid);

				render_mutex_.unlock();
			}
		}
	}
}
