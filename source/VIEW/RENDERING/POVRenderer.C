// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: POVRenderer.C,v 1.2 2003/08/26 18:35:38 amoll Exp $

#include <BALL/VIEW/RENDERING/POVRenderer.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/VIEW/KERNEL/stage.h>

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
				outfile_()
		{
		}

		POVRenderer::POVRenderer(const String& name)
			throw(Exception::FileNotFound)
			: Renderer(),
			  width(600),
				height(600)
		{
			outfile_.open(name, std::ios::out);
		}

		POVRenderer::~POVRenderer()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<POVRenderer>() << std::endl;
			#endif
		}

		void POVRenderer::clear()
			throw()
		{
			outfile_.clear();
		}

		void POVRenderer::setFileName(const String& name)
			throw(Exception::FileNotFound)
		{
			outfile_.open(name, std::ios::out);
		}

		String POVRenderer::POVColorRGBA(const ColorRGBA& input)
			throw()
		{
			float colorVal;
			
			String output = "rgbft <";
			input.getRed().get(colorVal);
			output+=String(colorVal/255.);
			output+=", ";
			input.getGreen().get(colorVal);
			output+=String(colorVal/255.);
			output+=", ";
			input.getBlue().get(colorVal);
			output+=String(colorVal/255.);
			output+=", ";
			// TODO: sensible parameter for "filter"
			output+="0.00";
			output+=", ";
			// TODO: transmit seems not to be linear in alpha
			input.getAlpha().get(colorVal);
			output+=String(1.-colorVal/255.);
			output+=">";

			return output;
		}

		String POVRenderer::POVFinish(const String& object, const ColorRGBA& input)
			throw()
		{
			String output = "finish { BALLFinish";
			output += object;

			float colorVal;
			input.getAlpha().get(colorVal);

			if (colorVal >= 255)
				output += "Solid";
			else
				output += "Transp";

			output += " }";

			return output;
		}
		
		String POVRenderer::POVVector3(Vector3 input)
			throw()
		{
			// we convert into the new coordinate system: the camera looks
			// down the negative z axis and sits at (0,0,0)
			input += origin_;
			input  = rotation_ * input;

			String output = "";
			output+="<";
			output+=String(input.x);
			output+=", ";
			output+=String(input.y);
			output+=", ";
			output+=String(-1.*input.z);
			output+=">";

			return output;
		}

		// init must be called right before the rendering starts, since
		// we need to fix the camera, light sources, etc...
		bool POVRenderer::init(const Stage& stage)
			throw()
		{
			#ifdef BALL_VIEW_DEBUG_PROCESSORS
				Log.info() << "Start the POVRender output..." << std::endl;
			#endif

			stage_ = &stage;

			outfile_	<< "// POVRay file created by the BALL POVRenderer" 
								<< std::endl 
								<< std::endl;

			// Find out the position of the camera.
			const Camera& camera = stage_->getCamera();

			// Build the translation vector so that we can put it
			// into (0,0,0), pointing down the negative z axis
			origin_ = camera.getViewPoint() * -1.;

			// Build the rotation matrix for all vectors in the system
			Vector3 view_point   = Vector3(0.,0.,0. );
			Vector3 direction    = Vector3(0.,0.,-1.);

			Vector3 old_pos      = camera.getLookAtPosition()+origin_;
			old_pos.normalize();

			Angle   rotation_angle(acos( old_pos * direction ));
			Vector3 rotation_axis = old_pos % direction; 
			
			if (rotation_axis.getLength() == 0)
			{
				rotation_.setRotation(Angle(0.), direction);
			}
			else
			{
				rotation_axis.normalize();
				rotation_.setRotation(Angle(rotation_angle), rotation_axis);
			}

			view_point           = camera.getViewPoint();
			Vector3 up_vector    = camera.getLookUpVector();
			Vector3 right_vector = camera.getRightVector(); 
			Vector3 look_at      = camera.getLookAtPosition();// camera.getLookAtPosition();

			outfile_ << "camera {" << endl;
			outfile_ << "\tlocation " << POVVector3(view_point) << endl;
			outfile_ << "\tlook_at  " << POVVector3(look_at) << endl;


//			right_vector += origin_;
			right_vector  = rotation_ * right_vector;
			right_vector.normalize();
			right_vector*=-1.;

			outfile_ << "\tright    ";
			String output = "<"; output+=String(right_vector.x);
			output+=", "; output+=String(right_vector.y);
			output+=", "; output+=String(-1.*right_vector.z);
			output+=">";
			outfile_ << output << endl;

//			up_vector += origin_;
			up_vector  = rotation_ * up_vector;
			up_vector.normalize();
			up_vector *= (float)height / (float)width;

			outfile_ << "\tup       ";
			output = "<"; output+=String(up_vector.x);
			output+=", "; output+=String(up_vector.y);
			output+=", "; output+=String(-1.*up_vector.z);
			output+=">";
			outfile_ << output << endl;

			outfile_ << "\trotate   <0, 0, 180>" << endl;

			outfile_ << "}" << endl;

			// Set the light sources
			List<LightSource>::ConstIterator it = stage_->getLightSources().begin();
			for (; it!=stage_->getLightSources().end(); it++)
			{
				// first, set up the color of the light source
				float light_rgba[4];

				light_rgba[0] = ((float)it->getColor().getRed())   * it->getIntensity();
				light_rgba[1] = ((float)it->getColor().getGreen()) * it->getIntensity();
				light_rgba[2] = ((float)it->getColor().getBlue())  * it->getIntensity();
				light_rgba[3] = ((float)it->getColor().getAlpha());
				
				
				ColorRGBA light_col(light_rgba[0], light_rgba[1], light_rgba[2], light_rgba[3]);
				
				if (it->getType() == LightSource::AMBIENT)
				{
					outfile_ << "global_settings { ambient_light " << POVColorRGBA(light_col) << " }" << endl;
					continue;
				}

				outfile_ << "light_source { ";
				outfile_ << POVVector3(it->getPosition()) << ", " << POVColorRGBA(light_col) << "}" << endl;

				// TODO: distinguish between directional / positional light sources
			}
			
			// Set the background color
			outfile_ << "background { " << POVColorRGBA(stage_->getBackgroundColor()) << " }" << endl << endl;

			// Define the finish we will use for our molecular objects (defining the molecular
			// "material properties"
			// TODO: allow for more than one finish in order to have seperate parameters for different objects
			outfile_ << "#declare BALLFinish             		 = finish { specular 0.4 diffuse 1.0 ambient 0.0 }" << endl;
			outfile_ << "#declare BALLFinishSphereSolid      = finish { specular 0.4 diffuse 1.0 ambient 0.0 }" << endl;
			outfile_ << "#declare BALLFinishSphereTransp     = finish { specular 0.4 diffuse 1.0 ambient 0.0 }" << endl;
			outfile_ << "#declare BALLFinishTubeSolid        = finish { specular 0.4 diffuse 1.0 ambient 0.0 }" << endl;
			outfile_ << "#declare BALLFinishTubeTransp       = finish { specular 0.4 diffuse 1.0 ambient 0.0 }" << endl;
			outfile_ << "#declare BALLFinishMesh             = finish { specular 0.4 diffuse 1.0 ambient 0.0 }" << endl << endl;
			
			// now begin the CSG union containing all the geometric objects
			outfile_ << "union {" << endl;

			return true;
		}

		bool POVRenderer::finish()
			throw()
		{
			outfile_ << "}" << endl;
			outfile_.close();

			return true;
		}

		void POVRenderer::renderSphere_(const Sphere& sphere)
			throw()
		{
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

			// now write the information into the outfile_
			outfile_ << "\tsphere {" << endl << "\t\t";
			outfile_ << POVVector3(position) << ", ";
			outfile_ << radius << endl;
			//outfile_ <<"\t\ttexture {" << endl;
			outfile_ << "\tpigment { " << POVColorRGBA(color) << " } " << endl;
			outfile_ << "\t" << POVFinish("Sphere", color) << endl;
			outfile_ << "\t}" << endl << endl;
		}

		void POVRenderer::renderDisc_(const Disc& disc)
			throw()
		{
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

			// then, find out its radius
			float radius;

			// its normal
			Vector3 normal;
			// and finally, its position
			Vector3 position;

			disc.getCircle().get(position, normal, radius);
		//Log.error() << normal << std::endl;
			normal -= origin_;

			// now write the information into the outfile_
			outfile_ << "\tdisc {" << endl << "\t\t";
			outfile_ << POVVector3(position) << ", ";
			outfile_ << POVVector3(normal) << ", ";
			outfile_ << radius << endl;
			//outfile_ <<"\t\ttexture {" << endl;
			outfile_ << "\tpigment { " << POVColorRGBA(color) << " } " << endl;
			outfile_ << "\t" << POVFinish("Tube", color) << endl; // We use the same finish as for tubes -> helices
			outfile_ << "\t} " << endl << endl;
		}

		void POVRenderer::renderTube_(const Tube& tube)
			throw()
		{
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

			// now write the information into the outfile_
			outfile_ << "\tcylinder {" << endl;
			outfile_ << "\t\t" << POVVector3(base_point) << ", ";
			outfile_           << POVVector3( cap_point) << ", ";
			outfile_           <<                 radius << endl;
			//outfile_ <<"\t\ttexture {" << endl;
			//outfile_ << "\t\t\t\tpigment { " << POVColorRGBA(color);
			outfile_ << "\tpigment { " << POVColorRGBA(color) << " } " << endl;
			outfile_ << "\t" << POVFinish("Tube", color) << endl; 
			outfile_ << "\t} " << endl;
		}	

		void POVRenderer::renderTwoColoredTube_(const TwoColoredTube& tube)
			throw()
		{
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

			// now write the information into the outfile_
			outfile_ << "\tcylinder {" << endl;
			outfile_ << "\t\t" << POVVector3(base_point) << ", ";
			outfile_           << POVVector3( mid_point) << ", ";
			outfile_           <<                 radius << endl;
			outfile_ << "\tpigment { " << POVColorRGBA(color1) << " } " << endl;
			outfile_ << "\t" << POVFinish("Tube", color1) << endl; 
			outfile_ << "\t}" << endl << endl;
			
			outfile_ << "\tcylinder {" << endl;
			outfile_ << "\t\t" << POVVector3(mid_point) << ", ";
			outfile_           << POVVector3(cap_point) << ", ";
			outfile_           <<                 radius << endl;
			outfile_ << "\tpigment { " << POVColorRGBA(color2) << " } " << endl;
			outfile_ << "\t" << POVFinish("Tube", color2) << endl; 
			outfile_ << "\t}" << endl << endl;
		}

		void POVRenderer::renderMesh_(const Mesh& mesh)
			throw()
		{
			// so we should let POVRay know...
			outfile_ << "\tmesh {" << endl;

			ColorRGBA c1, c2, c3;
			Surface::Triangle t;
			Vector3 v1, v2, v3;
			Vector3 n1, n2, n3;

			// first, find out if we have a different color for each
			// point
			if (mesh.colorList.size()<mesh.vertex.size())
			{
				// nope. we don't...
				if (mesh.colorList.size()==0)
				{
					c1 = ColorRGBA(255,255,255,255);
				}
				else
				{
					c1 = mesh.colorList[0];
				}

				for (Position i=0; i<mesh.triangle.size(); i++)
				{
					t = mesh.triangle[i];

					v1 = mesh.vertex[t.v1];
					n1 = mesh.normal[t.v1]-origin_;
					v2 = mesh.vertex[t.v2];
					n2 = mesh.normal[t.v2]-origin_;
					v3 = mesh.vertex[t.v3];
					n3 = mesh.normal[t.v3]-origin_;

					outfile_ << "\t\tsmooth_triangle {" << endl;
					outfile_ << "\t\t\t#local BALLColor = texture { pigment { " << POVColorRGBA(c1) << " } }" << endl;


					outfile_ << "\t\t\t" << POVVector3(v1) << ", ";
					outfile_             << POVVector3(n1) << ", " << endl;
					outfile_ << "\t\t\t" << POVVector3(v2) << ", ";
					outfile_             << POVVector3(n2) << ", " << endl;
					outfile_ << "\t\t\t" << POVVector3(v3) << ", ";
					outfile_             << POVVector3(n3) << endl << endl;
					

					// And now the color. This is easy here, because we
					// only have one color.
					outfile_ <<"\t\t\ttexture { BALLColor }" << endl;
					outfile_ << "\t\t}" << endl << endl;
				}
			}
			else
			{
				// we have a color for each of the vertices
				for (Position i=0; i<mesh.triangle.size(); i++)
				{
					t = mesh.triangle[i];

					v1 = mesh.vertex[t.v1];
					n1 = mesh.normal[t.v1]-origin_;
					c1 = mesh.colorList[t.v1];
					v2 = mesh.vertex[t.v2];
					n2 = mesh.normal[t.v2]-origin_;
					c2 = mesh.colorList[t.v2];
					v3 = mesh.vertex[t.v3];
					n3 = mesh.normal[t.v3]-origin_;
					c3 = mesh.colorList[t.v3];

					outfile_ << "\t\tsmooth_triangle {" << endl;
					outfile_ << "\t\t\t#local BALLColor1 = texture { pigment { " << POVColorRGBA(c1) << " } }" << endl;
					outfile_ << "\t\t\t#local BALLColor2 = texture { pigment { " << POVColorRGBA(c2) << " } }" << endl;
					outfile_ << "\t\t\t#local BALLColor3 = texture { pigment { " << POVColorRGBA(c3) << " } }" << endl << endl;

					outfile_ << "\t\t\t" << POVVector3(v1) << ", ";
					outfile_             << POVVector3(n1) << ", " << endl;
					outfile_ << "\t\t\t" << POVVector3(v2) << ", ";
					outfile_             << POVVector3(n2) << ", " << endl;
					outfile_ << "\t\t\t" << POVVector3(v3) << ", ";
					outfile_             << POVVector3(n3) << endl << endl;

					// And now the colors.
					outfile_ <<"\t\t\ttexture_list {" << endl;
					outfile_ << "\t\t\t\t\tBALLColor1 BALLColor2 BALLColor3" << endl;
					outfile_ << "\t\t\t}" << endl;
					outfile_ << "\t\t}" << endl << endl;
				}
			}

			// now close the mesh
			outfile_ << "\t\t finish { BALLFinishMesh }" << endl;
			outfile_ << "\t}" << endl << endl;
		}
	}
}
