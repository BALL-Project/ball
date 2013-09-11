varying vec4 diffuse,ambient;
varying vec3 normal,lightDir,halfVector;

uniform float material_shininess;
uniform vec4 material_specular;

void main()
{
	vec3 n,halfV;
	float NdotL,NdotHV;
	/* The ambient term will always be present */

	vec4 color = ambient;
	/* a fragment shader can't write a varying variable, hence we need
	   a new variable to store the normalized interpolated normal */
	n = normalize(normal);
	/* compute the dot product between normal and ldir */

	NdotL = max(dot(n,lightDir),0.0);

	if (NdotL > 0.0) {
		color += diffuse * NdotL;
		halfV = normalize(halfVector);
		NdotHV = max(dot(n,halfV),0.0);
		color += material_specular * gl_LightSource[0].specular * pow(NdotHV, material_shininess);
	}

	gl_FragColor = color;
}
