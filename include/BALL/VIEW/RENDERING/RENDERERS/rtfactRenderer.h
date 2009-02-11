// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_RENDERING_RTFACTRENDERER_H
#define BALL_VIEW_RENDERING_RTFACTRENDERER_H

#define BALLVIEW_RTFACTRENDERER_THROW(exceptionName, message) (throw BALL::Exception::##exceptionName##(__FILE__, __LINE__, message))
#include <BALL/VIEW/RENDERING/raytracingRenderer.h>

#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/VIEW/KERNEL/stage.h>
#include <BALL/VIEW/KERNEL/representationManager.h>
#include <BALL/VIEW/PRIMITIVES/mesh.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/MATHS/surface.h>
#include <BALL/DATATYPE/hashMap.h>

//RTRemote proxy to RTfact
#include <RTremote/Renderer.hpp>

using RTfact::Remote::GroupHandle;

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
						/// The group handles this object was assigned to
						std::vector<RTfact::Remote::GroupHandle> group_handle;
						
						/// The object handles
						std::vector<RTfact::Remote::GeoHandle> object_handles;

						/// The materials
						std::vector<RTfact::Remote::RTAppearanceHandle> material_handles;
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

				/************************************************************************/
				/* RaytracingRenderer methods					   */
				/************************************************************************/
				virtual bool init(const Scene& scene) throw();

				virtual String getRenderer()  
				{
					return "RTfact-RTRemote Ray Tracer";
				}

				virtual void formatUpdated()
				{
				}

				virtual void prepareBufferedRendering(const Stage& stage);
				virtual void renderToBufferImpl(FrameBufferPtr buffer);

				void bufferRepresentation(Representation const* rep);
				void updateMaterialForRepresentation(Representation const* rep);
				void updateMaterialForRepresentation(Representation const* rep, const Stage::RaytracingMaterial& new_material);

				GroupHandle transformTube(const TwoColoredTube& tube);
				void updateMaterialFromStage(RTfact::Remote::RTAppearanceHandle& material);
				void convertMaterial(Stage::RaytracingMaterial const& rt_material, RTfact::Remote::RTAppearanceHandle& material);

			private:

				std::vector<RTfact::Remote::RTLightHandle> lights_;

				Scene const* scene_;

				RTfact::Remote::Renderer m_renderer;

				HashMap<Representation const*, RTfactData> objects_;

				Surface sphere_template_;
				Surface tube_template_;

				template<typename taPixelType>
				void renderImpl(taPixelType* buffer, const unsigned int width, const unsigned int height, const unsigned int pitch);

				template<>
				void renderImpl(float* buffer, const unsigned int width, const unsigned int height, const unsigned int pitch)
				{
				}

				bool rtfact_needs_update_;
		};


	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_RENDERING_RTFACTRENDERER_H
