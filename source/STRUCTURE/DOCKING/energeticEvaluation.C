#include "energeticEvaluation.h"
#include <stdlib.h>
#include <iostream>
#include "rankedConformations.h"
using namespace::std;


void BALL::Docking::EnergeticEvaluation::operator () (RankedConformations& ranked_conformations) const
	throw()
{
	ranked_conformations = ranked_conformations;
	
	Log.error() << "This is just an interface definition. Check your dynamic binding" << endl;
}


