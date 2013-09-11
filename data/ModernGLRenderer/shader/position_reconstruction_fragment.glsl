#version 120

//-------------------------------------------- Inputs ----------------------------------------------

//Projection matrix to transform from clip space to view space.
uniform mat4 projection_matrix_inv;

//2D texture of the depth.
uniform sampler2D depth_map;

//describes the frustum (near, Wfar, Hfar, far)
uniform vec4 clip_range;

//describes the frustum planes (left, right, top, bottom)
uniform vec4 frustum;


//-------------------------------------- Function Definition ---------------------------------------

float decode (in vec4 rgba);

/**
 * Returns the depth value in [0,1).
 * @param uv Texture coordinates of the depth value to querry.
 **/
float getDepth(in vec2 uv)
{
	return decode(texture2D(depth_map, uv));
}

/**
 * Returns the depth value in [-farDist, 0] (view coordinates).
 * @param uv Texture coordinates of the depth value to querry.
 **/
float getLinearDepth(in vec2 uv)
{
	return getDepth(uv) * -clip_range.w;
}

/**
 * Returns the view space position.
 * @param uv Texture coordinates of the position to querry.
 **/
vec3 getPosition(in vec2 uv)    // uv.z = linear depth
{
// 	float depth =  getDepth(uv);
// 	vec4 pos_c = vec4(uv.x * 2.0f - 1.0f, (uv.y * 2.0f - 1.0f), depth, 1.0);
// 	vec4 pos_v  = projection_matrix_inv * pos_c;
// 	pos_v /= pos_v.w;
// 	return pos_v.xyz;

	vec2 ndc;             // Reconstructed NDC-space position
	vec3 eye;             // Reconstructed EYE-space position

	float left = frustum.x;
	float right = frustum.y;
	float near =1.5; //clip_range.x; //FIXME if we use mesa 9.0.2, the near= clip_range.x; causes artifacts, we do not know why.
	float far = clip_range.w;
	float bottom = frustum.z;
	float top = frustum.w;

	eye.z = getLinearDepth(uv);

	ndc = (uv-0.5) * 2. ;

// 	eye.x = ( (-ndc.x * eye.z) * (right-left)/(2*near)
//             - eye.z * (right+left)/(2*near) );
// 	eye.y = ( (-ndc.y * eye.z) * (top-bottom)/(2*near)
//             - eye.z * (top+bottom)/(2*near) );

   eye.x = (-ndc.x * eye.z) * right/near;
   eye.y = (-ndc.y * eye.z) * top/near;

  return eye;

}