// $Id: structureMapper.C,v 1.5 1999/12/28 18:17:12 oliver Exp $

#include <BALL/STRUCTURE/structureMapper.h>
#include <BALL/STRUCTURE/geometricProperties.h>
#include <BALL/KERNEL/PSE.h>
#include <BALL/DATATYPE/hashGrid.h>
#include <BALL/MATHS/quaternion.h>


#include <stack>
#include <vector>
#include <map>

using namespace std;

namespace BALL 
{

	/* Default constructor */
	StructureMapper::StructureMapper()
	{
	}

	/* Constructor */
	StructureMapper::StructureMapper(Composite& A, Composite& B)
	{
		set(A, B);
	}

	/** Destructor */
	StructureMapper::~StructureMapper()
	{
		A_ = 0;
		B_ = 0;
	}

	/* Assign the two objects to be mapped */
	void StructureMapper::set(Composite& A, Composite& B)
	{
		A_ = &A;
		B_ = &B;
	}

	/* Calculate the root mean squared deviation */
	float StructureMapper::calculateRMSD()
	{
		// calculate bijection, if it is not already defined
		if (bijection_.size() == 0) {
			calculateDefaultBijection();
		}

		// check whether we have to transform each coordinate first
		// (only in case of calculation of the RMSD of a mapping)
		bool transform = (transformation_ != Matrix4x4(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1));
			
		// for each pair in the bijection array, calculate square deviation for each 
		// coordinate set

		Size		i;
		float		square_deviation = 0;
		Vector3	r;

		if (transform) {
			for (i = 0; i < bijection_.size(); i++) {
				r = transformation_ * bijection_[i].first->getPosition();
				square_deviation += r.getSquareDistance(bijection_[i].second->getPosition());
			}
		} else {
			for (i = 0; i < bijection_.size(); i++) {
				r = bijection_[i].first->getPosition();
				square_deviation += r.getSquareDistance(bijection_[i].second->getPosition());
			}
		}

		// calculate mean square deviation
		square_deviation /= (double)bijection_.size();

		// return RMSD
		return sqrt(square_deviation);
	}

	/* Calculate the transformation to map the first of two isomorphous
			Composite objects onto the second */
	bool StructureMapper::calculateTransformation()
	{
		// check whether both composites are defined
		if ((A_ == 0) || (B_ == 0))
			return false;

		// check for same number of molecular fragments (or residues)
		if (countFragments_(*A_) != countFragments_(*B_))
			return false;

		
		
		return true;
	}

	StructureMapper::AtomBijection& StructureMapper::calculateFragmentBijection
		(const vector<Fragment*>& A, 
		 const vector<Fragment*>& B) 
	{
		AtomBijection&		pair_array = *new AtomBijection;
		AtomPairType			pair;

		AtomIterator	atom_iterator1;
		AtomIterator	atom_iterator2;

		Size	size_A;
		Size	size_B;
		 
		size_A = A.size();
		size_B = B.size();

		Size i;
		Size minimum;

		if (size_A >= size_B) 
		{
			minimum = size_B;
		} else {
			minimum = size_A;
		}

		Fragment *fragment_A;
		Fragment *fragment_B;
					
		for (i = 0; i < minimum ; i++) 
		{ 

			fragment_A = A[i];
			fragment_B = B[i];
		
			// BAUSTELLE: Atomnamen sollten auf Eindeutigkeit geprueft werden!

			//iterate over all atoms of A and compare names with atoms of B

			for (atom_iterator1 = fragment_A->beginAtom(); +atom_iterator1; ++atom_iterator1) 
				for (atom_iterator2 = fragment_B->beginAtom(); +atom_iterator2; ++atom_iterator2) {
					if ((*atom_iterator1).getName() == (*atom_iterator2).getName()) {
						pair.first = &(*atom_iterator1);
						pair.second = &(*atom_iterator2);
						pair_array.push_back(pair);
					}
			}
		}

		return pair_array;
	}


	bool StructureMapper::mapFragments
		(const vector<Fragment*>& A, 
		 const vector<Fragment*>& B, 
		 Matrix4x4* transformation,
		 float upper_bound, float lower_bound)
	{
		StructureMapper::AtomBijection fragment_bijection = calculateFragmentBijection(A, B);

		Size	size = fragment_bijection.size();
		
		// if no bijection could be found, return false
		if (size == 0)
			return false;

		Matrix4x4												tmp_transformation = transformation_;
		StructureMapper::AtomBijection		tmp_bijection = bijection_;

		bijection_ = fragment_bijection;

		// calculate all triangles from the bijection
		Size					i, j, k;
		float					square_distance;
		float					min_rmsd = FLT_MAX;
		float					rmsd;

		for (k = 0; k < size; k++) {
			for (j = 0; j < size; j++) {
				square_distance = fragment_bijection[k].first->getPosition().getSquareDistance(
															fragment_bijection[j].first->getPosition());
				if ((j != k) && (square_distance > (lower_bound * lower_bound)) 
										 && (square_distance < (upper_bound * upper_bound))) {
					for (i = 0; i < size; i++) {
						square_distance = fragment_bijection[k].first->getPosition().getSquareDistance(
															fragment_bijection[i].first->getPosition());
						if ((i != k) && (i != j) && (square_distance > (lower_bound * lower_bound)) 
																		 && (square_distance < (upper_bound * upper_bound))) {
							transformation_ = matchPoints_(fragment_bijection[k].first->getPosition(),
																						 fragment_bijection[j].first->getPosition(),
																						 fragment_bijection[i].first->getPosition(),
																						 fragment_bijection[k].second->getPosition(),
																						 fragment_bijection[j].second->getPosition(),
																						 fragment_bijection[i].second->getPosition());
							rmsd = calculateRMSD();
							if (rmsd < min_rmsd) {
								*transformation = transformation_;
								min_rmsd = rmsd;
							}
						}
					}
				}
			}
		}

		transformation_ = tmp_transformation;
		bijection_ = tmp_bijection;
		
		return true;
	}

	void StructureMapper::calculateDefaultBijection()
	{
	}

	 
	Size StructureMapper::countFragments_(const Composite& composite) const
	{
		Size number_of_mol_fragments = 0;

		Composite::CompositeIterator	it;

		for (it = composite.beginComposite(); +it; ++it){
			if (RTTI<Fragment>::isKindOf(*it));
				number_of_mol_fragments++;
		}

		return number_of_mol_fragments;
	}



	// constructor with the following properties: The transformation maps 
	// (1) the point (vector3) w1 onto the point v1 and  
	// (2) the point w2 onto the ray that starts in v1 and goes through v2
	// (3) the point w3 into the plane generated by v1, v2 and v3

	#define EPSILON 0.00001
	#define EPSILON2 0.00000001

	Matrix4x4 StructureMapper::matchPoints_
			 (const Vector3& w1,
			 const Vector3& w2,
			 const Vector3& w3,
			 const Vector3& v1,
			 const Vector3& v2,
			 const Vector3& v3) 
	{
		// initialize transformation matrix
		Matrix4x4 transformation(1,0,0,-w1.x, 0,1,0,-w1.y, 0,0,1,-w1.z, 0,0,0,1);

		// Compute the translations that map v1 and w1 onto the origin 
		// and apply them to v2,v3 and w2,w3

		Vector3 tw2(w2.x - w1.x, w2.y - w1.y, w2.z - w1.z); 
		Vector3 tw3(w3.x - w1.x, w3.y - w1.y, w3.z - w1.z); 

		Vector3 tv2(v2.x - v1.x, v2.y - v1.y, v2.z - v1.z); 
		Vector3 tv3(v3.x - v1.x, v3.y - v1.y, v3.z - v1.z);

		float dist_v2_v1 = tv2.getSquareLength();
		float dist_w2_w1 = tw2.getSquareLength();
		float dist_w3_w1 = tw3.getSquareLength();
		float dist_v3_v1 = tv3.getSquareLength();

		Vector3		rotation_axis;
		Quaternion	rotation_quat;
		Matrix4x4	rotation;

		// check  (v2 != v1) 
		if (dist_v2_v1 < EPSILON2) {
			// v1 = v2
			
			if (dist_v3_v1 < EPSILON2) {
				// v1 = v2 = v3
				
				
			} else {
				// v1 = v2 != v3
				tv2.swap(tv3);
			}

		} 

		// check  (w2 != w1) 
		if (dist_w2_w1 < EPSILON2) {
			// w1 = w2
			
			if (dist_w3_w1 < EPSILON2) {
				// w1 = w2 = w3
				
				
			} else {
				// w1 = w2 != w3
				tw2.swap(tw3);
			}
		} 

		if ((tv2.getSquareLength() > EPSILON2) && (tw2.getSquareLength() > EPSILON2)) {
			// calculate the rotation axis: orthogonal to tv2 and tw2
			tw2.normalize();
			tv2.normalize();

			rotation_axis = tw2 + tv2;
			
			if (rotation_axis.getSquareLength() < EPSILON){
				// the two axes seem to be antiparallel -
				// invert the second vector
				rotation.setIdentity();
				rotation.m11 = -1.0;
				rotation.m22 = -1.0;
				rotation.m33 = -1.0;
			} else {
				// rotate around the rotation axis
				rotation_quat.set(rotation_axis.x, rotation_axis.y, 
													rotation_axis.z, Constants::PI);


				// Compute the matrix4x4 form of the rotation
				// and apply it to tv3,tw2,tw3

				rotation_quat.getRotationMatrix(rotation);
			}

			tw2 = rotation * tw2;
			tw3 = rotation * tw3;

			transformation = rotation * transformation;
		
			
			if ((tw3.getSquareLength() > EPSILON2) && (tv3.getSquareLength() > EPSILON2)) {

				tw3.normalize();
				tv3.normalize();

				Vector3	axis_w = tv2 % tw3;
				Vector3	axis_v = tv2 % tv3;


				if ((axis_v.getSquareLength() > EPSILON2) && (axis_w.getSquareLength() > EPSILON2)) {
					axis_v.normalize();
					axis_w.normalize();
						 
					rotation_axis = axis_w % axis_v;

					if (rotation_axis.getSquareLength() < EPSILON2) {
						float	scalar_prod = axis_w * axis_v;
						if (scalar_prod < 0.0) {
							rotation_quat.set(tv2.x, tv2.y, tv2.z, Constants::PI);
							rotation_quat.getRotationMatrix(rotation);
						} else {
							rotation.setIdentity();
						}
					} else {

						// Compute the quaternion form of the rotation that maps tw3 onto tv3
						rotation_quat.set(rotation_axis.x, rotation_axis.y,
															rotation_axis.z, acos(axis_w * axis_v));

						// Compute the matrix4x4 form of the rotation
						// and add it to the transformation
						rotation_quat.getRotationMatrix(rotation);
					}

					transformation = rotation * transformation;
				}
			}
		}

		transformation.m14 += v1.x;
		transformation.m24 += v1.y;
		transformation.m34 += v1.z;
		
		return transformation;
	}


	Matrix4x4 StructureMapper::matchPointsOptimally_
			 (const Vector3& w1,
			 const Vector3& w2,
			 const Vector3& w3,
			 const Vector3& v1,
			 const Vector3& v2,
			 const Vector3& v3) 
	{
		// initialize transformation matrix
		Matrix4x4 transformation(1,0,0,-w1.x, 0,1,0,-w1.y, 0,0,1,-w1.z, 0,0,0,1);

		// Compute the translations that map v1 and w1 onto the origin 
		// and apply them to v2,v3 and w2,w3

		Vector3 tw2(w2.x - w1.x, w2.y - w1.y, w2.z - w1.z); 
		Vector3 tw3(w3.x - w1.x, w3.y - w1.y, w3.z - w1.z); 

		Vector3 tv2(v2.x - v1.x, v2.y - v1.y, v2.z - v1.z); 
		Vector3 tv3(v3.x - v1.x, v3.y - v1.y, v3.z - v1.z);

		float dist_v2_v1 = tv2.getSquareLength();
		float dist_w2_w1 = tw2.getSquareLength();
		float dist_w3_w1 = tw3.getSquareLength();
		float dist_v3_v1 = tv3.getSquareLength();

		Vector3		rotation_axis;
		Quaternion	rotation_quat;
		Matrix4x4	rotation;

		// check  (v2 != v1) 
		if (dist_v2_v1 < EPSILON2) {
			// v1 = v2
			
			if (dist_v3_v1 < EPSILON2) {
				// v1 = v2 = v3
				
				
			} else {
				// v1 = v2 != v3
				tv2.swap(tv3);
			}

		} 

		// check  (w2 != w1) 
		if (dist_w2_w1 < EPSILON2) {
			// w1 = w2
			
			if (dist_w3_w1 < EPSILON2) {
				// w1 = w2 = w3
				
				
			} else {
				// w1 = w2 != w3
				tw2.swap(tw3);
			}
		} 

		if ((tv2.getSquareLength() > EPSILON2) && (tw2.getSquareLength() > EPSILON2)) {
			// calculate the rotation axis: orthogonal to tv2 and tw2
			tw2.normalize();
			tv2.normalize();

			rotation_axis = tw2 + tv2;
			
			if (rotation_axis.getSquareLength() < EPSILON){
				// the two axes seem to be antiparallel -
				// invert the second vector
				rotation.setIdentity();
				rotation.m11 = -1.0;
				rotation.m22 = -1.0;
				rotation.m33 = -1.0;
			} else {
				// rotate around the rotation axis
				rotation_quat.set(rotation_axis.x, rotation_axis.y, 
													rotation_axis.z, Constants::PI);


				// Compute the matrix4x4 form of the rotation
				// and apply it to tv3,tw2,tw3

				rotation_quat.getRotationMatrix(rotation);
			}

			tw2 = rotation * tw2;
			tw3 = rotation * tw3;

			transformation = rotation * transformation;
		
			
			if ((tw3.getSquareLength() > EPSILON2) && (tv3.getSquareLength() > EPSILON2)) {

				tw3.normalize();
				tv3.normalize();

				Vector3	axis_w = tv2 % tw3;
				Vector3	axis_v = tv2 % tv3;


				if ((axis_v.getSquareLength() > EPSILON2) && (axis_w.getSquareLength() > EPSILON2)) {
					axis_v.normalize();
					axis_w.normalize();
						 
					rotation_axis = axis_w % axis_v;

					if (rotation_axis.getSquareLength() < EPSILON2) {
						float	scalar_prod = axis_w * axis_v;
						if (scalar_prod < 0.0) {
							rotation_quat.set(tv2.x, tv2.y, tv2.z, Constants::PI);
							rotation_quat.getRotationMatrix(rotation);
						} else {
							rotation.setIdentity();
						}
					} else {

						// Compute the quaternion form of the rotation that maps tw3 onto tv3
						rotation_quat.set(rotation_axis.x, rotation_axis.y,
															rotation_axis.z, acos(axis_w * axis_v));

						// Compute the matrix4x4 form of the rotation
						// and add it to the transformation
						rotation_quat.getRotationMatrix(rotation);
					}
				
					transformation = rotation * transformation;

					// now we jump right into twodimensional space:
					// both triangles are in a plane defined by (0,0,0)/tv2/tv3
					
					// first, generate unit vectors in this system:
					// u1 = tv2/|tv2|
					Vector3	u1 = tv2;

					// the second vector is orthogonal to tv3 and tv2
					Vector3	u2 = axis_v % tv2;
					Vector3	u3 = u1 % u2;
					u2.normalize();
					u3.normalize();
				
					// let's introduce some new vectors:
					// (A,B,C) describing the first triangle (v1,v2,v3)
					// (P,Q,R) describing the second triangle (w1,w2,w3)
					// A, P = (0,0), - in our new basis, too!
					Vector3	P(0,0,0);
					Vector3	B(sqrt(dist_v2_v1), 0.0, 0.0);
					Vector3	Q(sqrt(dist_w2_w1), 0.0, 0.0);
					Vector3	C;
					Vector3	R;

					// now solve a system of linear equations to perform
					// the basis transformations for C/R
					if (!(BALL_REAL_EQUAL(u1.x, 0.0, EPSILON) || BALL_REAL_EQUAL((u2.y - (u2.x * u1.y) / u1.x), 0.0, EPSILON))) {
						C.y = (v3.y - v1.y - ((v3.x -v1.x) * u1.y) / u1.x) / (u2.y - (u2.x * u1.y) / u1.x);
						C.x = ((v3.x - v1.x) - C.y * u2.x) / u1.x;
						C.z = 0.0;

						R.y = (w3.y - w1.y - ((w3.x -w1.x) * u1.y) / u1.x) / (u2.y - (u2.x * u1.y) / u1.x);
						R.x = ((w3.x - w1.x) - C.y * u2.x) / u1.x;
						R.z = 0.0;
					} else {
						if (!(BALL_REAL_EQUAL(u2.x, 0.0, EPSILON) || BALL_REAL_EQUAL((u1.y - (u1.x * u2.y) / u2.x), 0.0, EPSILON))) {
							C.x = (v3.y - v1.y - ((v3.x -v1.x) * u2.y) / u2.x) / (u1.y - (u1.x * u2.y) / u2.x);
							C.y = ((v3.x - v1.x) - C.x * u1.x) / u2.x;
							C.z = 0.0;

							R.x = (w3.y - w1.y - ((w3.x -w1.x) * u2.y) / u2.x) / (u1.y - (u1.x * u2.y) / u2.x);
							R.y = ((w3.x - w1.x) - C.x * u1.x) / u2.x;
							R.z = 0.0;
						}
					}
				
					// OK. that's it we now have a transformed the two 
					// triangles into twodimensional space.
					// We have to find the optimal solution for a transformation 
					// T(x, y, phi) that transforms PQR so that the sum of
					// the squared distances |AP| + |BQ| + |CR| is minimal
					// We use Polak-Ribiere (conjugate gradient).
					// The gradient is calculated analytically.
						
					// start vector in transformation space
					Vector3 v(0, 0, 0);

					// gradient vectors
					Vector3	G(0, 0, 0);
					Vector3	last_G(0, 0, 0);
					

					// direction vector
					Vector3	H(0, 0, 0);
					Vector3	last_H(0, 0, 0);
					
					bool converged = false;
					int max_iterations = 50;
					int iterations = 0;
					float cos_phi;
					float sin_phi;
					float gam;

					// main loop: iterate until convergence reached or 
					// too many iterations encountered
					// distance function is defined as:
					// x^2 + y^2 + (cos(phi) * Bx + x -Qx)^2 + (sin(phi)*Bx + y)^2
					//    + (cos(phi)*Cx - sin(phi)* Cy + x - Rx)^2 
					//		+ (sin*phi)*Cx + cos(phi)*Cy +y -Ry)^2
					while (!converged && (iterations < max_iterations)) {
						sin_phi = sin(v.z);
						cos_phi = cos(v.z);

						float rmsd = sqrt(v.x * v.x + (v.y * v.y) + (cos_phi * B.x + v.y) * (cos_phi * B.x + v.y) 
															+ (cos_phi * C.x - sin_phi * C.y + v.x -R.x) * (cos_phi * C.x - sin_phi * C.y + v.x -R.x)
												 + (sin_phi * C.x + cos_phi * C.y + v.y - R.y) *(sin_phi * C.x + cos_phi * C.y + v.y - R.y));
						
						G.x = 6 * v.x + 2 * (cos_phi * B.x - Q.x + cos_phi * C.x - sin_phi * C.y - R.x);
						G.y = 6 * v.y + 2 * (sin_phi * B.x + sin_phi * C.x + cos_phi * C.y - R.y);
						G.z = 2 * (-sin_phi * B.x * v.x + sin_phi * B.x * v.y - v.x * sin_phi * C.x
											- v.x * cos_phi * C.y + R.x * sin_phi * C.x + R.x * cos_phi * C.y
											+ v.y * cos_phi * C.x - v.y * sin_phi * C.y - R.y * cos_phi * C.x + R.y * sin_phi * C.y);
						
						// calculate the new scaling constant (according to Polak-Ribiere)
						if (iterations == 0) {
							gam = G * G;
						} else {
							gam = (G * G) / (last_G * last_G);
						}
																						

						// now calculate the new direction vector in transformation space
						H = -G + gam * last_H;

						H.normalize();
						// take a step in the given direction
						v = v + H * rmsd/4;
						
						// remember the last gradient
						last_G = G;
						
						
						cout << "GRAD: " << G << "   RMS: " << rmsd << endl;
						
						converged = (G.getLength() < 1e-4);
						iterations ++;
					}

					if (converged) {
						// transform P,Q,R accodring to the new transformation
						// in two dimensional space
						cos_phi = cos(v.z);
						sin_phi = sin(v.z);
						Matrix4x4 T(cos_phi, -sin_phi, 0,  v.x, 
														sin_phi, cos_phi,  0,  v.y,
														0,       0,        1,  0,
														0,			 0,			   0,  1);

						P = T * P;
						Q = T * Q;
						R = T * R;
						
						// revert P/Q/R back to threedimensional space
					}
				}
			}
		}

		transformation.m14 += v1.x;
		transformation.m24 += v1.y;
		transformation.m34 += v1.z;
		
		return transformation;
	}

	vector<vector<Fragment*> >& StructureMapper::searchPattern_
			 (vector<Fragment*> &pattern,
				Composite &composite,
				float max_rmsd,
				float max_center_tolerance,
				float upper_bound,
				float lower_bound)
	{
		// determine number of fragments in the pattern
		Size no_of_frag = pattern.size();

		// calculate the distances of the centers of the pattern fragments 
		// and store them in the array dist_pattern

		vector<float>         pattern_distances(no_of_frag*no_of_frag);
		vector<Vector3>  pattern_centers(no_of_frag);

		Size i, j;
		GeometricCenterProcessor geo_center;

		for (i = 0; i < no_of_frag ; i++) 
		{
			 pattern[i]->apply(geo_center);
			 pattern_centers[i] = geo_center.getCenter();  
		}

	 float                     distance;

		for (i = 0; i < no_of_frag ; i++) 
			for (j = i; j < no_of_frag ; j++)
			{
				distance = pattern_centers[i].getDistance(pattern_centers[j]);
				pattern_distances[i * no_of_frag + j] = distance;
				pattern_distances[j * no_of_frag + i] = distance;
		}

		pattern_centers.clear();

		// determine the molecular fragments in composite 
		// and store them in an array

		Composite::SubcompositeIterator     composite_it;
		vector<Fragment*> composite_fragments;

		for (composite_it = composite.beginSubcomposite(); 
				 composite_it != composite.endSubcomposite(); ++composite_it) {
					 if (RTTI<Fragment>::isKindOf(*composite_it)) {
						composite_fragments.push_back(RTTI<Fragment>::castTo(*composite_it));
					 }
		}

		// determine the number of fragments of the composite
		Size   no_of_comp_frag;
		
		no_of_comp_frag = composite_fragments.size();

		// calculate the centers of the composite fragments

		vector<Vector3>   composite_centers(no_of_comp_frag);

		for ( i = 1; i < no_of_comp_frag ; i++) {
				composite_fragments[i]->apply(geo_center);
				composite_centers[i] = geo_center.getCenter();    
		}
		
		// calculate the distances of the centers of composite fragments

		vector<float>    comp_frag_dist(no_of_comp_frag*no_of_comp_frag);
	 
		for (i = 0; i < no_of_comp_frag; i++) 
		 for (j = i; j < no_of_comp_frag ; j++) {
			 distance = composite_centers[i].getDistance(composite_centers[j]);
			 comp_frag_dist[i*no_of_comp_frag + j] = distance;
			 comp_frag_dist[j*no_of_comp_frag + i] = distance;
		} 
	 
		composite_centers.clear();  

		// calculate an array of arrays that contains the indices of potential matching fragments

		vector<vector<Size> >											indices_CF(no_of_frag);
		vector<vector<Fragment*> >*								result;
		bool																			ready = false;
		int																				counter;
		
		result = new vector<vector<Fragment*> >;
	 
		for (i = 0; i < no_of_frag  && ready == false ; i++) 
		{
			for (j = 0, counter = 0; j < no_of_comp_frag ; ++j) 
			{
				if (composite_fragments[j]->getName() == pattern[i]->getName()) 
				{
					counter++;
					indices_CF[i].push_back(j);
				}
			}
			if (counter == 0) 
			{	
				ready = true;
			}
		} 

		// search the pattern using the array of indices

		vector<Fragment*>	potential_pattern(no_of_frag);
		vector<Size>			indices_of_pot_pattern(no_of_frag);
		Matrix4x4					T;
		bool							distances_fit;
		Size							k;
		stack<Size>				index_stack;

		i = 0;
		j = 0;

	 

		while (!ready) 
		{
			indices_of_pot_pattern[i] = indices_CF[i][j];
			distances_fit = true;

			for (k = 0; k < i && distances_fit == true; k++ ) 
			{
				distance = pattern_distances[i*no_of_frag + k] - 
								comp_frag_dist[indices_of_pot_pattern[i]*no_of_comp_frag + indices_of_pot_pattern[k]];
				if (distance < -max_center_tolerance || distance > max_center_tolerance)
				{
					distances_fit = false; 
				}
			}

			if (distances_fit == true) 
			{
				index_stack.push(j);
				i++;
				if (i == no_of_frag) 
				{
					for (k = 0; k < no_of_frag ; k++)
					{
						potential_pattern.push_back(composite_fragments[indices_of_pot_pattern[k]]);
						
						mapFragments(potential_pattern,pattern,&T,upper_bound,lower_bound);
						if (rmsd_ <= max_rmsd) 
						{
							result->push_back(potential_pattern);
							potential_pattern.clear();
						} else {
							j = 0;
						}
					}
				} else {
					j++;
					if (j == indices_CF[i].size()) 
					{
						i--;
						// BAUSTELLE: change top + pop to pop
						j = (Size)index_stack.top() + 1;
						index_stack.pop();
					}
				}

				if ((i == 0) && (j == indices_CF[0].size())) 
				{
					ready = true;
				}
			}
		}

		return *result;
	}

	Matrix4x4 StructureMapper::mapProteins_
			 (Protein& P1, Protein& P2,
         map<String,int>& type_map, 
				 int&         no_matched_ca,
				 float&				rmsd,
				 float        upper_bound,
				 float        lower_bound,
				 float        tolerance)
	{
		// calculate bounding box of protein P1
		BoundingBoxProcessor   box_processor;

		P1.apply(box_processor);
		
		// insert positions of CA-atoms of P1 into a three-dimensional hashgrid 
		// and in the array ca_atoms

		Vector3			upper_bound_vector(upper_bound, upper_bound, upper_bound);

		HashGrid3<Position>	grid_P1(box_processor.getLower() - upper_bound_vector,
																box_processor.getUpper() - box_processor.getLower() + (float)2.0 * upper_bound_vector,
																upper_bound);

		AtomIterator			atom_it;
		vector<Vector3>   ca_atoms_P1;
		vector<Position>	index_ca_P1;
		Position          no_ca_P1 = 0;

		for (atom_it = P1.beginAtom(); +atom_it; ++atom_it) 
		{
			if (((*atom_it).getElement() == PSE[Element::C]) 
					&& ((*atom_it).getName().trim() == "CA")) 
			{
				 grid_P1.insert((*atom_it).getPosition(), no_ca_P1);
				 no_ca_P1++;
				 ca_atoms_P1.push_back((*atom_it).getPosition());
                                 index_ca_P1.push_back(type_map[(*atom_it).getFragment()->getName()]);
			}
		}
	 

		// calculate bounding box of protein P2
		P2.apply(box_processor);

		// insert positions of CA-atoms of P2 into the hashgrid grid_P2

		HashGrid3<Position>	grid_P2(box_processor.getLower() - upper_bound_vector,
																box_processor.getUpper() - box_processor.getLower() + (float)2.0 * upper_bound_vector,
																upper_bound);
		Vector3							tolerance_vector(2 * tolerance, 2 * tolerance, 2 * tolerance);
		HashGrid3<Position> fine_grid_P2(box_processor.getLower() - tolerance_vector,
																		 box_processor.getUpper() - box_processor.getLower() + tolerance_vector, 2 * tolerance);

		vector<Vector3>		ca_atoms_P2;
		vector<Position>	index_ca_P2;
		Size							no_ca_P2 = 0;

		for ( atom_it = P2.beginAtom(); +atom_it; ++atom_it)
			if (((*atom_it).getElement() == PSE[Element::C]) && ((*atom_it).getName().trim() == "CA")) 
			{
				grid_P2.insert((*atom_it).getPosition(),no_ca_P2);
				fine_grid_P2.insert((*atom_it).getPosition(),no_ca_P2);
				
				no_ca_P2++;
				ca_atoms_P2.push_back((*atom_it).getPosition());
				index_ca_P2.push_back(type_map[(*atom_it).getFragment()->getName()]);
			}


	 // calculate triangles between CA-atoms of P2 whose edge length are larger than lower_bound
	 // and smaller than upperbound and store them in a hashgrid with respect to their edge length
	 
	 Vector3		upper(upper_bound + 1, upper_bound + 1, upper_bound + 1);
	 Vector3    lower(lower_bound - 1, lower_bound - 1, lower_bound - 1);

	 HashGrid3< TVector3<Position> > triangles_P2(lower, upper-lower, tolerance);
		
	 HashGrid3<Position>::BoxIterator         b_it1;
	 HashGridBox3<Position>::BoxIterator      b_it2, b_it3;
	 HashGridBox3<Position>::DataIterator     d_it1, d_it2, d_it3;
	 TVector3<Position>												index_vector;
	 Vector3                                  distance_vector; 
	 float                                    square_upper = upper_bound * upper_bound;
	 float                                    square_lower = lower_bound * lower_bound;
	 float                                    distance1, distance2, distance3;



	 for (b_it1 = grid_P2.beginBox(); +b_it1 ; ++b_it1) 
		for (d_it1 = (*b_it1).beginData(); +d_it1 ; ++d_it1) {
			for (b_it2 = (*b_it1).beginBox(); +b_it2; ++b_it2) 
			 for (d_it2 = (*b_it2).beginData(); +d_it2 ; ++d_it2) {
				 if ((*d_it2) != (*d_it1)) {
					 distance1 = ca_atoms_P2[(*d_it1)].getSquareDistance(ca_atoms_P2[(*d_it2)]);
					 if ( distance1 < square_upper && distance1 > square_lower) {
						 for (b_it3 = (*b_it1).beginBox(); +b_it3; ++b_it3)
							for (d_it3 = (*b_it3).beginData(); +d_it3; ++d_it3) {
							 if ((*d_it3) != (*d_it1) && (*d_it3) != (*d_it2)) {
								 distance2 = ca_atoms_P2[(*d_it1)].getSquareDistance(ca_atoms_P2[(*d_it3)]);
								 if (distance2 < square_upper && distance2 > square_lower) {
										distance3 = ca_atoms_P2[(*d_it2)].getSquareDistance(ca_atoms_P2[(*d_it3)]);
										if (distance3 < square_upper && distance3 > square_lower) {
											distance1 = sqrt(distance1);
											distance2 = sqrt(distance2);
											distance3 = sqrt(distance3);
											distance_vector.set(distance1,distance2,distance3);
											index_vector.set((*d_it1),(*d_it2),(*d_it3));
											triangles_P2.insert(distance_vector,index_vector);
										}
								 }
							 }
						 }
					 }
				 }
			}
		}

	 // calculate all triangles between CA-Atoms of P1 and 
	 // search similar triangles between CA-Atoms of P2 stored in triangles_P2

		HashGridBox3<TVector3<Position> >::BoxIterator		b_it4;               
		HashGridBox3<TVector3<Position> >::DataIterator		d_it4;
		HashGridBox3<TVector3<Position> >*								box;
		HashGridBox3<Position>*														ibox;
		HashGridBox3<Position>::BoxIterator								ibox_it;                             
		HashGridBox3<Position>::DataIterator							id_it;

		Matrix4x4	T;
		Matrix4x4 T_best;
		Vector3		v;
		bool      matched;
		float     square_tolerance;
		int       matched_ca;
		 
		square_tolerance = 4 * tolerance * tolerance;
		no_matched_ca = 0;
		float squared_atom_dist;
		float current_rmsd;

		for (b_it1 = grid_P1.beginBox(); +b_it1 ; ++b_it1) 
			for (d_it1 = (*b_it1).beginData(); +d_it1 ; ++d_it1) {
				for (b_it2 = (*b_it1).beginBox(); +b_it2; ++b_it2) 
				 for (d_it2 = (*b_it2).beginData(); +d_it2 ; ++d_it2) {
					 if ((*d_it2) != (*d_it1)) {
						 distance1 = ca_atoms_P1[(*d_it1)].getSquareDistance(ca_atoms_P1[(*d_it2)]);
						 if ( distance1 < square_upper && distance1 > square_lower) {
							 distance1 = sqrt(distance1);
							 for (b_it3 = (*b_it1).beginBox(); +b_it3; ++b_it3)
								for (d_it3 = (*b_it3).beginData(); +d_it3; ++d_it3) {
								 if ((*d_it3) != (*d_it1) && (*d_it3) != (*d_it2)) {
									 distance2 = ca_atoms_P1[(*d_it1)].getSquareDistance(ca_atoms_P1[(*d_it3)]);
									 if (distance2 < square_upper && distance2 > square_lower) {
											distance2 = sqrt(distance2);
											distance3 = ca_atoms_P1[(*d_it2)].getSquareDistance(ca_atoms_P1[(*d_it3)]);
											if (distance3 < square_upper && distance3 > square_lower) {
												distance3 = sqrt(distance3);
												distance_vector.set(distance1,distance2,distance3);
												index_vector.set((*d_it1),(*d_it2),(*d_it3));

												box = triangles_P2.getBox(distance_vector);
												
												for (b_it4 = box->beginBox(); +b_it4 ; ++b_it4 ) 
													for (d_it4 = (*b_it4).beginData(); +d_it4 ; ++d_it4 ) 
														if ( index_ca_P1[(*d_it1)] == index_ca_P2[(*d_it4).x] &&
																 index_ca_P1[(*d_it2)] == index_ca_P2[(*d_it4).y] &&
																 index_ca_P1[(*d_it3)] == index_ca_P2[(*d_it4).z] ) {
														T = matchPoints_(ca_atoms_P1[(*d_it1)], ca_atoms_P1[(*d_it2)], ca_atoms_P1[(*d_it3)],
																						ca_atoms_P2[(*d_it4).x], ca_atoms_P2[(*d_it4).y], ca_atoms_P2[(*d_it4).z]);

														matched_ca = 0;
														current_rmsd = 0;
														squared_atom_dist = 0;

														for (Size i = 0; i < no_ca_P1; i++) 
														{
															v = T * ca_atoms_P1[i];
															ibox = fine_grid_P2.getBox(v);
														
															if (ibox != 0) 
															{
																matched = false;

																for (ibox_it = ibox->beginBox(); +ibox_it && !matched; ++ibox_it) 
																	for (id_it = (*ibox_it).beginData(); +id_it && !matched; ++id_it){
																			squared_atom_dist = v.getSquareDistance(ca_atoms_P2[(*id_it)]);
																			if (squared_atom_dist  <= square_tolerance) {
																				matched_ca++;
																				matched = true;
																				current_rmsd += squared_atom_dist;
																			}
																	}
																}
															}

														
														if (matched_ca >= no_matched_ca) {
															current_rmsd = sqrt(current_rmsd / matched_ca);
															if (matched_ca == no_matched_ca){
																if (current_rmsd < rmsd) {
																		T_best = T;
																		rmsd = current_rmsd;
																}
															} else {
																T_best = T;
																rmsd = current_rmsd;
															}
															no_matched_ca = matched_ca;
														}
													}
											}
											
									 }
								 }
								}
						 }
					 }
				 }
			}

		return T_best;

	}

} // namespace BALL

#undef EPSILON
#undef EPSILON2 
