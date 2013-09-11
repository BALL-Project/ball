#version 120


//-------------------------------------------- Inputs ----------------------------------------------

//Corner position of the screen space quad.
attribute vec3 position;


//------------------------------------------ Main Program ------------------------------------------

void main()
{
	gl_Position = vec4(position, 1.0);
}
