#version 120


//-------------------------------------------- Inputs ----------------------------------------------
//View matrix to transform from world space to view space.
uniform mat4 view_matrix;

//Projection matrix to transform from view space to clip space.
uniform mat4 projection_matrix;

//Midpoint of the point in world space.
attribute vec3 midpoint;

//Color of the point.
attribute vec3 texcoords;

//Radius of the point.
attribute float radius;

//--------------------------------------------- Outputs --------------------------------------------

//texcoords of the point.
varying vec3 texc;

//Fragment position in world space (homogeneous coordinates).
varying vec4 mid;

//Fragment position in view space (homogeneous coordinates).
varying vec4 mid_v;

//-------------------------------------- Function Definition ---------------------------------------
void setup()
{
	gl_PointSize = radius;
	
		// make texcoords available in fragment shader
	texc = texcoords;

	//use homogeneous coordinates for position
	mid = vec4(midpoint, 1.0);
	
	//transform vertex position to view space
	mid_v = view_matrix * mid;
	
	//transform vertex postion to clip space
	gl_Position = projection_matrix * mid_v;
}