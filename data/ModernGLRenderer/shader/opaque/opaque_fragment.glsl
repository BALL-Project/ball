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

//-------------------------------------- Function Definition ---------------------------------------

void getPositionAndNormal(out vec4 position, out vec3 normal);

void getMaterial(out Material material);

vec4 encode(in float fDist);

//------------------------------------------ Main Program ------------------------------------------
void main()
{
	vec4 p;
	vec3 n;
	
	Material material;
	
	getPositionAndNormal(p,n);

	
	getMaterial(material);

	//set the frag data used for compositing 
	gl_FragData[0] = material.ambient;
	gl_FragData[1] = material.diffuse;
	gl_FragData[2] = vec4(material.specular.rgb, material.shininess / 255.0);
	gl_FragData[3] = vec4(0.5 * n + 0.5, 1.0);
	gl_FragData[4] = encode(p.z / -clip_range.w);
}

