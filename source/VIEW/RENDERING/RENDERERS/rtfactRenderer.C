#include <BALL/STRUCTURE/triangulatedSurface.h>
#include <BALL/VIEW/KERNEL/clippingPlane.h>
#include <BALL/VIEW/PRIMITIVES/sphere.h>
#include <BALL/VIEW/PRIMITIVES/twoColoredTube.h>
#include <BALL/VIEW/PRIMITIVES/twoColoredLine.h>

#include <RTfact/Config/Common.hpp>
#include <RTfact/Config/Init.hpp>

#include <BALL/VIEW/RENDERING/RENDERERS/rtfactRenderer.h>

#include <QtGui/QImage>

#include <list>

static const float LINE_RADIUS = 0.02;

using RTfact::Vec3f;
using RTpieCpp::GeometryHandle;
using RTpieCpp::MeshHandle;
using RTpieCpp::PickTaskHandle;
using RTpieCpp::IntersectTaskHandle;
using RTpieCpp::LightHandle;
using RTpieCpp::Image2DHandle;
using RTpieCpp::BufferHandle;
using RTpieCpp::float3;
using RTpieCpp::InstanceHandle;
using RTpieCpp::FrameBufferHandle;
using RTpieCpp::SceneHandle;
using RTpieCpp::AppearanceHandle;

namespace BALL
{
	namespace VIEW
	{
		const float RTfactRenderer::vectorDifferenceTolerance_= 0.0f;

		bool RTfactRenderer::hasFPScounter()
		{
			return true;
		}

		double RTfactRenderer::getFPS()
		{
			return fpsMeter.getFPS();
		}

		bool RTfactRenderer::init(Scene& scene)
		{
			Renderer::init(scene);

			// create rtpie handles
			rayTracer = RTpieCpp::CreateRayTracerHandle();
			sceneHandle = RTpieCpp::CreateSceneHandle();

			rayTracer.attachScene(sceneHandle);

			renderTask = rayTracer.createRenderTask();
			renderTask.setOverSamplingRate(1);
            renderTask.setAccumulatePixels(false);
			renderTask.setMaxDepth(20);

			pickTask = rayTracer.createPickTask();

			cameraHandle = RTpieCpp::CreateCameraHandle();

			renderTask.setCamera(cameraHandle);

			// prepare the sphere template
			TriangulatedSphere sphere_template;
			sphere_template.pentakisDodecaeder();
			sphere_template.refine(1, true);

			sphere_template.exportSurface(sphere_template_);

			// prepare the tube template
			TriangulatedSurface* tube_template = TriangulatedSurface::createTube(18, 0, false, true);
			tube_template->exportSurface(tube_template_);
			delete (tube_template);

			objects_.clear();

			rtfact_needs_update_ = false;

			return true;
		}

		GeometricObject* RTfactRenderer::pickObject(Position x, Position y)
		{
			PickTaskHandle pickTask = rayTracer.createPickTask();
			pickTask.setCamera( cameraHandle );
			pickTask.pick(
				static_cast<int>(x),
				static_cast<int>(y),
				renderBuffer.getWidth(), renderBuffer.getHeight());

			//returns true if some object was picked
			if(pickTask.hasResult())
			{
				//for all scene object

				if(geometric_objects_.empty())
				{
					HashMap<RTpieCpp::MeshHandle, GeometricObject*>::iterator geo = geometric_objects_.begin();
					for (; geo != geometric_objects_.end(); ++geo)
					{
						if (pickTask.getMesh() == geo->first)
						{
							return geo->second;
						}
					}
				}

				if(geometric_objects_inst.empty())
				{
					HashMap<RTpieCpp::InstanceHandle, GeometricObject*>::iterator geo = geometric_objects_inst.begin();
					for (; geo != geometric_objects_inst.end(); ++geo)
					{
						if (pickTask.getInstance() == geo->first)
						{
							return geo->second;
						}
					}
				}
			}

			return NULL;
		}

		// TODO: this should be done in parallel, or directly in the ray tracer
		void RTfactRenderer::pickObjects(Position x1, Position y1, Position x2, Position y2,
		                                 std::list<GeometricObject*>& objects)
		{
			if (x1 > x2) std::swap(x1, x2);
			if (y1 > y2) std::swap(y1, y2);

			for (Position x=x1; x<=x2; ++x)
			{
				for (Position y=y1; y<=y2; ++y)
				{
					GeometricObject* result = pickObject(x, y);
					if (result)
					{
						objects.push_back(result);
					}
				}
			}
		}

		void RTfactRenderer::setSize(float width, float height)
		{
			width_ = width;
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

			setFrustum(1.5f, RTfact::Packet<1,float>::C_INFINITY, -2.f * x_scale_, 2.f * x_scale_, 2.f * y_scale_, -2.f * y_scale_);
		}

		void RTfactRenderer::setupStereo(float eye_separation, float focal_length)
		{
			// TODO: - make near and far clip configurable!!!
			//       - keep the same frustrum until either the size or the stereo settings change

			float nearf    = 1.5f;
			float ndfl    = nearf / focal_length;
			float left    = -2.0 * stereo_frustum_conversion_width_  * x_scale_;
			float right   =  2.0 * stereo_frustum_conversion_width_  * x_scale_;
			float bottom 	= -2.0 * stereo_frustum_conversion_height_ * y_scale_;
			float top    	=  2.0 * stereo_frustum_conversion_height_ * y_scale_;

			float new_left   = 2*left  - eye_separation * ndfl;
			float new_right  = 2*right - eye_separation * ndfl;

			if (stage_->getCamera().getProjectionMode() == Camera::PERSPECTIVE)
				setFrustum(nearf, RTfact::Packet<1,float>::C_INFINITY, left, right, top, bottom);
			else
			{
				//glOrtho(new_left * orthographic_zoom_, new_right * orthographic_zoom_,
				//         bottom_ * orthographic_zoom_,      top_ * orthographic_zoom_, near_, far_);
			}
		}

		void RTfactRenderer::getFrustum(float& near_f, float& far_f, float& left_f, float& right_f, float& top_f, float& bottom_f)
		{
		  cameraHandle.getFrustum(&left_f, &right_f, &top_f, &bottom_f, &near_f, &far_f);

			left_f   *= 2.f*x_scale_;
			right_f  *= 2.f*x_scale_;
			top_f    *= 2.f*y_scale_;
			bottom_f *= 2.f*y_scale_;
		}

		void RTfactRenderer::setFrustum(float near_f, float far_f, float left_f, float right_f, float top_f, float bottom_f)
		{
			cameraHandle.setFrustum(
				left_f/ (2.f*x_scale_), right_f/ (2.f*x_scale_),
				top_f/ (2.f*y_scale_), bottom_f/ (2.f*y_scale_),
				near_f, far_f);
		}

		void RTfactRenderer::setLights(bool reset_all)
		{
			if (reset_all)
			{
				for (Position i=0; i<lights_.size(); ++i)
				  lights_[i].clear();
				lights_.clear();
			}

			Vector3 direction, light_position, attenuation;

			Size current_light=0;
			Size num_lights=lights_.size();

			std::list<LightSource>::const_iterator it = stage_->getLightSources().begin();
			for (; it != stage_->getLightSources().end(); ++it, ++current_light)
			{
				if (current_light >= num_lights)
				{
					LightHandle light;
					switch (it->getType())
					{
						case LightSource::DIRECTIONAL:
						  light = sceneHandle.createLight("DirectionalLight");
							break;
						case LightSource::POSITIONAL:
						  light = sceneHandle.createLight("PointLight");
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
							direction = stage_->calculateAbsoluteCoordinates(direction);

						lights_[current_light].setParam3f("direction", float3(direction.x, direction.y, direction.z));
						break;
					case LightSource::POSITIONAL:
						light_position = it->getPosition();
						if (it->isRelativeToCamera())
						{
							light_position = stage_->calculateAbsoluteCoordinates(it->getPosition())+stage_->getCamera().getViewPoint();
						}
						lights_[current_light].setParam3f("position", float3(light_position.x, light_position.y, light_position.z));

						attenuation = it->getAttenuation();
						lights_[current_light].setParam3f("attenuation", float3(attenuation.x, attenuation.y, attenuation.z));
						break;
					default:
						std::cerr << "Light source type not supported!" << std::endl;
						break;
				}

				float intensity = it->getIntensity();
				ColorRGBA const& color = it->getColor();

				lights_[current_light].setParam3f("intensity", float3((float)color.getRed()*intensity,(float)color.getGreen()*intensity,(float)color.getBlue()*intensity));
			}
		}

		void RTfactRenderer::updateCamera(const Camera* camera)
		{

			// the renderer should be paused whenever the camera has been updated
			if (camera == 0) camera = &(stage_->getCamera());

			Vector3 const& position = camera->getViewPoint();
			// RTfact relies on a normalized view vector, so we have to normalize it prior to handing it to RTfact
			// TODO: Store a normalized view vector in our Camera, mind project files
			Vector3  view_vector = camera->getViewVector();
			view_vector.normalize();
			Vector3 const& look_up = camera->getLookUpVector();

            if(use_continuous_loop_)
            {
                if (doVectorsDiffer( last_camera_position, position ))
                {
                    cameraHandle.setPosition( float3(position.x, position.y, position.z) );
                    last_camera_position  = position;
                }

                if (doVectorsDiffer( last_camera_view_vec, view_vector ))
                {
                    cameraHandle.setDirection( float3(view_vector.x, view_vector.y, view_vector.z) );
                    last_camera_view_vec  = view_vector;
                }

                if (doVectorsDiffer( last_camera_lookup, look_up ))
                {
                    cameraHandle.setUpVector( float3(look_up.x, look_up.y, look_up.z) );
                    last_camera_lookup    = look_up;
                }
            }
            else
            {
                cameraHandle.setPosition( float3(position.x, position.y, position.z) );
                cameraHandle.setDirection( float3(view_vector.x, view_vector.y, view_vector.z) );
                cameraHandle.setUpVector( float3(look_up.x, look_up.y, look_up.z) );

                last_camera_position  = position;
                last_camera_view_vec  = view_vector;
                last_camera_lookup    = look_up;
            }

			if (lights_.size() != stage_->getLightSources().size()) return;

			// lights that are relative to the camera need to have their position updated
			if (lights_.size() == 0) return; // TEST
			std::list<LightSource>::const_iterator it = stage_->getLightSources().begin();
			Size current_light=0;
			Vector3 light_position, direction;

			for (; it != stage_->getLightSources().end(); ++it, ++current_light)
			{
				switch (it->getType())
				{
					case LightSource::DIRECTIONAL:
						direction = it->getDirection();
						if (it->isRelativeToCamera())
							direction = stage_->calculateAbsoluteCoordinates(direction);

						if(!(lights_[current_light].getParam3f("direction") == float3(direction.x, direction.y, direction.z)))
							lights_[current_light].setParam3f("direction", float3(direction.x, direction.y, direction.z));
					case LightSource::POSITIONAL:
						light_position = it->getPosition();
						if (it->isRelativeToCamera())
						{
							light_position = stage_->calculateAbsoluteCoordinates(it->getPosition())+stage_->getCamera().getViewPoint();
						}

						if(!(lights_[current_light].getParam3f("position") == float3(light_position.x, light_position.y, light_position.z)))
							lights_[current_light].setParam3f("position", float3(light_position.x, light_position.y, light_position.z));
						break;
					default:
						break;
				}
			}
		}

		void RTfactRenderer::updateBackgroundColor()
		{
			sceneHandle.setEnvironmentColor(stage_->getBackgroundColor().getRed(),
															  stage_->getBackgroundColor().getGreen(),
																stage_->getBackgroundColor().getBlue(), 1);
		}

		void RTfactRenderer::setupEnvironmentMap(const QImage& image)
		{
			// convert the image into an RTfact - compatible array
			Size num_pixels = image.width()*image.height();

			float* rtfact_env_map = new float[3*num_pixels];

			Position current_pixel = 0;
			for (Position i=0; i<image.width(); ++i)
			{
				for (Position j=0; j<image.height(); ++j, ++current_pixel)
				{
					QRgb pixel = image.pixel(i,j);

					rtfact_env_map[3*current_pixel]   = qRed(pixel)/255.;
					rtfact_env_map[3*current_pixel+1] = qGreen(pixel)/255.;
					rtfact_env_map[3*current_pixel+2] = qBlue(pixel)/255.;
				}
			}

			RTfact::uint colorBufferLength = image.width() * image.height() * 3 * sizeof(float);
			BufferHandle colorBuffer = RTpieCpp::CreateBufferHandleUseData(colorBufferLength, rtfact_env_map);
			Image2DHandle imageHandle = RTpieCpp::CreateImage2DHandleUseData(
								RTfact::RTpie::IImage2D::COMPONENT_FLOAT,
								3,
								image.width(), image.height(),
								colorBuffer
						);

			sceneHandle.setEnvironmentTexture(imageHandle);

			delete[] (rtfact_env_map);
		}

		void RTfactRenderer::prepareBufferedRendering(const Stage& stage)
		{
			// this function is not needed for this kind of raytracer
		}

		void transformMeshData(float *mat,
											float const *vertices, int num_vertices,
											float const *normals, int num_normals,
											float *t_vertices, float *t_normals)
		{
				TMatrix4x4<float> m(mat);
				float w;
				for(int i = 0; i < num_vertices; i++)
				{
						TVector4<float> v = m * TVector4<float>(vertices[i*3], vertices[i*3+1], vertices[i*3+2], 1);
						v.get(t_vertices[i*3], t_vertices[i*3+1], t_vertices[i*3+2], w);
				}

				for(int i = 0; i < num_normals; i++)
				{
						TVector4<float> v = m * TVector4<float>(normals[i*3], normals[i*3+1], normals[i*3+2], 0);
						v.get(t_normals[i*3], t_normals[i*3+1], t_normals[i*3+2], w);
				}
		}

		void setPrimitives(RTpieCpp::MeshHandle aMesh,
				const RTfact::RTpie::uint32 aTriangleCount,
				const Index* aIndices,
				const float* aVertices,
				const float* aNormals,
				const float* aVertexColors,
				const float* aTexCoords, bool reorder = true)
		{
				if(reorder)
				{
					//reorder
					RTfact::RTpie::int32* reordered_indices = new RTfact::RTpie::int32[aTriangleCount*3];
					for(int i = 0;  i < aTriangleCount; i++)
					{
							reordered_indices[i * 3 + 2] = aIndices[i * 3 + 0];
							reordered_indices[i * 3 + 1] = aIndices[i * 3 + 1];
							reordered_indices[i * 3 + 0] = aIndices[i * 3 + 2];
					}

					aMesh.setPrimitives(aTriangleCount,
							reordered_indices, aVertices, aNormals, aVertexColors, aTexCoords);

					delete[] reordered_indices;
				}
				else
				{
					RTfact::RTpie::int32* converted_indices = new RTfact::RTpie::int32[aTriangleCount*3];
					for(int i = 0;  i < aTriangleCount; i++)
					{
							converted_indices[i * 3 + 0] = aIndices[i * 3 + 0];
							converted_indices[i * 3 + 1] = aIndices[i * 3 + 1];
							converted_indices[i * 3 + 2] = aIndices[i * 3 + 2];
					}

					aMesh.setPrimitives(aTriangleCount,
							converted_indices, aVertices, aNormals, aVertexColors, aTexCoords);
				}
    }

		void RTfactRenderer::bufferRepresentationDynamic(const Representation& rep)
		{
			if (rep.getGeometricObjects().empty())
				return;

			if (rep.isHidden())
			{
				if (rep.needsUpdate())
				{
					// if the representation has been changed while it was hidden, we need
					// to recreate it from scratch the next time it is enabled
					//
					// NOTE: it is safe to call removeRepresentation even if the representation
					//       has not yet been added
					removeRepresentation(rep);
				}
				return;
			}

			if (objects_.find(&rep) != objects_.end())
			{
				// was the representation previously disabled and now just needs enabling?
				if (objects_[&rep].has_been_disabled)
				{
					RTfactData& rt_data = objects_[&rep];

					// iterate over all top group handles and add them to the root again to make them visible
					for (Position i=0; i<rt_data.object_handles.size(); ++i)
					{
						rt_data.instance_handles.push_back(rt_data.object_handles[i].createInstance());
					}

					objects_[&rep].has_been_disabled = false;
					rtfact_needs_update_ = true;

					return;
				}
				else
				{
					// TODO: handle the update more gracefully!
					removeRepresentation(rep);
				}
			}

			RTfactData rt_data;
			rt_data.has_been_disabled = false;
			float trafo[16];

			Stage::Material rt_material = scene_->getStage()->getMaterial();
			if (rep.hasProperty("Rendering::Material"))
			{
				NamedProperty rt_mat_property = rep.getProperty("Rendering::Material");
				boost::shared_ptr<PersistentObject> mat_ptr = rt_mat_property.getSmartObject();
				rt_material = *dynamic_cast<Stage::Material*>(mat_ptr.get());
			}

			std::list<GeometricObject*>::const_iterator it;
			for (it =  rep.getGeometricObjects().begin();
					 it != rep.getGeometricObjects().end();
					 it++)
			{

                                if (RTTI::isKindOf<Mesh>(*it))
				{
					Mesh const& mesh = *(const Mesh*)*it;

					float const* vertices = reinterpret_cast<float const*>(&(mesh.vertex[0]));
					float const* normals  = reinterpret_cast<float const*>(&(mesh.normal[0]));
					Index const* indices  = reinterpret_cast<Index const*>(&(mesh.triangle[0]));

					//
					rt_data.material_handles.push_back(
					    sceneHandle.createAppearance("PhongShader"));
					convertMaterial(rt_material, rt_data.material_handles.back());

					//
					float const* colors = 0;
					if (mesh.colors.size() > 1)
					{
						colors  = reinterpret_cast<float const*>(&(mesh.colors[0]));
						rt_data.material_handles.back().setParamb("useVertexColor", true);

						//colors conversion (rtfact accepts RGB)
						float* rgbcolors = new float[3*mesh.colors.size()];
						for(int i =0; i < mesh.colors.size(); i++)
						{
							rgbcolors[i * 3 + 0] = colors[i * 4 + 0];
							rgbcolors[i * 3 + 1] = colors[i * 4 + 1];
							rgbcolors[i * 3 + 2] = colors[i * 4 + 2];
						}

						rt_data.object_handles.push_back(
								sceneHandle.createGeometry());
						rt_data.mesh_handles.push_back(
								rt_data.object_handles.back().createMesh());
						setPrimitives(rt_data.mesh_handles.back(),
							(unsigned int)mesh.triangle.size(),
							indices, vertices, normals, rgbcolors, 0);
						rt_data.mesh_handles.back().setAppearance(
								rt_data.material_handles.back());

						delete[] rgbcolors;
					}
					else
					{
						ColorRGBA const &c = (mesh.colors.size() == 1) ? mesh.colors[0] : ColorRGBA(1., 1., 1., 1.);

						rt_data.material_handles.back().setParam3f(
								"diffuseColor", float3(c.getRed(), c.getGreen(), c.getBlue()));
						rt_data.material_handles.back().setParamb("useVertexColor", false);

						rt_data.object_handles.push_back(
								sceneHandle.createGeometry());
						rt_data.mesh_handles.push_back(
								rt_data.object_handles.back().createMesh());
						setPrimitives(rt_data.mesh_handles.back(),
							(unsigned int)mesh.triangle.size(),
							indices, vertices, normals, 0, 0);
						rt_data.mesh_handles.back().setAppearance(
								rt_data.material_handles.back());
					}

					if (!rep.isHidden()) {
						rt_data.instance_handles.push_back(
						    rt_data.object_handles.back().createInstance());
					}

					geometric_objects_inst[rt_data.instance_handles.back()] = *it;

					rtfact_needs_update_ = true;
				}

                                if (RTTI::isKindOf<Sphere>(*it))
				{
					Sphere const& sphere = *(const Sphere*)*it;

					float const* vertices = reinterpret_cast<float const*>(&(sphere_template_.vertex[0]));
					float const* normals  = reinterpret_cast<float const*>(&(sphere_template_.normal[0]));
					Index const* indices  = reinterpret_cast<Index const*>(&(sphere_template_.triangle[0]));

					ColorRGBA const& color = sphere.getColor();

					//
					rt_data.material_handles.push_back(
					    sceneHandle.createAppearance("PhongShader"));
					convertMaterial(rt_material, rt_data.material_handles.back());
					rt_data.material_handles.back().setParam3f(
							"diffuseColor", float3(color.getRed(), color.getGreen(), color.getBlue()));
					rt_data.material_handles.back().setParamb("useVertexColor", false);

					//
					rt_data.object_handles.push_back(
							sceneHandle.createGeometry());
					rt_data.mesh_handles.push_back(
							rt_data.object_handles.back().createMesh());
					setPrimitives(rt_data.mesh_handles.back(),
						(unsigned int)sphere_template_.triangle.size(),
						indices, vertices, normals, 0, 0);
					rt_data.mesh_handles.back().setAppearance(
							rt_data.material_handles.back());

					//
					Vector3 const& sphere_pos = sphere.getPosition();
					float radius = sphere.getRadius();

					float mat[16] = {
						radius, 0, 0, 0,
						0, radius, 0, 0,
						0, 0, radius, 0,
						sphere_pos.x, sphere_pos.y, sphere_pos.z, 1};

					if (!rep.isHidden()) {
						rt_data.instance_handles.push_back(
						    rt_data.object_handles.back().createInstance());
						rt_data.instance_handles.back().setTransform(mat, NULL);
					}

					geometric_objects_inst[rt_data.instance_handles.back()] = *it;

					rtfact_needs_update_ = true;
				}

                                if (RTTI::isKindOf<TwoColoredTube>(*it))
				{
					TwoColoredTube const& old_tube = *(const TwoColoredTube*)*it;

					float const* vertices = reinterpret_cast<float const*>(&(tube_template_.vertex[0]));
					float const* normals  = reinterpret_cast<float const*>(&(tube_template_.normal[0]));
					Index const* indices  = reinterpret_cast<Index const*>(&(tube_template_.triangle[0]));

					// we will produce two tubes using the same vertex/normal/color values, just with the correct offsets
					ColorRGBA const& color1 = old_tube.getColor();
					ColorRGBA const& color2 = old_tube.getColor2();

					//
					rt_data.material_handles.push_back(
					    sceneHandle.createAppearance("PhongShader"));
					convertMaterial(rt_material, rt_data.material_handles.back());
					rt_data.material_handles.back().setParam3f(
							"diffuseColor", float3(color1.getRed(), color1.getGreen(), color1.getBlue()));
					rt_data.material_handles.back().setParamb("useVertexColor", false);

					//
					rt_data.object_handles.push_back(
							sceneHandle.createGeometry());
					geometric_objects_inst[rt_data.instance_handles.back()] = *it;
					rt_data.mesh_handles.push_back(
							rt_data.object_handles.back().createMesh());
					setPrimitives(rt_data.mesh_handles.back(),
						(unsigned int)tube_template_.triangle.size(),
						indices, vertices, normals, 0, 0, false);
					rt_data.mesh_handles.back().setAppearance(
							rt_data.material_handles.back());

					if (color1 == color2)
					{

						if (!rep.isHidden()) {
							rt_data.instance_handles.push_back(
									rt_data.object_handles.back().createInstance());

							transformTube(old_tube, trafo);
							rt_data.instance_handles.back().setTransform(trafo, 0);
						}

					}
					else
					{
						//
						rt_data.material_handles.push_back(
								sceneHandle.createAppearance("PhongShader"));
						convertMaterial(rt_material, rt_data.material_handles.back());
						rt_data.material_handles.back().setParam3f(
								"diffuseColor", float3(color2.getRed(), color2.getGreen(), color2.getBlue()));
						rt_data.material_handles.back().setParamb("useVertexColor", false);

						//
						Size last_geom_index = rt_data.object_handles.size() - 1;
						rt_data.object_handles.push_back(
								sceneHandle.createGeometry());
						geometric_objects_inst[rt_data.instance_handles.back()] = *it;
						rt_data.mesh_handles.push_back(
								rt_data.object_handles.back().createMesh());
						setPrimitives(rt_data.mesh_handles.back(),
							(unsigned int)tube_template_.triangle.size(),
							indices, vertices, normals, 0, 0, false);
						rt_data.mesh_handles.back().setAppearance(
								rt_data.material_handles.back());

						// NOTE: Just copying tube would be highly dangerous; vertex2 can store pointers
						//       to the vertices instead of using its own, and these are copied as well!
						TwoColoredTube new_tube_1, new_tube_2;

						new_tube_1.setVertex1(old_tube.getVertex1());
						new_tube_1.setVertex2(old_tube.getMiddleVertex());
						new_tube_1.setRadius(old_tube.getRadius());

						new_tube_2.setVertex1(old_tube.getMiddleVertex());
						new_tube_2.setVertex2(old_tube.getVertex2());
						new_tube_2.setRadius(old_tube.getRadius());

						if (!rep.isHidden()) {
							rt_data.instance_handles.push_back(
									rt_data.object_handles[last_geom_index].createInstance());
							transformTube(new_tube_1, trafo);
							rt_data.instance_handles.back().setTransform(trafo, 0);

							rt_data.instance_handles.push_back(
									rt_data.object_handles.back().createInstance());
							transformTube(new_tube_2, trafo);
							rt_data.instance_handles.back().setTransform(trafo, 0);
						}
					}

					rtfact_needs_update_ = true;
				}

                                if (RTTI::isKindOf<TwoColoredLine>(*it))
				{
					TwoColoredLine const& old_line = *(const TwoColoredLine*)*it;

					float const* vertices = reinterpret_cast<float const*>(&(tube_template_.vertex[0]));
					float const* normals  = reinterpret_cast<float const*>(&(tube_template_.normal[0]));
					Index const* indices  = reinterpret_cast<Index const*>(&(tube_template_.triangle[0]));

					// we will produce two tubes using the same vertex/normal/color values, just with the correct offsets
					ColorRGBA const& color1 = old_line.getColor();
					ColorRGBA const& color2 = old_line.getColor2();

					//
					rt_data.material_handles.push_back(
					    sceneHandle.createAppearance("PhongShader"));
					updateMaterialFromStage(rt_data.material_handles.back());
					rt_data.material_handles.back().setParam3f(
							"diffuseColor", float3(color1.getRed(), color1.getGreen(), color1.getBlue()));
					rt_data.material_handles.back().setParamb("useVertexColor", false);

					//
					rt_data.object_handles.push_back(
							sceneHandle.createGeometry());
					geometric_objects_inst[rt_data.instance_handles.back()] = *it;
					rt_data.mesh_handles.push_back(
							rt_data.object_handles.back().createMesh());
					setPrimitives(rt_data.mesh_handles.back(),
						(unsigned int)tube_template_.triangle.size(),
						indices, vertices, normals, 0, 0, false);
					rt_data.mesh_handles.back().setAppearance(
							rt_data.material_handles.back());

					if (color1 == color2)
					{
						if (!rep.isHidden()) {
							rt_data.instance_handles.push_back(
									rt_data.object_handles.back().createInstance());

							transformLine(old_line, trafo);
							rt_data.instance_handles.back().setTransform(trafo, 0);
						}
					}
					else
					{
						//
						rt_data.material_handles.push_back(
								sceneHandle.createAppearance("PhongShader"));
						convertMaterial(rt_material, rt_data.material_handles.back());
						rt_data.material_handles.back().setParam3f(
								"diffuseColor", float3(color2.getRed(), color2.getGreen(), color2.getBlue()));
						rt_data.material_handles.back().setParamb("useVertexColor", false);

						//
						RTpieCpp::GeometryHandle& lastGeom = rt_data.object_handles.back();
						rt_data.object_handles.push_back(
								sceneHandle.createGeometry());
						geometric_objects_inst[rt_data.instance_handles.back()] = *it;
						rt_data.mesh_handles.push_back(
								rt_data.object_handles.back().createMesh());
						setPrimitives(rt_data.mesh_handles.back(),
							(unsigned int)tube_template_.triangle.size(),
							indices, vertices, normals, 0, 0, false);
						rt_data.mesh_handles.back().setAppearance(
								rt_data.material_handles.back());

						// NOTE: Just copying tube would be highly dangerous; vertex2 can store pointers
						//       to the vertices instead of using its own, and these are copied as well!
						TwoColoredLine new_line_1, new_line_2;

						new_line_1.setVertex1(old_line.getVertex1());
						new_line_1.setVertex2(old_line.getMiddleVertex());
						//new_line_1.setRadius(LINE_RADIUS);//old_line.getRadius());

						new_line_2.setVertex1(old_line.getMiddleVertex());
						new_line_2.setVertex2(old_line.getVertex2());
						//new_line_2.setRadius(LINE_RADIUS);//old_line.getRadius());

						if (!rep.isHidden()) {
							rt_data.instance_handles.push_back(
									lastGeom.createInstance());
							transformLine(new_line_1, trafo);
							rt_data.instance_handles.back().setTransform(trafo, 0);

							rt_data.instance_handles.push_back(
									rt_data.object_handles.back().createInstance());
							transformLine(new_line_2, trafo);
							rt_data.instance_handles.back().setTransform(trafo, 0);
						}
					}

					rtfact_needs_update_ = true;

				}
			}

			objects_[&rep] = rt_data;
		}

		void RTfactRenderer::bufferRepresentation(const Representation& rep)
		{
			if (rep.getGeometricObjects().empty())
				return;

			if (rep.isHidden())
			{
				if (rep.needsUpdate())
				{
					// if the representation has been changed while it was hidden, we need
					// to recreate it from scratch the next time it is enabled
					//
					// NOTE: it is safe to call removeRepresentation even if the representation
					//       has not yet been added
					removeRepresentation(rep);
				}
				return;
			}

			if (objects_.find(&rep) != objects_.end())
			{
				// was the representation previously disabled and now just needs enabling?
				if (objects_[&rep].has_been_disabled)
				{
					RTfactData& rt_data = objects_[&rep];

					// iterate over all top group handles and add them to the root again to make them visible
					for (Position i=0; i<rt_data.object_handles.size(); ++i)
					{
						rt_data.instance_handles.push_back(rt_data.object_handles[i].createInstance());
					}

					objects_[&rep].has_been_disabled = false;
					rtfact_needs_update_ = true;

					return;
				}
				else
				{
					// TODO: handle the update more gracefully!
					removeRepresentation(rep);
				}
			}

			RTfactData rt_data;
			rt_data.has_been_disabled = false;

			Stage::Material rt_material = scene_->getStage()->getMaterial();
			if (rep.hasProperty("Rendering::Material"))
			{
				NamedProperty rt_mat_property = rep.getProperty("Rendering::Material");
				boost::shared_ptr<PersistentObject> mat_ptr = rt_mat_property.getSmartObject();
				rt_material = *dynamic_cast<Stage::Material*>(mat_ptr.get());
			}

			rt_data.cutPlaneShader = sceneHandle.createAppearance("PhongShader");
			rt_data.cutPlaneShader.setParam3f("diffuseColor", float3(0.0f, 0.0, 1.0f));
			rt_data.cutPlaneShader.setParam3f("ambientIntensity", float3(.0f, .0f, .0f));
			rt_data.cutPlaneShader.setParamf("transparency", 0.7f);

			//
			rt_data.object_handles.push_back(
					sceneHandle.createGeometry());

			std::list<GeometricObject*>::const_iterator it;
			for (it =  rep.getGeometricObjects().begin();
					 it != rep.getGeometricObjects().end();
					 it++)
			{

                                if (RTTI::isKindOf<Mesh>(*it))
				{
					Mesh const& mesh = *(const Mesh*)*it;

					float const* vertices = reinterpret_cast<float const*>(&(mesh.vertex[0]));
					float const* normals  = reinterpret_cast<float const*>(&(mesh.normal[0]));
					Index const* indices  = reinterpret_cast<Index const*>(&(mesh.triangle[0]));

					//
					rt_data.material_handles.push_back(
					    sceneHandle.createAppearance("PhongShader"));
					convertMaterial(rt_material, rt_data.material_handles.back());

					//
					float const* colors = 0;
					if (mesh.colors.size() > 1)
					{
						colors  = reinterpret_cast<float const*>(&(mesh.colors[0]));
						rt_data.material_handles.back().setParamb("useVertexColor", true);

						//colors conversion (rtfact accepts RGB)
						float* rgbcolors = new float[3*mesh.colors.size()];
						for(int i =0; i < mesh.colors.size(); i++)
						{
							rgbcolors[i * 3 + 0] = colors[i * 4 + 0];
							rgbcolors[i * 3 + 1] = colors[i * 4 + 1];
							rgbcolors[i * 3 + 2] = colors[i * 4 + 2];
						}

						rt_data.mesh_handles.push_back(
								rt_data.object_handles.back().createMesh());
						rt_data.mesh_handles.back().setAppearance(
								rt_data.material_handles.back());

						setPrimitives(rt_data.mesh_handles.back(),
							(unsigned int)mesh.triangle.size(),
							indices, vertices, normals, rgbcolors, 0);

						delete[] rgbcolors;
					}
					else
					{
						ColorRGBA const &c = (mesh.colors.size() == 1) ? mesh.colors[0] : ColorRGBA(1., 1., 1., 1.);

						rt_data.material_handles.back().setParam3f(
								"diffuseColor", float3(c.getRed(), c.getGreen(), c.getBlue()));
						rt_data.material_handles.back().setParamb("useVertexColor", false);

						rt_data.mesh_handles.push_back(
								rt_data.object_handles.back().createMesh());
						rt_data.mesh_handles.back().setAppearance(
								rt_data.material_handles.back());

						setPrimitives(rt_data.mesh_handles.back(),
							(unsigned int)mesh.triangle.size(),
							indices, vertices, normals, 0, 0);
					}

					geometric_objects_[rt_data.mesh_handles.back()] = *it;

					rtfact_needs_update_ = true;
				}

                                if (RTTI::isKindOf<Sphere>(*it))
				{
					Sphere const& sphere = *(const Sphere*)*it;

					float const* vertices = reinterpret_cast<float const*>(&(sphere_template_.vertex[0]));
					float const* normals  = reinterpret_cast<float const*>(&(sphere_template_.normal[0]));
					Index const* indices  = reinterpret_cast<Index const*>(&(sphere_template_.triangle[0]));

					ColorRGBA const& color = sphere.getColor();

					//
					rt_data.material_handles.push_back(
					    sceneHandle.createAppearance("PhongShader"));
					convertMaterial(rt_material, rt_data.material_handles.back());
					rt_data.material_handles.back().setParam3f(
							"diffuseColor", float3(color.getRed(), color.getGreen(), color.getBlue()));
					rt_data.material_handles.back().setParamb("useVertexColor", false);

					//
					rt_data.mesh_handles.push_back(
							rt_data.object_handles.back().createMesh());
					rt_data.mesh_handles.back().setAppearance(
							rt_data.material_handles.back());

					Vector3 const& sphere_pos = sphere.getPosition();
					float radius = sphere.getRadius();

					float mat[16] = {
						radius, 0, 0, sphere_pos.x,
						0, radius, 0, sphere_pos.y,
						0, 0, radius, sphere_pos.z,
						0, 0, 0, 1};
					float* t_vertices = new float[3*sphere_template_.getNumberOfVertices()];
					float* t_normals = new float[3*sphere_template_.getNumberOfNormals()];

					transformMeshData(mat,
														vertices, sphere_template_.getNumberOfVertices(),
														normals, sphere_template_.getNumberOfNormals(),
														t_vertices, t_normals);
					geometric_objects_[rt_data.mesh_handles.back()] = *it;
					setPrimitives(rt_data.mesh_handles.back(),
						(unsigned int)sphere_template_.triangle.size(),
						indices, t_vertices, t_normals, 0, 0);

					delete[] t_vertices;
					delete[] t_normals;

					rtfact_needs_update_ = true;
				}

                                if (RTTI::isKindOf<TwoColoredTube>(*it))
				{
					TwoColoredTube const& old_tube = *(const TwoColoredTube*)*it;

					float const* vertices = reinterpret_cast<float const*>(&(tube_template_.vertex[0]));
					float const* normals  = reinterpret_cast<float const*>(&(tube_template_.normal[0]));
					Index const* indices  = reinterpret_cast<Index const*>(&(tube_template_.triangle[0]));

					// we will produce two tubes using the same vertex/normal/color values, just with the correct offsets
					ColorRGBA const& color1 = old_tube.getColor();
					ColorRGBA const& color2 = old_tube.getColor2();

					//
					rt_data.material_handles.push_back(
					    sceneHandle.createAppearance("PhongShader"));
					convertMaterial(rt_material, rt_data.material_handles.back());
					rt_data.material_handles.back().setParam3f(
							"diffuseColor", float3(color1.getRed(), color1.getGreen(), color1.getBlue()));
					rt_data.material_handles.back().setParamb("useVertexColor", false);

					float mat[16];
					float* t_vertices = new float[3*tube_template_.getNumberOfVertices()];
					float* t_normals = new float[3*tube_template_.getNumberOfNormals()];

					//
					if (color1 == color2)
					{
						transformTube(old_tube, mat);
						rt_data.mesh_handles.push_back(
								rt_data.object_handles.back().createMesh());
						rt_data.mesh_handles.back().setAppearance(
								rt_data.material_handles.back());
						transformMeshData(mat,
															vertices, tube_template_.getNumberOfVertices(),
															normals, tube_template_.getNumberOfNormals(),
															t_vertices, t_normals);
						geometric_objects_[rt_data.mesh_handles.back()] = *it;
						setPrimitives(rt_data.mesh_handles.back(),
							(unsigned int)tube_template_.triangle.size(),
							indices, t_vertices, t_normals, 0, 0, false);
					}
					else
					{
						TwoColoredTube new_tube_1, new_tube_2;

						//
						new_tube_1.setVertex1(old_tube.getVertex1());
						new_tube_1.setVertex2(old_tube.getMiddleVertex());
						new_tube_1.setRadius(old_tube.getRadius());

						//
						new_tube_2.setVertex1(old_tube.getMiddleVertex());
						new_tube_2.setVertex2(old_tube.getVertex2());
						new_tube_2.setRadius(old_tube.getRadius());

						//
						transformTube(new_tube_1, mat);
						rt_data.mesh_handles.push_back(
								rt_data.object_handles.back().createMesh());
						rt_data.mesh_handles.back().setAppearance(
								rt_data.material_handles.back());
						transformMeshData(mat,
															vertices, tube_template_.getNumberOfVertices(),
															normals, tube_template_.getNumberOfNormals(),
															t_vertices, t_normals);
						geometric_objects_[rt_data.mesh_handles.back()] = *it;
						setPrimitives(rt_data.mesh_handles.back(),
							(unsigned int)tube_template_.triangle.size(),
							indices, t_vertices, t_normals, 0, 0, false);

						//
						rt_data.material_handles.push_back(
								sceneHandle.createAppearance("PhongShader"));
						convertMaterial(rt_material, rt_data.material_handles.back());
						rt_data.material_handles.back().setParam3f(
								"diffuseColor", float3(color2.getRed(), color2.getGreen(), color2.getBlue()));
						rt_data.material_handles.back().setParamb("useVertexColor", false);

						//
						transformTube(new_tube_2, mat);
						rt_data.mesh_handles.push_back(
								rt_data.object_handles.back().createMesh());
						rt_data.mesh_handles.back().setAppearance(
								rt_data.material_handles.back());
						transformMeshData(mat,
															vertices, tube_template_.getNumberOfVertices(),
															normals, tube_template_.getNumberOfNormals(),
															t_vertices, t_normals);
						geometric_objects_[rt_data.mesh_handles.back()] = *it;
						setPrimitives(rt_data.mesh_handles.back(),
							(unsigned int)tube_template_.triangle.size(),
							indices, t_vertices, t_normals, 0, 0, false);
					}

					delete[] t_vertices;
					delete[] t_normals;

					rtfact_needs_update_ = true;
				}

                                if (RTTI::isKindOf<TwoColoredLine>(*it))
				{
					TwoColoredLine const& old_line = *(const TwoColoredLine*)*it;

					float const* vertices = reinterpret_cast<float const*>(&(tube_template_.vertex[0]));
					float const* normals  = reinterpret_cast<float const*>(&(tube_template_.normal[0]));
					Index const* indices  = reinterpret_cast<Index const*>(&(tube_template_.triangle[0]));

					// we will produce two tubes using the same vertex/normal/color values, just with the correct offsets
					ColorRGBA const& color1 = old_line.getColor();
					ColorRGBA const& color2 = old_line.getColor2();

					//
					rt_data.material_handles.push_back(
					    sceneHandle.createAppearance("PhongShader"));
					updateMaterialFromStage(rt_data.material_handles.back());
					rt_data.material_handles.back().setParam3f(
							"diffuseColor", float3(color1.getRed(), color1.getGreen(), color1.getBlue()));
					rt_data.material_handles.back().setParamb("useVertexColor", false);

					float mat[16];
					float* t_vertices = new float[3*tube_template_.getNumberOfVertices()];
					float* t_normals = new float[3*tube_template_.getNumberOfNormals()];

					if (color1 == color2)
					{
						transformLine(old_line, mat);
						rt_data.mesh_handles.push_back(
								rt_data.object_handles.back().createMesh());
						rt_data.mesh_handles.back().setAppearance(
								rt_data.material_handles.back());
						transformMeshData(mat,
															vertices, tube_template_.getNumberOfVertices(),
															normals, tube_template_.getNumberOfNormals(),
															t_vertices, t_normals);
						geometric_objects_[rt_data.mesh_handles.back()] = *it;
						setPrimitives(rt_data.mesh_handles.back(),
							(unsigned int)tube_template_.triangle.size(),
							indices, t_vertices, t_normals, 0, 0, false);
					}
					else
					{
						TwoColoredLine new_line_1, new_line_2;

						new_line_1.setVertex1(old_line.getVertex1());
						new_line_1.setVertex2(old_line.getMiddleVertex());
						//new_line_1.setRadius(LINE_RADIUS);//old_line.getRadius());

						new_line_2.setVertex1(old_line.getMiddleVertex());
						new_line_2.setVertex2(old_line.getVertex2());
						//new_line_2.setRadius(LINE_RADIUS);//old_line.getRadius());

						//
						transformLine(new_line_1, mat);
						rt_data.mesh_handles.push_back(
								rt_data.object_handles.back().createMesh());
						rt_data.mesh_handles.back().setAppearance(
								rt_data.material_handles.back());
						transformMeshData(mat,
															vertices, tube_template_.getNumberOfVertices(),
															normals, tube_template_.getNumberOfNormals(),
															t_vertices, t_normals);
						geometric_objects_[rt_data.mesh_handles.back()] = *it;
						setPrimitives(rt_data.mesh_handles.back(),
							(unsigned int)tube_template_.triangle.size(),
							indices, t_vertices, t_normals, 0, 0, false);

						//
						rt_data.material_handles.push_back(
								sceneHandle.createAppearance("PhongShader"));
						convertMaterial(rt_material, rt_data.material_handles.back());
						rt_data.material_handles.back().setParam3f(
								"diffuseColor", float3(color2.getRed(), color2.getGreen(), color2.getBlue()));
						rt_data.material_handles.back().setParamb("useVertexColor", false);

						//
						transformLine(new_line_2, mat);
						rt_data.mesh_handles.push_back(
								rt_data.object_handles.back().createMesh());
						rt_data.mesh_handles.back().setAppearance(
								rt_data.material_handles.back());
						transformMeshData(mat,
															vertices, tube_template_.getNumberOfVertices(),
															normals, tube_template_.getNumberOfNormals(),
															t_vertices, t_normals);
						geometric_objects_[rt_data.mesh_handles.back()] = *it;
						setPrimitives(rt_data.mesh_handles.back(),
							(unsigned int)tube_template_.triangle.size(),
							indices, t_vertices, t_normals, 0, 0, false);

					}

					rtfact_needs_update_ = true;

				}
			}

			if (!rep.isHidden()) {
				rt_data.instance_handles.push_back(
						rt_data.object_handles.back().createInstance());
			}

			objects_[&rep] = rt_data;
		}

		void RTfactRenderer::removeRepresentation(Representation const& rep)
		{
			if (objects_.find(&rep) != objects_.end())
			{
				// TODO: find out if this also deletes the geometries and materials
				RTfactData& rt_data = objects_[&rep];

				for (Position i=0; i<rt_data.mesh_handles.size(); ++i)
				{
					geometric_objects_.erase(rt_data.mesh_handles[i]);
				}
				for (Position i=0; i<rt_data.instance_handles.size(); ++i)
				{
					geometric_objects_inst.erase(rt_data.instance_handles[i]);
					rt_data.instance_handles[i].clear();
				}

				rt_data.instance_handles.clear();

				rt_data.material_handles.clear();

				rt_data.mesh_handles.clear();

				rtfact_needs_update_ = true;

				objects_.erase(&rep);
			}
		}

		void RTfactRenderer::useContinuousLoop(bool use_loop)
		{
            Renderer::useContinuousLoop(use_loop);
            renderTask.setAccumulatePixels(use_loop);
		}

		// line x plane intersection
		void intersection(Vector3 p, Vector3 d, // line = point + direction
										 Vector3 o, Vector3 n, // plane = origin + normal
										 Vector3* op, int& on)
		{
				float c = - o * n;
				float dn = d * n;
				if(dn == 0.0f)
						return;
				float t = - (n * p + c) / dn;
				if(t >= 0.0f && t <= 1.0f)
				{
						op[on++] = p + t * d;
				}
		}

		struct PlanarPointsCompare {
				Vector3 orig, norm;
				PlanarPointsCompare(Vector3 _orig, Vector3 _norm) : norm(_norm), orig(_orig) {}
				bool operator() (const Vector3 &a, const Vector3 &b) {
						Vector3 v = (a - orig) % (b - orig);
						return (v * norm) < 0;
				}
		};

		void RTfactRenderer::renderToBufferImpl(FrameBufferPtr buffer)
		{
			if (!getMainControl())
				return;

			Stage const& stage = *(scene_->getStage());

			// deactivate hidden representations (we need no reactivation code,
			// since reactivated representations will simply be buffered again)
			for (HashMap<Representation const*, RTfactData>::iterator it = objects_.begin(); it != objects_.end(); ++it)
			{
				if (it->first->isHidden() && !it->second.has_been_disabled)
				{
					// It is safe to remove a group multiple times from RTfact. It may not be
					// *fast*, but it should be safe.
					RTfactData& rt_data = it->second;

					for (Position i=0; i<rt_data.instance_handles.size(); ++i)
					{
					  rt_data.instance_handles[i].clear();
					}

					rt_data.instance_handles.clear();

					it->second.has_been_disabled = true;

					rtfact_needs_update_ = true;
				}
			}

			//clipping planes
			RepresentationManager& pm = getMainControl()->getRepresentationManager();
			const vector<ClippingPlane*>& vc = pm.getClippingPlanes();
			RepresentationList::const_iterator it = pm.getRepresentations().begin();
			for (; it != pm.getRepresentations().end(); it++)
			{
			    std::map< RTpieCpp::InstanceHandle, std::vector<Vector3> > normals;
			    std::map< RTpieCpp::InstanceHandle, std::vector<Vector3> > points;
			    std::map< RTpieCpp::InstanceHandle, std::vector<bool> > active;
                std::map< RTpieCpp::InstanceHandle, std::vector<ColorRGBA> > cappingColors;

					const Representation& rep = **it;

					// has this representation been buffered already?
					if (objects_.find(&rep) == objects_.end())
						continue;

					RTfactData& rtfactData = objects_[&rep];
					bool cappingEnabled = false;

					vector<ClippingPlane*>::const_iterator plane_it = vc.begin();
					for (;plane_it != vc.end(); plane_it++)
					{
							ClippingPlane& plane = **plane_it;

                            //This line means if the clipping plane clips the given representation
                            if (!plane.getRepresentations().has(*it)) continue;
							if (plane.isHidden()) continue;

							std::vector<RTpieCpp::InstanceHandle>::iterator iit = rtfactData.instance_handles.begin();
							for(; iit != rtfactData.instance_handles.end(); iit++)
							{

									const Vector3& n(plane.getNormal());
									const Vector3& p(plane.getPoint());

                                    normals[*iit].push_back(n);
                                    points[*iit].push_back(p);
                                    active[*iit].push_back(plane.isActive());
                                    cappingColors[*iit].push_back(plane.getCappingColor());

							}

							cappingEnabled = plane.cappingEnabled();
					}

					std::vector<RTpieCpp::InstanceHandle>::iterator iit = rtfactData.instance_handles.begin();
					for(; iit != rtfactData.instance_handles.end(); iit++)
					{
							const std::vector<Vector3>& instanceNormals = normals[*iit];
							const std::vector<Vector3>& instancePoints = points[*iit];
							const std::vector<bool>& instanceActive = active[*iit];
                            const std::vector<ColorRGBA>& instanceCappingColors = cappingColors[*iit];
							const int n = instanceNormals.size();

                            float* data = new float[n*9];

							int a = 0;
							for(int i = 0; i < n; i++)
							{
									if(!instanceActive[i]) continue;
									data[0*n + a] = instanceNormals[i].x;
									data[1*n + a] = instanceNormals[i].y;
									data[2*n + a] = instanceNormals[i].z;
									data[3*n + a] = instancePoints[i].x;
									data[4*n + a] = instancePoints[i].y;
									data[5*n + a] = instancePoints[i].z;
                                    data[6*n + a] = instanceCappingColors[i].getRed();
                                    data[7*n + a] = instanceCappingColors[i].getGreen();
                                    data[8*n + a] = instanceCappingColors[i].getBlue();
									a++;
							}

							//TODO: color from ballview configuration
                            iit->setCutPlanes(cappingEnabled, float3(data[6*n], data[7*n], data[8*n]), a,
									data+0*n, data+1*n, data+2*n,
									data+3*n, data+4*n, data+5*n);


							//visualization
							//TODO: there is a bug somewhere (some intersections are in the origin)
							float min[3];
							float max[3];
							iit->getBounds(min, max);

							rtfactData.cutPlaneInstances.clear();
							rtfactData.cutPlaneMeshes.clear();
							rtfactData.cutPlanes.clear();

                            //Add the geometry for drawing the clipping plane if it is visible but not active
                            for(int j = 0; j < n; j++)
                            {
                                    if(instanceActive[j]) continue;

									int m = 0;
									Vector3 p[6];

									//
									intersection(Vector3(min[0], min[1], min[2]), Vector3(max[0]-min[0], 0, 0), instancePoints[j], instanceNormals[j], p, m);
									intersection(Vector3(min[0], min[1], min[2]), Vector3(0, max[1]-min[1], 0), instancePoints[j], instanceNormals[j], p, m);
									intersection(Vector3(min[0], min[1], min[2]), Vector3(0, 0, max[2]-min[2]), instancePoints[j], instanceNormals[j], p, m);
									intersection(Vector3(max[0], max[1], max[2]), Vector3(min[0]-max[0], 0, 0), instancePoints[j], instanceNormals[j], p, m);
									intersection(Vector3(max[0], max[1], max[2]), Vector3(0, min[1]-max[1], 0), instancePoints[j], instanceNormals[j], p, m);
									intersection(Vector3(max[0], max[1], max[2]), Vector3(0, 0, min[2]-max[2]), instancePoints[j], instanceNormals[j], p, m);

									intersection(Vector3(max[0], max[1], min[2]), Vector3(min[0]-max[0], 0, 0), instancePoints[j], instanceNormals[j], p, m);
									intersection(Vector3(max[0], max[1], min[2]), Vector3(0, min[1]-max[1], 0), instancePoints[j], instanceNormals[j], p, m);

									intersection(Vector3(min[0], min[1], max[2]), Vector3(max[0]-min[0], 0, 0), instancePoints[j], instanceNormals[j], p, m);
									intersection(Vector3(min[0], min[1], max[2]), Vector3(0, max[1]-min[1], 0), instancePoints[j], instanceNormals[j], p, m);

									intersection(Vector3(max[0], min[1], min[2]), Vector3(0, 0, max[2]-min[2]), instancePoints[j], instanceNormals[j], p, m);
									intersection(Vector3(min[0], max[1], min[2]), Vector3(0, 0, max[2]-min[2]), instancePoints[j], instanceNormals[j], p, m);

									//
									PlanarPointsCompare cmp(p[0], instanceNormals[0]);
									std::sort(p, p + m, cmp);

									//
									Index* CUTPLANE_INDICES = new Index[3*m];
									float* CUTPLANE_POSITIONS = new float[3 * 3 * (m-2)];
									float* CUTPLANE_NORMALS = new float[3 * 3 * (m-2)];

									for(int i = 0; i < m-2; i++)
									{
											CUTPLANE_POSITIONS[i*3*3 + 0] = p[0].x;
											CUTPLANE_POSITIONS[i*3*3 + 1] = p[0].y;
											CUTPLANE_POSITIONS[i*3*3 + 2] = p[0].z;

											CUTPLANE_POSITIONS[i*3*3 + 3] = p[i+1].x;
											CUTPLANE_POSITIONS[i*3*3 + 4] = p[i+1].y;
											CUTPLANE_POSITIONS[i*3*3 + 5] = p[i+1].z;

											CUTPLANE_POSITIONS[i*3*3 + 6] = p[i+2].x;
											CUTPLANE_POSITIONS[i*3*3 + 7] = p[i+2].y;
											CUTPLANE_POSITIONS[i*3*3 + 8] = p[i+2].z;
									}

									for(int i = 0; i < 3*(m-2); i++)
									{
											CUTPLANE_INDICES[i] = i;
											CUTPLANE_NORMALS[i*3 + 0] = instanceNormals[0].x;
											CUTPLANE_NORMALS[i*3 + 1] = instanceNormals[0].y;
											CUTPLANE_NORMALS[i*3 + 2] = instanceNormals[0].z;
									}

									//
									rtfactData.cutPlanes.push_back(sceneHandle.createGeometry());
									rtfactData.cutPlaneMeshes.push_back(rtfactData.cutPlanes.back().createMesh());
									setPrimitives(rtfactData.cutPlaneMeshes.back(),
										m-2,
										CUTPLANE_INDICES,
										CUTPLANE_POSITIONS,
										CUTPLANE_NORMALS,
										0, 0);
									rtfactData.cutPlaneMeshes.back().setAppearance(rtfactData.cutPlaneShader);
									rtfactData.cutPlaneInstances.push_back( rtfactData.cutPlanes.back().createInstance() );

									float m1[16], m2[16];
									iit->getTransform(m1, m2);
									rtfactData.cutPlaneInstances.back().setTransform(m1,m2);

									rtfactData.cutPlaneInstances.back().setDropShadow(false);

									//
									delete[] CUTPLANE_INDICES;
									delete[] CUTPLANE_POSITIONS;
									delete[] CUTPLANE_NORMALS;
							}

					}

			}

			if (rtfact_needs_update_)
			{
				rayTracer.syncStructures();
				rtfact_needs_update_ = false;
			}

			FrameBufferFormat fmt = buffer->getFormat();
			if (objects_.size() != 0)
			{

				Image2DHandle distanceBufferHandle;
				Image2DHandle colorBufferHandle;

				if (fmt.getPixelFormat() == PixelFormat::RGBF_96)
				{
					//color buffer (use given memory)
					t_ColorImage* colorImage = new t_ColorImage((float*)buffer->getData(), 3, width_, height_, width_+stride_, false);
					RTfact::uint colorBufferLength = colorImage->getResX() * colorImage->getResY()* colorImage->getComponentCount() * sizeof(t_ColorImage::Component);
					BufferHandle colorBuffer = RTpieCpp::CreateBufferHandleUseData(colorBufferLength, colorImage->getFirstComponent(0,0));
					colorBufferHandle = RTpieCpp::CreateImage2DHandleUseData(RTfact::RTpie::IImage2D::COMPONENT_FLOAT, 3, width_, height_, colorBuffer);

					//distance buffer
					t_DistanceImage* distanceImage = new t_DistanceImage(1, width_, height_);
					RTfact::uint distanceBufferLength = distanceImage->getResX() * distanceImage->getResY()* distanceImage->getComponentCount() * sizeof(t_DistanceImage::Component);
					BufferHandle distanceBuffer = RTpieCpp::CreateBufferHandleUseData(distanceBufferLength, distanceImage->getFirstComponent(0,0));
					distanceBufferHandle = RTpieCpp::CreateImage2DHandleUseData(RTfact::RTpie::IImage2D::COMPONENT_FLOAT, 1, width_, height_, distanceBuffer);

					//set framebuffer buffers
					framebuffer.setColorImage(colorImage);
					framebuffer.setDistanceImage(distanceImage);
				}
				else if (fmt.getPixelFormat() == PixelFormat::RGBA_32)
				{
					//color buffer (use given memory)
					t_ByteColorImage* colorImage = new t_ByteColorImage((unsigned char*)buffer->getData(), 4, width_, height_, width_+stride_, false);
					RTfact::uint colorBufferLength = colorImage->getResX() * colorImage->getResY()* colorImage->getComponentCount() * sizeof(t_ByteColorImage::Component);
					BufferHandle colorBuffer = RTpieCpp::CreateBufferHandleUseData(colorBufferLength, colorImage->getFirstComponent(0,0));
					colorBufferHandle = RTpieCpp::CreateImage2DHandleUseData(RTfact::RTpie::IImage2D::COMPONENT_BYTE, 4, width_, height_, colorBuffer);

					//distance buffer
					t_ByteDistanceImage* distanceImage = new t_ByteDistanceImage(1, width_, height_);
					RTfact::uint distanceBufferLength = distanceImage->getResX() * distanceImage->getResY()* distanceImage->getComponentCount() * sizeof(t_ByteDistanceImage::Component);
					BufferHandle distanceBuffer = RTpieCpp::CreateBufferHandleUseData(distanceBufferLength, distanceImage->getFirstComponent(0,0));
					distanceBufferHandle = RTpieCpp::CreateImage2DHandleUseData(RTfact::RTpie::IImage2D::COMPONENT_BYTE, 1, width_, height_, distanceBuffer);

					//set framebuffer buffers
					byteFramebuffer.setColorImage(colorImage);
					byteFramebuffer.setDistanceImage(distanceImage);
				}
				else
				{
					return;
				}

				//set framebuffer to rendertask
				renderBuffer = RTpieCpp::CreateFrameBufferHandle();
				renderBuffer.setColorBuffer(colorBufferHandle);
				renderBuffer.setDistanceBuffer(distanceBufferHandle);
				renderTask.setFrameBuffer(renderBuffer);

				//
				framebuffer.prePaint();

				fpsMeter.startFrame();

				rayTracer.syncStructures();

				renderTask.run();

				fpsMeter.endFrame();

				framebuffer.postPaint();

			}
		}

		void RTfactRenderer::updateMaterialForRepresentation(Representation const* rep)
		{

			if (objects_.find(rep) != objects_.end())
			{
				RTfactData& rt_data = objects_[rep];

				for (Position i=0; i<rt_data.material_handles.size(); ++i)
				{
					if (rep->hasProperty("Rendering::Material"))
					{
						NamedProperty rt_mat_property = rep->getProperty("Rendering::Material");
						boost::shared_ptr<PersistentObject> mat_ptr = rt_mat_property.getSmartObject();
						convertMaterial(*dynamic_cast<Stage::Material*>(mat_ptr.get()), rt_data.material_handles[i]);
					}
					else
						updateMaterialFromStage(rt_data.material_handles[i]);
				}
			}
		}

		void RTfactRenderer::transformTube(const TwoColoredTube& tube, float *trafo)
		{
			Vector3 vec = tube.getVertex2() - tube.getVertex1();
			const double len = vec.getLength();
			const double angle = acos(vec.z / len); // the denominator accounts for the non-normalized rotation axis
			const float radius = tube.getRadius();

			Vector3 const& midpoint = tube.getVertex1();
			//Rotate the vector around the normal
			vec /= sqrt(vec.x*vec.x + vec.y*vec.y);

			Matrix4x4 matrix = Matrix4x4::getIdentity();
			matrix.rotate(Angle(-angle), vec.y, -vec.x, 0);

			Matrix4x4 temp;
			temp.setScale(radius, radius, len);
			matrix*=temp;

			temp.setTranslation(midpoint);
			matrix = temp*matrix;

			for (Position i=0; i<4; ++i)
				for (Position j=0; j<4; ++j)
					trafo[i*4+j] = matrix(i, j);
		}

		void RTfactRenderer::transformLine(const TwoColoredLine& line, float *trafo)
		{
			Vector3 vec = line.getVertex2() - line.getVertex1();
			const double len = vec.getLength();
			const double angle = acos(vec.z / len); // the denominator accounts for the non-normalized rotation axis
			const float radius = LINE_RADIUS;//line.getRadius();

			Vector3 const& midpoint = line.getVertex1();
			//Rotate the vector around the normal
			vec /= sqrt(vec.x*vec.x + vec.y*vec.y);

			Matrix4x4 matrix = Matrix4x4::getIdentity();
			matrix.rotate(Angle(-angle), vec.y, -vec.x, 0);

			Matrix4x4 temp;
			temp.setScale(radius, radius, len);
			matrix*=temp;

			temp.setTranslation(midpoint);
			matrix = temp*matrix;

			for (Position i=0; i<4; ++i)
				for (Position j=0; j<4; ++j)
                    trafo[i*4+j] = matrix(i, j);
		}

		void RTfactRenderer::updateMaterialFromStage(AppearanceHandle& material)
		{

			Stage::Material const& rt_material = scene_->getStage()->getMaterial();
			convertMaterial(rt_material, material);

		}

		void RTfactRenderer::convertMaterial(Stage::Material const& rt_material, AppearanceHandle& material)
		{
			// ambience
			float red   = (float)rt_material.ambient_color.getRed()   * rt_material.ambient_intensity;
			float blue  = (float)rt_material.ambient_color.getBlue()  * rt_material.ambient_intensity;
			float green = (float)rt_material.ambient_color.getGreen() * rt_material.ambient_intensity;

			material.setParam3f("ambientIntensity", float3(red, blue, green));

			// specularity
			red   = (float)rt_material.specular_color.getRed()   * rt_material.specular_intensity;
			blue  = (float)rt_material.specular_color.getBlue()  * rt_material.specular_intensity;
			green = (float)rt_material.specular_color.getGreen() * rt_material.specular_intensity;

			material.setParam3f("specularColor", float3(red, blue, green));

			// reflectiveness
			red   = (float)rt_material.reflective_color.getRed()   * rt_material.reflective_intensity;
			blue  = (float)rt_material.reflective_color.getBlue()  * rt_material.reflective_intensity;
			green = (float)rt_material.reflective_color.getGreen() * rt_material.reflective_intensity;

			material.setParam3f("reflective", float3(red, blue, green));

			// shininess
			material.setParamf("shininess", rt_material.shininess);

			// transparency            
            material.setParamf("transparency", rt_material.transparency * 0.01f);
		}

		std::vector<float> RTfactRenderer::intersectRaysWithGeometry(const std::vector<Vector3>& origins,
		                                                             const std::vector<Vector3>& directions)
		{
			if (origins.size() != directions.size())
			{
				Log.error() << "RTfactRenderer::intersectRaysWithGeometry: sizes of origins/directions vectors disagree!" << std::endl;

				return std::vector<float>();
			}

			std::vector<float> results(origins.size());

			IntersectTaskHandle intersectTask = rayTracer.createIntersectTask();
			intersectTask.intersectRays(
				reinterpret_cast<const float*>(&origins[0]),
				reinterpret_cast<const float*>(&directions[0]),
				origins.size(),
				reinterpret_cast<float*>(&results[0]));

            return results;
        }

        bool RTfactRenderer::doVectorsDiffer(const Vector3 &vecA, const Vector3 &vecB)
        {
            return ((vecA - vecB).getSquareLength() > vectorDifferenceTolerance_);
        }

	}
}
