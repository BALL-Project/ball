// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/RENDERING/tilingRenderer.h>

#include <BALL/VIEW/RENDERING/glRenderer.h>
#include <BALL/VIEW/RENDERING/glRenderWindow.h>

namespace BALL
{
	namespace VIEW
	{
		TilingRenderer::TilingRenderer(Renderer* real_renderer)
			: Renderer(),
				real_renderer_(real_renderer)
		{
		}

		TilingRenderer::TilingRenderer(const TilingRenderer& renderer)
			: Renderer(renderer),
				real_renderer_(renderer.real_renderer_)
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
			width_  = width;
			height_ = height; 
		}

		void TilingRenderer::renderRuler()
		{
			real_renderer_->renderRuler();
		}

		void TilingRenderer::render_(const GeometricObject* object)
		{
			real_renderer_->render_(object);
		}

		void TilingRenderer::renderToBuffer(RenderTarget* target)
		{
			if (RTTI::isKindOf<GLRenderer>(*real_renderer_))
			{
				static_cast<GLRenderer*>(real_renderer_)->renderToBuffer(target, GLRenderer::DISPLAY_LISTS_RENDERING);
			}
		}
	}
}

