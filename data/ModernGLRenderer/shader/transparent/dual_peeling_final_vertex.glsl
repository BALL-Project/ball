#version 120


//-------------------------------------------- Inputs ----------------------------------------------

//Corner position of the screen space quad.
attribute vec3 position;

//Texture coordinates of the screen space quad.
attribute vec2 texture_coordinates;


//--------------------------------------------- Outputs --------------------------------------------

//Texture coordinates of the screen space quad.
varying vec2 texcoords;


//------------------------------------------ Main Program ------------------------------------------

void main()
{
	texcoords = texture_coordinates;

	gl_Position = vec4(position, 1.0);
}
