class RMSDMinimizer
{
%TypeHeaderCode
	#include <BALL/STRUCTURE/RMSDMinimizer.h>
	using namespace BALL;
	using namespace BALL::Exception;
	using namespace std;
%End

	public:
		class IncompatibleCoordinateSets
			: GeneralException
		{
			public:
				IncompatibleCoordinateSets(const char*, int, Size, Size);
		};

		class TooFewCoordinates
			: GeneralException
		{
			public:
				TooFewCoordinates(const char*, int, Size);
		};

		typedef vector<Vector3> PointVector;
		typedef pair<Matrix4x4, double> Result;

		static Result computeTransformation(const AtomBijection& ab)
			throw(RMSDMinimizer::IncompatibleCoordinateSets, RMSDMinimizer::TooFewCoordinates);

		static Result computeTransformation(const PointVector& X, const PointVector& Y)
			throw(RMSDMinimizer::IncompatibleCoordinateSets, RMSDMinimizer::TooFewCoordinates);

		static double minimizeRMSD(AtomContainer& a, AtomContainer& b)
			throw(RMSDMinimizer::IncompatibleCoordinateSets, RMSDMinimizer::TooFewCoordinates);

};
