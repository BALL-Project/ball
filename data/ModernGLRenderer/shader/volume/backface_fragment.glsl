#version 120

/**
 * Fragment shader for backface rendering used in volume rendering.
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

//---------------------------- Interpolated Inputs from Vertex Shader ------------------------------

// Color of the vertex.
varying vec4 col;

// Position in view space;
varying vec4 pos_v;

// Describes the frustum (near, Wfar, Hfar, far)
uniform vec4 clip_range;

//-------------------------------------- Function Definition ---------------------------------------

vec4 encode (in float data);

//------------------------------------------ Main Program ------------------------------------------

void main()
{

	gl_FragData[0] = col;
	gl_FragData[1] =  encode(pos_v.z / -clip_range.w); // store position of the backface
}
