// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: POVRenderer.C,v 1.18.2.6 2005/01/01 12:50:44 amoll Exp $
//

#include <BALL/VIEW/RENDERING/POVRenderer.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/VIEW/KERNEL/stage.h>
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

		POVRenderer::POVRenderer()
			throw()
			: Renderer(),
				outfile_(&std::cout),
				human_readable_(true)
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
				human_readable_(true)
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
			String output = "rgbft <";
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
			String output = String(value).trimRight("0");
			for (Position p = 0; p < output.size(); p++)
			{
				if (output[p] == '.')
				{
					return output.left(p + 3);
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
					String filename = FileSystem::baseName((*(File*)outfile_).getName());
					out << "// povray +I" << filename 
							<< " +FN +O" << filename << ".png +QR +W" << width_ 
							<< " +H" << height_ << " -UV\n//" << endl;
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
			out << "\t\t" 
							 << m[0] / norm << ",  " << m[1] / norm << ", " << m[2] / norm << "," << endl;
			norm = sqrt(m[4] * m[4] + m[5] * m[5] + m[6] * m[6]);
			out << "\t\t" << m[4] / norm << ",  " << m[5] / norm << ", " << m[6] / norm << "," << endl;
			norm = sqrt(m[8] * m[8] + m[9] * m[9] + m[10] * m[10]);
			out << "\t\t" << m[8] / norm << ",  " << m[9] / norm << ", " << m[10] / norm << "," << endl;
			out << "\t\t" << m[12] << ",  " << m[13] << ", " << m[14] << endl;
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
				out << POVVector3(it->getPosition()) << ", " << POVColorRGBA(light_col) << "}" << endl;

				// TODO: distinguish between directional / positional light sources
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
			out << "diffuse " 	<< stage.getDiffuseIntensity() 	/ 2.0 + 0.5 << " ";
			// povray uses an other ambient setting
			out << "ambient 0.0 }"	 	<< endl;
			out << "#declare BALLFinishSphereSolid      = BALLFinish" << endl;
			out << "#declare BALLFinishSphereTransp     = BALLFinish" << endl;
			out << "#declare BALLFinishTubeSolid        = BALLFinish" << endl;
			out << "#declare BALLFinishTubeTransp       = BALLFinish" << endl;
			out << "#declare BALLFinishMesh             = BALLFinish" << endl;
			
			// now begin the CSG union containing all the geometric objects
			out << "union {" << endl;

			return true;
		}

		bool POVRenderer::finish()
			throw()
		{
			std::ostream& out = *outfile_;

			vector<POVRendererClippingPlane>::iterator it = clipping_planes_.begin();
			for (;it != clipping_planes_.end(); it++)
			{
				out << "  clipped_by{" << endl
								 << "   plane{< -"  // negate normal vector
					       << (*it).normal.x << ", -" 
					       << (*it).normal.y << ", -" 
					       << (*it).normal.z << ">, "
					       << (*it).translation
								 << "  }" << endl
								 << " }" << endl;
			}
			out << "}" << endl;


			if (outfile_ != 0 && RTTI::isKindOf<File>(*outfile_))
			{
				(*(File*)outfile_).close();
			}

			clipping_planes_.clear();
			return true;
		}

		void POVRenderer::renderSphere_(const Sphere& sphere)
			throw()
		{
			std::ostream& out = *outfile_;

			ColorRGBA color;
			// first find out its color
			if ((sphere.getComposite()) && sphere.getComposite()->isSelected())
			{
				color = BALL_SELECTED_COLOR;
			}
			else
			{
				color = sphere.getColor();
			}

			// then, find out its radius
			float radius = sphere.getRadius();

			// and finally, its position
			Vector3 position = sphere.getPosition();

			// now write the information into the out
			out << "\tsphere { ";
			out << POVVector3(position) << ", ";
			out << radius;
			//out <<"\t\ttexture {" << endl;
			out << " pigment { " << POVColorRGBA(color) << " } ";
			out << POVFinish("Sphere", color);
			out << " }" << endl << endl;
		}

		void POVRenderer::renderDisc_(const Disc& disc)
			throw()
		{
			std::ostream& out = *outfile_;

			ColorRGBA color;
			// first find out its color
			if ((disc.getComposite() && (disc.getComposite()->isSelected())))
			{
				color = BALL_SELECTED_COLOR;
			}
			else
			{
				color = disc.getColor();
			}

			// then, find out its radius, its normal, and its position
			float radius;
			Vector3 normal;
			Vector3 position;
			disc.getCircle().get(position, normal, radius);
			normal -= origin_;

			// now write the information into the out
			out << "\tdisc { ";
			out << POVVector3(position) << ", ";
			out << POVVector3(normal) << ", ";
			out << radius;
			out << " pigment { " << POVColorRGBA(color) << " } ";
			out << POVFinish("Tube", color); // We use the same finish as for tubes -> helices
			out << "} " << endl << endl;
		}

		void POVRenderer::renderTube_(const Tube& tube)
			throw()
		{
			std::ostream& out = *outfile_;

			ColorRGBA color;
			// first, find out its color
			if ((tube.getComposite()) && (tube.getComposite()->isSelected()))
			{
				color = BALL_SELECTED_COLOR;
			}
			else
			{
				color = tube.getColor();
			}

			// then, find out its radius
			float radius = tube.getRadius();

			// and finally, the base and the cap
			Vector3 base_point = tube.getVertex1();
			Vector3  cap_point = tube.getVertex2();

			// now write the information into the out
			out << "\tcylinder {";
			out << "\t" << POVVector3(base_point) << ", ";
			out << POVVector3( cap_point) << ", ";
			out << radius;
			out << " pigment { " << POVColorRGBA(color) << " } ";
			out << POVFinish("Tube", color);
			out << "} " << endl;
		}	

		void POVRenderer::renderTwoColoredTube_(const TwoColoredTube& tube)
			throw()
		{
			std::ostream& out = *outfile_;

			// we have found a two colored tube
			ColorRGBA color1, color2;

			// first, find out its color
			if (tube.getComposite() && (tube.getComposite()->isSelected()))
			{
				color1 = BALL_SELECTED_COLOR;
				color2 = BALL_SELECTED_COLOR;
			}
			else
			{
				color1 = tube.getColor();
				color2 = tube.getColor2();
			}

			// then, find out its radius
			float radius = tube.getRadius();

			// and finally, the base and the cap
			Vector3 base_point = tube.getVertex1();
			Vector3  cap_point = tube.getVertex2();
			Vector3  mid_point = tube.getMiddleVertex();

			// now write the information into the out
			out << "\tcylinder { ";
			out << POVVector3(base_point) << ", ";
			out << POVVector3( mid_point) << ", ";
			out << radius;
			out << " pigment { " << POVColorRGBA(color1) << " } ";
			out << POVFinish("Tube", color1);
			out << "}" << endl;
			
			out << "\tcylinder {";
			out << POVVector3(mid_point) << ", ";
			out << POVVector3(cap_point) << ", ";
			out << radius;
			out << " pigment { " << POVColorRGBA(color2) << " } ";
			out << POVFinish("Tube", color2);
			out << "}" << endl << endl;
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
			typedef HashMap<String, Position> ColorMap;
			ColorMap colors;
			vector<const ColorRGBA*> color_vector;
			String color_string;
			Position pos = 0;
			for (Position i = 0; i < mesh.colorList.size(); i++)
			{
				mesh.colorList[i].get(color_string);
				if (!colors.has(color_string))
				{
					colors.insert(ColorMap::ValueType(color_string, pos));
					color_vector.push_back(&mesh.colorList[i]);
					pos++;
				}
			}

			// write colors of vertices ---->
			out << "\t\ttexture_list{" << endl;
			out << "\t\t\t" << colors.size() + 1<< ","<< endl;

			ColorRGBA temp_color;
			for (Position p = 0; p < color_vector.size(); p++)
			{
   			temp_color.set((*color_vector[p]));
				out << "texture { pigment { " << POVColorRGBA(temp_color) << " }"
						<< " finish { BALLFinishMesh } }," << endl;
			}

			out << "texture { pigment { " << POVColorRGBA(temp_color) << " }"
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
					mesh.colorList[mesh.triangle[i].v1].get(color_temp);
					out << colors[color_temp] << ", ";
					mesh.colorList[mesh.triangle[i].v1].get(color_temp);
					out << colors[color_temp] << ", ";
					mesh.colorList[mesh.triangle[i].v1].get(color_temp);
					out << colors[color_temp] << endl;
				}
			}
			out << "\t\t}" << endl;

			out << "\t}" << endl;
		}
				
		void POVRenderer::renderClippingPlane_(const Representation& rep)
			throw()
		{
			POVRendererClippingPlane plane;
			plane.normal = Vector3(rep.getProperty("AX").getDouble(),
														 rep.getProperty("BY").getDouble(),
														 rep.getProperty("CZ").getDouble());
			plane.translation = rep.getProperty("D").getDouble();

			clipping_planes_.push_back(plane);
		}

		
	} // namespaces
}
