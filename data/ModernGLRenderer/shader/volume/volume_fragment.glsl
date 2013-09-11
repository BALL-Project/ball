#version 120

/**
 * Fragment shader for volume rendering.
 *
 * This algorithm sends one ray per screen pixel and traces this through the volume.
 *
 * We render a cube where colors represent coordinates. A direction vector is gerated by subtracting
 * the backface (prerendered and stored in a texture) from the the front face of the cube. The
 * direction vector and the front facing intersection can be used to step through the 3D volume
 * texture with a predefined stepsize delta and accumulate the opacity and color value according to
 * the nature of the volume data set.
 * We end the loop when the oparity is near to one, we leave the volume, or we intersect with an
 * opaque fragment.
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

// Projection matrix to transform from view space to clip space.
uniform mat4 projection_matrix;

// Stepsize, used to step through the volume.
uniform float stepsize;

// Texture of the backface of the volume cube. Color corresponds to position in 3D texture.
uniform sampler2D backface_tex;

// Texture of the backface position of the volume cube.
uniform sampler2D backface_pos_tex;

// 2D texture of the normals (of the opaque fragments).
uniform sampler2D normal_map;

// 3D volume texture to be rendered.
uniform sampler3D volume_tex;

//2D texture of the depth.
uniform sampler2D depth_map;



//---------------------------- Interpolated Inputs from Vertex Shader ------------------------------

// Vertex position in view space (homogeneous coordinates).
varying vec4 pos_v;

// Vertex position in projection space (homogeneous coordinates).
varying vec4 pos_vp;

// Texture coordinates.
varying vec4 texcoords;

// Color of the vertex.
varying vec4 col;

// Describes the frustum (near, Wfar, Hfar, far)
uniform vec4 clip_range;


//-------------------------------------- Function Definition ---------------------------------------

float decode (in vec4 rgba);

bool clipNearPlane(inout float z_near, in float z_far);

//------------------------------------------ Main Program ------------------------------------------

void main()
{

	// find the right place to lookup in the backside buffer
	vec2 texc = ((pos_vp.xy / pos_vp.w) + 1) / 2;

	// position of the backface in view space
	float far_z = decode(texture2D(backface_pos_tex, texc)) * -clip_range.w;

	// the start position of the ray is stored in the texture coordinate
	vec4 start = texcoords;

	// position of the backface
	vec4 back_position = texture2D(backface_tex, texc);

	float z_clip_v = pos_v.z;

	if(clipNearPlane(z_clip_v, far_z)) {
		// compute color of the clipped fragment
		float dist_far = far_z - z_clip_v;
		float dist_total = far_z - pos_v.z;
		float frac_far = dist_far / dist_total;
		vec4 new_col = col * frac_far + back_position * (1-frac_far);
		start = new_col;
	}

	// direction of the ray traced through the volume.
	vec3 dir = back_position.xyz - start.xyz; 

	// the length from front to back is calculated and used to terminate the ray
	float len = length(dir.xyz);

	// normalized direction vector
	vec3 norm_dir = normalize(dir);

	// stepsize used for tracing through the volume
	float delta = stepsize;
	vec3 delta_dir = norm_dir * delta;
	float delta_dir_len = length(delta_dir);

	// vector used for tracing through the volume
	vec3 vec = start.xyz; 

	// accumulated color, opacity and length
	vec4 col_acc = vec4(0,0,0,0); 
	float alpha_acc = 0;
	float length_acc = 0; 

	// sample read (color, opacity) from the 3D texture
	vec4 color_sample; 
	float alpha_sample; 
	
	//distance between first and second intersection with the cube (view space)
	float dist_v_z = far_z - z_clip_v;
	float delta_v = dist_v_z * stepsize;

	// position of the fist opaque fragment (view space)
	float opaque_depth_v = decode(texture2D(depth_map, texc)) * -clip_range.w;

	// normal of the fist opaque fragment
	vec4 nmap_sample = texture2D(normal_map, texc);

	// current position in the volume (view space)
	float curr_depth_v  = z_clip_v;


	// iterate and trace ray through volume
	// accumulate opacity and color until we hit a solid object or leave the volume.
	// Loop also ends if a solid object is infront of the volume.
  for(int i = 0; i < 450; i++)
	{
		color_sample = texture3D(volume_tex,vec);
		alpha_sample = color_sample.a * stepsize;
		col_acc   += (1.0 - alpha_acc) * color_sample * alpha_sample * 3;
		alpha_acc += alpha_sample;
		vec += delta_dir;
		length_acc += delta_dir_len;

		curr_depth_v += delta_v;
		
		// take only valid fragments
		if(!(abs(nmap_sample.w) <= 0.1)){
			// stop if we interect with an opaque object
			if(opaque_depth_v >= curr_depth_v) {
				break;
			}
		}
		
		if(length_acc >= len || alpha_acc > 0.95) break; // terminate if opacity > 1 or the ray is outside the volume
	}
	gl_FragData[0] = col_acc;
}
