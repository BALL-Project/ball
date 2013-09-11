#version 120


//2D texture of the SSAO component.
uniform sampler2D ssao_map;

//2D texture of the volume.
uniform sampler2D volume_map;

//2D texture of the volume.
uniform sampler2D normal_map;

#extension GL_ARB_texture_rectangle : enable
uniform sampler2DRect FrontBlenderTex;
uniform sampler2DRect BackBlenderTex;

//true if ambient occlusion should be used.
uniform bool use_ao;

//true if volume should be used.
uniform bool use_volume;


//Clear color =^ background color
uniform vec4 clear_color;


//---------------------------- Interpolated Inputs from Vertex Shader ------------------------------

//Texture coordinates of the screen space quad.
varying vec2 texcoords;


//-------------------------------------- Function Definition ---------------------------------------
float decode (in vec4 rgba);

//------------------------------------------ Main Program ------------------------------------------

void main(void)
{

 	vec4 nmap_sample = texture2D(normal_map, texcoords);
	vec4 volume = texture2D(volume_map, texcoords);
	
	vec4 frontColor = texture2DRect(FrontBlenderTex, gl_FragCoord.xy);
	vec3 backColor = texture2DRect(BackBlenderTex, gl_FragCoord.xy).rgb;
	float alphaMultiplier = 1.0 - frontColor.w;
	
	vec4 color;

	// front + back
	color.rgb = frontColor.rgb + backColor * alphaMultiplier;
	color.w = 1;
	
	// front blender
	//gl_FragColor.rgb = frontColor.rgb + vec3(alphaMultiplier);
	
	// back blender
	//gl_FragColor.rgb = backColor;
	
	if(use_ao) {
		float a = decode(texture2D(ssao_map, texcoords));
 		color -= vec4(a,a,a,0);
	}
	
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
