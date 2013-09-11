#version 120

/**
 * Mathematical background will be found in Sigg et al. / GPU-Based Ray Casting of Quadratic Surfaces
 */

//-------------------------------------------- Inputs ----------------------------------------------
//View matrix to transform from world space to view space.
uniform mat4 view_matrix;

//View matrix to transform from view space to world space.
uniform mat4 view_matrix_inv;

//Projection matrix to transform from view space to clip space.
uniform mat4 projection_matrix;

//Projection matrix to transform from clip space to view space.
uniform mat4 projection_matrix_inv;

//Transpose of the view projection matrix.
uniform mat4 view_proj_matrix_transp;

//Viewport (width and height of the rendering window).
uniform vec2 viewport;

// is the fragment clipable?
uniform int clipable;

//Midpoint of the impostor in world space.
attribute vec3 midpoint;

//Direction of the cylinder in world space.
attribute vec3 direction;

//Left color of the cylinder.
attribute vec4 color1;

//Right color of the cylinder.
attribute vec4 color2;

//Radius of the impostor.
attribute float radius;

//Ratio between the two colors in intervall [0,1].
attribute float color_ratio;

//Diagonal of the normalized diagonal form of the quadric.
const vec4 diag       = vec4(1, 1, -1, -1);


//--------------------------------------------- Outputs --------------------------------------------
//Radius of the impostor.
varying float rad;

//Left color of the impostor.
varying vec4 col1;

//Right color of the impostor.
varying vec4 col2;

//Midpoint of the impostor in view space.
varying vec3 mid_v;

//Direction of the impostor in view space.
varying vec3 dir_v;

//Ration between the two colors in intervall [-1,1].
varying float col_ratio;

varying mat4 me_;
varying vec4 tc_c3_;

//Diagonal of the normalized diagonal form of the quadric.
varying vec4 diag_;

varying float early_discard; //1 for discard, 0 else


//-------------------------------------- Function Definition ---------------------------------------
//Homogeneous dot product.
float dph(vec4 a,vec4 b)
{
	return dot(a.xyz,b.xyz) + b.w;
}

int test_early_discard(vec3 midpoint, float radius);

//------------------------------------------ Main Program ------------------------------------------
void setup()
{
	//check if we can discard fragments which are beyond a clipping plane
	early_discard = test_early_discard(midpoint, radius);
	if(clipable != 0 && early_discard == 1) {
		gl_PointSize = 0;
		
	} else {
		early_discard = 0;
	
		rad = radius;
		col1 = color1;
		col2 = color2;
		col_ratio = (color_ratio * 2) - 1;
		mid_v = vec3(view_matrix * vec4(midpoint, 1.0));
		dir_v = vec3(view_matrix * vec4(direction, .0));

		//mat4 mvp = transpose(projection_matrix * view_matrix);
		mat4 mvp = view_proj_matrix_transp;

		
		vec4 tc_c0, d_tc_c0;
		vec4 tc_c1, d_tc_c1;
		vec4 tc_c3, d_tc_c3;
		vec4 tmp = vec4(0,0,0,0);
		vec4 eqn, bbox;
		vec2 pointsize;
		
		mat4 tc, me;
		
		
		// basis in object space
		
		// normalize direction
		me[2].w = dot(direction,direction);
		me[2].w = inversesqrt(me[2].w);
		me[2].xyz = direction * me[2].w;
		
		// compute normalized orthogonal vector
		me[0].xyz = cross(me[2].xyz,me[2].yzx);
		me[0].w = dot(me[0].xyz,me[0].xyz);
		me[0].w = inversesqrt(me[0].w);
		me[0].xyz = me[0].xyz * me[0].w;
		
		// compute orthogonal basis of lentgh radius
		me[0].xyz = me[0].xyz * radius;
		me[1].xyz = cross(me[0].xyz,me[2].xyz);
		
		// *** bbox for disk at z = -1 ***
		
		me[3].xyz = midpoint - direction;

		
		// screen planes in parameter space
		tc_c0.x = dot(me[0].xyz,mvp[0].xyz);
		tc_c0.y = dot(me[1].xyz,mvp[0].xyz);
		tc_c0.z = dph(me[3],mvp[0]);
		tc_c0.w = 0; //to remove not initialized warning;
		
		tc_c1.x = dot(me[0].xyz,mvp[1].xyz);
		tc_c1.y = dot(me[1].xyz,mvp[1].xyz);
		tc_c1.z = dph(me[3],mvp[1]);
		tc_c1.w = 0; //to remove not initialized warning;
		
		tc_c3.x = dot(me[0].xyz,mvp[3].xyz);
		tc_c3.y = dot(me[1].xyz,mvp[3].xyz);
		tc_c3.z = dph(me[3],mvp[3]);
		tc_c3.w = 0; //to remove not initialized warning;
		
		
		// multiply by sign (cylinder: {1,1,-1,0})
		d_tc_c0.xyz = (diag * tc_c0).xyz;
		d_tc_c1.xyz = (diag * tc_c1).xyz;
		d_tc_c3.xyz = (diag * tc_c3).xyz;
		
		// solve two quadratic equations (x,y)
		eqn.x = dot(d_tc_c3.xyz,tc_c0.xyz);
		eqn.z = dot(d_tc_c0.xyz,tc_c0.xyz);
		
		eqn.y = dot(d_tc_c3.xyz,tc_c1.xyz);
		eqn.w = dot(d_tc_c1.xyz,tc_c1.xyz);
		
		tmp.w = dot(d_tc_c3.xyz,tc_c3.xyz);
		tmp.w = 1.0 / tmp.w;
		eqn = eqn * tmp.w;
		
		// compute bbox
		eqn.zw = ((eqn.zwxy * eqn.zwxy) - eqn).zw;
		tmp.z = inversesqrt(eqn.z);
		tmp.w = inversesqrt(eqn.w);
		eqn.zw = (eqn * tmp).zw;
		// xy = max,zw = -min
		bbox = (eqn.xyxy * diag) + eqn.zwzw;
		
		// *** bbox for disk at z = +1 ***
		
		me[3].xyz = midpoint + direction;
		
		// recompute stuff involving me_c3
		tc_c0.z = dph(me[3],mvp[0]);
		tc_c1.z = dph(me[3],mvp[1]);
		tc_c3.z = dph(me[3],mvp[3]);
		
		// multiply by sign (cylinder: {1,1,-1,0})
		d_tc_c0.z = (diag * tc_c0).z;
		d_tc_c1.z = (diag * tc_c1).z;
		d_tc_c3.z = (diag * tc_c3).z;
		
		// solve two quadratic equations (x,y)
		eqn.x = dot(d_tc_c3.xyz,tc_c0.xyz);
		eqn.z = dot(d_tc_c0.xyz,tc_c0.xyz);
		
		eqn.y = dot(d_tc_c3.xyz,tc_c1.xyz);
		eqn.w = dot(d_tc_c1.xyz,tc_c1.xyz);
		
		tmp.w = dot(d_tc_c3.xyz,tc_c3.xyz);
		tmp.w = 1.0 / tmp.w;
		eqn = eqn * tmp.w;
		
		// compute bbox
		eqn.zw = (eqn.xy * eqn.xy) - eqn.zw;
		tmp.z = inversesqrt(eqn.z);
		tmp.w = inversesqrt(eqn.w);
		eqn.zw = (eqn * tmp).zw;
		// xy = max,zw = -min
		eqn = (eqn.xyxy * diag) + eqn.zwzw;
		
		// combine bbox at z=-1 and z=1
		bbox = max(bbox,eqn);
		
		// output point sprite
		
		// pointsize
		pointsize = bbox.xy + bbox.zw;
		pointsize = pointsize * viewport;
		gl_PointSize = max(pointsize.x,pointsize.y) / 2;
		
		// transformed vertex position
		bbox = bbox * 0.5;
		gl_Position = vec4(bbox.xy - bbox.zw, 0., 1.);
		
			// compute inverse matrix for fragment program
		
		tc_c0.w = 1 / radius;
		tc_c0.w = tc_c0.w * tc_c0.w;
		
		tc_c0.xyz = me[0].xyz * tc_c0.w;
		tc_c1.xyz = me[1].xyz * tc_c0.w;
		tc_c3.xyz = me[2].xyz * me[2].w;
		
			// output T_e^(-1)
		me_[0].x = dot(view_matrix_inv[0].xyz,tc_c0.xyz);
		me_[0].y = dot(view_matrix_inv[0].xyz,tc_c1.xyz);
		me_[0].z = dot(view_matrix_inv[0].xyz,tc_c3.xyz);
		me_[0].w = 0;
		
		me_[1].x = dot(view_matrix_inv[1].xyz,tc_c0.xyz);
		me_[1].y = dot(view_matrix_inv[1].xyz,tc_c1.xyz);
		me_[1].z = dot(view_matrix_inv[1].xyz,tc_c3.xyz);
		me_[1].w = 0;
		
		me_[2].x = dot(view_matrix_inv[2].xyz,tc_c0.xyz);
		me_[2].y = dot(view_matrix_inv[2].xyz,tc_c1.xyz);
		me_[2].z = dot(view_matrix_inv[2].xyz,tc_c3.xyz);
		me_[2].w = 0;
		
		tmp.xyz = (view_matrix_inv[3].xyz - midpoint);
		me_[3].x = dot(tmp.xyz,tc_c0.xyz);
		me_[3].y = dot(tmp.xyz,tc_c1.xyz);
		me_[3].z = dot(tmp.xyz,tc_c3.xyz);
		me_[3].w = 1;
		
		
		// output delta_p
		tc_c3 = me_[3] * projection_matrix_inv[2].w;
		tc_c3_ = tc_c3;
		
		// output diag/me_c0
		d_tc_c3 = diag * tc_c3;
		diag_ = diag / dot(d_tc_c3.xyw,tc_c3.xyw);
	}
	
}