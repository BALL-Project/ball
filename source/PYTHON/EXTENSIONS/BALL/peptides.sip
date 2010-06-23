// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

namespace Peptides
{
%TypeHeaderCode
	#include <BALL/STRUCTURE/peptides.h>

	using namespace BALL;
	using namespace Peptides;
	using namespace std;
%End

	char OneLetterCode(const String& /In/);
	String ThreeLetterCode(char );
	String ThreeLetterToOneLetter(const list<String>& /In/);
	list<String> OneLetterToThreeLetter(const String& /In/);
	String GetSequence(const Protein& /In/);
	
};
