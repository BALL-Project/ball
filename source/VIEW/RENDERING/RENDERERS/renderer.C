/* TRANSLATOR BALL::VIEW::Renderer

		Necessary for lupdate.
*/

// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/RENDERING/RENDERERS/renderer.h>
#include <BALL/VIEW/RENDERING/renderSetup.h>
#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/VIEW/KERNEL/stage.h>

#include <BALL/VIEW/PRIMITIVES/label.h>
#include <BALL/VIEW/PRIMITIVES/line.h>
#include <BALL/VIEW/PRIMITIVES/multiLine.h>
#include <BALL/VIEW/PRIMITIVES/mesh.h>
#include <BALL/VIEW/PRIMITIVES/point.h>
#include <BALL/VIEW/PRIMITIVES/box.h>
#include <BALL/VIEW/PRIMITIVES/simpleBox.h>
#include <BALL/VIEW/PRIMITIVES/sphere.h>
#include <BALL/VIEW/PRIMITIVES/disc.h>
#include <BALL/VIEW/PRIMITIVES/tube.h>
#include <BALL/VIEW/PRIMITIVES/twoColoredLine.h>
#include <BALL/VIEW/PRIMITIVES/twoColoredTube.h>
#include <BALL/VIEW/PRIMITIVES/mesh.h>
#include <BALL/VIEW/PRIMITIVES/quadMesh.h>
#include <BALL/VIEW/PRIMITIVES/gridVisualisation.h>

namespace BALL
{
	namespace VIEW
	{

		Renderer::Renderer(const QString& name)
			: offset_(0),
			  stride_(0),
			  scene_(0),
			  stage_(0),
			  width_(0),
			  height_(0),
			  show_preview_(false),
			  volume_width_(0.),
			  show_light_sources_(false),
			  use_continuous_loop_(false),
			  stereo_frustum_conversion_width_(1),
			  stereo_frustum_conversion_height_(1),
			  stereo_(NO_STEREO),
			  name_(name)
		{
			stage_ = &RTTI::getDefault<Stage>();
		}

		bool Renderer::setFrameBufferFormat(const FrameBufferFormat& format)
		{
			if (supports(format))
			{
				bufferFormat = format;
				formatUpdated();

				return true;
			}

			return false;
		}

		const FrameBufferFormat& Renderer::getFrameBufferFormat() const
		{
			return bufferFormat;
		}

		void Renderer::renderToBuffer(RenderTarget* renderTarget)
		{
			FrameBufferPtr buffer = renderTarget->getBuffer();
			assert(buffer);

			if (buffer->getFormat() != bufferFormat)
			{
				throw Exception::InvalidFormat(__FILE__, __LINE__, "Buffer with wrong framebuffer format supplied");
			}

			renderToBufferImpl( buffer );

			renderTarget->releaseBuffer(buffer);
		}

		bool Renderer::hasStage() const
		{ 
			return stage_ != &RTTI::getDefault<Stage>();
		}

		bool Renderer::init(Scene& scene)
		{
			scene_ = &scene;

			Stage* stage = scene.getStage();
			if (stage == 0)
			{
				init(Stage(), scene.width(), scene.height());
			}
			else
			{
				init(*stage, scene.width(), scene.height());
			}

			return true;
		}

		bool Renderer::init(const Stage& stage, float width, float  height)
		{
			stage_ = &stage;
			width_ = width;
			height_ = height;
			return true;
		}

		void Renderer::setLights(bool /*reset_all*/)
		{
			Log.error() << (String)(qApp->translate("BALL::VIEW::Renderer", 
						          "Renderer::setLights() not implemented for this kind of renderer yet!")) << std::endl;
		}

		void Renderer::updateCamera(const Camera* /*camera*/)
		{
			Log.error() << (String)(qApp->translate("BALL::VIEW::Renderer", 
						          "Renderer::updateCamera() not implemented for this kind of renderer yet!")) << std::endl;
		}

		void Renderer::updateBackgroundColor()
		{
			Log.error() << (String)(qApp->translate("BALL::VIEW::Renderer", 
						         "Renderer::updateBackgroundColor() not implemented for this kind of renderer yet!")) << std::endl;
		}

		void Renderer::setupEnvironmentMap(const QImage& image)
		{
			Log.error() << "Renderer::setupEnvironmentMap() not implemented for this kind of renderer yet!" << std::endl;
		}

		void Renderer::setupStereo(float /*eye_separation*/, float /*focal_length*/)
		{
			Log.error() << (String)(qApp->translate("BALL::VIEW::Renderer", 
						         "Renderer::setupStereo() not implemented for this kind of renderer yet!")) << std::endl;
		}

		Resolution Renderer::getSupportedResolution(
			const Resolution &min, const Resolution &max,
			const PixelFormat &format) const
		{
			if(!supports(format))
			{
                throw BALL::Exception::FormatUnsupported(__FILE__, __LINE__);
			}
			return max;
		}

		bool Renderer::supports(const FrameBufferFormat &format) const
		{
			return supports(format.getPixelFormat());
		}

		boost::shared_ptr<RenderSetup> Renderer::createRenderSetup(RenderTarget* target, Scene* scene)
		{
			return boost::shared_ptr<RenderSetup>(new RenderSetup(this, target, scene));
		}

		std::vector<float> Renderer::intersectRaysWithGeometry(const std::vector<Vector3>& origins,
		                                                       const std::vector<Vector3>& directions)
		{
			Log.error() << "Sorry, intersectRaysWithGeometry() not implemented for this kind of renderer!" << std::endl;

			return std::vector<float>();
		}

		// Convert 2D screen coordinate to 3D coordinate on the view plane
		// TODO: allow orthographic projection as well
		Vector3 Renderer::mapViewportTo3D(Position x, Position y)
		{
			// the mapping works as follows:
			//   - all points are mapped to the view plane, defined by the view 
			//     vector and the right/up vectors of the camera
			//   - the width of the view plane in world coordinates can be computed
			//     using elementary geometry from the intercept theorem as
			//     (right - left) * distance_camera_view_plane / distance_camera_near_plane
			//   - the look_at_vector of the camera points at the center of the view plane
			// 	   this center should be mapped to (width()/2, height()/2)

			// get the frustum
			// NOTE: please don't rename these to near, far, ..., since MSVC defines these variables... *sigh*
			float near_f, far_f, left_f, right_f, top_f, bottom_f;
			getFrustum(near_f, far_f, left_f, right_f, top_f, bottom_f);

			const Camera&  camera  = stage_->getCamera();

			const Vector3& view	 = camera.getViewVector();
			const Vector3& right = camera.getRightVector();
			const Vector3& up    = camera.getLookUpVector();

			float distance_camera_view_plane = view.getLength();

			float scale_right_vector = (distance_camera_view_plane / near_f) * (right_f - left_f  )/2.;
			float scale_up_vector    = (distance_camera_view_plane / near_f) * (top_f   - bottom_f)/2.;

			Vector3 result = 		camera.getLookAtPosition()
												+ right   * (2.*(float)x/width_  - 1.) * scale_right_vector
												- up      * (2.*(float)y/height_ - 1.) * scale_up_vector;

			return result;
		}	

		// TODO: allow orthographic projection as well
		Vector2 Renderer::map3DToViewport(const Vector3& vec)
		{
			// get the frustum
			// NOTE: please don't rename these to near, far, ..., since MSVC defines these variables... *sigh*
			float near_f, far_f, left_f, right_f, top_f, bottom_f;
			getFrustum(near_f, far_f, left_f, right_f, top_f, bottom_f);

			const Camera&  camera  = stage_->getCamera();

			const Vector3& right   = camera.getRightVector();
			const Vector3& up      = camera.getLookUpVector();
			
			Vector3 point = vec - camera.getViewPoint();

			Vector3 normalized_view = camera.getViewVector();
			if (normalized_view.getLength() > 0)
				normalized_view.normalize();
			else
				normalized_view = Vector3(0.,0.,-1.);

			Vector2 result(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());

			float projection_on_view = normalized_view * point;
			if (projection_on_view <= 0)
				return result;

			Vector3 point_on_near = near_f / projection_on_view * point;

			// project point on plane
			point_on_near -= near_f * normalized_view;

			float near_width = right_f - left_f;
			result.x = width_ / near_width * ((point_on_near * right) + near_width/2.);

			float near_height = top_f - bottom_f;
			result.y = height_ / near_height * ((point_on_near * (-up)) + near_height/2.);

			return result;
		}

		void Renderer::pickObjects(Position x1, Position y1, Position x2, Position y2, 
			                         std::list<GeometricObject*>& objects)
		{
			Log.error() << "Renderer::pickObjects() not implemented for this kind of renderer yet!" << std::endl;
		}

		void Renderer::getFrustum(float& near_f, float& far_f, float& left_f, float& right_f, float& top_f, float& bottom_f)
		{
			Log.error() << "Renderer::getFrustum() not implemented for this kind of renderer yet!" << std::endl;
		}

		void Renderer::renderRuler()
		{
			Log.error() << (String)(qApp->translate("BALL::VIEW::Renderer", 
						         "Renderer::renderRuler() not implemented for this kind of renderer yet!")) << std::endl;
		}

		bool Renderer::finish()
		{
			return true;
		}

		Renderer::StereoMode Renderer::getStereoMode() const
		{
			return stereo_;
		}

		void Renderer::setStereoMode(Renderer::StereoMode state)
		{
			stereo_ = state;
		}

		void Renderer::setStereoFrustumConversion(int width_factor, int height_factor)
		{
			stereo_frustum_conversion_width_  = width_factor;
			stereo_frustum_conversion_height_ = height_factor;
		}


} } // namespaces
