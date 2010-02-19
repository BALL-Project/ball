// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: XML3DRenderer.C,v 1.22.16.3 2007/04/18 21:06:17 amoll Exp $
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
				font_file_("/local/amoll/povray-3.5/include/crystal.ttf")
		{
		}

		XML3DRenderer::XML3DRenderer(const XML3DRenderer& renderer)
			: Renderer(renderer),
				outfile_(&std::cout),
				human_readable_(renderer.human_readable_)
		{
		}


		XML3DRenderer::XML3DRenderer(const String& name)
			throw(Exception::FileNotFound)
			: Renderer(),
				human_readable_(true),
				font_file_("/local/amoll/povray-3.5/include/crystal.ttf")
		{
			outfile_ = new File(name, std::ios::out);
		}

		XML3DRenderer::XML3DRenderer(std::ostream& out_stream)
			: Renderer(),
				human_readable_(true)
		{
			if (outfile_ != 0 && RTTI::isKindOf<File>(*outfile_)) 
			{
				delete outfile_;
			}
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

		void XML3DRenderer::setFileName(const String& name)
			throw(Exception::FileNotFound)
		{
			if (outfile_ == 0 || !RTTI::isKindOf<File>(*outfile_)) 
			{
				outfile_ = new File();
			}
			(*(File*)outfile_).open(name, std::ios::out);
		}

		void XML3DRenderer::setOstream(std::ostream& out_stream)
		{
			if (outfile_ != 0 && RTTI::isKindOf<File>(*outfile_)) 
			{
				delete outfile_;
			}
			outfile_ = &out_stream;
		}


		String XML3DRenderer::XML3DColorRGBA(const ColorRGBA& input)
		{
			String output = "<";
 			output += trimFloatValue_(input.getRed()) + ", ";
 			output += trimFloatValue_(input.getGreen()) + ", ";
 			output += trimFloatValue_(input.getBlue()) + ", ";
			// TODO: sensible parameter for "filter"
			output += "0., ";
			// TODO: transmit seems not to be linear in alpha
			output += trimFloatValue_(1. - (float) input.getAlpha());
			output += ">";

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
		
		String XML3DRenderer::XML3DVector3(Vector3 input)
		{
			String output = "<";
			output += trimFloatValue_(input.x) + ", ";
			output += trimFloatValue_(input.y) + ", ";
			output += trimFloatValue_(input.z);
			output += ">";
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

			wireframes_.clear();
			representations_.clear();
			color_map_.clear();
			color_strings_.clear();
			color_index_ = 0;

			std::ostream& out = *outfile_;

			if (!Renderer::init(stage, width, height)) return false;

			createXHTMLHeader();

			// initialize xml3d environment	
			out << "<xml3d id=\"MyXml3d\"" << endl;

			//set height and width of the scene
			out << "		style=\"width:" << int(width) << "px; ";
			out << "height: " << int(height) << "px;\"" << endl;

			
			
			out << "		xmlns=\"http://www.w3.org/2009/xml3d\"" << endl;
			out << "	    xmlns:xs=\"http://www.w3.org/2001/XMLSchema\"" << endl;
			out << "        xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"" << endl;
			out << "        xmlns:f=\"http://myfunctions.org\"" << endl;
			out << "        xmlns:cx3d=\"xalan://org.xml3d.converter.x3d.ConverterFunctions\"" << endl;
			out << "        xmlns:xlink=\"http://www.w3.org/1999/xlink\"" << endl;
			out << "        xsi:schemaLocation=\"http://www.w3.org/2009/xml3d ../../org.xml3d.spec/generated/xsd/xml3d.xsd\"" << endl;
			
			//Enable Debug
			//out << "webgl:showLog=\"true\"" << endl;
			
			// set Active View, standard name ist ballview_camera
			out << "activeView=\"ballview_camera\"" << endl;

			out << "		>" << endl;
			

			// Define BALLView Camera view
			out << "<view id=\"ballview_camera\" ";
			out << "position=\"" << stage.getCamera().getViewPoint().x << " "
													<< stage.getCamera().getViewPoint().y << " "
													<< stage.getCamera().getViewPoint().z << "\"";
			
			// Calculate rotation to align BALLview camera with standard XML3D camera
			Vector3 direction = stage.getCamera().getViewVector();
			
			cout << "Direction " << direction << endl;
			if (direction.getSquareLength() < 1e-6)
			{
				Log.error() << "Mist" << endl;
				return false;
			}
			direction.normalize();
			cout << "Direction norm. " << direction << endl;

			Vector3 d_null = Vector3(0,0,-1);
			Vector3 dir_rotation_axis = (d_null % direction);
			cout << "dir_rotation_axis " << dir_rotation_axis << endl;
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
				cout << "dir_rotation_axis norm. " << dir_rotation_axis << endl;
				dir_rotation_angle = acos(direction * d_null);
				cout << "angle " << dir_rotation_angle << endl;
			}
			
			Quaternion dir_rotation(dir_rotation_axis, dir_rotation_angle);
			Vector3 quataxis;
			float quatangle;
			dir_rotation.toAxisAngle(quataxis, quatangle);
			cout << "quattest " << quataxis << " " << quatangle << endl; 
			Matrix4x4 dir_rotation_matrix;
			dir_rotation.getRotationMatrix(dir_rotation_matrix);
			Vector3 testergebnis = dir_rotation_matrix * Vector3(0,0,-1);
			cout << "testergebnis " << testergebnis << endl;
			cout << "alte direction " << direction<< endl;
			
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
			Vector3 testergebnis2 = final_rotation_matrix * Vector3(0,0,-1);
			cout << "testergebnis2 " << testergebnis2 << endl;
			cout << "alte direction " << direction<< endl;
			Vector3 testergebnis3 = final_rotation_matrix * Vector3(0,1,0);
			cout << "testergebnis3 " << testergebnis3 << endl;
			cout << "old upvector " << upvector << endl;

			//out << " orientation=\""<< screw_axis.x << " "
			//												<< screw_axis.y << " "
			//												<< screw_axis.z << " "
			//												<< (-angle) << "\"" ;
			out << " orientation=\""<< final_rotation_axis.x << " "
															<< final_rotation_axis.y << " "
															<< final_rotation_axis.z << " "
															<< final_rotation_angle << "\"" ;
			
			out << " fieldOfView=\"" << 1.2 << "\"";
			out << " >";
			out << "</view>" << endl;



			return true;
		}

		bool XML3DRenderer::finish()
		{
			std::ostream& out = *outfile_;

				

			vector<const Representation*>::iterator rit = representations_.begin();
			for (; rit != representations_.end(); rit++)
			{
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
			//add some javascripts, caution relative paths

  		out << "<script type=\"text/javascript\" src=\"../../../local/nicste/xml3d-dev/org.xml3d.renderer.webgl/script/xml3d_scene_controller.js\"></script>" << endl; 
			out << "<script type=\"text/javascript\" src=\"../../../local/nicste/xml3d-dev/org.xml3d.renderer.webgl/script/xml3d_util.js\"></script>" << endl;
			out << "<script type=\"text/javascript\" src=\"../../../local/nicste/xml3d-dev/org.xml3d.renderer.webgl/script/xml3d_renderer.js\"></script>" << endl;
			out << "<script type=\"text/javascript\" src=\"../../../local/nicste/xml3d-dev/org.xml3d.renderer.webgl/script/xml3d_datatypes.js\"></script>" << endl;
			out << "<script type=\"text/javascript\" src=\"../../../local/nicste/xml3d-dev/org.xml3d.renderer.webgl/script/xml3d_classes.js\"></script>" << endl;
			out << "<script type=\"text/javascript\" src=\"../../../local/nicste/xml3d-dev/org.xml3d.renderer.webgl/script/xml3d.js\"></script>" << endl;
			
			
			createXHTMLFooter();	

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
			out << "    <link rel=\"stylesheet\" type=\"text/css\" href=\"default.css\" />" << endl;
			out << "  </head>" << endl;
			
			// Start XHTML Body
			out << "  <body>" << endl;
			out << "    <h1>Look here:</h1>" << endl;
		}

		void XML3DRenderer::createXHTMLFooter()
		{
			std::ostream& out = *outfile_;
			
			// End XHTML Body
			out << "  </body>" << endl;
			out << "</html>" << endl;
		}
		
		void XML3DRenderer::renderSphere_(const Sphere& sphere)
		{
			//std::ostream& out = *outfile_;

			//const ColorRGBA& color = getColor_(sphere);

			//if ((Size) color.getAlpha() == 255) out << "Sphere(";
			//else 																out << "SphereT(";

		  //out << XML3DVector3(sphere.getPosition()) << ", "
			//		<< sphere.getRadius() << ", "
			//		<< getColorIndex_(color) << ")" << endl;
		}

		void XML3DRenderer::renderDisc_(const Disc& disc)
		{
			//std::ostream& out = *outfile_;

			//const ColorRGBA& color = getColor_(disc);

			//// then, find out its radius, its normal, and its position
			//float radius;
			//Vector3 normal;
			//Vector3 position;
			//disc.getCircle().get(position, normal, radius);
			//normal -= origin_;

			//// now write the information into the out
			//out << "disc { ";
			//out << XML3DVector3(position) << ", ";
			//out << XML3DVector3(normal) << ", ";
			//out << radius;
			//out << " pigment { " << getColorIndex_(color) << " } ";
			//out << XML3DFinish("Tube", color); // We use the same finish as for tubes -> helices
			//out << "} " << endl;
		}

		void XML3DRenderer::renderLine_(const Line& line)
		{
			//std::ostream& out = *outfile_;

			//const ColorRGBA& color = getColor_(line);

			//String p1 = XML3DVector3(line.getVertex1());
			//String p2 = XML3DVector3(line.getVertex2());

			//if (p1 == p2) return;

			//if ((Size) color.getAlpha() == 255) out << "Tube(";
			//else 																out << "TubeT(";

		  //out << p1 << ", " << p2 << ", "
			//		<< BALLVIEW_XML3DRAY_LINE_RADIUS << ", "
			//		<< getColorIndex_(color) << ")" << endl;
		}

		void XML3DRenderer::renderTwoColoredLine_(const TwoColoredLine& tube)
		{
			//std::ostream& out = *outfile_;

			//// we have found a two colored tube
			//const ColorRGBA& color1 = tube.getColor();
			//const ColorRGBA& color2 = tube.getColor2();

			//String p1 = XML3DVector3(tube.getVertex1());
			//String p2 = XML3DVector3(tube.getMiddleVertex());

			//if (p1 != p2)
			//{
			//	if ((Size) color1.getAlpha() == 255) out << "Tube(";
			//	else 																 out << "TubeT(";
			//	
			//	out << p1 << ", "
			//			<< p2 << ", "
			//			<< BALLVIEW_XML3DRAY_LINE_RADIUS << ", "
			//			<< getColorIndex_(color1) << ")" << endl;
			//}

			//p1 = XML3DVector3(tube.getMiddleVertex());
			//p2 = XML3DVector3(tube.getVertex2());

			//if (p1 == p2) return;

  		//if ((Size) color1.getAlpha() == 255) out << "Tube(";
			//else 																 out << "TubeT(";
			//
		  //out << p1 << ", "
		  //    << p2 << ", "
			//		<< BALLVIEW_XML3DRAY_LINE_RADIUS << ", "
			//		<< getColorIndex_(color2) << ")" << endl;
		}


		void XML3DRenderer::renderTube_(const Tube& tube)
		{
			//std::ostream& out = *outfile_;

			//String p1 = XML3DVector3(tube.getVertex1());
			//String p2 = XML3DVector3(tube.getVertex2());

			//if (p1 == p2) return;

			//const ColorRGBA& color = getColor_(tube);

			//if ((Size) color.getAlpha() == 255) out << "Tube(";
			//else 																out << "TubeT(";

		  //out << p1 << ", " << p2 << ", "
			//		<< tube.getRadius() << ", "
			//		<< getColorIndex_(color) << ")" << endl;
		}	

		void XML3DRenderer::renderPoint_(const Point& point)
		{
			//std::ostream& out = *outfile_;

			//const ColorRGBA& color = getColor_(point);

			//if ((Size) color.getAlpha() == 255) out << "Sphere(";
			//else 																out << "SphereT(";

		  //out << XML3DVector3(point.getVertex()) << ", "
			//		<< BALLVIEW_XML3DRAY_LINE_RADIUS << ", "
			//		<< getColorIndex_(color) << ")" << endl;
		}


		void XML3DRenderer::renderTwoColoredTube_(const TwoColoredTube& tube)
		{
			//std::ostream& out = *outfile_;

			//const ColorRGBA& color1 = tube.getColor();
			//const ColorRGBA& color2 = tube.getColor2();

			//String p1 = XML3DVector3(tube.getVertex1());
			//String p2 = XML3DVector3(tube.getMiddleVertex());

			//if (p1 != p2)
			//{
			//	if ((Size) color1.getAlpha() == 255) out << "Tube(";
			//	else 																 out << "TubeT(";
			//	
			//	out << p1 << ", "
			//			<< p2 << ", "
			//			<< tube.getRadius() << ", "
			//			<< getColorIndex_(color1) << ")" << endl;
			//}

			//p1 = XML3DVector3(tube.getMiddleVertex());
			//p2 = XML3DVector3(tube.getVertex2());

			//if (p1 == p2) return;

  		//if ((Size) color1.getAlpha() == 255) out << "Tube(";
			//else 																 out << "TubeT(";
			//
		  //out << p1 << ", "
		  //    << p2 << ", "
			//		<< tube.getRadius() << ", "
			//		<< getColorIndex_(color2) << ")" << endl;
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
			// Start a triangle env	
			out << "<mesh "; 
			out << "type=\"triangles\"";
			out << ">" << endl; 

			// First the indices
			out << "<bind semantic=\"index\">" << endl;
			out << "<int>" << endl;
 			vector<Surface::Triangle>::const_iterator itt = mesh.triangle.begin();
 			
			for (; itt != mesh.triangle.end(); itt++)
   		{
				out << (*itt).v1 << " " << (*itt).v2 << " " << (*itt).v3 << " ";
			}	
			
			out << "</int>" << endl;
			out << "</bind>" << endl;

			// then then position vectors
			out << "<bind semantic=\"position\">" << endl;
			out << "<float3>" << endl;
 			
			vector<Surface::Vertex>::const_iterator itv = mesh.vertex.begin();
 			for (; itv != mesh.vertex.end(); itv++)
   		{
				out << (*itv).x << " " << (*itv).y << " " << (*itv).z << endl;
			}	
			
			out << "</float3>" << endl;;
			out << "</bind>" << endl;
			
			// then then position normals
			out << "<bind semantic=\"normal\">" << endl;
			out << "<float3>" << endl;
 			
			vector<Surface::Normal>::const_iterator itn = mesh.normal.begin();
 			for (; itn != mesh.normal.end(); itn++)
   		{
				out << (*itn).x << " " << (*itn).y << " " << (*itn).z << endl;
			}	
			
			out << "</float3>" << endl;
			out << "</bind>" << endl;
			
			out << "</mesh>" << endl;

			// then then position texcoord
			//out << "<bind semantic=\"texcoord\">" << endl;
			//out << "<\bind>" << endl;




			//// is this a mesh in wireframe mode?
			//if (wireframes_.has(&mesh))
			//{
			//	if (mesh.colors.size() == 0) return;

			//	String pre = "Wire(";

			//	String color_index = getColorIndex_(mesh.colors[0]);

			//	for (Position tri = 0; tri < mesh.triangle.size(); tri++)
			//	{
			//		String v1 = XML3DVector3(mesh.vertex[mesh.triangle[tri].v1]);
			//		String v2 = XML3DVector3(mesh.vertex[mesh.triangle[tri].v2]);
			//		String v3 = XML3DVector3(mesh.vertex[mesh.triangle[tri].v3]);

			//		if (v1 == v2 || v2 == v3 || v3 == v1) continue;

			//		out << pre << v1 << ", " << v2 << ", " << v3 << ", ";

			//		if (mesh.colors.size() > 1)
			//		{
			//			out	<< getColorIndex_(mesh.colors[mesh.triangle[tri].v1]) << ","
			//					<< getColorIndex_(mesh.colors[mesh.triangle[tri].v2]) << ","
			//					<< getColorIndex_(mesh.colors[mesh.triangle[tri].v3]) << ")" << endl;
			//		}
			//		else
			//		{
			//			out << color_index << "," << color_index << "," << color_index <<")" << std::endl;
			//		}
			//	}
			//	return;
			//}

			//
			//// draw BALL Mesh as XML3DRay mesh2
			//
			//out << "\tmesh2 {" << endl;
			//// write vertices ---->
			//out << "\t\tvertex_vectors {" << endl;
			//out << "\t\t\t" << mesh.vertex.size() << ","  << endl;
			//out << "\t\t\t";
			//for (Position i = 0; i < mesh.vertex.size() - 1; i++)
			//{
			//	out << XML3DVector3(mesh.vertex[i]) << ", ";
			//}
			//out << XML3DVector3(mesh.vertex[mesh.vertex.size() - 1]) << endl;
			//out << "\t\t}" << endl;

			//// write normals ---->
			//out << "\t\tnormal_vectors {" << endl;
			//out << "\t\t\t" << mesh.normal.size() << "," << endl;
			//out << "\t\t\t";
			//for (Position i = 0; i < mesh.normal.size() - 1; i++)
			//{
			//	out << XML3DVector3(mesh.normal[i]) << ", ";
			//}
			//out << XML3DVector3(mesh.normal[mesh.normal.size() - 1]) << endl;
			//out << "\t\t}" << endl;

			///////////////////////////////////////////////////
			//// calculate a hashset of all colors in the mesh
			///////////////////////////////////////////////////
			//
      //ColorMap colors;
      //vector<const ColorRGBA*> color_vector;
      //String color_string;
      //for (Position i = 0; i < mesh.colors.size(); i++)
      //{
      //  mesh.colors[i].get(color_string);
      //  if (!colors.has(color_string))
      //  {
      //    colors.insert(ColorMap::ValueType(color_string, colors.size()));
      //    color_vector.push_back(&mesh.colors[i]);
      //  }
      //}

			//// write colors of vertices ---->
			//out << "\t\ttexture_list{" << endl;
			//out << "\t\t\t" << colors.size()<< ","<< endl;

			//for (Position p = 0; p < colors.size(); p++)
			//{
			//	out << "texture { pigment { " << getColorIndex_(*color_vector[p]) << " }"
			//			<< " finish { BALLFinishMesh } }";

			//	if (p < colors.size() - 1) out << ",";

			//	out << endl;
			//}

			//out << "\t\t}" << endl;
			//
			//// write vertex indices ---->
			//out << "\t\tface_indices {" << endl;
			//out << "\t\t\t" << mesh.triangle.size() << ","<<  endl;
			//if (mesh.colors.size() == 1)
			//{
			//	out << "\t\t\t";
			//	for (Position i = 0; i < mesh.triangle.size(); i++)
			//	{
			//		out << "<";
			//		out << mesh.triangle[i].v1 << ", ";
			//		out << mesh.triangle[i].v2 << ", ";
			//		out << mesh.triangle[i].v3 << ", ";
			//		// color index
			//		out << "> " << 0 << endl;
			//	}
			//}
			//else
			//{
			//	String color_temp;
			//	for (Position i = 0; i < mesh.triangle.size(); i++)
			//	{
			//		out << "<";
			//		out << mesh.triangle[i].v1 << ", ";
			//		out << mesh.triangle[i].v2 << ", ";
			//		out << mesh.triangle[i].v3 << ">, ";
			//		// color index
			//		mesh.colors[mesh.triangle[i].v1].get(color_temp);
			//		out << colors[color_temp] << ", ";
			//		mesh.colors[mesh.triangle[i].v2].get(color_temp);
			//		out << colors[color_temp] << ", ";
			//		mesh.colors[mesh.triangle[i].v3].get(color_temp);
			//		out << colors[color_temp] << endl;
			//	}
			//}
			//out << "\t\t}" << endl;
			//out << "\t inside_vector <0, 0, 1>" << endl;
			//out << "hollow" << endl;
			//out << "\t}" << endl;
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

		void XML3DRenderer::renderLabel_(const Label& label)
		{
			std::ostream& out = *outfile_;

			out << "text{ ttf BALLLabelFont, \"" << label.getExpandedText() << "\",0.2, 0" << std::endl;
			out << "  texture{ pigment{color rgb" << 	XML3DColorRGBA(label.getColor()) << " }"<< std::endl;
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
