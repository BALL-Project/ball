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

vector<ConformationSet::Conformation> EnergeticEvaluation::operator () (const ConformationSet& conformations)
	throw()
{
	return conformations.getScoring();
}


