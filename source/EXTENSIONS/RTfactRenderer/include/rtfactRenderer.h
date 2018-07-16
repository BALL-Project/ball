#ifndef BALL_RTFACTRENDERER_RTFACTRENDERER_H
#define BALL_RTFACTRENDERER_RTFACTRENDERER_H

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

#include <BALL/CONFIG/config.h>

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
	namespace RTfactRenderer
	{
		/** RTfactRenderer
			Provides ray tracing through RTfact-RTRemote
			\ingroup ViewRendering
			*/
		class BALL_VIEW_EXPORT RTfactRenderer
			: public BALL::VIEW::Renderer
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

				RTfactRenderer();

				/// Destructor
				~RTfactRenderer() noexcept override = default;

				bool hasFPScounter() override;

				double getFPS() override;

				/************************************************************************/
				/* RaytracingRenderer methods					   */
				/************************************************************************/
				bool init(BALL::VIEW::Scene& scene) override;

				virtual String getRenderer()
				{
					return "RTfact-RTRemote Ray Tracer";
				}

				void formatUpdated() override
				{
				}

				bool supports(const BALL::VIEW::PixelFormat&) const override;

				boost::shared_ptr<BALL::VIEW::RenderSetup> createRenderSetup(BALL::VIEW::RenderTarget* target,
					BALL::VIEW::Scene* scene) override;

				virtual BALL::VIEW::GeometricObject* pickObject(Position x, Position y);
				void pickObjects(Position x1, Position y1, Position x2, Position y2,
					std::list<BALL::VIEW::GeometricObject*>& objects) override;

				void setSize(float width, float height) override;

				void setupStereo(float eye_separation, float focal_length) override;

				void getFrustum(float& near_f, float& far_f, float& left_f, float& right_f, float& top_f, float& bottom_f) override;
				virtual void setFrustum(float near_f, float far_f, float left_f, float right_f, float top_f, float bottom_f);

				void renderToBufferImpl(BALL::VIEW::FrameBufferPtr buffer) override;

				void useContinuousLoop(bool use_loop) override;

				void bufferRepresentation(const BALL::VIEW::Representation& rep) override;
				virtual void bufferRepresentationDynamic(const BALL::VIEW::Representation& rep);
				void removeRepresentation(const BALL::VIEW::Representation& rep) override;

				void setLights(bool reset_all = false) override;

				void updateCamera(const BALL::VIEW::Camera* camera = 0) override;

				void updateBackgroundColor() override;

				void setupEnvironmentMap(const QImage& image) override;

				void updateMaterialForRepresentation(BALL::VIEW::Representation const* rep) override;

				virtual void transformTube(const BALL::VIEW::TwoColoredTube& tube, float *trafo);
				virtual void transformLine(const BALL::VIEW::TwoColoredLine& line, float *trafo);

				virtual void updateMaterialFromStage(RTpieCpp::AppearanceHandle& material);
				virtual void convertMaterial(BALL::VIEW::Stage::Material const& rt_material, RTpieCpp::AppearanceHandle& material);

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
				std::vector<float> intersectRaysWithGeometry(const std::vector<Vector3>& origins,
					const std::vector<Vector3>& directions) override;

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

                HashMap<BALL::VIEW::Representation const*, RTfactData>          objects_;
                HashMap<RTpieCpp::InstanceHandle, BALL::VIEW::GeometricObject*> geometric_objects_inst;
                HashMap<RTpieCpp::MeshHandle, BALL::VIEW::GeometricObject*>     geometric_objects_;

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
} // namespace RTfactRenderer

#endif // BALL_RTFACTRENDERER_RTFACTRENDERER_H
