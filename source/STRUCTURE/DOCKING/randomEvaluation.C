#include <BALL/STRUCTURE/DOCKING/randomEvaluation.h>

using namespace::std;
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

vector<ConformationSet::Conformation> RandomEvaluation::operator () (ConformationSet& conformations)
	throw()
{
	Log.info() << "in RandomEvaluation::operator() " << std::endl;
	vector<ConformationSet::Conformation> result;
	for (Size i=0; i<conformations.size(); i++)
	{
		float f = drand48();
		ConformationSet::Conformation c(i,f);
		result.push_back(c);
	}
		
	return result;
}


