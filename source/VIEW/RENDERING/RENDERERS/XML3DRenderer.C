// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/RENDERING/RENDERERS/XML3DRenderer.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/VIEW/KERNEL/stage.h>
#include <BALL/VIEW/KERNEL/clippingPlane.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/WIDGETS/scene.h>

#include <BALL/VIEW/PRIMITIVES/label.h>
#include <BALL/VIEW/PRIMITIVES/line.h>
#include <BALL/VIEW/PRIMITIVES/mesh.h>
#include <BALL/VIEW/PRIMITIVES/point.h>
#include <BALL/VIEW/PRIMITIVES/box.h>
#include <BALL/STRUCTURE/triangulatedSurface.h>
#include <BALL/VIEW/PRIMITIVES/simpleBox.h>
#include <BALL/VIEW/PRIMITIVES/sphere.h>
#include <BALL/VIEW/PRIMITIVES/tube.h>
#include <BALL/VIEW/PRIMITIVES/disc.h>
#include <BALL/VIEW/PRIMITIVES/twoColoredLine.h>
#include <BALL/VIEW/PRIMITIVES/twoColoredTube.h>
#include <BALL/VIEW/PRIMITIVES/mesh.h>
#include <BALL/VIEW/PRIMITIVES/multiLine.h>


using std::endl;

namespace BALL
{
	namespace VIEW
	{

#define BALLVIEW_XML3D_LINE_RADIUS "BALL_LINE_RADIUS"

		XML3DRenderer::XML3DRenderer()
			: Renderer(),
				outfile_(&std::cout),
				human_readable_(true),
				create_XHTML_(false)
		{
		}

		XML3DRenderer::XML3DRenderer(const XML3DRenderer& renderer)
			: Renderer(renderer),
				outfile_(&std::cout),
				human_readable_(renderer.human_readable_),
				create_XHTML_(renderer.create_XHTML_)
		{
		}


		XML3DRenderer::XML3DRenderer(const String& name)
			throw(Exception::FileNotFound)
			: Renderer(),
				human_readable_(true),
				create_XHTML_(true)
		{
			outfile_ = new File(name, std::ios::out);
		}

		XML3DRenderer::XML3DRenderer(std::ostream& out_stream)
			: Renderer(),
				human_readable_(true),
				create_XHTML_(false)
		{
			outfile_ = &out_stream;
		}
		
		XML3DRenderer::~XML3DRenderer()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << this << " of class XML3DRenderer" << endl;
			#endif

				clear();
		}

		void XML3DRenderer::clear()
		{
			if (outfile_ != 0 && RTTI::isKindOf<File>(*outfile_))
			{
				delete outfile_;
			}

			outfile_ = &std::cout;
			human_readable_ = true;

			representations_.clear();
			color_map_.clear();
			color_index_ = 0;
		}

		void XML3DRenderer::setSize(float width, float height)
		{
			// this should really be done in the camera...
			float x_scale, y_scale;
			if (width > height)
			{
				x_scale = width / (height * 2);
				y_scale = 0.5;
			}
			else
			{
				x_scale = 0.5;
				y_scale = height / (width * 2);
			}

			float nearf =  1.5f;
			float left  = -2.f*x_scale;
			float right =  2.f*x_scale;
			float bot   = -2.f*y_scale;
			float top   =  2.f*y_scale;

			fov_x_ = 2*atan( (right-left)/(2*nearf) );
			fov_y_ = 2*atan( (top-bot)/(2*nearf) );
		}

		void XML3DRenderer::setFileName(const String& name)
			throw(Exception::FileNotFound)
		{
			create_XHTML_ = true;
			if (outfile_ == 0 || !RTTI::isKindOf<File>(*outfile_)) 
			{
				outfile_ = new File();
			}
			(*(File*)outfile_).open(name, std::ios::out);
		}

		void XML3DRenderer::setOstream(std::ostream& out_stream)
		{
			create_XHTML_ = false;
			if (outfile_ != 0 && RTTI::isKindOf<File>(*outfile_)) 
			{
				delete outfile_;
			}
			outfile_ = &out_stream;
		}


		String XML3DRenderer::XML3DColorRGBA(const ColorRGBA& color, const String& name)
		{
			// NOTE: XML3D currently does not use the alpha value => use float3
			String output = "<float3 name=\"" + name + "\">\n";

			output += String("   ") + String((float)color.getRed())   + " "
																	    + String((float)color.getGreen()) + " "
																		  + String((float)color.getBlue())
																		  + "\n";

			output += "</float3>\n";

			return output;
		}

		String XML3DRenderer::XML3DFinish(const String& object, const ColorRGBA& input)
		{
			String output = "finish { BALLFinish";
			output += object;

			if ((Size) input.getAlpha() >= 255)
			{
				output += "Solid";
			}
			else
			{
				output += "Transp";
			}

			output += " }";

			return output;
		}
		
		String XML3DRenderer::XML3DRaytracingMaterial(const Stage::Material& input)
		{
			String result;
      	
			ColorRGBA old_ambience = rt_material_.ambient_color;
			ColorRGBA ambience((float)old_ambience.getRed()  * rt_material_.ambient_intensity,
			                  (float)old_ambience.getBlue()  * rt_material_.ambient_intensity,
											  (float)old_ambience.getGreen() * rt_material_.ambient_intensity, 1.);

      result += XML3DColorRGBA(ambience, "ambientIntensity");
			
			ColorRGBA old_specular = rt_material_.specular_color;
			ColorRGBA specular((float)old_specular.getRed()   * rt_material_.specular_intensity,
			                   (float)old_specular.getBlue()  * rt_material_.specular_intensity,
											   (float)old_specular.getGreen() * rt_material_.specular_intensity, 1.);

      result += XML3DColorRGBA(specular, "specularColor");

			ColorRGBA old_reflect = rt_material_.reflective_color;
			ColorRGBA reflect((float)old_reflect.getRed()   * rt_material_.reflective_intensity,
			                  (float)old_reflect.getBlue()  * rt_material_.reflective_intensity,
											  (float)old_reflect.getGreen() * rt_material_.reflective_intensity, 1.);

      result += XML3DColorRGBA(reflect, "reflective");
      
      result += "<float name=\"shininess\">\n";
      result += "   "+String(rt_material_.shininess/128.)+"\n";
      result += "</float>\n";

			result += "<float name=\"transparency\">\n";
      result += "   "+String(rt_material_.transparency)+"\n";
      result += "</float>\n";

      result += "<float name=\"transparent\">\n";
      result += "   " + String(rt_material_.transparency) + " "
			                       + String(rt_material_.transparency) + " "
														 + String(rt_material_.transparency) + "\n";
      result += "</float>\n";

			return result;
		}

		String XML3DRenderer::XML3DVector3(Vector3 input)
		{
			String output = String("<float3>") + input.x + " " + input.y + " " + input.z + "</float3>\n";
			return output;
		}

		String XML3DRenderer::trimFloatValue_(float value)
		{
			String output = String(value);
			for (Position p = 0; p < output.size(); p++)
			{
				if (output[p] == '.')
				{
					output = output.left(p + 4);
					output.trimRight("0");
					if (output == "-0.") output = "0.";
					return output;
				}
			}

			return output;
		}
		
		String XML3DRenderer::XML3DString(const String& input)
		{
			String output = input;
			while (output.substitute("<", "") != String::EndPos) {};
			while (output.substitute(">", "") != String::EndPos) {};

			return output;
		}

		bool XML3DRenderer::init(Scene& scene)
		{
			return Renderer::init(scene);
		}

		// init must be called right before the rendering starts, since
		// we need to fix the camera, light sources, etc...
		bool XML3DRenderer::init(const Stage& stage, float width, float height)
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Start the XML3DRender output..." << endl;
			#endif

			setSize(width, height);

			wireframes_.clear();
			representations_.clear();
			color_map_.clear();
			color_strings_.clear();
			color_index_ = 0;

			current_sphere_number_ = 0;
			current_tube_number_ = 0;

    // prepare the sphere template
      TriangulatedSphere sphere_template;
      sphere_template.pentakisDodecaeder();
      sphere_template.refine(1, true);

      sphere_template.exportSurface(sphere_template_);

      // prepare the tube template
      TriangulatedSurface* tube_template = TriangulatedSurface::createTube(18, 0, false, true);
      tube_template->exportSurface(tube_template_);
      delete (tube_template);

			std::ostream& out = *outfile_;

			if (!Renderer::init(stage, width, height)) return false;

			if (create_XHTML_) {createXHTMLHeader();};

			// initialize xml3d environment	
			out << "<xml3d id=\"MyXml3d\" " << endl;

			//set height and width of the scene
			out << "style=\"width:" << int(width) << "px; ";
			out << "height: " << int(height) << "px; ";
			String bg_color;
			stage_->getBackgroundColor().get(bg_color);
			bg_color.truncate(6);
			bg_color.toLower();
			out << "background-color: #" << bg_color << ";\" ";

			out << "xmlns=\"http://www.xml3d.org/2009/xml3d\" ";
			
			//Enable Debug
			//out << "webgl:showLog=\"true\"" << endl;
			
			// set Active View, standard name ist ballview_camera
			out << "activeView=\"ballview_camera\" ";

			// Close xml3d opening tag
			out << ">" << endl;
			

			// Define BALLView Camera view
			out << "	<view id=\"ballview_camera\" ";
			out << "	position=\"" << stage.getCamera().getViewPoint().x << " "
													<< stage.getCamera().getViewPoint().y << " "
													<< stage.getCamera().getViewPoint().z << "\"";
			
			// Calculate rotation to align BALLview camera with standard XML3D camera
			Vector3 direction = stage.getCamera().getViewVector();
			
			if (direction.getSquareLength() < 1e-6)
			{
				Log.error() << "Mist" << endl;
				return false;
			}
			direction.normalize();

			Vector3 d_null = Vector3(0,0,-1);
			Vector3 dir_rotation_axis = (d_null % direction);

			float dir_rotation_angle = 0.;
			if ((fabs(direction.x) < 1e-6) && (fabs(direction.y) < 1e-6))
			{
				dir_rotation_axis = Vector3(1,0,0);
				if (direction.z > 0)
				{
					dir_rotation_angle = M_PI; 
				}
				else
				{
					dir_rotation_angle = 0;
				}
			}
			else
			{
				dir_rotation_axis.normalize();
				dir_rotation_angle = acos(direction * d_null);
			}
			
			Quaternion dir_rotation(dir_rotation_axis, dir_rotation_angle);
			Vector3 quataxis;
			float quatangle;
			dir_rotation.toAxisAngle(quataxis, quatangle);

			Matrix4x4 dir_rotation_matrix;
			dir_rotation.getRotationMatrix(dir_rotation_matrix);
			Vector3 testergebnis = dir_rotation_matrix * Vector3(0,0,-1);
			
			// Correct the position of the new upvector as introduced by the first rotation
			Vector3 upvector = stage.getCamera().getLookUpVector();
			upvector.normalize();
			
			Vector3 up_rotated = dir_rotation_matrix * Vector3(0,1,0);
			Vector3 up_rotation_axis = upvector % up_rotated;
			float up_rotation_angle = 0.;	
			if (up_rotation_axis.getSquareLength() < 1e-6)
			{
				if (up_rotated.y < 1e-6)
				{
					up_rotation_angle = M_PI;	
				}
			}
			
			up_rotation_angle = acos(upvector * up_rotated);
			Quaternion up_rotation(up_rotation_axis, -up_rotation_angle);
			Quaternion final_rotation = up_rotation * dir_rotation;
			Vector3 final_rotation_axis;
			float final_rotation_angle;
			final_rotation.toAxisAngle(final_rotation_axis, final_rotation_angle);
			
			Matrix4x4 final_rotation_matrix;
			final_rotation.getRotationMatrix(final_rotation_matrix);

			out << " orientation=\""<< final_rotation_axis.x << " "
															<< final_rotation_axis.y << " "
															<< final_rotation_axis.z << " "
															<< final_rotation_angle << "\"";
			
			out << " fieldOfView=\"" << 1.2 << "\"";
			out << " fieldOfViewX=\"" << fov_x_ << "\"";
			out << " fieldOfViewY=\"" << fov_y_ << "\"";

			Vector3 look_at_vector = stage.getCamera().getLookAtPosition();
			out << " lookAtVector=\"" << look_at_vector.x << " "
				                      << look_at_vector.y << " "
									  << look_at_vector.z << "\"";


			out << " >";
			out << "</view>\n" << endl;
			
			//// Start the Defs Section
			//out << "<defs id=\"MainDef\">\n";
			
			list<LightSource>::const_iterator it = stage_->getLightSources().begin();
			for (int i=0; it!=stage_->getLightSources().end(); ++it, ++i)
			{
				if (it->getType() == LightSource::POSITIONAL)
				{
					Vector3 pos = it->getPosition();
					float intensity = it->getIntensity() ;
					ColorRGBA color = it->getColor();
					Vector3 attenuation = it->getAttenuation();
					if (it->isRelativeToCamera())
					{
						pos = stage_->calculateAbsoluteCoordinates(pos) + stage_->getCamera().getViewPoint();
					}
					out << "<transform id=\"lighttransform" << i << "\" translation=\"";
					out << pos.x << " " << pos.y << " " << pos.z << "\" /> " << endl;
					out << "<lightshader id=\"point" << i << "\" script=\"urn:xml3d:lightshader:point\" > "; 
					out << "<float3 name=\"intensity\">" << (float) color.getRed() * intensity << " " 
																												<< (float) color.getGreen() * intensity << " "
																												<< (float) color.getBlue() * intensity <<   "</float3> ";
					out << "<float3 name=\"attenuation\">" << attenuation.x << " " << attenuation.y << " " << attenuation.z << "</float3> ";
					out << "</lightshader>" << endl;
					out << "<group transform=\"lighttransform" << i << "\" >" << endl;
					out << "<light shader=\"#point" << i << "\" id=\"mypoint" << i << "\"";
					
					if (it->isRelativeToCamera())
					{
						out << " relativeToCamera=\"true\"";
					}
					
					out << " />" << endl;
					out << "</group>" << endl;
				}
				
			}
			//out << "</defs>\n";
			
			return true;
		}

		bool XML3DRenderer::finish()
		{
			std::ostream& out = *outfile_;

			rt_material_ = stage_->getMaterial();

			vector<const Representation*>::iterator rit = representations_.begin();
			for (; rit != representations_.end(); rit++)
			{
				if ((*rit)->hasProperty("Rendering::Material"))
				{
					NamedProperty rt_mat_property = (*rit)->getProperty("Rendering::Material");
					boost::shared_ptr<PersistentObject> mat_ptr = rt_mat_property.getSmartObject();
					rt_material_ = *dynamic_cast<Stage::Material*>(mat_ptr.get());
				}

				out << "<group ";
				out << "id=\"" << XML3DString((*rit)->getName()) << "\"";
				out << ">" << endl;
				
				list<GeometricObject*>::const_iterator it;
				for (it =  (*rit)->getGeometricObjects().begin();
						 it != (*rit)->getGeometricObjects().end();
						 it++)
				{
					render_(*it);
				}
				out << "</group>" << endl;
			}

			//close xml3d environment
			out << "		</xml3d>" << endl;
			
			
			if (create_XHTML_) {createXHTMLFooter();};	

			if (outfile_ != 0 && RTTI::isKindOf<File>(*outfile_))
			{
				(*(File*)outfile_).close();
			}

			return true;
		}
		
		void XML3DRenderer::createXHTMLHeader()
		{
			std::ostream& out = *outfile_;
			
			//Create X-HMTL encasing	
			out << "<?xml version=\"1.0\" encoding=\"utf-8\" ?>" << endl;
			out << "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\"" << endl;
			out << "\"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtdi\" >" << endl;
			out << "<html xmlns=\"http://www.w3.org/1999/xhtml\">" << endl;
			out << "  <head>" << endl;
			out << "  <title>Our BALLView Export</title>" << endl;
			out << "    <link rel=\"stylesheet\" type=\"text/css\" href=\"http://xml3d.org/xml3d/script/xml3d.css\" />" << endl;
			out << "  </head>" << endl;
			
			// Start XHTML Body
			out << "  <body>" << endl;
			out << "    <h1>Look here:</h1>" << endl;
		}

		void XML3DRenderer::createXHTMLFooter()
		{
			std::ostream& out = *outfile_;

			//add some javascripts, caution relative paths

  		out << "<script type=\"text/javascript\" src=\"http://xml3d.org/xml3d/script/xml3d.js\"></script>" << endl; 
			
			// End XHTML Body
			out << "  </body>" << endl;
			out << "</html>" << endl;
		}
	

		void XML3DRenderer::renderSphere_(const Sphere& sphere)
		{
			std::ostream& out = *outfile_;

      Vector3 const& sphere_pos = sphere.getPosition();
      float radius = sphere.getRadius();
			
			//define a shader for each sphere

      ColorRGBA const& color = sphere.getColor();
			out << "<shader id=\"" << &sphere << "shader\" script=\"urn:xml3d:shader:phong\" >" << endl;
     	out << XML3DColorRGBA(color, "diffuseColor");
			out << XML3DRaytracingMaterial(rt_material_);
      
      out << "</shader>" << endl;
			
			//define a transform for each sphere
			out << "<transform ";
			out << "id=\"" << &sphere_pos << "\" ";
			out << "translation=\"" << sphere_pos.x << " " << sphere_pos.y << " " << sphere_pos.z << "\" ";  
			out << "scale=\"" << radius << " " << radius << " " << radius << "\" ";  
			out << "/>" << endl;
			
			out << "<group id=\"Sphere_" << current_sphere_number_++ << "\" ";
			out << "transform=\"#" << &sphere_pos << "\" "; 
			out << "shader=\"#" << &sphere << "shader\" >" << endl; 
			
			//if (mesh.vertex.size() == 0 ||
			//    mesh.normal.size() == 0 ||
			//		mesh.triangle.size() == 0)
			//{
			//	return;
			//}

			//if (mesh.vertex.size() != mesh.normal.size())
			//{
			//	BALLVIEW_DEBUG;
			//	return;
			//}

			// Start a triangle env	
			out << "<mesh "; 
			out << "type=\"triangles\"";
			out << ">" << endl; 

			// First the indices
			out << "<int name=\"index\">" << endl;
 			vector<Surface::Triangle>::const_iterator itt = sphere_template_.triangle.begin();
 			
			for (; itt != sphere_template_.triangle.end(); itt++)
   		{
				out << (*itt).v1 << " " << (*itt).v2 << " " << (*itt).v3 << " ";
			}	
			
			out << "</int>" << endl;

			// then then position vectors
			out << "<float3 name=\"position\">" << endl;
 			
			vector<Surface::Vertex>::const_iterator itv = sphere_template_.vertex.begin();
 			for (; itv != sphere_template_.vertex.end(); itv++)
   		{
				out << (*itv).x << " " << (*itv).y << " " << (*itv).z << endl;
			}	
			
			out << "</float3>" << endl;;
			
			// then then position normals
			out << "<float3 name=\"normal\">" << endl;
 			
			vector<Surface::Normal>::const_iterator itn = sphere_template_.normal.begin();
 			for (; itn != sphere_template_.normal.end(); itn++)
   		{
				out << (*itn).x << " " << (*itn).y << " " << (*itn).z << endl;
			}	
			
			out << "</float3>" << endl;
			
			out << "</mesh>" << endl;
			out << "</group>" << endl; 

		}

		void XML3DRenderer::renderDisc_(const Disc& disc)
		{
			// *Not Implemented yet*
		}

		void XML3DRenderer::renderLine_(const Line& line)
		{
			// *Not implemented yet*
		}

		void XML3DRenderer::renderTwoColoredLine_(const TwoColoredLine& tube)
		{
			// *Not implmented yet*
		}


		void XML3DRenderer::renderTube_(const Tube& tube)
		{
			// *Not implmented yet*
		}	

		void XML3DRenderer::renderPoint_(const Point& point)
		{
			// *Not implmented yet*
		}

		void XML3DRenderer::renderTwoColoredTube_(const TwoColoredTube& tube)
		{
			std::ostream& out = *outfile_;
			
			ColorRGBA const& color1 = tube.getColor();
			ColorRGBA const& color2 = tube.getColor2();

			if (color1 == color2)
			{
				out << "<shader id=\"" << &tube << "shader\" script=\"urn:xml3d:shader:phong\" >" << endl;
     		out << XML3DColorRGBA(color1, "diffuseColor");
				out << XML3DRaytracingMaterial(rt_material_);
				out << "</shader>" << endl;
				createTubeTransform_(tube);

				out << "<group id=\"Tube_" << current_tube_number_++ << "\" ";
				out << "transform=\"#" << &tube << "trafo\" ";
				out << "shader=\"#" << &tube << "shader\" >" << endl; 
				
				// Start a triangle env	
				out << "<mesh "; 
				out << "type=\"triangles\"";
				out << ">" << endl; 

				// First the indices
				out << "<int name=\"index\">" << endl;
 				vector<Surface::Triangle>::const_iterator itt = tube_template_.triangle.begin();
 				
				for (; itt != tube_template_.triangle.end(); itt++)
   			{
					out << (*itt).v1 << " " << (*itt).v2 << " " << (*itt).v3 << " ";
				}	
				
				out << "</int>" << endl;

				// then then position vectors
				out << "<float3 name=\"position\">" << endl;
 				
				vector<Surface::Vertex>::const_iterator itv = tube_template_.vertex.begin();
 				for (; itv != tube_template_.vertex.end(); itv++)
   			{
					out << (*itv).x << " " << (*itv).y << " " << (*itv).z << endl;
				}	
				
				out << "</float3>" << endl;;
				
				// then then position normals
				out << "<float3 name=\"normal\">" << endl;
 				
				vector<Surface::Normal>::const_iterator itn = tube_template_.normal.begin();
 				for (; itn != tube_template_.normal.end(); itn++)
   			{
					out << (*itn).x << " " << (*itn).y << " " << (*itn).z << endl;
				}	
				
				out << "</float3>" << endl;
				
				out << "</mesh>" << endl;
				out << "</group>" << endl; 
			}
			else
			{
				TwoColoredTube new_tube_1, new_tube_2;
				out << "<shader id=\"" << &new_tube_1 << "shader\" script=\"urn:xml3d:shader:phong\" >" << endl;
     		out << XML3DColorRGBA(color1, "diffuseColor");
				out << XML3DRaytracingMaterial(rt_material_);
				out << "</shader>" << endl;

				out << "<shader id=\"" << &new_tube_2 << "shader\" script=\"urn:xml3d:shader:phong\" >" << endl;
     		out << XML3DColorRGBA(color2, "diffuseColor");
				out << XML3DRaytracingMaterial(rt_material_);
				out << "</shader>" << endl;

				new_tube_1.setVertex1(tube.getVertex1());
				new_tube_1.setVertex2(tube.getMiddleVertex());
				new_tube_1.setRadius(tube.getRadius());

				new_tube_2.setVertex1(tube.getMiddleVertex());
				new_tube_2.setVertex2(tube.getVertex2());
				new_tube_2.setRadius(tube.getRadius());
				
				createTubeTransform_(new_tube_1);
				createTubeTransform_(new_tube_2);
				
				out << "<group id=\"Tube_" << current_tube_number_ << "_1\" ";
				out << "transform=\"#" << &new_tube_1 << "trafo\" "; 
				out << "shader=\"#" << &new_tube_1 << "shader\" >" << endl; 
				
				// Start a triangle env	
				out << "<mesh "; 
				out << "type=\"triangles\"";
				out << ">" << endl; 

				// First the indices
				out << "<int name=\"index\">" << endl;
 				vector<Surface::Triangle>::const_iterator itt = tube_template_.triangle.begin();
 				
				for (; itt != tube_template_.triangle.end(); itt++)
   			{
					out << (*itt).v1 << " " << (*itt).v2 << " " << (*itt).v3 << " ";
				}	
				
				out << "</int>" << endl;

				// then then position vectors
				out << "<float3 name=\"position\">" << endl;
 				
				vector<Surface::Vertex>::const_iterator itv = tube_template_.vertex.begin();
 				for (; itv != tube_template_.vertex.end(); itv++)
   			{
					out << (*itv).x << " " << (*itv).y << " " << (*itv).z << endl;
				}	
				
				out << "</float3>" << endl;;
				
				// then then position normals
				out << "<float3 name=\"normal\">" << endl;
 				
				vector<Surface::Normal>::const_iterator itn = tube_template_.normal.begin();
 				for (; itn != tube_template_.normal.end(); itn++)
   			{
					out << (*itn).x << " " << (*itn).y << " " << (*itn).z << endl;
				}	
				
				out << "</float3>" << endl;
				
				out << "</mesh>" << endl;
				out << "</group>" << endl; 
				
				// the second tube
				out << "<group id=\"Tube_" << current_tube_number_++ << "_2\" ";
				out << "transform=\"#" << &new_tube_2 << "trafo\" "; 
				out << "shader=\"#" << &new_tube_2 << "shader\" >" << endl; 
				
				// Start a triangle env	
				out << "<mesh "; 
				out << "type=\"triangles\"";
				out << ">" << endl; 

				// First the indices
				out << "<int name=\"index\">" << endl;
 				vector<Surface::Triangle>::const_iterator itt2 = tube_template_.triangle.begin();
 				
				for (; itt2 != tube_template_.triangle.end(); itt2++)
   			{
					out << (*itt2).v1 << " " << (*itt2).v2 << " " << (*itt2).v3 << " ";
				}	
				
				out << "</int>" << endl;

				// then then position vectors
				out << "<float3 name=\"position\">" << endl;
 				
				vector<Surface::Vertex>::const_iterator itv2 = tube_template_.vertex.begin();
 				for (; itv2 != tube_template_.vertex.end(); itv2++)
   			{
					out << (*itv2).x << " " << (*itv2).y << " " << (*itv2).z << endl;
				}	
				
				out << "</float3>" << endl;;
				
				// then then position normals
				out << "<float3 name=\"normal\">" << endl;
 				
				vector<Surface::Normal>::const_iterator itn2 = tube_template_.normal.begin();
 				for (; itn2 != tube_template_.normal.end(); itn2++)
   			{
					out << (*itn2).x << " " << (*itn2).y << " " << (*itn2).z << endl;
				}	
				
				out << "</float3>" << endl;
				
				out << "</mesh>" << endl;
				out << "</group>" << endl; 
			}

		}

		void XML3DRenderer::renderMesh_(const Mesh& mesh)
		{
			
			if (mesh.vertex.size() == 0 ||
			    mesh.normal.size() == 0 ||
					mesh.triangle.size() == 0)
			{
				return;
			}

			if (mesh.vertex.size() != mesh.normal.size())
			{
				BALLVIEW_DEBUG;
				return;
			}

			std::ostream& out = *outfile_;
			
			ColorRGBA const &c = (mesh.colors.size() == 1) ? mesh.colors[0] : ColorRGBA(1., 1., 1., 1.);
    		
			out << "<shader id=\"" << &mesh << "shader\" script=\"urn:xml3d:shader:phong\" >" << endl;

			if (mesh.colors.size() == 1)
			{
				out << XML3DColorRGBA(c, "diffuseColor");
			}
			else
			{
				out << "<bool name=\"useVertexColor\">true</bool>\n";
			}

			out << XML3DRaytracingMaterial(rt_material_);
			out << "</shader>" << endl;

			out << "<group "; 
			out << "id=\"" << &mesh << "\" ";
			
			out << "shader=\"" << &mesh << "shader\" ";
			out << ">" << endl; 
			
			out << "<mesh "; 
			out << "type=\"triangles\"";
			out << ">" << endl; 

			// First the indices
			out << "<int name=\"index\">" << endl;
 			vector<Surface::Triangle>::const_iterator itt = mesh.triangle.begin();
 			
			for (; itt != mesh.triangle.end(); itt++)
   		{
				out << (*itt).v1 << " " << (*itt).v2 << " " << (*itt).v3 << " ";
			}	
			
			out << "</int>" << endl;

			// then then position vectors
			out << "<float3 name=\"position\">" << endl;
 			
			vector<Surface::Vertex>::const_iterator itv = mesh.vertex.begin();
 			for (; itv != mesh.vertex.end(); itv++)
   		{
				out << (*itv).x << " " << (*itv).y << " " << (*itv).z << endl;
			}	
			
			out << "</float3>" << endl;;
			
			// then then position normals
			out << "<float3 name=\"normal\">" << endl;
 			
			vector<Surface::Normal>::const_iterator itn = mesh.normal.begin();
 			for (; itn != mesh.normal.end(); itn++)
   		{
				out << (*itn).x << " " << (*itn).y << " " << (*itn).z << endl;
			}	
			
			out << "</float3>" << endl;
			
			// export Vertex Colors if necessary	
			if (mesh.colors.size() > 1)
			{
				out << "<float3 name=\"color\">" << endl;

				vector<ColorRGBA>::const_iterator itc = mesh.colors.begin();
				for (; itc != mesh.colors.end(); itc++)
				{
					out << (float)(*itc).getRed() << " " << (float)(*itc).getGreen() << " " << (float)(*itc).getBlue() << endl;
				}	

				out << "</float3>" << endl;
			}
 			
			
			out << "</mesh>" << endl;
			
			out << "</group>" << endl; 

		}
				
		const ColorRGBA& XML3DRenderer::getColor_(const GeometricObject& object)
		{
			if ((object.getComposite()) && object.getComposite()->isSelected())
			{
				return BALL_SELECTED_COLOR;
			}
			
			return object.getColor();
		}
		
		bool XML3DRenderer::renderOneRepresentation(const Representation& representation)
		{
			if (representation.isHidden()) return true;

			if (!representation.isValid())
			{
				Log.error() << "Representation " << &representation 
										<< "not valid, so aborting." << std::endl;
				return false;
			}	

			list<GeometricObject*>::const_iterator it;

			for (it =  representation.getGeometricObjects().begin();
					 it != representation.getGeometricObjects().end();
					 it++)
			{
				(**it).getColors(color_strings_);

				if (representation.getDrawingMode() == DRAWING_MODE_WIREFRAME)
				{
					//wireframes_.insert((Mesh*) *it);
				}
			}

			HashSet<String>::ConstIterator cit = color_strings_.begin();
			for (; +cit; ++cit)
			{
				//color_map_.insert(ColorMap::ValueType(*cit, color_index_));
				//color_index_++;
			}

			representations_.push_back(&representation);

			return true;
		}

		String XML3DRenderer::getColorIndex_(const ColorRGBA& color)
		{
			String color_temp;
			color.get(color_temp);

			return String("c") + String(color_map_[color_temp]);
		}
		
		void XML3DRenderer::createTubeTransform_(const TwoColoredTube& tube)
		{
			Vector3 vec = tube.getVertex2() - tube.getVertex1();
      const double len = vec.getLength();
      const double angle = acos(vec.z / len); // the denominator accounts for the non-normalized rotation axis
      const float radius = tube.getRadius();

      Vector3 const& midpoint = tube.getVertex1();
      //Rotate the vector iround the normal
      vec /= sqrt(vec.x*vec.x + vec.y*vec.y);

      Matrix4x4 matrix = Matrix4x4::getIdentity();
      matrix.rotate(Angle(-angle), vec.y, -vec.x, 0);

      Matrix4x4 temp;
     // temp.setScale(radius, radius, len);
     // matrix*=temp;

      temp.setTranslation(midpoint);
      matrix = temp*matrix;

			std::ostream& out = *outfile_;
			
			out << "<transform ";
			out << "id=\"" << &tube << "trafo\" ";
			out << "translation=\"" << midpoint.x << " " << midpoint.y << " " << midpoint.z << "\" ";  
			//out << "center=\"" << midpoint.x << " " << midpoint.y << " " << midpoint.z << "\" ";  
			out << "rotation=\"" << vec.y  << " " << -vec.x << " " << 0 << " " << -angle << "\" ";  
			//Quaternion rotation(Vector3(vec.y, -vec.x, 0), -angle);
			//Vector3 rot_axis;
			//float rot_angle;
			//rotation.fromAxisAngle(rot_axis, rot_angle);
			//out << "rotation=\"" << rot_axis.x  << " " << rot_axis.y << " " << rot_axis.z << " " << -rot_angle << "\" ";  
			out << "scale=\"" << radius << " " << radius << " " << len << "\" ";  
			out << "/>" << endl;
		}

		void XML3DRenderer::renderLabel_(const Label& label)
		{
			std::ostream& out = *outfile_;

			out << "text{ ttf BALLLabelFont, \"" << label.getExpandedText() << "\",0.2, 0" << std::endl;
			//out << "  texture{ pigment{color rgb" << 	XML3DColorRGBA(label.getColor()) << " }"<< std::endl;
			out << "  finish{ambient 0.15 diffuse 0.85} } " << std::endl;
			out << "  matrix < ";
			for (Position pos = 0; pos < 9; pos++)
			{
				out << m_[pos] << ", ";
			}
			out << "0, 0, 0 >";
			out << " rotate 180*y rotate 180*x " << endl;
			out << "  translate < ";
			out << label.getVertex().x << ", ";
			out << label.getVertex().y << ", ";
			out << label.getVertex().z << " ";
			out << " > }" << std::endl;
		}

		void XML3DRenderer::renderMultiLine_(const MultiLine& line)
		{
			std::ostream& out = *outfile_;

			String last;
			HashSet<Position> used;
			vector<String> vertices;
			for (Position i = 0; i < line.vertices.size(); i++)
			{
				String now = XML3DVector3(line.vertices[i]);
				if (last == now) continue;
				last = now;
				vertices.push_back(now);
			}

			out << "#declare vertices = array[" << String(vertices.size()) << "] { ";
			for (Position i = 0; i < vertices.size(); i++)
			{
				out << vertices[i] << " ";
				if (i != vertices.size() - 1) out << ", ";
			}

			out << " } " << endl; 

			out << "#declare colors = array[" << String(vertices.size()) << "] { ";
			bool multi_colors = true;
			if (line.colors.size() == 1) multi_colors = false;
			for (Position i = 0; i < vertices.size(); i++)
			{
				if (used.has(i)) continue;

				if (multi_colors) out << getColorIndex_(line.colors[i]);
				else              out << getColorIndex_(line.colors[0]);

				if (i != vertices.size() - 1) out << ", ";
			}

			out << " } " << endl; 
			out << "ILine(" << String(vertices.size()) << ", vertices, colors)" << endl;
		}

	} // namespaces
}
