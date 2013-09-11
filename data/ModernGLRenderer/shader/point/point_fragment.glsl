#version 120

struct Material
{
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
  float shininess;
};

//-------------------------------------------- Inputs ----------------------------------------------

//describes the frustum (near, Wfar, Hfar, far)
uniform vec4 clip_range;

//---------------------------- Interpolated Inputs from Vertex Shader ------------------------------
//Color of the fragment.
varying vec4 col;

//Fragment position in world space (homogeneous coordinates).
varying vec4 pos;

//Fragment position in view space (homogeneous coordinates).
varying vec4 pos_v;

//-------------------------------------- Function Definition ---------------------------------------

void clip(in vec4 frag_pos);

void getPositionAndNormal(out vec4 position, out vec3 normal)
{
	//perform fragment based clipping.
	clip(pos);

	position = pos_v;

	//we don't need a normal for this
	normal = vec3(0,0,1);
}

void getMaterial(out Material material)
{
	material.ambient = col;
	material.diffuse = col;
	material.specular = vec4(0,0,0,255);
	material.shininess = 255.;
}

void getFragDepth(out float depth)
{
	depth = gl_FragCoord.z;
}
