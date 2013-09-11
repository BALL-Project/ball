varying vec3 normal;

void main()
{
	gl_FragColor = vec4(normalize(normal) / 2.0f + 0.5f, 1.0f);
}