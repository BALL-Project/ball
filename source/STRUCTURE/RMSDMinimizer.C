// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: RMSDMinimizer.C,v 1.1 2006/01/23 20:45:03 oliver Exp $
//
// Compute RMSD-optimal transformation for two structures
// Coutsalis et al, J. Comput. Chem., 25(15), 1849 (2004)
//
// Author: 
//  Oliver Kohlbacher
//

#include <BALL/STRUCTURE/RMSDMinimizer.h>

#include <BALL/FORMAT/PDBFile.h>
#include <BALL/MATHS/matrix44.h>
#include <BALL/MATHS/quaternion.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/STRUCTURE/geometricTransformations.h>

// We need the GNU scientific Library (GSL) to solve the eigenvalue problem.
#include <gsl/gsl_math.h>
#include <gsl/gsl_eigen.h>

using namespace std;

namespace BALL
{


	void RMSDMinimizer::extractCAlpha(const AtomContainer& A, const AtomContainer& B)
	{
		// Remove old coordinates.
		positions_a_.clear();
		positions_b_.clear();

		// Fill with new C_alpha coordinates
		for (AtomIterator ai = S1.beginAtom(); +ai; ++ai)
		{
			if ((ai->getName() == "CA") && (ai->getElement() == PTE[Element::C]))
			{
				positions_a_.push_back(ai->getPosition());
			}
		}
	
		for (AtomIterator ai = S2.beginAtom(); +ai; ++ai)
		{
			if ((ai->getName() == "CA") && (ai->getElement() == PTE[Element::C]))
			{
				positions_b_.push_back(ai->getPosition());
			}
		}
	}

	void RMSDMinimizer::computeTransformation()
		throw(RMSDMinimizer::IncompatibleCoordinateSets, RMSDMinimizer::TooFewCoordinates)
	{
		// Make sure our two point sets have the same size...
		if (positions_a_.size() != positsions_b_.size())
		{
			throw IncompatibleCoordinateSets(__FILE__, __LINE__, positions_a_.size(), positions_b_.size());
		}
		// ...and contain at least three atoms each.
		if (positions_a_.size() < 3)
		{
			throw TooFewCoordinates(__FILE__, __LINE__, positions_a_.size());
		}

		
		// Compute the barycenters (geometric center of mass)
		Vector3 barycenter_A;
		Vector3 barycenter_B;
		for (Position i = 0; i < positions_a_.size(); ++i)
		{
			barycenter_A += postions_a_[i];
			barycenter_B += postions_b_[i];
		}
		barycenter_A /= (double)positions_a_.size();
		barycenter_B /= (double)positions_b_.size();

		// Remove the barycenters from the systems so we can get 
		// the optimal rotation wrt the barycenters only.
		for (Position i = 0; i < positions_a_.size(); ++i)
		{
			positions_a_[i] -= barycenter_A;
			positions_b_[i] -= barycenter_B;
		}

		// Calculate correlation matrix R
		Matrix4x4 R;
		for (Position i = 0; i < 3; ++i)
		{
			for (Position j = 0; j < 3; ++j)
			{
				R(i,j) = 0.0;
				for (Position k = 0; k < X.size(); ++k)
				{
					R(i,j) += X[k][i] * Y[k][j];
				}
			}
		}
	
		// Compute the residual matrix F (see original paper for details, names should
		// be identical).
		TMatrix4x4<double> F(R(0,0) + R(1,1) + R(2,2), R(1,2) - R(2,1),       R(2,0) - R(0,2),      R(0,1) - R(1,0),
												 R(1,2) - R(2,1),  R(0,0) - R(1,1) - R(2,2), R(0,1) + R(1,0),      R(0,2) + R(2,0),
												 R(2,0) - R(0,2),          R(0,1) + R(1,0),  -R(0,0) + R(1,1) - R(2,2), R(1,2) + R(2,1),
												 R(0,1) - R(1,0),          R(0,2) + R(2,0),       R(1,2) + R(2,1),  -R(0,0) - R(1,1) + R(2,2));

		// Prepare the matrix for the eigenvalue problem.
		gsl_matrix_view m = gsl_matrix_view_array(&F[0], 4, 4);
		gsl_vector* eval = gsl_vector_alloc(4);
		gsl_matrix* evec = gsl_matrix_alloc(4, 4);

		// Solve the eigenvalue problem and sort the matrix
		// w.r.t the eigenvalues
		gsl_eigen_symmv_workspace* w = gsl_eigen_symmv_alloc(4);
		gsl_eigen_symmv(&m.matrix, eval, evec, w);
		gsl_eigen_symmv_free(w);
		gsl_eigen_symmv_sort(eval, evec, GSL_EIGEN_SORT_VAL_DESC);	

		// Extract the eigenvector matrix, the largest eigenvalue,
		// and its corresponding eigenvector.
		TMatrix4x4<double> EV(evec->data);
		double eval_max = eval->data[0];
		TVector4<double> evec_max = EV.getColumn(0);

		// BALL uses a slightly different order of quaternion		
		// components than Coutsias et al., so we reorder:
		// q_max = (q0, _q_) -> (angle, (i,j,k))
		Quaternion q_max; 
		q_max.angle = evec_max[0];
		q_max.i = evec_max[1];
		q_max.j =	evec_max[2];
		q_max.k = evec_max[3];
	
		// Store transformation: move barycenter of A to the origin, apply T,
		// move to the barycenter of B. This will (optimally) map A onto B.
		translation_1_ = -barycenter_A;
		q_max.getRotationMatrix(transformation_);
		translation_2 = barycenter_B;

		// Compute final RMSD
		double sum_of_squares = 0.0;
		for (Position i = 0; i < X.size(); ++i)
		{
			sum_of_squares += X[i].getSquareLength() + Y[i].getSquareLength();
		}
		double rmsd = sqrt(fabs((sum_of_squares - 2.0 * eval_max)) / double(positions_a_.size()));

		return rmsd;
	}

} // namespace BALL
