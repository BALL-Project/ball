#version 120

struct Material
{
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
  float shininess;
};

//-------------------------------------------- Inputs ----------------------------------------------
//Materials.
uniform float material_shininess;
uniform vec4 material_specular;
uniform vec4 material_ambient;
uniform vec4 material_diffuse;

//---------------------------- Interpolated Inputs from Vertex Shader ------------------------------
//Vertex normal in view coordinates.
varying vec3 norm_v;

//Fragment position in world space (homogeneous coordinates).
varying vec4 pos;

//Fragment position in view space (homogeneous coordinates).
varying vec4 pos_v;

//Color of the fragment.
varying vec4 col;

//-------------------------------------- Function Definition ---------------------------------------

void clip(in vec4 frag_pos);

void getPositionAndNormal(out vec4 position, out vec3 normal)
{
	//perform fragment based clipping.
	clip(pos);
	
	position = pos_v;
	normal = norm_v;
}

void getMaterial(out Material material)
{
	material.ambient = material_ambient;
	material.diffuse = material_diffuse * col;
	material.specular = material_specular;
	material.shininess = material_shininess;
}


void getFragDepth(out float depth)
{
	depth = gl_FragCoord.z;
}
