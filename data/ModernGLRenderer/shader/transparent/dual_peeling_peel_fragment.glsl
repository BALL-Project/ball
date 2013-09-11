#version 120

//------------------------------------------- Structs ----------------------------------------------

struct Material
{
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
  float shininess;
};


//-------------------------------------------- Inputs ----------------------------------------------

#extension GL_ARB_texture_rectangle : enable
#extension GL_ARB_draw_buffers : require

uniform sampler2DRect DepthBlenderTex;
uniform sampler2DRect FrontBlenderTex;

#define MAX_DEPTH 1.0

//-------------------------------------- Function Definition ---------------------------------------

void calculate_lighting(in vec3 pos_v,
                        in vec3 norm_v,
                        inout Material material);

void getPositionAndNormal(out vec4 position, out vec3 normal);

void getMaterial(out Material material);

vec4 getColor(in vec4 pos_v, in vec3 norm_v, in Material material);

void getFragDepth(out float depth);


//------------------------------------------ Main Program ------------------------------------------

void main(void)
{
	vec4 position;
	vec3 normal;
	getPositionAndNormal(position, normal);
	
	// window-space depth interpolated linearly in screen space
	float fragDepth;
	getFragDepth(fragDepth);

	vec2 depthBlender = texture2DRect(DepthBlenderTex, gl_FragCoord.xy).xy;
	vec4 forwardTemp = texture2DRect(FrontBlenderTex, gl_FragCoord.xy);
	
	// Depths and 1.0-alphaMult always increase
	// so we can use pass-through by default with MAX blending
	gl_FragData[0].xy = depthBlender;
	
	// Front colors always increase (DST += SRC*ALPHA_MULT)
	// so we can use pass-through by default with MAX blending
	gl_FragData[1] = forwardTemp;
	
	// Because over blending makes color increase or decrease,
	// we cannot pass-through by default.
	// Each pass, only one fragment writes a color greater than 0
	gl_FragData[2] = vec4(0.0);

	float nearestDepth = -depthBlender.x;
	float farthestDepth = depthBlender.y;
	float alphaMultiplier = 1.0 - forwardTemp.w;

	if (fragDepth < nearestDepth || fragDepth > farthestDepth) {
		// Skip this depth in the peeling algorithm
		gl_FragData[0].xy = vec2(-MAX_DEPTH);
		return;
	}
	
	if (fragDepth > nearestDepth && fragDepth < farthestDepth) {
		// This fragment needs to be peeled again
		gl_FragData[0].xy = vec2(-fragDepth, fragDepth);
		return;
	}
	
	// If we made it here, this fragment is on the peeled layer from last pass
	// therefore, we need to shade it, and make sure it is not peeled any farther

	Material material;

	getMaterial(material);
	
	vec4 color = getColor(position, normal, material);
	
	gl_FragData[0].xy = vec2(-MAX_DEPTH);
	
	if (fragDepth == nearestDepth) {
		gl_FragData[1].xyz += color.rgb * color.a * alphaMultiplier;
		gl_FragData[1].w = 1.0 - alphaMultiplier * (1.0 - color.a);
	} else {
		gl_FragData[2] += color;
	}
}
