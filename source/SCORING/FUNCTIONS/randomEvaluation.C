#include <BALL/SCORING/FUNCTIONS/randomEvaluation.h>

//using namespace::std;
using namespace BALL;

RandomEvaluation::RandomEvaluation()
	throw()
	: EnergeticEvaluation()
{
}

RandomEvaluation::~RandomEvaluation()
	throw()
{
}

vector < ConformationSet::Conformation > RandomEvaluation::operator () (ConformationSet& conformations)
	throw()
{
	Log.info() << "in RandomEvaluation::operator() " << std::endl;
	vector < ConformationSet::Conformation > result;
	for (Size i = 0; i < conformations.size(); i++)
	{
	#ifdef BALL_COMPILER_MSVC
		float f = rand();
	#else
		float f = drand48();
	#endif
		ConformationSet::Conformation c(i, f);
		result.push_back(c);
	}

	return result;
}
