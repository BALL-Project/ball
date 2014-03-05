class RMSDMinimizer
{
%TypeHeaderCode
	#include <BALL/STRUCTURE/RMSDMinimizer.h>
	using namespace BALL;
	using namespace BALL::Exception;
	using namespace std;
%End

	public:
		%Exception RMSDMinimizer::IncompatibleCoordinateSets(GeneralException) /PyName=RMSDMinimizer_IncompatibleCoordinateSets/
		{
%TypeHeaderCode
#include <BALL/DATATYPE/string.h>
#include <BALL/COMMON/exception.h>
using namespace BALL;
using namespace BALL::Exception;
%End
%RaiseCode
		String detail = 
			(String)(sipExceptionRef.getName()) + " in line " + String(sipExceptionRef.getLine()) + " of " + sipExceptionRef.getFile()
																				 + ": " + sipExceptionRef.getMessage();

    SIP_BLOCK_THREADS
    PyErr_SetString(sipException_RMSDMinimizer_IncompatibleCoordinateSets, detail.c_str());
    SIP_UNBLOCK_THREADS
%End
		};

		%Exception RMSDMinimizer::TooFewCoordinates(GeneralException) /PyName=RMSDMinimizer_TooFewCoordinates/
		{
%TypeHeaderCode
#include <BALL/DATATYPE/string.h>
#include <BALL/COMMON/exception.h>
using namespace BALL;
using namespace BALL::Exception;
%End
%RaiseCode
		String detail = 
			(String)(sipExceptionRef.getName()) + " in line " + String(sipExceptionRef.getLine()) + " of " + sipExceptionRef.getFile()
																				 + ": " + sipExceptionRef.getMessage();

    SIP_BLOCK_THREADS
    PyErr_SetString(sipException_RMSDMinimizer_TooFewCoordinates, detail.c_str());
    SIP_UNBLOCK_THREADS
%End
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
