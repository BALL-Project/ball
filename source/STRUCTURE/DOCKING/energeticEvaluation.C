#include <BALL/STRUCTURE/DOCKING/energeticEvaluation.h>

using namespace::std;
using namespace BALL;

EnergeticEvaluation::EnergeticEvaluation()
	throw()
{
}

EnergeticEvaluation::~EnergeticEvaluation()
	throw()
{
}

vector<ConformationSet::Conformation> EnergeticEvaluation::operator () (ConformationSet& conformations)
	throw()
{
	Log.info() << "in EnergeticEvaluation::operator() " << std::endl;
	return conformations.getScoring();
}


