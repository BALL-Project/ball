// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: POVRenderer.C,v 1.19.4.16 2005/11/09 18:05:49 amoll Exp $
//

#include <BALL/VIEW/RENDERING/POVRenderer.h>
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


using std::endl;

namespace BALL
{
	namespace VIEW
	{

#define BALLVIEW_POVRAY_LINE_RADIUS "BALL_LINE_RADIUS"

		POVRenderer::POVRenderer()
			throw()
			: Renderer(),
				outfile_(&std::cout),
				human_readable_(true),
				font_file_("/local/amoll/povray-3.5/include/crystal.ttf")
		{
		}

		POVRenderer::POVRenderer(const POVRenderer& renderer)
			throw()
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
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << this << " of class POVRenderer" << endl;
			#endif

				clear();
		}

		void POVRenderer::clear()
			throw()
		{
			if (outfile_ != 0 && RTTI::isKindOf<File>(*outfile_))
			{
				delete outfile_;
			}

			outfile_ = &std::cout;
			human_readable_ = true;

			representations_.clear();
			color_map_.clear();
			color_vector_.clear();
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
			throw()
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
			throw()
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
			throw()
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
					output = output.left(p + 3);
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
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Start the POVRender output..." << endl;
			#endif

			wireframes_.clear();
			representations_.clear();
			color_map_.clear();
			color_vector_.clear();

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
					// -UV: due to problems with the orthogonality of the camera vectors
					// +FN: PNG format as the default
					// +W/+H: width and height, taken from the widget.
					out << "// povray +I" << infilename 
							<< " +FN +O" << outfilename << " +Q9 +W" << width_ 
							<< " +H" << height_ << " -UV +A0.3\n//" << endl;
				}
			}
			out << "camera {" << std::setprecision(12) << endl
			    << "\tperspective" << endl
			    << "\tdirection <0.0, 0.0, -1.0>" << endl
			    << "\tright " << (double)width_ / (double)height_ << " * x" << endl
			    << "\tangle 83.0" << endl
			    << "\ttransform {" << endl
			    << "\t\tmatrix <" << endl;

 			GLdouble m[16];	
			glGetDoublev(GL_MODELVIEW_MATRIX, m);
			
			double norm = sqrt(m[0] * m[0] + m[1] * m[1] + m[2] * m[2]);
			out << "\t\t" << m[0] / norm << ",  " << m[1] / norm << ", " << m[2] / norm << "," << endl;
			m_[0] = m[0] / norm;
			m_[1] = m[1] / norm;
			m_[2] = m[2] / norm;
			norm = sqrt(m[4] * m[4] + m[5] * m[5] + m[6] * m[6]);
			out << "\t\t" << m[4] / norm << ",  " << m[5] / norm << ", " << m[6] / norm << "," << endl;

			m_[3] = m[4] / norm;
			m_[4] = m[5] / norm;
			m_[5] = m[6] / norm;
			norm = sqrt(m[8] * m[8] + m[9] * m[9] + m[10] * m[10]);
			out << "\t\t" << m[8] / norm << ",  " << m[9] / norm << ", " << m[10] / norm << "," << endl;

			m_[6] = m[8] / norm;
			m_[7] = m[9] / norm;
			m_[8] = m[10] / norm;
			out << "\t\t" << m[12] << ",  " << m[13] << ", " << m[14] << endl;
			m_[9] = m[12];
			m_[10] = m[13];
			m_[11] = m[14];
		  out << "\t\t>" << endl;
			out << "\tinverse }" << endl;
			out << "}" << std::setprecision(6) << endl << endl;
				
			//
			if (human_readable_)
			{
				out << "// look up: " << stage.getCamera().getLookUpVector() << endl;
				out << "// look at: " << stage.getCamera().getLookAtPosition() << endl;
				out << "// view point: " << stage.getCamera().getViewPoint() << endl;
			}

			// Set the light sources
			List<LightSource>::ConstIterator it = stage_->getLightSources().begin();
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
			
			// Add some global blurb for radiosity support
			out << "global_settings { radiosity { brightness 0.6 } }" << endl;

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
			
			out << "diffuse " 	<< stage.getDiffuseIntensity() 	/ 2.0 + 0.5 << " ";

			// povray uses an other ambient setting
			out << "ambient 0.0 }"	 	<< endl;

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

			out << "#macro Wire(Position1, Position2, Position3, Color1, Color2, Color3)" << endl;
			out << "cylinder { Position1, Position2, BALL_WIRE_RADIUS pigment { Color1 } finish { BALLFinishWire} }" << endl;
			out << "cylinder { Position2, Position3, BALL_WIRE_RADIUS pigment { Color2 } finish { BALLFinishWire} }" << endl;
			out << "cylinder { Position3, Position1, BALL_WIRE_RADIUS pigment { Color3 } finish { BALLFinishWire} }" << endl;
			out << "#end" << endl << endl;


			return true;
		}

		bool POVRenderer::finish()
			throw()
		{
			std::ostream& out = *outfile_;

			for (Position p = 0; p < color_vector_.size(); p++)
			{
				out << "#declare c" << p << " = " << POVColorRGBA(*color_vector_[p]) << ";" << endl;
			}

			out << endl;
			

			vector<const Representation*>::iterator rit = representations_.begin();

			for (; rit != representations_.end(); rit++)
			{
				
				// now begin the CSG union containing all the geometric objects of this rep
				out << "union {" << endl;

				List<GeometricObject*>::ConstIterator it;
				for (it =  (*rit)->getGeometricObjects().begin();
						 it != (*rit)->getGeometricObjects().end();
						 it++)
				{
					render_(*it);
				}

				const vector<ClippingPlane*>& vc = getMainControl()->getPrimitiveManager().getClippingPlanes();
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

						out << "  clipped_by{" << endl
										 << "   plane{< -"  // negate normal vector
										 << plane.getNormal().x << ", -" 
										 << plane.getNormal().y << ", -" 
										 << plane.getNormal().z << ">, "
										 << plane.getDistance()
										 << "  }" << endl
										 << " }" << endl;
					}
				} // all clipping planes

				out << "}" << endl; // union

			} // all Representations


			if (outfile_ != 0 && RTTI::isKindOf<File>(*outfile_))
			{
				(*(File*)outfile_).close();
			}

			return true;
		}

		void POVRenderer::renderSphere_(const Sphere& sphere)
			throw()
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
			throw()
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
			throw()
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
			throw()
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
			throw()
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
			throw()
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
			throw()
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
			throw()
		{
			if (mesh.vertex.size() == 0 ||
			    mesh.normal.size() == 0 ||
					mesh.triangle.size() == 0)
			{
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
			out << "\t\t\t" << colors.size() + 1<< ","<< endl;

			ColorRGBA temp_color;
			for (Position p = 0; p < color_vector.size(); p++)
			{
   			temp_color.set((*color_vector[p]));
				out << "texture { pigment { " << getColorIndex_(temp_color) << " }"
						<< " finish { BALLFinishMesh } }," << endl;
			}

			out << "texture { pigment { " << getColorIndex_(temp_color) << " }"
					<< " finish { BALLFinishMesh } }" << endl;
			out << "\t\t}" << endl;
			
			// write vertex indices ---->
			out << "\t\tface_indices {" << endl;
			out << "\t\t\t" << mesh.triangle.size() << ","<<  endl;
			if (colors.size() == 1)
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
		
		bool POVRenderer::render(const Representation& representation)
			throw()
		{
			if (representation.isHidden()) return true;

			if (!representation.isValid())
			{
				Log.error() << "Representation " << &representation 
										<< "not valid, so aborting." << std::endl;
				return false;
			}	

			List<GeometricObject*>::ConstIterator it;

			for (it =  representation.getGeometricObjects().begin();
					 it != representation.getGeometricObjects().end();
					 it++)
			{
				if (!RTTI::isKindOf<Mesh>(**it))
				{
					storeColor_(**it);
				}
				else
				{
					if (representation.getDrawingMode() == DRAWING_MODE_WIREFRAME)
					{
						wireframes_.insert((Mesh*) *it);
					}

					Mesh& mesh = *dynamic_cast<Mesh*>(*it);
					String color_string;
					for (Position i = 0; i < mesh.colors.size(); i++)
					{
						mesh.colors[i].get(color_string);
						if (!color_map_.has(color_string))
						{
							color_map_.insert(ColorMap::ValueType(color_string, color_map_.size()));
							color_vector_.push_back(&mesh.colors[i]);
						}
					}
				}
			}

			representations_.push_back(&representation);

			return true;
		}

		void POVRenderer::storeColor_(const GeometricObject& object)
		{
			String color_string;
			getColor_(object).get(color_string);
			if (!color_map_.has(color_string))
			{
				color_map_.insert(ColorMap::ValueType(color_string, color_map_.size()));
				color_vector_.push_back(&getColor_(object));
			}
		}

		String POVRenderer::getColorIndex_(const ColorRGBA& color)
		{
			String color_temp;
			color.get(color_temp);
#ifdef BALL_VIEW_DEBUG
			if (!color_map_.has(color_temp))
			{
				BALLVIEW_DEBUG
				return "";
			}
#endif

			return String("c") + String(color_map_[color_temp]);
		}

		void POVRenderer::renderLabel_(const Label& label)
			throw()
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

	} // namespaces
}
