#version 120

//------------------------------------------- Structs ----------------------------------------------

struct LightSource
{
  vec4 position;
  vec4 diffuse;
  vec4 specular;
  float constant_attenuation, linear_attenuation, quadratic_attenuation;
};

struct Material
{
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
  float shininess;
};

//-------------------------------------------- Inputs ----------------------------------------------

// up to gl_MaxLights (often 8)
uniform float number_of_lights;

const int max_number_of_lights = 8;

uniform LightSource lights[max_number_of_lights];

//Ambient light source
uniform vec4 ambient_light;

// Ambient color of the scene to give everything at least a little bit of color.
uniform vec4 scene_ambient;

//-------------------------------------- Function Definition ---------------------------------------

/**
 * Lighting is calculated using Blinn-Phong lighting model.
 * Iterates over all lightsources to calculate overall ambient, diffuse and specular colors. The
 * Method distinguishes between directional and point lightsources (depending on the w component of
 * the position). Additionally one ambient lightsource can be used and the scene ambient color is
 * added.
 *
 * @param pos_v Position of the vertex (and fragment) in view space.
 * @param norm_v Surface normal vector in view space.
 * @param material FrontMaterial of the fragment of the given position.
 **/
void calculate_lighting(in vec3 pos_v,
                        in vec3 norm_v,
                        inout Material material)
{
	//lights[0] = light0;
	//lights[1] = light1;

	vec3 normal_direction = normalize(norm_v);
	vec3 view_direction = -normalize(pos_v);
	vec3 light_direction = vec3(0.0, 0.0, 0.0);
	float attenuation = 1.0;

 	vec4 diffuse_out = vec4(0);
 	vec4 specular_out = vec4(0);

	// for all light sources
	for (int i = 0; i < max_number_of_lights; i++) 
	{
		//only iterate over the the number of set lights
		if(i >= number_of_lights) {
			break;
		}
		
		// directional light?
		if (0.0 == lights[i].position.w) 
		{
			attenuation = 1.0; // no attenuation
			light_direction = normalize(vec3(lights[i].position));
		} 
		// point light? 
		if (0.0 != lights[i].position.w)
		{
			vec3 position_to_light_source = vec3(lights[i].position.xyz - pos_v);
			float distance = length(position_to_light_source);
			attenuation = 1.0 / (lights[i].constant_attenuation
														+ lights[i].linear_attenuation * distance
														+ lights[i].quadratic_attenuation * distance * distance);
			light_direction = normalize(position_to_light_source);
		}

		float NdotL = dot(normal_direction, light_direction);
		vec4 diffuse_reflection = attenuation * (lights[i].diffuse) * max(0.0, NdotL);

		vec4 specular_reflection;
		
		// light source on the wrong side?
		if (NdotL < 0.0) 
		{
			// no specular reflection
			specular_reflection = vec4(0.0, 0.0, 0.0, 0.0);
		}
		// light source on the right side
		else 
		{
			vec3 halfV = normalize(view_direction + light_direction);
			float NdotHV = max(dot(normal_direction, halfV),0.0);
			specular_reflection = attenuation * lights[i].specular * pow(NdotHV, material.shininess);
		}
		diffuse_out += diffuse_reflection;
		specular_out += specular_reflection;
	}
	
	material.ambient = (scene_ambient + ambient_light) * material.ambient;
	material.diffuse = diffuse_out * material.diffuse;
	material.specular = specular_out * vec4(material.specular);
}

//-------------------------------------- Function Definition ---------------------------------------

/**
 * Lighting is calculated using Blinn-Phong lighting model.
 * Iterates over all lightsources to calculate overall ambient, diffuse colors. The
 * Method distinguishes between directional and point lightsources (depending on the w component of
 * the position). Additionally one ambient lightsource can be used and the scene ambient color is
 * added.
 *
 * @param pos_v Position of the vertex (and fragment) in view space.
 * @param norm_v Surface normal vector in view space.
 * @param material FrontMaterial of the fragment of the given position.
 **/
void calculate_lighting_no_specular(in vec3 pos_v,
                                    in vec3 norm_v,
                                    inout Material material)
{
	//lights[0] = light0;
	//lights[1] = light1;

	vec3 normal_direction = normalize(norm_v);
	vec3 view_direction = -normalize(pos_v);
	vec3 light_direction = vec3(0.0, 0.0, 0.0);
	float attenuation = 1.0;

 	vec4 diffuse_out = vec4(0);
 	vec4 specular_out = vec4(0);

	// for all light sources
	for (int i = 0; i < max_number_of_lights; i++) 
	{
		//only iterate over the the number of set lights
		if(i >= number_of_lights) {
			break;
		}
		
		// directional light?
		if (0.0 == lights[i].position.w) 
		{
			attenuation = 1.0; // no attenuation
			light_direction = normalize(vec3(lights[i].position));
		} 
		// point light? 
		if (0.0 != lights[i].position.w)
		{
			vec3 position_to_light_source = vec3(lights[i].position.xyz - pos_v);
			float distance = length(position_to_light_source);
			attenuation = 1.0 / (lights[i].constant_attenuation
														+ lights[i].linear_attenuation * distance
														+ lights[i].quadratic_attenuation * distance * distance);
			light_direction = normalize(position_to_light_source);
		}

		float NdotL = dot(normal_direction, light_direction);
		vec4 diffuse_reflection = attenuation * (lights[i].diffuse) * material.diffuse * max(0.0, NdotL);

		diffuse_out += diffuse_reflection;
	}
	
	material.ambient = scene_ambient * material.ambient + ambient_light * material.ambient;
	material.diffuse = diffuse_out;
	material.specular = vec4(0.);
}

/**
 * Calculates the final color of a fragment using Blinn-Phong lighting model.
 * 
 * The method distinguishes between front and back side of a fragment. Backside fragments are
 * rendered without specular highlight and only with a fraction of ambient and diffuse color to
 * give the impression of a darker inner side of an object.
 *
 * @param pos_v Position of the vertex (and fragment) in view space.
 * @param norm_v Surface normal vector in view space.
 * @param material FrontMaterial of the fragment of the given position.
 **/
vec4 getColor(in vec4 pos_v, in vec3 norm_v, in Material material)
{
	vec4 color = vec4(0);
	
	float s1 = -15.;
	float s2 = -20.;
	
	if(pos_v.z < s1) {
		material.specular.w = -(s1 - pos_v.z) / (s1 - s2) + 1;
		material.specular.rgb *= material.specular.w;
	}
	
	Material front_material = Material(
		material.ambient,
		material.diffuse,
		vec4(material.specular.xyz, 1.),
		material.shininess
	);
	
	Material back_material = front_material;
	
	if(pos_v.z > s2) {
		calculate_lighting(pos_v.xyz, norm_v, front_material);
	} else {
		calculate_lighting_no_specular(pos_v.xyz, norm_v, front_material);
	}

	color = front_material.ambient + front_material.diffuse + front_material.specular;
	
	calculate_lighting_no_specular(pos_v.xyz, -norm_v, back_material);
	
	color += 0.25 * back_material.ambient +  0.25 * back_material.diffuse;
	
	color.a = material.diffuse.a;
	
	return color;
}
