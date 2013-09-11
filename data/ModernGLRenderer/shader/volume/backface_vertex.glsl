#version 120

/**
 * Vertex shader for backface rendering used in volume rendering.
 *
 * This shader is for calculating the the backface of the volume. The position is encoded in the
 * color value and written to the backface texture.
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

// View matrix to transform from world space to view space.
uniform mat4 view_matrix;

// Projection matrix to transform from view space to clip space.
uniform mat4 projection_matrix;

// Vertex position in world space.
attribute vec3 position;

// Color of the vertex. This is used to calculate the position inside the volume cube.
attribute vec4 color;

//--------------------------------------------- Outputs --------------------------------------------

// Color of the vertex.
varying vec4 col;

// Position in view space.
varying vec4 pos_v;

//------------------------------------------ Main Program ------------------------------------------

void main()
{
	col = color;
	
	pos_v = view_matrix * vec4(position, 1.0);

	gl_Position = projection_matrix  * pos_v;
}