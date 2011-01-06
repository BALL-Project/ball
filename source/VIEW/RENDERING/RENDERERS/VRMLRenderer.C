// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: VRMLRenderer.C,v 1.6.20.1 2007-03-25 21:57:01 oliver Exp $
// modified by Annette Treichel (2008.09.12)
//

#include <BALL/VIEW/RENDERING/RENDERERS/VRMLRenderer.h>
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

#include <BALL/MATHS/surface.h>

using std::endl;

namespace BALL
{
	namespace VIEW
	{

VRMLRenderer::VRMLRenderer()
	: Renderer(),
		outfile_(),
		current_indent_(0)
{
}

VRMLRenderer::VRMLRenderer(const String& name)
	throw(Exception::FileNotFound)
	: Renderer(),
		width(600),
		height(600),
		current_indent_(0)
{
	outfile_.open(name, std::ios::out);

	out_("#VRML V2.0 utf8");
	out_("");
}

VRMLRenderer::~VRMLRenderer()
{
	#ifdef BALL_VIEW_DEBUG
		Log.info() << "Destructing object " << (void *)this 
			<< " of class " << RTTI::getName<VRMLRenderer>() << std::endl;
	#endif
}

void VRMLRenderer::clear()
{
	outfile_.clear();
	current_indent_ = 0;
}

void VRMLRenderer::setFileName(const String& name)
	throw(Exception::FileNotFound)
{
	outfile_.open(name, std::ios::out);
	current_indent_ = 0;

	out_("#VRML V2.0 utf8");
	out_("");
}

String VRMLRenderer::VRMLColorRGBA(const ColorRGBA& input)
{
	return 
		String((float) input.getRed())   + " " + 
		String((float) input.getGreen()) + " " +
		String((float) input.getBlue());
}

String VRMLRenderer::VRMLVector3(Vector3 input)
{
	// we convert into the new coordinate system: the camera looks
	// down the negative z axis and sits at (0,0,0)
	/*
	input += origin_;
	input  = rotation_ * input;
	*/
	String output = "";
	output+=String(input.x);
	output+=" ";
	output+=String(input.y);
	output+=" ";
	output+=String(input.z);

	return output;
}

// init must be called right before the rendering starts, since
// we need to fix the camera, light sources, etc...
bool VRMLRenderer::init(const Stage& stage)
{
	#ifdef BALL_VIEW_DEBUG_PROCESSORS
		Log.info() << "Start the VRMLRender output..." << std::endl;
	#endif

	stage_ = &stage;
/*
	// Find out the position of the camera.
	const Camera& camera = stage_->getCamera();

	view_point           = camera.getViewPoint();
	Vector3 up_vector    = camera.getLookUpVector();
	Vector3 right_vector = camera.getRightVector(); 
	Vector3 look_at      = camera.getLookAtPosition();// camera.getLookAtPosition();
*/

	//prepatation for use of sclaing
	scalingUsed = false;

	return true;
}

bool VRMLRenderer::finish()
{
	//before finishing we write the scaling if was used
	String infos = "# Biggest Values: (" + (String)(bigX) + "|"+ (String)(bigY) + "|"+ (String)(bigZ) + "); Smallest Values: (" + (String)smallX + "|"+ (String)smallY + "|"+ (String)smallZ +")";
	String scaling = "# Boxsize: " + (String)(bigX - smallX) + " "+ (String)(bigY - smallY) + " "+ (String)(bigZ - smallZ) + "; Volume (unscaled, [mm^3]): "+ String((bigX - smallX)*(bigY - smallY)*(bigZ - smallZ));
	out_(infos);
	out_(scaling);


	outfile_.close();
	current_indent_ = 0;

	return true;
}

void VRMLRenderer::header_(const Vector3& translation, const ColorRGBA& color,
													 const String& rotation)
{
	outheader_("Transform {");
 	if (rotation != "")
	{
 		outheader_("rotation " + rotation);
	}
	outheader_("translation " + VRMLVector3(translation));
	outheader_("children [");

	outheader_("Shape {");

	VRMLColor(color);
}


void VRMLRenderer::footer_()
{
	current_indent_ --;
	outfinish_("}");
	outfinish_("}");
	outfinish_("]");
	out_("}");
}


void VRMLRenderer::renderSphere_(const Sphere& sphere)
{
	header_(sphere.getPosition(), sphere.getColor());
	outheader_("geometry Sphere {");
	out_("radius " + String(sphere.getRadius()));
	footer_();
}


void VRMLRenderer::renderLine_(const Line& miniTube)
{
	static Vector3 default_angle(0,1,0);

	Vector3 v = miniTube.getVertex2() - miniTube.getVertex1(); 

	float f = v.getAngle(default_angle);
	Vector3 a = default_angle % v;

	String r;
	if (!Maths::isZero(a.getSquareLength()))
	{
		a.normalize();
		r = String(a.x) + " " + 
				String(a.y) + " " + 
				String(a.z) + " ";

		r += String(f);
	}

	header_(miniTube.getVertex1() + v / 2.0, miniTube.getColor(), r);
	outheader_("geometry Cylinder {");
	out_("height " + String(v.getLength()));
	out_("radius " + String((v.getLength() / 100.0)));
	footer_();
}

void VRMLRenderer::renderTwoColoredLine_(const TwoColoredLine& miniTube)
{
	static Vector3 default_angle(0,1,0);

	Vector3 v = miniTube.getVertex2() - miniTube.getVertex1(); 

	float f = v.getAngle(default_angle);
	Vector3 a = default_angle % v;

	String r;
	if (!Maths::isZero(a.getSquareLength()))
	{
		a.normalize();
		r = String(a.x) + " " + 
				String(a.y) + " " + 
				String(a.z) + " ";

		r += String(f);
	}

	//erste halbe linie:
	header_(miniTube.getVertex1() + v / 4.0, miniTube.getColor(), r);
	outheader_("geometry Cylinder {");
	out_("height " + String((v.getLength() / 2.0)));
	out_("radius " + String((v.getLength() / 100.0)));
	footer_();

	//zweite halbe Linie:
	header_(miniTube.getVertex1() + v / (4.0 / 3.0), miniTube.getColor2(), r);
	outheader_("geometry Cylinder {");
	out_("height " + String((v.getLength() / 2.0)));
	out_("radius " + String((v.getLength() / 100.0 )));
	footer_();
}

void VRMLRenderer::renderTube_(const Tube& tube)
{
	static Vector3 default_angle(0,1,0);

	Vector3 v = tube.getVertex2() - tube.getVertex1(); 

	float f = v.getAngle(default_angle);
	Vector3 a = default_angle % v;

	String r;
	if (!Maths::isZero(a.getSquareLength()))
	{
		a.normalize();
		r = String(a.x) + " " + 
				String(a.y) + " " + 
				String(a.z) + " ";

		r += String(f);
	}

	header_(tube.getVertex1() + v / 2.0, tube.getColor(), r);
	outheader_("geometry Cylinder {");
	out_("height " + String(v.getLength()));
	out_("radius " + String(tube.getRadius()));
	footer_();
}


void VRMLRenderer::renderTwoColoredTube_(const TwoColoredTube& tube)
{
	static Vector3 default_angle(0,1,0);

	Vector3 v = tube.getVertex2() - tube.getVertex1(); 

	float f = v.getAngle(default_angle);
	Vector3 a = default_angle % v;

	String r;
	if (!Maths::isZero(a.getSquareLength()))
	{
		a.normalize();
		r = String(a.x) + " " + 
				String(a.y) + " " + 
				String(a.z) + " ";

		r += String(f);
	}


	//erstes halbes rohr:
	header_(tube.getVertex1() + v / 4.0, tube.getColor(), r);
	outheader_("geometry Cylinder {");
	out_("height " + String((v.getLength() / 2.0)));
	out_("radius " + String(tube.getRadius()));
	footer_();

	//zweites halbes rohr:
	header_(tube.getVertex1() + v / (4.0 / 3.0), tube.getColor2(), r);
	outheader_("geometry Cylinder {");
	out_("height " + String((v.getLength() / 2.0)));
	out_("radius " + String(tube.getRadius()));
	footer_();
}


void VRMLRenderer::VRMLColor(const ColorRGBA& color)
{
	outheader_("appearance Appearance {");
	outheader_("material Material {");
	out_("diffuseColor " + VRMLColorRGBA(color));

	out_("shininess 0.5");
	current_indent_ --;
	outfinish_("}");
	outfinish_("}");
}

void VRMLRenderer::renderMesh_(const Mesh& mesh)
{
	// so we should let VRMLRay know...
	outheader_("Shape {");
	// if we have no or only a single color, we need an apearance node for the whole mesh
	if (mesh.colors.size() == 0)
	{
		VRMLColor(ColorRGBA(1.,1,1));
	} 
	else if (mesh.colors.size() == 1)
	{
		VRMLColor(mesh.colors[0]);
	}
	
	outheader_("geometry IndexedFaceSet {");
	out_("normalPerVertex TRUE");
	outheader_("coord Coordinate {");
	outheader_("point [");

	// print vertices
	vector<Vector3>::const_iterator itv = mesh.vertex.begin(); 
	for (; itv != mesh.vertex.end(); itv++)
	{ 
		String out = VRMLVector3(*itv);
		if (itv != mesh.vertex.end()) 
		{
			out += ",";
		}
		out_(out);

		//scaling max and min search
		if(scalingUsed)
		{
			//X value:
			if (smallX > (*itv).x)
			{
				smallX = (*itv).x;
			}
			if (bigX < (*itv).x)
			{
				bigX = (*itv).x;
			}

			//Y value:
			if (smallY > (*itv).y)
			{
				smallY = (*itv).y;
			}
			if (bigY < (*itv).y)
			{
				bigY = (*itv).y;
			}

			//Z value:
			if (smallZ > (*itv).z)
			{
				smallZ = (*itv).z;
			}
			if (bigZ < (*itv).z)
			{
				bigZ = (*itv).z;
			}
		}
		else
		{
			//als initiationswerte werden einfach die ersten Koordinaten genommen
			smallX = (*itv).x;
			bigX = (*itv).x;
			smallY = (*itv).y;
			bigY = (*itv).y;
			smallZ = (*itv).z;
			bigZ = (*itv).z;
			scalingUsed = true;
		}
	}
	current_indent_ --;
	outfinish_("]");
	out_("}"); // correct
	
	// print triangles =====================================
	outheader_("coordIndex [");
	
	vector<Surface::Triangle>::const_iterator itt = mesh.triangle.begin(); 
	for (; itt != mesh.triangle.end(); itt++)
	{
  	String out = (String((*itt).v1) + " " 
						 + String((*itt).v2) + " " 
						 + String((*itt).v3) + ", -1"); 

		if (itt != mesh.triangle.end()) 
		{
			out +=  ",";
		}
		out_(out);
	}
	outfinish_("]");
	
	// print normals =====================================
	//Normals are not needed for printing and increase amount of data therefore I take them away	
/*	outheader_("normal Normal {");
	outheader_("vector [");
	itv = mesh.normal.begin(); 
	for (; itv != mesh.normal.end(); itv++)
	{
		String out = VRMLVector3(*itv);
		if (itv != mesh.normal.end()) 
		{
			out += ",";
		}
		out_(out);
	}
	outfinish_("]");
	outfinish_("}");
*/
	/*
	outheader_("normalIndex [");
	for (Position i = 0; i < mesh.vertex.size(); i++)
	{
		String si(i);
		out_(si + ", " + si + ", " + si);
	}
	outfinish_("]");
	*/
// print colors ========================================
	outheader_("color Color {");
	outheader_("color [");
	if (mesh.colors.size() > 1)
	{
		vector<ColorRGBA>::const_iterator itc = mesh.colors.begin(); 
		for (; itc != mesh.colors.end(); itc++)
		{
			String out = VRMLColorRGBA(*itc);
			if (itc != mesh.colors.end()) 
			{
				out += ",";
			}
			out_(out);
		}
	}
	current_indent_ --;
	outfinish_("]");
	out_("}");

	if (mesh.colors.size() > 1)
	{
		outfinish_("colorPerVertex TRUE");
	}
	else
	{
		outfinish_("colorPerVertex FALSE");
	}
	outfinish_("}");
	outfinish_("}");
}

void VRMLRenderer::out_(const String& data)
{
	if (current_indent_ < 0)
	{
//   		BALLVIEW_DEBUG
	}
	String out;
	for (Index p=0; p< current_indent_; p++)
	{
		out += " ";
	}
	out += data;
	outfile_ << out << std::endl;
}

} } // namespaces
