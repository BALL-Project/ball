#version 120

//-------------------------------------------- Inputs ----------------------------------------------
// Vertex position in world space.
attribute vec3 vertex_position;

// Color of the vertex.
attribute vec4 color;

// View matrix to transform from world space to view space.
uniform mat4 view_matrix;

// Projection matrix to transform from view space to clip space.
uniform mat4 projection_matrix;

//--------------------------------------------- Outputs --------------------------------------------

// Color of the vertex.
varying vec4 col;

// Vertex position in world space (homogeneous coordinates).
varying vec4 pos;

// Vertex position in view space (homogeneous coordinates).
varying vec4 pos_v;

//-------------------------------------- Function Definition ---------------------------------------
void setup()
{
	gl_PointSize = 1;

	// make color available in fragment shader
	col = color;

	//use homogeneous coordinates for position
	pos = vec4(vertex_position, 1.0);

	//transform vertex position to view space
	pos_v = view_matrix * pos;

	//transform vertex postion to clip space
	gl_Position = projection_matrix * pos_v;
}
