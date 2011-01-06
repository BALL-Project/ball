// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/RENDERING/RENDERERS/tilingRenderer.h>

#include <BALL/VIEW/RENDERING/RENDERERS/glRenderer.h>
#include <BALL/VIEW/RENDERING/glRenderWindow.h>
#include <BALL/VIEW/RENDERING/glOffscreenTarget.h>

namespace BALL
{
	namespace VIEW
	{

		TilingRenderer::TilingRenderer(Renderer* real_renderer, Size final_width, Size final_height, Size border)
			: Renderer(),
				real_renderer_(real_renderer),
				final_width_(final_width),
				final_height_(final_height),
				border_(border)
		{
		}

		TilingRenderer::TilingRenderer(const TilingRenderer& renderer)
			: Renderer(renderer),
				real_renderer_(renderer.real_renderer_),
				final_width_(renderer.final_width_),
				final_height_(renderer.final_height_),
				border_(renderer.border_)
		{
		}

		void TilingRenderer::setLights(bool reset_all)
		{
			real_renderer_->setLights(reset_all);
		}

		void TilingRenderer::updateCamera(const Camera* camera)
		{
			real_renderer_->updateCamera(camera);
		}

		void TilingRenderer::updateBackgroundColor()
		{
			real_renderer_->updateBackgroundColor();
		}

		bool TilingRenderer::finish()
		{
			return true;
		}

		Vector3 TilingRenderer::mapViewportTo3D(Position x, Position y)
		{
			return real_renderer_->mapViewportTo3D(x, y);
		}

		Vector2 TilingRenderer::map3DToViewport(const Vector3& vec)
		{
			return real_renderer_->map3DToViewport(vec);
		}

		bool TilingRenderer::renderOneRepresentation(const Representation& representation)
		{
			return real_renderer_->renderOneRepresentation(representation);
		}

		void TilingRenderer::bufferRepresentation(const Representation& rep)
		{
			real_renderer_->bufferRepresentation(rep);
		}

		void TilingRenderer::removeRepresentation(const Representation& rep)
		{
			real_renderer_->removeRepresentation(rep);
		}

		void TilingRenderer::setSize(float width, float height)
		{
			Renderer::setSize(width, height);
			real_renderer_->setSize(width, height);

			computeTilingSetup_();
		}

		void TilingRenderer::renderRuler()
		{
			real_renderer_->renderRuler();
		}

		void TilingRenderer::render_(const GeometricObject* object)
		{
			real_renderer_->render_(object);
		}

#ifdef BALL_COMPILER_MSVC
// yes, it *is* *that* stupid...
#undef near
#undef far
#endif

		void TilingRenderer::renderToBuffer(RenderTarget* target)
		{
			if (RTTI::isKindOf<GLRenderer>(*real_renderer_))
			{
				GLRenderer* gl_renderer = static_cast<GLRenderer*>(real_renderer_);

				GLOffscreenTarget* off_target = static_cast<GLOffscreenTarget*>(target);

				if (off_target)
					off_target->prepareUpscaling(final_width_, final_height_);

				// store the original viewport of the rendering context
				GLint old_viewport[4];
				glGetIntegerv(GL_VIEWPORT, old_viewport);

				// some convenient shortcuts
				Size width_no_border  = width_  - 2*border_;
				Size height_no_border = height_ - 2*border_;

				Size current_x = 0;
				Size current_y = 0;

				// save old projection matrix
				GLint matrix_mode;
				glGetIntegerv(GL_MATRIX_MODE, &matrix_mode);
				glMatrixMode(GL_PROJECTION);
				glPushMatrix();
				glMatrixMode(matrix_mode);

				// iterate over all the tiles we have to render
				for (Size current_col = 0; current_col < num_cols_; ++current_col)
				{
					Size tile_width  = (current_col < num_cols_-1) ? width_ 
																												 : final_width_  - (num_cols_ - 1) * width_no_border + 2*border_;

					for (Size current_row = 0; current_row < num_rows_; ++current_row)
					{
						Size tile_height = (current_row < num_rows_-1) ? height_ 
							                                             : final_height_ - (num_rows_ - 1) * height_no_border + 2*border_;
						// set the viewport corresponding to the current tile dimensions (including borders)
						glViewport(0, 0, tile_width, tile_height);

						// save current matrix mode
						glGetIntegerv(GL_MATRIX_MODE, &matrix_mode);
						
						// move to projection matrix
						glMatrixMode(GL_PROJECTION);
						glLoadIdentity();

						float near, far, left, right, top, bottom;
						gl_renderer->getFrustum(near, far, left, right, top, bottom);

						// adapt frustum to current tile
						float new_left  = left     + (right - left) * (current_col * width_no_border - border_) / final_width_;
						float new_right = new_left + (right - left) * tile_width / final_width_;

						float new_bottom = bottom     + (top - bottom) * (current_row * height_no_border - border_) / final_height_;
						float new_top    = new_bottom + (top - bottom) * tile_height / final_height_;

						glFrustum(new_left, new_right, new_bottom, new_top, near, far);

						// restore old matrix mode
						glMatrixMode(matrix_mode);

						gl_renderer->updateCamera();
						gl_renderer->renderToBuffer(target, GLRenderer::DISPLAY_LISTS_RENDERING);
						glFlush();

						if (off_target)
							off_target->updateImageTile(current_x, current_y, current_x + tile_width - 1, current_y + tile_height - 1);
						current_y += tile_height;
					}
					current_x += tile_width;
					current_y = 0;
				}

				// restore old projection matrix
				glGetIntegerv(GL_MATRIX_MODE, &matrix_mode);
				glMatrixMode(GL_PROJECTION);
				glPopMatrix();
				glMatrixMode(matrix_mode);

				// restore the old viewport
				glViewport(old_viewport[0], old_viewport[1], old_viewport[2], old_viewport[3]);
			}
		}

		void TilingRenderer::computeTilingSetup_()
		{
			num_cols_ = (final_width_  + (width_  - 2*border_) - 1) / (width_  - 2*border_);
			num_rows_ = (final_height_ + (height_ - 2*border_) - 1) / (height_ - 2*border_);
		}

	}
}

