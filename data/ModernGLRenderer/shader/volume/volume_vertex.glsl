#version 120

/**
 * Vertex shader for volume rendering.
 *
 * Positions in view and projection space, as well as color and texture coordinates  are written to
 * varyings to be interpolated for the fragment shader.
 *
 * This shader is based on the work of
 * J. Krüger and R. Westermann. Acceleration techniques for
 * GPU-based volume rendering. In Proceedings of IEEE Visualization
 * 2003, pages 287–292, 2003.
 * and
 * and a tutorial of http://www.daimi.au.dk/~trier/?page_id=98
 *
 */

//-------------------------------------------- Inputs ----------------------------------------------

//View matrix to transform from world space to view space.
uniform mat4 view_matrix;

//Projection matrix to transform from view space to clip space.
uniform mat4 projection_matrix;

//Vertex position in world space.
attribute vec3 position;

//Color of the vertex. This is used to calculate the position inside the volume cube.
attribute vec4 color;

//--------------------------------------------- Outputs --------------------------------------------

//Vertex position in view space (homogeneous coordinates).
varying vec4 pos_v;

//Vertex position in projection space (homogeneous coordinates).
varying vec4 pos_vp;

//Texture coordinates.
varying vec4 texcoords;

//Color of the vertex.
varying vec4 col;

//------------------------------------------ Main Program ------------------------------------------

void main()
{
	col = color;
	texcoords = color;
	pos_v = view_matrix * vec4(position, 1.0);
	pos_vp = projection_matrix * pos_v;
	
	vec4 p = pos_vp;
	p.z = -0.0001 * (abs(p.z) + 0.5);
	//p.z = 0; //avoid clipping of fragments of the cube, we need the fragment and perform clipping manually
	
	gl_Position = p;
}
