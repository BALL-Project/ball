// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:

#include <BALL/VIEW/RENDERING/raytracingRenderer.h>
#include <BALL/VIEW/RENDERING/renderer.h>
#include <BALL/VIEW/KERNEL/stage.h>
#include <BALL/VIEW/DATATYPE/colorMap.h>
#include <BALL/VIEW/WIDGETS/scene.h>

namespace BALL
{
	namespace VIEW
	{
	
		RaytracingRenderer::RaytracingRenderer()
			throw()
			: Renderer(),
				scene_(0)
		{

		}

		RaytracingRenderer::~RaytracingRenderer()
			throw()
		{
			clear();
		}
		
		void RaytracingRenderer::clear()
			throw()
		{
		}
		
		void RaytracingRenderer::dump(std::ostream& s, Size depth) const
			throw()
		{
			BALL_DUMP_STREAM_PREFIX(s);

			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BALL_DUMP_DEPTH(s, depth);
			s << "Drawing Precision: " 	<< drawing_precision_ 	<< std::endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "Drawing Mode: " 		 	<< drawing_mode_  << std::endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "Width: " << width_ << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "Height: " << height_ << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "XScale: " << x_scale_ << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "YScale: " << y_scale_ << endl;

			BALL_DUMP_STREAM_SUFFIX(s);     
		}

		bool RaytracingRenderer::init(Scene& scene)
			throw()
		{
			scene_ = &scene;

			Stage* stage = scene.getStage();
			if (stage == 0)
			{
				// I am not sure, if heigth and width are in the correct order... copied this from glRenderer.C
				Renderer::init(Stage(), scene.width(), scene.height());

			}
			else
			{
				// I am not sure, if heigth and width are in the correct order... copied this from glRenderer.C
				Renderer::init(*stage, scene.width(), scene.height());

			}

			return true;
		}
		

		void RaytracingRenderer::setSize(float width, float height)
			throw()
		{
			width_ 	= width;
			height_ = height;

			if (width > height)
			{
				x_scale_ = width / (height * 2);
				y_scale_ = 0.5;
			}
			else
			{
				x_scale_ = 0.5;
				y_scale_ = height / (width * 2);
			}

			// Lukas: is there an equivalent to 
			// 	glViewport(0, 0, (int)width_, (int)height_);
			// ?
		}
		
/*		float  RaytracingRenderer::getXScale() const
			throw()
		{
			return 	x_scale_;
		}

		float  RaytracingRenderer::getYScale() const
			throw();
		{
			return 	y_scale_;
		} */
	
		void RaytracingRenderer::updateCamera(const Camera* camera)
			throw()
		{
			if (camera == 0) camera = &stage_->getCamera();

			if (Maths::isZero(camera->getViewVector().getSquareLength()))
			{
				Log.error() << "Unvalid camera settings: View point = LookAt point" << std::endl;
				return;
			}

			/*Lukas: is there something else to do?? */
			
			normal_vector_ = (-camera->getViewVector().normalize());
		}

		
		void RaytracingRenderer::updateBackgroundColor() 
			throw()
		{
			/* Lukas: something similar to 
			glClearColor((float) stage_->getBackgroundColor().getRed(),
					(float) stage_->getBackgroundColor().getGreen(),
					(float) stage_->getBackgroundColor().getBlue(),
					(float) stage_->getBackgroundColor().getAlpha()); */
		} 
		
		void  RaytracingRenderer::removeRepresentation(const Representation& rep)
			throw()
		{
		}

		void  RaytracingRenderer::addRepresentation(const Representation& rep)
				throw()
		{
		}

/*		RenderMode RaytracingRenderer::getRenderMode() const
				throw()
		{
			return render_mode_;
		}
		*/
			
		bool RaytracingRenderer::render(const Representation& representation)
			throw()
		{
			if (representation.isHidden()) return true;

			if (!representation.isValid())
			{
				BALLVIEW_DEBUG;
				representation.dump(std::cout, 0);
				return false;
			}

			drawing_precision_  = representation.getDrawingPrecision();
			drawing_mode_ 		  = representation.getDrawingMode();

			/* Lukas: perhaps we can consider different drawing modes, 
			 * transparency..
			 if (representation.getDrawingMode() == DRAWING_MODE_DOTS)
			 {
			 }
			 else
			 {
			 }

			 if (representation.hasProperty(Representation::PROPERTY__ALWAYS_FRONT))
			 {
			 }
			 else if (representation.getTransparency() == 0)
			 {
			 }
			 else
			 {
			 }
			 */
			return true;
		}

		String  RaytracingRenderer::getRenderer()
			throw()
		{
			return (char*)"";	
		}

		void RaytracingRenderer::initPerspective()
		{
		}
		
		void RaytracingRenderer::raytraceRepresentation_(const Representation& representation)
			throw()
		{
		}

		void RaytracingRenderer::setColorRGBA_(const ColorRGBA& color)
			throw()
		{
		}
			
		void RaytracingRenderer::generateIlluminationTexture_(float ka, float kd, float kr, float shininess)
		{
		}

		inline Position RaytracingRenderer::getTextureIndex_(Position x, Position y, Position z, Size width, Size height)
		{
		}
		
		Position RaytracingRenderer::createTextureFromGrid(const RegularData3D& grid, const ColorMap& map)
		{

		}
		
		void RaytracingRenderer::removeTextureFor_(const RegularData3D& grid)
		{
		}
			

	}
}
