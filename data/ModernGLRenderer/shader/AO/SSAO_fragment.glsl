#version 120

//-------------------------------------------- Inputs ----------------------------------------------
//Projection matrix to transform from clip space to view space.
uniform mat4 projection_matrix_inv;

//Viewport (width and height of the rendering window).
uniform vec2 viewport;

//2D texture of the normals.
uniform sampler2D normal_map;

//Texture to create random normals from.
uniform sampler2D rand_norm;

//size of the random texture.
uniform float random_size;

//The sampling radius.
uniform float g_sample_rad;

//The ao intensity.
uniform float g_intensity;

//Scales distance between occluders and occlude.
uniform float g_scale;

//Controls the width of the occlusion cone considered by the occludee.
uniform float g_bias;

//---------------------------- Interpolated Inputs from Vertex Shader ------------------------------
//Direction from the camera.
varying vec3 camera_dir;

//Texture coordinates of the screen space quad.
varying vec2 texcoords;

//-------------------------------------- Function Definition ---------------------------------------

vec4 encode (in float data);
float computeAO(in vec3 p, in vec3 n);
float computeAOPoission(in vec3 p, in vec3 n);


vec3 getPosition(in vec2 uv);

vec2 getRandom(in vec2 uv)
{
 return normalize(texture2D(rand_norm, viewport * uv / random_size).xy * 2.0 - 1.0);
}

float doAmbientOcclusion(in vec2 tcoord, in vec2 uv, in vec3 p, in vec3 cnorm)
{
 vec3 diff = getPosition(tcoord + uv) - p;
 vec3 v = normalize(diff);
 float d = length(diff)*g_scale;
 return max(0.0, dot(cnorm, v) - g_bias)*(1.0/(1.0+d))*g_intensity;
}

float computeAO(in vec3 p, in vec3 n)
{
	const vec2 samples[] = vec2[](
		vec2( 1, 0),
		vec2(-1, 0),
		vec2( 0, 1),
		vec2( 0,-1)/*,
		vec2(0.707, 0.707),
		vec2(-0.707, 0.707),
		vec2(0.707, -0.707),
		vec2(-0.707, -0.707)*/
	);

	vec2 rand = getRandom(texcoords);

	float ao = 0.0f;
	float rad = g_sample_rad / p.z;

	//**SSAO Calculation**//
	int iterations = 4;
	for (int j = 0; j < iterations; ++j)
 	{
		vec2 coord1 = reflect(samples[j], rand) * rad;
  	vec2 coord2 = vec2(coord1.x*0.707 - coord1.y*0.707,
		                   coord1.x*0.707 + coord1.y*0.707);
  
		ao += doAmbientOcclusion(texcoords, coord1 * 0.25, p, n);
		ao += doAmbientOcclusion(texcoords, coord2 * 0.5,  p, n);
		ao += doAmbientOcclusion(texcoords, coord1 * 0.75, p, n);
		ao += doAmbientOcclusion(texcoords, coord2,        p, n);
	}

	ao /= float(iterations)*4.0;
	//**END**//

	return clamp(ao, 0, 0.99999);
 	//return ao;
}

float computeAOPoission(in vec3 p, in vec3 n)
{
	#define NUM_SAMPLES 16
	const vec2 samples[NUM_SAMPLES] = vec2[](
		vec2(-0.614, 0.026000000000000023),
		vec2(-0.8999054511853396, -0.34576009041382594),
		vec2(-0.20072497518976729, -0.3498483747236415),
		vec2(-0.26918139353818604, 0.27634441429696355),
		vec2(-0.7166901708332754, 0.5616473363031236),
		vec2(-0.5927248821951132, -0.7200480178942895),
		vec2(0.17384010996931587, 0.2464177654416937),
		vec2(0.05775686192030305, -0.7642375025082713),
		vec2(-0.4720950216162708, 0.9958957689838652),
		vec2(0.17501054715965103, 0.9718281902837984),
		vec2(-0.9676781231517022, 0.9197416112244183),
		vec2(0.8671876869808779, 0.8339274946645288),
		vec2(0.5984528091508641, -0.14111019566149885),
		vec2(0.7055599181139718, -0.7760923914921121),
		vec2(0.4956370453259358, 0.5553150122287727),
		vec2(0.9893525872761428, 0.3225829068927095)
	);
	
//8
// 		vec2(-0.648, 0.99),
// 		vec2(-0.4570547313698292, 0.29504388403404613),
// 		vec2(-0.04999831183303127, 0.7810363550907442),
// 		vec2(0.43358144020586153, 0.2587423773385915),
// 		vec2(-0.3150799230275879, -0.47613401462960714),
// 		vec2(0.9316609865805228, -0.3630474338853231),
// 		vec2(0.9990617349738309, 0.7060620563962838),
// 		vec2(0.345408837663127, -0.6584819499117867)

//32
// vec2(0.81, 0.9219999999999999),
// vec2(0.5350748579034685, 0.47215535449656554),
// vec2(0.38756186234206425, 0.957169253417226),
// vec2(0.8555965193120882, 0.6005076609112387),
// vec2(-0.18434809853591072, 0.9259634538070198),
// vec2(0.13307527949445808, 0.49161575934678803),
// vec2(0.189170177240076, 0.15997080045387646),
// vec2(-0.4197964213352193, 0.35441580782455917),
// vec2(-0.17442758382066004, 0.5996269247479102),
// vec2(-0.11719144188647601, 0.15602283635239012),
// vec2(0.32002368182301466, -0.31155181978798463),
// vec2(-0.07898641450623467, -0.18360969400506644),
// vec2(0.6423452271313821, -0.016268766316568595),
// vec2(0.3963091332618731, -0.830734571016804),
// vec2(-0.047128270746146894, -0.6366621246293666),
// vec2(0.7472559601303563, -0.505043156217909),
// vec2(0.9181282729918598, -0.7611253646557943),
// vec2(0.8839260433414211, -0.2220490924208548),
// vec2(-0.5920244841653212, -0.11789488071766052),
// vec2(-0.726512266593569, 0.440408884489099),
// vec2(-0.4982796407327905, 0.7378014073999972),
// vec2(-0.9987478661086301, 0.28264131913268953),
// vec2(0.9397254500316048, 0.14394891022191558),
// vec2(0.06809330023447902, -0.9352302027606625),
// vec2(-0.8991188966154557, 0.7002124203560653),
// vec2(-0.9526327329073437, -0.08209549534763705),
// vec2(-0.49645224561710244, -0.5826986736204149),
// vec2(-0.9183534115583933, -0.5443232538242705),
// vec2(-0.2855900857395729, -0.999279848403561),
// vec2(-0.9890974785584439, -0.8847769556241915),
// vec2(-0.6320452578076907, -0.9053113785579703),
// vec2(-0.7263597683216909, 0.9801793003266972)



	vec2 rand = getRandom(texcoords);

	float ao = 0.0f;
	float rad = g_sample_rad / p.z;

	//**SSAO Calculation**//
	int iterations = NUM_SAMPLES;
	for (int j = 0; j < iterations; ++j)
 	{
		vec2 coord = reflect(samples[j], rand) * rad;
		ao += doAmbientOcclusion(texcoords, coord, p, n);
	}

	ao /= float(iterations);
	//**END**//

	return clamp(ao, 0, 0.99999);
}

//------------------------------------------ Main Program ------------------------------------------
void main()
{
	vec4 nmap_sample = texture2D(normal_map, texcoords);

	if(nmap_sample.w == 0.0) {
		discard;
	}

	vec3 normal = normalize(2.0*nmap_sample.rgb - 1.0);
	vec3 position = getPosition(texcoords);

	gl_FragData[0] = encode(computeAO(position, normal));
}
