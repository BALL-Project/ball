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
uniform vec4 material_specular;
uniform vec4 material_ambient;
uniform vec4 material_diffuse;

//Viewport (width and height of the rendering window).
uniform vec2 viewport;

//describes the frustum (near, Wfar, Hfar, far)
uniform vec4 clip_range;

// is the fragment clipable?
uniform int clipable;

//---------------------------- Interpolated Inputs from Vertex Shader ------------------------------
//Midpoint of the impostor in view space.
varying vec3  mid_v;
//Direction of the cylinder in view space.
varying vec3  dir_v;

//Radius of the cylinder.
varying float rad;

//Left color of the cylinder.
varying vec4 col1;

//Right color of the cylinder.
varying vec4 col2;

//Ration between the two colors in intervall [-1,1].
varying float col_ratio;

varying mat4 me_;
varying vec4 tc_c3_;

//Diagonal of the normalized diagonal form of the quadric.
varying vec4 diag_;


varying float early_discard; //1 for discard, 0 else

//Final color of the fragment.
vec4 color = vec4(0,0,0,1);

//-------------------------------------- Function Definition ---------------------------------------
void clip(inout vec4 intersection_near, in vec4 intersection_far, inout vec4 frag_normal);
bool clipNearPlane(inout vec4 intersection_near, in vec4 intersection_far, inout vec4 frag_normal);

//returns true if cap intersection
bool calculateCap(out vec3 intersection, out vec3 normal, in vec3 view_dir, in vec3 dir)
{
	vec3 n = normalize(dir);
	vec3 p = mid_v + dir;
	vec3 raydir = normalize(view_dir);
	float t = dot(n,p) / dot(n,raydir);

	intersection = raydir * t;
	normal = n;

	return length(p-intersection) < rad;
}


void calculateIntersection(out vec3 intersection, out vec3 intersection2, out vec3 normal)
{
	intersection = vec3(0); //this defaults should never be used, but removes usage before initialization warnings
	intersection2 = vec3(0); //this defaults should never be used, but removes usage before initialization warnings
	normal = vec3(0); //this defaults should never be used, but removes usage before initialization warnings
	vec4 view_c, view_e, pos, eqn, tmp;

	// inverse viewport transformation
	// (bug in original code, this here should be correct)
	view_c = vec4(2.0 * gl_FragCoord.xy / viewport - 1.0, 0.0, 1.0);

	//view direction in eye space
	view_e = projection_matrix_inv * view_c;

	//view direction in parameter space
	view_c = me_ * view_e;

	// quadratic equation
	tmp = diag_ * view_c;
	eqn.y = dot(tmp.xyw,tc_c3_.xyw);
	eqn.z = dot(tmp.xyw,view_c.xyw);

	eqn.w = (eqn.y * eqn.y) - eqn.z;

	// solve quadratic equation
	// x is the closer one, y the farther one
	vec2 quadratic_solution = vec2((eqn.w * -inversesqrt(eqn.w)) - eqn.y, (eqn.w * inversesqrt(eqn.w)) - eqn.y);
		 
	view_c = (tc_c3_ * quadratic_solution.x) + view_c;
	
	eqn.z = abs(view_c.z);
	eqn.x = view_c.w - eqn.z;
	eqn.y = view_c.w - eqn.z;
	eqn.z = view_c.w - eqn.z;

	vec3 intersection_right, normal_right;
	vec3 intersection_left,  normal_left;

	bool hasRightCap = calculateCap(intersection_right, normal_right, view_e.xyz, dir_v);
	bool hasLeftCap  = calculateCap(intersection_left, normal_left, view_e.xyz, -dir_v);
	bool hasCap = hasLeftCap || hasRightCap;

	//discriminant < 0 or off length => kill
	//but not a cap
	if((eqn.x < 0.0 || eqn.w < 0.0) && !hasCap) {
		discard;
	}

	//compute normal, intersection and depth if we are looking at cylinder
	if(!(eqn.x < 0.0 || eqn.w < 0.0)) {

		intersection  = view_e.xyz / (view_e.w + projection_matrix_inv[2].w * quadratic_solution.x);

		//calculate normal by projecting intersection orthogonal onto cylinder axis
		vec3 a = normalize(dir_v);
		vec3 b = intersection - mid_v;
		vec3 b_a =dot(b, a) * a;
		
		normal = normalize(b - b_a);
		
		if(dot(b,a) >= length(dir_v) * col_ratio) {
			color = col1;
		} else {
			color = col2;
		}
	}

	//set cap normals, intersection and depth if we are looking at caps
	if(hasCap){
		if(hasRightCap && intersection_right.z > intersection_left.z) {
			color = col1;
			normal = normal_right;
			intersection = intersection_right;
		} else if(hasLeftCap && intersection_right.z < intersection_left.z) {
			color = col2;
			normal = normal_left;
			intersection = intersection_left;
		}
	}

	//determine which is the second intersection

	//we are looking at the tube
	if(!(eqn.x < 0.0 || eqn.w < 0.0)) {

		if(hasLeftCap && intersection.z > intersection_left.z) {
			//first intersection: tube
			//second intersection: left cap
			intersection2 = intersection_left;
		} else if(hasRightCap && intersection.z > intersection_right.z) {
		  //first intersection: tube
			//second intersection: right cap
			intersection2 = intersection_right;
		} else {
			//first intersection: tube
			//second intersection: tube
			intersection2 = view_e.xyz / (view_e.w + projection_matrix_inv[2].w * quadratic_solution.y);
		}
	}
	
	//we are looking at a cap
	if(hasCap){
	  //we are looking at right cap
		if(hasRightCap && intersection_right.z > intersection_left.z) {
			if(hasLeftCap){
			  //second intersection is left cap
				intersection2 = intersection_left;
			} else {
				//second intersection is tube
				intersection2 = view_e.xyz / (view_e.w + projection_matrix_inv[2].w * quadratic_solution.y);
			}
		//we are looking at left cap
		} else if(hasLeftCap && intersection_right.z < intersection_left.z) {
			if(hasRightCap){
			//second intersection is right cap
				intersection2 = intersection_right;
			} else {
				//second intersection is tube
				intersection2 = view_e.xyz / (view_e.w + projection_matrix_inv[2].w * quadratic_solution.y);
			}
		}
	}
}

void getPositionAndNormal(out vec4 position, out vec3 normal)
{
	//discard everythin which does not pass the early discard test
	if(early_discard > 0) {
		discard;
	}

	vec3 intersection;
	vec3 intersection2;

	//calculate intersection and normal
	calculateIntersection(intersection, intersection2, normal);

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
	material.diffuse = material_diffuse * color;
	material.specular = material_specular;
	material.shininess = material_shininess;
}

void getFragDepth(out float depth)
{
	depth = gl_FragDepth;
}