#version 120

//-------------------------------------------- Inputs ----------------------------------------------

#extension GL_ARB_texture_rectangle : enable
uniform sampler2DRect TempTex;

//------------------------------------------ Main Program ------------------------------------------

void main(void)
{
	gl_FragColor = texture2DRect(TempTex, gl_FragCoord.xy);
	// for occlusion query
	if (gl_FragColor.a == 0.) discard;
}
