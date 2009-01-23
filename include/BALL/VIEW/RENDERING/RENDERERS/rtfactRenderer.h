// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_RENDERING_RTFACTRENDERER_H
#define BALL_VIEW_RENDERING_RTFACTRENDERER_H

#define BALLVIEW_RTFACTRENDERER_THROW(exceptionName, message) (throw BALL::Exception::##exceptionName##(__FILE__, __LINE__, message))
#include <BALL/VIEW/RENDERING/raytracingRenderer.h>

//RTRemote proxy to RTfact
#include <RTremote/Renderer.hpp>

#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/VIEW/KERNEL/stage.h>
#include <BALL/VIEW/KERNEL/representationManager.h>
#include <BALL/VIEW/PRIMITIVES/mesh.h>
#include <BALL/VIEW/KERNEL/mainControl.h>

#include <set>

using namespace RTfact::Remote;

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

				/// Default Constructor.
				RTfactRenderer()
				{

				}

				/// Destructor
				virtual ~RTfactRenderer()
				{

				}

				/************************************************************************/
				/* RaytracingRenderer methods					   */
				/************************************************************************/
				virtual bool init(const Scene& scene)
					throw()
					{		     
						scene_ = &scene;

						using RTfact::Triangle;
						using RTfact::Vec3f;
						using RTfact::Remote::GroupHandle;
						using RTfact::Remote::GeoHandle;
						using RTfact::Transform;
						using RTfact::Remote::float3;

                GroupHandle root = m_renderer.getRoot();
                TriangleVector faces;                               
                Triangle t(Vec3f<1>(0,0,0),Vec3f<1>(1,0,0),Vec3f<1>(1,1,0));
                t.texCoords[0] = std::make_pair(0.f,0.f);
                t.texCoords[1] = std::make_pair(0.f,512.f); 
                t.texCoords[2] = std::make_pair(512.f,0.f);
                faces.push_back(t);                

                for(TriangleVector::iterator it = faces.begin(); it != faces.end(); it++)
                {
                    it->normals[0]=it->normal;
                    it->normals[1]=it->normal;
                    it->normals[2]=it->normal;
                }        
                
                GeoHandle geo1 = m_renderer.createGeometry(faces);                
                GroupHandle topGroup = m_renderer.createGroup(Transform::identity());
                topGroup->add(geo1);                

                root->add(topGroup);

                RTfact::Remote::RTAppearanceHandle mat1 = m_renderer.createAppearance("PhongShader");                
                mat1->setParam("diffuseColor", float3(1.0, 1.0, 0));
                mat1->setParam("ambientIntensity", float3(.7, .7, .7));
                
                geo1->setAppearance(mat1);                                

                m_renderer.setCameraPosition(
                    float3(-1.48978, 4.14466, 11.4965),
                    float3(0.864694, -0.481748, 0.142207),
                    float3(0, 1, 0));            

                //root->setTransform(Transform::rotationY(0.2f));
                //root->add(geo2);
                m_renderer.createAccelStruct();


						return true;
					}

				virtual String getRenderer()  
					throw()
					{
						return "RTfact-RTRemote Ray Tracer";
					}

				virtual void formatUpdated()
				{

				}

				virtual void prepareBufferedRendering(const Stage& stage)
				{
					Camera  const& camera = stage.getCamera();
					Vector3 const& position = camera.getViewPoint();
					Vector3 const& view_vector = camera.getViewVector();
					Vector3 const& look_up = camera.getLookUpVector();

					m_renderer.setCameraPosition(float3(position.x, position.y, position.z),
																			 float3(view_vector.x, view_vector.y, view_vector.z),
																			 float3(look_up.x, look_up.y, look_up.z));

					Vector3  light_pos = position - camera.getRightVector()*20;

					Size num_lights = lights_.size();

					Vector3 direction, light_position;
					Size current_light=0;
					List<LightSource>::ConstIterator it = stage.getLightSources().begin();
					for (; it != stage.getLightSources().end(); ++it, ++current_light)
					{
						if (current_light >= num_lights)
						{
							RTfact::Remote::RTLightHandle light; 
							switch (it->getType())
							{
								case LightSource::DIRECTIONAL:
									light = m_renderer.createLight("DirectionalLight"); 
									break;
								case LightSource::POSITIONAL:
									light = m_renderer.createLight("PointLight"); 
									break;
								default:
									std::cerr << "Light source type not supported!" << std::endl;
									break;
							}
							lights_.push_back(light);
						}
						switch (it->getType())
						{
							case LightSource::DIRECTIONAL:
								direction = it->getDirection();
								if (it->isRelativeToCamera())
									direction = stage.calculateAbsoluteCoordinates(direction);

								lights_[current_light]->setParam("direction", float3(direction.x, direction.y, direction.z));
								break;
							case LightSource::POSITIONAL:
								light_position = it->getPosition();
								if (it->isRelativeToCamera())
								{
									light_position = stage.calculateAbsoluteCoordinates(it->getPosition()+stage.getCamera().getViewPoint());
								}
								lights_[current_light]->setParam("position", float3(light_position.x, light_position.y, light_position.z));
								break;
							default:
								std::cerr << "Light source type not supported!" << std::endl;
								break;
						}
						float intensity = it->getIntensity()*500;
						ColorRGBA const& color = it->getColor();

						lights_[current_light]->setParam("intensity", float3((float)color.getRed()*intensity,(float)color.getGreen()*intensity,(float)color.getBlue()*intensity));
					}
				}

				virtual void renderToBufferImpl(FrameBufferPtr buffer);
			private:

				std::vector<RTfact::Remote::RTLightHandle> lights_;

				Scene const* scene_;

				RTfact::Remote::Renderer m_renderer;

				std::set<GeometricObject const*> objects_;

				template<typename taPixelType>
					void renderImpl(taPixelType* buffer, const unsigned int width, const unsigned int height, const unsigned int pitch);

				template<>
					void renderImpl(float* buffer, const unsigned int width, const unsigned int height, const unsigned int pitch)
					{
						//m_renderer.attachFrameBuffer(width, height, pitch, buffer);			 
						//m_renderer.renderToBuffer();		   


						for (unsigned int j = 0; j < height; j++)
						{
							for (unsigned int i = 0; i < width; i++)
							{			     
								if ( (((i / 64) % 2) == 0) && (((j / 64) % 2) == 0))
								{							 
									buffer[3 * (j*width + i) + 0] = 1;
									buffer[3 * (j*width + i) + 1] = 0;
									buffer[3 * (j*width + i) + 2] = 0;				  
								}
								else
								{			     
									buffer[3 * (j*width + i) + 0] = 0;
									buffer[3 * (j*width + i) + 1] = 1;
									buffer[3 * (j*width + i) + 2] = 1;				  
								}
							}
						}		  
					}

		};


	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_RENDERING_RTFACTRENDERER_H
