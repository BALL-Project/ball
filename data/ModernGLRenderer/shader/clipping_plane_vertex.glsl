#version 120

/**
 * Contains tests for discarding fragments early. Fragments are discarded if they are further than
 * the radius away from the clipping side of a clipping plane or outside the viewable area e.g.
 * behind the near plane.
 **/

// Maximal number of clipping planes. Must be the same number as in cpp code.
const int MAX_CLIPPING_PLANES = 12;

// Stores the clipping planes in form Ax + By + Cz + D = 0.
// The plane equations need to be normalized.
uniform vec4 clipping_planes[MAX_CLIPPING_PLANES]; 

// Index of the highest clipping plane used + 1. Must not be greater than MAX_CLIPPING_PLANES.
uniform int used_clipping_planes;

/**
 * Test if a pointsprite fragment may be discarded. Function returns 1 (discard) if the midpoint of
 * the pointsprite fragment is further than the radius away from a clipping plane. Thus, the full
 * fragment will not be rendered and can safely be discarded early.
 *
 * @param midpoint The midpoint of the fragment.
 * @param radius The radius of the fragment.
 * @return 1 for discard, 0 else.
 * 
 **/
int test_early_discard(vec3 midpoint, float radius)
{
	for(int i = 0; i < used_clipping_planes; i++) {
		vec4 plane = clipping_planes[i];
		float d_mid_plane = dot(midpoint.xyz,plane.xyz) + plane.w;
		
		// discard the fragment if the midpoint is further than the radius away from the plane (normals
		// direct inside frustrum)
		if(-d_mid_plane > radius) {
			return 1;
		}
	}
	return 0;
}
