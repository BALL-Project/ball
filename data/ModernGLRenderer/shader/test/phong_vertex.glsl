attribute vec3 vertex_position;
attribute vec3 vertex_normal;

uniform vec4 material_ambient;
uniform vec4 material_diffuse;
uniform mat4 view_matrix;
uniform mat4 view_projection_matrix;

varying vec4 diffuse,ambient;
varying vec3 normal,lightDir,halfVector;

void main()
{
	/* first transform the normal into eye space and
	normalize the result */
	normal = normalize(vec3(view_matrix * vec4(vertex_normal, 0.0f)));

	/* now normalize the light's direction. Note that
	according to the OpenGL specification, the light
	is stored in eye space. Also since we're talking about
	a directional light, the position field is actually direction */
	lightDir = normalize(vec3(gl_LightSource[0].position));

	/* Normalize the halfVector to pass it to the fragment shader */
	halfVector = normalize(gl_LightSource[0].halfVector.xyz);
	/* Compute the diffuse, ambient and globalAmbient terms */

	diffuse = material_diffuse * gl_LightSource[0].diffuse;
	ambient = material_ambient * gl_LightSource[0].ambient;
	ambient += gl_LightModel.ambient * material_ambient;

	gl_Position = view_projection_matrix * vec4(vertex_position, 1.0);
}
