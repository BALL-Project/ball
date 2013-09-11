#version 120

// Maximal number of clipping planes. Must be the same number as in cpp code.
const int MAX_CLIPPING_PLANES = 12;

// Stores the clipping planes in form Ax + By + Cz + D = 0.
// The plane equations need to be normalized.
uniform vec4 clipping_planes[MAX_CLIPPING_PLANES]; 

// Index of the highest clipping plane used + 1. Must not be greater than MAX_CLIPPING_PLANES.
uniform int used_clipping_planes;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform vec4 clip_range;

/**
 * Discards all fragments which are on the right side of on of the clipping planes. The clipping
 * planes are specified by a half-space using a four-component plane equation.
 *
 * If the dot product of the eye coordinates of a vertex with the stored plane equation components
 * is positive, the vertex is out with respect to that clipping plane. Otherwise, it is in.
 *
 * @param frag_pos The fragment position in eye space to be tested for clipping.
 * @return void
 **/
void clip(in vec4 frag_pos)
{
	for(int i = 0; i < used_clipping_planes; i++) {
		vec4 plane = clipping_planes[i];
		
		float dist = dot(frag_pos.xyz,plane.xyz) +  plane.w;
		if (dist < 0) {
			discard;
		}
	}
}

/**
 * Discards all fragments which are on the right side of on of the clipping planes. The clipping
 * planes are specified by a half-space using a four-component plane equation.
 *
 * If the dot product of the eye coordinates of a vertex with the stored plane equation components
 * is positive, the vertex is out with respect to that clipping plane. Otherwise, it is in.
 *
 * @param intersection_near The fragment position in eye space to be tested for clipping.
 * @param intersection_far The second intersection with the object.
 * @param frag_normal The normal to be set.
 * @param plane Four-component plane equation used for clipping.
 * @return void
 **/
void clip(inout vec4 intersection_near, in vec4 intersection_far, inout vec4 frag_normal, in vec4 plane)
{
	float d_intersection_near_plane = dot(intersection_near.xyz,plane.xyz) + plane.w;
	float d_intersection_far_plane = dot(intersection_far.xyz,plane.xyz) + plane.w;
	

	
	if(d_intersection_near_plane < 0) { // we are at the right side of the plane
		if(d_intersection_far_plane > 0) { //perform clipping
			intersection_near.xyz = intersection_near.xyz + plane.xyz * d_intersection_near_plane;
			frag_normal = -plane;
		} else {
			discard;
			
		}
	}
}

/**
 * Discards all fragments which are on the right side of on of the clipping planes. The clipping
 * planes are specified by a half-space using a four-component plane equation.
 *
 * If the dot product of the eye coordinates of a vertex with the stored plane equation components
 * is positive, the vertex is out with respect to that clipping plane. Otherwise, it is in.
 *
 * @param intersection_near The fragment position in eye space to be tested for clipping.
 * @param intersection_far The second intersection in eye space with the object.
 * @param frag_normal The normal to be set.
 * @return void
 **/
void clip(inout vec4 intersection_near, in vec4 intersection_far, inout vec4 frag_normal)
{
	for(int i = 0; i < MAX_CLIPPING_PLANES; i++) {
		//if(i < used_clipping_planes) { //there is no dynamic branching in the mesa driver, however with clip(in vec4) it works?!
			clip(intersection_near, intersection_far, frag_normal, clipping_planes[i]);
		//} else {
		//	break;
		//}
	}
}

/**
 * Perform clipping for the near plane. The depth for near and far intersection with the impostor is
 * tested. If both are in front of the clipping plane, the fragment is discarded. If one point falls
 * in the viewers side of the plane and one behind, the fragment needs to be clipped. The fragment
 * is moved to the near clipping plane and the normal overwritten with (0,0,1) to create the
 * impression of a flat surface. If a second fragment is clipped to the same depth, it would be
 * drawn over the fist one. The capped fragment will be set to a small positive number which depends
 * on the original computed depth value z. Thus, the new depth is set to eps * (K + z) with a small
 * eps and a constant K.
 *
 * @param intersection_near The fragment position in view space to be tested for clipping.
 * @param intersection_far The second intersection in view space with the object.
 * @param frag_normal The normal to be set.
 * @return bool
 *
 **/
bool clipNearPlane(inout vec4 intersection_near, in vec4 intersection_far, inout vec4 frag_normal)
{
	float z = -clip_range.x;
	float d_intersection_near_plane = intersection_near.z;
	float d_intersection_far_plane = intersection_far.z;
	
	if(d_intersection_near_plane > z-0.001) { // we are in front of the plane
		if(d_intersection_far_plane < z) { //perform clipping
			intersection_near.z = -clip_range.x;
			frag_normal = vec4(0,0,1,1);
			gl_FragDepth = 0.0001 * (abs(d_intersection_near_plane) + 0.5);
			return true;
		} else {
			discard;
		}
	}
	return false;
}

/**
 * Perform clipping for the near plane. The depth for near and far intersection with the fragment is
 * tested. If both are in front of the clipping plane, the fragment is discarded. If one point falls
 * in the viewers side of the plane and one behind, the fragment needs to be clipped. The fragment
 * is moved to the near clipping plane and the normal overwritten with (0,0,1) to create the
 * impression of a flat surface. If a second fragment is clipped to the same depth, it would be
 * drawn over the fist one. The capped fragment will be set to a small positive number which depends
 * on the original computed depth value z. Thus, the new depth is set to eps * (K + z) with a small
 * eps and a constant K.
 *
 * @param z_near The fragment depth in view space to be tested for clipping.
 * @param z_far The depth of the backside of the obejct in view space.
 * @return bool
 *
 **/
bool clipNearPlane(inout float z_near, in float z_far)
{
	if(z_near > -clip_range.x-0.001) { // we are in front of the plane
			if(z_far < -clip_range.x) { // perform clipping
				gl_FragDepth = 0.0001 * (abs(z_near) + 0.5);
				z_near = -clip_range.x;
				return true;
			} else {
				discard;
			}
		}
	return false;
}
