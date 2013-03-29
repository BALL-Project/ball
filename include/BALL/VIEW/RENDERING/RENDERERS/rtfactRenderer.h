// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_RENDERING_RENDERERS_RTFACTRENDERER_H
#define BALL_VIEW_RENDERING_RENDERERS_RTFACTRENDERER_H

#define BALLVIEW_RTFACTRENDERER_THROW(exceptionName, message) (throw BALL::Exception::##exceptionName##(__FILE__, __LINE__, message))
#include <BALL/VIEW/RENDERING/RENDERERS/raytracingRenderer.h>

#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/VIEW/KERNEL/stage.h>
#include <BALL/VIEW/KERNEL/representationManager.h>
#include <BALL/VIEW/PRIMITIVES/mesh.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/MATHS/surface.h>
#include <BALL/MATHS/vector3.h>
#include <BALL/DATATYPE/hashMap.h>

#include <RTpieCpp/RayTracerHandle.hpp>
#include <RTfact/Model/Image/BasicImage2D.hpp>
#include <RTfact/Model/Framebuffer/Image2DFramebuffer.hpp>
#include <RTfact/Utils/FPSMeter.hpp>

namespace boost
{
	template <>
	struct hash< RTpieCpp::MeshHandle >
	{
		public:
			union conv
			{
				size_t s;
				const void  *p;
			};

			size_t operator()(const RTpieCpp::MeshHandle& x ) const throw() {
				conv c;
				c.p=x.get();
				return c.s;
			}
	};

	template <>
	struct hash< RTpieCpp::InstanceHandle >
	{
		public:
			union conv
			{
				size_t s;
				const void  *p;
			};

			size_t operator()(const RTpieCpp::InstanceHandle& x ) const throw() {
				conv c;
				c.p=x.get();
				return c.s;
			}
	};
}

namespace BALL
{
  namespace VIEW
	{
		/** RTfactRenderer
			Provides ray tracing through RTfact-RTRemote
			\ingroup ViewRendering
			*/
		class BALL_VIEW_EXPORT RTfactRenderer
			: public RaytracingRenderer
		{
			public:

				/** This class encapsulates RTfact's data structures per Representation.
				 */
				class RTfactData
				{
					public:

						/// The object handles (all objects)
						std::vector<RTpieCpp::GeometryHandle> object_handles;

						/// The meshes
						std::vector<RTpieCpp::MeshHandle> mesh_handles;

						/// The instances (visible objects)
						std::vector<RTpieCpp::InstanceHandle> instance_handles;

						/// The materials
						std::vector<RTpieCpp::AppearanceHandle> material_handles;

						/// Cut-plane geometries
						std::vector<RTpieCpp::GeometryHandle> cutPlanes;

						/// Cut-plane meshes
						std::vector<RTpieCpp::MeshHandle> cutPlaneMeshes;

						/// Cut-plane instances
						std::vector<RTpieCpp::InstanceHandle> cutPlaneInstances;

						/// Cut-planes material
						RTpieCpp::AppearanceHandle cutPlaneShader;

						/// Mark previously disabled representations
						bool has_been_disabled;
				};

				/// Default Constructor.
				RTfactRenderer()
					: RaytracingRenderer(),
						rtfact_needs_update_(false)
				{
				}

				/// Destructor
				virtual ~RTfactRenderer()
				{
				}

				virtual bool hasFPScounter();

				virtual double getFPS();

				/************************************************************************/
				/* RaytracingRenderer methods					   */
				/************************************************************************/
				virtual bool init(Scene& scene);

				virtual String getRenderer()
				{
					return "RTfact-RTRemote Ray Tracer";
				}

				virtual void formatUpdated()
				{
				}

				virtual GeometricObject* pickObject(Position x, Position y);
				virtual void pickObjects(Position x1, Position y1, Position x2, Position y2,
				                         std::list<GeometricObject*>& objects);



				virtual void setSize(float width, float height);

				virtual void setupStereo(float eye_separation, float focal_length);

				virtual void getFrustum(float& near_f, float& far_f, float& left_f, float& right_f, float& top_f, float& bottom_f);
				virtual void setFrustum(float near_f, float far_f, float left_f, float right_f, float top_f, float bottom_f);

				virtual void prepareBufferedRendering(const Stage& stage);
				virtual void renderToBufferImpl(FrameBufferPtr buffer);

				virtual void useContinuousLoop(bool use_loop);

				void bufferRepresentation(const Representation& rep);
				void bufferRepresentationDynamic(const Representation& rep);
				void removeRepresentation(const Representation& rep);

				void setLights(bool reset_all = false);

				void updateCamera(const Camera* camera = 0);

				void updateBackgroundColor();

				void setupEnvironmentMap(const QImage& image);

				void updateMaterialForRepresentation(Representation const* rep);

				void transformTube(const TwoColoredTube& tube, float *trafo);
				void transformLine(const TwoColoredLine& line, float *trafo);

				void updateMaterialFromStage(RTpieCpp::AppearanceHandle& material);
				void convertMaterial(Stage::Material const& rt_material, RTpieCpp::AppearanceHandle& material);

				/** Raytracing-related functionality **/
				//@{
				/** Intersect a set of rays with the geometry buffered by this renderer.
				 *
				 *  This function will intersect the rays
				 *
				 *     origins[i] + l * directions[i]
				 *
				 *  with the geometry that has been buffered by this renderer previously.
				 */
                virtual std::vector<float> intersectRaysWithGeometry(const std::vector<Vector3>& origins, const std::vector<Vector3>& directions);

                //@}

                private:

                static const float vectorDifferenceTolerance_;

                bool doVectorsDiffer(const Vector3& vecA, const Vector3& vecB);

                template<typename taPixelType>
                void renderImpl(taPixelType* buffer, const unsigned int width, const unsigned int height, const unsigned int pitch);

                void renderImpl(float* buffer, const unsigned int width, const unsigned int height, const unsigned int pitch)
                {
                }

                std::vector<RTpieCpp::LightHandle>                      lights_;

                typedef RTfact::BasicImage2D<float>                     t_ColorImage;
                typedef RTfact::BasicImage2D<float>                     t_DistanceImage;
                typedef RTfact::Image2DFramebuffer<
                                                t_ColorImage,
                                                t_DistanceImage>        t_Framebuffer;

                typedef RTfact::BasicImage2D<unsigned char>             t_ByteColorImage;
                typedef RTfact::BasicImage2D<unsigned char>             t_ByteDistanceImage;
                typedef RTfact::Image2DFramebuffer<
                                                t_ByteColorImage,
                                                t_ByteDistanceImage>    t_ByteFramebuffer;

                RTpieCpp::SceneHandle                                   sceneHandle;
                RTpieCpp::RayTracerHandle                               rayTracer;
                RTpieCpp::CameraHandle                                  cameraHandle;
                RTpieCpp::FrameBufferHandle                             renderBuffer;
                RTpieCpp::RenderTaskHandle                              renderTask;
                t_Framebuffer                                           framebuffer;
                t_ByteFramebuffer                                       byteFramebuffer;


                RTfact::FPSMeter                                        fpsMeter;
                RTpieCpp::PickTaskHandle                                pickTask;

                HashMap<Representation const*, RTfactData>              objects_;
                HashMap<RTpieCpp::InstanceHandle, GeometricObject*>     geometric_objects_inst;
                HashMap<RTpieCpp::MeshHandle, GeometricObject*>         geometric_objects_;

                Surface                                                 sphere_template_;
                Surface                                                 tube_template_;

                bool                                                    rtfact_needs_update_;

                Vector3                                                 last_camera_position;
                Vector3                                                 last_camera_view_vec;
                Vector3                                                 last_camera_lookup;

                float                                                   x_scale_;
                float                                                   y_scale_;

        };

    } // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_RENDERING_RTFACTRENDERER_H
