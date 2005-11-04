// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: VRMLRenderer.C,v 1.3.8.9 2005/11/04 12:51:41 amoll Exp $
//

#include <BALL/VIEW/RENDERING/VRMLRenderer.h>
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
	throw()
	: Renderer(),
		outfile_(),
		current_intend_(0)
{
}

VRMLRenderer::VRMLRenderer(const String& name)
	throw(Exception::FileNotFound)
	: Renderer(),
		width(600),
		height(600),
		current_intend_(0)
{
	outfile_.open(name, std::ios::out);

	out_("#VRML V2.0 utf8");
	out_("");
}

VRMLRenderer::~VRMLRenderer()
	throw()
{
	#ifdef BALL_VIEW_DEBUG
		Log.info() << "Destructing object " << (void *)this 
			<< " of class " << RTTI::getName<VRMLRenderer>() << std::endl;
	#endif
}

void VRMLRenderer::clear()
	throw()
{
	outfile_.clear();
	current_intend_ = 0;
}

void VRMLRenderer::setFileName(const String& name)
	throw(Exception::FileNotFound)
{
	outfile_.open(name, std::ios::out);
	current_intend_ = 0;

	out_("#VRML V2.0 utf8");
	out_("");
}

String VRMLRenderer::VRMLColorRGBA(const ColorRGBA& input)
	throw()
{
	return 
		String((float) input.getRed())   + " " + 
		String((float) input.getGreen()) + " " +
		String((float) input.getBlue());
}

String VRMLRenderer::VRMLVector3(Vector3 input)
	throw()
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
	throw()
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

	return true;
}

bool VRMLRenderer::finish()
	throw()
{
	outfile_.close();
	current_intend_ = 0;

	return true;
}

void VRMLRenderer::header_(const Vector3& translation, const ColorRGBA& color,
													 const String& rotation)
	throw()
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
	throw()
{
	current_intend_ --;
	outfinish_("}");
	outfinish_("}");
	outfinish_("]");
	out_("}");
}


void VRMLRenderer::renderSphere_(const Sphere& sphere)
	throw()
{
	header_(sphere.getPosition(), sphere.getColor());
	outheader_("geometry Sphere {");
	out_("radius " + String(sphere.getRadius()));
	footer_();
}


void VRMLRenderer::renderTube_(const Tube& tube)
	throw()
{
	header_(tube.getVertex1(), tube.getColor());
	outheader_("geometry Cylinder {");
	Vector3 v = tube.getVertex2() - tube.getVertex1(); 
	out_("height " + String(v.getLength()));
	out_("radius " + String(tube.getRadius()));
	footer_();
}


void VRMLRenderer::renderTwoColoredTube_(const TwoColoredTube& tube)
	throw()
{
	static Vector3 default_angle(0,1,0);

	Vector3 v = tube.getVertex2() - tube.getVertex1(); 

	float f = v.getAngle(default_angle);
	Vector3 a = v % default_angle;

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


void VRMLRenderer::VRMLColor(const ColorRGBA& color)
	throw()
{
	outheader_("appearance Appearance {");
	outheader_("material Material {");
	out_("diffuseColor " + VRMLColorRGBA(color));

	out_("shininess 0.5");
	current_intend_ --;
	outfinish_("}");
	outfinish_("}");
}

void VRMLRenderer::renderMesh_(const Mesh& mesh)
	throw()
{
	// so we should let VRMLRay know...
	outheader_("Shape {");
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
	}
	current_intend_ --;
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
	outheader_("normal Normal {");
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
	if (mesh.colors.size() == 0)
	{
		out_(VRMLColorRGBA(ColorRGBA(1.,1,1)));
	}
	else
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
	current_intend_ --;
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
	throw()
{
	if (current_intend_ < 0)
	{
//   		BALLVIEW_DEBUG
	}
	String out;
	for (Index p=0; p< current_intend_; p++)
	{
		out += " ";
	}
	out += data;
	outfile_ << out << std::endl;
}

} } // namespaces
