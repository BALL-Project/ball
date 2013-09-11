#version 120

/**
 * Mathematical background will be found in Sigg et al. / GPU-Based Ray Casting of Quadratic Surfaces
 */

struct Material
{
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
  float shininess;
};
 
//-------------------------------------------- Inputs ----------------------------------------------
//View matrix to transform from world space to view space.
uniform mat4 view_matrix;

//View matrix to transform from view space to world space.
uniform mat4 view_matrix_inv;

//Projection matrix to transform from view space to clip space.
uniform mat4 projection_matrix;

//Projection matrix to transform from clip space to view space.
uniform mat4 projection_matrix_inv;

//Materials
uniform float material_shininess;
uniform vec4  material_specular;
uniform vec4  material_ambient;
uniform vec4  material_diffuse;

//Viewport (width and height of the rendering window).
uniform vec2 viewport;

//describes the frustum (near, Wfar, Hfar, far)
uniform vec4 clip_range;

// is the fragment clipable?
uniform int clipable;

//---------------------------- Interpolated Inputs from Vertex Shader ------------------------------
//Midpoint of the impostor/sphere in view space.
varying vec3 mid_v;

//Radius of the sphere.
varying float rad;

//Color of the fragment.
varying vec4 col;


//Diagonal of the normalized diagonal form of the quadric.
varying vec4 diag_;
varying mat4 me;
varying vec4 mc_c2_;

varying float early_discard;  //1 for discard, 0 else

//-------------------------------------- Function Definition ---------------------------------------
void clip(inout vec4 intersection_near, in vec4 intersection_far, inout vec4 frag_normal);
bool clipNearPlane(inout vec4 intersection_near, in vec4 intersection_far, inout vec4 frag_normal);

//calculates the at most two intersections and the normal of the closer intersection
void calculateIntersection(out vec3 intersection, out vec3 intersection2, out vec3 normal)
{
vec4 view_c, view_e, eqn;

		// inverse viewport transformation
		// (bug in original code, this here should be correct)
		view_c = vec4(2.0*gl_FragCoord.xy / viewport - 1.0, 0.0, 1.0);

		//view direction in eye space
		view_e = projection_matrix_inv * view_c;

		//view direction in parameter space
		view_c = me * view_e;

		//quadratic equation
		vec4 tmp = diag_ * view_c;
		eqn.y = dot(tmp, mc_c2_);
		eqn.z = dot(tmp, view_c);

		eqn.w = (eqn.y * eqn.y) - eqn.z;

		if(eqn.w < 0.0) {
			discard;
		}
		
		vec2 quadratic_solutions = vec2(- sqrt(eqn.w) - eqn.y, sqrt(eqn.w) - eqn.y);

		intersection  = view_e.xyz / (view_e.w + projection_matrix_inv[2].w * quadratic_solutions.x);
		intersection2 = view_e.xyz / (view_e.w + projection_matrix_inv[2].w * quadratic_solutions.y);

		// Material properties
		normal = normalize(intersection - mid_v);
}

void getPositionAndNormal(out vec4 position, out vec3 normal)
{
	//discard everythin which does not pass the early discard test
	if(early_discard > 0) {
		discard;
	}

	vec3 intersection;
	vec3 intersection2;

		//orthographic case
	if(projection_matrix[3].w != 0) {

		// calculate normal from texture coordinates
    normal.xy = gl_PointCoord.xy*vec2(2.0, -2.0) + vec2(-1.0, 1.0);
    float mag = dot(normal.xy, normal.xy);
    if (mag > 1.0) discard;   // kill pixels outside circle

    normal.z = sqrt(1.0-mag);

		intersection = mid_v + normal*rad;
		vec3 n2= normal;
		n2.z *= -1;
		intersection2 = mid_v + n2*rad;


	} else {
		calculateIntersection(intersection, intersection2, normal);
	}

	vec4 n = view_matrix_inv * vec4(normal, 1);

	//transform intersection back to world coordiantes and appy clipping planes
	if(clipable != 0) {
		vec4 p = view_matrix_inv * vec4(intersection, 1);
		vec4 p2 = view_matrix_inv * vec4(intersection2, 1);

		clip(p, p2, n);

		intersection = (view_matrix * p).xyz;
		normal = (view_matrix * n).xyz;
	}

	vec4 proj_pos = projection_matrix * vec4(intersection, 1.0);
	float new_z = proj_pos.z / proj_pos.w;
	gl_FragDepth = (gl_DepthRange.diff * new_z + gl_DepthRange.near + gl_DepthRange.far) / 2.0;

	vec4 i = vec4(intersection, 1);
	if (clipNearPlane(i, vec4(intersection2, 1), n)) {
		intersection = i.xyz;
		normal = n.xyz;
	}

	position = vec4(intersection, 1);
}

void getMaterial(out Material material)
{
	material.ambient = material_ambient;
	material.diffuse = material_diffuse * col;
	material.specular = material_specular;
	material.shininess = material_shininess;
}

void getFragDepth(out float depth)
{
	depth = gl_FragDepth;
}