attribute vec3 vertex_position;
attribute vec3 vertex_normal;
attribute vec2 vertex_texcoord;

uniform mat4 view_projection_matrix;

varying vec2 texcoords;

void main()
{
	texcoords = vertex_texcoord;

	gl_Position = view_projection_matrix * vec4(vertex_position, 1.0);
}