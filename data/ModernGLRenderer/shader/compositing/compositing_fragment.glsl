#version 120

struct Material
{
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
  float shininess;
};

//-------------------------------------------- Inputs ----------------------------------------------
//View matrix to transform from world space to view space.
uniform mat4 view_matrix;

//2D texture of the diffuse color.
uniform sampler2D ambient_map;

//2D texture of the diffuse color.
uniform sampler2D diffuse_map;

//2D texture of the specular color
uniform sampler2D specular_map;

//2D texture of the normals.
uniform sampler2D normal_map;

//2D texture of the SSAO component.
uniform sampler2D ssao_map;

//2D texture of the volume.
uniform sampler2D volume_map;

//true if ambient occlusion should be used.
uniform bool use_ao;

//true if volume should be used.
uniform bool use_volume;

//describes the frustum (near, Wfar, Hfar, far)
uniform vec4 clip_range;

//Clear color =^ background color
uniform vec4 clear_color;

//---------------------------- Interpolated Inputs from Vertex Shader ------------------------------
//Direction from the camera.
varying vec3 camera_dir;

//Texture coordinates of the screen space quad.
varying vec2 texcoords;

//-------------------------------------- Function Definition ---------------------------------------
float decode (in vec4 rgba);
float computeAO(in vec3 p, in vec3 n);

vec3 getPosition(in vec2 uv);

vec4 getColor(in vec4 pos_v, in vec3 norm_v, in Material material);


//------------------------------------------ Main Program ------------------------------------------
void main()
{
	vec4 nmap_sample = texture2D(normal_map, texcoords);
	vec4 volume = texture2D(volume_map, texcoords);

	//discard if we have no opaque fragment and no volume fragment or don't use the volume
	if(nmap_sample.w == 0.0 && (volume.a == 0.0 || !use_volume)) {
		discard;
	}

	//fragment position in view space
	vec3 pos_v = getPosition(texcoords);
	//normal direction in view space
	vec3 norm_v = normalize(2.0*(nmap_sample.rgb - 0.5));
	
	vec4 ambient  = texture2D(ambient_map, texcoords);
	vec4 diffuse  = texture2D(diffuse_map, texcoords);
	vec4 specular = texture2D(specular_map,texcoords);
	
	Material material = Material(
		ambient,
		diffuse,
		vec4(specular.xyz, 1.),
		specular.w * 255
	);
	
	//color computed by total lighting
	vec4 color = getColor(vec4(pos_v,1), norm_v, material);
 	
 	vec4 ao;
 	
 	if(use_ao) {
		float a = decode(texture2D(ssao_map, texcoords));
 		//color = vec4(1.0-a,1.0-a,1.0-a, 1.0);
 		color -= vec4(a,a,a,0);
	}/* else {
		float a = decode(texture2D(ssao_map, texcoords));
		color -= vec4(a,a,a,0);
	}*/
	
	//Blend the volume over the solid objects
	if(volume.a!=0. && use_volume)
	{
		if(nmap_sample.w == 0.0)
		{
			color = clear_color;
		}
		color = color*(1-volume.a) + volume*(volume.a);
	}
	gl_FragColor = color;
}
