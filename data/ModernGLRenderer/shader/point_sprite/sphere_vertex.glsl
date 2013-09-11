#version 120

/**
 * Mathematical background will be found in Sigg et al. / GPU-Based Ray Casting of Quadratic Surfaces
 **/

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

//Midpoint of the impostor/sphere in world space.
attribute vec3 midpoint;

//Color of the impostor.
attribute vec4 color;

//Radius of the sphere.
attribute float radius;

//--------------------------------------------- Outputs --------------------------------------------
//Midpoint of the impostor in view space.
varying vec3 mid_v;

//Color of the impostor.
varying vec4 col;

//Radius of the impostor.
varying float rad;

//Diagonal of the normalized diagonal form of the quadric.
varying vec4 diag_;

varying mat4 me;
varying vec4 mc_c2_;

varying float early_discard; //1 for discard, 0 else

const vec4 diag = vec4(1, 1, 1, -1);

//-------------------------------------- Function Definition ---------------------------------------
//homogeneous dot product
float dph(vec3 a,vec4 b)
{
	return dot(a.xyz,b.xyz) + b.w;
}

int test_early_discard(vec3 midpoint, float radius);

//------------------------------------------ Main Program ------------------------------------------
void setup()
{
	//check if we can discard fragments which are beyond a clipping plane
	early_discard = test_early_discard(midpoint, radius);
	if(clipable != 0 && early_discard  == 1) {
		gl_PointSize = 0;
		
	} else {
		early_discard = 0;
		
		mid_v = vec3(view_matrix * vec4(midpoint, 1.0));
		col = color;
		rad = radius;
	
		//orthographic case
		if(projection_matrix[3].w == 1) {
			float dist = length(mid_v);
			float scale= 5;
			gl_PointSize = 0.5*radius*(scale/dist)* min(viewport.x, viewport.y);
			gl_Position = projection_matrix * view_matrix * vec4(midpoint, 1.0);
			gl_Position.z = 0; //avoide near plane clipping (will be overwritten in fragment shader)
		
		//perspective case
		} else {

			//mat4 mvp = transpose(projection_matrix * view_matrix);
			mat4 mvp = view_proj_matrix_transp;

			vec2 center_pos; // center position of the sphere

			vec2 border_pos; // vertical and horizontal border positions

			vec2 box_size; // x = width, y = height

			//screen planes in parameter space
			mat4 tc = mvp * radius;
			tc[0][3] = dph(midpoint, mvp[0]);
			tc[1][3] = dph(midpoint, mvp[1]);
			tc[3][3] = dph(midpoint, mvp[3]);

			//solve two quadratic equations (x,y)
			//solutions are center positions in clip coordinates
			center_pos.x = dot(diag * tc[3],tc[0]);
			border_pos.x = dot(diag * tc[0],tc[0]);

			//solutions are border positions
			center_pos.y = dot(diag * tc[3],tc[1]);
			border_pos.y = dot(diag * tc[1],tc[1]);

			float w =  1.0 / dot(diag * tc[3],tc[3]);

			border_pos = border_pos * w;
			center_pos = center_pos * w;

			//radius in window coordinates
			border_pos = sqrt((center_pos * center_pos) - border_pos);

			//transformed vertex position
			// z-coordinate will be overwitten in fragment shader
			gl_Position = vec4(center_pos, 0., 1.);

			//pointsize
			box_size.xy = border_pos * viewport ;

			//set half of the larger value (width, height) to point size radius
			gl_PointSize = max(box_size.x, box_size.y);

			//output T_e^(-1)
			me = view_matrix_inv / radius;

			//output delta_p
			me[3] = vec4((view_matrix_inv[3].xyz - midpoint) / radius, 1.0);

			mc_c2_ = me[3] * projection_matrix_inv[2].w;

			//output diag/a
			diag_ = diag / dot(diag * mc_c2_, mc_c2_);
		}
	}
}