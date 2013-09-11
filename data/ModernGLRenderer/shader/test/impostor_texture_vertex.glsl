attribute vec3 vertex_position;
attribute vec3 vertex_normal;

uniform mat4 view_matrix;
uniform mat4 view_projection_matrix;

varying vec3 normal;

void main()
{
	/* first transform the normal into eye space and
	normalize the result */
	normal = vec3(view_matrix * vec4(vertex_normal, 0.0f));

	gl_Position = view_projection_matrix * vec4(vertex_position, 1.0);
}