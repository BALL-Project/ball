// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

//
// Compute RMSD-optimal transformation for two structures
// Coutsalis et al, J. Comput. Chem., 25(15), 1849 (2004)
//

#include <BALL/STRUCTURE/RMSDMinimizer.h>
#include <BALL/STRUCTURE/structureMapper.h>

#include <BALL/FORMAT/PDBFile.h>
#include <BALL/MATHS/matrix44.h>
#include <BALL/MATHS/quaternion.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/STRUCTURE/geometricTransformations.h>

#include <Eigen/Core>
#include <Eigen/Eigenvalues>

using namespace std;

namespace BALL
{

	RMSDMinimizer::TooFewCoordinates::TooFewCoordinates(const char* file, int line, Size size)
		:	Exception::GeneralException(file, line, "RMSDMinimizer::TooFewCoordinates",
																	String("too few coordinates to determine unique transformation: ") + String(size))
	{
	}

	RMSDMinimizer::IncompatibleCoordinateSets::IncompatibleCoordinateSets(const char* file, int line, Size size_a, Size size_b)
		:	Exception::GeneralException(file, line, "RMSDMinimizer::IncompatibleCoordinateSets",
																	String("coordinate sets of differing size are incompatible ( size of A: ") 
																	+ String(size_a) + " / size of B: " + String(size_b))
	{
	}

	RMSDMinimizer::Result RMSDMinimizer::computeTransformation
		(const AtomBijection& ab)
		throw(RMSDMinimizer::IncompatibleCoordinateSets, RMSDMinimizer::TooFewCoordinates)
	{
		RMSDMinimizer::PointVector X(ab.size());
		RMSDMinimizer::PointVector Y(ab.size());
		for (Position i = 0; i < ab.size(); ++i)
		{
			X[i] = ab[i].first->getPosition();
			Y[i] = ab[i].second->getPosition();
		}
		return computeTransformation(X, Y);
	}

	std::pair<Matrix4x4, double> RMSDMinimizer::computeTransformation
		(const RMSDMinimizer::PointVector& A, const RMSDMinimizer::PointVector& B)
		throw(RMSDMinimizer::IncompatibleCoordinateSets, RMSDMinimizer::TooFewCoordinates)
	{
		// Copy the point sets so we can remove the barycenters easily
		PointVector X(A);
		PointVector Y(B);

		// Make sure our two point sets have the same size...
		if (X.size() != Y.size())
		{
			throw IncompatibleCoordinateSets(__FILE__, __LINE__, X.size(), Y.size());
		}
		// ...and contain at least three atoms each.
		if (X.size() < 3)
		{
			throw TooFewCoordinates(__FILE__, __LINE__, X.size());
		}

		
		// Compute the barycenters (geometric center of mass)
		Vector3 barycenter_X;
		Vector3 barycenter_Y;
		for (Position i = 0; i < X.size(); ++i)
		{
			barycenter_X += X[i];
			barycenter_Y += Y[i];
		}
		barycenter_X /= (double)X.size();
		barycenter_Y /= (double)Y.size();

		// Remove the barycenters from the systems so we can get 
		// the optimal rotation wrt the barycenters only.
		for (Position i = 0; i < X.size(); ++i)
		{
			X[i] -= barycenter_X;
			Y[i] -= barycenter_Y;
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
		Eigen::Matrix4d F;
		F << R(0,0) + R(1,1) + R(2,2), R(1,2) - R(2,1),          R(2,0) - R(0,2),          R(0,1) - R(1,0),
		     R(1,2) - R(2,1),          R(0,0) - R(1,1) - R(2,2), R(0,1) + R(1,0),          R(0,2) + R(2,0),
		     R(2,0) - R(0,2),          R(0,1) + R(1,0),         -R(0,0) + R(1,1) - R(2,2), R(1,2) + R(2,1),
		     R(0,1) - R(1,0),          R(0,2) + R(2,0),          R(1,2) + R(2,1),         -R(0,0) - R(1,1) + R(2,2);

		Eigen::SelfAdjointEigenSolver<Eigen::Matrix4d> es(F);

		// Extract the largest eigenvalue, and its corresponding eigenvector.
		double eval_max = es.eigenvalues()[3];

		Quaternion q_max;
		q_max.set(
			es.eigenvectors()(0,3),
			es.eigenvectors()(1,3),
			es.eigenvectors()(2,3),
			es.eigenvectors()(3,3)
		);

		// Compute the complete transformation: move barycenter of A to the origin, 
		// apply quaternion,  move to the barycenter of B. This will (optimally) 
		// map X onto Y.
		Matrix4x4 t1;
		t1.setTranslation(-barycenter_X);
		Matrix4x4 t2;
		q_max.getRotationMatrix(t2);
		Matrix4x4 t3;
		t3.setTranslation(barycenter_Y);
		Matrix4x4 T = t3 * t2 * t1;

		// Compute final RMSD
		double sum_of_squares = 0.0;
		for (Position i = 0; i < X.size(); ++i)
		{
			sum_of_squares += X[i].getSquareLength() + Y[i].getSquareLength();
		}
		double rmsd = sqrt(fabs((sum_of_squares - 2.0 * eval_max)) / double(X.size()));

		return make_pair(T, rmsd);
	}

	double RMSDMinimizer::minimizeRMSD(AtomContainer& a, AtomContainer& b)
		throw(RMSDMinimizer::IncompatibleCoordinateSets, RMSDMinimizer::TooFewCoordinates)
	{
		StructureMapper sm(a, b);
		sm.calculateDefaultBijection();

		Result transform = computeTransformation(sm.getBijection());

		TransformationProcessor tp(transform.first);
		a.apply(tp);

		return transform.second;
	}
} // namespace BALL
