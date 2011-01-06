// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: POVRenderer.C,v 1.22.16.3 2007/04/18 21:06:17 amoll Exp $
//

#include <BALL/VIEW/RENDERING/RENDERERS/POVRenderer.h>
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

#define BALLVIEW_POVRAY_LINE_RADIUS "BALL_LINE_RADIUS"

		POVRenderer::POVRenderer()
			: Renderer(),
				outfile_(&std::cout),
				human_readable_(true),
				font_file_("/local/amoll/povray-3.5/include/crystal.ttf")
		{
		}

		POVRenderer::POVRenderer(const POVRenderer& renderer)
			: Renderer(renderer),
				outfile_(&std::cout),
				human_readable_(renderer.human_readable_)
		{
		}


		POVRenderer::POVRenderer(const String& name)
			throw(Exception::FileNotFound)
			: Renderer(),
				human_readable_(true),
				font_file_("/local/amoll/povray-3.5/include/crystal.ttf")
		{
			outfile_ = new File(name, std::ios::out);
		}

		POVRenderer::~POVRenderer()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << this << " of class POVRenderer" << endl;
			#endif

				clear();
		}

		void POVRenderer::clear()
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

		void POVRenderer::setFileName(const String& name)
			throw(Exception::FileNotFound)
		{
			if (outfile_ == 0 || !RTTI::isKindOf<File>(*outfile_)) 
			{
				outfile_ = new File();
			}
			(*(File*)outfile_).open(name, std::ios::out);
		}

		void POVRenderer::setOstream(std::ostream& out_stream)
		{
			if (outfile_ != 0 && RTTI::isKindOf<File>(*outfile_)) 
			{
				delete outfile_;
			}
			outfile_ = &out_stream;
		}


		String POVRenderer::POVColorRGBA(const ColorRGBA& input)
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

		String POVRenderer::POVFinish(const String& object, const ColorRGBA& input)
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
		
		String POVRenderer::POVVector3(Vector3 input)
		{
			String output = "<";
			output += trimFloatValue_(input.x) + ", ";
			output += trimFloatValue_(input.y) + ", ";
			output += trimFloatValue_(input.z);
			output += ">";
			return output;
		}

		String POVRenderer::trimFloatValue_(float value)
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

		// init must be called right before the rendering starts, since
		// we need to fix the camera, light sources, etc...
		bool POVRenderer::init(const Stage& stage, float width, float height)
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Start the POVRender output..." << endl;
			#endif

			wireframes_.clear();
			representations_.clear();
			color_map_.clear();
			color_strings_.clear();
			color_index_ = 0;

			std::ostream& out = *outfile_;

			if (!Renderer::init(stage, width, height)) return false;

			if (human_readable_)
			{
				out	<< "// POVRay file created by the BALL POVRenderer" << endl << endl
				    << "// Width of the original scene: " << width_ << endl
				    << "// Height of the original scene: " << height_ << endl
				    << "// To render this scene, call povray (available from www.povray.org)"
					  << " like this:\n//" << endl;

				if (outfile_ != 0 && RTTI::isKindOf<File>(*outfile_))
				{
					// Add a command line with the correct options to call POVRay to the header
					// so we can just copy&paste this to render this file.
					String infilename = FileSystem::baseName((*(File*)outfile_).getName());
					String outfilename(infilename);
					if (outfilename.hasSuffix(".pov"))
					{
						outfilename.getSubstring(-4) = ".png";
					}
					// +QR: highest quiality
					// +A0.3 : antialiasing
					// +FN: PNG format as the default
					// +W/+H: width and height, taken from the widget.
					out << "// povray +I" << infilename 
							<< " +FN +O" << outfilename << " +Q9 +W" << width_ 
							<< " +H" << height_ << " +A0.3\n//" << endl;
				}
			}

		
			out << "camera {" << std::setprecision(12) << endl;
			out << "\t location " << POVVector3(stage.getCamera().getViewPoint()) << endl;

			Angle fovx(105, false);
			float ratio = 1;

			if (Scene::getInstance(0) != 0)
			{
				// matrix for the Projection matrix 	
				GLdouble projection_matrix[16];

				// take the Projection matrix	
				glGetDoublev(GL_PROJECTION_MATRIX, projection_matrix);

				// determine the projection variables
				if(projection_matrix[0]==0. || projection_matrix[5]==0. || projection_matrix[10]==1.)
				{	
					Log.error() << "Projection variables equal zero! " << endl;
					return false;
				}	
				float nearv   = projection_matrix[14]/(projection_matrix[10]-1);
				float left   = projection_matrix[14]*(projection_matrix[8]-1) / (projection_matrix[0]*(projection_matrix[10]-1));
				float bottom = projection_matrix[14]*(projection_matrix[9]-1) / (projection_matrix[5]*(projection_matrix[10]-1));
				float top    = projection_matrix[14]*(projection_matrix[9]+1) / (projection_matrix[5]*(projection_matrix[10]-1));

				ratio = left / bottom;
				fovx.set(2*atan(ratio*(top-bottom)/(2.*nearv)));
			}
			
			out << "\t up y" << endl;
			out << "\t right -" << ratio << "*x" << endl;
			out << "\t angle " << fovx.toDegree() << endl;
			out << "\t sky " << POVVector3(stage.getCamera().getLookUpVector()) << endl;
			out << "\t look_at " << POVVector3(stage.getCamera().getLookAtPosition()) << endl;
			out << "}" << std::setprecision(6) << endl << endl;

			//
			if (human_readable_)
			{
				out << "// look up: " << stage.getCamera().getLookUpVector() << endl;
				out << "// look at: " << stage.getCamera().getLookAtPosition() << endl;
				out << "// view point: " << stage.getCamera().getViewPoint() << endl;
			}

			// Set the light sources
			list<LightSource>::const_iterator it = stage_->getLightSources().begin();
			for (; it!=stage_->getLightSources().end(); ++it)
			{
				// first, set up the color of the light source
				float light_rgba[4];

				light_rgba[0] = ((float)it->getColor().getRed()) * it->getIntensity();
				light_rgba[1] = ((float)it->getColor().getGreen()) * it->getIntensity();
				light_rgba[2] = ((float)it->getColor().getBlue())  * it->getIntensity();
				light_rgba[3] = ((float)it->getColor().getAlpha());
				
				
				ColorRGBA light_col(light_rgba[0], light_rgba[1], light_rgba[2], light_rgba[3]);
				
				if (it->getType() == LightSource::AMBIENT)
				{
					out << "global_settings { ambient_light " << POVColorRGBA(light_col) << " }" << endl;
					continue;
				}

				out << "light_source { ";

				if (it->getType() == LightSource::POSITIONAL)
				{
					Vector3 pos = it->getPosition();
					if (it->isRelativeToCamera())
					{
						pos = stage_->calculateAbsoluteCoordinates(pos) + stage_->getCamera().getViewPoint();
					}
					out << POVVector3(pos) << ", " << POVColorRGBA(light_col) << "}" << endl;
				}
				else
				{
					// directional light sources
					Vector3 dir = it->getDirection();
					if (it->isRelativeToCamera())
					{
						dir = stage_->calculateAbsoluteCoordinates(dir);
					}

					out << POVVector3(dir * -1000) << ", " << POVColorRGBA(light_col) 
							<< " parallel point_at " << POVVector3(dir) << "}" << endl;
				}
			}
			
			// Add some global blurb for radiosity support and max trace level
			out << "global_settings { max_trace_level 99 radiosity { brightness 0.6 } }" << endl;

			// Set the background color
			out << "background { " << POVColorRGBA(stage_->getBackgroundColor()) << " }" << endl << endl;

			// Define the finish we will use for our molecular objects (defining the molecular
			// "material properties"
			// TODO: allow for more than one finish in order to have seperate parameters for different objects
			out << "#declare BALLFinish            		 = finish { ";

			// stage uses opengl values for material parameters (-1.0 -> 1.0), so normalize these
			out << "specular " 	<< stage.getSpecularIntensity() / 2.0 + 0.5 << " ";

			// shininess   0 -> roughness: 0.1
			// shininess 128 -> roughness: 0.01
			float r = 0.1 - ((stage.getShininess() / 128.0) * 0.09);
			out << "roughness " << r << " ";
			
			out << "diffuse " 	<< stage.getDiffuseIntensity() << " ";

			// povray uses an other ambient setting
			out << "ambient "  << stage.getAmbientIntensity() << " }"	 	<< endl;

			out << "#declare BALLFinishSphereSolid      = BALLFinish" << endl;
			out << "#declare BALLFinishSphereTransp     = BALLFinish" << endl;
			out << "#declare BALLFinishTubeSolid        = BALLFinish" << endl;
			out << "#declare BALLFinishTubeTransp       = BALLFinish" << endl;
			out << "#declare BALLFinishMesh             = BALLFinish" << endl;
			out << "#declare BALLFinishWire             = BALLFinish" << endl;
			out << "#declare BALL_WIRE_RADIUS 					= 0.01;" << std::endl;
			out << "#declare BALL_LINE_RADIUS 					= 0.02;" << std::endl;
			out << "// enter the path to your desired font here: " << std::endl;
			out << "#declare BALLLabelFont              = \"" << font_file_ << "\";" << std::endl;
			out << std::endl;
						
			out << "#macro Sphere(Position, Radius, Color)" << endl;
			out << "sphere { Position, Radius pigment { Color } finish { BALLFinishSphereSolid } }" << endl;
			out << "#end" << endl << endl;

			out << "#macro SphereT(Position, Radius, Color)" << endl;
			out << "sphere { Position, Radius pigment { Color } finish { BALLFinishSphereTransp} }" << endl;
			out << "#end" << endl << endl;

			out << "#macro Tube(Position1, Position2, Radius, Color)" << endl;
			out << "cylinder { Position1, Position2, Radius pigment { Color } finish { BALLFinishTubeSolid } }" << endl;
			out << "#end" << endl << endl;

			out << "#macro TubeT(Position1, Position2, Radius, Color)" << endl;
			out << "cylinder { Position1, Position2, Radius pigment { Color } finish { BALLFinishTubeTransp } }" << endl;
			out << "#end" << endl << endl;

			out << "// open Tube" << endl;
			out << "#macro OT(Position1, Radius, Color)" << endl;
			out << "cylinder { last_position, Position1, Radius open pigment { Color } finish { BALLFinishTubeSolid } }" << endl;
			out << "#define last_position = Position1" << endl;
			
			out << "#end" << endl << endl;

			out << "// open Tube transparent" << endl;
			out << "#macro OTT(Position1, Position2, Radius, Color)" << endl;
			out << "cylinder { Position1, Position2, Radius open pigment { Color } finish { BALLFinishTubeTransp } }" << endl;
			out << "#end" << endl << endl;


			out << "#macro Wire(Position1, Position2, Position3, Color1, Color2, Color3)" << endl;
			out << "cylinder { Position1, Position2, BALL_WIRE_RADIUS pigment { Color1 } finish { BALLFinishWire} }" << endl;
			out << "cylinder { Position2, Position3, BALL_WIRE_RADIUS pigment { Color2 } finish { BALLFinishWire} }" << endl;
			out << "cylinder { Position3, Position1, BALL_WIRE_RADIUS pigment { Color3 } finish { BALLFinishWire} }" << endl;
			out << "#end" << endl << endl;

			out << "\n\
#macro ILine(Size, Points, Colors)\n\
  #local colors = array[Size]\n\
  #local colors = Colors\n\
  #local points = array[Size]\n\
  #local points = Points\n\
  #local Index = 1;\n\
  #local point = points[0];\n\
  #while(Index < Size - 1)\n\
		cylinder { point, points[Index], BALL_LINE_RADIUS open pigment { colors[Index] } finish { BALLFinishTubeSolid } }\n\
		#declare point = (points[Index] + points[Index+1]) / 2.0;\n\
		cylinder { points[Index], point, BALL_LINE_RADIUS open pigment { colors[Index] } finish { BALLFinishTubeSolid } }\n\
		#declare Index = Index + 1;\n\
  #end\n\
#end\n\
		" << endl << endl;

			return true;
		}

		bool POVRenderer::finish()
		{
			std::ostream& out = *outfile_;

			// write all colors
			ColorMap::ConstIterator cit = color_map_.begin();
			for (; cit != color_map_.end(); ++cit)
			{
				out << "#declare c" << cit->second << " = " << POVColorRGBA(ColorRGBA(cit->first)) << ";" << endl;
			}

			out << endl;

			MainControl* mc = getMainControl();
			vector<ClippingPlane*> vc;
			if (mc) vc = mc->getRepresentationManager().getClippingPlanes();
			
			// write data for all Representations in an own union
			vector<const Representation*>::iterator rit = representations_.begin();
			for (; rit != representations_.end(); rit++)
			{
				// give the user some hints what this triangle soup once was
				out << "// Representation " << (*rit)->getName() << std::endl;

				// determine all clipping and capping planes for this representation
				vector<ClippingPlane*> clipped_by;
				vector<ClippingPlane*> capped_by;

				vector<ClippingPlane*>::const_iterator plane_it = vc.begin();
				for (;plane_it != vc.end(); plane_it++)
				{
					ClippingPlane& plane = **plane_it;

					if (!plane.isActive() ||
							plane.isHidden())
					{
						continue;
					}

					if (plane.getRepresentations().has((Representation*)*rit))
					{
						if (plane.cappingEnabled())
							capped_by.push_back(&plane);
						else
							clipped_by.push_back(&plane);
					}
				} // all clipping planes

				// if there is at least one capping plane active for this representation, we will need
				// a CSG intersection
				if (capped_by.size() > 0)
				{
					out << "intersection {" << endl;
				}

				// if we have more than one object in the representation we now begin a CSG 
				// union containing all the geometric objects of this rep
				if (((*rit)->getGeometricObjects().size() > 1) || (clipped_by.size() > 0))
				{
					out << "union {" << endl;
				}

				list<GeometricObject*>::const_iterator it;
				for (it =  (*rit)->getGeometricObjects().begin();
						 it != (*rit)->getGeometricObjects().end();
						 it++)
				{
					render_(*it);
				}

				// put all clipping planes in the correct location
				if (clipped_by.size() > 0)
				{
					out << "  clipped_by{" << endl;
				}

				for (Position i=0; i<clipped_by.size(); ++i)
				{
					ClippingPlane& plane = *clipped_by[i];

					out << "   plane{< -"  // negate normal vector
							<< plane.getNormal().x << ", -" 
							<< plane.getNormal().y << ", -" 
							<< plane.getNormal().z << ">, "
							<< plane.getDistance()
							<< "  }" << endl;
				}

				if (clipped_by.size() > 0)
				{
					out << " }" << endl
						  << "}" << endl;
				}

				if ((*rit)->getGeometricObjects().size() > 1)
				{
					out << "}" << endl; // union
				}

				// now put all the capping planes
				for (Position i=0; i<capped_by.size(); ++i)
				{
					ClippingPlane& plane = *capped_by[i];

					out << " plane {< -" // negate normal vector
							<< plane.getNormal().x << ", -" 
							<< plane.getNormal().y << ", -" 
							<< plane.getNormal().z << ">, "
							<< plane.getDistance() << " "
							<< "pigment { color rgb" << POVColorRGBA(plane.getCappingColor()) << " }"
							<< "  }" << endl;
				}

				if (capped_by.size() > 0)
				{
					out << "}" << endl;
				}
			} // all Representations


			if (outfile_ != 0 && RTTI::isKindOf<File>(*outfile_))
			{
				(*(File*)outfile_).close();
			}

			return true;
		}

		void POVRenderer::renderSphere_(const Sphere& sphere)
		{
			std::ostream& out = *outfile_;

			const ColorRGBA& color = getColor_(sphere);

			if ((Size) color.getAlpha() == 255) out << "Sphere(";
			else 																out << "SphereT(";

		  out << POVVector3(sphere.getPosition()) << ", "
					<< sphere.getRadius() << ", "
					<< getColorIndex_(color) << ")" << endl;
		}

		void POVRenderer::renderDisc_(const Disc& disc)
		{
			std::ostream& out = *outfile_;

			const ColorRGBA& color = getColor_(disc);

			// then, find out its radius, its normal, and its position
			float radius;
			Vector3 normal;
			Vector3 position;
			disc.getCircle().get(position, normal, radius);
			normal -= origin_;

			// now write the information into the out
			out << "disc { ";
			out << POVVector3(position) << ", ";
			out << POVVector3(normal) << ", ";
			out << radius;
			out << " pigment { " << getColorIndex_(color) << " } ";
			out << POVFinish("Tube", color); // We use the same finish as for tubes -> helices
			out << "} " << endl;
		}

		void POVRenderer::renderLine_(const Line& line)
		{
			std::ostream& out = *outfile_;

			const ColorRGBA& color = getColor_(line);

			String p1 = POVVector3(line.getVertex1());
			String p2 = POVVector3(line.getVertex2());

			if (p1 == p2) return;

			if ((Size) color.getAlpha() == 255) out << "Tube(";
			else 																out << "TubeT(";

		  out << p1 << ", " << p2 << ", "
					<< BALLVIEW_POVRAY_LINE_RADIUS << ", "
					<< getColorIndex_(color) << ")" << endl;
		}

		void POVRenderer::renderTwoColoredLine_(const TwoColoredLine& tube)
		{
			std::ostream& out = *outfile_;

			// we have found a two colored tube
			const ColorRGBA& color1 = tube.getColor();
			const ColorRGBA& color2 = tube.getColor2();

			String p1 = POVVector3(tube.getVertex1());
			String p2 = POVVector3(tube.getMiddleVertex());

			if (p1 != p2)
			{
				if ((Size) color1.getAlpha() == 255) out << "Tube(";
				else 																 out << "TubeT(";
				
				out << p1 << ", "
						<< p2 << ", "
						<< BALLVIEW_POVRAY_LINE_RADIUS << ", "
						<< getColorIndex_(color1) << ")" << endl;
			}

			p1 = POVVector3(tube.getMiddleVertex());
			p2 = POVVector3(tube.getVertex2());

			if (p1 == p2) return;

  		if ((Size) color1.getAlpha() == 255) out << "Tube(";
			else 																 out << "TubeT(";
			
		  out << p1 << ", "
		      << p2 << ", "
					<< BALLVIEW_POVRAY_LINE_RADIUS << ", "
					<< getColorIndex_(color2) << ")" << endl;
		}


		void POVRenderer::renderTube_(const Tube& tube)
		{
			std::ostream& out = *outfile_;

			String p1 = POVVector3(tube.getVertex1());
			String p2 = POVVector3(tube.getVertex2());

			if (p1 == p2) return;

			const ColorRGBA& color = getColor_(tube);

			if ((Size) color.getAlpha() == 255) out << "Tube(";
			else 																out << "TubeT(";

		  out << p1 << ", " << p2 << ", "
					<< tube.getRadius() << ", "
					<< getColorIndex_(color) << ")" << endl;
		}	

		void POVRenderer::renderPoint_(const Point& point)
		{
			std::ostream& out = *outfile_;

			const ColorRGBA& color = getColor_(point);

			if ((Size) color.getAlpha() == 255) out << "Sphere(";
			else 																out << "SphereT(";

		  out << POVVector3(point.getVertex()) << ", "
					<< BALLVIEW_POVRAY_LINE_RADIUS << ", "
					<< getColorIndex_(color) << ")" << endl;
		}


		void POVRenderer::renderTwoColoredTube_(const TwoColoredTube& tube)
		{
			std::ostream& out = *outfile_;

			const ColorRGBA& color1 = tube.getColor();
			const ColorRGBA& color2 = tube.getColor2();

			String p1 = POVVector3(tube.getVertex1());
			String p2 = POVVector3(tube.getMiddleVertex());

			if (p1 != p2)
			{
				if ((Size) color1.getAlpha() == 255) out << "Tube(";
				else 																 out << "TubeT(";
				
				out << p1 << ", "
						<< p2 << ", "
						<< tube.getRadius() << ", "
						<< getColorIndex_(color1) << ")" << endl;
			}

			p1 = POVVector3(tube.getMiddleVertex());
			p2 = POVVector3(tube.getVertex2());

			if (p1 == p2) return;

  		if ((Size) color1.getAlpha() == 255) out << "Tube(";
			else 																 out << "TubeT(";
			
		  out << p1 << ", "
		      << p2 << ", "
					<< tube.getRadius() << ", "
					<< getColorIndex_(color2) << ")" << endl;
		}

		void POVRenderer::renderMesh_(const Mesh& mesh)
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


			// is this a mesh in wireframe mode?
			if (wireframes_.has(&mesh))
			{
				if (mesh.colors.size() == 0) return;

				String pre = "Wire(";

				String color_index = getColorIndex_(mesh.colors[0]);

				for (Position tri = 0; tri < mesh.triangle.size(); tri++)
				{
					String v1 = POVVector3(mesh.vertex[mesh.triangle[tri].v1]);
					String v2 = POVVector3(mesh.vertex[mesh.triangle[tri].v2]);
					String v3 = POVVector3(mesh.vertex[mesh.triangle[tri].v3]);

					if (v1 == v2 || v2 == v3 || v3 == v1) continue;

					out << pre << v1 << ", " << v2 << ", " << v3 << ", ";

					if (mesh.colors.size() > 1)
					{
						out	<< getColorIndex_(mesh.colors[mesh.triangle[tri].v1]) << ","
								<< getColorIndex_(mesh.colors[mesh.triangle[tri].v2]) << ","
								<< getColorIndex_(mesh.colors[mesh.triangle[tri].v3]) << ")" << endl;
					}
					else
					{
						out << color_index << "," << color_index << "," << color_index <<")" << std::endl;
					}
				}
				return;
			}

			
			// draw BALL Mesh as POVRay mesh2
			
			out << "\tmesh2 {" << endl;
			// write vertices ---->
			out << "\t\tvertex_vectors {" << endl;
			out << "\t\t\t" << mesh.vertex.size() << ","  << endl;
			out << "\t\t\t";
			for (Position i = 0; i < mesh.vertex.size() - 1; i++)
			{
				out << POVVector3(mesh.vertex[i]) << ", ";
			}
			out << POVVector3(mesh.vertex[mesh.vertex.size() - 1]) << endl;
			out << "\t\t}" << endl;

			// write normals ---->
			out << "\t\tnormal_vectors {" << endl;
			out << "\t\t\t" << mesh.normal.size() << "," << endl;
			out << "\t\t\t";
			for (Position i = 0; i < mesh.normal.size() - 1; i++)
			{
				out << POVVector3(mesh.normal[i]) << ", ";
			}
			out << POVVector3(mesh.normal[mesh.normal.size() - 1]) << endl;
			out << "\t\t}" << endl;

			/////////////////////////////////////////////////
			// calculate a hashset of all colors in the mesh
			/////////////////////////////////////////////////
			
      ColorMap colors;
      vector<const ColorRGBA*> color_vector;
      String color_string;
      for (Position i = 0; i < mesh.colors.size(); i++)
      {
        mesh.colors[i].get(color_string);
        if (!colors.has(color_string))
        {
          colors.insert(ColorMap::ValueType(color_string, colors.size()));
          color_vector.push_back(&mesh.colors[i]);
        }
      }

			// write colors of vertices ---->
			out << "\t\ttexture_list{" << endl;
			out << "\t\t\t" << colors.size()<< ","<< endl;

			for (Position p = 0; p < colors.size(); p++)
			{
				out << "texture { pigment { " << getColorIndex_(*color_vector[p]) << " }"
						<< " finish { BALLFinishMesh } }";

				if (p < colors.size() - 1) out << ",";

				out << endl;
			}

			out << "\t\t}" << endl;
			
			// write vertex indices ---->
			out << "\t\tface_indices {" << endl;
			out << "\t\t\t" << mesh.triangle.size() << ","<<  endl;
			if (mesh.colors.size() == 1)
			{
				out << "\t\t\t";
				for (Position i = 0; i < mesh.triangle.size(); i++)
				{
					out << "<";
					out << mesh.triangle[i].v1 << ", ";
					out << mesh.triangle[i].v2 << ", ";
					out << mesh.triangle[i].v3 << ", ";
					// color index
					out << "> " << 0 << endl;
				}
			}
			else
			{
				String color_temp;
				for (Position i = 0; i < mesh.triangle.size(); i++)
				{
					out << "<";
					out << mesh.triangle[i].v1 << ", ";
					out << mesh.triangle[i].v2 << ", ";
					out << mesh.triangle[i].v3 << ">, ";
					// color index
					mesh.colors[mesh.triangle[i].v1].get(color_temp);
					out << colors[color_temp] << ", ";
					mesh.colors[mesh.triangle[i].v2].get(color_temp);
					out << colors[color_temp] << ", ";
					mesh.colors[mesh.triangle[i].v3].get(color_temp);
					out << colors[color_temp] << endl;
				}
			}
			out << "\t\t}" << endl;
			out << "\t inside_vector <0, 0, 1>" << endl;
			out << "hollow" << endl;
			out << "\t}" << endl;
		}
				
		const ColorRGBA& POVRenderer::getColor_(const GeometricObject& object)
		{
			if ((object.getComposite()) && object.getComposite()->isSelected())
			{
				return BALL_SELECTED_COLOR;
			}
			
			return object.getColor();
		}
		
		bool POVRenderer::renderOneRepresentation(const Representation& representation)
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
					wireframes_.insert((Mesh*) *it);
				}
			}

			HashSet<String>::ConstIterator cit = color_strings_.begin();
			for (; +cit; ++cit)
			{
				color_map_.insert(ColorMap::ValueType(*cit, color_index_));
				color_index_++;
			}

			representations_.push_back(&representation);

			return true;
		}

		String POVRenderer::getColorIndex_(const ColorRGBA& color)
		{
			String color_temp;
			color.get(color_temp);

			return String("c") + String(color_map_[color_temp]);
		}

		void POVRenderer::renderLabel_(const Label& label)
		{
			std::ostream& out = *outfile_;

			out << "text{ ttf BALLLabelFont, \"" << label.getExpandedText() << "\",0.2, 0" << std::endl;
			out << "  texture{ pigment{color rgb" << 	POVColorRGBA(label.getColor()) << " }"<< std::endl;
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

		void POVRenderer::renderMultiLine_(const MultiLine& line)
		{
			std::ostream& out = *outfile_;

			String last;
			HashSet<Position> used;
			vector<String> vertices;
			for (Position i = 0; i < line.vertices.size(); i++)
			{
				String now = POVVector3(line.vertices[i]);
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
