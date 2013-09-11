#version 120


//-------------------------------------- Function Definition ---------------------------------------

void getPositionAndNormal(out vec4 position, out vec3 normal);

void getFragDepth(out float depth);


//------------------------------------------ Main Program ------------------------------------------

void main(void)
{
	vec4 p;
	vec3 n;
	getPositionAndNormal(p,n);
	
	float fragDepth;
	getFragDepth(fragDepth);
	
	gl_FragColor.xy = vec2(-fragDepth, fragDepth);
}
