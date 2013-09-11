#version 120

struct Material
{
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
  float shininess;
};

//-------------------------------------------- Inputs ----------------------------------------------

// Describes the frustum (near, Wfar, Hfar, far)
uniform vec4 clip_range;

//---------------------------- Interpolated Inputs from Vertex Shader ------------------------------
// Fragment position in world space (homogeneous coordinates).
varying vec4 mid;

// Fragment position in view space (homogeneous coordinates).
varying vec4 mid_v;

// texcoords of the point.
varying vec3 texc;

// 3D volume texture to be rendered.
uniform sampler3D volume_tex;

//-------------------------------------- Function Definition ---------------------------------------
void clip(in vec4 frag_pos);

void getPositionAndNormal(out vec4 position, out vec3 normal)
{

	//perform fragment based clipping.
	clip(mid);

	//we use a always front facing normal
	normal = vec3(0,0,1);

	position = mid_v;
}

void getMaterial(out Material material)
{
	vec4 color_sample = texture3D(volume_tex,texc);

	material.ambient = color_sample;
	material.diffuse = color_sample;
	material.specular = vec4(0,0,0,255);
	material.shininess = 255.;
}

void getFragDepth(out float depth)
{
	depth = gl_FragCoord.z;
}