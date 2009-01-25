#include <BALL/VIEW/RENDERING/RENDERERS/rtfactRenderer.h>
#include <RTfact/Config/Init.inc.cpp>
//#include <RTfact/Utils/Packets/Detail/Constants.inc.cpp>

#include <BALL/STRUCTURE/triangulatedSurface.h>
#include <BALL/VIEW/PRIMITIVES/sphere.h>
#include <BALL/VIEW/PRIMITIVES/twoColoredTube.h>

using namespace RTfact::Remote;

namespace BALL
{
	namespace VIEW
	{
		bool RTfactRenderer::init(const Scene& scene)
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

			// prepare the sphere template
			TriangulatedSphere sphere_template;
			sphere_template.icosaeder();
			sphere_template.refine(3, true);

			sphere_template.exportSurface(sphere_template_);

			// prepare the tube template
			TriangulatedSurface* tube_template = TriangulatedSurface::createTube(18, 0, false, true);
			tube_template->exportSurface(tube_template_);
			delete (tube_template);
				
			return true;
		}


		void RTfactRenderer::prepareBufferedRendering(const Stage& stage)
		{
			Camera  const& camera = stage.getCamera();
			Vector3 const& position = camera.getViewPoint();
			Vector3 const& view_vector = camera.getViewVector();
			Vector3 const& look_up = camera.getLookUpVector();

			m_renderer.setCameraPosition(float3(position.x, position.y, position.z),
																	 float3(view_vector.x, view_vector.y, view_vector.z),
																	 float3(look_up.x, look_up.y, look_up.z));

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
							light_position = stage.calculateAbsoluteCoordinates(it->getPosition())+stage.getCamera().getViewPoint();
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

		void RTfactRenderer::renderToBufferImpl(FrameBufferPtr buffer)
		{		    
			bool recreate_accel = false;
			RepresentationManager& pm = scene_->getMainControl()->getRepresentationManager();

			RepresentationList::ConstIterator it = pm.getRepresentations().begin();
			for (; it != pm.getRepresentations().end(); ++it)
			{
				Representation const* rep = *it;
				if (rep->isHidden()) continue;

				List<GeometricObject*>::ConstIterator it;
				for (it =  rep->getGeometricObjects().begin();
						it != rep->getGeometricObjects().end();
						it++)
				{
					if (objects_.find(*it) == objects_.end())
					{
						recreate_accel = true;
						objects_.insert(*it);
						if (RTTI::isKindOf<Mesh>(**it))
						{
							Mesh const& mesh = *(const Mesh*)*it;

							float const* vertices = reinterpret_cast<float const*>(&(mesh.vertex[0]));
							float const* normals  = reinterpret_cast<float const*>(&(mesh.normal[0]));
							Index const* indices = reinterpret_cast<Index const*>(&(mesh.triangle[0]));

							RTAppearanceHandle material = m_renderer.createAppearance("PhongShader");
							material->setParam("ambientIntensity", float3(0.25, 0.25, 0.25));
							material->setParam("specularColor", float3(0.774597, 0.774597, 0.774597));
							material->setParam("reflective", float3(0.0, 0.0, 0.0));
							material->setParam("shininess", (float)76.8);

							GeoHandle handle;

							float const* colors = 0;
							if (mesh.colors.size() > 1)
							{
								colors  = reinterpret_cast<float const*>(&(mesh.colors[0]));
								material->setParam("useVertexColor", true);

							 	handle = m_renderer.createGeometry(vertices, normals, colors, (const unsigned int*)indices, (unsigned int)mesh.triangle.size(), material);
							} 
							else 
							{
								ColorRGBA const &c = (mesh.colors.size() == 1) ? mesh.colors[0] : ColorRGBA(1., 1., 1., 1.);

								material->setParam("diffuseColor", float3(c.getRed(), c.getGreen(), c.getBlue()));
								material->setParam("useVertexColor", false);

							 	handle = m_renderer.createGeometry(vertices, normals, (const unsigned int*)indices, (unsigned int)mesh.triangle.size(), material);
							}

							GroupHandle meshGroup = m_renderer.createGroup(Transform::identity());
							meshGroup->add(handle);                
							m_renderer.getRoot()->add(meshGroup);
						} 

						if (RTTI::isKindOf<Sphere>(**it))
						{
							Sphere const& sphere = *(const Sphere*)*it;

							float const* vertices = reinterpret_cast<float const*>(&(sphere_template_.vertex[0]));
							float const* normals  = reinterpret_cast<float const*>(&(sphere_template_.normal[0]));
							Index const* indices  = reinterpret_cast<Index const*>(&(sphere_template_.triangle[0]));

							ColorRGBA const& color = sphere.getColor();

							RTAppearanceHandle material = m_renderer.createAppearance("PhongShader");
							material->setParam("diffuseColor", float3(color.getRed(), color.getGreen(), color.getBlue()));
							material->setParam("ambientIntensity", float3(0.25, 0.25, 0.25));
							material->setParam("specularColor", float3(0.774597, 0.774597, 0.774597));
							material->setParam("shininess", (float)76.8);
							material->setParam("useVertexColor", false);

							GeoHandle handle   = m_renderer.createGeometry(vertices, normals, (const unsigned int*)indices, (unsigned int)sphere_template_.triangle.size(), material);

							Vector3 const& sphere_pos = sphere.getPosition();
							float radius = sphere.getRadius();

							GroupHandle sphereGroup = m_renderer.createGroup(  Transform::translation(sphere_pos.x, sphere_pos.y, sphere_pos.z)
																																*Transform::scale(Vec3f<1>(radius, radius, radius)));
							sphereGroup->add(handle);
							m_renderer.getRoot()->add(sphereGroup);
						}

						if (RTTI::isKindOf<TwoColoredTube>(**it))
						{
							TwoColoredTube const& old_tube = *(const TwoColoredTube*)*it;

							float const* vertices = reinterpret_cast<float const*>(&(tube_template_.vertex[0]));
							float const* normals  = reinterpret_cast<float const*>(&(tube_template_.normal[0]));
							Index const* indices  = reinterpret_cast<Index const*>(&(tube_template_.triangle[0]));

							// we will produce two tubes using the same vertex/normal/color values, just with the correct offsets
							ColorRGBA const& color1 = old_tube.getColor();
							ColorRGBA const& color2 = old_tube.getColor2();

							RTAppearanceHandle material_1 = m_renderer.createAppearance("PhongShader");
							material_1->setParam("diffuseColor", float3(color1.getRed(), color1.getGreen(), color1.getBlue()));
							material_1->setParam("ambientIntensity", float3(0.25, 0.25, 0.25));
							material_1->setParam("specularColor", float3(0.774597, 0.774597, 0.774597));
							material_1->setParam("shininess", (float)76.8);
							material_1->setParam("useVertexColor", false);

							GeoHandle handle_1 = m_renderer.createGeometry(vertices, normals, (const unsigned int*)indices, (unsigned int)tube_template_.triangle.size(), material_1);

							if (color1 == color2)
							{
								GroupHandle tubeGroup = transformTube(old_tube);
								tubeGroup->add(handle_1);

								m_renderer.getRoot()->add(tubeGroup);
							} 
							else 
							{
								RTAppearanceHandle material_2 = m_renderer.createAppearance("PhongShader");
								material_2->setParam("diffuseColor", float3(color2.getRed(), color2.getGreen(), color2.getBlue()));
								material_2->setParam("ambientIntensity", float3(0.25, 0.25, 0.25));
								material_2->setParam("specularColor", float3(0.774597, 0.774597, 0.774597));
								material_2->setParam("shininess", (float)76.8);
								material_2->setParam("useVertexColor", false);

								GeoHandle handle_2 = m_renderer.createGeometry(vertices, normals, (const unsigned int*)indices, (unsigned int)tube_template_.triangle.size(), material_2);

								TwoColoredTube new_tube = old_tube;
								new_tube.setVertex2(old_tube.getMiddleVertex());

								GroupHandle tubeGroup_1 = transformTube(new_tube);
								tubeGroup_1->add(handle_1);

								new_tube.setVertex1(old_tube.getVertex2());
								new_tube.setVertex2(old_tube.getMiddleVertex());
								GroupHandle tubeGroup_2 = transformTube(new_tube);
								tubeGroup_2->add(handle_2);

								m_renderer.getRoot()->add(tubeGroup_1);
								m_renderer.getRoot()->add(tubeGroup_2);
							}
						}

					}
				}
			}
			if (recreate_accel)
				m_renderer.createAccelStruct();
			FrameBufferFormat fmt = buffer->getFormat();		    
			if (fmt.getPixelFormat() == PixelFormat::RGBF_96)
			{
				m_renderer.attachFrameBuffer(fmt.getWidth(), fmt.getHeight(), fmt.getPitch(), (float*)buffer->getData());
				m_renderer.renderToBuffer();
			}
		}

		GroupHandle RTfactRenderer::transformTube(const TwoColoredTube& tube) 
		{
			Vector3 vec = tube.getVertex2() - tube.getVertex1();
			const double len = vec.getLength();
			const double angle = acos(vec.z / len); // the denominator accounts for the non-normalized rotation axis
			const float radius = tube.getRadius();

			Matrix4x4 trafo = Matrix4x4::getIdentity();
			//Rotate the vector around the normal
			vec /= sqrt(vec.x*vec.x + vec.y*vec.y);

			trafo.rotate(Angle(-angle), vec.y, -vec.x, 0);

			Matrix4f matrix(trafo.m11, trafo.m12, trafo.m13, trafo.m14,
											trafo.m21, trafo.m22, trafo.m23, trafo.m24,
											trafo.m31, trafo.m32, trafo.m33, trafo.m34,
											trafo.m41, trafo.m42, trafo.m43, trafo.m44);	

			Vector3 const& midpoint = tube.getVertex1();

			return m_renderer.createGroup(	Transform::translation(midpoint.x, midpoint.y, midpoint.z)
																		 *matrix*Transform::scale(Vec3f<1>(radius, radius, len)));
		}
	}
}
